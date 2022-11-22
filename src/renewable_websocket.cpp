#include "binapi/renewable_websocket.h"

using namespace binapi::ws;

renewable_websocket::renewable_websocket(boost::asio::io_context& ioc, boost::asio::io_context::strand& strand, binapi::ws::websockets& websocket, boost::posix_time::time_duration web_socket_timeout, boost::posix_time::time_duration web_socket_channel_renew) :
	_ioc(ioc),
	_strand(strand),
	_channel_renew_timer(ioc),
    _websockets(websocket),
	_web_socket_timeout(web_socket_timeout),
	_web_socket_channel_renew(web_socket_channel_renew),
	_active_channel(nullptr),
	_secondary_channel(nullptr)
{
}

void renewable_websocket::create_channel(async_channel_creation_callback callback)
{
    subscribe_channel(boost::asio::bind_executor(_strand, [this, callback](binapi::ws::websockets::handle handle)
        {
            _channel_renew_timer.expires_from_now(_web_socket_channel_renew);
            _channel_renew_timer.async_wait(boost::asio::bind_executor(_strand, std::bind(&renewable_websocket::deal_channel_renew_timer_event, this, std::placeholders::_1)));

            callback(handle);
        }));
}

void renewable_websocket::deal_channel_renew_timer_event(boost::system::error_code ec)
{
    if (ec)
    {
        return;
    }

    create_channel(boost::asio::bind_executor(_strand,[this](binapi::ws::websockets::handle handle)
        {
            _secondary_channel = handle;
        }));
}

/*virtual*/ void renewable_websocket::unsubscribe_channel(binapi::ws::websockets::handle handle)
{
    _websockets.async_unsubscribe(handle);
}

void renewable_websocket::internal_switch_to_secondary_channel()
{
    if (!_secondary_channel)
    {
        return;
    }

    unsubscribe_channel(_active_channel);
    _active_channel = _secondary_channel;
    _secondary_channel = nullptr;
}

/*virtual*/ void renewable_websocket::start()
{
    create_channel(boost::asio::bind_executor(_strand, [this](binapi::ws::websockets::handle handle)
        {
            _active_channel = handle;
        }));
}

/*virtual*/ void renewable_websocket::stop()
{
    const auto unsubscribe_channel = [this](auto& handle)
    {
        if (handle)
        {
            this->unsubscribe_channel(handle);
            handle = nullptr;
        }
    };

    unsubscribe_channel(_secondary_channel);
    unsubscribe_channel(_active_channel);
}