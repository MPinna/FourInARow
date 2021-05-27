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
#include <cstddef>

typedef struct __attribute__((packed))
{   
    unsigned short int  _sig_size{0};
    unsigned char *     _signature{NULL};

    // DESCRIPTION: Setter
    void setSignature(unsigned char *sig);
    // DESCRIPTION: member structure
    void serialize(unsigned char *to_ser_buf);
    void deserialize(unsigned char *ser_buf);
} Signature;

typedef struct __attribute__((packed))
{
    unsigned short int  _dig_size{0};
    unsigned char *     _digest{NULL};

    // DESCRIPTION: Setter
    void setDigest(unsigned char *digest);
    // DESCRIPTION: member structure
    void serialize(unsigned char *to_ser_buf);
    void deserialize(unsigned char *ser_buf);
} Digest;


/**
 * DESCRIPTION: 
 * The tag structure it is used to store the tag used to encrypt data according to the AEAD (Authenticated Encryption with Associated Data)
 */
typedef struct __attribute__((packed))
{   
    unsigned short int  _tag_size{0};
    unsigned char *     _tag{NULL};
    
    // DESCRIPTION: Setter
    void setTag(unsigned char * tag); 
    // DESCRIPTION: member structure
    void serialize(unsigned char * to_ser_buf);
    void deserialize(unsigned char * ser_buf);
} Tag;

typedef struct __attribute__((packed))
{
    unsigned short int  _lenght{0};
    unsigned char *     _cert{NULL};

    // DESCRIPTION: Setter
    void setCert(unsigned char * cert);
    // DESCRIPTION: member structure
    void serialize(unsigned char * to_ser_buf);
    void deserialize(unsigned char * ser_buf);
} Certificate;

typedef struct __attribute__((packed))
{
    unsigned int        _nonce{0}; // NOTE: Value computed as (g)^(a) mod p
    unsigned short int  _dh_lenght{0};
    unsigned char *     _dh_key{NULL};

    // DESCRIPTION: Setter
    void setDHKey(unsigned char *dhkey);
    // DESCRIPTION: member structure
    void serialize(unsigned char *to_ser_buf);
    void deserialize(unsigned char *ser_buf);
} DHKey;
#endif