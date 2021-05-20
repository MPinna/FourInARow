/**
 * DESCRIPTION 
 * The crypto header it is used to manage the possible action applied to messages:
 *  - signature (used during authentication phase)
 *  - AEAD (Authenticated Encryption Authenticated data, used during communication phase)
 * The structure is common both for client messages and server messages
 */
#ifndef _ENCRYPTION_FORMS_H_
#define _ENCRYPTION_FORMS_H_

#include <cstddef>

typedef struct __attribute__((packed))
{
    uint32_t        _nonce{0};
    uint32_t        _opp_nounce{0};
    unsigned char   _digest[DIGEST_256_LEN+1]{NULL};
    unsigned char   _signature[RSA_2048_LEN+1]{NULL};
    
    // DESCRIPTION: member structure
    void serialize(char * to_ser_buf);
    void deserialize(char * ser_buf);
} Auth;

typedef struct __attribute__((packed))
{
    unsigned char _digest[DIGEST_256_LEN+1]{NULL};
    unsigned char _tag[TAG_SIZE_LEN+1]{NULL};
    
    // DESCRIPTION: member structure
    void serialize(char * to_ser_buf);
    void deserialize(char * ser_buf);
} AEAD; // Auth Encryption Auth Data


typedef struct __attribute__((packed))
{
    uint32_t _len{0};
    unsigned char cert[];
} Certificate;

// typedef struct __attribute__((packed))
// {
//      uint32_t        nonce;
// 	    uint32_t        len;
// 	    unsigned char   key[];
// } DHKey;
#endif