#include "binapi/renewable_websocket.h"

using namespace binapi::ws;

renewable_websocket::renewable_websocket(boost::asio::io_context& ioc, binapi::ws::websockets& websocket, boost::posix_time::time_duration web_socket_timeout, boost::posix_time::time_duration web_socket_channel_renew, log_callback log_callback) :
	_ioc(ioc),
	_strand_ptr(std::make_shared<boost::asio::io_context::strand>(ioc)),
	_channel_renew_timer(ioc),
    _websockets(websocket),
	_web_socket_timeout(web_socket_timeout),
	_web_socket_channel_renew(web_socket_channel_renew),
	_active_channel(nullptr),
	_secondary_channel(nullptr),
    _log_callback(log_callback),
    _active_channel_last_attempt_connection(boost::gregorian::date(1900, 01, 01)),
    _secondary_channel_last_attempt_connection(boost::gregorian::date(1900, 01, 01)),
    _reconnection_delay(boost::posix_time::seconds(5)),
    _stopped(false)
{
}

void renewable_websocket::create_channel(async_channel_creation_callback callback, const bool primary)
{
    auto & connection_timer_ptr = primary ? _active_channel_connection_attempt_timer_ptr : _secondary_channel_connection_attempt_timer_ptr;
    auto & last_attempt_connection = primary ? _active_channel_last_attempt_connection : _secondary_channel_last_attempt_connection;

    if(connection_timer_ptr)
    {
        _log_callback("Connection timer already set");
        return;
    }

    auto subscribe_result = [this, me_ptr=shared_from_this(), callback](binapi::ws::websockets::handle handle) 
    {
        _strand_ptr->wrap([this, me_ptr, callback](binapi::ws::websockets::handle handle)
        {
            _channel_renew_timer.expires_from_now(_web_socket_channel_renew);
            _channel_renew_timer.async_wait(boost::asio::bind_executor(*_strand_ptr, std::bind(&renewable_websocket::deal_channel_renew_timer_event, this, std::placeholders::_1)));

            callback(handle);    
        });
    };

    const auto now = boost::posix_time::second_clock::universal_time();
    const auto next_reconnection_attempt = last_attempt_connection + _reconnection_delay;
    last_attempt_connection = now;

    if(last_attempt_connection + _reconnection_delay > now)
    {
        _log_callback("Too soon to reconnect. Waiting the right time");

        connection_timer_ptr = std::make_shared<boost::asio::deadline_timer>(_ioc);
        connection_timer_ptr->expires_at(next_reconnection_attempt);
        connection_timer_ptr->async_wait(_strand_ptr->wrap([this, me_ptr=shared_from_this(), subscribe_result, primary](boost::system::error_code ec)
        {
            auto & connection_timer_ptr = primary ? _active_channel_connection_attempt_timer_ptr : _secondary_channel_connection_attempt_timer_ptr;
            connection_timer_ptr.reset();

            _log_callback("Delayied reconnect.");
            subscribe_channel(subscribe_result);
        }));

        return;
    }

    _log_callback("Connecting.");
    subscribe_channel(subscribe_result);
}

void renewable_websocket::deal_channel_renew_timer_event(boost::system::error_code ec)
{
    if (ec)
    {
        return;
    }

    create_channel([this](binapi::ws::websockets::handle handle)
        {
            boost::asio::dispatch(*_strand_ptr, [this, me_ptr=shared_from_this(), handle]()
                {
                    _secondary_channel = handle;    
                });
        }, 
        false);
}

/*virtual*/ void renewable_websocket::unsubscribe_channel(binapi::ws::websockets::handle handle, binapi::ws::websockets::async_stop_callback callback)
{
    _websockets.async_unsubscribe(handle, callback);
}

void renewable_websocket::internal_switch_to_secondary_channel()
{
    if (!_secondary_channel)
    {
        return;
    }

    unsubscribe_channel(_active_channel, []() {});
    _active_channel = _secondary_channel;
    _secondary_channel = nullptr;
}

/*virtual*/ void renewable_websocket::start()
{
    create_channel([this](binapi::ws::websockets::handle handle)
        {
            boost::asio::dispatch(*_strand_ptr, [this, me_ptr=shared_from_this(), handle]() 
                {
                    _active_channel = handle;    
                });
        },
        true);
}

/*virtual*/ void renewable_websocket::stop(binapi::ws::websockets::async_stop_callback callback)
{
    _channel_renew_timer.cancel();
    _stopped = true;

    auto counter_ptr = std::make_shared<int>(0);

    const auto add_if_exists = [&counter_ptr](auto& handle)
    {
        if (handle)
            ++(*counter_ptr);
    };

    const auto unsubscribe_channel = [this, callback, counter_ptr](auto& handle)
    {
        if (handle)
        {
            this->unsubscribe_channel(handle, [callback, counter_ptr]()
            {
                if (--(*counter_ptr) == 0)
                {
                    callback();
                }
            });

            handle = nullptr;
        }
    };

    add_if_exists(_secondary_channel);
    add_if_exists(_active_channel);

    if (*counter_ptr == 0)
    {
        callback();
        return;
    }

    unsubscribe_channel(_secondary_channel);
    unsubscribe_channel(_active_channel);
}