/**
 * DESCRIPTION
 * The aim of this file, is to provide all required structure used to represent complex data
 */
#if !defined(_STRUCTURE_H)
#define _STRUCTURE_H
#include "../Messages/packet.hpp"
#include "constant.hpp"
#include <cstring>

typedef struct __attribute__((packed))
{
    char                _name[USERNAME_LENGHT_16];
    unsigned short int  _status{0};

    void setName(char *name);
} User;

typedef struct __attribute__((packed))  
{
    size_t received;
    size_t next;
    size_t expected;
} SecurityCheck;
#endif // _STRUCTURE_H

