#include "../../include/Messages/crypto.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
// TOCHECK
void
Signature::serialize(char *to_ser_buf)
{
    short int pos{0};
    uint16_t sig_size{htons(this->_sig_size)};

    memcpy(to_ser_buf, &sig_size, sizeof(sig_size));
    pos += sizeof(sig_size);
    memcpy(to_ser_buf + pos, this->_signature, sig_size);
}

void
Signature::deserialize(char *ser_buf)
{
    short int pos{0};
    uint16_t sig_size;

    memcpy(&sig_size, ser_buf, sizeof(sig_size));
    this->_sig_size{ntohs(sig_size)};
    pos += sizeof(sig_size);
    this->_signature[sig_size];
    memcpy(this->_signature, ser_buf, sig_size);
}
// TODO if to check pass, implement also this one
void
AEAD::serialize(char *to_ser_buf)
{

}

void
AEAD::deserialize(char *ser_buf)
{
    
}