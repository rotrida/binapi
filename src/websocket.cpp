
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

/*
#include <boost/asio/io_context.hpp>
#include <boost/asio/io_context_strand.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
*/

#include <boost/callable_traits.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <map>
#include <unordered_map>
#include <cstring>

//#include <iostream> // TODO: comment out

#define __BINAPI_CB_ON_ERROR(cb, ec) \
    cb(__FILE__ "(" BOOST_PP_STRINGIZE(__LINE__) ")", ec.value(), ec.message(), nullptr, 0);

namespace binapi {
namespace ws {

struct websockets;

/*************************************************************************************************/

struct websocket: std::enable_shared_from_this<websocket> {
    friend struct websockets;

    explicit websocket(boost::asio::io_context &ioctx)
        :m_ioctx{ioctx}
        ,m_ssl{boost::asio::ssl::context::sslv23_client}
        ,m_resolver{m_ioctx}
        ,m_ws{boost::asio::make_strand(ioctx), m_ssl}
        ,m_buf{}
        ,m_host{}
        ,m_target{}
        ,m_stop_requested{}
    {}
    virtual ~websocket()
    {}

    using on_message_received_cb = std::function<
        bool(const char *fl, int ec, std::string errmsg, const char *ptr, std::size_t size)
    >; // when 'false' returned the stop will called

    using holder_type = std::shared_ptr<websocket>;
    void start(const std::string &host, const std::string &port, const std::string &target, on_message_received_cb cb, holder_type holder)
    { return async_start(host, port, target, std::move(cb), std::move(holder)); }

    void stop() {
        m_stop_requested = true;

        if (m_ws.is_open() ) {
            boost::system::error_code ec;
            m_ws.close(boost::beast::websocket::close_code::normal, ec);
        }
    }

    void async_stop() {
        m_stop_requested = true;
        holder_type holder = shared_from_this();

        if ( m_ws.is_open() ) {
            m_ws.async_close(
                 boost::beast::websocket::close_code::normal
                ,[holder=std::move(holder)](const boost::system::error_code &){}
            );
        }
    }

private:
    void async_start(const std::string &host, const std::string &port, const std::string &target, on_message_received_cb cb, holder_type holder) {
        m_host = host;
        m_target = target;

        m_resolver.async_resolve(
             m_host
            ,port
            ,[this, cb=std::move(cb), holder=std::move(holder)]
             (boost::system::error_code ec, boost::asio::ip::tcp::resolver::results_type res) mutable {
                if ( ec ) {
                    if ( !m_stop_requested ) { __BINAPI_CB_ON_ERROR(cb, ec); }
                } else {
                    async_connect(std::move(res), std::move(cb), std::move(holder));
                }
            }
        );
    }
    void async_connect(boost::asio::ip::tcp::resolver::results_type res, on_message_received_cb cb, holder_type holder) {
        if( !SSL_set_tlsext_host_name(m_ws.next_layer().native_handle() ,m_host.c_str())) {
            auto error_code = boost::beast::error_code(
                 static_cast<int>(::ERR_get_error())
                ,boost::asio::error::get_ssl_category()
            );

            __BINAPI_CB_ON_ERROR(cb, error_code);

            return;
        }

        boost::beast::get_lowest_layer(m_ws).async_connect(
            res.begin()
            ,res.end()
            ,[this, cb=std::move(cb), holder=std::move(holder)]
             (boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator) mutable {
                if ( ec ) {
                    if ( !m_stop_requested ) { __BINAPI_CB_ON_ERROR(cb, ec); }
                } else {
                    on_connected(std::move(cb), std::move(holder));
                }
            }
        );
    }
    void on_connected(on_message_received_cb cb, holder_type holder) {
        
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
            ,[this, cb=std::move(cb), holder=std::move(holder)]
             (boost::system::error_code ec) mutable {
                if ( ec ) {
                    if ( !m_stop_requested ) { __BINAPI_CB_ON_ERROR(cb, ec); }
                } else {
                    on_async_ssl_handshake(std::move(cb), std::move(holder));
                }
            }
        );
    }
    void on_async_ssl_handshake(on_message_received_cb cb, holder_type holder) {
        m_ws.async_handshake(
             m_host
            ,m_target
            ,[this, cb=std::move(cb), holder=std::move(holder)]
             (boost::system::error_code ec) mutable
             { start_read(ec, std::move(cb), std::move(holder)); }
        );
    }

    void start_read(boost::system::error_code ec, on_message_received_cb cb, holder_type holder) {
        if ( ec ) {
            if ( !m_stop_requested ) {
                __BINAPI_CB_ON_ERROR(cb, ec);
            }

            stop();

            return;
        }

        m_ws.async_read(
             m_buf
            ,[this, cb=std::move(cb), holder=std::move(holder)]
             (boost::system::error_code ec, std::size_t rd) mutable
             { on_read(ec, rd, std::move(cb), std::move(holder)); }
        );
    }
    void on_read(boost::system::error_code ec, std::size_t rd, on_message_received_cb cb, holder_type holder) {
        if ( ec ) {
            if ( !m_stop_requested ) {
                __BINAPI_CB_ON_ERROR(cb, ec);
            }

            stop();

            return;
        }

        auto size = m_buf.size();
        assert(size == rd);

        std::string strbuf;
        strbuf.reserve(size);

        for ( const auto &it: m_buf.data() ) {
            strbuf.append(static_cast<const char *>(it.data()), it.size());
        }
        m_buf.consume(m_buf.size());

        bool ok = cb(nullptr, 0, std::string{}, strbuf.data(), strbuf.size());
        if ( !ok ) {
            stop();
        } else {
            start_read(boost::system::error_code{}, std::move(cb), std::move(holder));
        }
    }

    boost::asio::io_context &m_ioctx;
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
    websockets::handle start_channel(const char *pair, const char *channel, F cb) {
        using args_tuple = typename boost::callable_traits::args<F>::type;
        using message_type = typename std::tuple_element<3, args_tuple>::type;

        std::shared_ptr<websocket> ws = std::make_shared<websocket>(m_ioctx);
        std::string schannel = make_channel_name(pair, channel);

        auto wscb = [this, schannel, cb=std::move(cb)]
            (const char *fl, int ec, std::string errmsg, const char *ptr, std::size_t size) -> bool
        {
            if ( ec ) {
                try {
                    cb(fl, ec, std::move(errmsg), message_type{});
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
                    return cb(__MAKE_FILELINE, ecode, std::move(emsg), std::move(message));
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
                return cb(fl, ec, std::move(errmsg), std::move(message));
            } catch (const std::exception &ex) {
                std::fprintf(stderr, "%s: %s\n", __MAKE_FILELINE, ex.what());
                std::fflush(stderr);
            }

            return false;
        };

        ws->start(
             m_host
            ,m_port
            ,schannel
            ,std::move(wscb)
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
    void async_stop_channel(handle h) {
        return stop_channel_impl(h, [](auto sp){ sp->async_stop(); });
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
    void async_unsubscribe_all() {
        return unsubscribe_all_impl([](auto sp){ sp->async_stop(); });
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
    :pimpl{std::make_unique<impl>(ioctx, std::move(host), std::move(port), std::move(cb))}
{}

websockets::~websockets()
{}

/*************************************************************************************************/

websockets::handle websockets::part_depth(const char *pair, e_levels level, e_freq freq, on_part_depths_received_cb cb) {
    std::string ch = "depth";
    ch += std::to_string(static_cast<std::size_t>(level));
    ch += "@";
    ch += std::to_string(static_cast<std::size_t>(freq)) + "ms";
    return pimpl->start_channel(pair, ch.c_str(), std::move(cb));
}

/*************************************************************************************************/

websockets::handle websockets::diff_depth(const char *pair, e_freq freq, on_diff_depths_received_cb cb) {
    std::string ch = "depth@" + std::to_string(static_cast<std::size_t>(freq)) + "ms";
    return pimpl->start_channel(pair, ch.c_str(), std::move(cb));
}

/*************************************************************************************************/

websockets::handle websockets::klines(const char *pair, const char *period, on_kline_received_cb cb) {
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

    return pimpl->start_channel(pair, p, std::move(cb));
}

/*************************************************************************************************/

websockets::handle websockets::trade(const char *pair, on_trade_received_cb cb)
{ return pimpl->start_channel(pair, "trade", std::move(cb)); }

/*************************************************************************************************/

websockets::handle websockets::agg_trade(const char *pair, on_agg_trade_received_cb cb)
{ return pimpl->start_channel(pair, "aggTrade", std::move(cb)); }

/*************************************************************************************************/

websockets::handle websockets::mini_ticker(const char *pair, on_mini_ticker_received_cb cb)
{ return pimpl->start_channel(pair, "miniTicker", std::move(cb)); }

websockets::handle websockets::mini_tickers(on_mini_tickers_received_cb cb)
{ return pimpl->start_channel("!miniTicker", "arr", std::move(cb)); }

/*************************************************************************************************/

websockets::handle websockets::market(const char *pair, on_market_received_cb cb)
{ return pimpl->start_channel(pair, "ticker", std::move(cb)); }

websockets::handle websockets::markets(on_markets_received_cb cb)
{ return pimpl->start_channel("!ticker", "arr", std::move(cb)); }

/*************************************************************************************************/

websockets::handle websockets::book(const char *pair, on_book_received_cb cb)
{ return pimpl->start_channel(pair, "bookTicker", std::move(cb)); }

websockets::handle websockets::books(on_books_received_cb cb)
{ return pimpl->start_channel(nullptr, "!bookTicker", std::move(cb)); }

/*************************************************************************************************/

websockets::handle websockets::userdata(
     const char *lkey
    ,on_account_update_cb account_update
    ,on_balance_update_cb balance_update
    ,on_order_update_cb order_update)
{
    auto cb = [acb=std::move(account_update), bcb=std::move(balance_update), ocb=std::move(order_update)]
        (const char *fl, int ec, std::string errmsg, userdata::userdata_stream_t msg)
    {
        if ( ec ) {
            acb(fl, ec, errmsg, userdata::account_update_t{});
            bcb(fl, ec, errmsg, userdata::balance_update_t{});
            ocb(fl, ec, std::move(errmsg), userdata::order_update_t{});

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
                return acb(fl, ec, std::move(errmsg), std::move(res));
            }
            case fnv1a("balanceUpdate"): {
                userdata::balance_update_t res = userdata::balance_update_t::construct(json);
                return bcb(fl, ec, std::move(errmsg), std::move(res));
            }
            case fnv1a("executionReport"): {
                userdata::order_update_t res = userdata::order_update_t::construct(json);
                return ocb(fl, ec, std::move(errmsg), std::move(res));
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

    return pimpl->start_channel(nullptr, lkey, std::move(cb));
}

/*************************************************************************************************/

void websockets::unsubscribe(const handle &h) { return pimpl->stop_channel(h); }
void websockets::async_unsubscribe(const handle &h) { return pimpl->async_stop_channel(h); }

void websockets::unsubscribe_all() { return pimpl->unsubscribe_all(); }
void websockets::async_unsubscribe_all() { return pimpl->async_unsubscribe_all(); }

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

} // ns ws
} // ns binapi
