/**
 * DESCRIPTION 
 * The crypto header it is used to manage the possible action applied to messages:
 *  - signature (used during authentication phase)
 *  - AEAD (Authenticated Encryption Authenticated data, used during communication phase)
 * The structure is common both for client messages and server messages
 */
#ifndef _ENCRYPTION_FORMS_H_
#define _ENCRYPTION_FORMS_H_
typedef struct 
{
    unsigned short int  _sig_size;
    unsigned char *     _signature;
    
    // DESCRIPTION: member structure
    void serialize(char * to_ser_buf);
    void deserialize(char * ser_buf);
}Signature, __attribute__((packed));

typedef struct 
{
    unsigned short int  _tag_size;
    unsigned char *     _tag;

    // DESCRIPTION: member structure
    void serialize(char * to_ser_buf);
    void deserialize(char * ser_buf);
}AEAD, __attribute__((packed));
#endif