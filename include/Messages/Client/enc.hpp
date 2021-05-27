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
    unsigned char       _randomness[USERNAME_LENGHT_16];
    AEAD                _tag;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
} PlayerList;

typedef struct __attribute__((packed))
{
    unsigned char   _opponent_username[USERNAME_LENGHT_16];
    AEAD            _tag;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
} ChallengeRequest;

typedef struct __attribute__((packed))
{
    bool _answer;
    AEAD _tag;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
} ChallengeResponse;

typedef struct __attribute__((packed))
{
    // TOCHECK if it is possible to send without randomness exploiting AAD only
    short unsigned int  _column_num {0};
    unsigned char       _randomness[RANDOMNESS_LENGTH_16];
    AEAD                tag;

    // DESCRIPTION: Member methods
    // TODO generic methods to fill randomness with 16 random bytes
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
} GameMove;

// TOCHECK we need something to close the game
typedef struct __attribute__((packed))
{
    unsigned char       _randomness[RANDOMNESS_LENGTH_16];
    unsigned short int  _signal{0};
    AEAD                tag;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
} GameEnd;
#endif