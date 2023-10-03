#pragma once

#include <eosio/asset.hpp>
#include <eosio/privileged.hpp>
#include <eosio/singleton.hpp>
#include <eosio/system.hpp>
#include <eosio/time.hpp>
#include <eosio/binary_extension.hpp> 
// #include <utils.hpp>

#include <optional>
#include <string>
#include <map>
#include <set>
#include <type_traits>

#define HASH256(str) sha256(const_cast<char*>(str.c_str()), str.size())

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

#define TBL struct [[eosio::table, eosio::contract("ans")]]

namespace AnsType {
    static constexpr eosio::name ALIAS          { "alias"_n     }; //alias to account name/address

    static constexpr eosio::name BTC            { "btc"_n       }; //Bitcoin addr
    static constexpr eosio::name ETH            { "eth"_n       }; //EVM type addr
}

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


} //namespace amax