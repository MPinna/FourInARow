/**
 * SECTION
 * cliSerMessages.hpp (Client and Server Messages) header file, describe packet used to:
 *  - Exchange information messages between client and server
 *  - Receive the opponent's information
 *  - Launch a challenge
 */
#ifndef _SERVER_ENCRYPTED_MESSAGES_H_
#define _SERVER_ENCRYPTED_MESSAGES_H_
#include "../../Utils/constant.hpp"
#include "../crypto.hpp"

typedef struct 
{
    unsigned short int  _num_of_clients{0};
    unsigned char *     _clients_list; // TOCHECK
    AEAD                _tag;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}ListResponse, __attribute__((packed));

typedef struct 
{
    unsigned char       _opponent_username[_16_BYTES]{0};
    bool                _flag;
    AEAD                _tag;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}OppGameResponse, __attribute__((packed));

typedef struct 
{
    unsigned char       _opponent_username[_16_BYTES]{0};
    unsigned short int  _peer_port{0};
    unsigned short int  _pubkey_size{0};
    unsigned char *     _opponent_pubkey;
    unsigned short int  _dh_par_p{0};
    unsigned short int  _dh_par_g{0};
    AEAD                _tag;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
}GameChallengeInfo, __attribute__((packed));
#endif