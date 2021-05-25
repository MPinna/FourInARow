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
    this->_digest = (unsigned char *)malloc(this->_dig_size);
    memcpy(this->_digest, ser_buf + pos, this->_dig_size);
    pos += this->_dig_size;
    memcpy(&sig_size, ser_buf + pos, sizeof(uint16_t));
    this->_sig_size = ntohs(sig_size);
    pos += sizeof(uint16_t);
    this->_signature = (unsigned char *)malloc(this->_sig_size);
    memcpy(this->_signature, ser_buf + pos, this->_sig_size);
    pos += this->_sig_size;
}