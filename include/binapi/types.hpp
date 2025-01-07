
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __binapi__types_hpp
#define __binapi__types_hpp

#include "double_type.hpp"
#include "enums.hpp"

#include <boost/variant.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <cstdint>
#include <cassert>

// forward
namespace flatjson {
struct fjson;
} // ns flatjson

namespace binapi {

/*************************************************************************************************/

namespace rest {

struct ping_t {
    bool ok;

    static ping_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const ping_t &f);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#check-server-time
struct server_time_t {
    std::size_t serverTime;

    static server_time_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const server_time_t &f);
};

// https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#current-average-price
struct avg_price_t {
    std::size_t mins;
    double_type price;

    static avg_price_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const avg_price_t &f);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#symbol-price-ticker
struct prices_t {
    struct price_t {
        std::string symbol;
        double_type price;

        static price_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const price_t &f);
    };

    static prices_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const prices_t &f);

    std::map<std::string, price_t> prices;

    bool is_valid_symbol(const std::string &sym) const
        { return is_valid_symbol(sym.c_str()); }
    bool is_valid_symbol(const char *sym) const;

    const price_t& get_by_symbol(const std::string &sym) const
        { return get_by_symbol(sym.c_str()); }
    const price_t& get_by_symbol(const char *sym) const;
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#24hr-ticker-price-change-statistics
struct _24hrs_tickers_t {
    struct _24hrs_ticker_t {
        std::string symbol;
        double_type priceChange;
        double_type priceChangePercent;
        double_type weightedAvgPrice;
        double_type prevClosePrice;
        double_type lastPrice;
        double_type lastQty;
        double_type bidPrice;
        double_type askPrice;
        double_type openPrice;
        double_type highPrice;
        double_type lowPrice;
        double_type volume;
        double_type quoteVolume;
        std::size_t openTime;
        std::size_t closeTime;
        std::size_t firstId;
        std::size_t lastId;
        std::size_t count;

        static _24hrs_ticker_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const _24hrs_ticker_t &f);
    };

    std::unordered_map<std::string, _24hrs_ticker_t> tickers;

    static _24hrs_tickers_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const _24hrs_tickers_t &f);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#account-information-user_data
struct account_info_t {
    std::size_t makerCommission;
    std::size_t takerCommission;
    std::size_t buyerCommission;
    std::size_t sellerCommission;
    bool canTrade;
    bool canWithdraw;
    bool canDeposit;
    std::size_t updateTime;

    struct balance_t {
        std::string asset;
        double_type free;
        double_type locked;

        static balance_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const balance_t &f);
    };
    std::unordered_map<std::string, balance_t> balances;

    const balance_t& get_balance(const std::string &symbol) const
        { return get_balance(symbol.c_str()); }
    const balance_t& get_balance(const char *symbol) const;

    const double_type& add_balance(const std::string &symbol, const double_type &amount)
    { return add_balance(symbol.c_str(), amount); }
    const double_type& add_balance(const char *symbol, const double_type &amount);

    const double_type& sub_balance(const std::string &symbol, const double_type &amount)
    { return sub_balance(symbol.c_str(), amount); }
    const double_type& sub_balance(const char *symbol, const double_type &amount);

    static account_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const account_info_t &f);
};

// https://developers.binance.com/docs/derivatives/option/account
struct option_account_info_t {
    std::size_t time;
    std::string riskLevel;

    struct asset_t
    {
        std::string asset;
        double_type marginBalance;
        double_type equity;
        double_type available;
        double_type locked;
        double_type unrealizedPNL;

        static asset_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const asset_t &f);
    };

    std::unordered_map<std::string, asset_t> assets;

    struct greek_t
    {
        std::string underlying;
        double_type delta;
        double_type gamma;
        double_type theta;
        double_type vega;

        static greek_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const greek_t &f);
    };

    std::unordered_map<std::string, greek_t> greeks;

    const asset_t& get_asset(const std::string &asset) const
        { return get_asset(asset.c_str()); }
    const asset_t& get_asset(const char *asset) const;

    const greek_t& get_greek(const std::string &underlying) const
        { return get_greek(underlying.c_str()); }
    const greek_t& get_greek(const char *underlying) const;

    //const double_type& add_balance(const std::string &symbol, const double_type &amount)
    //{ return add_balance(symbol.c_str(), amount); }
    //const double_type& add_balance(const char *symbol, const double_type &amount);
    //
    //const double_type& sub_balance(const std::string &symbol, const double_type &amount)
    //{ return sub_balance(symbol.c_str(), amount); }
    //const double_type& sub_balance(const char *symbol, const double_type &amount);

    static option_account_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const option_account_info_t &f);
};

// https://developers.binance.com/docs/derivatives/usds-margined-futures/account/rest-api/Account-Information-V2
struct linear_future_account_info_t {
    
	double_type totalInitialMargin;             // the sum of USD value of all cross positions/open order initial margin
	double_type totalMaintMargin;  	            // the sum of USD value of all cross positions maintenance margin
	double_type totalWalletBalance;             // total wallet balance in USD
	double_type totalUnrealizedProfit;          // total unrealized profit in USD
	double_type totalMarginBalance;             // total margin balance in USD
	double_type totalPositionInitialMargin;     // the sum of USD value of all cross positions initial margin
	double_type totalOpenOrderInitialMargin;    // initial margin required for open orders with current mark price in USD
	double_type totalCrossWalletBalance;        // crossed wallet balance in USD
	double_type totalCrossUnPnl;	            // unrealized profit of crossed positions in USD
	double_type availableBalance;               // available balance in USD
	double_type maxWithdrawAmount;              // maximum virtual amount for transfer out in USD

    struct asset_t
    {
        std::string asset;			        // asset name
        double_type walletBalance;          // wallet balance
        double_type unrealizedProfit;       // unrealized profit
        double_type marginBalance;          // margin balance
        double_type maintMargin;	        // maintenance margin required
        double_type initialMargin;          // total initial margin required with current mark price 
        double_type positionInitialMargin;  //initial margin required for positions with current mark price
        double_type openOrderInitialMargin; // initial margin required for open orders with current mark price
        double_type crossWalletBalance;     // crossed wallet balance
        double_type crossUnPnl;             // unrealized profit of crossed positions
        double_type availableBalance;       // available balance
        double_type maxWithdrawAmount;      // maximum amount for transfer out
        bool marginAvailable;               // whether the asset can be used as margin in Multi-Assets mode
        size_t updateTime;                  // last update time

        static asset_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const asset_t &f);
    };

    std::unordered_map<std::string, asset_t> assets;

    struct position_t
    {
        std::string symbol;  	                // symbol name
        std::string positionSide;  	            // position side
        double_type positionAmt;			    // position amount
        double_type unrealizedProfit;           // unrealized profit
        double_type isolatedMargin;             // unrealized profit
        double_type notional;                   // notional
        double_type isolatedWallet;             // isolated wallet
        double_type initialMargin;	            // initial margin required with current mark price 
        double_type maintMargin;		        // maintenance margin required
        size_t updateTime;                      // last update time

        static position_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const position_t &f);
    };

    std::unordered_map<std::string, position_t> positions;

    const asset_t& get_asset(const std::string &asset) const
        { return get_asset(asset.c_str()); }
    const asset_t& get_asset(const char *asset) const;

    const position_t& get_position(const std::string &symbol) const
        { return get_position(symbol.c_str()); }
    const position_t& get_position(const char *symbol) const;

    //const double_type& add_balance(const std::string &symbol, const double_type &amount)
    //{ return add_balance(symbol.c_str(), amount); }
    //const double_type& add_balance(const char *symbol, const double_type &amount);
    //
    //const double_type& sub_balance(const std::string &symbol, const double_type &amount)
    //{ return sub_balance(symbol.c_str(), amount); }
    //const double_type& sub_balance(const char *symbol, const double_type &amount);

    static linear_future_account_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const linear_future_account_info_t &f);
};

// https://developers.binance.com/docs/derivatives/usds-margined-futures/account/rest-api/Account-Information-V2
struct inverse_future_account_info_t {
    bool canDeposit;  	        // if can transfer in asset
    bool canTrade;  	        // if can trade
	bool canWithdraw; 	        // if can transfer out asset
	double_type feeTier;  		// account commission tier 
	size_t updateTime;          // reserved property, please ignore 

    struct asset_t
    {
        std::string asset;			        // asset name
        double_type walletBalance;          // wallet balance
        double_type unrealizedProfit;       // unrealized profit
        double_type marginBalance;          // margin balance
        double_type maintMargin;	        // maintenance margin required
        double_type initialMargin;          // total initial margin required with current mark price 
        double_type positionInitialMargin;  //initial margin required for positions with current mark price
        double_type openOrderInitialMargin; // initial margin required for open orders with current mark price
        double_type maxWithdrawAmount;      // maximum amount for transfer out
        double_type crossWalletBalance;     // crossed wallet balance
        double_type crossUnPnl;             // unrealized profit of crossed positions
        double_type availableBalance;       // available balance
        size_t updateTime;                  // last update time

        static asset_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const asset_t &f);
    };

    std::unordered_map<std::string, asset_t> assets;

    struct position_t
    {
        std::string symbol;  	                // symbol name
        double_type positionAmt;			    // position amount
        double_type initialMargin;	            // initial margin required with current mark price 
        double_type maintMargin;		        // maintenance margin required
        double_type unrealizedProfit;           // unrealized profit
        double_type positionInitialMargin;      // initial margin required for positions with current mark price
        double_type openOrderInitialMargin;     // initial margin required for open orders with current mark price
        double_type leverage;		            // current initial leverage
        bool isolated;  		                // if the position is isolated
        std::string positionSide;  	            // position side
        double_type entryPrice;  	            // average entry price
        double_type breakEvenPrice;             // 
        double_type maxQty;	                    // maximum quantity of base asset
        size_t updateTime;                      // last update time

        static position_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const position_t &f);
    };

    std::unordered_map<std::string, position_t> positions;

    const asset_t& get_asset(const std::string &asset) const
        { return get_asset(asset.c_str()); }
    const asset_t& get_asset(const char *asset) const;

    const position_t& get_position(const std::string &symbol) const
        { return get_position(symbol.c_str()); }
    const position_t& get_position(const char *symbol) const;

    //const double_type& add_balance(const std::string &symbol, const double_type &amount)
    //{ return add_balance(symbol.c_str(), amount); }
    //const double_type& add_balance(const char *symbol, const double_type &amount);
    //
    //const double_type& sub_balance(const std::string &symbol, const double_type &amount)
    //{ return sub_balance(symbol.c_str(), amount); }
    //const double_type& sub_balance(const char *symbol, const double_type &amount);

    static inverse_future_account_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const inverse_future_account_info_t &f);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#exchange-information
struct exchange_info_t {
    std::string timezone;
    std::size_t serverTime;
    std::vector<std::string> exchangeFilters;

    struct rate_limit_t {
        std::string rateLimitType;
        std::string interval;
        std::size_t limit;

        friend std::ostream &operator<<(std::ostream &os, const rate_limit_t &f);
    };
    std::vector<rate_limit_t> rateLimits;

    struct symbol_t {
        std::string symbol;
        std::string status;
        std::string baseAsset;
        std::size_t baseAssetPrecision;
        std::string quoteAsset;
        std::size_t quotePrecision;
        std::vector<std::string> orderTypes;
        bool icebergAllowed;

        struct filter_t {
            struct price_t {
                double_type minPrice;
                double_type maxPrice;
                double_type tickSize;

                friend std::ostream &operator<<(std::ostream &os, const price_t &f);
            };
            struct percent_price_t {
                double_type multiplierUp;
                double_type multiplierDown;
                std::size_t avgPriceMins;

                friend std::ostream &operator<<(std::ostream &os, const percent_price_t &f);
            };
            struct percent_price_by_side_t {
                double_type bidMultiplierUp;
                double_type bidMultiplierDown;
                double_type askMultiplierUp;
                double_type askMultiplierDown;
                std::size_t avgPriceMins;
            };
            struct lot_size_t {
                double_type minQty;
                double_type maxQty;
                double_type stepSize;

                friend std::ostream &operator<<(std::ostream &os, const lot_size_t &f);
            };
            struct market_lot_size_t {
                double_type minQty;
                double_type maxQty;
                double_type stepSize;

                friend std::ostream &operator<<(std::ostream &os, const market_lot_size_t &f);
            };
            struct min_notional_t {
                double_type minNotional;

                friend std::ostream &operator<<(std::ostream &os, const min_notional_t &f);
            };
            struct iceberg_parts_t {
                std::size_t limit;

                friend std::ostream &operator<<(std::ostream &os, const iceberg_parts_t &f);
            };
            struct max_num_orders_t {
                std::size_t maxNumOrders;

                friend std::ostream &operator<<(std::ostream &os, const max_num_orders_t &f);
            };

            struct max_num_algo_orders_t {
                std::size_t maxNumAlgoOrders;

                friend std::ostream &operator<<(std::ostream &os, const max_num_algo_orders_t &f);
            };

            struct max_position_t {
                double_type maxPosition;

                friend std::ostream &operator<<(std::ostream &os, const max_position_t &f);
            };

            struct trailing_delta_t {
                std::size_t minTrailingAboveDelta;
                std::size_t maxTrailingAboveDelta;
                std::size_t minTrailingBelowDelta;
                std::size_t maxTrailingBelowDelta;

                friend std::ostream &operator<<(std::ostream &os, const trailing_delta_t &f);
            };

            std::string filterType;
            boost::variant<
                 price_t
                ,percent_price_t
                ,percent_price_by_side_t
                ,lot_size_t
                ,market_lot_size_t
                ,min_notional_t
                ,iceberg_parts_t
                ,max_num_orders_t
                ,max_num_algo_orders_t
                ,max_position_t
                ,trailing_delta_t
            > filter;

            friend std::ostream &operator<<(std::ostream &os, const filter_t &f);
        };
        std::vector<filter_t> filters;

        template<typename T>
        const T& get_filter() const {
            for ( const auto &it: filters ) {
                const T *p = boost::get<T>(&it.filter);
                if ( p ) {
                    return *p;
                }
            }

            assert("bad T type" == nullptr);
            throw std::runtime_error("bad T type");
        }

        const filter_t::price_t& get_filter_price() const
        { return get_filter<filter_t::price_t>(); }
        const filter_t::percent_price_t& get_filter_percent_price() const
        { return get_filter<filter_t::percent_price_t>(); }
        const filter_t::percent_price_by_side_t& get_filter_percent_price_by_side() const
        { return get_filter<filter_t::percent_price_by_side_t>(); }
        const filter_t::lot_size_t& get_filter_lot_size() const
        { return get_filter<filter_t::lot_size_t>(); }
        const filter_t::market_lot_size_t& get_filter_market_lot_size() const
        { return get_filter<filter_t::market_lot_size_t>(); }
        const filter_t::min_notional_t& get_filter_min_notional() const
        { return get_filter<filter_t::min_notional_t>(); }
        const filter_t::iceberg_parts_t& get_filter_iceberg_parts() const
        { return get_filter<filter_t::iceberg_parts_t>(); }
        const filter_t::max_num_orders_t& get_filter_max_num_orders() const
        { return get_filter<filter_t::max_num_orders_t>(); }
        const filter_t::max_num_algo_orders_t& get_filter_max_num_algo_orders() const
        { return get_filter<filter_t::max_num_algo_orders_t>(); }
        const filter_t::max_position_t& get_filter_max_position() const
        { return get_filter<filter_t::max_position_t>(); }
        const filter_t::trailing_delta_t& get_filter_trailing_delta() const
        { return get_filter<filter_t::trailing_delta_t>(); }

        friend std::ostream &operator<<(std::ostream &os, const symbol_t &s);
    };

    std::unordered_map<std::string, symbol_t> symbols;

    bool is_valid_symbol(const std::string &sym) const
        { return is_valid_symbol(sym.c_str()); }
    bool is_valid_symbol(const char *sym) const;

    const symbol_t& get_by_symbol(const std::string &sym) const
        { return get_by_symbol(sym.c_str()); }
    const symbol_t& get_by_symbol(const char *sym) const;

    static exchange_info_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const exchange_info_t &s);
};

struct options_exchange_info_t 
{
	std::string timezone;
	std::size_t serverTime;
	
	struct rate_limit_t {
        std::string rateLimitType;
        std::string interval;
		size_t intervalNum;
        std::size_t limit;

        friend std::ostream &operator<<(std::ostream &os, const rate_limit_t &f);
    };
    std::vector<rate_limit_t> rateLimits;
	
	struct option_contract_t
	{
		std::string baseAsset;
		std::string quoteAsset;
		std::string underlying;
		std::string settleAsset;

        friend std::ostream &operator<<(std::ostream &os, const option_contract_t &f);
	};
	std::vector<option_contract_t> optionContracts;

	struct option_asset_t
	{
		std::string name;

        friend std::ostream &operator<<(std::ostream &os, const option_asset_t &f);
	};
	std::vector<option_asset_t> optionAssets;

	struct option_symbol_t
	{
		int contractId;
        std::size_t expiryDate;
		size_t id;
        std::string symbol;
        std::string side;
        double_type strikePrice;
        std::string underlying;
        size_t unit;
        double_type makerFeeRate;
        double_type takerFeeRate;
        double_type minQty;
        double_type maxQty;
        double_type initialMargin;
        double_type maintenanceMargin;
        double_type minInitialMargin;
        double_type minMaintenanceMargin;
        size_t priceScale;
        size_t quantityScale;
        std::string quoteAsset;
        
		struct filter_t {
            struct price_t {
                double_type minPrice;
                double_type maxPrice;
                double_type tickSize;

                friend std::ostream &operator<<(std::ostream &os, const price_t &f);
            };
			struct lot_size_t {
                double_type minQty;
                double_type maxQty;
                double_type stepSize;

                friend std::ostream &operator<<(std::ostream &os, const lot_size_t &f);
            };
			
			std::string filterType;
			
			boost::variant<
				 price_t
				,lot_size_t
			> filter;

			friend std::ostream &operator<<(std::ostream &os, const filter_t &f);
		};
		std::vector<filter_t> filters;
        
		template<typename T>
		const T& get_filter() const {
			for ( const auto &it: filters ) {
				const T *p = boost::get<T>(&it.filter);
				if ( p ) {
					return *p;
				}
			}

			assert("bad T type" == nullptr);
            throw std::runtime_error("bad T type");
		}

		const filter_t::price_t& get_filter_price() const
		{ return get_filter<filter_t::price_t>(); }

		const filter_t::lot_size_t& get_filter_lot_size() const
		{ return get_filter<filter_t::lot_size_t>(); }
		
		friend std::ostream &operator<<(std::ostream &os, const option_symbol_t &s);
	};

	std::unordered_map<std::string, option_symbol_t> symbols;

    bool is_valid_symbol(const std::string &sym) const
        { return is_valid_symbol(sym.c_str()); }
    bool is_valid_symbol(const char *sym) const;

    const option_symbol_t& get_by_symbol(const std::string &sym) const
        { return get_by_symbol(sym.c_str()); }
    const option_symbol_t& get_by_symbol(const char *sym) const;

    static options_exchange_info_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const options_exchange_info_t &s);
};

struct linear_future_exchange_info_t 
{
	std::string timezone;
	std::size_t serverTime;
	
	struct rate_limit_t {
        std::string rateLimitType;
        std::string interval;
		size_t intervalNum;
        std::size_t limit;

        friend std::ostream &operator<<(std::ostream &os, const rate_limit_t &f);
    };
    std::vector<rate_limit_t> rateLimits;
	
	struct asset_t
	{
		std::string asset;
        bool marginAvailable;
        int autoAssetExchange;

        friend std::ostream &operator<<(std::ostream &os, const asset_t &f);
	};
	std::vector<asset_t> assets;

	struct symbol_t
	{
        std::string symbol;
        std::string pair;
        std::string contractType;
        size_t deliveryDate;
        size_t onboardDate;
        std::string status;
        double_type maintMarginPercent;
        double_type requiredMarginPercent;
        std::string baseAsset;
        std::string quoteAsset;
        std::string marginAsset;
        size_t pricePrecision;
        size_t quantityPrecision;
        size_t baseAssetPrecision;
        size_t quotePrecision;
        std::string underlyingType;
        std::vector<std::string> underlyingSubType;
        double_type triggerProtect;
        double_type liquidationFee;
        double_type marketTakeBound;
        std::vector<std::string> orderType;
        std::vector<std::string> timeInForce;

		struct filter_t {
            struct price_t {
                double_type minPrice;
                double_type maxPrice;
                double_type tickSize;

                friend std::ostream &operator<<(std::ostream &os, const price_t &f);
            };
			struct lot_size_t {
                double_type minQty;
                double_type maxQty;
                double_type stepSize;

                friend std::ostream &operator<<(std::ostream &os, const lot_size_t &f);
            };
            struct market_lot_size_t {
                double_type minQty;
                double_type maxQty;
                double_type stepSize;

                friend std::ostream &operator<<(std::ostream &os, const market_lot_size_t &f);
            };
            struct max_num_orders_t {
                size_t limit;

                friend std::ostream &operator<<(std::ostream &os, const max_num_orders_t &f);
            };
            struct max_num_algo_orders_t {
                size_t limit;

                friend std::ostream &operator<<(std::ostream &os, const max_num_algo_orders_t &f);
            };
            struct min_notional_t {
                double_type notional;

                friend std::ostream &operator<<(std::ostream &os, const max_num_algo_orders_t &f);
            };
            struct percent_price_t {
                double_type multiplierUp;
                double_type multiplierDown;
                size_t multiplierDecimal;

                friend std::ostream &operator<<(std::ostream &os, const percent_price_t &f);
            };
			
			std::string filterType;
			
			boost::variant<
				 price_t
				,lot_size_t
                ,market_lot_size_t
                ,min_notional_t
                ,max_num_orders_t
                ,max_num_algo_orders_t
                ,percent_price_t
			> filter;

			friend std::ostream &operator<<(std::ostream &os, const filter_t &f);
		};
		std::vector<filter_t> filters;
        
		template<typename T>
		const T& get_filter() const {
			for ( const auto &it: filters ) {
				const T *p = boost::get<T>(&it.filter);
				if ( p ) {
					return *p;
				}
			}

			assert("bad T type" == nullptr);
            throw std::runtime_error("bad T type");
		}

		const filter_t::price_t& get_filter_price() const
		{ return get_filter<filter_t::price_t>(); }

		const filter_t::lot_size_t& get_filter_lot_size() const
		{ return get_filter<filter_t::lot_size_t>(); }

        const filter_t::market_lot_size_t& get_filter_market_lot_size() const
		{ return get_filter<filter_t::market_lot_size_t>(); }

        const filter_t::min_notional_t& get_filter_min_notional() const
		{ return get_filter<filter_t::min_notional_t>(); }

        const filter_t::max_num_orders_t& get_filter_max_num_orders() const
		{ return get_filter<filter_t::max_num_orders_t>(); }

        const filter_t::max_num_algo_orders_t& get_filter_max_num_algo_orders() const
		{ return get_filter<filter_t::max_num_algo_orders_t>(); }
		
        const filter_t::percent_price_t& get_filter_percentage_price() const
		{ return get_filter<filter_t::percent_price_t>(); }

		friend std::ostream &operator<<(std::ostream &os, const symbol_t &s);
	};

	std::unordered_map<std::string, symbol_t> symbols;

    bool is_valid_symbol(const std::string &sym) const
        { return is_valid_symbol(sym.c_str()); }
    bool is_valid_symbol(const char *sym) const;

    const symbol_t& get_by_symbol(const std::string &sym) const
        { return get_by_symbol(sym.c_str()); }
    const symbol_t& get_by_symbol(const char *sym) const;

    static linear_future_exchange_info_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const linear_future_exchange_info_t &s);
};

struct inverse_future_exchange_info_t 
{
	std::string timezone;
	std::size_t serverTime;
	
	struct rate_limit_t {
        std::string rateLimitType;
        std::string interval;
		size_t intervalNum;
        std::size_t limit;

        friend std::ostream &operator<<(std::ostream &os, const rate_limit_t &f);
    };
    std::vector<rate_limit_t> rateLimits;
	
	struct symbol_t
	{
        double_type liquidationFee;
        double_type marketTakeBound;
        std::string symbol;
        std::string pair;
        std::string contractType;
        size_t deliveryDate;
        size_t onboardDate;
        std::string contractStatus;
        double_type contractSize;
        std::string quoteAsset;
        std::string baseAsset;
        std::string marginAsset;
        size_t pricePrecision;
        size_t quantityPrecision;
        size_t baseAssetPrecision;
        size_t quotePrecision;
        size_t equalQtyPrecision;
        double_type triggerProtect;
        double_type maintMarginPercent;
        double_type requiredMarginPercent;
        std::string underlyingType;
        std::vector<std::string> underlyingSubType;
        std::vector<std::string> orderType;
        std::vector<std::string> timeInForce;

		struct filter_t {
            struct price_t {
                double_type minPrice;
                double_type maxPrice;
                double_type tickSize;

                friend std::ostream &operator<<(std::ostream &os, const price_t &f);
            };
			struct lot_size_t {
                double_type minQty;
                double_type maxQty;
                double_type stepSize;

                friend std::ostream &operator<<(std::ostream &os, const lot_size_t &f);
            };
            struct market_lot_size_t {
                double_type minQty;
                double_type maxQty;
                double_type stepSize;

                friend std::ostream &operator<<(std::ostream &os, const market_lot_size_t &f);
            };
            struct max_num_orders_t {
                size_t limit;

                friend std::ostream &operator<<(std::ostream &os, const max_num_orders_t &f);
            };
            struct percent_price_t {
                double_type multiplierUp;
                double_type multiplierDown;
                size_t multiplierDecimal;

                friend std::ostream &operator<<(std::ostream &os, const percent_price_t &f);
            };
			
			std::string filterType;
			
			boost::variant<
				 price_t
				,lot_size_t
                ,market_lot_size_t
                ,max_num_orders_t
                ,percent_price_t
			> filter;

			friend std::ostream &operator<<(std::ostream &os, const filter_t &f);
		};
		std::vector<filter_t> filters;
        
		template<typename T>
		const T& get_filter() const {
			for ( const auto &it: filters ) {
				const T *p = boost::get<T>(&it.filter);
				if ( p ) {
					return *p;
				}
			}

			assert("bad T type" == nullptr);
            throw std::runtime_error("bad T type");
		}

		const filter_t::price_t& get_filter_price() const
		{ return get_filter<filter_t::price_t>(); }

		const filter_t::lot_size_t& get_filter_lot_size() const
		{ return get_filter<filter_t::lot_size_t>(); }
		
		friend std::ostream &operator<<(std::ostream &os, const symbol_t &s);
	};

	std::unordered_map<std::string, symbol_t> symbols;

    bool is_valid_symbol(const std::string &sym) const
        { return is_valid_symbol(sym.c_str()); }
    bool is_valid_symbol(const char *sym) const;

    const symbol_t& get_by_symbol(const std::string &sym) const
        { return get_by_symbol(sym.c_str()); }
    const symbol_t& get_by_symbol(const char *sym) const;

    static inverse_future_exchange_info_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const linear_future_exchange_info_t &s);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#order-book
struct depths_t {
    struct depth_t {
        double_type price;
        double_type amount;

        friend std::ostream &operator<<(std::ostream &os, const depth_t &s);
    };

    std::size_t lastUpdateId;
    std::vector<depth_t> bids;
    std::vector<depth_t> asks;

    static depths_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const depths_t &s);
};

struct option_depths_t : public depths_t
{
    int64_t T;
    std::size_t u;

    static option_depths_t construct(const flatjson::fjson &json);
};

struct linear_future_depths_t : public depths_t
{
    std::size_t lastUpdateId;
    int64_t E;
    int64_t T;

    static linear_future_depths_t construct(const flatjson::fjson &json);
};

struct inverse_future_depths_t : public depths_t
{
    std::size_t lastUpdateId;
    std::string symbol;
    std::string pair;
    int64_t E;
    int64_t T;

    static inverse_future_depths_t construct(const flatjson::fjson &json);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#recent-trades-list
struct trades_t {
    struct trade_t {
        std::size_t id;
        double_type price;
        double_type qty;
        std::size_t time;
        bool isBuyerMaker;
        bool isBestMatch;

        static trade_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const trade_t &s);
    };

    std::vector<trade_t> trades;

    static trades_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const trades_t &s);
};

struct option_trades_t {
    struct option_trade_t {
        std::size_t id;
        double_type price;
        double_type qty;
        std::size_t time;

        static option_trade_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const option_trade_t &s);
    };

    std::vector<option_trade_t> trades;

    static option_trades_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const option_trades_t &s);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#compressedaggregate-trades-list
struct agg_trades_t {
    struct agg_trade_t {
        std::size_t id;
        double_type price;
        double_type qty;
        std::size_t first_id;
        std::size_t last_id;
        std::size_t time;
        bool isBuyerMaker;
        bool isBestMatch;

        static agg_trade_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const agg_trade_t &s);
    };

    std::vector<agg_trade_t> trades;

    static agg_trades_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const agg_trades_t &s);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#klinecandlestick-data
struct klines_t {
    struct kline_t {
        std::size_t start_time;
        std::size_t end_time;
        double_type open;
        double_type high;
        double_type low;
        double_type close;
        double_type volume;
        double_type quote_volume;
        std::size_t num_trades;
        double_type taker_buy_base_vol;
        double_type taker_buy_quote_vol;

        friend std::ostream &operator<<(std::ostream &os, const kline_t &s);
    };

    std::vector<kline_t> klines;

    static klines_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const klines_t &s);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#query-order-user_data
struct order_info_t {
    std::string symbol;
    std::size_t orderId;
    std::string clientOrderId;
    std::string origClientOrderId;
    double_type price;
    double_type origQty;
    double_type executedQty;
    double_type cummulativeQuoteQty;
    std::string status;
    std::string timeInForce;
    std::string type;
    std::string side;
    double_type stopPrice;
    double_type icebergQty;
    std::size_t time;
    std::size_t updateTime;
    bool isWorking;

    static order_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const order_info_t &o);
};

struct margin_order_info_t : public order_info_t{
    bool isIsolated;

    static margin_order_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const margin_order_info_t &o);
};

struct option_order_info_t
{
    size_t orderId;         // System order number
    std::string symbol;     // Option trading pair
    double_type price;                  // Order Price
    double_type quantity;               // Order Quantity
    double_type executedQty;            // Number of completed trades
    double_type fee;                    // Fee 
    std::string side;                   // Buy/sell direction
    std::string type;                   // Order type
    std::string timeInForce;            // Time in force method
    bool reduceOnly;                    // Order is reduce only Y/N
    bool postOnly;
    size_t createTime;                  // Order Time
    size_t updateTime;                  // Update Time
    std::string status;                 // Order status
    double_type avgPrice;               // Average price of completed trade
    std::string clientOrderId;          // Client order ID         
    size_t priceScale;
    size_t quantityScale;
    std::string optionSide;
    std::string quoteAsset;
    bool mmp;

    static option_order_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const option_order_info_t &o);
};

struct linear_future_order_info_t
{
  	double_type avgPrice;
  	std::string clientOrderId;
  	double_type cumQuote; 	double_type executedQty; 	
    size_t orderId;
  	double_type origQty;
  	std::string origType;
  	double_type price; 	
    bool reduceOnly;
  	std::string side;
  	std::string positionSide;
  	std::string status;
  	double_type stopPrice;		// please ignore when order type is TRAILING_STOP_MARKET
  	bool closePosition;         // if Close-All
  	std::string symbol;
  	size_t time;				// order time
  	std::string timeInForce;
  	std::string type;
  	double_type activatePrice;	    // activation price, only return with TRAILING_STOP_MARKET order
  	double_type priceRate;			// callback rate, only return with TRAILING_STOP_MARKET order
  	size_t updateTime;		        // update time
  	std::string workingType;
  	bool priceProtect;                      // if conditional order trigger is protected	
	std::string priceMatch;                 //price match mode
    std::string selfTradePreventionMode;    //self trading preventation mode
    size_t goodTillDate;                    //order pre-set auot cancel time for TIF GTD order

    static linear_future_order_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const linear_future_order_info_t &o);
};

struct inverse_future_order_info_t
{
  	double_type avgPrice;
  	std::string clientOrderId;
  	double_type cumBase;
  	double_type executedQty;
  	size_t orderId;
  	double_type origQty;
  	std::string origType;
  	double_type price;
  	bool reduceOnly;
  	std::string side;
  	std::string positionSide;
  	std::string status;
  	double_type stopPrice;			// please ignore when order type is TRAILING_STOP_MARKET
  	bool closePosition;   			// if Close-All
  	std::string symbol;
  	size_t time;				    // order time
  	std::string timeInForce;
  	std::string type;
  	double_type activatePrice;			// activation price, only return with TRAILING_STOP_MARKET order
  	double_type priceRate;				// callback rate, only return with TRAILING_STOP_MARKET order
  	size_t updateTime;		            // update time
  	std::string workingType;
  	bool priceProtect;                      // if conditional order trigger is protected
  	std::string priceMatch;                 //price match mode
  	std::string selfTradePreventionMode;    //self trading preventation mode
  
    static inverse_future_order_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const inverse_future_order_info_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#current-open-orders-user_data
// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#all-orders-user_data
struct orders_info_t {
    std::unordered_map<std::string, std::vector<order_info_t>> orders;

    static orders_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const orders_info_t &o);
};

struct margin_orders_info_t {
    std::unordered_map<std::string, std::vector<margin_order_info_t>> orders;

    static margin_orders_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const orders_info_t &o);
};

struct option_orders_info_t {
    std::unordered_map<std::string, std::vector<option_order_info_t>> orders;

    static option_orders_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const option_orders_info_t &o);
};

struct linear_future_orders_info_t {
    std::unordered_map<std::string, std::vector<linear_future_order_info_t>> orders;

    static linear_future_orders_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const linear_future_orders_info_t &o);
};

struct inverse_future_orders_info_t {
    std::unordered_map<std::string, std::vector<inverse_future_order_info_t>> orders;

    static inverse_future_orders_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const inverse_future_orders_info_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#new-order--trade
struct new_order_info_ack_t {
    std::string symbol;
    std::size_t orderId;
    std::string clientOrderId;
    std::size_t transactTime;

    static new_order_info_ack_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const new_order_info_ack_t &o);
};

struct new_order_info_result_t {
    std::string symbol;
    std::size_t orderId;
    std::string clientOrderId;
    std::size_t transactTime;
    double_type price;
    double_type origQty;
    double_type executedQty;
    double_type cummulativeQuoteQty;
    std::string status;
    std::string timeInForce;
    std::string type;
    std::string side;

    static new_order_info_result_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const new_order_info_result_t &o);
};

struct new_order_info_full_t {
    std::string symbol;
    std::size_t orderId;
    std::string clientOrderId;
    std::size_t transactTime;
    double_type price;
    double_type origQty;
    double_type executedQty;
    double_type cummulativeQuoteQty;
    std::string status;
    std::string timeInForce;
    std::string type;
    std::string side;
    bool isIsolated;
    struct fill_part {
        double_type price;
        double_type qty;
        double_type commission;
        std::string commissionAsset;
    };
    std::vector<fill_part> fills;

    static double_type avg_price(const std::vector<fill_part> &parts);
    static double_type max_price(const std::vector<fill_part> &parts);
    static double_type sum_amount(const std::vector<fill_part> &parts);
    static double_type sum_commission(const std::vector<fill_part> &parts);

    static new_order_info_full_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const new_order_info_full_t &o);
};

// https://developers.binance.com/docs/derivatives/option/trade
struct new_option_order_info_ack_t {
    std::size_t orderId;
    std::string symbol;
    double_type price;
    double_type quantity;
    std::string side;
    std::string type;
    std::size_t createDate;
    bool reduceOnly;
    bool postOnly;
    bool mmp;

    static new_option_order_info_ack_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const new_option_order_info_ack_t &o);
};

struct new_option_order_info_result_t {
    std::size_t orderId;
    std::string symbol;
    double_type price;
    double_type quantity;
    double_type executedQty;
    double_type fee;
    std::string side;
    std::string type;
    std::string timeInForce;
    bool reduceOnly;
    bool postOnly;
    std::size_t createTime;
    std::size_t updateTime;
    std::string status;
    double_type avgPrice;
    std::string clientOrderId;
    size_t priceScale;
    size_t quantityScale;
    std::string optionSide;
    std::string quoteAsset;
    bool mmp;

    static new_option_order_info_result_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const new_option_order_info_result_t &o);
};

struct linear_future_order_result_t
{
    std::string clientOrderId;
 	double_type cumQty;
 	double_type cumQuote;
 	double_type executedQty;
 	size_t orderId;
 	double_type avgPrice;
 	double_type origQty;
 	double_type price;
  	bool reduceOnly;
  	std::string side;
  	std::string positionSide;
  	std::string status;
  	double_type stopPrice;
  	bool closePosition;
  	std::string symbol;
  	std::string timeInForce;
  	std::string type;
  	std::string origType;
  	double_type activatePrice;
  	double_type priceRate;
 	size_t updateTime;
 	std::string workingType;
 	bool priceProtect;
 	std::string priceMatch;
 	std::string selfTradePreventionMode;
 	size_t goodTillDate;

    static linear_future_order_result_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const linear_future_order_result_t &o);
};

struct inverse_future_order_result_t
{
    std::string clientOrderId;
 	double_type cumQty;
 	double_type cumBase;
 	double_type executedQty;
 	size_t orderId;
 	double_type avgPrice;
 	double_type origQty;
 	double_type price;
  	bool reduceOnly;
  	std::string side;
  	std::string positionSide;
  	std::string status;
  	double_type stopPrice;
  	bool closePosition;
  	std::string symbol;
    std::string pair;
  	std::string timeInForce;
  	std::string type;
  	std::string origType;
  	double_type activatePrice;
  	double_type priceRate;
 	size_t updateTime;
 	std::string workingType;
 	bool priceProtect;
 	std::string priceMatch;
 	std::string selfTradePreventionMode;

    static inverse_future_order_result_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const inverse_future_order_result_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#test-new-order-trade
struct new_test_order_info_t {
    bool ok;

    static new_test_order_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const new_test_order_info_t &o);
};

struct new_order_resp_type
    :boost::variant<
         new_order_info_ack_t
        ,new_order_info_result_t
        ,new_order_info_full_t
        ,new_test_order_info_t
    >
{
    // ctor inheritance
    using boost::variant<
         new_order_info_ack_t
        ,new_order_info_result_t
        ,new_order_info_full_t
        ,new_test_order_info_t
    >::variant;

    std::pair<e_trade_resp_type, const void *>
    get_responce_type() const {
        if ( const auto *p = boost::get<new_order_info_ack_t>(this) ) {
            return {e_trade_resp_type::ACK, p};
        } else if ( const auto *p = boost::get<new_order_info_result_t>(this) ) {
            return {e_trade_resp_type::RESULT, p};
        } else if ( const auto *p = boost::get<new_order_info_full_t>(this) ) {
            return {e_trade_resp_type::FULL, p};
        } else if ( const auto *p = boost::get<new_test_order_info_t>(this) ) {
            return {e_trade_resp_type::TEST, p};
        }

        return {e_trade_resp_type::UNKNOWN, nullptr};
    }

    bool is_valid_responce_type()  const { const auto r =  get_responce_type(); return r.first != e_trade_resp_type::UNKNOWN; }
    bool is_ack_responce_type()    const { const auto r =  get_responce_type(); return r.first == e_trade_resp_type::ACK; }
    bool is_result_responce_type() const { const auto r =  get_responce_type(); return r.first == e_trade_resp_type::RESULT; }
    bool is_full_responce_type()   const { const auto r =  get_responce_type(); return r.first == e_trade_resp_type::FULL; }
    bool is_test_responce_type()   const { const auto r =  get_responce_type(); return r.first == e_trade_resp_type::TEST; }

    const new_order_info_ack_t& get_responce_ack() const {
        const auto r =  get_responce_type();
        assert(r.first == e_trade_resp_type::ACK);

        return *static_cast<const new_order_info_ack_t *>(r.second);
    }
    const new_order_info_result_t& get_responce_result() const {
        const auto r =  get_responce_type();
        assert(r.first == e_trade_resp_type::RESULT);

        return *static_cast<const new_order_info_result_t *>(r.second);
    }
    const new_order_info_full_t& get_responce_full() const {
        const auto r =  get_responce_type();
        assert(r.first == e_trade_resp_type::FULL);

        return *static_cast<const new_order_info_full_t *>(r.second);
    }
    const new_test_order_info_t& get_responce_test() const {
        const auto r =  get_responce_type();
        assert(r.first == e_trade_resp_type::TEST);

        return *static_cast<const new_test_order_info_t *>(r.second);
    }

    std::size_t get_order_id() const {
        const auto r =  get_responce_type();
        assert(
            r.first == e_trade_resp_type::ACK ||
            r.first == e_trade_resp_type::RESULT ||
            r.first == e_trade_resp_type::FULL
        );

        switch ( r.first ) {
            case e_trade_resp_type::ACK: return static_cast<const new_order_info_ack_t *>(r.second)->orderId;
            case e_trade_resp_type::RESULT: return static_cast<const new_order_info_result_t *>(r.second)->orderId;
            case e_trade_resp_type::FULL: return static_cast<const new_order_info_full_t *>(r.second)->orderId;;
            default: break;
        }

        assert(!"unreachable");

        return 0u;
    }

    static new_order_resp_type construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const new_order_resp_type &o);
};

struct new_option_order_resp_type
    :boost::variant<
         new_option_order_info_ack_t
        ,new_option_order_info_result_t
    >
{
    // ctor inheritance
    using boost::variant<
         new_option_order_info_ack_t
        ,new_option_order_info_result_t
    >::variant;

    std::pair<e_trade_resp_type, const void *>
    get_responce_type() const {
        if ( const auto *p = boost::get<new_option_order_info_ack_t>(this) ) {
            return {e_trade_resp_type::ACK, p};
        } else if ( const auto *p = boost::get<new_option_order_info_result_t>(this) ) {
            return {e_trade_resp_type::RESULT, p};
        } 

        return {e_trade_resp_type::UNKNOWN, nullptr};
    }

    bool is_valid_responce_type()  const { const auto r =  get_responce_type(); return r.first != e_trade_resp_type::UNKNOWN; }
    bool is_ack_responce_type()    const { const auto r =  get_responce_type(); return r.first == e_trade_resp_type::ACK; }
    bool is_result_responce_type() const { const auto r =  get_responce_type(); return r.first == e_trade_resp_type::RESULT; }

    const new_order_info_ack_t& get_responce_ack() const {
        const auto r =  get_responce_type();
        assert(r.first == e_trade_resp_type::ACK);

        return *static_cast<const new_order_info_ack_t *>(r.second);
    }
    const new_order_info_result_t& get_responce_result() const {
        const auto r =  get_responce_type();
        assert(r.first == e_trade_resp_type::RESULT);

        return *static_cast<const new_order_info_result_t *>(r.second);
    }
    
    std::size_t get_order_id() const {
        const auto r =  get_responce_type();
        assert(
            r.first == e_trade_resp_type::ACK ||
            r.first == e_trade_resp_type::RESULT
        );

        switch ( r.first ) {
            case e_trade_resp_type::ACK: return static_cast<const new_order_info_ack_t *>(r.second)->orderId;
            case e_trade_resp_type::RESULT: return static_cast<const new_order_info_result_t *>(r.second)->orderId;
            default: break;
        }

        assert(!"unreachable");

        return 0u;
    }

    static new_option_order_resp_type construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const new_option_order_resp_type &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#cancel-order-trade
struct cancel_order_info_t {
    std::string symbol;
    std::size_t orderId;
    std::string origClientOrderId;
    std::string clientOrderId;
    double_type price;
    double_type origQty;
    double_type executedQty;
    double_type cummulativeQuoteQty;
    std::string status;
    std::string timeInForce;
    std::string type;
    std::string side;

    static cancel_order_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const cancel_order_info_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#account-trade-list-user_data
struct my_trades_info_t {
    struct my_trade_info_t {
        std::string symbol;
        std::size_t id;
        std::size_t orderId;
        double_type price;
        double_type qty;
        double_type commission;
        std::string commissionAsset;
        std::size_t time;
        bool isBuyer;
        bool isMaker;
        bool isBestMatch;

        static my_trade_info_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const my_trade_info_t &o);
    };

    std::vector<my_trade_info_t> trades;

    static my_trades_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const my_trades_info_t &o);
};


/*************************************************************************************************/

// https://developers.binance.com/docs/derivatives/option/trade/Account-Trade-List
struct my_option_trades_info_t {
    struct my_option_trade_info_t {
        std::size_t id;
        std::size_t tradeId;
        std::size_t orderId;
        std::string symbol;
        double_type price;
        double_type quantity;
        double_type fee;
        double_type realizedProfit;
        std::string side;
        std::string type;
        double_type volatility;
        std::string liquidity;
        std::string quoteAsset;
        std::size_t time;
        std::size_t priceScale;
        std::size_t quantityScale;
        std::string optionSide;

        static my_option_trade_info_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const my_option_trade_info_t &o);
    };

    std::vector<my_option_trade_info_t> trades;

    static my_option_trades_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const my_option_trades_info_t &o);
};

/*************************************************************************************************/

// https://developers.binance.com/docs/derivatives/usds-margined-futures/trade/rest-api/Account-Trade-List
struct my_linear_future_trades_info_t {
    struct my_linear_future_trade_info_t {

        bool buyer;
        double_type commission;
        std::string commissionAsset;
        std::size_t id;
        bool maker;
        std::size_t orderId;
        double_type price;
        double_type qty;
        double_type quoteQty;
        double_type realizedPnl;
        std::string side;
        std::string positionSide;
        std::string symbol;
        std::size_t time;

        static my_linear_future_trade_info_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const my_linear_future_trade_info_t &o);
    };

    std::vector<my_linear_future_trade_info_t> trades;

    static my_linear_future_trades_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const my_linear_future_trades_info_t &o);
};

/*************************************************************************************************/

// https://developers.binance.com/docs/derivatives/usds-margined-futures/trade/rest-api/Account-Trade-List
struct my_inverse_future_trades_info_t {
    struct my_inverse_future_trade_info_t {

        std::string symbol;
        std::size_t id;
        std::size_t orderId;
        std::string pair;
        std::string side;
        double_type price;
        double_type qty;
        double_type realizedPnl;
        std::string marginAsset;
        double_type baseQty;
        double_type commission;
        std::string commissionAsset;
        std::size_t time;
        std::string positionSide;
        bool buyer;
        bool maker;        

        static my_inverse_future_trade_info_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const my_inverse_future_trade_info_t &o);
    };

    std::vector<my_inverse_future_trade_info_t> trades;

    static my_inverse_future_trades_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const my_inverse_future_trades_info_t &o);
};

/*************************************************************************************************/

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#start-user-data-stream-user_stream
struct start_user_data_stream_t {
    std::string listenKey;

    static start_user_data_stream_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const start_user_data_stream_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#keepalive-user-data-stream-user_stream
struct ping_user_data_stream_t {
    bool ok;

    static ping_user_data_stream_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const ping_user_data_stream_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#close-user-data-stream-user_stream
struct close_user_data_stream_t {
    bool ok;

    static close_user_data_stream_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const close_user_data_stream_t &o);
};

/*************************************************************************************************/

} // ns rest

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

namespace ws {

/*************************************************************************************************/

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/web-socket-streams.md#aggregate-trade-streams
struct agg_trade_t {
    std::string e; // Event type
    std::size_t E; // Event time
    std::string s; // Symbol
    std::size_t a; // Aggregate trade ID
    double_type p; // Price
    double_type q; // Quantity
    std::size_t f; // First trade ID
    std::size_t l; // Last trade ID
    std::size_t T; // Trade time
    bool m; // Is the buyer the market maker?
    bool M; // Ignore

    static agg_trade_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const agg_trade_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/web-socket-streams.md#trade-streams
struct trade_t {
    std::size_t E; // Event time
    std::string s; // Symbol
    std::size_t t; // Trade ID
    double_type p; // Price
    double_type q; // Quantity
    std::size_t b; // Buyer order ID
    std::size_t a; // Seller order ID
    std::size_t T; // Trade time
    bool m; // Is the buyer the market maker?
    bool M; // Ignore

    static trade_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const trade_t &o);
};

/*************************************************************************************************/

// https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#partial-book-depth-streams
struct part_depths_t {
    struct depth_t {
        double_type price;
        double_type amount;

        friend std::ostream &operator<<(std::ostream &os, const depth_t &o);
    };

    std::vector<depth_t> a;
    std::vector<depth_t> b;

    static part_depths_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const part_depths_t &o);
};

/*************************************************************************************************/

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/web-socket-streams.md#diff-depth-stream
struct diff_depths_t {
    struct depth_t {
        double_type price;
        double_type amount;

        friend std::ostream &operator<<(std::ostream &os, const depth_t &o);
    };

    std::size_t E;
    std::string s;
    std::size_t u;
    std::size_t U;
    std::vector<depth_t> a;
    std::vector<depth_t> b;

    static diff_depths_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const diff_depths_t &o);
};

/*************************************************************************************************/

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/web-socket-streams.md#klinecandlestick-streams
struct kline_t {
    std::size_t E; // Event time
    std::string s; // Symbol
    std::size_t t; // Kline start time
    std::size_t T; // Kline close time
    std::string i; // Interval
    std::size_t f; // First trade ID
    std::size_t L; // Last trade ID
    double_type o; // Open price
    double_type c; // Close price
    double_type h; // High price
    double_type l; // Low price
    double_type v; // Base asset volume
    std::size_t n; // Number of trades
    bool        x; // Is this kline closed?
    double_type q; // Quote asset volume
    double_type V; // Taker buy base asset volume
    double_type Q; // Taker buy quote asset volume

    static kline_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const kline_t &o);
    friend bool ohlc_equal(const kline_t &l, const kline_t &r);
};

std::ostream& ohlc(std::ostream &os, const kline_t &o);

/*************************************************************************************************/

// https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#individual-symbol-mini-ticker-stream
struct mini_ticker_t {
    std::size_t E; // Event time
    std::string s; // Symbol
    double_type c; // Close price
    double_type o; // Open price
    double_type h; // High price
    double_type l; // Low price
    double_type v; // Total traded base asset volume
    double_type q; // Total traded quote asset volume

    static mini_ticker_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const mini_ticker_t &o);
};

// https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#all-market-mini-tickers-stream
struct mini_tickers_t {
    std::map<std::string, mini_ticker_t> tickers;

    static mini_tickers_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const mini_tickers_t &o);
};

/*************************************************************************************************/

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/web-socket-streams.md#individual-symbol-ticker-streams
struct market_ticker_t {
    std::size_t E; // Event time
    std::string s; // Symbol
    double_type p; // Price change
    double_type P; // Price change percent
    double_type w; // Weighted average price
    double_type x; // First trade(F)-1 price (first trade before the 24hr rolling window)
    double_type c; // Last price
    double_type Q; // Last quantity
    double_type b; // Best bid price
    double_type B; // Best bid quantity
    double_type a; // Best ask price
    double_type A; // Best ask quantity
    double_type o; // Open price
    double_type h; // High price
    double_type l; // Low price
    double_type v; // Total traded base asset volume
    double_type q; // Total traded quote asset volume
    std::size_t O; // Statistics open time
    std::size_t C; // Statistics close time
    std::size_t F; // First trade ID
    std::size_t L; // Last trade Id
    std::size_t n; // Total number of trades

    static market_ticker_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const market_ticker_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/web-socket-streams.md#all-market-tickers-stream
struct markets_tickers_t {
    std::unordered_map<std::string, market_ticker_t> tickers;

    static markets_tickers_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const markets_tickers_t &o);
};

/*************************************************************************************************/

// https://developers.binance.com/docs/derivatives/usds-margined-futures/websocket-market-streams/Individual-Symbol-Ticker-Streams
struct linear_future_market_ticker_t {
    std::size_t E; // Event time
    std::string s; // Symbol
    double_type p; // Price change
    double_type P; // Price change percent
    double_type w; // Weighted average price
    double_type c; // Last price
    double_type Q; // Last quantity
    double_type o; // Open price
    double_type h; // High price
    double_type l; // Low price
    double_type v; // Total traded base asset volume
    double_type q; // Total traded quote asset volume
    std::size_t O; // Statistics open time
    std::size_t C; // Statistics close time
    std::size_t F; // First trade ID
    std::size_t L; // Last trade Id
    std::size_t n; // Total number of trades

    static linear_future_market_ticker_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const linear_future_market_ticker_t &o);
};

/*************************************************************************************************/

// https://developers.binance.com/docs/derivatives/usds-margined-futures/websocket-market-streams/Individual-Symbol-Ticker-Streams
struct inverse_future_market_ticker_t {
    std::size_t E; // Event time
    std::string s; // Symbol
    std::string ps; // Pair
    double_type p; // Price change
    double_type P; // Price change percent
    double_type w; // Weighted average price
    double_type c; // Last price
    double_type Q; // Last quantity
    double_type o; // Open price
    double_type h; // High price
    double_type l; // Low price
    double_type v; // Total traded base asset volume
    double_type q; // Total traded quote asset volume
    std::size_t O; // Statistics open time
    std::size_t C; // Statistics close time
    std::size_t F; // First trade ID
    std::size_t L; // Last trade Id
    std::size_t n; // Total number of trades

    static inverse_future_market_ticker_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const inverse_future_market_ticker_t &o);
};

/*************************************************************************************************/

// https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#individual-symbol-book-ticker-streams
struct book_ticker_t {
    std::size_t u;
    std::string s;
    double_type b;
    double_type B;
    double_type a;
    double_type A;

    static book_ticker_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const book_ticker_t &o);
};

/*************************************************************************************************/

// https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#individual-symbol-book-ticker-streams
struct option_ticker_t {

    std::string e;
    int64_t E;
    std::string s;
    double_type o;
    double_type h;
    double_type l;
    double_type c;
    double_type V;
    double_type A;
    double_type P;
    double_type p;
    double_type Q;
    std::string F;
    std::string L;
    int64_t n;
    double_type bo;
    double_type ao;
    double_type bq;
    double_type aq;
    double_type b;
    double_type a;
    double_type d;
    double_type t;
    double_type g;
    double_type v;
    double_type vo;
    double_type mp;
    double_type hl;
    double_type ll;
    double_type eep;
    
    static option_ticker_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const option_ticker_t &o);
};

/*************************************************************************************************/

// https://binance-docs.github.io/apidocs/voptions/en/#new-symbol-info
struct new_symbol_info_t {
    
    std::size_t E; // Event time
    size_t id;
    size_t cid;
    std::string u;
    std::string qa;
    std::string s;
    size_t unit;
    double_type mq;
    std::string d;
    double_type sp;
    std::size_t ed;

    static new_symbol_info_t construct(const flatjson::fjson& json);
    friend std::ostream& operator<<(std::ostream& os, const new_symbol_info_t& o);
};

/*************************************************************************************************/

struct new_linear_future_symbol_info_t {
    
    std::size_t E;      // Event Time
    std::string s;      // Symbol
    std::string ps;     // Pair
    std::string ct;     // Contract type
    size_t dt;          // Delivery date time 
    size_t ot;          // onboard date time 
    std::string cs;     // Contract status 

    static new_linear_future_symbol_info_t construct(const flatjson::fjson& json);
    friend std::ostream& operator<<(std::ostream& os, const new_linear_future_symbol_info_t& o);
};

/*************************************************************************************************/

struct new_inverse_future_symbol_info_t {
    
    std::size_t E;      // Event Time
    std::string s;      // Symbol
    std::string ps;     // Pair
    std::string ct;     // Contract type
    size_t dt;          // Delivery date time 
    size_t ot;          // onboard date time 
    std::string cs;     // Contract status 

    static new_inverse_future_symbol_info_t construct(const flatjson::fjson& json);
    friend std::ostream& operator<<(std::ostream& os, const new_inverse_future_symbol_info_t& o);
};

/*************************************************************************************************/

} // ns ws

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

namespace userdata {

/*************************************************************************************************/

// https://github.com/binance/binance-spot-api-docs/blob/master/user-data-stream.md#account-update
struct account_update_t {
    struct balance_t {
        std::string a;
        double_type f;
        double_type l;

        friend std::ostream& operator<<(std::ostream &os, const balance_t &o);
    };

    std::string e;
    std::size_t E;
    std::size_t u;
    std::unordered_map<std::string, balance_t> B;

    static account_update_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const account_update_t &o);
};

/*************************************************************************************************/

// https://github.com/binance/binance-spot-api-docs/blob/master/user-data-stream.md#balance-update
struct balance_update_t {
    std::string e;
    std::size_t E;
    std::string a;
    double_type d;
    std::size_t T;

    static balance_update_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const balance_update_t &o);
};

/*************************************************************************************************/

// https://github.com/binance/binance-spot-api-docs/blob/master/user-data-stream.md#order-update
struct order_update_t {
    std::string e;
    std::size_t E;
    std::string s;
    std::string c;
    std::string S;
    std::string o;
    std::string f;
    double_type q;
    double_type p;
    double_type P;
    double_type d;
    double_type F;
    std::string C;
    std::string x;
    std::string X;
    std::string r;
    std::size_t i;
    double_type l;
    double_type z;
    double_type L;
    double_type n;
    std::string N;
    std::size_t T;
    std::size_t t;
    std::size_t I;
    bool        w;
    bool        m;
    bool        M;
    std::size_t O;
    double_type Z;

    static order_update_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const order_update_t &o);
};

/*************************************************************************************************/

// wrapper for account_update_t and order_update_t
struct userdata_stream_t {
    std::string data;

    static userdata_stream_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const userdata_stream_t &o);
};

/*************************************************************************************************/

} // ns userdata

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

} // ns binapi

#endif // __binapi__types_hpp
