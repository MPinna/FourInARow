#include <string>

/**
 * SECTION
 * The p2pChallResp (Peer to peer challenge response) header file, describe packet used to:
 *  - manage all messages exchanged between a Client and a Server during authentication phase
 */
struct peerHello
{
    /* data */
    std::string username {NULL};
    int nounce {0};
    short unsigned int sig_size {0};
    std::string signature {NULL};
};

struct peerChallenge
{
    /* data */
    int nounce {0};
    int peer_nounce {0};
    short unsigned int dh_key_size {0};
    std::string dh_key {NULL};
    short unsigned int sig_size {0};
    std::string signature {NULL};
};

struct peerResponse
{
    /* data */
    int peer_nounce {0};
    short unsigned int dh_key_size {0};
    std::string dh_key {NULL};
    short unsigned int sig_size {0};
    std::string signature {NULL};
};