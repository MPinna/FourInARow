/**
 * DESCRIPTION
 * This file it is used to declare all usefull constants for the include files
 */

#ifndef _UTIL_COSTANTS_H_
#define _UTIL_COSTANTS_H_

#define _16_BYTES           16
#define _32_BYTES           32

#define DEFAULT_ADDR        "localhost"
#define DEFAULT_PORT        8888

// DESCRIPTION packet type
#define CLIENT_HELLO        1
#define SERVER_HELLO        2
#define SERVER_CHALLENGE    3
#define CLIENT_RESPONSE     4
#define LIST_REQUEST        5
#define LIST_RESPONSE       6
#define GAME_REQUEST        7
#define GAME_RESPONSE       8
#define OPP_GAME_RESPONSE   9
#define GAME_CHALL_INFO     10

// DESCRIPTION network constants
#define BACKLOG_QUEUE       5

#endif