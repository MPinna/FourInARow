/**
 * DESCRIPTION
 * This file it is used to declare all usefull constants for the include files
 */
#ifndef _UTIL_COSTANTS_H_
#define _UTIL_COSTANTS_H_

// DESCRIPTION: size
#define USERNAME_LENGHT_16      (16 + 1)
#define RANDOMNESS_LENGTH_16    (16 + 1)

// DESCRIPTION network constants
#define BACKLOG_QUEUE           5
#define DEFAULT_SERVER_ADDR     "localhost"
#define DEFAULT_PEER_ADDR       "localhost"
#define DEFAULT_SERVER_PORT     "8888"
#define DFLT_OPP_LISTENER_PORT  "11000"     // Peer stands for receiver
#define ADDRESS_LENGTH          (16 + 1)

// DESCRIPTION: Auth messages code
#define CLIENT_HELLO        0
#define SERVER_HELLO        1
#define SERVER_CHALLENGE    2
#define CLIENT_RESPONSE     3
#define SERVER_CONFIRMATION 4

// DESCRIPTION: ADAE (Authenticated Data Authenticated Encryption) messages code
#define PEER_HELLO          5
#define PEER_RESPONSE       6
#define PLAYER_LIST         7
#define CHALLENGE_RESPONSE  8
#define GAME_MOVE           9
#define GAME_END            10
#define DH_PARAMS           11
#define LIST_RESPONSE       12
#define GAME_INFO           13

// DESCRIPTION crypto algorithm parameters
#define DIGEST_256_LEN  32
#define DH_SECRET_LEN   32
#define TAG_SIZE_LEN    16
#define RSA_2048_LEN    256
#define RSA_4096_LEN    512
#define IV_12_LEN       12
#define IV_16_LEN       16
#define DEFAULT_CRL     "cert/crl.pem"
#define DEFAULT_CA_CERT "cert/ca_cert.pem"
#define DEFAULT_CERT    "cert/cert.pem"
#define DEFAULT_SER_KEY "key/serverprvkey.pem"

// DESCRIPTION generic
#define CLOSE_SIGNAL    0
#endif