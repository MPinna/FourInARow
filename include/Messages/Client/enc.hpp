/**
 * SECTION
 * cliSerMessages.hpp (Client and Server Messages) header file, describe packet used to:
 *  - Exchange information messages between client and server
 *  - Receive the opponent's information
 *  - Launch a challenge
 */
#ifndef _CLIENT_ENCRYPTED_MESSAGES_H_
#define _CLIENT_ENCRYPTED_MESSAGES_H_
#include "Utils/constant.hpp"

struct ListRequest // TOCHECK if you can send this packet even without randomness field
{
    /* data */
    unsigned short int  rand_size{0};
    unsigned char *     randomness[_16_BYTES];
    unsigned short int  gcm_tag_size{0};
    unsigned char *     gcm_tag;

    // SECTION: Member methods
    void serialize(ListRequest *packet, char *ser_buf);
    void deserilize(char *ser_buf, ListRequest *packet);
}__attribute__((packed));

struct GameRequest
{
    /* data */
    unsigned char       opponent_username[_16_BYTES];
    unsigned short int  gcm_size;
    unsigned char *     gcm_tag;

    // SECTION: Member methods
    void serialize(GameRequest *packet, char *ser_buf);
    void deserilize(char *ser_buf, GameRequest *packet);
}__attribute__((packed));
#endif

/**
 * SECTION peer to peer messages
 */
struct gameMove // TOCHECK if it is possible to send without randomness exploiting AAD only
{
    /* data */
    unsigned char       randomness[_16_BYTES]; // perche' non aggiungerla fuori ? Magari con una procedura apposita ? 
    short unsigned int  column_num {0};
    unsigned short int  gcm_size{0};
    unsigned char  *    gcm_tag;
}__attribute__((packed));

// TOCHECK gameEnd message
struct endGame
{
    /* data */
    unsigned char       randomness[_16_BYTES];
    unsigned short int  signal{0};
    unsigned short int  gcm_size{0};
    unsigned char  *    gcm_tag;
}__attribute__((packed));
#endif