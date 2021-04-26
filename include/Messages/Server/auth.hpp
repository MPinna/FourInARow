/**
 * SECTION
 * The clientServerChallRes (Client and Server challenge response) header file, describe packet used to: 
 *  - manage all messages exchanged between a Client and a Server during authentication phase
 */
#ifndef _SERVER_AUTH_MESSAGES_H_
#define _SERVER_AUTH_MESSAGES_H_
// TOCHECK review all struct -> remove redundant 

struct serverHello
{
    /* data size = 20 + cert */
    unsigned short int  cert_size {0};
    unsigned char       *certificate;
    unsigned short int  sig_size {0};
    unsigned char       *signature;

    // SECTION: Member methods
    void serialize(serverHello *packet, char *ser_buf);
    void deserilize(char *ser_buf, serverHello *packet);
}__attribute__((packed));

struct serverChallenge
{
    /* data size = 544 */
    unsigned int        server_nounce {0};
    unsigned int        client_nounce {0};
    unsigned short int  dh_param_p {0};
    unsigned short int  dh_param_g {0};
    unsigned short int  dh_key_size {0};
    unsigned char       *dh_key; 
    unsigned short int  sig_size {0};
    unsigned char       *signature;

    // SECTION: Member methods
    void serialize(serverChallenge *packet, char *ser_buf);
    void deserilize(char *ser_buf, serverChallenge *packet);
}__attribute__((packed));
#endif