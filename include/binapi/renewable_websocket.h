#pragma once

#include <boost/asio.hpp>
#include "binapi/websocket.hpp"

namespace binapi
{
	namespace ws
	{
		class renewable_websocket: public std::enable_shared_from_this<renewable_websocket>
		{
		protected:

			boost::asio::io_context& _ioc;
			boost::asio::io_context::strand _strand;
			boost::asio::deadline_timer _channel_renew_timer;
			binapi::ws::websockets& _websockets;
			boost::posix_time::time_duration _web_socket_timeout;
			boost::posix_time::time_duration _web_socket_channel_renew;
			binapi::ws::websockets::handle _active_channel;
			binapi::ws::websockets::handle _secondary_channel;
			bool _stopped;

			using async_channel_creation_callback = std::function<void(binapi::ws::websockets::handle)>;
			void create_channel(async_channel_creation_callback callback);
			
			virtual void subscribe_channel(async_channel_creation_callback callback) = 0;
			virtual void unsubscribe_channel(binapi::ws::websockets::handle handle, binapi::ws::websockets::async_stop_callback callback);

			void deal_channel_renew_timer_event(boost::system::error_code ec);
			virtual void internal_switch_to_secondary_channel();

		public:

			renewable_websocket(boost::asio::io_context& ioc, binapi::ws::websockets& websocket, boost::posix_time::time_duration web_socket_timeout, boost::posix_time::time_duration web_socket_channel_renew);

			virtual void start();
			virtual void stop(binapi::ws::websockets::async_stop_callback callback);
		};
	}
}