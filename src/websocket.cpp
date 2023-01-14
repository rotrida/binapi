
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include <binapi/websocket.hpp>
#include <binapi/types.hpp>
#include <binapi/message.hpp>
#include <binapi/fnv1a.hpp>
#include <binapi/flatjson.hpp>
#include <binapi/errors.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/strand.hpp>

#include <boost/callable_traits.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/date_time.hpp>

#include <map>
#include <unordered_map>
#include <cstring>
#include <iostream>

//#include <iostream> // TODO: comment out

#define __BINAPI_CB_ON_ERROR(cb, ec, hnd) \
    cb(__FILE__ "(" BOOST_PP_STRINGIZE(__LINE__) ")", ec.value(), ec.message(), nullptr, 0, hnd);

namespace binapi {
namespace ws {

struct websockets;

/*************************************************************************************************/

struct websocket: std::enable_shared_from_this<websocket> {
    friend struct websockets;

    using on_message_received_cb = std::function<
        bool(const char* fl, int ec, std::string errmsg, const char* ptr, std::size_t size, void * hnd)
    >; // when 'false' returned the stop will called

    explicit websocket(boost::asio::io_context& ioctx, on_message_received_cb cb, boost::posix_time::time_duration timeout)
        :m_ioctx{ioctx}
        ,m_strand{ioctx}
        ,m_timeout{timeout}
        ,m_timeout_timer{ioctx}
        ,m_cb(cb)
        ,m_ssl{boost::asio::ssl::context::sslv23_client}
        ,m_resolver{m_ioctx}
        ,m_ws{boost::asio::make_strand(ioctx), m_ssl}
        ,m_buf{}
        ,m_host{}
        ,m_target{}
        ,m_stop_requested{}
    {}
    
    virtual ~websocket()
    {
        m_timeout_timer.cancel();
    }

    using holder_type = std::shared_ptr<websocket>;
    void start(const std::string &host, const std::string &port, const std::string &target, holder_type holder)
    { return async_start(host, port, target, std::move(holder)); }

    void stop() {
        m_stop_requested = true;

        if (m_ws.is_open() ) {
            boost::system::error_code ec;
            m_ws.close(boost::beast::websocket::close_code::normal, ec);
        }

        m_timeout_timer.cancel();
    }

    void async_stop(websockets::async_stop_callback callback) {
        m_stop_requested = true;
        holder_type holder = shared_from_this();

        if ( m_ws.is_open() ) {
            m_ws.async_close(
                 boost::beast::websocket::close_code::normal
                , [holder = std::move(holder), callback](const boost::system::error_code&) { callback(); }
            );
        }
    }

private:
    void async_start(const std::string &host, const std::string &port, const std::string &target, holder_type holder) {
        m_host = host;
        m_target = target;

        m_resolver.async_resolve(
             m_host
            ,port
            ,[this, holder=std::move(holder)]
             (boost::system::error_code ec, boost::asio::ip::tcp::resolver::results_type res) mutable {
                if ( ec ) {
                    if ( !m_stop_requested ) { __BINAPI_CB_ON_ERROR(m_cb, ec, this); }
                } else {
                    async_connect(std::move(res), std::move(holder));
                }
            }
        );
    }
    void async_connect(boost::asio::ip::tcp::resolver::results_type res, holder_type holder) {
        if( !SSL_set_tlsext_host_name(m_ws.next_layer().native_handle() ,m_host.c_str())) {
            auto error_code = boost::beast::error_code(
                 static_cast<int>(::ERR_get_error())
                ,boost::asio::error::get_ssl_category()
            );

            __BINAPI_CB_ON_ERROR(m_cb, error_code, this);

            return;
        }

        boost::beast::get_lowest_layer(m_ws).async_connect(
            res.begin()
            ,res.end()
            ,[this, holder=std::move(holder)]
             (boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator) mutable {
                if ( ec ) {
                    if ( !m_stop_requested ) { __BINAPI_CB_ON_ERROR(m_cb, ec, this); }
                } else {
                    on_connected(std::move(holder));
                }
            }
        );
    }
    void on_connected(holder_type holder) {
        
        m_ws.control_callback([this, me_ptr=shared_from_this()] (boost::beast::websocket::frame_type kind, boost::beast::string_view payload) mutable 
            {
                (void)kind; (void)payload;

                boost::asio::dispatch(m_strand, [this, me_ptr]() 
                {
                    m_last_message_received = boost::posix_time::second_clock::universal_time();
                });
            }
        );

        // Turn off the timeout on the tcp_stream, because
        // the websocket stream has its own timeout system.
        boost::beast::get_lowest_layer(m_ws).expires_never();

        // Set suggested timeout settings for the websocket
        m_ws.set_option(
            boost::beast::websocket::stream_base::timeout::suggested(
                boost::beast::role_type::client));

        // Set a decorator to change the User-Agent of the handshake
        m_ws.set_option(boost::beast::websocket::stream_base::decorator(
            [](boost::beast::websocket::request_type& req)
            {
                req.set(boost::beast::http::field::user_agent,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                    " websocket-client-async-ssl");
            }));

        m_ws.next_layer().async_handshake(
             boost::asio::ssl::stream_base::client
            ,[this, holder=std::move(holder)]
             (boost::system::error_code ec) mutable {
                if ( ec ) {
                    if ( !m_stop_requested ) { __BINAPI_CB_ON_ERROR(m_cb, ec, this); }
                } else {
                    on_async_ssl_handshake(std::move(holder));
                }
            }
        );
    }

    void on_timeout_timer_control(boost::system::error_code ec)
    {
        if (ec)
        {
            return;
        }

        const auto now = boost::posix_time::second_clock::universal_time();

        if (m_last_message_received + m_timeout < now)
        {
            m_cb(__FILE__, -1, "Websocket timeout", nullptr, 0, this);
            stop();

            return;
        }
        else if (m_last_message_received + m_timeout_verification < now)
        {
            auto me = shared_from_this();

            m_ws.async_ping(
                boost::beast::websocket::ping_data{}
                , [this, me](boost::beast::error_code ec)
                { 
                    if (!ec)
                        return;

                    __BINAPI_CB_ON_ERROR(m_cb, ec, this);
                }
            );
        }

        m_timeout_timer.expires_from_now(m_timeout_verification);
        m_timeout_timer.async_wait(boost::asio::bind_executor(m_strand, std::bind(&websocket::on_timeout_timer_control, shared_from_this(), std::placeholders::_1)));
    }

    void on_async_ssl_handshake(holder_type holder) {
        m_ws.async_handshake(
            m_host
            ,m_target
            ,m_strand.wrap([this, holder = std::move(holder)]
                (boost::system::error_code ec) mutable
                { 
                    start_read(ec, std::move(holder)); 
                }
            )
        );

        if (m_timeout != boost::posix_time::time_duration())
        {
            m_timeout_verification = m_timeout / 2;

            m_last_message_received = boost::posix_time::second_clock::universal_time();

            m_timeout_timer.expires_from_now(m_timeout_verification);
            m_timeout_timer.async_wait(boost::asio::bind_executor(m_strand, std::bind(&websocket::on_timeout_timer_control, shared_from_this(), std::placeholders::_1)));
        }
    }

    void start_read(boost::system::error_code ec, holder_type holder) {
        if ( ec ) {
            if ( !m_stop_requested ) {
                __BINAPI_CB_ON_ERROR(m_cb, ec, this);
            }

            stop();

            return;
        }

        m_ws.async_read(
             m_buf,
            m_strand.wrap(
                [this, holder = std::move(holder)]
                (boost::system::error_code ec, std::size_t rd) mutable
                { on_read(ec, rd, std::move(holder)); }
            )
        );
    }
    void on_read(boost::system::error_code ec, std::size_t rd, holder_type holder) {
        if ( ec ) {
            if ( !m_stop_requested ) {
                __BINAPI_CB_ON_ERROR(m_cb, ec, this);
            }

            stop();

            return;
        }

        m_last_message_received = boost::posix_time::second_clock::universal_time();

        auto size = m_buf.size();
        assert(size == rd);

        std::string strbuf;
        strbuf.reserve(size);

        for ( const auto &it: m_buf.data() ) {
            strbuf.append(static_cast<const char *>(it.data()), it.size());
        }
        m_buf.consume(m_buf.size());

        bool ok = m_cb(nullptr, 0, std::string{}, strbuf.data(), strbuf.size(), this);
        if ( !ok ) {
            stop();
        } else {
            start_read(boost::system::error_code{}, std::move(holder));
        }
    }

    boost::asio::io_context &m_ioctx;
    boost::asio::io_context::strand m_strand;
    boost::posix_time::time_duration m_timeout;
    boost::posix_time::time_duration m_timeout_verification;
    boost::asio::deadline_timer m_timeout_timer;
    on_message_received_cb m_cb;
    boost::posix_time::ptime m_last_message_received;
    boost::asio::ssl::context m_ssl;
    boost::asio::ip::tcp::resolver m_resolver;
    boost::beast::websocket::stream<boost::beast::ssl_stream<boost::beast::tcp_stream>> m_ws;
    boost::beast::multi_buffer m_buf;
    std::string m_host;
    std::string m_target;
    bool m_stop_requested;
    boost::intrusive::set_member_hook<> m_intrusive_set_hook;
};

struct websocket_id_getter {
    using type = const void *;
    type operator()(const websocket &sock) const { return std::addressof(sock); }
};

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

struct websockets::impl {
    impl(boost::asio::io_context &ioctx, std::string host, std::string port, on_message_received_cb cb)
        :m_ioctx{ioctx}
        ,m_strand{ioctx}
        ,m_host{std::move(host)}
        ,m_port{std::move(port)}
        ,m_on_message{std::move(cb)}
    {}
    
    ~impl() {
        unsubscribe_all();
    }

    static std::string make_channel_name(const char *pair, const char *channel) {
        std::string res{"/ws/"};
        if ( pair ) {
            res += pair;
            if ( *pair != '!' ) {
                boost::algorithm::to_lower(res);
            }

            res += '@';
        }

        res += channel;

        return res;
    }

    template<typename F>
    websockets::handle start_channel(const char *pair, const char *channel, F cb, boost::posix_time::time_duration timeout) {
        using args_tuple = typename boost::callable_traits::args<F>::type;
        using message_type = typename std::tuple_element<3, args_tuple>::type;

        std::string schannel = make_channel_name(pair, channel);

        auto wscb = [this, schannel, cb = std::move(cb)]
            (const char *fl, int ec, std::string errmsg, const char *ptr, std::size_t size, handle hnd) -> bool
        {
            if ( ec ) {
                try {
                    cb(fl, ec, std::move(errmsg), message_type{}, hnd);
                } catch (const std::exception &ex) {
                    std::fprintf(stderr, "%s: %s\n", __MAKE_FILELINE, ex.what());
                    std::fflush(stderr);
                }

                return false;
            }

            const flatjson::fjson json{ptr, size};
            if ( json.is_object() && binapi::rest::is_api_error(json) ) {
                auto error = binapi::rest::construct_error(json);
                auto ecode = error.first;
                auto emsg  = std::move(error.second);

                try {
                    message_type message{};
                    return cb(__MAKE_FILELINE, ecode, std::move(emsg), std::move(message), hnd);
                } catch (const std::exception &ex) {
                    std::fprintf(stderr, "%s: %s\n", __MAKE_FILELINE, ex.what());
                    std::fflush(stderr);
                }
            }

            try {
                if ( m_on_message ) { m_on_message(schannel.c_str(), ptr, size); }
            } catch (const std::exception &ex) {
                std::fprintf(stderr, "%s: %s\n", __MAKE_FILELINE, ex.what());
                std::fflush(stderr);
            }

            try {
                message_type message = message_type::construct(json);
                return cb(fl, ec, std::move(errmsg), std::move(message), hnd);
            } catch (const std::exception &ex) {
                std::fprintf(stderr, "%s: %s\n", __MAKE_FILELINE, ex.what());
                std::fflush(stderr);
            }

            return false;
        };

        std::shared_ptr<websocket> ws = std::make_shared<websocket>(m_ioctx, wscb, timeout);

        ws->start(
             m_host
            ,m_port
            ,schannel
            ,ws
        );

        boost::asio::dispatch(m_strand, [this, ws]()
        {
            m_websockets.insert(std::make_pair(ws.get(), ws));
        });
        
        return ws.get();
    }

    template<typename F>
    void stop_channel_impl(handle h, F f) {
        boost::asio::dispatch(m_strand, [this, h, f]()
        {
            auto it = m_websockets.find(h);
            if (it == m_websockets.end()) { return; }

            f(it->second);

            m_websockets.erase(it);
        });
    }

    void stop_channel(handle h) {
        return stop_channel_impl(h, [](auto sp){ sp->stop(); });
    }
    void async_stop_channel(handle h, async_stop_callback callback) {
        return stop_channel_impl(h, [callback](auto sp){ sp->async_stop(callback); });
    }

    template<typename F>
    void unsubscribe_all_impl(F f) {
        boost::asio::dispatch(m_strand, [this, f]()
        {
            for (auto & it : m_websockets) 
            {
                f(it.second);
            }

            m_websockets.clear();
        });
    }
    void unsubscribe_all() {
        return unsubscribe_all_impl([](auto sp){ sp->stop(); });
    }
    void async_unsubscribe_all(async_stop_callback callback) {
        return unsubscribe_all_impl([callback](auto sp){ sp->async_stop(callback); });
    }

    boost::asio::io_context &m_ioctx;
    boost::asio::io_context::strand m_strand;
    std::string m_host;
    std::string m_port;
    on_message_received_cb m_on_message;
    std::unordered_map<handle, std::shared_ptr<websocket>> m_websockets;
};

/*************************************************************************************************/

websockets::websockets(
     boost::asio::io_context &ioctx
    ,std::string host
    ,std::string port
    ,on_message_received_cb cb
)
    :pimpl{std::make_unique<impl>(ioctx, std::move(host), std::move(port),cb)}
{}

websockets::~websockets()
{}

/*************************************************************************************************/

websockets::handle websockets::part_depth(const char *pair, e_levels level, e_freq freq, on_part_depths_received_cb cb, boost::posix_time::time_duration timeout) {
    std::string ch = "depth";
    ch += std::to_string(static_cast<std::size_t>(level));
    ch += "@";
    ch += std::to_string(static_cast<std::size_t>(freq)) + "ms";
    return pimpl->start_channel(pair, ch.c_str(), std::move(cb), timeout);
}

/*************************************************************************************************/

websockets::handle websockets::diff_depth(const char *pair, e_freq freq, on_diff_depths_received_cb cb, boost::posix_time::time_duration timeout) {
    std::string ch = "depth@" + std::to_string(static_cast<std::size_t>(freq)) + "ms";
    return pimpl->start_channel(pair, ch.c_str(), std::move(cb), timeout);
}

/*************************************************************************************************/

websockets::handle websockets::klines(const char *pair, const char *period, on_kline_received_cb cb, boost::posix_time::time_duration timeout) {
    static const auto switch_ = [](const char *period) -> const char * {
        const auto hash = fnv1a(period);
        switch ( hash ) {
            // mins
            case fnv1a("1m"): return "kline_1m";
            case fnv1a("3m"): return "kline_3m";
            case fnv1a("5m"): return "kline_5m";
            case fnv1a("15m"): return "kline_15m";
            case fnv1a("30m"): return "kline_30m";
            // hours
            case fnv1a("1h"): return "kline_1h";
            case fnv1a("2h"): return "kline_2h";
            case fnv1a("4h"): return "kline_4h";
            case fnv1a("6h"): return "kline_6h";
            case fnv1a("8h"): return "kline_8h";
            case fnv1a("12h"): return "kline_12h";
            // days
            case fnv1a("1d"): return "kline_1d";
            case fnv1a("3d"): return "kline_3d";
            // other
            case fnv1a("1w"): return "kline_1w";
            case fnv1a("1M"): return "kline_1M";
            //
            default: return nullptr;
        }
    };

    const char *p = switch_(period);
    assert(p != nullptr);

    return pimpl->start_channel(pair, p, std::move(cb), timeout);
}

/*************************************************************************************************/

websockets::handle websockets::trade(const char *pair, on_trade_received_cb cb, boost::posix_time::time_duration timeout)
{ return pimpl->start_channel(pair, "trade", std::move(cb), timeout); }

/*************************************************************************************************/

websockets::handle websockets::agg_trade(const char *pair, on_agg_trade_received_cb cb, boost::posix_time::time_duration timeout)
{ return pimpl->start_channel(pair, "aggTrade", std::move(cb), timeout); }

/*************************************************************************************************/

websockets::handle websockets::mini_ticker(const char *pair, on_mini_ticker_received_cb cb, boost::posix_time::time_duration timeout)
{ return pimpl->start_channel(pair, "miniTicker", std::move(cb), timeout); }

websockets::handle websockets::mini_tickers(on_mini_tickers_received_cb cb, boost::posix_time::time_duration timeout)
{ return pimpl->start_channel("!miniTicker", "arr", std::move(cb), timeout); }

/*************************************************************************************************/

websockets::handle websockets::market(const char *pair, on_market_received_cb cb, boost::posix_time::time_duration timeout)
{ return pimpl->start_channel(pair, "ticker", std::move(cb), timeout); }

websockets::handle websockets::markets(on_markets_received_cb cb, boost::posix_time::time_duration timeout)
{ return pimpl->start_channel("!ticker", "arr", std::move(cb), timeout); }

/*************************************************************************************************/

websockets::handle websockets::book(const char *pair, on_book_received_cb cb, boost::posix_time::time_duration timeout)
{ return pimpl->start_channel(pair, "bookTicker", std::move(cb), timeout); }

websockets::handle websockets::books(on_books_received_cb cb, boost::posix_time::time_duration timeout)
{ return pimpl->start_channel(nullptr, "!bookTicker", std::move(cb), timeout); }

/*************************************************************************************************/

websockets::handle websockets::userdata(
     const char *lkey
    ,on_account_update_cb account_update
    ,on_balance_update_cb balance_update
    ,on_order_update_cb order_update
    ,boost::posix_time::time_duration timeout)
{
    auto cb = [acb=std::move(account_update), bcb=std::move(balance_update), ocb=std::move(order_update)]
        (const char *fl, int ec, std::string errmsg, userdata::userdata_stream_t msg, handle hnd)
    {
        if ( ec ) {
            acb(fl, ec, errmsg, userdata::account_update_t{}, hnd);
            bcb(fl, ec, errmsg, userdata::balance_update_t{}, hnd);
            ocb(fl, ec, std::move(errmsg), userdata::order_update_t{}, hnd);

            return false;
        }

        const flatjson::fjson json{msg.data.c_str(), msg.data.length()};
        assert(json.contains("e"));
        const auto e = json.at("e");
        const auto es = e.to_sstring();
        const auto ehash = fnv1a(es.data(), es.size());
        switch ( ehash ) {
            case fnv1a("outboundAccountPosition"): {
                userdata::account_update_t res = userdata::account_update_t::construct(json);
                return acb(fl, ec, std::move(errmsg), std::move(res), hnd);
            }
            case fnv1a("balanceUpdate"): {
                userdata::balance_update_t res = userdata::balance_update_t::construct(json);
                return bcb(fl, ec, std::move(errmsg), std::move(res), hnd);
            }
            case fnv1a("executionReport"): {
                userdata::order_update_t res = userdata::order_update_t::construct(json);
                return ocb(fl, ec, std::move(errmsg), std::move(res), hnd);
            }
            case fnv1a("listStatus"): {
                assert(!"not implemented");
                return false;
            }
            default: {
                assert(!"unreachable");
                return false;
            }
        }

        return false;
    };

    return pimpl->start_channel(nullptr, lkey, std::move(cb), timeout);
}

/*************************************************************************************************/

void websockets::unsubscribe(const handle &h) { return pimpl->stop_channel(h); }
void websockets::async_unsubscribe(const handle &h, async_stop_callback callback) { return pimpl->async_stop_channel(h, callback); }

void websockets::unsubscribe_all() { return pimpl->unsubscribe_all(); }
void websockets::async_unsubscribe_all(async_stop_callback callback) { return pimpl->async_unsubscribe_all(callback); }

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

} // ns ws
} // ns binapi
