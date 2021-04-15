/**
 * SECTION
 * The clientServerChallRes (Client and Server challenge response) header file, describe packet used to: 
 *  - manage all messages exchanged between a Client and a Server during authentication phase
 */
#ifndef _CLIENTSERVERCHALLRES_H_
#define _CLIENTSERVERCHALLRES_H_
#include <string>
#include "Utils/constant.hpp"

struct clientHello
{
    /* data size = 40 Bytes */
    unsigned int        nounce {0};
    char                username[_16_BYTES];
    short unsigned int  port_number {0};
    short unsigned int  sig_size {0};
    char                *signature;
};

struct serverHello
{
    /* data size = 20 + cert */
    short unsigned int  cert_size {0};
    unsigned char       *certificate;
    short unsigned int  sig_size {0};
    unsigned char       *signature;
};

struct serverChallenge
{
    /* data size = 544 */
    unsigned int        server_nounce {0};
    unsigned int        client_nounce {0};
    short unsigned int  dh_param_p {0};
    short unsigned int  dh_param_g {0};
    short unsigned int  dh_key_size {0};
    unsigned char       *dh_key; 
    short unsigned int  sig_size {0};
    unsigned char       *signature; 
};

struct clientResponse
{
    /* data size = 534 */
    unsigned int        server_nounce {0};
    short unsigned int  dh_param_g {0};
    short unsigned int  dh_key_size {0}; 
    unsigned char       *dh_key;
    short unsigned int  sig_size {0};
    unsigned char       *signature;
};
#endif