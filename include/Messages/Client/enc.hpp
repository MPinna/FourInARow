/**
 * SECTION
 * cliSerMessages.hpp (Client and Server Messages) header file, describe packet used to:
 *  - Exchange information messages between client and server
 *  - Receive the opponent's information
 *  - Launch a challenge
 */
#ifndef _CLIENT_ENCRYPTED_MESSAGES_H_
#define _CLIENT_ENCRYPTED_MESSAGES_H_

#include "../../Utils/constant.hpp"
#include "../crypto.hpp"

typedef struct __attribute__((packed)) 
{
    // TOCHECK if you can send this packet even without randomness field
    unsigned char   _randomness[RANDOMNESS_LENGTH_16];
    Tag             tag;

    // TODO function to set N bytes at random in a string
    // DESCRIPTION: Member methods
    void serialize(unsigned char *to_ser_buf);
    void deserialize(unsigned char *ser_buf);
} PlayerList;

typedef struct __attribute__((packed))
{
    unsigned char   _opp_username[USERNAME_LENGHT_16];
    Tag             tag;

    // DESCRIPTION: Member methods
    void serialize(unsigned char *to_ser_buf);
    void deserialize(unsigned char *ser_buf);
} ChallengeRequest;

typedef struct __attribute__((packed))
{
    bool    _answer;
    Tag     tag;

    // DESCRIPTION: Member methods
    void serialize(unsigned char *to_ser_buf);
    void deserialize(unsigned char *ser_buf);
} ChallengeResponse;

typedef struct __attribute__((packed))
{
    unsigned short int  _column_num {0};
    unsigned char       _randomness[RANDOMNESS_LENGTH_16];
    Tag                 tag;

    // DESCRIPTION: Member methods
    void serialize(unsigned char *to_ser_buf);
    void deserialize(unsigned char *ser_buf);
} GameMove;

// TOCHECK we need something to close the game
typedef struct __attribute__((packed))
{
    unsigned short int  _signal{0};
    unsigned char       _randomness[RANDOMNESS_LENGTH_16];
    Tag                 tag;

    // DESCRIPTION: Member methods
    void serialize(unsigned char *to_ser_buf);
    void deserialize(unsigned char *ser_buf); 
} GameEnd;
#endif