/**
 * DESCRIPTION
 * The client challenge response header file, describe packet used to authentication phase sent by client only
 */
#ifndef _CLIENT_AUTH_MESSAGES_H_
#define _CLIENT_AUTH_MESSAGES_H_

#include "../../Utils/constant.hpp"
#include "../crypto.hpp"
#include <cstddef>

// TODO ClientHello -> Just a message to present itself to the server

/**
 * DESCRIPTION: the ClientChallenge structure it is used for:
 *  - Exchange hello message for client - server communication
 *  - Exchange hello message for peer-to-peer communication
 */
struct ClientChallenge
{
    unsigned char       _username[USERNAME_LENGHT_16];
    unsigned short int  _port_number;
    unsigned int        _nonce;

    // DESCRIPTION: Setter / Getter
    int setUsername(const char *user);
    int getType();
    int getSize();
    // DESCRIPTION: Member methods
    size_t serialize(unsigned char **data);
    size_t HtoN(unsigned char **data);
    size_t NtoH(unsigned char *ser_buf);
    void print();
}__attribute__((packed));

/**
 * DESCRIPTION: the ClientChallenge structure it is used for:
 *  - Response message for client - server communication
 *  - Response message for peer-to-peer communication
 */
struct ClientResponse
{
    unsigned int    _opp_nonce;
    DHKey           dh_key;

    // DESCRIPTION: Member methods
    int getType();
    int getSize();
    // DESCRIPTION: Member methods
    size_t serialize(unsigned char **data);
    size_t HtoN(unsigned char **data);
    size_t NtoH(unsigned char *ser_buf);
    void print();
}__attribute__((packed));

/**
 * DESCRIPTION: the ClientChallenge structure it is used for:
 *  - Challenge - Response phase between two peers
 */
struct ChallengeRequest
{
    unsigned int    _nonce;
    unsigned int    _opp_nonce;
    DHKey           dh_key;

    // DESCRIPTION: Member methods
    int getType();
    int getSize(); // TOCHECK
    // DESCRIPTION: Member methods
    size_t serialize(unsigned char **data);
    size_t HtoN(unsigned char **data);
    size_t NtoH(unsigned char *ser_buf);
    void print();
}__attribute__((packed));

struct ChallengeResponse
{
    unsigned int    _opp_nonce;
    short int       _signal; // used to accept or decline challenge between two player
    DHKey           dh_key;

    // DESCRIPTION: Member methods
    int getType();
    int getSize(); // TOCHECK
    // DESCRIPTION: Member methods
    size_t serialize(unsigned char **data);
    size_t HtoN(unsigned char **data);
    size_t NtoH(unsigned char *ser_buf);
    void print();
}__attribute__((packed)); 
#endif