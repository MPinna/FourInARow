/**
 * DESCRIPTION
 * The aim of this file, is to provide all required structure used to represent complex data
 */
#if !defined(_STRUCTURE_H)
#define _STRUCTURE_H
#include "../Messages/packet.hpp"
#include "constant.hpp"
#include <cstring>

struct User
{
    char                _name[USERNAME_LENGHT_16];
    unsigned short int  _status;

    User()
        : _name{NULL}, _status{0} {}

    void setName(char *name);
}__attribute__((packed));

struct SecurityCheck
{
    size_t _received;
    size_t _next;
    size_t _expected;

    SecurityCheck()
        : _received{0}, _next{0}, _expected{0} {}

    int update();
} __attribute__((packed));
#endif // _STRUCTURE_H

