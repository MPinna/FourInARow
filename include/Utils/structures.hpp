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
    size_t _received;
    size_t _next;
    size_t _expected;
    
    // Default constructor
    SecurityCheck()
        : _received{0}, _next{0}, _expected{0} {}
    // Class methods
    void updateFields();
};

struct ClientInfo
{
    std::string     username;
    unsigned char   ip_addr[15]; // TOCHECK
    bool            _status;
    EVP_PKEY *      pubkey;
    Packet          packet;
    SecurityCheck   check;

    ClientInfo()
        : username{NULL}, _status{false}, pubkey{NULL} {}

    void setUsername();
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

