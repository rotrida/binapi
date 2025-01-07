
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __binapi__enums_hpp
#define __binapi__enums_hpp

#include <cstdint>

using namespace std;

namespace binapi {

/*************************************************************************************************/

enum class e_side: size_t {
    buy
   ,sell
};

e_side e_side_from_string(const char *str);
const char* e_side_to_string(e_side side);

/*************************************************************************************************/

enum class e_type: size_t {
     limit
    ,market
    ,stop_loss
    ,stop_loss_limit
    ,take_profit
    ,take_profit_limit
    ,limit_maker
};

e_type e_type_from_string(const char *str);
const char* e_type_to_string(e_type type);

/*************************************************************************************************/

enum class e_time: size_t {
     GTC
    ,IOC
    ,FOK
};

e_time e_time_from_string(const char *str);
const char* e_time_to_string(e_time time);

/*************************************************************************************************/

enum class e_freq: size_t {
     _100ms  = 100
    ,_1000ms = 1000
};

e_freq e_freq_from_string(const char *str);
const char* e_freq_to_string(e_freq freq);

/*************************************************************************************************/

enum class e_levels: size_t {
     _5 = 5
    ,_10 = 10
    ,_20 = 20
};

e_levels e_levels_from_string(const char *str);
const char* e_levels_to_string(e_levels level);

/*************************************************************************************************/

enum class e_trade_resp_type: size_t {
     ACK
    ,RESULT
    ,FULL
    ,TEST
    ,UNKNOWN
};

e_trade_resp_type e_trade_resp_type_from_string(const char *str);
const char* e_trade_resp_type_to_string(e_trade_resp_type resp);

/*************************************************************************************************/

enum class e_side_effect_type : size_t {
    NO_SIDE_EFFECT
    , MARGIN_BUY
    , AUTO_REPAY
};

e_side_effect_type e_side_effect_type_from_string(const char* str);
const char* e_side_effect_type_to_string(e_side_effect_type side_effect);

/*************************************************************************************************/

enum class e_position_side_type : size_t {
    BOTH
    , LONG
    , SHORT
};

e_position_side_type e_position_side_type_from_string(const char* str);
const char* e_position_side_type_to_string(e_position_side_type side_effect);

/*************************************************************************************************/

enum class e_working_type : size_t {
    MARK_PRICE
    , CONTRACT_PRICE
};

e_working_type e_working_type_from_string(const char* str);
const char* e_working_type_to_string(e_working_type side_effect);

/*************************************************************************************************/

enum class e_price_match : size_t {
    OPPONENT
    , OPPONENT_5
    , OPPONENT_10
    , OPPONENT_20
    , QUEUE
    , QUEUE_5
    , QUEUE_10
    , QUEUE_20
};

e_price_match e_price_match_from_string(const char* str);
const char* e_price_match_to_string(e_price_match price_match);

/*************************************************************************************************/

enum class e_self_trade_prevention_mode : size_t {
    NONE
    , EXPIRE_TAKER
    , EXPIRE_MAKER
    , EXPIRE_BOTH
};

e_self_trade_prevention_mode e_self_trade_prevention_mode_from_string(const char* str);
const char* e_self_trade_prevention_mode_to_string(e_self_trade_prevention_mode self_trade_prevention_mode);

} // ns binapi

#endif // __binapi__enums_hpp
