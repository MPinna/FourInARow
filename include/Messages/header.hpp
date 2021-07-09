#ifndef _HEADER_H_
#define _HEADER_H_
#include "../Utils/constant.hpp"
#include <netinet/in.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>

struct Header
{   // 8 Bytes
    unsigned short int  _type;          // To distinguish packet type
    unsigned int        _counter;       // To keep track of send/receive
    unsigned short int  _payload_size;  // To allocate buffer space

    // DESCRIPTION class member
    size_t  HtoN(unsigned char *buf);
    size_t  serialize(unsigned char *buf);
    size_t  NtoH(unsigned char *buf);
} __attribute__((packed));
#endif