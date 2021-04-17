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
#include "Utils/constant.hpp"

struct clientListRequest // TOCHECK if you can send this packet even without randomness field
{
    /* data */
    unsigned short int rand_size{0};
    unsigned char *randomness[_16_BYTES];
    unsigned short int gcm_tag_size{0};
    unsigned char *gcm_tag;
};

struct serverListResponse
{
    /* data */
    short unsigned int num_of_clients{0};
    unsigned char *clients_list;
    short unsigned int gcm_tag_size{0};
    unsigned char *gcm_tag;
};

struct clientGameRequest
{
    /* data */
    char opponent_username[_16_BYTES];
    int gcm_size;
    char *gcm_tag;
};

struct serverGameForward
{
    /* data */
    unsigned char *opponent_username[_16_BYTES];
    int gcm_size;
    unsigned char *gcm_tag;
};

struct peerGameResponse
{
    /* data */
    unsigned char randomness{NULL};
    bool flag;
    int gcm_size;
    unsigned char *gcm_tag;
};

struct serverGameResponse
{
    /* data */
    std::string opponent_username{NULL};
    enum challenge
    {
        ACCEPTED = 0,
        REJECTED = 1
    };
    int gcm_size;
    unsigned char *gcm_tag;
};

struct serverGameResponseData
{
    /* data */
    unsigned char *opponent_username;
    short unsigned int peer_port{0};
    short unsigned int pubkey_size{0};
    unsigned char *opponent_pubkey;
    short unsigned int dh_par_p{0};
    short unsigned int dh_par_g{0};
    short unsigned int gcm_size{0};
    unsigned char *gcm_tag;
};
#endif