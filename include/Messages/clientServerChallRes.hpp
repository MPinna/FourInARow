/**
 * SECTION
 * The clientServerChallRes (Client and Server challenge response) header file, describe packet used to: 
 *  - manage all messages exchanged between a Client and a Server during authentication phase
 */
#ifndef _CLIENTSERVERCHALLRES_H_
#define _CLIENTSERVERCHALLRES_H_
#include <string>

struct clientHello
{
    /* data size = 36 Bytes */
    unsigned int nounce {0};
    std::string username {NULL};
    short unsigned int port_number {0};
    short unsigned int sig_size {0};
    std::string signature {NULL};
};

struct serverHello
{
    /* data size = 20 + cert */
    short unsigned int cert_size;
    std::string certificate {NULL};
    short unsigned int sig_size {0};
    std::string signature {NULL};
};

struct serverChallenge
{
    /* data size = 544 */
    unsigned int server_nounce {0};
    unsigned int client_nounce {0};
    short unsigned int dh_param_p {0};
    short unsigned int dh_param_g {0};
    short unsigned int dh_key_size {0};
    std::string dh_key {NULL};
    short unsigned int sig_size {0};
    std::string signature {NULL};
};

struct clientResponse
{
    /* data size = 534 */
    unsigned int server_nounce {0};
    short unsigned int dh_param_g {0};
    short unsigned int dh_key_size {0}; // 512
    std::string dh_key {NULL};
    short unsigned int sig_size {0};
    std::string signature {NULL};
};
#endif