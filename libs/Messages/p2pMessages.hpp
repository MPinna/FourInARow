/**
 * SECTION
 * p2pMessages (Peer to peer messages) header file, describe packet used to:
 *  - send the move to the adversaries
 *  - send a signal which terminate the challenge
 */
#ifndef _P2PMESSAGES_H_
#define _P2PMESSAGES_H_
#include "Utils/constant.hpp"

struct gameMove // TOCHECK if it is possible to send 
{
    /* data */
    unsigned short int  rand_size{0};
    unsigned char       randomness[_16_BYTES];
    short unsigned int  column_num {0};
    unsigned short int  gcm_size{0};
    unsigned char  *    gcm_tag;
}__attribute__((packed));

// TODO gameEnd message
struct endGame // TOCHECK if it is possible to send
{
    /* data */
    unsigned short int  rand_size{0};
    unsigned char       randomness[_16_BYTES];
    unsigned short int  signal{0};
    unsigned short int  gcm_size{0};
    unsigned char  *    gcm_tag;
}__attribute__((packed));
#endif