/**
 * SECTION
 * p2pMessages (Peer to peer messages) header file, describe packet used to:
 *  - send the move to the adversaries
 *  - send a signal which terminate the challenge
 */
#ifndef _P2PMESSAGES_H_
#define _P2PMESSAGES_H_
#include <string>

struct gameMove // TOCHECK if it is possible to send 
{
    /* data */
    unsigned short int  rand_size{0};
    unsigned char       *randomness;
    short unsigned int  column_num {0};
    unsigned short int  gcm_size{0};
    unsigned char       *gcm_tag;
};

// TODO gameEnd message
struct endGame // TOCHECK if it is possible to send
{
    /* data */
    unsigned short int  rand_size{0};
    unsigned char       *randomness;
    unsigned short int  signal{0};
    unsigned short int  gcm_size{0};
    unsigned char       *gcm_tag;
};
#endif