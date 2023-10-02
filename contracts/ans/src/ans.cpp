#include <ans/ans.hpp>

#include <math.hpp>
#include <utils.hpp>

#define ALLOT_APPLE(farm_contract, lease_id, to, quantity, memo) \
    {   aplink::farm::allot_action(farm_contract, { {_self, active_perm} }).send( \
            lease_id, to, quantity, memo );}

namespace amax {

using namespace eosio;
using namespace std;
using namespace db;
using namespace wasm::safemath;


/**
 * @brief ANS entry applicant, owner or bidder to send AMAX
 *       - case-1: applicant to pay for new entry
 *          @memo: reg:$ans_type:$ans_name:$ans_content:$ask_price
 * 
 *       - case-2: to pay for renewing/extending the period
 *          @memo: renew:$owner:$ans_type:$ans_id
 * 
 *       - case-3: bidder to pay for bidding for an existing ANS entry
 *          @memo: bid:$ans_type:$ans_id
 */
[[eosio::on_notify("amax.token::transfer")]]
void ans::ontransfer( name from, name to, asset quantity, string memo ) {
   if( _self == from ) return;
   if( to != _self ) return;
   if( get_first_receiver() != SYS_BANK ) return;

   CHECKC( quantity.symbol == SYS_SYMBOL, err::SYMBOL_MISMATCH, "not sys_sysmbol" )
   CHECKC( quantity.amount > 0, err::NOT_POSITIVE, "must only transfer positive quantity" )

   auto parts                 = split( memo, ":" );
   auto param_size            = parts.size();
   CHECKC( param_size >= 3,   err::PARAM_ERROR, "invalid memo format" );
   auto flag                  = parts[0];
   
   CHECKC( quantity >= _g.ns_monthly_fee, err::FEE_INSUFFICIENT, "fees sent insufficient for a single month" )

   if( flag == "reg" ) {   //register a new ANS entry, @memo: reg:$ans_type:$ans_name:$ans_content
      CHECKC(param_size == 4, err::MEMO_FORMAT_ERROR, "reg memo format invalid" )
      auto ans_type           = name(parts[1]);
      auto ans_name           = string(parts[2]);
      auto ans_content        = string(parts[3]);
      CHECKC( ans_name.size() < MAX_ANS_KEY_SIZE, err::OVERSIZED, "ANS name oversized" )
      CHECKC( ans_content.size() < MAX_CONTENT_SIZE, err::OVERSIZED, "ANS content oversized" )
      CHECKC( AnsTypeVals.find( ans_type ) != AnsTypeVals.end(), err::PARAM_ERROR, "reg memo has incorrect ans_type" )
     
      auto duration_seconds   = mul( MONTH_SECONDS, 
                                    div( quantity.amount, _g.ns_monthly_fee.amount, SYS_PRECISION ), 
                                    SYS_PRECISION );
      _add_ans( from, ans_type, ans_name, ans_content, duration_seconds);


   } else if( flag == "renew" ) { //renew an existing ANS entry, @memo: renew:$owner:$ans_type:$ans_id
      CHECKC(parts.size() == 4, err::MEMO_FORMAT_ERROR, "renew memo format invalid" )
      auto owner              = name(parts[1]);
      auto ans_type           = name(parts[2]);
      auto ans_id             = stoi( string(parts[3]) );
      CHECKC( AnsTypeVals.find( ans_type ) != AnsTypeVals.end(), err::PARAM_ERROR, "renew memo has incorrect ans_type" )

      auto duration_seconds   = mul( MONTH_SECONDS, 
                                    div( quantity.amount, _g.ns_monthly_fee.amount, SYS_PRECISION ), 
                                    SYS_PRECISION );

      _renew_ans( owner, ans_type, ans_id, duration_seconds);

   } else if( flag == "bid" ) {  //bid an existing ANS entry, @memo: bid:$ans_scope:$ans_id
      CHECKC(parts.size() == 3, err::MEMO_FORMAT_ERROR, "bid memo format invalid" )
      auto ans_type           = name(parts[1]);
      auto ans_id             = stoi( string(parts[2]) );
   
      _bid_ans(from, quantity, ans_type, ans_id);

   } else {
      CHECKC( false, err::MEMO_FORMAT_ERROR, "memo header error" )
   }
}

// owner actions
ACTION ans::setaskprice( const name& submitter, const name& ans_type, const uint64_t& ans_id, const asset& ask_price ) {
   require_auth( submitter );

   auto ans_registry       = ans_registry_t::tbl_t(_self, ans_type.value);
   auto ans_itr            = ans_registry.find( ans_id );
   CHECKC( ans_itr != ans_registry.end(), err::RECORD_NOT_FOUND, "ans registry not found for ans_id: " + to_string(ans_id) )
   CHECKC( ans_itr->owner == submitter, err::NO_AUTH, "submitter is not the ans owner" )
   CHECKC( ans_itr->ask_price != ask_price, err::PARAM_ERROR, "ask price is the same" )
   CHECKC( ans_itr->ask_price.amount >= 0, err::NOT_POSITIVE, "ask price cannot be negative" )

   db::set(ans_registry, ans_itr, _self, [&]( auto& r, bool is_new ) {
      r.ask_price         = ask_price;
   });
};

ACTION ans::acceptbid( const name& submitter, const name& ans_type, const uint64_t& ans_id, const name& bidder ) {
   require_auth( submitter );
   
   CHECKC( AnsTypeVals.find( ans_type ) != AnsTypeVals.end(), err::PARAM_ERROR, "ans_type invalid: " + ans_type.to_string() )
   auto ans_type_id        = AnsTypeVals.at( ans_type );
   auto scope              = _get_ans_bid_scope( ans_type_id, ans_id );
   auto ans_bids           = ans_bid_t::tbl_t(_self, scope);
   auto ans_bid_itr        = ans_bids.find( ans_id );
   CHECKC( ans_bid_itr != ans_bids.end(), err::RECORD_NOT_FOUND, "ans bid not found for ans_id: " + to_string(ans_id) )
   auto bid_price          = ans_bid_itr->bid_price;

   auto ans_registry       = ans_registry_t::tbl_t(_self, ans_type.value);
   auto ans_itr            = ans_registry.find( ans_id );
   CHECKC( ans_itr != ans_registry.end(), err::RECORD_NOT_FOUND, "ans registry not found for ans_id: " + to_string(ans_id) )
   CHECKC( ans_itr->owner == submitter, err::NO_AUTH, "submitter is not the ans owner" )

   //ownership transfer
   db::set( ans_registry, ans_itr, _self, [&]( auto& r, bool is_new ) {
      r.owner              = ans_bid_itr->bidder; 
   });

   auto fee = bid_price;
   fee.amount = div( mul( bid_price.amount, _g.ns_bid_transfer_fee_rate, SYS_PRECISION ), RATIO_BOOST, SYS_PRECISION ); 
   TRANSFER( SYS_BANK, _g.fee_collector, fee, "ans bid transfer fee for bidder: " + ans_bid_itr->bidder.to_string() )
   bid_price -= fee;
   if( bid_price.amount > 0 ) {
      TRANSFER( SYS_BANK, submitter, bid_price, "accept ans bid by: " + ans_bid_itr->bidder.to_string() )
   }
   db::del( ans_bids, ans_bid_itr );
};

ACTION ans::setansvalue( const name& submitter, const name& ans_type, const uint64_t& ans_id, const string& ans_content ) {
   require_auth( submitter );

   auto ans_registry       = ans_registry_t::tbl_t(_self, ans_type.value);
   auto ans_itr            = ans_registry.find( ans_id );
   CHECKC( ans_content.size() < MAX_CONTENT_SIZE, err::PARAM_ERROR, "ans_content oversized" )
   CHECKC( ans_itr != ans_registry.end(), err::RECORD_NOT_FOUND, "ans registry not found for ans_id: " + to_string(ans_id) )
   CHECKC( ans_itr->owner == submitter, err::NO_AUTH, "submitter is not the ans owner" )
   CHECKC( ans_itr->ans_content != ans_content, err::PARAM_ERROR, "ans_content is the same" )

   db::set(ans_registry, ans_itr, _self, [&]( auto& r, bool is_new ) {
      r.ans_content         = ans_content;
   });
}

//bidder actions
ACTION ans::cancelbid( const name& submitter, const name& ans_type, const uint64_t& ans_id ) {
   require_auth( submitter );
   
   CHECKC( AnsTypeVals.find( ans_type ) != AnsTypeVals.end(), err::PARAM_ERROR, "ans_type invalid: " + ans_type.to_string() )
   auto ans_type_id        = AnsTypeVals.at( ans_type );
   auto scope              = _get_ans_bid_scope( ans_type_id, ans_id );
   auto ans_bids           = ans_bid_t::tbl_t(_self, scope);
   auto ans_bid_itr        = ans_bids.find( ans_id );
   CHECKC( ans_bid_itr != ans_bids.end(), err::RECORD_NOT_FOUND, "ans id not found with bids: " + to_string(ans_id) )
   CHECKC( ans_bid_itr->bidder == submitter, err::NO_AUTH, "no auth to cancel bid for " + ans_bid_itr->bidder.to_string() )

   auto bid_price          = ans_bid_itr->bid_price;
   TRANSFER( SYS_BANK, submitter, bid_price, "ans bid refund" )
   db::del( ans_bids, ans_bid_itr );
}

////////////////////////////////////   private functions    /////////////////////////////////////////////////////////// 

void ans::_add_ans(        const name& submitter, 
                           const name& ans_type, 
                           const string& ans_name, 
                           const string& ans_content, 
                           const uint64_t& duration ) {

   auto ans_registry       = ans_registry_t::tbl_t(_self, ans_type.value);
   auto ans_name_idx       = ans_registry.get_index<"nameidx"_n>();
   auto ans_reg_itr        = ans_name_idx.find( HASH256(ans_name) );
   CHECKC( ans_reg_itr == ans_name_idx.end(), err::RECORD_EXISTING, "ans registry already exists: " + ans_name )

   auto ans_itr            = ans_registry.end();
   db::set(ans_registry, ans_itr, _self, [&]( auto& r, bool is_new ) {
      r.id                 = ans_registry.available_primary_key();
      r.ans_name           = ans_name;
      r.ans_content        = ans_content;
      r.owner              = submitter;
      r.ask_price          = asset(0, SYS_SYMBOL);
      r.created_at         = current_time_point();
      r.expired_at         = r.created_at + duration;
   });
}
    
void ans::_renew_ans(      const name& owner, 
                           const name& ans_type, 
                           const uint64_t& ans_id, 
                           const uint64_t& duration ) {

   auto ans_registry       = ans_registry_t::tbl_t(_self, ans_type.value);
   auto ans_itr            = ans_registry.find( ans_id );

   db::set(ans_registry, ans_itr, _self, [&]( auto& r, bool is_new ) {
      CHECKC( !is_new, err::RECORD_NOT_FOUND, "ans registry not found for ans_id: " + to_string(ans_id) )

      r.expired_at         = r.expired_at + duration;
   });
}

void ans::_bid_ans(        const name& bidder, 
                           const asset& bid_price, 
                           const name& ans_type, 
                           const uint64_t& ans_id ) {

   CHECKC( AnsTypeVals.find( ans_type ) != AnsTypeVals.end(), err::PARAM_ERROR, "renew memo has incorrect ans_type" )
   auto ans_type_id        = AnsTypeVals.at( ans_type );
   auto scope              = _get_ans_bid_scope( ans_type_id, ans_id );

   auto ans_bids           = ans_bid_t::tbl_t(_self, scope);
   auto ans_bid_itr        = ans_bids.find( ans_id );
   db::set(ans_bids, ans_bid_itr, _self, [&]( auto& b, bool is_new ) {
      if( is_new ) {
         b.bidder          = bidder;
         b.bid_price       = bid_price;
         b.bidden_at       = current_time_point();

      } else {
         b.bid_price       += bid_price;
         b.bidden_at       = current_time_point();
      }
   });

   auto ans_registry       = ans_registry_t::tbl_t(_self, ans_type.value);
   auto ans_reg_itr        = ans_registry.find( ans_id );
   CHECKC( ans_reg_itr != ans_registry.end(), err::RECORD_NOT_FOUND, "ANS registry not found: " + ans_reg_itr->ans_name )
   CHECKC( ans_reg_itr->ask_price.amount > 0, err::NOT_STARTED, "ANS not yet for sale: " + ans_reg_itr->ans_name )

   if( ans_bid_itr->bid_price >= ans_reg_itr->ask_price ) { //transfer ownership
      db::set(ans_registry, ans_reg_itr, _self, [&]( auto& r, bool is_new ) {
         r.owner           = bidder;
      });

      db::del( ans_bids, ans_bid_itr);
   }
}

}//namespace amax
