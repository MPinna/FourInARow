/**
 * DESCRIPTION
 * The client challenge response header file, describe packet used to authentication phase sent by client only
 */
#ifndef _CLIENT_AUTH_MESSAGES_H_
#define _CLIENT_AUTH_MESSAGES_H_

#include "../../Utils/constant.hpp"
#include "../crypto.hpp"
#include <cstddef>

typedef struct __attribute__((packed))
{
    unsigned char       _username[_16_BYTES + 1]{NULL};
    unsigned short int  _port_number{0};
    Auth                _auth_data;

    // DESCRIPTION: Setter
    void setUsername(unsigned char *name);
    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
} PeerHello;

typedef struct __attribute__((packed))
{
    unsigned int    _dh_param_g{0};
    unsigned int    _dh_param_p{0};
    Auth            _auth_data;
    DHKey           _dh_key;

    // DESCRIPTION: Member methods
    void serialize(char *to_ser_buf);
    void deserilize(char *ser_buf);
} PeerResponse;
#endif