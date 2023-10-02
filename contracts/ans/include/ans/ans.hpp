#pragma once


#include "ans.db.hpp"
#include <wasm_db.hpp>

#include <eosio/asset.hpp>
#include <eosio/action.hpp>

#include <string>

namespace amax {

using std::string;
using std::vector;

#define CHECKC(exp, code, msg) \
   { if (!(exp)) eosio::check(false, string("[[") + to_string((int)code) + string("]] ")  \
                                    + string("[[") + _self.to_string() + string("]] ") + msg); }

#define TRANSFER(bank, to, quantity, memo) \
    {	mtoken::transfer_action act{ bank, { {_self, active_perm} } };\
			act.send( _self, to, quantity , memo );}
         
using namespace wasm::db;
using namespace eosio;

static constexpr eosio::name active_perm{"active"_n};


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
class [[eosio::contract("ans")]] ans : public contract {
   private:
      dbc                     _dbc;
      global_singleton        _global;
      global_t                _g;

   public:
      using contract::contract;
  
   ans(eosio::name receiver, eosio::name code, datastream<const char*> ds): contract(receiver, code, ds),
         _dbc(get_self()),
         _global(get_self(), get_self().value)
    {
        _g = _global.exists() ? _global.get() : global_t{};
        
    }

    ~ans() { _global.set( _g, get_self() ); }

   //admin actions
   ACTION init( const name& admin, const name& fee_collector) {
      CHECKC( has_auth(_self) || has_auth( _g.admin ), err::NO_AUTH, "no auth" )

      _g.admin                                         = admin;
      _g.fee_collector                                 = fee_collector;
   };

   ACTION setconf( conf_st& conf ) { 
      require_auth( _g.admin );

      _g.ns_monthly_fee                        = conf.ns_monthly_fee;
      _g.ns_bid_increase_min_rate                      = conf.ns_bid_increase_min_rate;
      _g.ns_max_pay_in_month                           = conf.ns_max_pay_in_month;
      _g.ns_advance_pay_in_month                       = conf.ns_advance_pay_in_month;       
   }

   /**
    * @brief ANS entry applicant, owner or bidder to send AMAX
    *       - case-1: applicant to pay for new entry
    *          @memo: reg:$ans_scope:$ans_name:$ans_content:$ask_price
    *       - case-2: owner to pay for renewing/extending the period
    *          @memo: renew:$ans_scope:$ans_id
    *       - case-3: bidder to pay for bidding for an existing ANS entry
    *          @memo: bid:$ans_scope:$ans_id
    */
   [[eosio::on_notify("amax.token::transfer")]]
   void ontransfer( name from, name to, asset quantity, string memo );

   // owner actions
   ACTION setaskprice( const name& submitter, const name& ans_type, const uint64_t& ans_id, const asset& ask_price );
   ACTION acceptbid( const name& submitter, const name& ans_type, const uint64_t& ans_id, const name& bidder );
   ACTION setansvalue( const name& submitter, const name& ans_type, const uint64_t& ans_id, const string& ans_content );

   //bidder actions
   
   //query actions
   ACTION ansbidscope( const name& ans_type, const uint64_t& ans_id) {
         check( AnsTypeVals.find( ans_type ) != AnsTypeVals.end(), "ans type invalid: " + ans_type.to_string() );

         auto ans_type_id  = AnsTypeVals.at( ans_type );
         auto scope        = _get_ans_bid_scope( ans_type_id, ans_id );
         check( false,     "scope: " + to_string(scope) );
   };

   private:
      //private methods
      uint64_t _get_ans_bid_scope( const uint64_t& ans_type_id, const uint64_t& ans_id ) {
         return(  (ans_type_id << 56) | ans_id );
      }

      void _add_ans(       const name& submitter, 
                           const name& ans_type, 
                           const string& ans_name, 
                           const string& ans_content, 
                           const uint64_t& duration );

      void _renew_ans(     const name& owner, 
                           const name& ans_type, 
                           const uint64_t& ans_id,
                           const uint64_t& duration );

      void _bid_ans(       const name& bidder, 
                           const asset& bid_price, 
                           const name& ans_type, 
                           const uint64_t& ans_id );

};
} //namespace amax
