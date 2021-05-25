/**
 * DESCRIPTION
 * This file it is used to declare all usefull constants for the include files
 */
#ifndef _UTIL_COSTANTS_H_
#define _UTIL_COSTANTS_H_

// DESCRIPTION: size
#define _16_BYTES   16
#define _32_BYTES   32

// DESCRIPTION network constants
#define BACKLOG_QUEUE       5
#define DEFAULT_SERVER_ADDR "localhost"
#define DEFAULT_PEER_ADDR   "localhost"
#define DEFAULT_SERVER_PORT "8888"
#define DEFAULT_RPEER_PORT  "11000" // R stands for receiver
#define DEFAULT_SPEER_PORT  "11001" // S stands for sender

// DESCRIPTION: packet type
typedef enum : uint16_t
{
    PEER_HELLO,
    PEER_RESPONSE,
    PLAYER_LIST,
    CHALLENGE_REQUEST,
    CHALLENGE_RESPONSE,
    GAME_MOVE,
    GAME_END,
    SERVER_HELLO,
    SERVER_RESPONSE,
    DH_PARAMS,
    LIST_RESPONSE,
    GAME_INFO
} Type; // TOCHECK check if this is the complete list of messages

// DESCRIPTION crypto algorithm parameters
#define DIGEST_256_LEN  32
#define DH_SECRET_LEN   32
#define TAG_SIZE_LEN    16
#define RSA_2048_LEN    256
#endif