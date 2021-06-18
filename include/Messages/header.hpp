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

    // DESCRIPTION: default constructor
    Header();
    ~Header();
} __attribute__((packed));
#endif