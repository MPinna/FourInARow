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

struct PlayerList
{
    // TOCHECK if you can send this packet even without randomness field
    unsigned char   _randomness[RANDOMNESS_LENGTH_16];

    // Setter 
    // TODO function to set N bytes at random in a string
    int setRandomness();
    // Member methods
    size_t serialize(unsigned char **data);
    size_t HtoN(unsigned char **data);
    size_t NtoH(unsigned char *ser_buf);
    void print();
}__attribute__((packed));

 struct ChallengeRequest
{
    unsigned char   _opp_username[USERNAME_LENGHT_16];
    // Setter 
    int setUsername(const char *user);
    // Member methods
    size_t serialize(unsigned char **data);
    size_t HtoN(unsigned char **data);
    size_t NtoH(unsigned char *ser_buf);
    void print();
}__attribute__((packed));

struct ChallengeResponse
{
    unsigned short int _answer;

    // Member methods
    size_t serialize(unsigned char **data);
    size_t HtoN(unsigned char **data);
    size_t NtoH(unsigned char *ser_buf);
    void print();
}__attribute__((packed));

struct GameMove
{
    unsigned short int  _column_num;
    unsigned char       _randomness[RANDOMNESS_LENGTH_16];

    // TODO
    // Setter
    int setRandomness();
    // Member methods
    size_t serialize(unsigned char **data);
    size_t HtoN(unsigned char **data);
    size_t NtoH(unsigned char *ser_buf);
    void print();
}__attribute__((packed));

// TOCHECK we need something to close the game
 struct GameEnd
{
    unsigned short int  _signal;
    unsigned char       _randomness[RANDOMNESS_LENGTH_16];

    // TODO
    // Setter
    int setRandomness();
    // Member methods
    size_t serialize(unsigned char **data);
    size_t HtoN(unsigned char **data);
    size_t NtoH(unsigned char *ser_buf);
    void print();
}__attribute__((packed));
#endif