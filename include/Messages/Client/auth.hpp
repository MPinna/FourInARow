/**
 * DESCRIPTION
 * The clientServerChallRes (Client and Server challenge response) header file, describe packet used to: 
 *  - manage all messages exchanged between a Client and a Server during authentication phase
 */
#ifndef _CLIENT_AUTH_MESSAGES_H_
#define _CLIENT_AUTH_MESSAGES_H_
#include "Utils/constant.hpp"

struct clientHello
{
    /* data size = 40 Bytes */
    unsigned int        _nounce{0};
    unsigned char       _username[_16_BYTES];
    unsigned short int  _port_number {0};
    Signature           _signature;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}__attribute__((packed));

struct clientResponse
{
    /* data size = 534 */
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
struct peerHello
{
    /* data */
    unsigned char *     _username[_16_BYTES]{0};
    int                 _nounce {0};
    Signature           _signature;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}__attribute__((packed));

struct peerChallenge
{
    /* data */
    int                 _nounce {0};
    int                 _peer_nounce {0};
    short unsigned int  _dh_key_size {0};
    unsigned char *     _dh_key;
    Signature           _signature;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}__attribute__((packed));

struct peerResponse
{
    /* data */
    int                 _peer_nounce {0};
    short unsigned int  _dh_key_size {0};
    unsigned char *     _dh_key;
    Signature           _signature;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}__attribute__((packed));
#endif