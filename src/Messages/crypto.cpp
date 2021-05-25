#include "../../include/Messages/crypto.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <cstring>

void
Auth::setDigest(unsigned char *digest)
{
    this->_digest = (unsigned char *)malloc(strlen((char *)digest));
    memcpy(this->_digest, digest, strlen((char *)digest));
}

void
Auth::setSignature(unsigned char *signature)
{
    this->_signature = (unsigned char *)malloc(strlen((char *)signature));
    memcpy(this->_signature, signature, strlen((char *)signature));
}

void
Auth::serialize(unsigned char * to_ser_buf)
{
    size_t pos{0};
    uint32_t nonce{htonl(this->_nonce)}, opp_nonce{htonl(this->_opp_nonce)};
    uint16_t dig_size{htons(this->_dig_size)}, sig_size{htons(this->_sig_size)};

    memcpy(to_ser_buf, &nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(to_ser_buf + pos, &opp_nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(to_ser_buf + pos, &dig_size, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(to_ser_buf + pos, this->_digest, this->_dig_size);
    pos += this->_dig_size;
    memcpy(to_ser_buf + pos, &sig_size, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(to_ser_buf + pos, this->_signature, this->_sig_size);
}

void
Auth::deserialize(unsigned char *ser_buf)
{
    size_t pos{0};
    uint32_t nonce{0}, opp_nonce{0};
    uint16_t dig_size{0}, sig_size{0};
    
    memcpy(&nonce, ser_buf, sizeof(uint32_t));
    this->_nonce = ntohl(nonce);
    pos += sizeof(uint32_t);
    memcpy(&opp_nonce, ser_buf + pos, sizeof(uint32_t));
    this->_opp_nonce = ntohl(opp_nonce);
    pos += sizeof(uint32_t);
    memcpy(&dig_size, ser_buf + pos, sizeof(uint16_t));
    this->_dig_size = ntohs(dig_size);
    pos += sizeof(uint16_t);
    this->setDigest(ser_buf + pos);
    pos += this->_dig_size;
    memcpy(&sig_size, ser_buf + pos, sizeof(uint16_t));
    this->_sig_size = ntohs(sig_size);
    pos += sizeof(uint16_t);
    this->setSignature(ser_buf + pos);
}

void
AEAD::setTag(unsigned char *tag)
{
    this->_tag = (unsigned char *)malloc(this->_tag_size);
    memcpy(this->_tag, tag, this->_tag_size);
}

void
AEAD::serialize(unsigned char *to_ser)
{
    short int pos{0};
    uint16_t tag_size{htons(this->_tag_size)};

    memcpy(to_ser, &tag_size, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(to_ser + pos, this->_tag, this->_tag_size);
}

void
AEAD::deserialize(unsigned char *ser_buf)
{
    short int pos{0};
    uint16_t tag_size;

    memcpy(&tag_size, ser_buf, sizeof(uint16_t));
    this->_tag_size = ntohs(tag_size);
    pos += sizeof(uint16_t);
    this->setTag(ser_buf + pos);
}

void
Certificate::setCert(unsigned char * cert)
{
    this->_cert = (unsigned char *)malloc(this->_lenght);
    memcpy(this->_cert, cert, this->_lenght);
}

void
Certificate::serialize(unsigned char * to_ser)
{
    short int pos{0};
    uint16_t lenght{htons(this->_lenght)};

    memcpy(to_ser, &lenght, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(to_ser + pos, this->_cert, this->_lenght);
}

void
Certificate::deserialize(unsigned char *ser_buf)
{
    short int pos{0};
    uint16_t lenght{0};

    memcpy(&lenght, ser_buf, sizeof(uint16_t));
    this->_lenght = ntohs(lenght);
    pos += sizeof(uint16_t);
    this->setCert(ser_buf + pos);
}

void
DHKey::setDHKey(unsigned char *dhkey)
{
    this->_dh_key = (unsigned char *)malloc(this->_dh_lenght);
    memcpy(this->_dh_key, dhkey, this->_dh_lenght);
}

void
DHKey::serialize(unsigned char *to_ser)
{
    unsigned short int pos{0};
    uint16_t dh_lenght{htons(this->_dh_lenght)};
    uint32_t nonce{htonl(this->_nonce)};

    memcpy(to_ser, &nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(to_ser + pos, &dh_lenght, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(to_ser + pos, this->_dh_key, this->_dh_lenght);
}

void
DHKey::deserialize(unsigned char *ser_buf)
{
    unsigned short int pos{0};
    uint16_t dh_lenght{0};
    uint32_t nonce{0};

    memcpy(&nonce, ser_buf, sizeof(uint32_t));
    this->_nonce = ntohl(nonce);
    pos += sizeof(uint32_t);
    memcpy(&dh_lenght, ser_buf + pos, sizeof(uint16_t));
    this->_dh_lenght = ntohs(dh_lenght);
    pos += sizeof(uint16_t);
    this->setDHKey(ser_buf + pos);
}