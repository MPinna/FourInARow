/**
 * SECTION
 * cliSerMessages.hpp (Client and Server Messages) header file, describe packet used to:
 *  - Exchange information messages between client and server
 *  - Receive the opponent's information
 *  - Launch a challenge
 */
#ifndef _SERVER_ENCRYPTED_H_
#define _SERVER_ENCRYPTED_H_
#include "Utils/constant.hpp"

struct ListResponse
{
    /* data */
    unsigned short int  num_of_clients{0};
    unsigned char *     clients_list;
    unsigned short int  gcm_tag_size{0};
    unsigned char *     gcm_tag;

    // SECTION: Member methods
    void serialize(ListResponse *packet, char *ser_buf);
    void deserilize(char *ser_buf, ListResponse *packet);
}__attribute__((packed));

struct OppGameResponse
{
    /* data */
    unsigned char       opponent_username[_16_BYTES];
    bool                flag;
    unsigned short int  gcm_size;
    unsigned char *     gcm_tag;

    // SECTION: Member methods
    void serialize(OppGameResponse *packet, char *ser_buf);
    void deserilize(char *ser_buf, OppGameResponse *packet);
}__attribute__((packed));

struct GameChallengeInfo
{
    /* data */
    unsigned char *     opponent_username;
    unsigned short int  peer_port{0};
    unsigned short int  pubkey_size{0};
    unsigned char *     opponent_pubkey;
    unsigned short int  dh_par_p{0};
    unsigned short int  dh_par_g{0};
    unsigned short int  gcm_size{0};
    unsigned char *     gcm_tag;

    // SECTION: Member methods
    void serialize(GameChallengeInfo *packet, char *ser_buf);
    void deserilize(char *ser_buf, GameChallengeInfo *packet);
}__attribute__((packed));
#endif