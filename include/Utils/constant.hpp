/**
 * DESCRIPTION
 * This file it is used to declare all usefull constants for the include files
 */
#ifndef _UTIL_COSTANTS_H_
#define _UTIL_COSTANTS_H_

// DESCRIPTION: size
#define _16_BYTES           16
#define _32_BYTES           32

// DESCRIPTION: server / peer addrinfo
#define DEFAULT_SERVER_ADDR "localhost"
#define DEFAULT_SERVER_PORT "8888"
#define DEFAULT_PEER_ADDR   "localhost"
#define DEFAULT_SPEER_PORT  "11000"     // R stands for receiver
#define DEFAULT_RPEER_PORT  "11001"     // S stands for sender

// DESCRIPTION: packet type
enum PacketType : uint16_t{ 
    CLIENT_HELLO,
    SERVER_HELLO,
    SERVER_CHALLENGE,
    CLIENT_RESPONSE,
    LIST_REQUEST,
    LIST_RESPONSE,
    GAME_REQUEST,
    GAME_RESPONSE,
    OPP_GAME_RESPONSE,
    GAME_CHALL_INFO
}; // TOCHECK check if this is the complete list of messages

// DESCRIPTION network constants
#define BACKLOG_QUEUE       5

#endif