/**
 * SECTION
 * The clientServerChallRes (Client and Server challenge response) header file, describe packet used to: 
 *  - manage all messages exchanged between a Client and a Server during authentication phase
 */
#ifndef _PEER_AUTH_MESSAGES_H_
#define _PEER_AUTH_MESSAGES_H_
#include "Utils/constant.hpp"

struct clientHello
{
    /* data size = 40 Bytes */
    unsigned int        nounce{0};
    unsigned char       username[_16_BYTES];
    unsigned short int  port_number {0};
    unsigned short int  sig_size {0};
    unsigned char *     signature;

    // SECTION: Member methods
    void serialize(clientHello *packet, char *ser_buf);
    void deserilize(char *ser_buf, clientHello *packet);
}__attribute__((packed));

struct clientResponse
{
    /* data size = 534 */
    unsigned int        server_nounce {0};
    unsigned short int  dh_param_g {0};
    unsigned short int  dh_key_size {0}; 
    unsigned char       *dh_key;
    unsigned short int  sig_size {0};
    unsigned char       *signature;

    // SECTION: Member methods
    void serialize(clientResponse *packet, char *ser_buf);
    void deserilize(char *ser_buf, clientResponse *packet);
}__attribute__((packed));
#endif

/**
 * SECTION peer to peer messages
 */
struct peerHello
{
    /* data */
    unsigned char *     username[_16_BYTES];
    int                 nounce {0};
    short unsigned int  sig_size {0};
    unsigned char *     signature;

    // SECTION: Member methods
    void serialize(peerHello *packet, char *ser_buf);
    void deserialize(char *ser_buf, peerHello *packet);
}__attribute__((packed));

struct peerChallenge
{
    /* data */
    int                 nounce {0};
    int                 peer_nounce {0};
    short unsigned int  dh_key_size {0};
    unsigned char *     dh_key;
    short unsigned int  sig_size {0};
    unsigned char *     signature;

    // SECTION: Member methods
    void serialize(peerChallenge *packet, char *ser_buf);
    void deserilize(char *ser_buf, peerChallenge *packet);
}__attribute__((packed));

struct peerResponse
{
    /* data */
    int                 peer_nounce {0};
    short unsigned int  dh_key_size {0};
    unsigned char *     dh_key;
    short unsigned int  sig_size {0};
    unsigned char *     signature;

    // SECTION: Member methods
    void serialize(peerResponse *packet, char *ser_buf);
    void deserilize(char *ser_buf, peerResponse *packet);
}__attribute__((packed));
#endif