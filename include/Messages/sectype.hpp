/**
 * DESCRIPTION 
 * The sectype header it is used to manage the case in which the packet it is signed only or AEAD (authenticated and encypted)
 */
#ifndef _ENCRYPTION_FORMS_H_
#define _ENCRYPTION_FORMS_H_

struct Signature
{
    unsigned short int  _sig_size;
    unsigned char *     _signature;

    // DESCRIPTION: member structure
    void serialize(char * to_ser_buf);
    void deserialize(char * ser_buf);
}__attribute__((packed));

struct AEAD
{
    unsigned short int  _tag_size;
    unsigned char *     _tag;

    // DESCRIPTION: member structure
    void serialize(char * to_ser_buf);
    void deserialize(char * ser_buf);
}__attribute__((packed));
#endif