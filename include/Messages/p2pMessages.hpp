#include <string>

/**
 * SECTION
 * p2pMessages (Peer to peer messages) header file, describe packet used to:
 *  - send the move to the adversaries
 *  - send a signal which terminate the challenge
 */
struct gameMove // TOCHECK if it is possible to send 
{
    /* data */
    std::string randomness {NULL};
    short unsigned int column_num {0};
    std::string gcm_tag {NULL};
};

// TODO gameEnd message
struct endGame // TOCHECK if it is possible to send
{
    /* data */
    std::string randomness {NULL};
    std::string signal {NULL};
    std::string gcm_tag {NULL};
};