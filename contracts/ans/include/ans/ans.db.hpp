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
static constexpr uint32_t MAX_TITLE_SIZE        = 2048;

#define TBL struct [[eosio::table, eosio::contract("ans")]]
#define NTBL(name) struct [[eosio::table(name), eosio::contract("ans")]]

namespace ProducerStatus {
    static constexpr eosio::name DISABLE     { "disable"_n   };
    static constexpr eosio::name ENABLE      { "enable"_n  };
}

namespace AnsType {
    static constexpr eosio::name TXT            { "txt"_n        }; //any textual info, E.g. ETH address
    static constexpr eosio::name URI            { "uri"_n        }; //URI info
    static constexpr eosio::name ADDR           { "addr"_n       }; //blockchain address like btc/eth address
    static constexpr eosio::name A              { "a"_n          }; //IPV4 address
    static constexpr eosio::name AAAA           { "aaaa"_n       }; //IPv6 address
    static constexpr eosio::name CNAME          { "cname"_n      }; //alias to a-record
}

NTBL("global") global_t {
    name                     admin;   

    EOSLIB_SERIALIZE( global_t, (admin) )
};

typedef eosio::singleton< "global"_n, global_t > global_singleton;

//scope: AnsType
TBL ans_account_t {
    name                      owner; 
    name                        status;                     // disable | enable
    time_point_sec              created_at;
    time_point_sec              updated_at;
    
    ans_account_t() {}
    ans_account_t(const name& i): owner(i) {}

    uint64_t primary_key()const { return owner.value ; }

    typedef eosio::multi_index< "producers"_n,  ans_account_t> table;

    EOSLIB_SERIALIZE( ans_account_t, (owner)(logo_uri)(org_name)(org_info)
                                    (dao_code)(reward_shared_ratio)(manifesto)(issuance_plan)
                                    (reward_shared_plan)(status)
                                    (created_at)(updated_at))
};

TBL auth_t {
    name                        auth;              //PK
    set<name>                   actions;              //set of action types

    auth_t() {}
    auth_t(const name& i): auth(i) {}

    uint64_t primary_key()const { return auth.value; }

    typedef eosio::multi_index< "auths"_n,  auth_t > idx_t;

    EOSLIB_SERIALIZE( auth_t, (auth)(actions) )
};


} //namespace amax