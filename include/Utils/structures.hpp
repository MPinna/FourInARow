/**
 * DESCRIPTION
 * The aim of this file, is to provide all required structure used to represent complex data
 */
#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_
#include "../Messages/packet.hpp"
#include "constant.hpp"
#include <cstring>

struct SecurityCheck
{
    unsigned int    _nonce;
    size_t          _received;
    size_t          _expected;
    
    // Default constructor
    SecurityCheck()
        : _nonce{0}, _received{0}, _expected{0} {}
    // Class methods
    void updateFields();
};

struct ClientInfo
{
    std::string     _username;
    std::string     _ip_addr; // TOCHECK
    bool            _status;
    EVP_PKEY *      _pubkey;
    ESP             packet;
    SecurityCheck   check;

    ClientInfo()
        : _username{NULL}, _status{false}, _pubkey{NULL} {}

    int setUsername(char *data);
};

// TOCHECK do we need this ? 
struct GameInfo
{
    std::string username;
    std::string opp_username;
    std::string game_port;
    std::string opp_game_port;
    EVP_PKEY *privkey;
    EVP_PKEY *peerkey;
};
#endif // _STRUCTURE_H

