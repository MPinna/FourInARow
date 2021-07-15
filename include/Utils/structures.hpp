/**
 * DESCRIPTION
 * The aim of this file, is to provide all required structure used to represent complex data
 */
#if !defined(_STRUCTURE_H)
#define _STRUCTURE_H
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
    void update();
}__attribute__((packed));

struct ClientInfo
{
    std::string     username;
    bool            _status;
    EVP_PKEY *      pubkey;
    Packet          packet;
    SecurityCheck   check;

    ClientInfo()
        : username{NULL}, _status{false}, pubkey{NULL} {}

    void setUsername();
};

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

