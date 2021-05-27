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
    // TOCHECK here we don't need auth data (correct?)
    Certificate _certificate;

    // DESCRIPTION: member structure
    void serialize(unsigned char * to_ser_buf);
    void deserialize(unsigned char * ser_buf);
} ServerHello;

typedef struct __attribute__((packed))
{
    unsigned int    _dh_param_p {0};
    unsigned int    _dh_param_g {0};
    DHKey           dh_key;
    Digest          digest;
    Signature       signature;

    // DESCRIPTION: member structure
    void serialize(unsigned char * to_ser_buf);
    void deserialize(unsigned char * ser_buf);
} ServerChallenge;
#endif