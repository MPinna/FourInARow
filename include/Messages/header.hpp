#ifndef _HEADER_H_
#define _HEADER_H_

#include "../Utils/constant.hpp"

typedef struct __attribute__((packed))
{
    Type                _type;          // To distinguish packet type
    unsigned int        _counter;
    unsigned short int  _payload_size;  // To allocate buffer space
} Header;
#endif