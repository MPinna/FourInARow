#ifndef _HEADER_H_
#define _HEADER_H_

struct Header
{
    unsigned short int  _type; // NOTE defined as constants
    unsigned int        _counter;
    unsigned short int  _payload_size; // To allocate buffer space
} __attribute__((packed));

#endif