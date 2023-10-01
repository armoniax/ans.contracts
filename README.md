# ans.contracts
Armonia Name Service (`ANS`)

# Purpose
In order to achieve the most decentralization, domain name service itself must be as well decentralized, hence the name of DDNS, i.e. decentralized domain name service. `ANS` is the official code name for `DDNS` implementation.

`ANS` is different from premimum account name bidding. Instead, it is created to provide domain name resolution on-chain!

# TLD domain names

Top-level domain names are reserved for general public to use only. Following are the list of reserved TLDs:

- com
- eth
- btc

The above list may be extended/updated by `ANS DAO` along the way.

# Supported ANS Types
| Type  | Description                                                                                           | Usages         |
|-------|-------------------------------------------------------------------------------------------------------|----------------|
| ALIAS | account alias, can be of any string, with the max length of 64 characters, unicode supported as well. | token transfer |
| A     | domain name including TLDs like `.com` `.eth` `.btc`                                                  | DDNS           |
| AAAA  | IPv4 E.g. `192.168.100.1`                                                                             | DDNS           |
| URI   | uri content                                                                                           | DDNS           |
| TXT   | any key-value record                                                                                  | DDNS           |

* Token transfers with `alias`

  * Token transfers take the following fields:
```
$from $to $quant $memo
```
  * for token transfer with account `alias`, it goes as following:
    - `$to` will be a special contract that parses memo and extracts the alias account out and get the actual destination account/address to use it inside an inline transfer
    - `memo` contains the account `alias` and if ultimate memo is provided, it goes like this: `$alias:$memo`

# Ownership cost

For the initial ownership, it costs `0.2 AMAX`. In order to make it effective, one must wait for 24 hours and let others bid for higher prices and the highest wins the ownership eventually whereas all the other bidders including the first requester will be all refunded while the initial requester will be rewarded. 

# Holding cost
The owner of each domain name must pay either `0.1 AMAX` or `1 MUSDT` for at least one month, otherwise risking losing the ownership. The maximum duration one can pay for owning a domain each time is set in a global state variable. 