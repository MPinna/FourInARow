/**
 * DESCRIPTION
 * This file it is used to declare all usefull constants for the include files
 */
#ifndef _UTIL_COSTANTS_H_
#define _UTIL_COSTANTS_H_

// DESCRIPTION: size
#define USERNAME_LENGHT_16      16 + 1
#define RANDOMNESS_LENGTH_16    16

// DESCRIPTION network constants
#define BACKLOG_QUEUE 5
#define DEFAULT_SERVER_ADDR "localhost"
#define DEFAULT_PEER_ADDR   "localhost"
#define DEFAULT_SERVER_PORT "8888"
#define DEFAULT_RPEER_PORT  "11000" // R stands for receiver
#define DEFAULT_SPEER_PORT  "11001" // S stands for sender
#define ADDRESS_LENGTH      16 + 1

#define PEER_HELLO          0
#define PEER_RESPONSE       1
#define PLAYER_LIST         2
#define CHALLENGE_REQUEST   3
#define CHALLENGE_RESPONSE  4
#define GAME_MOVE           5 
#define GAME_END            6
#define SERVER_HELLO        7
#define SERVER_RESPONSE     8
#define DH_PARAMS           9
#define LIST_RESPONSE       10
#define GAME_INFO           11

// DESCRIPTION crypto algorithm parameters
#define DIGEST_256_LEN 32
#define DH_SECRET_LEN 32
#define TAG_SIZE_LEN 16
#define RSA_2048_LEN 256
#define IV_DEFAULT_LEN 16
#endif