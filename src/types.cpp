
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include <binapi/types.hpp>
#include <binapi/flatjson.hpp>
#include <binapi/fnv1a.hpp>

#include <type_traits>

//#include <iostream> // TODO: comment out

namespace binapi {

/*************************************************************************************************/

template<typename T>
typename std::enable_if<std::is_integral<T>::value>::type
__get_json(T &v, const char *member, const flatjson::fjson &j) {
    v = j.at(member).to<T>();
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value>::type
__get_json_def(T &v, const char *member, const flatjson::fjson &j, T default_value) {
    
    if(j.contains(member))
    {
        const auto& o = j.at(member);
        v = (o.is_null() ? default_value : o.to<T>());
    }
    else
    {
        v = default_value;
    }
}

template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value>::type
__get_json(T &v, const char *member, const flatjson::fjson &j) {
    const auto &o = j.at(member);
    v = (o.is_null() ? std::string{} : o.to_string());
}

template<typename T>
typename std::enable_if<std::is_same<T, double_type>::value>::type
__get_json(T &v, const char *member, const flatjson::fjson &j) {
    v = new_qube::fixed_point<>(j.at(member).to_string());
}


#define __BINAPI_GET2(obj, member, json) \
    __get_json(obj.member, #member, json)

#define __BINAPI_GET2_DEF(obj, member, json, default_value) \
    __get_json_def(obj.member, #member, json, default_value)

#define __BINAPI_GET(member) __BINAPI_GET2(res, member, json)

#define __BINAPI_GET_DEFAULT(member, default_value) __BINAPI_GET2_DEF(res, member, json, default_value)

/*************************************************************************************************/

namespace rest {

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

ping_t ping_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());
    assert(json.is_object());

    ping_t res{};
    res.ok = json.size() == 0;

    return res;
}

std::ostream &operator<<(std::ostream &os, const ping_t &o) {
    os
    << "{"
    << "\"ok\":" << std::boolalpha << o.ok
    << "}";

    return os;
}

/*************************************************************************************************/

server_time_t server_time_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    server_time_t res{};
    __BINAPI_GET(serverTime);

    return res;
}

std::ostream &operator<<(std::ostream &os, const server_time_t &o) {
    os
    << "{"
    << "\"serverTime\":" << o.serverTime
    << "}";

    return os;
}

/*************************************************************************************************/

avg_price_t avg_price_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    avg_price_t res{};
    __BINAPI_GET(mins);
    __BINAPI_GET(price);

    return res;
}

std::ostream &operator<<(std::ostream &os, const avg_price_t &o) {
    os
    << "{"
    << "\"mins\":" << o.mins << ","
    << "\"price\":\"" << o.price << "\""
    << "}";

    return os;
}

/*************************************************************************************************/

prices_t::price_t prices_t::price_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    prices_t::price_t res{};
    __BINAPI_GET(symbol);
    __BINAPI_GET(price);

    return res;
}

std::ostream &operator<<(std::ostream &os, const prices_t::price_t &o) {
    os
    << "{"
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"price\":\"" << o.price << "\""
    << "}";

    return os;
}

/*************************************************************************************************/

prices_t prices_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());
    assert(json.is_array());

    prices_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        const auto it = json[idx];
        prices_t::price_t item{};

        __BINAPI_GET2(item, symbol, it);
        __BINAPI_GET2(item, price, it);

        std::string symbol = item.symbol;
        res.prices.emplace(std::move(symbol), std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const prices_t &o) {
    os
    << "[";
    for ( auto it = o.prices.begin(); it != o.prices.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != o.prices.end() ) {
            os << ",";
        }
    }

    os
    << "]";

    return os;
}

bool prices_t::is_valid_symbol(const char *sym) const {
    return prices.find(sym) != prices.end();
}

const prices_t::price_t& prices_t::get_by_symbol(const char *sym) const {
    auto it = prices.find(sym);
    if ( it != prices.end() ) {
        return it->second;
    }

    assert(!"unreachable");
}

/*************************************************************************************************/

_24hrs_tickers_t::_24hrs_ticker_t
_24hrs_tickers_t::_24hrs_ticker_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    _24hrs_tickers_t::_24hrs_ticker_t res{};
    __BINAPI_GET(symbol);
    __BINAPI_GET(priceChange);
    __BINAPI_GET(priceChangePercent);
    __BINAPI_GET(weightedAvgPrice);
    __BINAPI_GET(prevClosePrice);
    __BINAPI_GET(lastPrice);
    __BINAPI_GET(lastQty);
    __BINAPI_GET(bidPrice);
    __BINAPI_GET(askPrice);
    __BINAPI_GET(openPrice);
    __BINAPI_GET(highPrice);
    __BINAPI_GET(lowPrice);
    __BINAPI_GET(volume);
    __BINAPI_GET(quoteVolume);
    __BINAPI_GET(openTime);
    __BINAPI_GET(closeTime);
    __BINAPI_GET(firstId);
    __BINAPI_GET(lastId);
    __BINAPI_GET(count);

    return res;
}

std::ostream &operator<<(std::ostream &os, const _24hrs_tickers_t::_24hrs_ticker_t &f) {
    os
    << "{"
    << "\"symbol\":\"" << f.symbol << "\","
    << "\"priceChange\":\"" << f.priceChange << "\","
    << "\"priceChangePercent\":\"" << f.priceChangePercent << "\","
    << "\"weightedAvgPrice\":\"" << f.weightedAvgPrice << "\","
    << "\"prevClosePrice\":\"" << f.prevClosePrice << "\","
    << "\"lastPrice\":\"" << f.lastPrice << "\","
    << "\"lastQty\":\"" << f.lastQty << "\","
    << "\"bidPrice\":\"" << f.bidPrice << "\","
    << "\"askPrice\":\"" << f.askPrice << "\","
    << "\"openPrice\":\"" << f.openPrice << "\","
    << "\"highPrice\":\"" << f.highPrice << "\","
    << "\"lowPrice\":\"" << f.lowPrice << "\","
    << "\"volume\":\"" << f.volume << "\","
    << "\"quoteVolume\":\"" << f.quoteVolume << "\","
    << "\"openTime\":" << f.openTime << ","
    << "\"closeTime\":" << f.closeTime << ","
    << "\"firstId\":" << f.firstId << ","
    << "\"lastId\":" << f.lastId << ","
    << "\"count\":" << f.count
    << "}";

    return os;
}

_24hrs_tickers_t _24hrs_tickers_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    _24hrs_tickers_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        auto ji = json.at(idx);
        assert(ji.is_object());

        _24hrs_tickers_t::_24hrs_ticker_t item = _24hrs_tickers_t::_24hrs_ticker_t::construct(ji);
        std::string symbol = item.symbol;
        res.tickers.emplace(std::move(symbol), std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const _24hrs_tickers_t &f) {
    os
    << "[";

    for ( auto it = f.tickers.begin(); it != f.tickers.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != f.tickers.end() ) {
            os << ",";
        }
    }
    os
    << "]";

    return os;
}

/*************************************************************************************************/

account_info_t::balance_t account_info_t::balance_t::construct(const flatjson::fjson &json) {
    (void)json;

    static const account_info_t::balance_t res{};

    return res;
}

std::ostream &operator<<(std::ostream &os, const account_info_t::balance_t &o) {
    os
    << "{"
    << "\"asset\":\"" << o.asset << "\","
    << "\"free\":\"" << o.free << "\","
    << "\"locked\":\"" << o.locked << "\""
    << "}";

    return os;
}

account_info_t account_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    account_info_t res{};
    __BINAPI_GET(makerCommission);
    __BINAPI_GET(takerCommission);
    __BINAPI_GET(buyerCommission);
    __BINAPI_GET(sellerCommission);
    __BINAPI_GET(canTrade);
    __BINAPI_GET(canWithdraw);
    __BINAPI_GET(canDeposit);
    __BINAPI_GET(updateTime);
    const auto arr = json.at("balances");
    assert(arr.is_array());
    for ( auto idx = 0u; idx < arr.size(); ++idx ) {
        account_info_t::balance_t item{};
        const auto it = arr.at(idx);
        __BINAPI_GET2(item, asset, it);
        __BINAPI_GET2(item, free, it);
        __BINAPI_GET2(item, locked, it);

        std::string symbol = item.asset;
        res.balances.emplace(std::move(symbol), std::move(item));
    }

    return res;
}

const account_info_t::balance_t& account_info_t::get_balance(const char *symbol) const {
    auto it = balances.find(symbol);
    if ( it != balances.end() ) {
        return it->second;
    }

    assert(!"unreachable");
}

const double_type& account_info_t::add_balance(const char *symbol, const double_type &amount) {
    auto it = balances.find(symbol);
    if ( it != balances.end() ) {
        it->second.free += amount;
        return it->second.free;
    }

    assert(!"unreachable");
}

const double_type& account_info_t::sub_balance(const char *symbol, const double_type &amount) {
    auto it = balances.find(symbol);
    if ( it != balances.end() ) {
        it->second.free -= amount;
        return it->second.free;
    }

    assert(!"unreachable");
}

std::ostream &operator<<(std::ostream &os, const account_info_t &o) {
    os
    << "{"
    << "\"makerCommission\":" << o.makerCommission << ","
    << "\"takerCommission\":" << o.takerCommission << ","
    << "\"buyerCommission\":" << o.buyerCommission << ","
    << "\"sellerCommission\":" << o.sellerCommission << ","
    << "\"canTrade\":" << (o.canTrade ? "true" : "false") << ","
    << "\"canWithdraw\":" << (o.canWithdraw ? "true" : "false") << ","
    << "\"canDeposit\":" << (o.canDeposit ? "true" : "false") << ","
    << "\"updateTime\":" << o.updateTime << ","
    << "\"balances\":[";
    for ( auto it = o.balances.begin(); it != o.balances.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != o.balances.end() ) {
            os << ",";
        }
    }

    os << "]}";

    return os;
}

/*************************************************************************************************/

option_account_info_t::asset_t option_account_info_t::asset_t::construct(const flatjson::fjson &json) {

    option_account_info_t::asset_t res{};

    __BINAPI_GET(asset);
    __BINAPI_GET(marginBalance);
    __BINAPI_GET(equity);
    __BINAPI_GET(available);
    __BINAPI_GET(locked);
    __BINAPI_GET(unrealizedPNL);

    return res;
}

std::ostream &operator<<(std::ostream &os, const option_account_info_t::asset_t &o) {
    os
    << "{"
    << "\"asset\":\"" << o.asset << "\","
    << "\"marginBalance\":\"" << o.marginBalance << "\","
    << "\"equity\":\"" << o.equity << "\","
    << "\"available\":\"" << o.available << "\","
    << "\"locked\":\"" << o.locked << "\","
    << "\"unrealizedPNL\":\"" << o.unrealizedPNL << "\""
    << "}";

    return os;
}

option_account_info_t::greek_t option_account_info_t::greek_t::construct(const flatjson::fjson &json) {

    option_account_info_t::greek_t res{};

    __BINAPI_GET(underlying);
    __BINAPI_GET(delta);
    __BINAPI_GET(gamma);
    __BINAPI_GET(theta);
    __BINAPI_GET(vega);

    return res;
}

std::ostream &operator<<(std::ostream &os, const option_account_info_t::greek_t &o) {
    os
    << "{"
    << "\"underlying\":\"" << o.underlying << "\","
    << "\"delta\":\"" << o.delta << "\","
    << "\"gamma\":\"" << o.gamma << "\","
    << "\"theta\":\"" << o.theta << "\","
    << "\"vega\":\"" << o.vega << "\""
    << "}";

    return os;
}

option_account_info_t option_account_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    option_account_info_t res{};

    __BINAPI_GET(time);
    __BINAPI_GET(riskLevel);

    const auto asset_arr = json.at("asset");
    assert(asset_arr.is_array());
    for ( auto idx = 0u; idx < asset_arr.size(); ++idx ) {

        const auto it = asset_arr.at(idx);

        asset_t asset = asset_t::construct(it);
        std::string symbol = asset.asset;

        res.assets.emplace(std::move(symbol), std::move(asset));
    }

    const auto greek_arr = json.at("greek");
    assert(greek_arr.is_array());
    for ( auto idx = 0u; idx < greek_arr.size(); ++idx ) {

        const auto it = greek_arr.at(idx);

        greek_t greek = greek_t::construct(it);
        std::string underlying = greek.underlying;

        res.greeks.emplace(std::move(underlying), std::move(greek));
    }

    return res;
}

const option_account_info_t::asset_t& option_account_info_t::get_asset(const char *asset) const {
    auto it = assets.find(asset);
    if ( it != assets.end() ) {
        return it->second;
    }

    assert(!"unreachable");
}

const option_account_info_t::greek_t& option_account_info_t::get_greek(const char *underlying) const {
    auto it = greeks.find(underlying);
    if ( it != greeks.end() ) {
        return it->second;
    }

    assert(!"unreachable");
}

std::ostream &operator<<(std::ostream &os, const option_account_info_t &o) {
    os
    << "{"
    << "\"time\":" << o.time << ","
    << "\"riskLevel\":" << o.riskLevel << ","
    << "\"assets\":[";
    for ( auto it = o.assets.begin(); it != o.assets.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != o.assets.end() ) {
            os << ",";
        }
    }
    os << "\"greeks\":[";
    for ( auto it = o.greeks.begin(); it != o.greeks.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != o.greeks.end() ) {
            os << ",";
        }
    }
    os << "]}";

    return os;
}

/*************************************************************************************************/

linear_future_account_info_t::asset_t linear_future_account_info_t::asset_t::construct(const flatjson::fjson &json) {

    linear_future_account_info_t::asset_t res{};

    __BINAPI_GET(asset);
    __BINAPI_GET(walletBalance);
    __BINAPI_GET(unrealizedProfit);
    __BINAPI_GET(marginBalance);
    __BINAPI_GET(maintMargin);
    __BINAPI_GET(initialMargin);
    __BINAPI_GET(positionInitialMargin);
    __BINAPI_GET(openOrderInitialMargin);
    __BINAPI_GET(crossWalletBalance);
    __BINAPI_GET(crossUnPnl);
    __BINAPI_GET(availableBalance);
    __BINAPI_GET(maxWithdrawAmount);
    __BINAPI_GET(marginAvailable);
    __BINAPI_GET(updateTime);

    return res;
}

std::ostream &operator<<(std::ostream &os, const linear_future_account_info_t::asset_t &o) {
    os
    << "{"
    << "\"asset\":\"" << o.asset << "\","
    << "\"walletBalance\":\"" << o.walletBalance << "\","
    << "\"unrealizedProfit\":\"" << o.unrealizedProfit << "\","
    << "\"marginBalance\":\"" << o.marginBalance << "\","
    << "\"maintMargin\":\"" << o.maintMargin << "\","
    << "\"initialMargin\":\"" << o.initialMargin << "\","
    << "\"positionInitialMargin\":\"" << o.positionInitialMargin << "\","
    << "\"openOrderInitialMargin\":\"" << o.openOrderInitialMargin << "\","
    << "\"crossWalletBalance\":\"" << o.crossWalletBalance << "\","
    << "\"crossUnPnl\":\"" << o.crossUnPnl << "\","
    << "\"availableBalance\":\"" << o.availableBalance << "\","
    << "\"maxWithdrawAmount\":\"" << o.maxWithdrawAmount << "\","
    << "\"marginAvailable\":\"" << o.marginAvailable << "\","
    << "\"updateTime\":\"" << o.updateTime << "\""
    << "}";

    return os;
}

linear_future_account_info_t::position_t linear_future_account_info_t::position_t::construct(const flatjson::fjson &json) {

    linear_future_account_info_t::position_t res{};

    __BINAPI_GET(symbol);
    __BINAPI_GET(positionSide);
    __BINAPI_GET(positionAmt);
    __BINAPI_GET(unrealizedProfit);
    __BINAPI_GET(isolatedMargin);
    __BINAPI_GET(notional);
    __BINAPI_GET(isolatedWallet);
    __BINAPI_GET(initialMargin);
    __BINAPI_GET(maintMargin);
    __BINAPI_GET(updateTime);

    return res;
}

std::ostream &operator<<(std::ostream &os, const linear_future_account_info_t::position_t &o) {
    os
    << "{"
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"positionSide\":\"" << o.positionSide << "\","
    << "\"positionAmt\":\"" << o.positionAmt << "\","
    << "\"unrealizedProfit\":\"" << o.unrealizedProfit << "\","
    << "\"isolatedMargin\":\"" << o.isolatedMargin << "\","
    << "\"notional\":\"" << o.notional << "\","
    << "\"isolatedWallet\":\"" << o.isolatedWallet << "\","
    << "\"initialMargin\":\"" << o.initialMargin << "\","
    << "\"maintMargin\":\"" << o.maintMargin << "\","
    << "\"updateTime\":\"" << o.updateTime << "\""
    << "}";

    return os;
}

linear_future_account_info_t linear_future_account_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    linear_future_account_info_t res{};

    __BINAPI_GET(totalInitialMargin);
    __BINAPI_GET(totalMaintMargin);
    __BINAPI_GET(totalWalletBalance);
    __BINAPI_GET(totalUnrealizedProfit);
    __BINAPI_GET(totalMarginBalance);
    __BINAPI_GET(totalPositionInitialMargin);
    __BINAPI_GET(totalOpenOrderInitialMargin);
    __BINAPI_GET(totalCrossWalletBalance);
    __BINAPI_GET(totalCrossUnPnl);
    __BINAPI_GET(availableBalance);
    __BINAPI_GET(maxWithdrawAmount);

    const auto asset_arr = json.at("asset");
    assert(asset_arr.is_array());
    for ( auto idx = 0u; idx < asset_arr.size(); ++idx ) {

        const auto it = asset_arr.at(idx);

        asset_t asset = asset_t::construct(it);
        std::string symbol = asset.asset;

        res.assets.emplace(std::move(symbol), std::move(asset));
    }

    const auto position_arr = json.at("positions");
    assert(position_arr.is_array());
    for ( auto idx = 0u; idx < position_arr.size(); ++idx ) {

        const auto it = position_arr.at(idx);

        position_t position = position_t::construct(it);
        std::string symbol = position.symbol;

        res.positions.emplace(std::move(symbol), std::move(position));
    }

    return res;
}

const linear_future_account_info_t::asset_t& linear_future_account_info_t::get_asset(const char *asset) const {
    auto it = assets.find(asset);
    if ( it != assets.end() ) {
        return it->second;
    }

    assert(!"unreachable");
}

const linear_future_account_info_t::position_t& linear_future_account_info_t::get_position(const char *symbol) const {
    auto it = positions.find(symbol);
    if ( it != positions.end() ) {
        return it->second;
    }

    assert(!"unreachable");
}

std::ostream &operator<<(std::ostream &os, const linear_future_account_info_t &o) {
    os
    << "{"
    << "\"totalInitialMargin\":" << o.totalInitialMargin << ","
    << "\"totalMaintMargin\":" << o.totalMaintMargin << ","
    << "\"totalWalletBalance\":" << o.totalWalletBalance << ","
    << "\"totalUnrealizedProfit\":" << o.totalUnrealizedProfit << ","
    << "\"totalMarginBalance\":" << o.totalMarginBalance << ","
    << "\"totalPositionInitialMargin\":" << o.totalPositionInitialMargin << ","
    << "\"totalOpenOrderInitialMargin\":" << o.totalOpenOrderInitialMargin << ","
    << "\"totalCrossWalletBalance\":" << o.totalCrossWalletBalance << ","
    << "\"totalCrossUnPnl\":" << o.totalCrossUnPnl << ","
    << "\"availableBalance\":" << o.availableBalance << ","
    << "\"maxWithdrawAmount\":" << o.maxWithdrawAmount << ","
    << "\"assets\":[";
    for ( auto it = o.assets.begin(); it != o.assets.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != o.assets.end() ) {
            os << ",";
        }
    }
    os << "\"positions\":[";
    for ( auto it = o.positions.begin(); it != o.positions.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != o.positions.end() ) {
            os << ",";
        }
    }
    os << "]}";

    return os;
}

/*************************************************************************************************/

inverse_future_account_info_t::asset_t inverse_future_account_info_t::asset_t::construct(const flatjson::fjson &json) {

    inverse_future_account_info_t::asset_t res{};

    __BINAPI_GET(asset);
    __BINAPI_GET(walletBalance);
    __BINAPI_GET(unrealizedProfit);
    __BINAPI_GET(marginBalance);
    __BINAPI_GET(maintMargin);
    __BINAPI_GET(initialMargin);
    __BINAPI_GET(positionInitialMargin);
    __BINAPI_GET(openOrderInitialMargin);
    __BINAPI_GET(crossWalletBalance);
    __BINAPI_GET(crossUnPnl);
    __BINAPI_GET(availableBalance);
    __BINAPI_GET(maxWithdrawAmount);
    __BINAPI_GET(updateTime);

    return res;
}

std::ostream &operator<<(std::ostream &os, const inverse_future_account_info_t::asset_t &o) {
    os
    << "{"
    << "\"asset\":\"" << o.asset << "\","
    << "\"walletBalance\":\"" << o.walletBalance << "\","
    << "\"unrealizedProfit\":\"" << o.unrealizedProfit << "\","
    << "\"marginBalance\":\"" << o.marginBalance << "\","
    << "\"maintMargin\":\"" << o.maintMargin << "\","
    << "\"initialMargin\":\"" << o.initialMargin << "\","
    << "\"positionInitialMargin\":\"" << o.positionInitialMargin << "\","
    << "\"openOrderInitialMargin\":\"" << o.openOrderInitialMargin << "\","
    << "\"crossWalletBalance\":\"" << o.crossWalletBalance << "\","
    << "\"crossUnPnl\":\"" << o.crossUnPnl << "\","
    << "\"availableBalance\":\"" << o.availableBalance << "\","
    << "\"maxWithdrawAmount\":\"" << o.maxWithdrawAmount << "\","
    << "\"updateTime\":\"" << o.updateTime << "\""
    << "}";

    return os;
}

inverse_future_account_info_t::position_t inverse_future_account_info_t::position_t::construct(const flatjson::fjson &json) {

    inverse_future_account_info_t::position_t res{};

    __BINAPI_GET(symbol);
    __BINAPI_GET(positionAmt);
    __BINAPI_GET(initialMargin);
    __BINAPI_GET(maintMargin);
    __BINAPI_GET(unrealizedProfit);
    __BINAPI_GET(positionInitialMargin);
    __BINAPI_GET(openOrderInitialMargin);
    __BINAPI_GET(leverage);
    __BINAPI_GET(isolated);
    __BINAPI_GET(positionSide);
    __BINAPI_GET(entryPrice);
    __BINAPI_GET(breakEvenPrice);
    __BINAPI_GET(maxQty);
    __BINAPI_GET(updateTime);

    return res;
}

std::ostream &operator<<(std::ostream &os, const inverse_future_account_info_t::position_t &o) {
    os
    << "{"
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"positionAmt\":\"" << o.positionAmt << "\","
    << "\"initialMargin\":\"" << o.initialMargin << "\","
    << "\"maintMargin\":\"" << o.maintMargin << "\","
    << "\"unrealizedProfit\":\"" << o.unrealizedProfit << "\","
    << "\"positionInitialMargin\":\"" << o.positionInitialMargin << "\","
    << "\"openOrderInitialMargin\":\"" << o.openOrderInitialMargin << "\","
    << "\"leverage\":\"" << o.leverage << "\","
    << "\"isolated\":\"" << o.isolated << "\","
    << "\"positionSide\":\"" << o.positionSide << "\","
    << "\"entryPrice\":\"" << o.entryPrice << "\","
    << "\"breakEvenPrice\":\"" << o.breakEvenPrice << "\","
    << "\"maxQty\":\"" << o.maxQty << "\","
    << "\"updateTime\":\"" << o.updateTime << "\""
    << "}";

    return os;
}

inverse_future_account_info_t inverse_future_account_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    inverse_future_account_info_t res{};

    __BINAPI_GET(canDeposit);
    __BINAPI_GET(canTrade);
    __BINAPI_GET(canWithdraw);
    __BINAPI_GET(feeTier);
    __BINAPI_GET(updateTime);
    
    const auto asset_arr = json.at("asset");
    assert(asset_arr.is_array());
    for ( auto idx = 0u; idx < asset_arr.size(); ++idx ) {

        const auto it = asset_arr.at(idx);

        asset_t asset = asset_t::construct(it);
        std::string symbol = asset.asset;

        res.assets.emplace(std::move(symbol), std::move(asset));
    }

    const auto position_arr = json.at("positions");
    assert(position_arr.is_array());
    for ( auto idx = 0u; idx < position_arr.size(); ++idx ) {

        const auto it = position_arr.at(idx);

        position_t position = position_t::construct(it);
        std::string symbol = position.symbol;

        res.positions.emplace(std::move(symbol), std::move(position));
    }

    return res;
}

const inverse_future_account_info_t::asset_t& inverse_future_account_info_t::get_asset(const char *asset) const {
    auto it = assets.find(asset);
    if ( it != assets.end() ) {
        return it->second;
    }

    assert(!"unreachable");
}

const inverse_future_account_info_t::position_t& inverse_future_account_info_t::get_position(const char *symbol) const {
    auto it = positions.find(symbol);
    if ( it != positions.end() ) {
        return it->second;
    }

    assert(!"unreachable");
}

std::ostream &operator<<(std::ostream &os, const inverse_future_account_info_t &o) {
    os
    << "{"
    << "\"canDeposit\":" << o.canDeposit << ","
    << "\"canTrade\":" << o.canTrade << ","
    << "\"canWithdraw\":" << o.canWithdraw << ","
    << "\"feeTier\":" << o.feeTier << ","
    << "\"updateTime\":" << o.updateTime << ","
    << "\"assets\":[";
    for ( auto it = o.assets.begin(); it != o.assets.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != o.assets.end() ) {
            os << ",";
        }
    }
    os << "\"positions\":[";
    for ( auto it = o.positions.begin(); it != o.positions.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != o.positions.end() ) {
            os << ",";
        }
    }
    os << "]}";

    return os;
}

/*************************************************************************************************/

std::ostream& operator<<(std::ostream &os, const exchange_info_t::rate_limit_t &o) {
    os
    << "{"
    << "\"rateLimitType\":\"" << o.rateLimitType << "\","
    << "\"interval\":\"" << o.interval << "\","
    << "\"limit\":" << o.limit
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const exchange_info_t::symbol_t::filter_t::price_t &o) {
    os
    << "{"
    << "\"filterType\":\"PRICE_FILTER\","
    << "\"minPrice\":\"" << o.minPrice << "\","
    << "\"maxPrice\":\"" << o.maxPrice << "\","
    << "\"tickSize\":\"" << o.tickSize << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const exchange_info_t::symbol_t::filter_t::percent_price_t &o) {
    os
    << "{"
    << "\"filterType\":\"PERCENT_PRICE\","
    << "\"multiplierUp\":\"" << o.multiplierUp << "\","
    << "\"multiplierDown\":\"" << o.multiplierDown << "\","
    << "\"avgPriceMins\":" << o.avgPriceMins << ""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const exchange_info_t::symbol_t::filter_t::percent_price_by_side_t &o) {
    os
    << "{"
    << "\"filterType\":\"PERCENT_PRICE_BY_SIDE\","
    << "\"bidMultiplierUp\":\"" << o.bidMultiplierUp << "\","
    << "\"bidMultiplierDown\":\"" << o.bidMultiplierDown << "\","
    << "\"askMultiplierUp\":\"" << o.askMultiplierUp << "\","
    << "\"askMultiplierDown\":\"" << o.askMultiplierDown << "\","
    << "\"avgPriceMins\":" << o.avgPriceMins << ""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const exchange_info_t::symbol_t::filter_t::lot_size_t &o) {
    os
    << "{"
    << "\"filterType\":\"LOT_SIZE\","
    << "\"minQty\":\"" << o.minQty << "\","
    << "\"maxQty\":\"" << o.maxQty << "\","
    << "\"stepSize\":\"" << o.stepSize << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const exchange_info_t::symbol_t::filter_t::market_lot_size_t &o) {
    os
    << "{"
    << "\"filterType\":\"MARKET_LOT_SIZE\","
    << "\"minQty\":\"" << o.minQty << "\","
    << "\"maxQty\":\"" << o.maxQty << "\","
    << "\"stepSize\":\"" << o.stepSize << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const exchange_info_t::symbol_t::filter_t::min_notional_t &o) {
    os
    << "{"
    << "\"filterType\":\"MIN_NOTIONAL\","
    << "\"minNotional\":\"" << o.minNotional << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const exchange_info_t::symbol_t::filter_t::iceberg_parts_t &o) {
    os
    << "{"
    << "\"filterType\":\"ICEBERG_PARTS\","
    << "\"limit\":" << o.limit
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const exchange_info_t::symbol_t::filter_t::max_num_orders_t &o) {
    os
    << "{"
    << "\"filterType\":\"MAX_NUM_ORDERS\","
    << "\"maxNumOrders\":" << o.maxNumOrders
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const exchange_info_t::symbol_t::filter_t::max_num_algo_orders_t &o) {
    os
    << "{"
    << "\"filterType\":\"MAX_NUM_ALGO_ORDERS\","
    << "\"maxNumAlgoOrders\":" << o.maxNumAlgoOrders
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const exchange_info_t::symbol_t::filter_t::max_position_t &o) {
    os
    << "{"
    << "\"filterType\":\"MAX_POSITION\","
    << "\"maxPosition\":\"" << o.maxPosition << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const exchange_info_t::symbol_t::filter_t::trailing_delta_t &o) {
    os
    << "{"
    << "\"filterType\":\"TRAILING_DELTA\","
    << "\"minTrailingAboveDelta\":\"" << o.minTrailingAboveDelta << "\","
    << "\"maxTrailingAboveDelta\":\"" << o.maxTrailingAboveDelta << "\","
    << "\"minTrailingBelowDelta\":\"" << o.minTrailingBelowDelta << "\","
    << "\"maxTrailingBelowDelta\":\"" << o.maxTrailingBelowDelta << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const exchange_info_t::symbol_t::filter_t &o) {
    static const auto visitor = [&os](const auto &o){ os << o; };
    boost::apply_visitor(visitor, o.filter);

    return os;
}

std::ostream &operator<<(std::ostream &os, const exchange_info_t::symbol_t &o) {
    os
    << "{"
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"status\":\"" << o.status << "\","
    << "\"baseAsset\":\"" << o.baseAsset << "\","
    << "\"baseAssetPrecision\":" << o.baseAssetPrecision << ","
    << "\"quoteAsset\":\"" << o.quoteAsset << "\","
    << "\"quotePrecision\":" << o.quotePrecision << ","
    << "\"orderTypes\":[";
    for ( auto it = o.orderTypes.begin(); it != o.orderTypes.end(); ++it ) {
        os << "\"" << *it << "\"";
        if ( std::next(it) != o.orderTypes.end() ) {
            os << ",";
        }
    }
    os
    << "],"
    << "\"icebergAllowed\":" << (o.icebergAllowed ? "true" : "false") << ","
    << "\"filters\":[";
    for ( auto it = o.filters.begin(); it != o.filters.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.filters.end() ) {
            os << ",";
        }
    }
    os
    << "]";

    os
    << "}";

    return os;
}

bool exchange_info_t::is_valid_symbol(const char *sym) const {
    return symbols.find(sym) != symbols.end();
}
const exchange_info_t::symbol_t& exchange_info_t::get_by_symbol(const char *sym) const {
    auto it = symbols.find(sym);
    if ( it != symbols.end() ) {
        return it->second;
    }

    assert(!"unreachable");
}

exchange_info_t exchange_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    exchange_info_t res{};
    __BINAPI_GET(timezone);
    __BINAPI_GET(serverTime);

    const auto filters = json.at("exchangeFilters");
    assert(filters.is_array());
    for ( auto idx = 0u; idx < filters.size(); ++idx ) {
        auto it = filters.at(idx).to_string();
        res.exchangeFilters.push_back(std::move(it));
    }

    const auto limits = json.at("rateLimits");
    assert(limits.is_array());
    for ( auto idx = 0u; idx < limits.size(); ++idx ) {
        exchange_info_t::rate_limit_t item{};
        const auto it = limits.at(idx);
        __BINAPI_GET2(item, rateLimitType, it);
        __BINAPI_GET2(item, interval, it);
        __BINAPI_GET2(item, limit, it);

        res.rateLimits.push_back(std::move(item));
    }

    const auto symbols = json.at("symbols");
    assert(symbols.is_array());
    for ( auto idx = 0u; idx < symbols.size(); ++idx ) {
        exchange_info_t::symbol_t sym{};
        const auto sit = symbols.at(idx);
        __BINAPI_GET2(sym, symbol, sit);
        __BINAPI_GET2(sym, status, sit);
        __BINAPI_GET2(sym, baseAsset, sit);
        __BINAPI_GET2(sym, baseAssetPrecision, sit);
        __BINAPI_GET2(sym, quoteAsset, sit);
        __BINAPI_GET2(sym, quotePrecision, sit);
        __BINAPI_GET2(sym, icebergAllowed, sit);
        const auto types = sit.at("orderTypes");
        assert(types.is_array());
        for ( auto idx2 = 0u; idx2 < types.size(); ++idx2 ) {
            auto oit = types.at(idx2).to_string();
            sym.orderTypes.push_back(std::move(oit));
        }

        const auto filters2 = sit.at("filters");
        assert(filters2.is_array());
        for ( auto idx2 = 0u; idx2 < filters2.size(); ++idx2 ) {
            exchange_info_t::symbol_t::filter_t filter{};
            const auto fit = filters2.at(idx2);
            __BINAPI_GET2(filter, filterType, fit);

            const auto filter_hash = fnv1a(filter.filterType.c_str(), filter.filterType.size());
            switch ( filter_hash ) {
                case fnv1a("PRICE_FILTER"): {
                    exchange_info_t::symbol_t::filter_t::price_t item{};
                    __BINAPI_GET2(item, minPrice, fit);
                    __BINAPI_GET2(item, maxPrice, fit);
                    __BINAPI_GET2(item, tickSize, fit);
                    filter.filter = std::move(item);

                    break;
                }
                case fnv1a("PERCENT_PRICE"): {
                    exchange_info_t::symbol_t::filter_t::percent_price_t item{};
                    __BINAPI_GET2(item, multiplierUp, fit);
                    __BINAPI_GET2(item, multiplierDown, fit);
                    __BINAPI_GET2(item, avgPriceMins, fit);
                    filter.filter = std::move(item);

                    break;
                }
                case fnv1a("PERCENT_PRICE_BY_SIDE"): {
                    exchange_info_t::symbol_t::filter_t::percent_price_by_side_t item{};
                    __BINAPI_GET2(item, bidMultiplierUp, fit);
                    __BINAPI_GET2(item, bidMultiplierDown, fit);
                    __BINAPI_GET2(item, askMultiplierUp, fit);
                    __BINAPI_GET2(item, askMultiplierDown, fit);
                    __BINAPI_GET2(item, avgPriceMins, fit);
                    filter.filter = std::move(item);

                    break;
                }
                case fnv1a("LOT_SIZE"): {
                    exchange_info_t::symbol_t::filter_t::lot_size_t item{};
                    __BINAPI_GET2(item, minQty, fit);
                    __BINAPI_GET2(item, maxQty, fit);
                    __BINAPI_GET2(item, stepSize, fit);
                    filter.filter = std::move(item);

                    break;
                }
                case fnv1a("MARKET_LOT_SIZE"): {
                    exchange_info_t::symbol_t::filter_t::market_lot_size_t item{};
                    __BINAPI_GET2(item, minQty, fit);
                    __BINAPI_GET2(item, maxQty, fit);
                    __BINAPI_GET2(item, stepSize, fit);
                    filter.filter = std::move(item);

                    break;
                }
                case fnv1a("MIN_NOTIONAL"): {
                    exchange_info_t::symbol_t::filter_t::min_notional_t item{};
                    __BINAPI_GET2(item, minNotional, fit);
                    filter.filter = std::move(item);

                    break;
                }
                case fnv1a("ICEBERG_PARTS"): {
                    exchange_info_t::symbol_t::filter_t::iceberg_parts_t item{};
                    __BINAPI_GET2(item, limit, fit);
                    filter.filter = std::move(item);

                    break;
                }
                case fnv1a("MAX_NUM_ORDERS"): {
                    exchange_info_t::symbol_t::filter_t::max_num_orders_t item{};
                    __BINAPI_GET2(item, maxNumOrders, fit);
                    filter.filter = std::move(item);

                    break;
                }
                case fnv1a("MAX_NUM_ALGO_ORDERS"): {
                    exchange_info_t::symbol_t::filter_t::max_num_algo_orders_t item{};
                    __BINAPI_GET2(item, maxNumAlgoOrders, fit);
                    filter.filter = std::move(item);

                    break;
                }
                case fnv1a("MAX_POSITION"): {
                    exchange_info_t::symbol_t::filter_t::max_position_t item{};
                    __BINAPI_GET2(item, maxPosition, fit);
                    filter.filter = std::move(item);

                    break;
                }
                case fnv1a("TRAILING_DELTA"): {
                    exchange_info_t::symbol_t::filter_t::trailing_delta_t item{};
                    __BINAPI_GET2(item, minTrailingAboveDelta, fit);
                    __BINAPI_GET2(item, maxTrailingAboveDelta, fit);
                    __BINAPI_GET2(item, minTrailingBelowDelta, fit);
                    __BINAPI_GET2(item, maxTrailingBelowDelta, fit);
                    filter.filter = std::move(item);

                    break;
                }
                default: assert("bad filterType" == nullptr);
            }

            sym.filters.push_back(std::move(filter));
        }
        std::string symbol = sym.symbol;
        res.symbols.emplace(std::move(symbol), std::move(sym));
    }

    return res;
}

std::ostream& operator<<(std::ostream &os, const exchange_info_t &o) {
    os
    << "{"
    << "\"timezone\":\"" << o.timezone << "\","
    << "\"serverTime\":" << o.serverTime << ","
    << "\"exchangeFilters\":[";
    for ( auto it = o.exchangeFilters.begin(); it != o.exchangeFilters.end(); ++it ) {
        os << "\"" << *it << "\"";
        if ( std::next(it) != o.exchangeFilters.end() ) {
            os << ",";
        }
    }
    os
    << "],"
    << "\"rateLimits\":[";
    for ( auto it = o.rateLimits.begin(); it != o.rateLimits.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.rateLimits.end() ) {
            os << ",";
        }
    }
    os
    << "],"
    << "\"symbols\":[";
    for ( auto it = o.symbols.begin(); it != o.symbols.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != o.symbols.end() ) {
            os << ",";
        }
    }
    os
    << "]";

    os << "}";

    return os;
}

/*************************************************************************************************/

std::ostream& operator<<(std::ostream &os, const options_exchange_info_t::rate_limit_t &o) {
    os
    << "{"
    << "\"rateLimitType\":\"" << o.rateLimitType << "\","
    << "\"interval\":\"" << o.interval << "\","
    << "\"intervalNum\":\"" << o.intervalNum << "\","
    << "\"limit\":" << o.limit
    << "}";

    return os;
}

std::ostream& operator<<(std::ostream &os, const options_exchange_info_t::option_contract_t &o) {
    os
    << "{"
    << "\"baseAsset\":\"" << o.baseAsset << "\","
    << "\"quoteAsset\":\"" << o.quoteAsset << "\","
    << "\"underlying\":" << o.underlying
    << "\"settleAsset\":" << o.settleAsset
    << "}";

    return os;
}

std::ostream& operator<<(std::ostream &os, const options_exchange_info_t::option_asset_t &o) {
    os
    << "{"
    << "\"name\":\"" << o.name
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const options_exchange_info_t::option_symbol_t::filter_t::price_t &o) {
    os
    << "{"
    << "\"filterType\":\"PRICE_FILTER\","
    << "\"minPrice\":\"" << o.minPrice << "\","
    << "\"maxPrice\":\"" << o.maxPrice << "\","
    << "\"tickSize\":\"" << o.tickSize << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const options_exchange_info_t::option_symbol_t::filter_t::lot_size_t &o) {
    os
    << "{"
    << "\"filterType\":\"LOT_SIZE\","
    << "\"minQty\":\"" << o.minQty << "\","
    << "\"maxQty\":\"" << o.maxQty << "\","
    << "\"stepSize\":\"" << o.stepSize << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const options_exchange_info_t::option_symbol_t::filter_t &o) {
    static const auto visitor = [&os](const auto &o){ os << o; };
    boost::apply_visitor(visitor, o.filter);

    return os;
}

std::ostream &operator<<(std::ostream &os, const options_exchange_info_t::option_symbol_t &o) {

    os
    << "{"
    << "\"contractId\":\"" << o.contractId << "\","
    << "\"expiryDate\":\"" << o.expiryDate << "\","
	<< "\"id\":\"" << o.id << "\","
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"strikePrice\":\"" << o.strikePrice << "\","
    << "\"underlying\":\"" << o.underlying << "\","
    << "\"unit\":\"" << o.unit << "\","
    << "\"makerFeeRate\":\"" << o.makerFeeRate << "\","
    << "\"takerFeeRate\":\"" << o.takerFeeRate << "\","
    << "\"minQty\":\"" << o.minQty << "\","
    << "\"maxQty\":\"" << o.maxQty << "\","
    << "\"initialMargin\":\"" << o.initialMargin << "\","
    << "\"maintenanceMargin\":\"" << o.maintenanceMargin << "\","
    << "\"minInitialMargin\":\"" << o.minInitialMargin << "\","
    << "\"minMaintenanceMargin\":\"" << o.minMaintenanceMargin << "\","
    << "\"priceScale\":\"" << o.priceScale << "\","
    << "\"quantityScale\":\"" << o.quantityScale << "\","
    << "\"quoteAsset\":\"" << o.quoteAsset << "\","
    << "\"filters\":[";
    for ( auto it = o.filters.begin(); it != o.filters.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.filters.end() ) {
            os << ",";
        }
    }
    os
    << "]";

    os
    << "}";

    return os;
}

bool options_exchange_info_t::is_valid_symbol(const char *sym) const {
    return symbols.find(sym) != symbols.end();
}
const options_exchange_info_t::option_symbol_t& options_exchange_info_t::get_by_symbol(const char *sym) const {
    auto it = symbols.find(sym);
    if ( it != symbols.end() ) {
        return it->second;
    }

    assert(!"unreachable");
}

options_exchange_info_t options_exchange_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    options_exchange_info_t res{};
    __BINAPI_GET(timezone);
    __BINAPI_GET(serverTime);

    const auto option_contracts = json.at("optionContracts");
    assert(option_contracts.is_array());
    for ( auto idx = 0u; idx < option_contracts.size(); ++idx ) {
        options_exchange_info_t::option_contract_t item{};
        const auto it = option_contracts.at(idx);
        __BINAPI_GET2(item, baseAsset, it);
        __BINAPI_GET2(item, quoteAsset, it);
        __BINAPI_GET2(item, underlying, it);
        __BINAPI_GET2(item, settleAsset, it);
        res.optionContracts.emplace_back(std::move(item));
    }

    const auto option_assets = json.at("optionAssets");
    assert(option_assets.is_array());
    for ( auto idx = 0u; idx < option_assets.size(); ++idx ) {
        options_exchange_info_t::option_asset_t item{};
        const auto it = option_assets.at(idx);
        __BINAPI_GET2(item, name, it);
        res.optionAssets.emplace_back(std::move(item));
    }

    const auto limits = json.at("rateLimits");
    assert(limits.is_array());
    for ( auto idx = 0u; idx < limits.size(); ++idx ) {
        options_exchange_info_t::rate_limit_t item{};
        const auto it = limits.at(idx);
        __BINAPI_GET2(item, rateLimitType, it);
        __BINAPI_GET2(item, interval, it);
        __BINAPI_GET2(item, intervalNum, it);
        __BINAPI_GET2(item, limit, it);

        res.rateLimits.emplace_back(std::move(item));
    }

    const auto symbols = json.at("optionSymbols");
    assert(symbols.is_array());
    for ( auto idx = 0u; idx < symbols.size(); ++idx ) {
        options_exchange_info_t::option_symbol_t sym{};
        const auto sit = symbols.at(idx);
        __BINAPI_GET2(sym, expiryDate, sit);
        __BINAPI_GET2(sym, symbol, sit);
        __BINAPI_GET2(sym, side, sit);
        __BINAPI_GET2(sym, strikePrice, sit);
        __BINAPI_GET2(sym, underlying, sit);
        __BINAPI_GET2(sym, unit, sit);
        __BINAPI_GET2(sym, makerFeeRate, sit);
        __BINAPI_GET2(sym, takerFeeRate, sit);
        __BINAPI_GET2(sym, minQty, sit);
        __BINAPI_GET2(sym, maxQty, sit);
        __BINAPI_GET2(sym, initialMargin, sit);
        __BINAPI_GET2(sym, maintenanceMargin, sit);
        __BINAPI_GET2(sym, minInitialMargin, sit);
        __BINAPI_GET2(sym, minMaintenanceMargin, sit);
        __BINAPI_GET2(sym, priceScale, sit);
        __BINAPI_GET2(sym, quantityScale, sit);
        __BINAPI_GET2(sym, quoteAsset, sit);

        res.symbols.emplace(sym.symbol, std::move(sym));
    }

    return res;
}

std::ostream& operator<<(std::ostream &os, const options_exchange_info_t &o) {
    os
    << "{"
    << "\"timezone\":\"" << o.timezone << "\","
    << "\"serverTime\":" << o.serverTime << ","
    << "\"optionContracts\":[";
    for ( auto it = o.optionContracts.begin(); it != o.optionContracts.end(); ++it ) {
        os << "\"" << *it << "\"";
        if ( std::next(it) != o.optionContracts.end() ) {
            os << ",";
        }
    }
    os
    << "],"
    << "\"optionAssets\":[";
    for ( auto it = o.optionAssets.begin(); it != o.optionAssets.end(); ++it ) {
        os << "\"" << *it << "\"";
        if ( std::next(it) != o.optionAssets.end() ) {
            os << ",";
        }
    }
    os
    << "],"
    << "\"rateLimits\":[";
    for ( auto it = o.rateLimits.begin(); it != o.rateLimits.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.rateLimits.end() ) {
            os << ",";
        }
    }
    os
    << "],"
    << "\"optionSymbols\":[";
    for ( auto it = o.symbols.begin(); it != o.symbols.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != o.symbols.end() ) {
            os << ",";
        }
    }
    os
    << "]";

    os << "}";

    return os;
}

/*************************************************************************************************/

std::ostream& operator<<(std::ostream &os, const linear_future_exchange_info_t::rate_limit_t &o) {
    os
    << "{"
    << "\"rateLimitType\":\"" << o.rateLimitType << "\","
    << "\"interval\":\"" << o.interval << "\","
    << "\"intervalNum\":\"" << o.intervalNum << "\","
    << "\"limit\":" << o.limit
    << "}";

    return os;
}

std::ostream& operator<<(std::ostream &os, const linear_future_exchange_info_t::asset_t &o) {
    os
    << "{"
    << "\"name\":\"" << o.asset << "\","
    << "\"marginAvailable\":\"" << o.marginAvailable << "\","
    << "\"autoAssetExchange\":\"" << o.autoAssetExchange
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const linear_future_exchange_info_t::symbol_t::filter_t::price_t &o) {
    os
    << "{"
    << "\"filterType\":\"PRICE_FILTER\","
    << "\"minPrice\":\"" << o.minPrice << "\","
    << "\"maxPrice\":\"" << o.maxPrice << "\","
    << "\"tickSize\":\"" << o.tickSize << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const linear_future_exchange_info_t::symbol_t::filter_t::lot_size_t &o) {
    os
    << "{"
    << "\"filterType\":\"LOT_SIZE\","
    << "\"minQty\":\"" << o.minQty << "\","
    << "\"maxQty\":\"" << o.maxQty << "\","
    << "\"stepSize\":\"" << o.stepSize << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const linear_future_exchange_info_t::symbol_t::filter_t::market_lot_size_t &o) {
    os
    << "{"
    << "\"filterType\":\"MARKET_LOT_SIZE\","
    << "\"minQty\":\"" << o.minQty << "\","
    << "\"maxQty\":\"" << o.maxQty << "\","
    << "\"stepSize\":\"" << o.stepSize << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const linear_future_exchange_info_t::symbol_t::filter_t::min_notional_t &o) {
    os
    << "{"
    << "\"filterType\":\"MIN_NOTIONAL\","
    << "\"notional\":\"" << o.notional << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const linear_future_exchange_info_t::symbol_t::filter_t::max_num_orders_t &o) {
    os
    << "{"
    << "\"filterType\":\"MIN_NOTIONAL\","
    << "\"limit\":\"" << o.limit << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const linear_future_exchange_info_t::symbol_t::filter_t::max_num_algo_orders_t &o) {
    os
    << "{"
    << "\"filterType\":\"MAX_NUM_ALGO_ORDERS\","
    << "\"limit\":\"" << o.limit << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const linear_future_exchange_info_t::symbol_t::filter_t::percent_price_t &o) {
    os
    << "{"
    << "\"filterType\":\"MAX_NUM_ALGO_ORDERS\","
    << "\"multiplierUp\":\"" << o.multiplierUp << "\","
    << "\"multiplierDown\":\"" << o.multiplierDown << "\","
    << "\"multiplierDecimal\":\"" << o.multiplierDecimal << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const linear_future_exchange_info_t::symbol_t::filter_t &o) {
    static const auto visitor = [&os](const auto &o){ os << o; };
    boost::apply_visitor(visitor, o.filter);

    return os;
}

std::ostream &operator<<(std::ostream &os, const linear_future_exchange_info_t::symbol_t &o) {

    os
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"pair\":\"" << o.pair << "\","
    << "\"contractType\":\"" << o.contractType << "\","
    << "\"deliveryDate\":\"" << o.deliveryDate << "\","
    << "\"onboardDate\":\"" << o.onboardDate << "\","
    << "\"status\":\"" << o.status << "\","
    << "\"maintMarginPercent\":\"" << o.maintMarginPercent << "\","
    << "\"requiredMarginPercent\":\"" << o.requiredMarginPercent << "\","
    << "\"baseAsset\":\"" << o.baseAsset << "\","
    << "\"quoteAsset\":\"" << o.quoteAsset << "\","
    << "\"marginAsset\":\"" << o.marginAsset << "\","
    << "\"pricePrecision\":\"" << o.pricePrecision << "\","
    << "\"quantityPrecision\":\"" << o.quantityPrecision << "\","
    << "\"baseAssetPrecision\":\"" << o.baseAssetPrecision << "\","
    << "\"quotePrecision\":\"" << o.quotePrecision << "\","
    << "\"underlyingType\":\"" << o.underlyingType << "\","
    << "\"underlyingSubType\":[";
    for ( auto it = o.underlyingSubType.begin(); it != o.underlyingSubType.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.underlyingSubType.end() ) {
            os << ",";
        }
    }
    os
    << "]"
    << "\"triggerProtect\":\"" << o.triggerProtect << "\","
    << "\"liquidationFee\":\"" << o.liquidationFee << "\","
    << "\"marketTakeBound\":\"" << o.marketTakeBound << "\","

    << "\"filters\":[";
    for ( auto it = o.filters.begin(); it != o.filters.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.filters.end() ) {
            os << ",";
        }
    }
    os
    << "]"

    << "\"orderType\":[";
    for ( auto it = o.orderType.begin(); it != o.orderType.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.orderType.end() ) {
            os << ",";
        }
    }
    os
    << "]"

    << "\"timeInForce\":[";
    for ( auto it = o.timeInForce.begin(); it != o.timeInForce.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.timeInForce.end() ) {
            os << ",";
        }
    }
    os
    << "]"


    << "}";

    return os;
}

bool linear_future_exchange_info_t::is_valid_symbol(const char *sym) const {
    return symbols.find(sym) != symbols.end();
}
const linear_future_exchange_info_t::symbol_t& linear_future_exchange_info_t::get_by_symbol(const char *sym) const {
    auto it = symbols.find(sym);
    if ( it != symbols.end() ) {
        return it->second;
    }

    assert(!"unreachable");
}

linear_future_exchange_info_t linear_future_exchange_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    linear_future_exchange_info_t res{};
    __BINAPI_GET(timezone);
    __BINAPI_GET(serverTime);

    const auto assets = json.at("assets");
    assert(assets.is_array());
    for ( auto idx = 0u; idx < assets.size(); ++idx ) {
        linear_future_exchange_info_t::asset_t item{};
        const auto it = assets.at(idx);
        __BINAPI_GET2(item, asset, it);
        __BINAPI_GET2(item, marginAvailable, it);
        __BINAPI_GET2(item, autoAssetExchange, it);
        res.assets.emplace_back(std::move(item));
    }

    const auto limits = json.at("rateLimits");
    assert(limits.is_array());
    for ( auto idx = 0u; idx < limits.size(); ++idx ) {
        linear_future_exchange_info_t::rate_limit_t item{};
        const auto it = limits.at(idx);
        __BINAPI_GET2(item, rateLimitType, it);
        __BINAPI_GET2(item, interval, it);
        __BINAPI_GET2(item, intervalNum, it);
        __BINAPI_GET2(item, limit, it);

        res.rateLimits.emplace_back(std::move(item));
    }

    const auto symbols = json.at("symbols");
    assert(symbols.is_array());
    for ( auto idx = 0u; idx < symbols.size(); ++idx ) {
        linear_future_exchange_info_t::symbol_t sym{};
        const auto sit = symbols.at(idx);

        __BINAPI_GET2(sym, symbol, sit);
        __BINAPI_GET2(sym, pair, sit);
        __BINAPI_GET2(sym, contractType, sit);
        __BINAPI_GET2(sym, deliveryDate, sit);
        __BINAPI_GET2(sym, onboardDate, sit);
        __BINAPI_GET2(sym, status, sit);
        __BINAPI_GET2(sym, maintMarginPercent, sit);
        __BINAPI_GET2(sym, requiredMarginPercent, sit);
        __BINAPI_GET2(sym, baseAsset, sit);
        __BINAPI_GET2(sym, quoteAsset, sit);
        __BINAPI_GET2(sym, marginAsset, sit);
        __BINAPI_GET2(sym, pricePrecision, sit);
        __BINAPI_GET2(sym, quantityPrecision, sit);
        __BINAPI_GET2(sym, baseAssetPrecision, sit);
        __BINAPI_GET2(sym, quotePrecision, sit);
        __BINAPI_GET2(sym, underlyingType, sit);
        
        auto underlyingSubType = sit.at("underlyingSubType");
        assert(underlyingSubType.is_array());

        if(underlyingSubType.is_array())
        {
            for ( auto idx = 0u; idx < underlyingSubType.size(); ++idx ) 
            {
                auto type = underlyingSubType.at(idx);

                sym.underlyingSubType.push_back(type.to_string());
            }
        }
        
        __BINAPI_GET2(sym, triggerProtect, sit);
        __BINAPI_GET2(sym, liquidationFee, sit);
        __BINAPI_GET2(sym, marketTakeBound, sit);

        const auto filters = sit.at("filters");

        assert(filters.is_array());

        for ( auto idx = 0u; idx < filters.size(); ++idx ) 
        {
            const auto filter = filters.at(idx);

            const auto filter_type = filter.at("filterType");
            assert(filter_type.is_string());

            auto filter_type_str = filter_type.to_string();

            if(filter_type_str == "PRICE_FILTER")
            {
                symbol_t::filter_t::price_t price;
                __BINAPI_GET2(price, maxPrice, filter);
                __BINAPI_GET2(price, minPrice, filter);
                __BINAPI_GET2(price, tickSize, filter);

                symbol_t::filter_t filt;
                filt.filterType = filter_type_str;
                filt.filter = std::move(price);

                sym.filters.emplace_back(std::move(filt));
            }
            else if(filter_type_str == "LOT_SIZE")
            {
                symbol_t::filter_t::lot_size_t lot;
                __BINAPI_GET2(lot, maxQty, filter);
                __BINAPI_GET2(lot, minQty, filter);
                __BINAPI_GET2(lot, stepSize, filter);

                symbol_t::filter_t filt;
                filt.filterType = filter_type_str;
                filt.filter = std::move(lot);

                sym.filters.emplace_back(std::move(filt));
            }
            else if(filter_type_str == "MARKET_LOT_SIZE")
            {
                symbol_t::filter_t::market_lot_size_t max_lot_size;
                __BINAPI_GET2(max_lot_size, maxQty, filter);
                __BINAPI_GET2(max_lot_size, minQty, filter);
                __BINAPI_GET2(max_lot_size, stepSize, filter);

                symbol_t::filter_t filt;
                filt.filterType = filter_type_str;
                filt.filter = std::move(max_lot_size);

                sym.filters.emplace_back(std::move(filt));
            }
            else if(filter_type_str == "MAX_NUM_ORDERS")
            {
                symbol_t::filter_t::max_num_orders_t max_num_orders;
                __BINAPI_GET2(max_num_orders, limit, filter);

                symbol_t::filter_t filt;
                filt.filterType = filter_type_str;
                filt.filter = std::move(max_num_orders);

                sym.filters.emplace_back(std::move(filt));
            }
            else if(filter_type_str == "MAX_NUM_ALGO_ORDERS")
            {
                symbol_t::filter_t::max_num_orders_t max_num_algo_orders;
                __BINAPI_GET2(max_num_algo_orders, limit, filter);

                symbol_t::filter_t filt;
                filt.filterType = filter_type_str;
                filt.filter = std::move(max_num_algo_orders);

                sym.filters.emplace_back(std::move(filt));
            }
            else if(filter_type_str == "MIN_NOTIONAL")
            {
                symbol_t::filter_t::min_notional_t min_notional;
                __BINAPI_GET2(min_notional, notional, filter);

                symbol_t::filter_t filt;
                filt.filterType = filter_type_str;
                filt.filter = std::move(min_notional);

                sym.filters.emplace_back(std::move(filt));
            }
            else if(filter_type_str == "PERCENT_PRICE")
            {
                symbol_t::filter_t::percent_price_t percent_price;
                __BINAPI_GET2(percent_price, multiplierUp, filter);
                __BINAPI_GET2(percent_price, multiplierDown, filter);
                __BINAPI_GET2(percent_price, multiplierDecimal, filter);

                symbol_t::filter_t filt;
                filt.filterType = filter_type_str;
                filt.filter = std::move(percent_price);

                sym.filters.emplace_back(std::move(filt));
            }
        }

        res.symbols.emplace(sym.symbol, std::move(sym));
    }

    return res;
}

std::ostream& operator<<(std::ostream &os, const linear_future_exchange_info_t &o) {
    os
    << "{"
    << "\"timezone\":\"" << o.timezone << "\","
    << "\"serverTime\":" << o.serverTime << ","
    << "],"
    << "\"assets\":[";
    for ( auto it = o.assets.begin(); it != o.assets.end(); ++it ) {
        os << "\"" << *it << "\"";
        if ( std::next(it) != o.assets.end() ) {
            os << ",";
        }
    }
    os
    << "],"
    << "\"rateLimits\":[";
    for ( auto it = o.rateLimits.begin(); it != o.rateLimits.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.rateLimits.end() ) {
            os << ",";
        }
    }
    os
    << "],"
    << "\"symbols\":[";
    for ( auto it = o.symbols.begin(); it != o.symbols.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != o.symbols.end() ) {
            os << ",";
        }
    }
    os
    << "]";

    os << "}";

    return os;
}

/*************************************************************************************************/

std::ostream& operator<<(std::ostream &os, const inverse_future_exchange_info_t::rate_limit_t &o) {
    os
    << "{"
    << "\"rateLimitType\":\"" << o.rateLimitType << "\","
    << "\"interval\":\"" << o.interval << "\","
    << "\"intervalNum\":\"" << o.intervalNum << "\","
    << "\"limit\":" << o.limit
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const inverse_future_exchange_info_t::symbol_t::filter_t::price_t &o) {
    os
    << "{"
    << "\"filterType\":\"PRICE_FILTER\","
    << "\"minPrice\":\"" << o.minPrice << "\","
    << "\"maxPrice\":\"" << o.maxPrice << "\","
    << "\"tickSize\":\"" << o.tickSize << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const inverse_future_exchange_info_t::symbol_t::filter_t::lot_size_t &o) {
    os
    << "{"
    << "\"filterType\":\"LOT_SIZE\","
    << "\"minQty\":\"" << o.minQty << "\","
    << "\"maxQty\":\"" << o.maxQty << "\","
    << "\"stepSize\":\"" << o.stepSize << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const inverse_future_exchange_info_t::symbol_t::filter_t::market_lot_size_t &o) {
    os
    << "{"
    << "\"filterType\":\"MARKET_LOT_SIZE\","
    << "\"minQty\":\"" << o.minQty << "\","
    << "\"maxQty\":\"" << o.maxQty << "\","
    << "\"stepSize\":\"" << o.stepSize << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const inverse_future_exchange_info_t::symbol_t::filter_t::max_num_orders_t &o) {
    os
    << "{"
    << "\"filterType\":\"MIN_NOTIONAL\","
    << "\"limit\":\"" << o.limit << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const inverse_future_exchange_info_t::symbol_t::filter_t::percent_price_t &o) {
    os
    << "{"
    << "\"filterType\":\"MAX_NUM_ALGO_ORDERS\","
    << "\"multiplierUp\":\"" << o.multiplierUp << "\","
    << "\"multiplierDown\":\"" << o.multiplierDown << "\","
    << "\"multiplierDecimal\":\"" << o.multiplierDecimal << "\""
    << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const inverse_future_exchange_info_t::symbol_t::filter_t &o) {
    static const auto visitor = [&os](const auto &o){ os << o; };
    boost::apply_visitor(visitor, o.filter);

    return os;
}

std::ostream &operator<<(std::ostream &os, const inverse_future_exchange_info_t::symbol_t &o) {

    os
    << "\"liquidationFee\":\"" << o.liquidationFee << "\","
    << "\"marketTakeBound\":\"" << o.marketTakeBound << "\","
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"pair\":\"" << o.pair << "\","
    << "\"contractType\":\"" << o.contractType << "\","
    << "\"deliveryDate\":\"" << o.deliveryDate << "\","
    << "\"onboardDate\":\"" << o.onboardDate << "\","
    << "\"contractStatus\":\"" << o.contractStatus << "\","
    << "\"contractSize\":\"" << o.contractSize << "\","
    << "\"quoteAsset\":\"" << o.quoteAsset << "\","
    << "\"baseAsset\":\"" << o.baseAsset << "\","
    << "\"marginAsset\":\"" << o.marginAsset << "\","
    << "\"pricePrecision\":\"" << o.pricePrecision << "\","
    << "\"quantityPrecision\":\"" << o.quantityPrecision << "\","
    << "\"baseAssetPrecision\":\"" << o.baseAssetPrecision << "\","
    << "\"quotePrecision\":\"" << o.quotePrecision << "\","
    << "\"equalQtyPrecision\":\"" << o.equalQtyPrecision << "\","
    << "\"triggerProtect\":\"" << o.triggerProtect << "\","
    << "\"maintMarginPercent\":\"" << o.maintMarginPercent << "\","
    << "\"requiredMarginPercent\":\"" << o.requiredMarginPercent << "\","
    << "\"underlyingType\":\"" << o.underlyingType << "\","

    << "\"filters\":[";
    for ( auto it = o.filters.begin(); it != o.filters.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.filters.end() ) {
            os << ",";
        }
    }
    os
    << "]"

    << "\"orderType\":[";
    for ( auto it = o.orderType.begin(); it != o.orderType.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.orderType.end() ) {
            os << ",";
        }
    }
    os
    << "]"

    << "\"timeInForce\":[";
    for ( auto it = o.timeInForce.begin(); it != o.timeInForce.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.timeInForce.end() ) {
            os << ",";
        }
    }
    os
    << "]"

    << "}";

    return os;
}

bool inverse_future_exchange_info_t::is_valid_symbol(const char *sym) const {
    return symbols.find(sym) != symbols.end();
}
const inverse_future_exchange_info_t::symbol_t& inverse_future_exchange_info_t::get_by_symbol(const char *sym) const {
    auto it = symbols.find(sym);
    if ( it != symbols.end() ) {
        return it->second;
    }

    assert(!"unreachable");
}

inverse_future_exchange_info_t inverse_future_exchange_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    inverse_future_exchange_info_t res{};
    __BINAPI_GET(timezone);
    __BINAPI_GET(serverTime);

    const auto limits = json.at("rateLimits");
    assert(limits.is_array());
    for ( auto idx = 0u; idx < limits.size(); ++idx ) {
        inverse_future_exchange_info_t::rate_limit_t item{};
        const auto it = limits.at(idx);
        __BINAPI_GET2(item, rateLimitType, it);
        __BINAPI_GET2(item, interval, it);
        __BINAPI_GET2(item, intervalNum, it);
        __BINAPI_GET2(item, limit, it);

        res.rateLimits.emplace_back(std::move(item));
    }

    const auto symbols = json.at("symbols");
    assert(symbols.is_array());
    for ( auto idx = 0u; idx < symbols.size(); ++idx ) {
        inverse_future_exchange_info_t::symbol_t sym{};
        const auto sit = symbols.at(idx);

        __BINAPI_GET2(sym,liquidationFee, sit);
        __BINAPI_GET2(sym,marketTakeBound, sit);
        __BINAPI_GET2(sym,symbol, sit);
        __BINAPI_GET2(sym,pair, sit);
        __BINAPI_GET2(sym,contractType, sit);
        __BINAPI_GET2(sym,deliveryDate, sit);
        __BINAPI_GET2(sym,onboardDate, sit);
        __BINAPI_GET2(sym,contractStatus, sit);
        __BINAPI_GET2(sym,contractSize, sit);
        __BINAPI_GET2(sym,quoteAsset, sit);
        __BINAPI_GET2(sym,baseAsset, sit);
        __BINAPI_GET2(sym,marginAsset, sit);
        __BINAPI_GET2(sym,pricePrecision, sit);
        __BINAPI_GET2(sym,quantityPrecision, sit);
        __BINAPI_GET2(sym,baseAssetPrecision, sit);
        __BINAPI_GET2(sym,quotePrecision, sit);
        __BINAPI_GET2(sym,equalQtyPrecision, sit);
        __BINAPI_GET2(sym,triggerProtect, sit);
        __BINAPI_GET2(sym,maintMarginPercent, sit);
        __BINAPI_GET2(sym,requiredMarginPercent, sit);
        __BINAPI_GET2(sym,underlyingType, sit);

        const auto filters = sit.at("filters");

        assert(filters.is_array());

        for ( auto idx = 0u; idx < filters.size(); ++idx ) 
        {
            const auto filter = filters.at(idx);

            const auto filter_type = filter.at("filterType");
            assert(filter_type.is_string());

            auto filter_type_str = filter_type.to_string();

            if(filter_type_str == "PRICE_FILTER")
            {
                symbol_t::filter_t::price_t price;
                __BINAPI_GET2(price, maxPrice, filter);
                __BINAPI_GET2(price, minPrice, filter);
                __BINAPI_GET2(price, tickSize, filter);

                symbol_t::filter_t filt;
                filt.filterType = filter_type_str;
                filt.filter = std::move(price);

                sym.filters.emplace_back(std::move(filt));
            }
            else if(filter_type_str == "LOT_SIZE")
            {
                symbol_t::filter_t::lot_size_t lot;
                __BINAPI_GET2(lot, maxQty, filter);
                __BINAPI_GET2(lot, minQty, filter);
                __BINAPI_GET2(lot, stepSize, filter);

                symbol_t::filter_t filt;
                filt.filterType = filter_type_str;
                filt.filter = std::move(lot);

                sym.filters.emplace_back(std::move(filt));
            }
            else if(filter_type_str == "MARKET_LOT_SIZE")
            {
                symbol_t::filter_t::market_lot_size_t max_lot_size;
                __BINAPI_GET2(max_lot_size, maxQty, filter);
                __BINAPI_GET2(max_lot_size, minQty, filter);
                __BINAPI_GET2(max_lot_size, stepSize, filter);

                symbol_t::filter_t filt;
                filt.filterType = filter_type_str;
                filt.filter = std::move(max_lot_size);

                sym.filters.emplace_back(std::move(filt));
            }
            else if(filter_type_str == "MAX_NUM_ORDERS")
            {
                symbol_t::filter_t::max_num_orders_t max_num_orders;
                __BINAPI_GET2(max_num_orders, limit, filter);

                symbol_t::filter_t filt;
                filt.filterType = filter_type_str;
                filt.filter = std::move(max_num_orders);

                sym.filters.emplace_back(std::move(filt));
            }
            else if(filter_type_str == "PERCENT_PRICE")
            {
                symbol_t::filter_t::percent_price_t percent_price;
                __BINAPI_GET2(percent_price, multiplierUp, filter);
                __BINAPI_GET2(percent_price, multiplierDown, filter);
                __BINAPI_GET2(percent_price, multiplierDecimal, filter);

                symbol_t::filter_t filt;
                filt.filterType = filter_type_str;
                filt.filter = std::move(percent_price);

                sym.filters.emplace_back(std::move(filt));
            }
        }

        res.symbols.emplace(sym.symbol, std::move(sym));
    }

    return res;
}

std::ostream& operator<<(std::ostream &os, const inverse_future_exchange_info_t &o) {
    os
    << "{"
    << "\"timezone\":\"" << o.timezone << "\","
    << "\"serverTime\":" << o.serverTime << ","
    << "],"

    << "\"rateLimits\":[";
    for ( auto it = o.rateLimits.begin(); it != o.rateLimits.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.rateLimits.end() ) {
            os << ",";
        }
    }
    os
    << "],"
    << "\"symbols\":[";
    for ( auto it = o.symbols.begin(); it != o.symbols.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != o.symbols.end() ) {
            os << ",";
        }
    }
    os
    << "]";

    os << "}";

    return os;
}

/*************************************************************************************************/

std::ostream &operator<<(std::ostream &os, const depths_t::depth_t &o) {
    os
    << "["
    << "\"" << o.price << "\","
    << "\"" << o.amount << "\","
    << "]";

    return os;
}

depths_t depths_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    depths_t res{};
    __BINAPI_GET(lastUpdateId);

    const auto bids = json.at("bids");
    assert(bids.is_array());
    for ( auto idx = 0u; idx < bids.size(); ++idx ) {
        depths_t::depth_t item{};
        const auto it = bids.at(idx);
        assert(it.is_array());
        item.price = new_qube::fixed_point<>(it.at(0).to_string()); //.assign(it.at(0).to_string());
        item.amount = new_qube::fixed_point<>(it.at(1).to_string()); // .assign(it.at(1).to_string());

        res.bids.push_back(std::move(item));
    }
    const auto asks = json.at("asks");
    assert(asks.is_array());
    for ( auto idx = 0u; idx < asks.size(); ++idx ) {
        depths_t::depth_t item{};
        const auto it = asks.at(idx);
        assert(it.is_array());
        item.price = new_qube::fixed_point<>(it.at(0).to_string()); //.assign(it.at(0).to_string());
        item.amount = new_qube::fixed_point<>(it.at(1).to_string()); // .assign(it.at(1).to_string());

        res.asks.push_back(std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const depths_t &o) {
    os
    << "{"
    << "\"lastUpdateId\":" << o.lastUpdateId << ","
    << "\"bids\":[";
    for ( auto it = o.bids.begin(); it != o.bids.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.bids.end() ) {
            os << ",";
        }
    }
    os
    << "],"
    << "\"asks\":[";
    for ( auto it = o.asks.begin(); it != o.asks.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.asks.end() ) {
            os << ",";
        }
    }

    os << "]}";

    return os;
}

/*************************************************************************************************/

/*static*/ option_depths_t option_depths_t::construct(const flatjson::fjson& json)
{
    assert(json.is_valid());

    option_depths_t res{};
    __BINAPI_GET(T);
    __BINAPI_GET(u);

    res.lastUpdateId = res.u;

    const auto bids = json.at("bids");
    assert(bids.is_array());
    for ( auto idx = 0u; idx < bids.size(); ++idx ) {
        depths_t::depth_t item{};
        const auto it = bids.at(idx);
        assert(it.is_array());
        item.price = new_qube::fixed_point<>(it.at(0).to_string()); //.assign(it.at(0).to_string());
        item.amount = new_qube::fixed_point<>(it.at(1).to_string()); // .assign(it.at(1).to_string());

        res.bids.push_back(std::move(item));
    }
    const auto asks = json.at("asks");
    assert(asks.is_array());
    for ( auto idx = 0u; idx < asks.size(); ++idx ) {
        depths_t::depth_t item{};
        const auto it = asks.at(idx);
        assert(it.is_array());
        item.price = new_qube::fixed_point<>(it.at(0).to_string()); //.assign(it.at(0).to_string());
        item.amount = new_qube::fixed_point<>(it.at(1).to_string()); // .assign(it.at(1).to_string());

        res.asks.push_back(std::move(item));
    }

    return res;
}

/*************************************************************************************************/

/*static*/ linear_future_depths_t linear_future_depths_t::construct(const flatjson::fjson& json)
{
    assert(json.is_valid());

    linear_future_depths_t res{};
    __BINAPI_GET(lastUpdateId);
    __BINAPI_GET(E);
    __BINAPI_GET(T);

    const auto bids = json.at("bids");
    assert(bids.is_array());
    for ( auto idx = 0u; idx < bids.size(); ++idx ) {
        depths_t::depth_t item{};
        const auto it = bids.at(idx);
        assert(it.is_array());
        item.price = new_qube::fixed_point<>(it.at(0).to_string()); //.assign(it.at(0).to_string());
        item.amount = new_qube::fixed_point<>(it.at(1).to_string()); // .assign(it.at(1).to_string());

        res.bids.push_back(std::move(item));
    }
    const auto asks = json.at("asks");
    assert(asks.is_array());
    for ( auto idx = 0u; idx < asks.size(); ++idx ) {
        depths_t::depth_t item{};
        const auto it = asks.at(idx);
        assert(it.is_array());
        item.price = new_qube::fixed_point<>(it.at(0).to_string()); //.assign(it.at(0).to_string());
        item.amount = new_qube::fixed_point<>(it.at(1).to_string()); // .assign(it.at(1).to_string());

        res.asks.push_back(std::move(item));
    }

    return res;
}

/*************************************************************************************************/

/*static*/ inverse_future_depths_t inverse_future_depths_t::construct(const flatjson::fjson& json)
{
    assert(json.is_valid());

    inverse_future_depths_t res{};
    __BINAPI_GET(lastUpdateId);
    __BINAPI_GET(symbol);
    __BINAPI_GET(pair);
    __BINAPI_GET(E);
    __BINAPI_GET(T);

    const auto bids = json.at("bids");
    assert(bids.is_array());
    for ( auto idx = 0u; idx < bids.size(); ++idx ) {
        depths_t::depth_t item{};
        const auto it = bids.at(idx);
        assert(it.is_array());
        item.price = new_qube::fixed_point<>(it.at(0).to_string()); //.assign(it.at(0).to_string());
        item.amount = new_qube::fixed_point<>(it.at(1).to_string()); // .assign(it.at(1).to_string());

        res.bids.push_back(std::move(item));
    }
    const auto asks = json.at("asks");
    assert(asks.is_array());
    for ( auto idx = 0u; idx < asks.size(); ++idx ) {
        depths_t::depth_t item{};
        const auto it = asks.at(idx);
        assert(it.is_array());
        item.price = new_qube::fixed_point<>(it.at(0).to_string()); //.assign(it.at(0).to_string());
        item.amount = new_qube::fixed_point<>(it.at(1).to_string()); // .assign(it.at(1).to_string());

        res.asks.push_back(std::move(item));
    }

    return res;
}

/*************************************************************************************************/

trades_t::trade_t trades_t::trade_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    trades_t::trade_t res{};
    const auto it = json.at(0);
    __BINAPI_GET2(res, id, it);
    __BINAPI_GET2(res, price, it);
    __BINAPI_GET2(res, qty, it);
    __BINAPI_GET2(res, time, it);
    __BINAPI_GET2(res, isBuyerMaker, it);
    __BINAPI_GET2(res, isBestMatch, it);

    return res;
}

std::ostream &operator<<(std::ostream &os, const trades_t::trade_t &o) {
    os
    << "{"
    << "\"id\":" << o.id << ","
    << "\"price\":\"" << o.price << "\","
    << "\"qty\":\"" << o.qty << "\","
    << "\"time\":" << o.time << ","
    << "\"isBuyerMaker\":" << (o.isBuyerMaker ? "true" : "false") << ","
    << "\"isBestMatch\":" << (o.isBestMatch ? "true" : "false")
    << "}";

    return os;
}

trades_t trades_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    trades_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        trades_t::trade_t item{};
        const auto it = json.at(idx);
        __BINAPI_GET2(item, id, it);
        __BINAPI_GET2(item, price, it);
        __BINAPI_GET2(item, qty, it);
        __BINAPI_GET2(item, time, it);
        __BINAPI_GET2(item, isBuyerMaker, it);
        __BINAPI_GET2(item, isBestMatch, it);

        res.trades.push_back(std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const trades_t &o) {
    os
    << "[";
    for ( auto it = o.trades.begin(); it != o.trades.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.trades.end() ) {
            os << ",";
        }
    }

    os << "]";

    return os;
}

option_trades_t::option_trade_t option_trades_t::option_trade_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    option_trades_t::option_trade_t res{};
    const auto it = json.at(0);
    __BINAPI_GET2(res, id, it);
    __BINAPI_GET2(res, price, it);
    __BINAPI_GET2(res, qty, it);
    __BINAPI_GET2(res, time, it);

    return res;
}

std::ostream &operator<<(std::ostream &os, const option_trades_t::option_trade_t &o) {
    os
    << "{"
    << "\"id\":" << o.id << ","
    << "\"price\":\"" << o.price << "\","
    << "\"qty\":\"" << o.qty << "\","
    << "\"time\":" << o.time << ","
    << "}";

    return os;
}

option_trades_t option_trades_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    option_trades_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        option_trades_t::option_trade_t item{};
        const auto it = json.at(idx);
        __BINAPI_GET2(item, id, it);
        __BINAPI_GET2(item, price, it);
        __BINAPI_GET2(item, qty, it);
        __BINAPI_GET2(item, time, it);

        res.trades.push_back(std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const option_trades_t &o) {
    os
    << "[";
    for ( auto it = o.trades.begin(); it != o.trades.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.trades.end() ) {
            os << ",";
        }
    }

    os << "]";

    return os;
}

/*************************************************************************************************/

agg_trades_t::agg_trade_t agg_trades_t::agg_trade_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    agg_trades_t::agg_trade_t res{};
    const auto it = json.at(0);
    __get_json(res.id, "a", it);
    __get_json(res.price, "p", it);
    __get_json(res.qty, "q", it);
    __get_json(res.first_id, "f", it);
    __get_json(res.last_id, "l", it);
    __get_json(res.time, "T", it);
    __get_json(res.isBuyerMaker, "m", it);
    __get_json(res.isBestMatch, "M", it);

    return res;
}

std::ostream &operator<<(std::ostream &os, const agg_trades_t::agg_trade_t &o) {
    os
    << "{"
    << "\"id\":" << o.id << ","
    << "\"price\":\"" << o.price << "\","
    << "\"qty\":\"" << o.qty << "\","
    << "\"first_id\":" << o.first_id << ","
    << "\"last_id\":" << o.last_id << ","
    << "\"time\":" << o.time << ","
    << "\"isBuyerMaker\":" << (o.isBuyerMaker ? "true" : "false") << ","
    << "\"isBestMatch\":" << (o.isBestMatch ? "true" : "false")
    << "}";

    return os;
}

/*************************************************************************************************/

agg_trades_t agg_trades_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    agg_trades_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        agg_trades_t::agg_trade_t item{};
        const auto it = json.at(idx);
        __get_json(item.id, "a", it);
        __get_json(item.price, "p", it);
        __get_json(item.qty, "q", it);
        __get_json(item.first_id, "f", it);
        __get_json(item.last_id, "l", it);
        __get_json(item.time, "T", it);
        __get_json(item.isBuyerMaker, "m", it);
        __get_json_def(item.isBestMatch, "M", it, false);

        res.trades.push_back(std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const agg_trades_t &o) {
    os
    << "[";
    for ( auto it = o.trades.begin(); it != o.trades.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.trades.end() ) {
            os << ",";
        }
    }

    os
    << "]";

    return os;
}

/*************************************************************************************************/

std::ostream &operator<<(std::ostream &os, const klines_t::kline_t &o) {
    os
    << "["
    << o.start_time << ","
    << "\"" << o.open << "\","
    << "\"" << o.high << "\","
    << "\"" << o.low << "\","
    << "\"" << o.close << "\","
    << "\"" << o.volume << "\","
    << o.end_time << ","
    << "\"" << o.quote_volume << "\","
    << o.num_trades << ","
    << "\"" << o.taker_buy_base_vol << "\","
    << "\"" << o.taker_buy_quote_vol << "\""
    << "]";

    return os;
}

klines_t klines_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    klines_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        kline_t item{};
        const auto it = json.at(idx);
        assert(it.is_array());
        item.start_time = it.at(0).to<std::size_t>(); // Open time
        item.open = new_qube::fixed_point<>(it.at(1).to_string()); // Open
        item.high = new_qube::fixed_point<>(it.at(2).to_string()); // High
        item.low = new_qube::fixed_point<>(it.at(3).to_string()); // Low
        item.close = new_qube::fixed_point<>(it.at(4).to_string()); // Close
        item.volume = new_qube::fixed_point<>(it.at(5).to_string()); // Volume
        item.end_time = it.at(6).to<std::size_t>(); // Close time
        item.quote_volume = new_qube::fixed_point<>(it.at(7).to_string()); // Quote asset volume
        item.num_trades = it.at(8).to<std::size_t>(); // Number of trades
        item.taker_buy_base_vol = new_qube::fixed_point<>(it.at(9).to_string()); // Taker buy base asset volume
        item.taker_buy_quote_vol = new_qube::fixed_point<>(it.at(10).to_string()); // Taker buy quote asset volume

        res.klines.push_back(std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const klines_t &o) {
    os
    << "[";
    for ( auto it = o.klines.begin(); it != o.klines.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.klines.end() ) {
            os << ",";
        }
    }
    os << "]";

    return os;
}

/*************************************************************************************************/

order_info_t order_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    order_info_t res{};
    __BINAPI_GET(symbol);
    __BINAPI_GET(orderId);
    __BINAPI_GET(clientOrderId);
    __BINAPI_GET(origClientOrderId);
    __BINAPI_GET(price);
    __BINAPI_GET(origQty);
    __BINAPI_GET(executedQty);
    __BINAPI_GET(cummulativeQuoteQty);
    __BINAPI_GET(status);
    __BINAPI_GET(timeInForce);
    __BINAPI_GET(type);
    __BINAPI_GET(side);
    __BINAPI_GET(stopPrice);
    __BINAPI_GET(icebergQty);
    __BINAPI_GET(time);
    __BINAPI_GET(updateTime);
    __BINAPI_GET(isWorking);

    return res;
}

std::ostream &operator<<(std::ostream &os, const order_info_t &o) {
    os
    << "{"
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"orderId\":" << o.orderId << ","
    << "\"clientOrderId\":\"" << o.clientOrderId << "\","
    << "\"origClientOrderId\":\"" << o.origClientOrderId << "\","
    << "\"price\":\"" << o.price << "\","
    << "\"origQty\":\"" << o.origQty << "\","
    << "\"executedQty\":\"" << o.executedQty << "\","
    << "\"cummulativeQuoteQty\":\"" << o.cummulativeQuoteQty << "\","
    << "\"status\":\"" << o.status << "\","
    << "\"timeInForce\":\"" << o.timeInForce << "\","
    << "\"type\":\"" << o.type << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"stopPrice\":\"" << o.stopPrice << "\","
    << "\"icebergQty\":\"" << o.icebergQty << "\","
    << "\"time\":" << o.time << ","
    << "\"updateTime\":" << o.updateTime << ","
    << "\"isWorking\":" << (o.isWorking ? "true" : "false")
    << "}";

    return os;
}

/*************************************************************************************************/

/*************************************************************************************************/

margin_order_info_t margin_order_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    margin_order_info_t res{};
    __BINAPI_GET(symbol);
    __BINAPI_GET(orderId);
    __BINAPI_GET(clientOrderId);
    __BINAPI_GET(price);
    __BINAPI_GET(origQty);
    __BINAPI_GET(executedQty);
    __BINAPI_GET(cummulativeQuoteQty);
    __BINAPI_GET(status);
    __BINAPI_GET(timeInForce);
    __BINAPI_GET(type);
    __BINAPI_GET(side);
    __BINAPI_GET(stopPrice);
    __BINAPI_GET(icebergQty);
    __BINAPI_GET(time);
    __BINAPI_GET(updateTime);
    __BINAPI_GET(isWorking);
    __BINAPI_GET(isIsolated);

    return res;
}

std::ostream &operator<<(std::ostream &os, const margin_order_info_t &o) {
    os
    << "{"
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"orderId\":" << o.orderId << ","
    << "\"clientOrderId\":\"" << o.clientOrderId << "\","
    << "\"price\":\"" << o.price << "\","
    << "\"origQty\":\"" << o.origQty << "\","
    << "\"executedQty\":\"" << o.executedQty << "\","
    << "\"cummulativeQuoteQty\":\"" << o.cummulativeQuoteQty << "\","
    << "\"status\":\"" << o.status << "\","
    << "\"timeInForce\":\"" << o.timeInForce << "\","
    << "\"type\":\"" << o.type << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"stopPrice\":\"" << o.stopPrice << "\","
    << "\"icebergQty\":\"" << o.icebergQty << "\","
    << "\"time\":" << o.time << ","
    << "\"updateTime\":" << o.updateTime << ","
    << "\"isWorking\":" << (o.isWorking ? "true" : "false")
    << "\"isIsolated\":" << (o.isIsolated ? "true" : "false")
    << "}";

    return os;
}

/*************************************************************************************************/

option_order_info_t option_order_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    option_order_info_t res{};

    __BINAPI_GET(orderId);
    __BINAPI_GET(symbol);
    __BINAPI_GET(price);
    __BINAPI_GET(quantity);
    __BINAPI_GET(executedQty);
    __BINAPI_GET(fee);
    __BINAPI_GET(side);
    __BINAPI_GET(type);
    __BINAPI_GET(timeInForce);
    __BINAPI_GET(reduceOnly);
    __BINAPI_GET(postOnly);
    __BINAPI_GET(createTime);
    __BINAPI_GET(updateTime);
    __BINAPI_GET(status);
    __BINAPI_GET(avgPrice);
    __BINAPI_GET(clientOrderId);
    __BINAPI_GET(priceScale);
    __BINAPI_GET(quantityScale);
    __BINAPI_GET(optionSide);
    __BINAPI_GET(quoteAsset);
    __BINAPI_GET(mmp);

    return res;
}

std::ostream &operator<<(std::ostream &os, const option_order_info_t &o) {
    os
    << "{"
    << "\"orderId\":\"" << o.orderId << "\","
    << "\"symbol\":" << o.symbol << ","
    << "\"price\":\"" << o.price << "\","
    << "\"quantity\":\"" << o.quantity << "\","
    << "\"executedQty\":\"" << o.executedQty << "\","
    << "\"fee\":\"" << o.fee << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"type\":\"" << o.type << "\","
    << "\"timeInForce\":\"" << o.timeInForce << "\","
    << "\"reduceOnly\":\"" << o.reduceOnly << "\","
    << "\"postOnly\":\"" << o.postOnly << "\","
    << "\"createTime\":\"" << o.createTime << "\","
    << "\"updateTime\":\"" << o.updateTime << "\","
    << "\"status\":" << o.status << ","
    << "\"avgPrice\":" << o.avgPrice << ","
    << "\"clientOrderId\":\"" << o.clientOrderId << "\","
    << "\"priceScale\":\"" << o.priceScale << "\","
    << "\"quantityScale\":\"" << o.quantityScale << "\","
    << "\"optionSide\":\"" << o.optionSide << "\","
    << "\"quoteAsset\":\"" << o.quoteAsset << "\","
    << "\"mmp\":" << o.mmp
    << "}";

    return os;
}

/*************************************************************************************************/

linear_future_order_info_t linear_future_order_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    linear_future_order_info_t res{};

    __BINAPI_GET(avgPrice);
  	__BINAPI_GET(clientOrderId);
  	__BINAPI_GET(cumQuote);
    __BINAPI_GET(executedQty);
    __BINAPI_GET(orderId);
  	__BINAPI_GET(origQty);
  	__BINAPI_GET(origType);
  	__BINAPI_GET(price);
    __BINAPI_GET(reduceOnly);
  	__BINAPI_GET(side);
  	__BINAPI_GET(positionSide);
  	__BINAPI_GET(status);
  	__BINAPI_GET(stopPrice);
  	__BINAPI_GET(closePosition);
  	__BINAPI_GET(symbol);
  	__BINAPI_GET(time);
  	__BINAPI_GET(timeInForce);
  	__BINAPI_GET(type);
  	__BINAPI_GET(activatePrice);
  	__BINAPI_GET(priceRate);
  	__BINAPI_GET(updateTime);
  	__BINAPI_GET(workingType);
  	__BINAPI_GET(priceProtect);
	__BINAPI_GET(priceMatch);
    __BINAPI_GET(selfTradePreventionMode);
    __BINAPI_GET(goodTillDate);

    return res;
}

std::ostream &operator<<(std::ostream &os, const linear_future_order_info_t &o) {
    os
    << "{"
    << "\"avgPrice\":\"" << o.avgPrice << "\","
    << "\"clientOrderId\":" << o.clientOrderId << ","
    << "\"cumQuote\":\"" << o.cumQuote << "\","
    << "\"executedQty\":\"" << o.executedQty << "\","
    << "\"orderId\":\"" << o.orderId << "\","
    << "\"origQty\":\"" << o.origQty << "\","
    << "\"origType\":\"" << o.origType << "\","
    << "\"price\":\"" << o.price << "\","
    << "\"reduceOnly\":\"" << o.reduceOnly << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"positionSide\":\"" << o.positionSide << "\","
    << "\"status\":\"" << o.status << "\","
    << "\"stopPrice\":\"" << o.stopPrice << "\","
    << "\"closePosition\":" << o.closePosition << ","
    << "\"symbol\":" << o.symbol << ","
    << "\"time\":\"" << o.time << "\","
    << "\"timeInForce\":\"" << o.timeInForce << "\","
    << "\"type\":\"" << o.type << "\","
    << "\"activatePrice\":\"" << o.activatePrice << "\","
    << "\"priceRate\":\"" << o.priceRate << "\","
    << "\"updateTime\":\"" << o.updateTime << "\","
    << "\"workingType\":\"" << o.workingType << "\","
    << "\"priceProtect\":\"" << o.priceProtect << "\","
    << "\"priceMatch\":\"" << o.priceMatch << "\","
    << "\"selfTradePreventionMode\":\"" << o.selfTradePreventionMode << "\","
    << "\"goodTillDate\":" << o.goodTillDate
    << "}";

    return os;
}

/*************************************************************************************************/

inverse_future_order_info_t inverse_future_order_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    inverse_future_order_info_t res{};

    __BINAPI_GET(avgPrice);
    __BINAPI_GET(clientOrderId);
    __BINAPI_GET(cumBase);
    __BINAPI_GET(executedQty);
    __BINAPI_GET(orderId);
    __BINAPI_GET(origQty);
    __BINAPI_GET(origType);
    __BINAPI_GET(price);
    __BINAPI_GET(reduceOnly);
    __BINAPI_GET(side);
    __BINAPI_GET(positionSide);
    __BINAPI_GET(status);
    __BINAPI_GET(stopPrice);
    __BINAPI_GET(closePosition);
    __BINAPI_GET(symbol);
    __BINAPI_GET(time);
    __BINAPI_GET(timeInForce);
    __BINAPI_GET(type);
    __BINAPI_GET(activatePrice);
    __BINAPI_GET(priceRate);
    __BINAPI_GET(updateTime);
    __BINAPI_GET(workingType);
    __BINAPI_GET(priceProtect);
    __BINAPI_GET(priceMatch);
    __BINAPI_GET(selfTradePreventionMode);

    return res;
}

std::ostream &operator<<(std::ostream &os, const inverse_future_order_info_t &o) {
    os
    << "{"
    << "\"avgPrice\":\"" << o.avgPrice << "\","
    << "\"clientOrderId\":" << o.clientOrderId << ","
    << "\"cumBase\":\"" << o.cumBase << "\","
    << "\"executedQty\":\"" << o.executedQty << "\","
    << "\"orderId\":\"" << o.orderId << "\","
    << "\"origQty\":\"" << o.origQty << "\","
    << "\"origType\":\"" << o.origType << "\","
    << "\"price\":\"" << o.price << "\","
    << "\"reduceOnly\":\"" << o.reduceOnly << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"positionSide\":\"" << o.positionSide << "\","
    << "\"status\":\"" << o.status << "\","
    << "\"stopPrice\":" << o.stopPrice << ","
    << "\"closePosition\":" << o.closePosition << ","
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"time\":\"" << o.time << "\","
    << "\"timeInForce\":\"" << o.timeInForce << "\","
    << "\"type\":\"" << o.type << "\","
    << "\"activatePrice\":\"" << o.activatePrice << "\","
    << "\"priceRate\":\"" << o.priceRate << "\","
    << "\"updateTime\":\"" << o.updateTime << "\","
    << "\"workingType\":\"" << o.workingType << "\","
    << "\"priceProtect\":\"" << o.priceProtect << "\","
    << "\"priceMatch\":\"" << o.priceMatch << "\","
    << "\"selfTradePreventionMode\":" << o.selfTradePreventionMode
    << "}";

    return os;
}

/*************************************************************************************************/

orders_info_t orders_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    orders_info_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        order_info_t item{};
        const auto it = json.at(idx);
        __BINAPI_GET2(item, symbol, it);
        __BINAPI_GET2(item, orderId, it);
        __BINAPI_GET2(item, clientOrderId, it);
        __BINAPI_GET2(item, price, it);
        __BINAPI_GET2(item, origQty, it);
        __BINAPI_GET2(item, executedQty, it);
        __BINAPI_GET2(item, cummulativeQuoteQty, it);
        __BINAPI_GET2(item, status, it);
        __BINAPI_GET2(item, timeInForce, it);
        __BINAPI_GET2(item, type, it);
        __BINAPI_GET2(item, side, it);
        __BINAPI_GET2(item, stopPrice, it);
        __BINAPI_GET2(item, icebergQty, it);
        __BINAPI_GET2(item, time, it);
        __BINAPI_GET2(item, updateTime, it);
        __BINAPI_GET2(item, isWorking, it);

        res.orders[item.symbol].push_back(std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const orders_info_t &o) {
    os
    << "[";
    for ( const auto &it: o.orders ) {
        for ( auto vit = it.second.begin(); vit != it.second.end(); ++vit ) {
            os << *vit;
            if ( std::next(vit) != it.second.end() ) {
                os << ",";
            }
        }
    }
    os
    << "]";

    return os;
}

/*************************************************************************************************/

/*************************************************************************************************/

margin_orders_info_t margin_orders_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    margin_orders_info_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        margin_order_info_t item{};
        const auto it = json.at(idx);
        __BINAPI_GET2(item, symbol, it);
        __BINAPI_GET2(item, orderId, it);
        __BINAPI_GET2(item, clientOrderId, it);
        __BINAPI_GET2(item, price, it);
        __BINAPI_GET2(item, origQty, it);
        __BINAPI_GET2(item, executedQty, it);
        __BINAPI_GET2(item, cummulativeQuoteQty, it);
        __BINAPI_GET2(item, status, it);
        __BINAPI_GET2(item, timeInForce, it);
        __BINAPI_GET2(item, type, it);
        __BINAPI_GET2(item, side, it);
        __BINAPI_GET2(item, stopPrice, it);
        __BINAPI_GET2(item, icebergQty, it);
        __BINAPI_GET2(item, time, it);
        __BINAPI_GET2(item, updateTime, it);
        __BINAPI_GET2(item, isWorking, it);
        __BINAPI_GET2(item, isIsolated, it);

        res.orders[item.symbol].push_back(std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const margin_orders_info_t &o) {
    os
    << "[";
    for ( const auto &it: o.orders ) {
        for ( auto vit = it.second.begin(); vit != it.second.end(); ++vit ) {
            os << *vit;
            if ( std::next(vit) != it.second.end() ) {
                os << ",";
            }
        }
    }
    os
    << "]";

    return os;
}

/*************************************************************************************************/

option_orders_info_t option_orders_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    option_orders_info_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        const auto it = json.at(idx);

        option_order_info_t item = option_order_info_t::construct(it);
        auto symbol = item.symbol;

        res.orders[symbol].emplace_back(std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const option_orders_info_t &o) {
    os
    << "[";
    for ( const auto &it: o.orders ) {
        for ( auto vit = it.second.begin(); vit != it.second.end(); ++vit ) {
            os << *vit;
            if ( std::next(vit) != it.second.end() ) {
                os << ",";
            }
        }
    }
    os
    << "]";

    return os;
}

/*************************************************************************************************/

linear_future_orders_info_t linear_future_orders_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    linear_future_orders_info_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        const auto it = json.at(idx);

        linear_future_order_info_t item = linear_future_order_info_t::construct(it);
        auto symbol = item.symbol;

        res.orders[symbol].emplace_back(std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const linear_future_orders_info_t &o) {
    os
    << "[";
    for ( const auto &it: o.orders ) {
        for ( auto vit = it.second.begin(); vit != it.second.end(); ++vit ) {
            os << *vit;
            if ( std::next(vit) != it.second.end() ) {
                os << ",";
            }
        }
    }
    os
    << "]";

    return os;
}

/*************************************************************************************************/

inverse_future_orders_info_t inverse_future_orders_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    inverse_future_orders_info_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        const auto it = json.at(idx);

        inverse_future_order_info_t item = inverse_future_order_info_t::construct(it);
        auto symbol = item.symbol;

        res.orders[symbol].emplace_back(std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const inverse_future_orders_info_t &o) {
    os
    << "[";
    for ( const auto &it: o.orders ) {
        for ( auto vit = it.second.begin(); vit != it.second.end(); ++vit ) {
            os << *vit;
            if ( std::next(vit) != it.second.end() ) {
                os << ",";
            }
        }
    }
    os
    << "]";

    return os;
}

/*************************************************************************************************/

new_order_info_ack_t new_order_info_ack_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    new_order_info_ack_t res{};
    __BINAPI_GET(symbol);
    __BINAPI_GET(orderId);
    __BINAPI_GET(clientOrderId);
    __BINAPI_GET(transactTime);

    return res;
}
std::ostream &operator<<(std::ostream &os, const new_order_info_ack_t &o) {
    os
    << "{"
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"orderId\":" << o.orderId << ","
    << "\"clientOrderId\":\"" << o.clientOrderId << "\","
    << "\"transactTime\":" << o.transactTime
    << "}";

    return os;
}

new_order_info_result_t new_order_info_result_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    new_order_info_result_t res{};
    __BINAPI_GET(symbol);
    __BINAPI_GET(orderId);
    __BINAPI_GET(clientOrderId);
    __BINAPI_GET(transactTime);
    __BINAPI_GET(price);
    __BINAPI_GET(origQty);
    __BINAPI_GET(executedQty);
    __BINAPI_GET(cummulativeQuoteQty);
    __BINAPI_GET(status);
    __BINAPI_GET(timeInForce);
    __BINAPI_GET(type);
    __BINAPI_GET(side);

    return res;
}
std::ostream &operator<<(std::ostream &os, const new_order_info_result_t &o) {
    os
    << "{"
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"orderId\":" << o.orderId << ","
    << "\"clientOrderId\":\"" << o.clientOrderId << "\","
    << "\"transactTime\":" << o.transactTime << ","
    << "\"price\":\"" << o.price << "\","
    << "\"origQty\":\"" << o.origQty << "\","
    << "\"executedQty\":\"" << o.executedQty << "\","
    << "\"cummulativeQuoteQty\":\"" << o.cummulativeQuoteQty << "\","
    << "\"status\":\"" << o.status << "\","
    << "\"timeInForce\":\"" << o.timeInForce << "\","
    << "\"type\":\"" << o.type << "\","
    << "\"side\":\"" << o.side << "\""
    << "}";

    return os;
}

double_type new_order_info_full_t::avg_price(const std::vector<fill_part> &parts) {
    double_type res{};
    for ( const auto &it: parts ) {
        res += it.price;
    }
    res /= parts.size();

    return res;
}
double_type new_order_info_full_t::max_price(const std::vector<fill_part> &parts) {
    double_type res{};
    for ( const auto &it: parts ) {
        res = res < it.price ? it.price : res;
    }

    return res;
}

double_type new_order_info_full_t::sum_amount(const std::vector<fill_part> &parts) {
    double_type res{};
    for ( const auto &it: parts ) {
        res += it.qty;
    }

    return res;
}

double_type new_order_info_full_t::sum_commission(const std::vector<fill_part> &parts) {
    double_type res{};
    for ( const auto &it: parts ) {
        res += it.commission;
    }

    return res;
}

new_order_info_full_t new_order_info_full_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    new_order_info_full_t res{};
    __BINAPI_GET(symbol);
    __BINAPI_GET(orderId);
    __BINAPI_GET(clientOrderId);
    __BINAPI_GET(transactTime);
    __BINAPI_GET(price);
    __BINAPI_GET(origQty);
    __BINAPI_GET(executedQty);
    __BINAPI_GET(cummulativeQuoteQty);
    __BINAPI_GET(status);
    __BINAPI_GET(timeInForce);
    __BINAPI_GET(type);
    __BINAPI_GET(side);
    __BINAPI_GET_DEFAULT(isIsolated, false);
    const auto fills = json.at("fills");
    for ( auto idx = 0u; idx < fills.size(); ++idx ) {
        new_order_info_full_t::fill_part item{};
        const auto it = fills.at(idx);
        __BINAPI_GET2(item, price, it);
        __BINAPI_GET2(item, qty, it);
        __BINAPI_GET2(item, commission, it);
        __BINAPI_GET2(item, commissionAsset, it);

        res.fills.push_back(std::move(item));
    }

    return res;
}
std::ostream &operator<<(std::ostream &os, const new_order_info_full_t &o) {
    os
    << "{"
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"orderId\":" << o.orderId << ","
    << "\"clientOrderId\":\"" << o.clientOrderId << "\","
    << "\"transactTime\":" << o.transactTime << ","
    << "\"price\":\"" << o.price << "\","
    << "\"origQty\":\"" << o.origQty << "\","
    << "\"executedQty\":\"" << o.executedQty << "\","
    << "\"cummulativeQuoteQty\":\"" << o.cummulativeQuoteQty << "\","
    << "\"status\":\"" << o.status << "\","
    << "\"timeInForce\":\"" << o.timeInForce << "\","
    << "\"type\":\"" << o.type << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"fills\":[";
    for ( auto it = o.fills.begin(); it != o.fills.end(); ++it ) {
        os
        << "{"
        << "\"price\":\"" << it->price << "\","
        << "\"qty\":\"" << it->qty << "\","
        << "\"commission\":\"" << it->commission << "\","
        << "\"commissionAsset\":\"" << it->commissionAsset << "\""
        << "}";
        if ( std::next(it) != o.fills.end() ) {
            os << ",";
        }
    }

    os << "]}";

    return os;
}

/*************************************************************************************************/

new_option_order_info_ack_t new_option_order_info_ack_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    new_option_order_info_ack_t res{};
    
    __BINAPI_GET(orderId);
    __BINAPI_GET(symbol);
    __BINAPI_GET(price);
    __BINAPI_GET(quantity);
    __BINAPI_GET(side);
    __BINAPI_GET(type);
    __BINAPI_GET(createDate);
    __BINAPI_GET(reduceOnly);
    __BINAPI_GET(postOnly);
    __BINAPI_GET(mmp);

    return res;
}
std::ostream &operator<<(std::ostream &os, const new_option_order_info_ack_t &o) {
    
    os
    << "{"
    << "\"orderId\":\"" << o.orderId << "\","
    << "\"symbol\":" << o.symbol << ","
    << "\"price\":\"" << o.price << "\","
    << "\"quantity\":\"" << o.quantity << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"type\":\"" << o.type << "\","
    << "\"createDate\":\"" << o.createDate << "\","
    << "\"reduceOnly\":\"" << o.reduceOnly << "\","
    << "\"postOnly\":\"" << o.postOnly << "\","
    << "\"mmp\":" << o.mmp
    << "}";

    return os;
}

new_option_order_info_result_t new_option_order_info_result_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    new_option_order_info_result_t res{};
    __BINAPI_GET(orderId);
    __BINAPI_GET(symbol);
    __BINAPI_GET(price);
    __BINAPI_GET(quantity);
    __BINAPI_GET(executedQty);
    __BINAPI_GET(fee);
    __BINAPI_GET(side);
    __BINAPI_GET(type);
    __BINAPI_GET(timeInForce);
    __BINAPI_GET(reduceOnly);
    __BINAPI_GET(postOnly);
    __BINAPI_GET(createTime);
    __BINAPI_GET(updateTime);
    __BINAPI_GET(status);
    __BINAPI_GET(avgPrice);
    __BINAPI_GET(clientOrderId);
    __BINAPI_GET(priceScale);
    __BINAPI_GET(quantityScale);
    __BINAPI_GET(optionSide);
    __BINAPI_GET(quoteAsset);
    __BINAPI_GET(mmp);

    return res;
}

std::ostream &operator<<(std::ostream &os, const new_option_order_info_result_t &o) {
    os
    << "{"
    << "\"orderId\":\"" << o.orderId << "\","
    << "\"symbol\":" << o.symbol << ","
    << "\"price\":\"" << o.price << "\","
    << "\"quantity\":" << o.quantity << ","
    << "\"executedQty\":\"" << o.executedQty << "\","
    << "\"fee\":\"" << o.fee << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"type\":\"" << o.type << "\","
    << "\"timeInForce\":\"" << o.timeInForce << "\","
    << "\"reduceOnly\":\"" << o.reduceOnly << "\","
    << "\"postOnly\":\"" << o.postOnly << "\","
    << "\"createTime\":\"" << o.createTime << "\","
    << "\"updateTime\":\"" << o.updateTime << "\","
    << "\"status\":\"" << o.status << "\","
    << "\"avgPrice\":\"" << o.avgPrice << "\","
    << "\"clientOrderId\":\"" << o.clientOrderId << "\","
    << "\"priceScale\":\"" << o.priceScale << "\","
    << "\"quantityScale\":\"" << o.quantityScale << "\","
    << "\"optionSide\":\"" << o.optionSide << "\","
    << "\"quoteAsset\":\"" << o.quoteAsset << "\","
    << "\"mmp\":\"" << o.mmp << "\""
    << "}";

    return os;
}

/*************************************************************************************************/

new_order_resp_type new_order_resp_type::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    if ( json.contains("fills") ) {
        // FULL
        new_order_info_full_t res = new_order_info_full_t::construct(json);
        return res;
    } else if ( json.contains("side") ) {
        // RESULT
        new_order_info_result_t res = new_order_info_result_t::construct(json);
        return res;
    } else if ( json.contains("symbol") ) {
        // ASK
        new_order_info_ack_t res = new_order_info_ack_t::construct(json);
        return res;
    } else if ( json.is_object() && json.size() == 0 ) {
        // TEST
        new_test_order_info_t res = new_test_order_info_t::construct(json);
        return res;
    }

    assert(!"unreachable");
}

std::ostream &operator<<(std::ostream &os, const new_order_resp_type &o) {
    if ( const auto *p = boost::get<new_order_info_ack_t>(&o) ) {
        return os << *p;
    } else if ( const auto *p = boost::get<new_order_info_result_t>(&o) ) {
        return os << *p;
    } else if ( const auto *p = boost::get<new_order_info_full_t>(&o) ) {
        return os << *p;
    }

    assert(!"unreachable");

    return os;
}

/*************************************************************************************************/

new_linear_future_order_info_ack_t construct(const flatjson::fjson &json)
{
    assert(json.is_valid());
    new_linear_future_order_info_ack_t res{};

    __BINAPI_GET(orderId);
    __BINAPI_GET(symbol);
    __BINAPI_GET(price);
    __BINAPI_GET(quantity);
    __BINAPI_GET(side);
    __BINAPI_GET(type);
    __BINAPI_GET(createDate);
    __BINAPI_GET(reduceOnly);
    __BINAPI_GET(postOnly);
    __BINAPI_GET(mmp);

    return res;
}

std::ostream &operator<<(std::ostream &os, const new_linear_future_order_info_ack_t &o)
{
    os
    << "{"
    << "\"orderId\":\"" << o.orderId << "\","
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"price\":\"" << o.price << "\","
    << "\"quantity\":\"" << o.quantity << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"type\":\"" << o.type << "\","
    << "\"createDate\":\"" << o.createDate << "\","
    << "\"reduceOnly\":\"" << o.reduceOnly << "\","
    << "\"postOnly\":\"" << o.postOnly << "\","
    << "\"mmp\":\"" << o.mmp << "\""
    << "}";

    return os;
}

new_inverse_future_order_info_ack_t new_inverse_future_order_info_ack_t::construct(const flatjson::fjson &json)
{
    assert(json.is_valid());
    new_inverse_future_order_info_ack_t res{};

    __BINAPI_GET(orderId);
    __BINAPI_GET(symbol);
    __BINAPI_GET(price);
    __BINAPI_GET(quantity);
    __BINAPI_GET(side);
    __BINAPI_GET(type);
    __BINAPI_GET(createDate);
    __BINAPI_GET(reduceOnly);
    __BINAPI_GET(postOnly);
    __BINAPI_GET(mmp);

    return res;
}

std::ostream &operator<<(std::ostream &os, const new_inverse_future_order_info_ack_t &o)
{
    os
    << "{"
    << "\"orderId\":\"" << o.orderId << "\","
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"price\":\"" << o.price << "\","
    << "\"quantity\":\"" << o.quantity << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"type\":\"" << o.type << "\","
    << "\"createDate\":\"" << o.createDate << "\","
    << "\"reduceOnly\":\"" << o.reduceOnly << "\","
    << "\"postOnly\":\"" << o.postOnly << "\","
    << "\"mmp\":\"" << o.mmp << "\""
    << "}";

    return os;
}

new_linear_future_order_info_result_t new_linear_future_order_info_result_t::construct(const flatjson::fjson &json)
{
    assert(json.is_valid());
    new_linear_future_order_info_result_t res{};

    __BINAPI_GET(clientOrderId);
    __BINAPI_GET(cumQty);
    __BINAPI_GET(cumQuote);
    __BINAPI_GET(executedQty);
    __BINAPI_GET(orderId);
    __BINAPI_GET(avgPrice);
    __BINAPI_GET(origQty);
    __BINAPI_GET(price);
    __BINAPI_GET(reduceOnly);
    __BINAPI_GET(side);
    __BINAPI_GET(positionSide);
    __BINAPI_GET(status);
    __BINAPI_GET(stopPrice);
    __BINAPI_GET(closePosition);
    __BINAPI_GET(symbol);
    __BINAPI_GET(timeInForce);
    __BINAPI_GET(type);
    __BINAPI_GET(origType);
    __BINAPI_GET(activatePrice);
    __BINAPI_GET(priceRate);
    __BINAPI_GET(updateTime);
    __BINAPI_GET(workingType);
    __BINAPI_GET(priceProtect);
    __BINAPI_GET(priceMatch);
    __BINAPI_GET(selfTradePreventionMode);
    __BINAPI_GET(goodTillDate);

    return res;
}

std::ostream &operator<<(std::ostream &os, const new_linear_future_order_info_result_t &o)
{
    os
    << "{"
    << "\"clientOrderId\":\"" << o.clientOrderId << "\","
    << "\"cumQty\":" << o.cumQty << ","
    << "\"cumQuote\":\"" << o.cumQuote << "\","
    << "\"executedQty\":" << o.executedQty << ","
    << "\"orderId\":\"" << o.orderId << "\","
    << "\"avgPrice\":\"" << o.avgPrice << "\","
    << "\"origQty\":\"" << o.origQty << "\","
    << "\"price\":\"" << o.price << "\","
    << "\"reduceOnly\":\"" << o.reduceOnly << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"positionSide\":\"" << o.positionSide << "\","
    << "\"status\":\"" << o.status << "\","
    << "\"stopPrice\":\"" << o.stopPrice << "\","
    << "\"closePosition\":\"" << o.closePosition << "\","
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"timeInForce\":\"" << o.timeInForce << "\","
    << "\"type\":\"" << o.type << "\","
    << "\"origType\":\"" << o.origType << "\","
    << "\"activatePrice\":\"" << o.activatePrice << "\","
    << "\"priceRate\":\"" << o.priceRate << "\","
    << "\"updateTime\":\"" << o.updateTime << "\","
    << "\"workingType\":\"" << o.workingType << "\","
    << "\"priceProtect\":\"" << o.priceProtect << "\","
    << "\"priceMatch\":\"" << o.priceMatch << "\","
    << "\"selfTradePreventionMode\":\"" << o.selfTradePreventionMode << "\","
    << "\"goodTillDate\":\"" << o.goodTillDate << "\""
    << "}";

    return os;
}

/*************************************************************************************************/

new_inverse_future_order_info_result_t new_inverse_future_order_info_result_t::construct(const flatjson::fjson &json)
{
    assert(json.is_valid());
    new_inverse_future_order_info_result_t res{};

    __BINAPI_GET(clientOrderId);
    __BINAPI_GET(cumQty);
    __BINAPI_GET(cumBase);
    __BINAPI_GET(executedQty);
    __BINAPI_GET(orderId);
    __BINAPI_GET(avgPrice);
    __BINAPI_GET(origQty);
    __BINAPI_GET(price);
    __BINAPI_GET(reduceOnly);
    __BINAPI_GET(side);
    __BINAPI_GET(positionSide);
    __BINAPI_GET(status);
    __BINAPI_GET(stopPrice);
    __BINAPI_GET(closePosition);
    __BINAPI_GET(symbol);
    __BINAPI_GET(pair);
    __BINAPI_GET(timeInForce);
    __BINAPI_GET(type);
    __BINAPI_GET(origType);
    __BINAPI_GET(activatePrice);
    __BINAPI_GET(priceRate);
    __BINAPI_GET(updateTime);
    __BINAPI_GET(workingType);
    __BINAPI_GET(priceProtect);
    __BINAPI_GET(priceMatch);
    __BINAPI_GET(selfTradePreventionMode);

    return res;
}

std::ostream &operator<<(std::ostream &os, const new_inverse_future_order_info_result_t &o)
{
    os
    << "{"
    << "\"clientOrderId\":\"" << o.clientOrderId << "\","
    << "\"cumQty\":" << o.cumQty << ","
    << "\"cumBase\":\"" << o.cumBase << "\","
    << "\"executedQty\":" << o.executedQty << ","
    << "\"orderId\":\"" << o.orderId << "\","
    << "\"avgPrice\":\"" << o.avgPrice << "\","
    << "\"origQty\":\"" << o.origQty << "\","
    << "\"price\":\"" << o.price << "\","
    << "\"reduceOnly\":\"" << o.reduceOnly << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"positionSide\":\"" << o.positionSide << "\","
    << "\"status\":\"" << o.status << "\","
    << "\"stopPrice\":\"" << o.stopPrice << "\","
    << "\"closePosition\":\"" << o.closePosition << "\","
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"pair\":\"" << o.pair << "\","
    << "\"timeInForce\":\"" << o.timeInForce << "\","
    << "\"type\":\"" << o.type << "\","
    << "\"origType\":\"" << o.origType << "\","
    << "\"activatePrice\":\"" << o.activatePrice << "\","
    << "\"priceRate\":\"" << o.priceRate << "\","
    << "\"updateTime\":\"" << o.updateTime << "\","
    << "\"workingType\":\"" << o.workingType << "\","
    << "\"priceProtect\":\"" << o.priceProtect << "\","
    << "\"priceMatch\":\"" << o.priceMatch << "\","
    << "\"selfTradePreventionMode\":\"" << o.selfTradePreventionMode << "\""
    << "}";

    return os;
}

/*************************************************************************************************/

new_option_order_resp_type new_option_order_resp_type::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    if ( json.contains("quoteAsset") ) {
        // RESULT
        new_option_order_info_result_t res = new_option_order_info_result_t::construct(json);
        return res;
    } else if ( json.contains("symbol") ) {
        // ASK
        new_option_order_info_ack_t res = new_option_order_info_ack_t::construct(json);
        return res;
    }

    assert(!"unreachable");
}

std::ostream &operator<<(std::ostream &os, const new_option_order_resp_type &o) {
    if ( const auto *p = boost::get<new_option_order_info_ack_t>(&o) ) {
        return os << *p;
    } else if ( const auto *p = boost::get<new_option_order_info_result_t>(&o) ) {
        return os << *p;
    } 

    assert(!"unreachable");

    return os;
}

/*************************************************************************************************/

new_test_order_info_t new_test_order_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());
    assert(json.is_object());

    new_test_order_info_t res{};
    res.ok = json.size() == 0;

    return res;
}

std::ostream &operator<<(std::ostream &os, const new_test_order_info_t &o) {
    os << "{\"ok\":" << (o.ok ? "true" : "false") << "}";

    return os;
}

/*************************************************************************************************/

cancel_order_info_t cancel_order_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    cancel_order_info_t res{};
    __BINAPI_GET(symbol);
    __BINAPI_GET(orderId);
    __BINAPI_GET(origClientOrderId);
    __BINAPI_GET(clientOrderId);
    __BINAPI_GET(price);
    __BINAPI_GET(origQty);
    __BINAPI_GET(executedQty);
    __BINAPI_GET(cummulativeQuoteQty);
    __BINAPI_GET(status);
    __BINAPI_GET(timeInForce);
    __BINAPI_GET(type);
    __BINAPI_GET(side);

    return res;
}

std::ostream &operator<<(std::ostream &os, const cancel_order_info_t &o) {
    os
    << "{"
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"orderId\":" << o.orderId << ","
    << "\"origClientOrderId\":\"" << o.origClientOrderId << "\","
    << "\"clientOrderId\":\"" << o.clientOrderId << "\","
    << "\"price\":\"" << o.price << "\","
    << "\"origQty\":\"" << o.origQty << "\","
    << "\"executedQty\":\"" << o.executedQty << "\","
    << "\"cummulativeQuoteQty\":\"" << o.cummulativeQuoteQty << "\","
    << "\"status\":\"" << o.status << "\","
    << "\"timeInForce\":\"" << o.timeInForce << "\","
    << "\"type\":\"" << o.type << "\","
    << "\"side\":\"" << o.side << "\""
    << "}";

    return os;
}

/*************************************************************************************************/

my_trades_info_t::my_trade_info_t my_trades_info_t::my_trade_info_t::construct(const flatjson::fjson &json) {
    my_trade_info_t res{};
    __BINAPI_GET(symbol);
    __BINAPI_GET(id);
    __BINAPI_GET(orderId);
    __BINAPI_GET(price);
    __BINAPI_GET(qty);
    __BINAPI_GET(commission);
    __BINAPI_GET(commissionAsset);
    __BINAPI_GET(time);
    __BINAPI_GET(isBuyer);
    __BINAPI_GET(isMaker);
    __BINAPI_GET(isBestMatch);

    return res;
}

std::ostream &operator<<(std::ostream &os, const my_trades_info_t::my_trade_info_t &o) {
    os
    << "{"
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"id\":" << o.id << ","
    << "\"orderId\":" << o.orderId << ","
    << "\"price\":\"" << o.price << "\","
    << "\"qty\":\"" << o.qty << "\","
    << "\"commission\":\"" << o.commission << "\","
    << "\"commissionAsset\":\"" << o.commissionAsset << "\","
    << "\"time\":" << o.time << ","
    << "\"isBuyer\":" << (o.isBuyer ? "true" : "false") << ","
    << "\"isMaker\":" << (o.isMaker ? "true" : "false") << ","
    << "\"isBestMatch\":" << (o.isBestMatch ? "true" : "false")
    << "}";

    return os;
}

/*************************************************************************************************/

my_trades_info_t my_trades_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    my_trades_info_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        my_trade_info_t item = my_trades_info_t::my_trade_info_t::construct(json.at(idx));
        res.trades.push_back(std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const my_trades_info_t &o) {
    os
    << "[";
    for ( auto it = o.trades.begin(); it != o.trades.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.trades.end() ) {
            os << ",";
        }
    }

    os
    << "]";

    return os;
}

/*************************************************************************************************/

my_option_trades_info_t::my_option_trade_info_t my_option_trades_info_t::my_option_trade_info_t::construct(const flatjson::fjson &json) {
    my_option_trade_info_t res{};

    __BINAPI_GET(id);
    __BINAPI_GET(tradeId);
    __BINAPI_GET(orderId);
    __BINAPI_GET(symbol);
    __BINAPI_GET(price);
    __BINAPI_GET(quantity);
    __BINAPI_GET(fee);
    __BINAPI_GET(realizedProfit);
    __BINAPI_GET(side);
    __BINAPI_GET(type);
    __BINAPI_GET(volatility);
    __BINAPI_GET(liquidity);
    __BINAPI_GET(quoteAsset);
    __BINAPI_GET(time);
    __BINAPI_GET(priceScale);
    __BINAPI_GET(quantityScale);
    __BINAPI_GET(optionSide);

    return res;
}

std::ostream &operator<<(std::ostream &os, const my_option_trades_info_t::my_option_trade_info_t &o) {
    os
    << "{"
    << "\"id\":\"" << o.id << "\","
    << "\"tradeId\":\"" << o.tradeId << "\","
    << "\"orderId\":\"" << o.orderId << "\","
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"price\":\"" << o.price << "\","
    << "\"quantity\":\"" << o.quantity << "\","
    << "\"fee\":\"" << o.fee << "\","
    << "\"realizedProfit\":\"" << o.realizedProfit << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"type\":\"" << o.type << "\","
    << "\"volatility\":\"" << o.volatility << "\","
    << "\"liquidity\":\"" << o.liquidity << "\","
    << "\"quoteAsset\":\"" << o.quoteAsset << "\","
    << "\"time\":\"" << o.time << "\","
    << "\"priceScale\":\"" << o.priceScale << "\","
    << "\"quantityScale\":\"" << o.quantityScale << "\","
    << "\"optionSide\":\"" << o.optionSide << "\","
    << "}";

    return os;
}

/*************************************************************************************************/

my_option_trades_info_t my_option_trades_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    my_option_trades_info_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        my_option_trade_info_t item = my_option_trades_info_t::my_option_trade_info_t::construct(json.at(idx));
        res.trades.push_back(std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const my_option_trades_info_t &o) {
    os
    << "[";
    for ( auto it = o.trades.begin(); it != o.trades.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.trades.end() ) {
            os << ",";
        }
    }

    os
    << "]";

    return os;
}


/*************************************************************************************************/

my_linear_future_trades_info_t::my_linear_future_trade_info_t my_linear_future_trades_info_t::my_linear_future_trade_info_t::construct(const flatjson::fjson &json) {
    my_linear_future_trade_info_t res{};

    __BINAPI_GET(buyer);
    __BINAPI_GET(commission);
    __BINAPI_GET(commissionAsset);
    __BINAPI_GET(id);
    __BINAPI_GET(maker);
    __BINAPI_GET(orderId);
    __BINAPI_GET(price);
    __BINAPI_GET(qty);
    __BINAPI_GET(quoteQty);
    __BINAPI_GET(realizedPnl);
    __BINAPI_GET(side);
    __BINAPI_GET(positionSide);
    __BINAPI_GET(symbol);
    __BINAPI_GET(time);

    return res;
}

std::ostream &operator<<(std::ostream &os, const my_linear_future_trades_info_t::my_linear_future_trade_info_t &o) {
    os
    << "{"
    << "\"buyer\":\"" << o.buyer << "\","
    << "\"commission\":\"" << o.commission << "\","
    << "\"commissionAsset\":\"" << o.commissionAsset << "\","
    << "\"id\":\"" << o.id << "\","
    << "\"maker\":\"" << o.maker << "\","
    << "\"orderId\":\"" << o.orderId << "\","
    << "\"price\":\"" << o.price << "\","
    << "\"qty\":\"" << o.qty << "\","
    << "\"quoteQty\":\"" << o.quoteQty << "\","
    << "\"realizedPnl\":\"" << o.realizedPnl << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"positionSide\":\"" << o.positionSide << "\","
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"time\":\"" << o.time
    << "}";

    return os;
}

/*************************************************************************************************/

my_linear_future_trades_info_t my_linear_future_trades_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    my_linear_future_trades_info_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        my_linear_future_trade_info_t item = my_linear_future_trades_info_t::my_linear_future_trade_info_t::construct(json.at(idx));
        res.trades.push_back(std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const my_linear_future_trades_info_t &o) {
    os
    << "[";
    for ( auto it = o.trades.begin(); it != o.trades.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.trades.end() ) {
            os << ",";
        }
    }

    os
    << "]";

    return os;
}

/*************************************************************************************************/

my_inverse_future_trades_info_t::my_inverse_future_trade_info_t my_inverse_future_trades_info_t::my_inverse_future_trade_info_t::construct(const flatjson::fjson &json) {
    my_inverse_future_trade_info_t res{};

    __BINAPI_GET(symbol);
    __BINAPI_GET(id);
    __BINAPI_GET(orderId);
    __BINAPI_GET(pair);
    __BINAPI_GET(side);
    __BINAPI_GET(price);
    __BINAPI_GET(qty);
    __BINAPI_GET(realizedPnl);
    __BINAPI_GET(marginAsset);
    __BINAPI_GET(baseQty);
    __BINAPI_GET(commission);
    __BINAPI_GET(commissionAsset);
    __BINAPI_GET(time);
    __BINAPI_GET(positionSide);
    __BINAPI_GET(buyer);
    __BINAPI_GET(maker);

    return res;
}

std::ostream &operator<<(std::ostream &os, const my_inverse_future_trades_info_t::my_inverse_future_trade_info_t &o) {
    os
    << "{"
    << "\"symbol\":\"" << o.symbol << "\","
    << "\"id\":\"" << o.id << "\","
    << "\"orderId\":\"" << o.orderId << "\","
    << "\"pair\":\"" << o.pair << "\","
    << "\"side\":\"" << o.side << "\","
    << "\"price\":\"" << o.price << "\","
    << "\"qty\":\"" << o.qty << "\","
    << "\"realizedPnl\":\"" << o.realizedPnl << "\","
    << "\"marginAsset\":\"" << o.marginAsset << "\","
    << "\"baseQty\":\"" << o.baseQty << "\","
    << "\"commission\":\"" << o.commission << "\","
    << "\"commissionAsset\":\"" << o.commissionAsset << "\","
    << "\"time\":\"" << o.time << "\","
    << "\"positionSide\":\"" << o.positionSide << "\","
    << "\"buyer\":\"" << o.buyer << "\","
    << "\"maker\":\"" << o.maker
    << "}";

    return os;
}

/*************************************************************************************************/

my_inverse_future_trades_info_t my_inverse_future_trades_info_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    my_inverse_future_trades_info_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        my_inverse_future_trade_info_t item = my_inverse_future_trades_info_t::my_inverse_future_trade_info_t::construct(json.at(idx));
        res.trades.push_back(std::move(item));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const my_inverse_future_trades_info_t &o) {
    os
    << "[";
    for ( auto it = o.trades.begin(); it != o.trades.end(); ++it ) {
        os << *it;
        if ( std::next(it) != o.trades.end() ) {
            os << ",";
        }
    }

    os
    << "]";

    return os;
}

/*************************************************************************************************/

start_user_data_stream_t start_user_data_stream_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    start_user_data_stream_t res{};
    __BINAPI_GET(listenKey);

    return res;
}

std::ostream &operator<<(std::ostream &os, const start_user_data_stream_t &o) {
    os
    << "{"
    << "\"listenKey\":\"" << o.listenKey << "\""
    << "}";

    return os;
}

/*************************************************************************************************/

ping_user_data_stream_t ping_user_data_stream_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    ping_user_data_stream_t res{};
    res.ok = json.is_object() && json.is_empty();

    return res;
}

std::ostream &operator<<(std::ostream &os, const ping_user_data_stream_t &o) {
    os
    << "{"
    << "\"ok\":" << (o.ok ? "true" : "false")
    << "}";

    return os;
}

/*************************************************************************************************/

close_user_data_stream_t close_user_data_stream_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    close_user_data_stream_t res{};
    res.ok = json.is_object() && json.is_empty();

    return res;
}

std::ostream &operator<<(std::ostream &os, const close_user_data_stream_t &o) {
    os
    << "{"
    << "\"ok\":" << (o.ok ? "true" : "false")
    << "}";

    return os;
}

} // ns rest

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

namespace ws {

agg_trade_t agg_trade_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    agg_trade_t res{};
    __BINAPI_GET(e);
    __BINAPI_GET(E);
    __BINAPI_GET(s);
    __BINAPI_GET(a);
    __BINAPI_GET(p);
    __BINAPI_GET(q);
    __BINAPI_GET(f);
    __BINAPI_GET(l);
    __BINAPI_GET(T);
    __BINAPI_GET(m);
    __BINAPI_GET_DEFAULT(M, false);

    return res;
}

std::ostream &operator<<(std::ostream &os, const agg_trade_t &o) {
    os
    << "{"
    << "\"e\":\"" << o.e << "\","
    << "\"E\":" << o.E << ","
    << "\"s\":\"" << o.s << "\","
    << "\"a\":" << o.a << ","
    << "\"p\":\"" << o.p << "\","
    << "\"q\":\"" << o.q << "\","
    << "\"f\":" << o.f << ","
    << "\"l\":" << o.l << ","
    << "\"T\":" << o.T << ","
    << "\"m\":" << (o.m ? "true" : "false") << ","
    << "\"M\":" << (o.M ? "true" : "false")
    << "}";

    return os;
}

/*************************************************************************************************/

trade_t trade_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    trade_t res{};
    __BINAPI_GET(E);
    __BINAPI_GET(s);
    __BINAPI_GET(t);
    __BINAPI_GET(p);
    __BINAPI_GET(q);
    __BINAPI_GET_DEFAULT(b, size_t(0));
    __BINAPI_GET_DEFAULT(a, size_t(0));
    __BINAPI_GET(T);
    __BINAPI_GET_DEFAULT(m, false);
    __BINAPI_GET_DEFAULT(M, false);

    return res;
}

std::ostream &operator<<(std::ostream &os, const trade_t &o) {
    os
    << "{"
    << "\"E\":" << o.E << ","
    << "\"s\":\"" << o.s << "\","
    << "\"t\":" << o.t << ","
    << "\"p\":\"" << o.p << "\","
    << "\"q\":\"" << o.q << "\","
    << "\"b\":" << o.b << ","
    << "\"a\":" << o.a << ","
    << "\"T\":" << o.T << ","
    << "\"m\":" << (o.m ? "true" : "false") << ","
    << "\"M\":" << (o.M ? "true" : "false")
    << "}";

    return os;
}

/*************************************************************************************************/

std::ostream& operator<< (std::ostream &os, const part_depths_t::depth_t &o) {
    (void)o;

    return os;
}

part_depths_t part_depths_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    part_depths_t res{};
    const auto a = json.at("asks");
    for ( auto idx = 0u; idx < a.size(); ++idx ) {
        part_depths_t::depth_t item{};
        const auto it = a.at(idx);
        item.price = new_qube::fixed_point<>(it.at(0).to_string());
        item.amount = new_qube::fixed_point<>(it.at(1).to_string());

        res.a.push_back(std::move(item));
    }
    const auto b = json.at("bids");
    for ( auto idx = 0u; idx < b.size(); ++idx ) {
        part_depths_t::depth_t item{};
        const auto it = b.at(idx);
        item.price = new_qube::fixed_point<>(it.at(0).to_string());
        item.amount = new_qube::fixed_point<>(it.at(1).to_string());

        res.b.push_back(std::move(item));
    }

    return res;
}

std::ostream& operator<<(std::ostream &os, const part_depths_t &o) {
    os
    << "{"
    << "\"asks\":[";
    for ( auto it = o.a.begin(); it != o.a.end(); ++it ) {
        os << "[\"" << it->price << "\", \"" << it->amount << "\"]";
        if ( std::next(it) != o.a.end() ) {
            os << ",";
        }
    }
    os
    << "],"
    << "\"bids\":[";
    for ( auto it = o.b.begin(); it != o.b.end(); ++it ) {
        os << "[\"" << it->price << "\", \"" << it->amount << "\"]";
        if ( std::next(it) != o.b.end() ) {
            os << ",";
        }
    }
    os
    << "]}";

    return os;
}

/*************************************************************************************************/

std::ostream &operator<<(std::ostream &os, const diff_depths_t::depth_t &o) {
    (void)o;

    return os;
}

diff_depths_t diff_depths_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    diff_depths_t res{};
    __BINAPI_GET(E);
    __BINAPI_GET(s);
    __BINAPI_GET(u);
    __BINAPI_GET(U);
    const auto a = json.at("a");
    for ( auto idx = 0u; idx < a.size(); ++idx ) {
        diff_depths_t::depth_t item{};
        const auto it = a.at(idx);
        item.price = new_qube::fixed_point<>(it.at(0).to_string());
        item.amount = new_qube::fixed_point<>(it.at(1).to_string());

        res.a.push_back(std::move(item));
    }
    const auto b = json.at("b");
    for ( auto idx = 0u; idx < b.size(); ++idx ) {
        diff_depths_t::depth_t item{};
        const auto it = b.at(idx);
        item.price = new_qube::fixed_point<>(it.at(0).to_string());
        item.amount = new_qube::fixed_point<>(it.at(1).to_string());

        res.b.push_back(std::move(item));
    }

    return res;
}

std::ostream& operator<<(std::ostream &os, const diff_depths_t &o) {
    os
    << "{"
    << "\"E\":" << o.E << ","
    << "\"s\":\"" << o.s << "\","
    << "\"u\":" << o.u << ","
    << "\"U\":" << o.U << ","
    << "\"a\":[";
    for ( auto it = o.a.begin(); it != o.a.end(); ++it ) {
        os << "[\"" << it->price << "\", \"" << it->amount << "\"]";
        if ( std::next(it) != o.a.end() ) {
            os << ",";
        }
    }
    os
    << "],"
    << "\"b\":[";
    for ( auto it = o.b.begin(); it != o.b.end(); ++it ) {
        os << "[\"" << it->price << "\", \"" << it->amount << "\"]";
        if ( std::next(it) != o.b.end() ) {
            os << ",";
        }
    }
    os
    << "]}";

    return os;
}

/*************************************************************************************************/

/*static*/ new_symbol_info_t new_symbol_info_t::construct(const flatjson::fjson& json)
{
    assert(json.is_valid());

    new_symbol_info_t res{};

    __BINAPI_GET(E);
    __BINAPI_GET(id);
    __BINAPI_GET(cid);
    __BINAPI_GET(u);
    __BINAPI_GET(qa);
    __BINAPI_GET(s);
    __BINAPI_GET(unit);
    __BINAPI_GET(mq);
    __BINAPI_GET(d);
    __BINAPI_GET(sp);
    __BINAPI_GET(ed);

    return res;
}

/*friend*/ std::ostream& operator<<(std::ostream& os, const new_symbol_info_t& o)
{
    os
    << "{"
    << "\"E\":" << o.E <<","
    << "\"id\":" << o.id <<","
    << "\"cid\":" << o.cid <<","
    << "\"u\":" << o.u <<","
    << "\"qa\":" << o.qa <<","
    << "\"s\":" << o.s <<","
    << "\"unit\":" << o.unit <<","
    << "\"mq\":" << o.mq <<","
    << "\"d\":" << o.d <<","
    << "\"sp\":" << o.sp <<","
    << "\"ed\":" << o.ed <<","
    << "}";

    return os;
}

/*************************************************************************************************/

/*static*/ new_linear_future_symbol_info_t new_linear_future_symbol_info_t::construct(const flatjson::fjson& json)
{
    assert(json.is_valid());

    new_linear_future_symbol_info_t res{};

    __BINAPI_GET(E);
    __BINAPI_GET(s);
    __BINAPI_GET(ps);
    __BINAPI_GET(ct);
    __BINAPI_GET(dt);
    __BINAPI_GET(ot);
    __BINAPI_GET(cs);

    return res;
}

/*friend*/ std::ostream& operator<<(std::ostream& os, const new_linear_future_symbol_info_t & o)
{
    os
    << "{"
    << "\"E\":" << o.E <<","
    << "\"s\":" << o.s << ","
    << "\"ps\":" << o.ps << ","
    << "\"ct\":" << o.ct << ","
    << "\"dt\":" << o.dt << ","
    << "\"ot\":" << o.ot << ","
    << "\"cs\":" << o.cs << ","
    << "}";

    return os;
}

/*************************************************************************************************/

/*static*/ new_inverse_future_symbol_info_t new_inverse_future_symbol_info_t::construct(const flatjson::fjson& json)
{
    assert(json.is_valid());

    new_inverse_future_symbol_info_t res{};

    __BINAPI_GET(E);
    __BINAPI_GET(s);
    __BINAPI_GET(ps);
    __BINAPI_GET(ct);
    __BINAPI_GET(dt);
    __BINAPI_GET(ot);
    __BINAPI_GET(cs);

    return res;
}

/*friend*/ std::ostream& operator<<(std::ostream& os, const new_inverse_future_symbol_info_t & o)
{
    os
    << "{"
    << "\"E\":" << o.E <<","
    << "\"s\":" << o.s << ","
    << "\"ps\":" << o.ps << ","
    << "\"ct\":" << o.ct << ","
    << "\"dt\":" << o.dt << ","
    << "\"ot\":" << o.ot << ","
    << "\"cs\":" << o.cs << ","
    << "}";

    return os;
}

/*************************************************************************************************/

kline_t kline_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    kline_t res{};
    __BINAPI_GET(E);
    __BINAPI_GET(s);

    const auto k = json.at("k");
    __BINAPI_GET2(res, t, k);
    __BINAPI_GET2(res, T, k);
    __BINAPI_GET2(res, i, k);
    __BINAPI_GET2(res, f, k);
    __BINAPI_GET2(res, L, k);
    __BINAPI_GET2(res, o, k);
    __BINAPI_GET2(res, c, k);
    __BINAPI_GET2(res, h, k);
    __BINAPI_GET2(res, l, k);
    __BINAPI_GET2(res, v, k);
    __BINAPI_GET2(res, n, k);
    __BINAPI_GET2(res, x, k);
    __BINAPI_GET2(res, q, k);
    __BINAPI_GET2(res, V, k);
    __BINAPI_GET2(res, Q, k);

    return res;
}

std::ostream& operator<<(std::ostream &os, const kline_t &o) {
    os
    << "{"
    << "\"E\": " << o.E << ","
    << "\"s\": \"" << o.s << "\","
    << "\"t\": " << o.t << ","
    << "\"T\": " << o.T << ","
    << "\"i\": \"" << o.i << "\","
    << "\"f\": " << o.f << ","
    << "\"L\": " << o.L << ","
    << "\"o\": \"" << o.o << "\","
    << "\"c\": \"" << o.c << "\","
    << "\"h\": \"" << o.h << "\","
    << "\"l\": \"" << o.l << "\","
    << "\"v\": \"" << o.v << "\","
    << "\"n\": " << o.n << ","
    << "\"x\": " << (o.x ? "true" : "false") << ","
    << "\"q\": \"" << o.q << "\","
    << "\"V\": \"" << o.V << "\","
    << "\"Q\": \"" << o.Q << "\""
    << "}";

    return os;
}


bool ohlc_equal(const kline_t &l, const kline_t &r) {
    return l.o == r.o && l.h == r.h && l.l == r.l && l.c == r.c;
}

std::ostream& ohlc(std::ostream &os, const kline_t &o) {
    os
    << "{"
    << "\"o\": \"" << o.o << "\","
    << "\"h\": \"" << o.h << "\","
    << "\"l\": \"" << o.l << "\","
    << "\"c\": \"" << o.c
    << "}";

    return os;
}

/*************************************************************************************************/

mini_ticker_t mini_ticker_t::construct(const flatjson::fjson &json) {
    mini_ticker_t res{};
    __BINAPI_GET(E);
    __BINAPI_GET(s);
    __BINAPI_GET(c);
    __BINAPI_GET(o);
    __BINAPI_GET(h);
    __BINAPI_GET(l);
    __BINAPI_GET(v);
    __BINAPI_GET(q);

    return res;
}

std::ostream& operator<<(std::ostream &os, const mini_ticker_t &o) {
    os
    << "{"
    << "\"E:\"" << o.E << ","
    << "\"s\":\"" << o.s << "\","
    << "\"c\":\"" << o.c << "\","
    << "\"o\":\"" << o.o << "\","
    << "\"h\":\"" << o.h << "\","
    << "\"l\":\"" << o.l << "\","
    << "\"v\":\"" << o.v << "\","
    << "\"q\":\"" << o.q << "\""
    << "}";

    return os;
}

mini_tickers_t mini_tickers_t::construct(const flatjson::fjson &json) {
    assert(json.is_array());

    mini_tickers_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        const auto it = json.at(idx);
        std::string symbol = it.at("s").to_string();
        mini_ticker_t item = mini_ticker_t::construct(it);
        res.tickers.emplace(std::move(symbol), std::move(item));
    }

    return res;
}

std::ostream& operator<<(std::ostream &os, const mini_tickers_t &o) {
    os << "[";
    for ( auto it = o.tickers.begin(); it !=o.tickers.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != o.tickers.end() ) {
            os << ",";
        }
    }
    os << "]";

    return os;
}

/*************************************************************************************************/

market_ticker_t market_ticker_t::construct(const flatjson::fjson &json) {
    market_ticker_t res{};
    __BINAPI_GET(E);
    __BINAPI_GET(s);
    __BINAPI_GET(p);
    __BINAPI_GET(P);
    __BINAPI_GET(w);
    __BINAPI_GET(x);
    __BINAPI_GET(c);
    __BINAPI_GET(Q);
    __BINAPI_GET(b);
    __BINAPI_GET(B);
    __BINAPI_GET(a);
    __BINAPI_GET(A);
    __BINAPI_GET(o);
    __BINAPI_GET(h);
    __BINAPI_GET(l);
    __BINAPI_GET(v);
    __BINAPI_GET(q);
    __BINAPI_GET(O);
    __BINAPI_GET(C);
    __BINAPI_GET(F);
    __BINAPI_GET(L);
    __BINAPI_GET(n);

    return res;
}

std::ostream& operator<<(std::ostream &os, const market_ticker_t &o) {
    os
    << "{"
    << "\"E\":" << o.E << ","
    << "\"s\":\"" << o.s << "\","
    << "\"p\":\"" << o.p << "\","
    << "\"P\":\"" << o.P << "\","
    << "\"w\":\"" << o.w << "\","
    << "\"x\":\"" << o.x << "\","
    << "\"c\":\"" << o.c << "\","
    << "\"Q\":\"" << o.Q << "\","
    << "\"b\":\"" << o.b << "\","
    << "\"B\":\"" << o.B << "\","
    << "\"a\":\"" << o.a << "\","
    << "\"A\":\"" << o.A << "\","
    << "\"o\":\"" << o.o << "\","
    << "\"h\":\"" << o.h << "\","
    << "\"l\":\"" << o.l << "\","
    << "\"v\":\"" << o.v << "\","
    << "\"q\":\"" << o.q << "\","
    << "\"O\":" << o.O << ","
    << "\"C\":" << o.C << ","
    << "\"F\":" << o.F << ","
    << "\"L\":" << o.L << ","
    << "\"n\":" << o.n
    << "}";

    return os;
}

/*************************************************************************************************/

linear_future_market_ticker_t linear_future_market_ticker_t::construct(const flatjson::fjson &json) {
    linear_future_market_ticker_t res{};
    __BINAPI_GET(E);
    __BINAPI_GET(s);
    __BINAPI_GET(p);
    __BINAPI_GET(P);
    __BINAPI_GET(w);
    __BINAPI_GET(c);
    __BINAPI_GET(Q);
    __BINAPI_GET(o);
    __BINAPI_GET(h);
    __BINAPI_GET(l);
    __BINAPI_GET(v);
    __BINAPI_GET(q);
    __BINAPI_GET(O);
    __BINAPI_GET(C);
    __BINAPI_GET(F);
    __BINAPI_GET(L);
    __BINAPI_GET(n);

    return res;
}

std::ostream& operator<<(std::ostream &os, const linear_future_market_ticker_t &o) {
    os
    << "{"
    << "\"E\":" << o.E << ","
    << "\"s\":\"" << o.s << "\","
    << "\"p\":\"" << o.p << "\","
    << "\"P\":\"" << o.P << "\","
    << "\"w\":\"" << o.w << "\","
    << "\"c\":\"" << o.c << "\","
    << "\"Q\":\"" << o.Q << "\","
    << "\"o\":\"" << o.o << "\","
    << "\"h\":\"" << o.h << "\","
    << "\"l\":\"" << o.l << "\","
    << "\"v\":\"" << o.v << "\","
    << "\"q\":\"" << o.q << "\","
    << "\"O\":" << o.O << ","
    << "\"C\":" << o.C << ","
    << "\"F\":" << o.F << ","
    << "\"L\":" << o.L << ","
    << "\"n\":" << o.n
    << "}";

    return os;
}

/*************************************************************************************************/

inverse_future_market_ticker_t inverse_future_market_ticker_t::construct(const flatjson::fjson &json) {
    inverse_future_market_ticker_t res{};
    __BINAPI_GET(E);
    __BINAPI_GET(s);
    __BINAPI_GET(ps);
    __BINAPI_GET(p);
    __BINAPI_GET(P);
    __BINAPI_GET(w);
    __BINAPI_GET(c);
    __BINAPI_GET(Q);
    __BINAPI_GET(o);
    __BINAPI_GET(h);
    __BINAPI_GET(l);
    __BINAPI_GET(v);
    __BINAPI_GET(q);
    __BINAPI_GET(O);
    __BINAPI_GET(C);
    __BINAPI_GET(F);
    __BINAPI_GET(L);
    __BINAPI_GET(n);

    return res;
}

std::ostream& operator<<(std::ostream &os, const inverse_future_market_ticker_t &o) {
    os
    << "{"
    << "\"E\":" << o.E << ","
    << "\"s\":\"" << o.s << "\","
    << "\"ps\":\"" << o.ps << "\","
    << "\"p\":\"" << o.p << "\","
    << "\"P\":\"" << o.P << "\","
    << "\"w\":\"" << o.w << "\","
    << "\"c\":\"" << o.c << "\","
    << "\"Q\":\"" << o.Q << "\","
    << "\"o\":\"" << o.o << "\","
    << "\"h\":\"" << o.h << "\","
    << "\"l\":\"" << o.l << "\","
    << "\"v\":\"" << o.v << "\","
    << "\"q\":\"" << o.q << "\","
    << "\"O\":" << o.O << ","
    << "\"C\":" << o.C << ","
    << "\"F\":" << o.F << ","
    << "\"L\":" << o.L << ","
    << "\"n\":" << o.n
    << "}";

    return os;
}

/*************************************************************************************************/

markets_tickers_t markets_tickers_t::construct(const flatjson::fjson &json) {
    assert(json.is_array());

    markets_tickers_t res{};
    for ( auto idx = 0u; idx < json.size(); ++idx ) {
        market_ticker_t item{};
        const auto it = json.at(idx);
        __BINAPI_GET2(item, E, it);
        __BINAPI_GET2(item, s, it);
        __BINAPI_GET2(item, p, it);
        __BINAPI_GET2(item, P, it);
        __BINAPI_GET2(item, w, it);
        __BINAPI_GET2(item, x, it);
        __BINAPI_GET2(item, c, it);
        __BINAPI_GET2(item, Q, it);
        __BINAPI_GET2(item, b, it);
        __BINAPI_GET2(item, B, it);
        __BINAPI_GET2(item, a, it);
        __BINAPI_GET2(item, A, it);
        __BINAPI_GET2(item, o, it);
        __BINAPI_GET2(item, h, it);
        __BINAPI_GET2(item, l, it);
        __BINAPI_GET2(item, v, it);
        __BINAPI_GET2(item, q, it);
        __BINAPI_GET2(item, O, it);
        __BINAPI_GET2(item, C, it);
        __BINAPI_GET2(item, F, it);
        __BINAPI_GET2(item, L, it);
        __BINAPI_GET2(item, n, it);

        std::string symbol = item.s;
        res.tickers.emplace(std::move(symbol), std::move(item));
    }

    return res;
}

std::ostream& operator<<(std::ostream &os, const markets_tickers_t &o) {
    os << "[";
    for ( auto it = o.tickers.begin(); it !=o.tickers.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != o.tickers.end() ) {
            os << ",";
        }
    }
    os << "]";

    return os;
}

/*************************************************************************************************/

book_ticker_t book_ticker_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    book_ticker_t res{};
    __BINAPI_GET(u);
    __BINAPI_GET(s);
    __BINAPI_GET(b);
    __BINAPI_GET(B);
    __BINAPI_GET(a);
    __BINAPI_GET(A);

    return res;
}

std::ostream& operator<<(std::ostream &os, const book_ticker_t &o) {
    os
    << "{"
    << "\"u\":" << o.u << ","
    << "\"s\":\"" << o.s << "\","
    << "\"b\":\"" << o.b << "\","
    << "\"B\":\"" << o.B << "\","
    << "\"a\":\"" << o.a << "\","
    << "\"A\":\"" << o.A << "\""
    << "}";

    return os;
}

/*************************************************************************************************/

/*************************************************************************************************/

option_ticker_t option_ticker_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    option_ticker_t res{};

    __BINAPI_GET(e);
    __BINAPI_GET(E);
    __BINAPI_GET(s);
    __BINAPI_GET(o);
    __BINAPI_GET(h);
    __BINAPI_GET(l);
    __BINAPI_GET(c);
    __BINAPI_GET(V);
    __BINAPI_GET(A);
    __BINAPI_GET(P);
    __BINAPI_GET(p);
    __BINAPI_GET(Q);
    __BINAPI_GET(F);
    __BINAPI_GET(L);
    __BINAPI_GET(n);
    __BINAPI_GET(bo);
    __BINAPI_GET(ao);
    __BINAPI_GET(bq);
    __BINAPI_GET(aq);
    __BINAPI_GET(b);
    __BINAPI_GET(a);
    __BINAPI_GET(d);
    __BINAPI_GET(t);
    __BINAPI_GET(g);
    __BINAPI_GET(v);
    __BINAPI_GET(vo);
    __BINAPI_GET(mp);
    __BINAPI_GET(hl);
    __BINAPI_GET(ll);
    __BINAPI_GET(eep);

    return res;
}

std::ostream& operator<<(std::ostream &os, const option_ticker_t &o) {
/*    os
    << "{"
    << "\"u\":" << o.u << ","
    << "\"s\":\"" << o.s << "\","
    << "\"b\":\"" << o.b << "\","
    << "\"B\":\"" << o.B << "\","
    << "\"a\":\"" << o.a << "\","
    << "\"A\":\"" << o.A << "\""
    << "}";
    */
    return os;
}

/*************************************************************************************************/

/*************************************************************************************************/
/*************************************************************************************************/

} // ns ws

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

namespace userdata {

std::ostream& operator<<(std::ostream &os, const account_update_t::balance_t &o) {
    os
    << "{"
    << "\"a\":\"" << o.a << "\","
    << "\"f\":\"" << o.f << "\","
    << "\"l\":\"" << o.l << "\""
    << "}";

    return os;
}

account_update_t account_update_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    account_update_t res{};
    __BINAPI_GET(e);
    __BINAPI_GET(E);
    __BINAPI_GET(u);
    const auto B = json.at("B");
    for ( auto idx = 0u; idx < B.size(); ++idx ) {
        account_update_t::balance_t item{};
        const auto it = B.at(idx);
        __BINAPI_GET2(item, a, it);
        __BINAPI_GET2(item, l, it);
        __BINAPI_GET2(item, f, it);

        res.B[item.a] = std::move(item);
    }

    return res;
}

std::ostream& operator<<(std::ostream &os, const account_update_t &o) {
    os
    << "{"
    << "\"e\":\"" << o.e << "\","
    << "\"E\":" << o.E << ","
    << "\"u\":" << o.u << ","
    << "\"B\":[";
    for ( auto it = o.B.begin(); it != o.B.end(); ++it ) {
        os << it->second;
        if ( std::next(it) != o.B.end() ) {
            os << ",";
        }
    }

    os << "]}";

    return os;
}

/*************************************************************************************************/

balance_update_t balance_update_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    balance_update_t res{};
    __BINAPI_GET(e);
    __BINAPI_GET(E);
    __BINAPI_GET(a);
    __BINAPI_GET(d);
    __BINAPI_GET(T);

    return res;
}

std::ostream& operator<<(std::ostream &os, const balance_update_t &o) {
    os
    << "{"
    << "\"e\":\"" << o.e << "\","
    << "\"E\":"   << o.E << ","
    << "\"a\":\"" << o.a << "\","
    << "\"d\":\"" << o.d << "\","
    << "\"T\":"   << o.T
    << "}";

    return os;
}

/*************************************************************************************************/

order_update_t order_update_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    order_update_t res{};
    __BINAPI_GET(e);
    __BINAPI_GET(E);
    __BINAPI_GET(s);
    __BINAPI_GET(c);
    __BINAPI_GET(S);
    __BINAPI_GET(o);
    __BINAPI_GET(f);
    __BINAPI_GET(q);
    __BINAPI_GET(p);
    __BINAPI_GET(P);
    //__BINAPI_GET(d);
    __BINAPI_GET(F);
    __BINAPI_GET(C);
    __BINAPI_GET(x);
    __BINAPI_GET(X);
    __BINAPI_GET(r);
    __BINAPI_GET(i);
    __BINAPI_GET(l);
    __BINAPI_GET(z);
    __BINAPI_GET(L);
    __BINAPI_GET(n);
    __BINAPI_GET(N);
    __BINAPI_GET(T);
    __BINAPI_GET(t);
    __BINAPI_GET(I);
    __BINAPI_GET(w);
    __BINAPI_GET(m);
    __BINAPI_GET(M);
    __BINAPI_GET(O);
    __BINAPI_GET(Z);

    return res;
}

std::ostream& operator<<(std::ostream &os, const order_update_t &o) {
    os
    << "{"
    << "\"e\":\"" << o.e << "\","
    << "\"E\":" << o.E << ","
    << "\"s\":\"" << o.s << "\","
    << "\"c\":\"" << o.c << "\","
    << "\"S\":\"" << o.S << "\","
    << "\"o\":\"" << o.o << "\","
    << "\"f\":\"" << o.f << "\","
    << "\"q\":\"" << o.q << "\","
    << "\"p\":\"" << o.p << "\","
    << "\"P\":\"" << o.P << "\","
    << "\"F\":\"" << o.F << "\","
    << "\"C\":\"" << o.C << "\","
    << "\"x\":\"" << o.x << "\","
    << "\"X\":\"" << o.X << "\","
    << "\"r\":\"" << o.r << "\","
    << "\"i\":"   << o.i << ","
    << "\"l\":\"" << o.l << "\","
    << "\"z\":\"" << o.z << "\","
    << "\"L\":\"" << o.L << "\","
    << "\"n\":\"" << o.n << "\","
    << "\"N\":\"" << o.N << "\","
    << "\"T\":"   << o.T << ","
    << "\"t\":"   << o.t << ","
    << "\"I\":"   << o.I << ","
    << "\"w\":"   << (o.w ? "true" : "false") << ","
    << "\"m\":"   << (o.m ? "true" : "false") << ","
    << "\"M\":"   << (o.M ? "true" : "false") << ","
    << "\"O\":"   << o.O << ","
    << "\"Z\":\"" << o.Z << "\""
    << "}";


    return os;
}

/*************************************************************************************************/

userdata_stream_t userdata_stream_t::construct(const flatjson::fjson &json) {
    userdata_stream_t res{};
    const auto json_src = json.get_source_data();
    res.data.assign(json_src.first, json_src.second);

    return res;
}

std::ostream& operator<<(std::ostream &os, const userdata_stream_t &o) {
    (void)o;

    return os;
}

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

} // ns userdata
} // ns binapi
