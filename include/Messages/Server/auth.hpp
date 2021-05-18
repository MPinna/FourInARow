/**
 * DESCRIPTION
 * The server challenge response header file, describe packet used to authentication phase sent by server only
 */
#ifndef _SERVER_AUTH_MESSAGES_H_
#define _SERVER_AUTH_MESSAGES_H_
#include "Messages/sectype.hpp"
#include "../sectype.hpp"

// TOCHECK review all struct -> remove redundant 

struct ServerHello
{
    unsigned short int  _cert_size {0};
    unsigned char *     _certificate; // TOCHECK cosi' oppure X509? 
    Signature           _signature;

    // DESCRIPTION: member structure
    void serialize(char * to_ser_buf);
    void deserialize(char * ser_buf);
}__attribute__((packed));

struct ServerChallenge
{
    unsigned int        _server_nounce {0};
    unsigned int        _client_nounce {0};
    unsigned short int  _dh_param_p {0};
    unsigned short int  _dh_param_g {0};
    unsigned short int  _dh_key_size {0};
    unsigned char *     _dh_key; 
    Signature           _signature;

    // DESCRIPTION: member structure
    void serialize(char * to_ser_buf);
    void deserialize(char * ser_buf);
}__attribute__((packed));
#endif