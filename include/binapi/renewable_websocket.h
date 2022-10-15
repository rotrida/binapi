#pragma once

#include <boost/asio.hpp>
#include "binapi/websocket.hpp"

class renewable_websocket
{
protected:

	boost::asio::io_context& _ioc;
	boost::asio::io_context::strand _strand;
	boost::asio::deadline_timer _channel_renew_timer;
	binapi::ws::websockets& _websocket;
	boost::posix_time::time_duration _web_socket_timeout;
	boost::posix_time::time_duration _web_socket_channel_renew;
	binapi::ws::websockets::handle _active_channel;
	binapi::ws::websockets::handle _secondary_channel;

	void create_channel(binapi::ws::websockets::handle& handle);
	virtual binapi::ws::websockets::handle subscribe_channel() = 0;

	void deal_channel_renew_timer_event(boost::system::error_code ec);
	void switch_to_secondary_channel();

public:

	renewable_websocket(boost::asio::io_context& ioc, binapi::ws::websockets& websocket, boost::posix_time::time_duration web_socket_timeout, boost::posix_time::time_duration web_socket_channel_renew);

	void unsubscribe();
};