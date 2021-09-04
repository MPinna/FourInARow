/**
 * DESCRIPTION 
 * The crypto header it is used to manage the possible action applied to messages:
 *  - signature (used during authentication phase)
 *  - AEAD (Authenticated Encryption Authenticated data, used during communication phase)
 * The structure is common both for client messages and server messages
 */
#ifndef _ENCRYPTION_FORMS_H_
#define _ENCRYPTION_FORMS_H_
#include "../Utils/constant.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <cstddef>
#include <cstring>
#include <cassert>
#include <openssl/dh.h>

/**
 * Tag structure represent a generic container which can be used to store:
 *  1) Signature and signature length
 *  2) Tag and tag length of AADD data
 *  3) Hash and hash length
 */
struct Tag 
{   
    unsigned short int  _taglen;
    unsigned char *     _tag;

    // Setter
    int setTag(unsigned char *tag, unsigned short int size);
    int getSize();
    // Member structure
    size_t HtoN(unsigned char *data);
    size_t NtoH(unsigned char *serialized_buf);
    size_t NtoHtaglen(unsigned char *data);
    int print();
}__attribute__((packed));

struct DHKey
{
    unsigned short int  _dh_lenght;
    unsigned char *     _dh_key;

    int setKey(EVP_PKEY *key);
    size_t getSize();
    // Member methods
    size_t HtoN(unsigned char *data);
    size_t NtoH(unsigned char *ser_data);
    void print();
}__attribute__((packed));

struct DHParams
{
    uint16_t        _params_length;
    unsigned char * _params;

    int setParams(DH *params);
    size_t getSize();
    // Member methods
    size_t HtoN(unsigned char *data);
    size_t NtoH(unsigned char *ser_data);
    void print();
}__attribute__((packed));

struct Digest
{
    unsigned short int  _dig_size;
    unsigned char *     _digest;

    // Setter
    void setDigest(unsigned char *digest);
    // Member structure
    void serialize(unsigned char *to_ser_buf);
    void deserialize(unsigned char *ser_buf);
} __attribute__((packed));
#endif