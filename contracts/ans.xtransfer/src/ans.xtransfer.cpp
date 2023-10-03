#include <ans.xtransfer/ans.xtransfer.hpp>
#include <ans/ans.db.hpp>
#include "utils.hpp"

#include "amax.token.hpp"


namespace amax {

using namespace eosio;
using namespace std;
using namespace db;

static string to_hex(const checksum256 &hashed) {
    // Construct variables
    string result;
    const char *hex_chars = "0123456789abcdef";
    const auto bytes = hashed.extract_as_byte_array();
    // Iterate hash and build result
    for (uint32_t i = 0; i < bytes.size(); ++i) {
        (result += hex_chars[(bytes.at(i) >> 4)]) += hex_chars[(bytes.at(i) & 0x0f)];
    }
    // Return string
    return result;
}

/**
 * @brief ANS entry applicant, owner or bidder to send AMAX
 *          @memo: $ans_alias
 */
[[eosio::on_notify("*::transfer")]]
void ans_xtransfer::ontransfer( name from, name to, asset quantity, string memo ) {
   if( _self == from ) return;
   if( to != _self ) return;

   CHECKC( quantity.amount > 0, err::NOT_POSITIVE, "must only transfer positive quantity" )
   auto from_bank = get_first_receiver();

   auto parts                 = split( memo, ":" );
   auto param_size            = parts.size();
   CHECKC( param_size <= 2,   err::PARAM_ERROR, "invalid memo format" );
   auto ans_alias             = param_size == 1 ? memo : string(parts[0]);
   auto submemo               = param_size == 1 ? "" :   string(parts[1]); 

   auto ans_registry          = ans_registry_t::tbl_t(_g.registry_contract, AnsType::ALIAS.value);
   auto ans_name_idx          = ans_registry.get_index<"nameidx"_n>();
   auto ans_reg_itr           = ans_name_idx.find( HASH256(ans_alias) );
   CHECKC( ans_reg_itr != ans_name_idx.end(), err::RECORD_NOT_FOUND, "ANS alias not found: " + ans_alias + " \nhash256(alias) = " + to_hex(HASH256(ans_alias)) )

   auto dest_account          = name( ans_reg_itr->ans_content );
   CHECKC( is_account( dest_account ), err::ACCOUNT_INVALID, "alias account invalid: " + dest_account.to_string() )

   TRANSFER( from_bank, dest_account, quantity, submemo );
}


}//namespace amax
