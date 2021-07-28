/**
 * DESCRIPTION
 * The server challenge response header file, describe packet used to authentication phase sent by server only
 */
#ifndef _SERVER_AUTH_MESSAGES_H_
#define _SERVER_AUTH_MESSAGES_H_
#include "../../Utils/constant.hpp"
#include "../crypto.hpp"

struct ServerCertificate 
{
    unsigned short int  _lenght;
    unsigned char *     _certificate;

    // Setter / Getter
    size_t setCert(unsigned char *cert);
    size_t getSize();
    // Member structure
    size_t serialize(unsigned char **data);
    size_t HtoN(unsigned char **data);
    size_t NtoH(unsigned char *ser_buf);
}__attribute__((packed));

struct ServerResponse
{
    unsigned int    _opp_nonce;
    unsigned int    _dh_param_p;
    unsigned int    _dh_param_g;
    DHKey           dh_key;

    // Getter
    int getType();
    size_t getSize(); // TOCHECK
    // Member methods
    size_t serialize(unsigned char **data);
    size_t HtoN(unsigned char **data);
    size_t NtoH(unsigned char *ser_buf);
    void print();
}__attribute__((packed));

// TODO: message sent by the server to confirm client authentication, but also to setup client info (status, pubkey, etc...). Check fields correctness and member methods
struct Confirmation
{
    bool authenticated;
    
    // Getter
    int getType();
    size_t getSize(); 
    // Member methods
    size_t serialize(unsigned char **data);
    size_t HtoN(unsigned char **data);
    size_t NtoH(unsigned char *ser_buf);
    void print();
}__attribute__((packed));
#endif