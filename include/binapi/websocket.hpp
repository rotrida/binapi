
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __binapi__websocket_hpp
#define __binapi__websocket_hpp

#include "enums.hpp"

#include <memory>
#include <functional>
#include <string>
#include <boost/date_time.hpp>

namespace boost {
namespace asio {
class io_context;
} // ns asio
} // ns boost

namespace binapi {
namespace userdata {

struct account_update_t;
struct balance_update_t;
struct order_update_t;

struct option_account_update_t;
struct option_risk_level_change_t;
struct option_order_trade_update_t;

struct future_listen_key_expired_event_t;
struct future_account_update_event_t;
struct future_margin_call_event_t;
struct linear_future_order_update_event_t;
struct linear_future_trade_lite_update_t;
struct linear_future_configuration_update_t;
struct future_strategy_update_t;
struct future_grid_update_t;
struct linear_future_conditional_order_trigger_rejection_event_t;

} // ns userdata

namespace ws {

struct part_depths_t;
struct diff_depths_t;
struct trade_t;
struct agg_trade_t;
struct kline_t;
struct mini_ticker_t;
struct mini_tickers_t;
struct market_ticker_t;
struct linear_future_market_ticker_t;
struct inverse_future_market_ticker_t;
struct markets_tickers_t;
struct book_ticker_t;
struct new_symbol_info_t;
struct new_linear_future_symbol_info_t;
struct new_inverse_future_symbol_info_t;
struct option_ticker_t;

/*************************************************************************************************/

struct websockets {
    websockets(const websockets &) = delete;
    websockets& operator= (const websockets &) = delete;
    websockets(websockets &&) noexcept = default;
    websockets& operator= (websockets &&) noexcept = default;

    using on_message_received_cb = std::function<void(const char *channel, const char *ptr, std::size_t size)>;
    using log_callback = std::function<void(const std::string_view)>;

    websockets(
         boost::asio::io_context &ioctx
        ,std::string host
        ,std::string port
        ,log_callback log_callback_
        ,on_message_received_cb cb = {}
    );
    ~websockets();

    using handle = void *;
    using async_stop_callback = std::function<void()>;

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#partial-book-depth-streams
    using on_part_depths_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, part_depths_t msg, handle hnd)>;
    handle part_depth(const char *pair, e_levels level, e_freq freq, on_part_depths_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#diff-depth-stream
    using on_diff_depths_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, diff_depths_t msg, handle hnd)>;
    handle diff_depth(const char *pair, e_freq freq, on_diff_depths_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    handle diff_depth_options(const char *pair, on_diff_depths_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#klinecandlestick-streams
    using on_kline_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, kline_t msg, handle hnd)>;
    // period - 1m, 3m, 5m, 15m, 30m, 1h, 2h, 4h, 6h, 8h, 12h, 1d, 3d, 1w, 1M
    handle klines(const char *pair, const char *period, on_kline_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#trade-streams
    using on_trade_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, trade_t msg, handle hnd)>;
    handle trade(const char *pair, on_trade_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    handle options_trade(const char *pair, on_trade_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#aggregate-trade-streams
    using on_agg_trade_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, agg_trade_t msg, handle hnd)>;
    handle agg_trade(const char *pair, on_agg_trade_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#individual-symbol-mini-ticker-stream
    using on_mini_ticker_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, mini_ticker_t msg, handle hnd)>;
    handle mini_ticker(const char *pair, on_mini_ticker_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#all-market-mini-tickers-stream
    using on_mini_tickers_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, mini_tickers_t msg, handle hnd)>;
    handle mini_tickers(on_mini_tickers_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#individual-symbol-ticker-streams
    using on_market_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, market_ticker_t msg, handle hnd)>;
    handle market(const char *pair, on_market_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    // https://developers.binance.com/docs/derivatives/usds-margined-futures/websocket-market-streams/Individual-Symbol-Ticker-Streams
    using on_linear_future_market_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, linear_future_market_ticker_t msg, handle hnd)>;
    handle linear_future_market(const char *pair, on_linear_future_market_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    // https://developers.binance.com/docs/derivatives/coin-margined-futures/websocket-market-streams/Individual-Symbol-Ticker-Streams
    using on_inverse_future_market_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, inverse_future_market_ticker_t msg, handle hnd)>;
    handle inverse_future_market(const char *pair, on_inverse_future_market_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#all-market-tickers-stream
    using on_markets_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, markets_tickers_t msg, handle hnd)>;
    handle markets(on_markets_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#individual-symbol-book-ticker-streams
    using on_book_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, book_ticker_t msg, handle hnd)>;
    handle book(const char *pair, on_book_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#all-book-tickers-stream
    using on_books_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, book_ticker_t msg, handle hnd)>;
    handle books(on_books_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    using on_option_ticker_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, option_ticker_t msg, handle hnd)>;
    handle option_ticker(const char *pair, on_option_ticker_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    handle option_tickers(const char *currency, int expire, on_option_ticker_received_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    using on_new_symbol_info_cb = std::function<bool(const char* fl, int ec, std::string errmsg, new_symbol_info_t msg, handle hnd)>;
    handle new_symbol_info(on_new_symbol_info_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    using on_new_linear_future_symbol_info_cb = std::function<bool(const char* fl, int ec, std::string errmsg, new_linear_future_symbol_info_t msg, handle hnd)>;
    handle new_linear_future_symbol_info(on_new_linear_future_symbol_info_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    using on_new_inverse_future_symbol_info_cb = std::function<bool(const char* fl, int ec, std::string errmsg, new_inverse_future_symbol_info_t msg, handle hnd)>;
    handle new_inverse_future_symbol_info(on_new_inverse_future_symbol_info_cb cb, boost::posix_time::time_duration timeout = boost::posix_time::time_duration());

    using on_account_update_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::account_update_t msg, handle hnd)>;
    using on_balance_update_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::balance_update_t msg, handle hnd)>;
    using on_order_update_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::order_update_t msg, handle hnd)>;
    handle userdata(
         const char *lkey
        ,on_account_update_cb account_update
        ,on_balance_update_cb balance_update
        ,on_order_update_cb order_update
        ,boost::posix_time::time_duration timeout = boost::posix_time::time_duration()
    );

    using on_option_account_update_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::option_account_update_t msg, handle hnd)>;
    using on_option_risk_level_change_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::option_risk_level_change_t msg, handle hnd)>;
    using on_option_order_update_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::option_order_trade_update_t msg, handle hnd)>;
    handle option_userdata(
         const char *lkey
        ,on_option_account_update_cb option_account_update
        ,on_option_risk_level_change_cb risk_level_change
        ,on_option_order_update_cb option_order_update
        ,boost::posix_time::time_duration timeout = boost::posix_time::time_duration()
    );

    using on_linear_future_listen_key_expired_event_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::future_listen_key_expired_event_t msg, handle hnd)>;
    using on_linear_future_account_update_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::future_account_update_event_t msg, handle hnd)>;
    using on_linear_future_margin_call_event_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::future_margin_call_event_t msg, handle hnd)>;
    using on_linear_future_order_update_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::linear_future_order_update_event_t msg, handle hnd)>;
    using on_linear_future_trade_lite_event_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::linear_future_trade_lite_update_t msg, handle hnd)>;
    using on_linear_future_configuration_update_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::linear_future_configuration_update_t msg, handle hnd)>;
    using on_linear_future_strategy_update_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::future_strategy_update_t msg, handle hnd)>;
    using on_linear_grid_update_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::future_grid_update_t msg, handle hnd)>;
    using on_linear_future_conditional_order_trigger_rejection_event_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::linear_future_conditional_order_trigger_rejection_event_t msg, handle hnd)>;

    handle linear_future_userdata(
         const char *lkey
        ,on_linear_future_listen_key_expired_event_cb linear_future_listen_key_expired_event_cb
        ,on_linear_future_account_update_cb linear_future_account_update_cb
        ,on_linear_future_margin_call_event_cb linear_future_margin_call_event_cb
        ,on_linear_future_order_update_cb linear_future_order_update_cb
        ,on_linear_future_trade_lite_event_cb linear_future_trade_lite_event_cb
        ,on_linear_future_configuration_update_cb linear_future_configuration_update_cb
        ,on_linear_future_strategy_update_cb linear_future_strategy_update_cb
        ,on_linear_grid_update_cb linear_grid_update_cb
        ,on_linear_future_conditional_order_trigger_rejection_event_cb linear_future_conditional_order_trigger_rejection_event_cb
        ,boost::posix_time::time_duration timeout = boost::posix_time::time_duration()
    );

    void unsubscribe(const handle &h);
    void async_unsubscribe(const handle &h, async_stop_callback callback);
    void unsubscribe_all();
    void async_unsubscribe_all(async_stop_callback callback);

private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

/*************************************************************************************************/

} // ns ws
} // ns binapi

#endif // __binapi__websocket_hpp
