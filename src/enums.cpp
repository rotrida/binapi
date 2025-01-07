
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include <binapi/enums.hpp>
#include <binapi/fnv1a.hpp>

#include <cassert>

namespace binapi {

/*************************************************************************************************/

e_side e_side_from_string(const char *str) {
    const auto hash = fnv1a(str);
    switch ( hash ) {
        case fnv1a("BUY"): return e_side::buy;
        case fnv1a("SELL"): return e_side::sell;
    }

    assert(!"unreachable");
}

const char* e_side_to_string(e_side side) {
    switch ( side ) {
        case e_side::buy: return "BUY";
        case e_side::sell: return "SELL";
    }

    assert(!"unreachable");

    return nullptr;
}

/*************************************************************************************************/

e_type e_type_from_string(const char *str) {
    const auto hash = fnv1a(str);
    switch ( hash ) {
        case fnv1a("LIMIT"): return e_type::limit;
        case fnv1a("MARKET"): return e_type::market;
        case fnv1a("STOP_LOSS"): return e_type::stop_loss;
        case fnv1a("STOP_LOSS_LIMIT"): return e_type::stop_loss_limit;
        case fnv1a("TAKE_PROFIT"): return e_type::take_profit;
        case fnv1a("TAKE_PROFIT_LIMIT"): return e_type::take_profit_limit;
        case fnv1a("LIMIT_MAKER"): return e_type::limit_maker;
    }

    assert(!"unreachable");
}

const char* e_type_to_string(e_type type) {
    switch ( type ) {
        case e_type::limit: return "LIMIT";
        case e_type::market: return "MARKET";
        case e_type::stop_loss: return "STOP_LOSS";
        case e_type::stop_loss_limit: return "STOP_LOSS_LIMIT";
        case e_type::take_profit: return "TAKE_PROFIT";
        case e_type::take_profit_limit: return "TAKE_PROFIT_LIMIT";
        case e_type::limit_maker: return "LIMIT_MAKER";
    }

    assert(!"unreachable");

    return nullptr;
}

/*************************************************************************************************/

e_time e_time_from_string(const char *str) {
    const auto hash = fnv1a(str);
    switch ( hash ) {
        case fnv1a("GTC"): return e_time::GTC;
        case fnv1a("IOC"): return e_time::IOC;
        case fnv1a("FOK"): return e_time::FOK;
    }

    assert(!"unreachable");
}

const char* e_time_to_string(e_time time) {
    switch ( time ) {
        case e_time::GTC: return "GTC";
        case e_time::IOC: return "IOC";
        case e_time::FOK: return "FOK";
    }

    assert(!"unreachable");

    return nullptr;
}

/*************************************************************************************************/

e_trade_resp_type e_trade_resp_type_from_string(const char *str) {
    const auto hash = fnv1a(str);
    switch ( hash ) {
        case fnv1a("ACK"): return e_trade_resp_type::ACK;
        case fnv1a("RESULT"): return e_trade_resp_type::RESULT;
        case fnv1a("FULL"): return e_trade_resp_type::FULL;
        case fnv1a("TEST"): return e_trade_resp_type::TEST;
        case fnv1a("UNKNOWN"): return e_trade_resp_type::UNKNOWN;
    }

    assert(!"unreachable");
}

const char* e_trade_resp_type_to_string(e_trade_resp_type resp) {
    switch ( resp ) {
        case e_trade_resp_type::ACK: return "ACK";
        case e_trade_resp_type::RESULT: return "RESULT";
        case e_trade_resp_type::FULL: return "FULL";
        case e_trade_resp_type::TEST: return "TEST";
        case e_trade_resp_type::UNKNOWN: return "UNKNOWN";
    }

    assert(!"unreachable");

    return nullptr;
}

e_side_effect_type e_side_effect_type_from_string(const char* str)
{
    const auto hash = fnv1a(str);
    switch (hash) {
    case fnv1a("NO_SIDE_EFFECT"): return e_side_effect_type::NO_SIDE_EFFECT;
    case fnv1a("MARGIN_BUY"): return e_side_effect_type::MARGIN_BUY;
    case fnv1a("AUTO_REPAY"): return e_side_effect_type::AUTO_REPAY;
    }

    assert(!"unreachable");
}

const char* e_side_effect_type_to_string(e_side_effect_type side_effect)
{
    switch (side_effect) {
    case e_side_effect_type::NO_SIDE_EFFECT: return "NO_SIDE_EFFECT";
    case e_side_effect_type::MARGIN_BUY: return "MARGIN_BUY";
    case e_side_effect_type::AUTO_REPAY: return "AUTO_REPAY";
    }

    assert(!"unreachable");

    return nullptr;
}

e_position_side_type e_position_side_type_from_string(const char* str)
{
    const auto hash = fnv1a(str);
    switch (hash) {
    case fnv1a("BOTH"): return e_position_side_type::BOTH;
    case fnv1a("LONG"): return e_position_side_type::LONG;
    case fnv1a("SHORT"): return e_position_side_type::SHORT;
    }

    assert(!"unreachable");
}

const char* e_position_side_type_to_string(e_position_side_type position_side)
{
    switch (position_side) {
    case e_position_side_type::BOTH: return "BOTH";
    case e_position_side_type::LONG: return "LONG";
    case e_position_side_type::SHORT: return "SHORT";
    }

    assert(!"unreachable");

    return nullptr;
}

e_working_type e_working_type_from_string(const char* str)
{
    const auto hash = fnv1a(str);
    switch (hash) {
    case fnv1a("MARK_PRICE"): return e_working_type::MARK_PRICE;
    case fnv1a("CONTRACT_PRICE"): return e_working_type::CONTRACT_PRICE;
    }

    assert(!"unreachable");
}

const char* e_working_type_to_string(e_working_type working_type)
{
    switch (working_type) {
    case e_working_type::MARK_PRICE: return "MARK_PRICE";
    case e_working_type::CONTRACT_PRICE: return "CONTRACT_PRICE";
    }

    assert(!"unreachable");

    return nullptr;
}

e_price_match e_price_match_from_string(const char* str)
{
    const auto hash = fnv1a(str);
    switch (hash) {
    case fnv1a("OPPONENT"): return e_price_match::OPPONENT;
    case fnv1a("OPPONENT_5"): return e_price_match::OPPONENT_5;
    case fnv1a("OPPONENT_10"): return e_price_match::OPPONENT_10;
    case fnv1a("OPPONENT_20"): return e_price_match::OPPONENT_20;
    case fnv1a("QUEUE"): return e_price_match::QUEUE;
    case fnv1a("QUEUE_5"): return e_price_match::QUEUE_5;
    case fnv1a("QUEUE_10"): return e_price_match::QUEUE_10;
    case fnv1a("QUEUE_20"): return e_price_match::QUEUE_20;
    }

    assert(!"unreachable");
}

const char* e_price_match_to_string(e_price_match price_match)
{
    switch (price_match) 
    {
    case e_price_match::OPPONENT: return "OPPONENT";
    case e_price_match::OPPONENT_5: return "OPPONENT_5";
    case e_price_match::OPPONENT_10: return "OPPONENT_10";
    case e_price_match::OPPONENT_20: return "OPPONENT_20";
    case e_price_match::QUEUE: return "QUEUE";
    case e_price_match::QUEUE_5: return "QUEUE_5";
    case e_price_match::QUEUE_10: return "QUEUE_10";
    case e_price_match::QUEUE_20: return "QUEUE_20";
    }

    assert(!"unreachable");

    return nullptr;
}

e_self_trade_prevention_mode e_self_trade_prevention_mode_from_string(const char* str)
{
    const auto hash = fnv1a(str);
    switch (hash) 
    {
    case fnv1a("NONE"): return e_self_trade_prevention_mode::NONE;
    case fnv1a("EXPIRE_TAKER"): return e_self_trade_prevention_mode::EXPIRE_TAKER;
    case fnv1a("EXPIRE_MAKER"): return e_self_trade_prevention_mode::EXPIRE_MAKER;
    case fnv1a("EXPIRE_BOTH"): return e_self_trade_prevention_mode::EXPIRE_BOTH;
    }

    assert(!"unreachable");
}

const char* e_self_trade_prevention_mode_to_string(e_self_trade_prevention_mode self_trade_prevention_mode)
{
    switch (self_trade_prevention_mode) 
    {
    case e_self_trade_prevention_mode::NONE: return "NONE";
    case e_self_trade_prevention_mode::EXPIRE_TAKER: return "EXPIRE_TAKER";
    case e_self_trade_prevention_mode::EXPIRE_MAKER: return "EXPIRE_MAKER";
    case e_self_trade_prevention_mode::EXPIRE_BOTH: return "EXPIRE_BOTH";
    }

    assert(!"unreachable");

    return nullptr;
}

/*************************************************************************************************/

} // ns binapi
