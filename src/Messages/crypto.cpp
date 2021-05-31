#include "../../include/Messages/crypto.hpp"

void Signature::setSignature(unsigned char *signature)
{
    this->_signature = (unsigned char *)malloc(strlen((char *)signature) + 1);
    memcpy(this->_signature, signature + '\0', strlen((char *)signature) + 1);
    this->_sig_size = strlen((char *)signature) + 1;
}

void Signature::serialize(unsigned char *to_ser_buf)
{
    size_t pos{0};
    uint16_t sig_size{htons(this->_sig_size)};

    memcpy(to_ser_buf + pos, &sig_size, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(to_ser_buf + pos, this->_signature, this->_sig_size);
}

void Signature::deserialize(unsigned char *ser_buf)
{
    size_t pos{0};
    uint16_t sig_size{0};

    memcpy(&sig_size, ser_buf + pos, sizeof(uint16_t));
    this->_sig_size = ntohs(sig_size);
    pos += sizeof(uint16_t);
    this->setSignature(ser_buf + pos);
}

void Digest::setDigest(unsigned char *digest)
{
    this->_digest = (unsigned char *)malloc(strlen((char *)digest) + 1);
    memcpy(this->_digest, digest + '\0', strlen((char *)digest) + 1);
    this->_dig_size = strlen((char *)digest) + 1;
}

void Digest::serialize(unsigned char *to_ser_buf)
{
    size_t pos{0};
    uint16_t dig_size{htons(this->_dig_size)};

    memcpy(to_ser_buf + pos, &dig_size, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(to_ser_buf + pos, this->_digest, this->_dig_size);
    pos += this->_dig_size;
}

void Digest::deserialize(unsigned char *ser_buf)
{
    size_t pos{0};
    uint16_t dig_size{0};

    memcpy(&dig_size, ser_buf + pos, sizeof(uint16_t));
    this->_dig_size = ntohs(dig_size);
    pos += sizeof(uint16_t);
    this->setDigest(ser_buf + pos);
    pos += this->_dig_size;
}

void
Tag::setTag(unsigned char *tag)
{
    this->_tag = (unsigned char *)malloc(strlen((char *)tag + 1));
    memcpy(this->_tag, tag + '\0', strlen((char *)tag+1));
    this->_tag_size = strlen((char *)tag) + 1;
}

void
Tag::serialize(unsigned char *to_ser)
{
    short int pos{0};
    uint16_t tag_size{htons(this->_tag_size)};

    memcpy(to_ser, &tag_size, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(to_ser + pos, this->_tag, this->_tag_size);
}

void
Tag::deserialize(unsigned char *ser_buf)
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
{   // TOCHECK check if in this case you can assign length this way
    this->_cert = (unsigned char *)malloc(strlen((char *)cert+1));
    memcpy(this->_cert, cert, strlen((char *)cert+1));
    this->_lenght = strlen((char *)cert) + 1;
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
    this->_dh_key = (unsigned char *)malloc(strlen((char *)dhkey) + 1);
    memcpy(this->_dh_key, dhkey + '\0', strlen((char *)dhkey) + 1);
    this->_dh_lenght = strlen((char *)dhkey) + 1;
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