/**
 * DESCRIPTION
 * The client challenge response header file, describe packet used to authentication phase sent by client only
 */
#ifndef _CLIENT_AUTH_MESSAGES_H_
#define _CLIENT_AUTH_MESSAGES_H_
#include "../../Utils/constant.hpp"
#include "../crypto.hpp"

typedef struct
{
    unsigned int        _nounce{0};
    unsigned char       _username[_16_BYTES]{0};
    unsigned short int  _port_number {0};
    Signature           _signature;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
} ClientHello, __attribute__((packed));

typedef struct 
{
    unsigned int        _server_nounce {0};
    unsigned short int  _dh_param_g {0};
    unsigned short int  _dh_key_size {0}; 
    unsigned char *     _dh_key;
    Signature           _signature;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}ClientResponse, __attribute__((packed));
#endif

/**
 * DESCRIPTION peer to peer messages
 */
typedef struct 
{
    unsigned char       _username[_16_BYTES]{0};
    int                 _nounce {0};
    Signature           _signature;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}PeerHello, __attribute__((packed));

typedef struct 
{
    int                 _nounce {0};
    int                 _peer_nounce {0};
    short unsigned int  _dh_key_size {0};
    unsigned char *     _dh_key;
    Signature           _signature;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}PeerChallenge, __attribute__((packed));

typedef struct 
{
    int                 _peer_nounce {0};
    short unsigned int  _dh_key_size {0};
    unsigned char *     _dh_key;
    Signature           _signature;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}PeerResponse, __attribute__((packed));
#endif