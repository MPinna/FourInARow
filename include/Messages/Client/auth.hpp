/**
 * DESCRIPTION
 * The client challenge response header file, describe packet used to authentication phase sent by client only
 */
#ifndef _CLIENT_AUTH_MESSAGES_H_
#define _CLIENT_AUTH_MESSAGES_H_

#include "../../Utils/constant.hpp"
#include "../crypto.hpp"
#include <cstddef>

typedef struct __attribute__((packed)) 
{
    unsigned char       _username[USERNAME_LENGHT_16];
    unsigned short int  _port_number{0};
    unsigned int        _nonce{0};
    Digest              digest;
    Signature           signature;

    // DESCRIPTION: Setter
    void setUsername(char *user);
    // DESCRIPTION: Member methods
    void serialize(unsigned char *to_ser_buf);
    void deserialize(unsigned char *ser_buf);
} ClientHello;

typedef struct __attribute__((packed)) 
{
    unsigned int    _opp_nonce{0};
    DHKey           dh_key;
    Digest          digest;
    Signature       signature;

    // DESCRIPTION: Member methods
    void serialize(unsigned char *to_ser_buf);
    void deserialize(unsigned char *ser_buf);
} ClientResponse;

typedef struct __attribute__((packed))
{
    unsigned char   _username[USERNAME_LENGHT_16];
    unsigned int    _nonce{0};
    Digest          digest;
    Signature       signature;

    void setUsername(unsigned char *name);
    // DESCRIPTION: Member methods
    void serialize(unsigned char *to_ser_buf);
    void deserialize(unsigned char *ser_buf);
} PeerHello;

typedef struct __attribute__((packed))
{   
    unsigned int    _nonce{0};
    unsigned int    _opp_nonce{0};
    DHKey           dh_key;
    Digest          digest;
    Signature       signature;

    // DESCRIPTION: Member methods
    void serialize(unsigned char *to_ser_buf);
    void deserialize(unsigned char *ser_buf);
} PeerChallenge;

typedef struct __attribute__((packed)) 
{
    unsigned int    _opp_nonce{0};
    short int       _signal{-1};
    DHKey           dh_key;
    Digest          digest;
    Signature       signature;

    // DESCRIPTION: Member methods
    void serialize(unsigned char *to_ser_buf);
    void deserialize(unsigned char *ser_buf);
} PeerResponse; 
#endif