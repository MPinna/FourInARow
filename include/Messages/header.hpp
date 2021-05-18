#ifndef _HEADER_H_
#define _HEADER_H_

#include "../Utils/constant.hpp"

struct Header
{
    PacketType          _type; // To distinguish packet type
    unsigned int        _counter;
    unsigned short int  _payload_size; // To allocate buffer space
} __attribute__((packed));
#endif