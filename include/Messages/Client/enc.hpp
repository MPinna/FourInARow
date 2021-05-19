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

typedef struct  // TOCHECK if you can send this packet even without randomness field
{
    unsigned short int  _rand_size{0};
    unsigned char       _randomness[_16_BYTES]{0};
    AEAD                _tag;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}ListRequest, __attribute__((packed));

typedef struct 
{
    unsigned char   _opponent_username[_16_BYTES]{0};
    AEAD            _tag;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}GameRequest, __attribute__((packed));
#endif

/**
 * SECTION peer to peer messages
 */
typedef struct  // TOCHECK if it is possible to send without randomness exploiting AAD only
{
    unsigned char       _randomness[_16_BYTES];
    short unsigned int  _column_num {0};
    AEAD                _tag;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}GameMove, __attribute__((packed));

// TOCHECK we need something to close the game
typedef struct 
{
    unsigned char       _randomness[_16_BYTES];
    unsigned short int  _signal{0};
    AEAD                _tag;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}EndGame, __attribute__((packed));
#endif