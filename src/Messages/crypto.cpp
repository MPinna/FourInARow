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
    memcpy(this->_signature, ser_buf, this->_sig_size);
}
// TODO if to check pass, implement also this one
void
AEAD::serialize(char *to_ser_buf)
{
    short int pos{0};
    uint16_t tag_size{htons(this->_tag_size)};

    memcpy(to_ser_buf, &tag_size, sizeof(tag_size));
    pos += sizeof(tag_size);
    memcpy(to_ser_buf + pos, this->_tag, this->_tag_size);
}

void
AEAD::deserialize(char *ser_buf)
{
    short int pos{0};
    uint16_t tag_size;

    memcpy(&tag_size, ser_buf, sizeof(tag_size));
    this->_tag_size{ntohs(tag_size)};
    pos += sizeof(tag_size);
    this->_tag[tag_size];
    mempcpy(this->_tag, ser_buf, this->_tag_size);
}