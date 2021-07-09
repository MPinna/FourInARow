/**
 * SECTION
 * cliSerMessages.hpp (Client and Server Messages) header file, describe packet used to:
 *  - Exchange information messages between client and server
 *  - Receive the opponent's information
 *  - Launch a challenge
 */
#ifndef _SERVER_ENCRYPTED_MESSAGES_H_
#define _SERVER_ENCRYPTED_MESSAGES_H_
#include "../../Utils/structures.hpp"
#include "../../Utils/constant.hpp"
#include "../crypto.hpp"
#include "../packet.hpp"
#include <vector>

struct ClientList
{
    unsigned short int _n_of_elem{0};
    std::vector<User>  users;

    void addUser(char *name);
    // DESCRIPTION: Member methods
    size_t serialize(unsigned char **data);
    size_t HtoN(unsigned char **data);
    size_t NtoH(unsigned char *ser_buf);
    void print();
}__attribute__((packed));

struct GameInfo 
{
    unsigned char       _ipaddr[ADDRESS_LENGTH];
    unsigned short int  _peer_port{0};
    unsigned short int  _pubkey_size{0};
    unsigned char       _opp_pubkey{NULL};
    Tag                 tag;
    
    // DESCRIPTION: Member methods
    void serialize(unsigned char *to_ser_buf);
    void deserialize(unsigned char *ser_buf);
}__attribute__((packed));
#endif