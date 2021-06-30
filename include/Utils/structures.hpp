/**
 * DESCRIPTION
 * The aim of this file, is to provide all required structure used to represent complex data
 */
#if !defined(_STRUCTURE_H)
#define _STRUCTURE_H
#include "../Messages/packet.hpp"
#include "constant.hpp"
#include <cstring>

typedef struct __attribute__((packed)) User
{
    char                _name[USERNAME_LENGHT_16];
    unsigned short int  _status;
    // Default Constructor
    User() 
        : _name{NULL}, _status{0} {}
    // Class methods
    void setName(char *name);
};


typedef struct __attribute__((packed)) SecurityCheck
{
    size_t _received;
    size_t _next;
    size_t _expected;
    // Default constructor
    SecurityCheck()
        : _received{0}, _next{0}, _expected{0} {}
    // Class methods
    int update();
};
#endif // _STRUCTURE_H

