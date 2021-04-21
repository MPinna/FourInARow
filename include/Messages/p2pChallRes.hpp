/**
 * SECTION
 * The p2pChallResp (Peer to peer challenge response) header file, describe packet used to:
 *  - manage all messages exchanged between a Client and a Server during authentication phase
 */
#ifndef _P2PCHALLRES_H_
#define _P2PCHALLRES_H_
#include "Utils/constant.hpp"

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