#pragma once

#include <eosio/asset.hpp>
#include <eosio/privileged.hpp>
#include <eosio/singleton.hpp>
#include <eosio/system.hpp>
#include <eosio/time.hpp>
#include <eosio/binary_extension.hpp> 
#include <utils.hpp>

#include <optional>
#include <string>
#include <map>
#include <set>
#include <type_traits>


namespace db {

    template<typename table, typename Lambda>
    inline void set( table& tbl,
                     typename table::const_iterator& itr, 
                     const eosio::name& ram_payer,
                     Lambda&& setter )
   {
        if (itr == tbl.end()) {
            tbl.emplace(ram_payer, [&]( auto& p ) {
               setter(p, true);
            });
        } else {
            tbl.modify(itr, ram_payer, [&]( auto& p ) {
               setter(p, false);
            });
        }
    }

    template<typename table>
    inline void del( table& tbl, typename table::const_iterator& itr) 
    {
        if (itr != tbl.end()) {
            tbl.erase( itr );
        }
    }

}// namespace db


namespace amax {

using namespace std;
using namespace eosio;

#define HASH256(str) sha256(const_cast<char*>(str.c_str()), str.size())

static constexpr uint32_t MAX_ANS_KEY_SIZE      = 64;
static constexpr uint32_t MAX_CONTENT_SIZE      = 2048;
static constexpr uint32_t DAY_SECONDS           = 3600 * 24;
static constexpr uint32_t MONTH_SECONDS         = 3600 * 24 * 30;

static constexpr eosio::name SYS_BANK           {"amax.token"_n};
static constexpr symbol SYS_SYMBOL              = symbol(symbol_code("AMAX"), 8);
static constexpr uint64_t SYS_PRECISION         = 100'000'000;

#define TBL struct [[eosio::table, eosio::contract("ans")]]
#define NTBL(name) struct [[eosio::table(name), eosio::contract("ans")]]


namespace AnsType {
    static constexpr eosio::name ALIAS          { "alias"_n     }; //alias to account name/address

    static constexpr eosio::name A              { "a"_n         }; //IPV4 address
    static constexpr eosio::name AAAA           { "aaaa"_n      }; //IPv6 address
    static constexpr eosio::name CNAME          { "cname"_n     }; //alias to a-record

    static constexpr eosio::name URI            { "uri"_n       }; //URI info
    static constexpr eosio::name TXT            { "txt"_n       }; //generic textual info

    static constexpr eosio::name BTC            { "btc"_n       }; //Bitcoin addr
    static constexpr eosio::name ETH            { "eth"_n       }; //EVM type addr
}

const std::map<name, uint64_t> AnsTypeVals {
    { AnsType::ALIAS,           0 },
    { AnsType::A,               1 },
    { AnsType::AAAA,            2 },
    { AnsType::CNAME,           3 },
    { AnsType::URI,             10 },
    { AnsType::TXT,             11 },
};

struct conf_st {
    asset                       ns_monthly_fee              = asset(1000'0000, SYS_SYMBOL); //[ 0.1 AMAX  ]
    uint16_t                     ns_bid_increase_min_rate   = 1000;                         //10% * 10K (boost)
    uint16_t                     ns_max_pay_in_month        = 12;                           //12 months max to pay each time 
    uint16_t                     ns_advance_pay_in_month    = 1;       
};

NTBL("global") global_t {
    name                        admin;
    name                        fee_collector               = "amax.devfund"_n;
    asset                       ns_monthly_fee              = asset(1000'0000, SYS_SYMBOL); //[ 0.1 AMAX  ]
    uint16_t                    ns_bid_increase_min_rate    = 1000;                         //10% * 10K (boost)
    uint16_t                    ns_max_pay_in_month         = 12;                           //12 months max to pay each time 
    uint16_t                    ns_advance_pay_in_month     = 1;                            //1 month in advance to apy
    uint16_t                    ns_bid_lock_days            = 3;                            //every bid must locked by 3 days before redemption

    EOSLIB_SERIALIZE( global_t, (admin)(fee_collector)
                                (ns_monthly_fee)(ns_bid_increase_min_rate)
                                (ns_max_pay_in_month)(ns_advance_pay_in_month) )
};
typedef eosio::singleton< "global"_n, global_t > global_singleton;

//scope: AnsType: [ alias | a | aaaa | cname | eth | btc | uri | txt ]
TBL ans_registry_t {
    uint64_t                    id;                  //PK: range: 0-6 bytes, 7th byte reserved for scope
    string                      ans_name;            //UK. E.g. $alias
    string                      ans_content;
    name                        owner;               //current owner of the entry
    asset                       ask_price;           //ownership transfer upon bid price accepted by the owner, zero price means off-sales
    time_point_sec              created_at;
    time_point_sec              expired_at;          //ownership can be transferred upon new owner paying the fees

    ans_registry_t() {}
    ans_registry_t(const uint64_t& i): id(i) {}

    uint64_t primary_key()const { return id; }
    uint64_t by_owner()const    { return owner.value; }
    checksum256 by_name()const  { return HASH256(ans_name); }

    typedef eosio::multi_index<"ansregistry"_n, ans_registry_t,
        indexed_by<"owneridx"_n,    const_mem_fun<ans_registry_t, uint64_t,     &ans_registry_t::by_owner>>,
        indexed_by<"nameidx"_n,     const_mem_fun<ans_registry_t, checksum256,  &ans_registry_t::by_name>>
    > tbl_t;

    EOSLIB_SERIALIZE( ans_registry_t, (id)(ans_name)(ans_content)(owner)(ask_price)(created_at)(expired_at) )
};

//scope: [ans_type:0 | ans_id : 1-7 ] 
TBL ans_bid_t {
    name                        bidder;
    asset                       bid_price;
    time_point_sec              bidden_at;

    ans_bid_t() {}
    ans_bid_t(const name& b): bidder(b) {}
    
    uint64_t primary_key()const { return bidder.value; }
    uint64_t by_bid_price_inv()const { return std::numeric_limits< int64_t >::max() - bid_price.amount; }

    typedef eosio::multi_index<"ansbids"_n, ans_bid_t,
        indexed_by<"bidpriceidx"_n,     const_mem_fun<ans_bid_t, uint64_t,  &ans_bid_t::by_bid_price_inv>>
    > tbl_t;

    EOSLIB_SERIALIZE( ans_bid_t, (bidder)(bid_price)(bidden_at) )
};

} //namespace amax