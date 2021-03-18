/**
 * SECTION
 * cliSerMessages.hpp (Client and Server Messages) header file, describe packet used to:
 *  - Exchange information messages between client and server
 *  - Receive the opponent's information
 *  - Launch a challenge
 */
#ifndef _CLIENTSERVERMESSAGES_H_
#define _CLIENTSERVERMESSAGES_H_
#include <string>
#include <vector>

struct clientListRequest // TOCHECK if you can send this packet even without randomness field
{
    /* data */
    std::string randomness {NULL};
    std::string gcm_tag {NULL};
};

struct serverListResponse
{
    /* data */
    short unsigned int num_of_clients {0};
    std::vector<std::string> clients_list {NULL};
    std::string gcm_tag {NULL};
};

struct clientGameRequest
{
    /* data */
    std::string opponent_username {NULL};
    std::string gcm_tag {NULL};
};

struct serverGameForward
{
    /* data */
    std::string opponent_username {NULL};
    std::string gcm_tag {NULL};
};


struct peerGameResponse
{
    /* data */
    std::string randomness {NULL};
    bool flag;
    std::string gcm_tag {NULL};
};


struct serverGameResponse
{
    /* data */
    std::string opponent_username {NULL};
    bool challenge;
    std::string gcm_tag {NULL};
};

struct serverGameResponseData
{
    /* data */
    std::string opponent_username {NULL};
    short unsigned int peer_port {0};
    short unsigned int pubkey_size {0};
    std::string opponent_pubkey {NULL};
    short unsigned int dh_par_p {0};
    short unsigned int dh_par_g {0};
    std::string gcm_tag {NULL};
};
#endif