/**
 * DESCRIPTION
 * The server challenge response header file, describe packet used to authentication phase sent by server only
 */
#ifndef _SERVER_AUTH_MESSAGES_H_
#define _SERVER_AUTH_MESSAGES_H_
#include "../../Utils/constant.hpp"
#include "../crypto.hpp"

// TOCHECK review all struct -> remove redundant 

typedef struct __attribute__((packed))
{
    Certificate _server_cert;
    Auth        _auth_data;

    // DESCRIPTION: member structure
    void serialize(char * to_ser_buf);
    void deserialize(char * ser_buf);
} ServerHello;

typedef struct __attribute__((packed))
{
    unsigned short int  _dh_param_p {0};
    unsigned short int  _dh_param_a {0};
    unsigned char       _dh_key[DH_SECRET_LEN];
    Auth                _auth_data;

    // DESCRIPTION: member structure
    void serialize(char * to_ser_buf);
    void deserialize(char * ser_buf);
} ServerChallenge;
#endif