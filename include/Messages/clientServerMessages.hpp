/**
 * SECTION
 * cliSerMessages.hpp (Client and Server Messages) header file, describe packet used to:
 *  - Exchange information messages between client and server
 *  - Receive the opponent's information
 *  - Launch a challenge
 */
#ifndef _CLIENTSERVERMESSAGES_H_
#define _CLIENTSERVERMESSAGES_H_
#include "Utils/constant.hpp"

struct clientListRequest // TOCHECK if you can send this packet even without randomness field
{
    /* data */
    unsigned short int  rand_size{0};
    unsigned char *     randomness[_16_BYTES];
    unsigned short int  gcm_tag_size{0};
    unsigned char *     gcm_tag;

    // SECTION: Member methods
    void serialize(clientListRequest *packet, char *ser_buf);
    void deserilize(char *ser_buf, clientListRequest *packet);
}__attribute__((packed));

struct serverListResponse
{
    /* data */
    unsigned short int  num_of_clients{0};
    unsigned char *     clients_list;
    unsigned short int  gcm_tag_size{0};
    unsigned char *     gcm_tag;

    // SECTION: Member methods
    void serialize(serverListResponse *packet, char *ser_buf);
    void deserilize(char *ser_buf, serverListResponse *packet);
}__attribute__((packed));

struct clientGameRequest
{
    /* data */
    unsigned char       opponent_username[_16_BYTES];
    unsigned short int  gcm_size;
    unsigned char *     gcm_tag;

    // SECTION: Member methods
    void serialize(clientGameRequest *packet, char *ser_buf);
    void deserilize(char *ser_buf, clientGameRequest *packet);
}__attribute__((packed));

struct serverGameForward
{
    /* data */
    unsigned char *     opponent_username[_16_BYTES];
    unsigned short int  gcm_size;
    unsigned char *     gcm_tag;

    // SECTION: Member methods
    void serialize(serverGameForward *packet, char *ser_buf);
    void deserilize(char *ser_buf, serverGameForward *packet);
}__attribute__((packed));

struct peerGameResponse
{
    /* data */
    unsigned char       randomness[_16_BYTES];
    bool                flag;
    unsigned short int  gcm_size;
    unsigned char *     gcm_tag;

    // SECTION: Member methods
    void serialize(peerGameResponse *packet, char *ser_buf);
    void deserilize(char *ser_buf, peerGameResponse *packet);
}__attribute__((packed));

struct serverGameResponse
{
    /* data */
    unsigned char       opponent_username[_16_BYTES];
    enum challenge
    {
        ACCEPTED = 0,
        REJECTED = 1
    };
    unsigned short int  gcm_size;
    unsigned char *     gcm_tag;

    // SECTION: Member methods
    void serialize(serverGameResponse *packet, char *ser_buf);
    void deserilize(char *ser_buf, serverGameResponse *packet);
}__attribute__((packed));

struct serverGameResponseData
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
    void serialize(serverGameResponseData *packet, char *ser_buf);
    void deserilize(char *ser_buf, serverGameResponseData *packet);
}__attribute__((packed));
#endif