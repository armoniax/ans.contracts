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


namespace amax {

using namespace std;
using namespace eosio;

#define HASH256(str) sha256(const_cast<char*>(str.c_str()), str.size())

static constexpr uint32_t MAX_LOGO_SIZE        = 512;
static constexpr uint32_t MAX_TITLE_SIZE       = 2048;

#define TBL struct [[eosio::table, eosio::contract("ans")]]
#define NTBL(name) struct [[eosio::table(name), eosio::contract("ans")]]


namespace AnsType {
    static constexpr eosio::name ALIAS          { "alias"_n     }; //alias to account name/address

    static constexpr eosio::name A              { "a"_n         }; //IPV4 address
    static constexpr eosio::name AAAA           { "aaaa"_n      }; //IPv6 address
    static constexpr eosio::name CNAME          { "cname"_n     }; //alias to a-record

    static constexpr eosio::name BTC            { "btc"_n       }; //Bitcoin addr
    static constexpr eosio::name ETH            { "eth"_n       }; //EVM type addr

    static constexpr eosio::name URI            { "uri"_n       }; //URI info
    static constexpr eosio::name TXT            { "txt"_n       }; //generic textual info
}

namespace AnsStatus {
    static constexpr eosio::name SUBMITTED      { "submitted"   };
    static constexpr eosio::name SUBMITTED      { "submitted"   };
}  


NTBL("global") global_t {
    name                        admin;   
    asset                       ns_monthly_fee_in_amax      = asset(1000'0000, "AMAX");     //[ 0.1 AMAX  ]
    asset                       ns_monthly_fee_in_usdt      = asset( 100'0000, "MUSDT");    //[   1 MUSDT ]
    uint8_t                     ns_max_pay_in_month         = 12;                           //12 months max to pay each time 
    uint8_t                     ns_advance_pay_in_month     = 1;                            //1 month in advance to apy
        
    EOSLIB_SERIALIZE( global_t, (admin)
                                (ns_monthly_fee_in_amax)(ns_monthly_fee_in_usdt)
                                (ns_max_pay_in_month)(ns_advance_pay_in_month) )
};

//Monthly fee for getting a domain
NTBL("priceconf") global_t {
    name                     admin;   

    EOSLIB_SERIALIZE( global_t, (admin) )
};

typedef eosio::singleton< "global"_n, global_t > global_singleton;

//Scope: _self
TBL ans_bidding_t {
    uint64_t                    id;                         //PK
    string                      ans_name;
    string                      ans_content;
};

//scope: AnsType, Eg. a | aaaa | cname | eth | btc | uri | txt
TBL ans_registry_t {
    uint64_t                    id;                         //PK
    string                      ans_name;                   //UK. E.g. 
    string                      ans_content;
    name                        owner; 
    time_point_sec              created_at;
    time_point_sec              expired_at = time_point();  //default is open for bidding
    
    ans_registry_t() {}
    ans_registry_t(const id& i): id(i) {}

    checksum256 by_ans_name() { return HASH256(ans_name); }

    uint64_t primary_key()const { return id; }

    typedef eosio::multi_index< "ansregistry"_n,  ans_registry_t> table;

    EOSLIB_SERIALIZE( ans_registry_t, (owner)(logo_uri)(org_name)(org_info)
                                    (dao_code)(reward_shared_ratio)(manifesto)(issuance_plan)
                                    (reward_shared_plan)(status)
                                    (created_at)(updated_at))
};


} //namespace amax