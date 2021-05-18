/**
 * DESCRIPTION
 * The client challenge response header file, describe packet used to authentication phase sent by client only
 */
#ifndef _CLIENT_AUTH_MESSAGES_H_
#define _CLIENT_AUTH_MESSAGES_H_
#include "Utils/constant.hpp"
#include "../sectype.hpp"

struct ClientHello
{
    unsigned int        _nounce{0};
    unsigned char       _username[_16_BYTES]{0};
    unsigned short int  _port_number {0};
    Signature           _signature;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}__attribute__((packed));

struct ClientResponse
{
    unsigned int        _server_nounce {0};
    unsigned short int  _dh_param_g {0};
    unsigned short int  _dh_key_size {0}; 
    unsigned char *     _dh_key;
    Signature           _signature;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}__attribute__((packed));
#endif

/**
 * DESCRIPTION peer to peer messages
 */
struct PeerHello
{
    unsigned char       _username[_16_BYTES]{0};
    int                 _nounce {0};
    Signature           _signature;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}__attribute__((packed));

struct PeerChallenge
{
    int                 _nounce {0};
    int                 _peer_nounce {0};
    short unsigned int  _dh_key_size {0};
    unsigned char *     _dh_key;
    Signature           _signature;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}__attribute__((packed));

struct PeerResponse
{
    int                 _peer_nounce {0};
    short unsigned int  _dh_key_size {0};
    unsigned char *     _dh_key;
    Signature           _signature;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}__attribute__((packed));
#endif