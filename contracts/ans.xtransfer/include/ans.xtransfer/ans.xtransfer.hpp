#pragma once
#include <wasm_db.hpp>
#include <eosio/asset.hpp>
#include <eosio/action.hpp>
#include <eosio/singleton.hpp>
#include <eosio/system.hpp>

#include <string>

namespace amax {

using std::string;
using std::vector;
         
using namespace wasm::db;
using namespace eosio;

static constexpr uint32_t RATIO_BOOST           = 10000;


#define CHECKC(exp, code, msg) \
   { if (!(exp)) eosio::check(false, string("[[") + std::to_string((int)code) + string("]] ") + msg); }

#define NTBL(name) struct [[eosio::table(name), eosio::contract("ans.xtransfer")]]

NTBL("global") global_t {
   name                       admin;
   name                       registry_contract;
   name                       fee_collector;
   uint64_t                   fee_rate = 0;        //E.g. 1 out of 10K          

   EOSLIB_SERIALIZE( global_t, (admin)(registry_contract)(fee_collector)(fee_rate) )
};
typedef eosio::singleton< "global"_n, global_t > global_singleton;

enum class err: uint8_t {
   NONE                 = 0,
   RECORD_NOT_FOUND     = 1,
   RECORD_EXISTING      = 2,
   SYMBOL_MISMATCH      = 4,
   PARAM_ERROR          = 5,
   MEMO_FORMAT_ERROR    = 6,
   PAUSED               = 7,
   NO_AUTH              = 8,
   NOT_POSITIVE         = 9,
   NOT_STARTED          = 10,
   OVERSIZED            = 11,
   TIME_EXPIRED         = 12,
   NOTIFY_UNRELATED     = 13,
   ACTION_REDUNDANT     = 14,
   ACCOUNT_INVALID      = 15,
   FEE_INSUFFICIENT     = 16,
   FIRST_CREATOR        = 17,
   STATUS_ERROR         = 18,
   SCORE_NOT_ENOUGH     = 19,
   NEED_REQUIRED_CHECK  = 20

};

/**
 * The `ans` sample system contract defines the structures and actions that allow users to create, issue, and manage tokens for AMAX based blockchains. It demonstrates one way to implement a smart contract which allows for creation and management of tokens. It is possible for one to create a similar contract which suits different needs. However, it is recommended that if one only needs a token with the below listed actions, that one uses the `ans` contract instead of developing their own.
 *
 * The `ans` contract class also implements two useful public static methods: `get_supply` and `get_balance`. The first allows one to check the total supply of a specified token, created by an account and the second allows one to check the balance of a token for a specified account (the token creator account has to be specified as well).
 *
 * The `ans` contract manages the set of tokens, accounts and their corresponding balances, by using two internal multi-index structures: the `accounts` and `stats`. The `accounts` multi-index table holds, for each row, instances of `account` object and the `account` object holds information about the balance of one token. The `accounts` table is scoped to an eosio account, and it keeps the rows indexed based on the token's symbol.  This means that when one queries the `accounts` multi-index table for an account name the result is all the tokens that account holds at the moment.
 *
 * Similarly, the `stats` multi-index table, holds instances of `currency_stats` objects for each row, which contains information about current supply, maximum supply, and the creator account for a symbol token. The `stats` table is scoped to the token symbol.  Therefore, when one queries the `stats` table for a token symbol the result is one single entry/row corresponding to the queried symbol token if it was previously created, or nothing, otherwise.
 */
class [[eosio::contract("ans.xtransfer")]] ans_xtransfer : public contract {
   private:
      global_singleton        _global;
      global_t                _g;

   public:
      using contract::contract;
  
      ans_xtransfer(eosio::name receiver, eosio::name code, datastream<const char*> ds): contract(receiver, code, ds),
         _global(get_self(), get_self().value) {  _g = _global.exists() ? _global.get() : global_t{}; }

      ~ans_xtransfer() { _global.set( _g, get_self() ); }

      ACTION init( const name& admin, const name& registry_contract ) { 
         CHECKC( has_auth( _self ) || has_auth( "armoniaadmin"_n ), err::NO_AUTH, "no auth" )

         _g.admin             = admin;
         _g.registry_contract = registry_contract;
      }

      ACTION setfee( const name& fee_collector, const uint64_t& fee_rate ) {
         CHECKC( has_auth( _self ) || has_auth( _g.admin ), err::NO_AUTH, "no auth" )

         _g.fee_collector     = fee_collector;
         _g.fee_rate          = fee_rate;
      }

      /**
       * @brief ANS entry applicant, owner or bidder to send AMAX
       *          @memo: 
       *          - format-1: $alias
       *          - format-2: $alias:$submemo
       */
      [[eosio::on_notify("*::transfer")]]
      void ontransfer( name from, name to, asset quantity, string memo );

   private:
      //private methods

};

} //namespace amax