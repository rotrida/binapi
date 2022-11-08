#include "binapi/renewable_websocket.h"

using namespace binapi::ws;

renewable_websocket::renewable_websocket(boost::asio::io_context& ioc, binapi::ws::websockets& websocket, boost::posix_time::time_duration web_socket_timeout, boost::posix_time::time_duration web_socket_channel_renew) :
	_ioc(ioc),
	_strand(ioc),
	_channel_renew_timer(ioc),
    _websockets(websocket),
	_web_socket_timeout(web_socket_timeout),
	_web_socket_channel_renew(web_socket_channel_renew),
	_active_channel(nullptr),
	_secondary_channel(nullptr)
{
}

void renewable_websocket::create_channel(binapi::ws::websockets::handle& handle)
{
    handle = subscribe_channel();

	_channel_renew_timer.expires_from_now(_web_socket_channel_renew);
	_channel_renew_timer.async_wait(boost::asio::bind_executor(_strand, std::bind(&renewable_websocket::deal_channel_renew_timer_event, this, std::placeholders::_1)));
}

void renewable_websocket::deal_channel_renew_timer_event(boost::system::error_code ec)
{
    if (ec)
    {
        return;
    }

    create_channel(_secondary_channel);
}

void renewable_websocket::internal_switch_to_secondary_channel()
{
    if (!_secondary_channel)
    {
        return;
    }

    _websockets.unsubscribe(_active_channel);
    _active_channel = _secondary_channel;
    _secondary_channel = nullptr;
}

void renewable_websocket::start()
{
    create_channel(_active_channel);
}

void renewable_websocket::stop()
{
    const auto unsubscribe_channel = [&](auto& handle)
    {
        if (handle)
        {
            _websockets.async_unsubscribe(handle);
            handle = nullptr;
        }
    };

    unsubscribe_channel(_secondary_channel);
    unsubscribe_channel(_active_channel);
}