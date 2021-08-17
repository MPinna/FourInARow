#include "../../include/Messages/crypto.hpp"
#include <openssl/evp.h>

int 
Tag::setTag(unsigned char *tag, unsigned short int size)
{
    if(!tag)
    {
        std::cerr << "Tag::setTag(): EMPTY Tag!";
        return -1;
    }
    if(size > SIZE_MAX / sizeof(unsigned short int))
    {
        std::cerr << "Tag::setTag(): size overcame SIZE_MAX!";
        return -1;
    }
    this->_tag = new unsigned char[size+1];
    memcpy(this->_tag, tag + '\0', size+1);
    this->_taglen = size;

    return 1;
}

int
Tag::getSize()
{
    return (
        sizeof(this->_taglen) + 
        this->_taglen
    );
}

size_t
Tag::HtoN(unsigned char *data)
{
    size_t pos{0};
    uint16_t tag_size{htons(this->_taglen)};
    
    memcpy(data, &tag_size, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(data + pos, this->_tag, this->_taglen);
    pos += this->_taglen;

    return pos;
}

size_t
Tag::NtoH(unsigned char *ser_buf)
{
    size_t pos{0};
    uint16_t tag_size{0};

    memcpy(&tag_size, ser_buf + pos, sizeof(uint16_t));
    this->_taglen = ntohs(tag_size);
    pos += sizeof(uint16_t);
    this->setTag(ser_buf + pos, this->_taglen);
    pos += this->_taglen;
    
    return pos;
}

size_t
Tag::NtoHtaglen(unsigned char *ser_buf)
{
    size_t pos{0};
    uint16_t tag_size{0};

    memcpy(&tag_size, ser_buf + pos, sizeof(uint16_t));
    this->_taglen = ntohs(tag_size);
    pos += sizeof(uint16_t);
    
    return pos;
}

int 
Tag::print()
{
    assert(this->_taglen != 0);
    if(!this->_tag)
    {
        std::cerr << "Tag::print() tag length = 0, tag not initialized yet";
        return -1;
    }
    assert(this->_tag);
    if(!this->_tag)
    {
        std::cerr << "Tag::print() empty tag, cannot display output";
        return -1;
    }
    std::cout << "Tag: " << std::endl;
    BIO_dump_fp(stdout, (const char *)this->_tag, this->_taglen);
    std::cout << "\nTag size: " << this->_taglen << std::endl;
    return 1;
}

int
DHKey::getSize()
{
    return (
        sizeof(this->_nonce) +
        sizeof(this->_dh_lenght) +
        this->_dh_lenght
    );
}

size_t
DHKey::setDHKey(unsigned char *data)
{
    size_t size{strlen((char *)data)};
    this->_dh_key = new unsigned char [(size + 1)];
    memcpy(this->_dh_key, data + '\0', size + 1);
    
    return size;
}

size_t 
DHKey::serialize(unsigned char *data)
{
    size_t pos{0};

    memcpy(data, &this->_nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(data + pos, &_dh_lenght, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(data + pos, this->_dh_key, this->_dh_lenght);
    pos += this->_dh_lenght;

    return pos;
}

size_t 
DHKey::HtoN(unsigned char *data)
{
    size_t pos{0};
    uint32_t nonce{htonl(this->_nonce)};
    uint16_t dh_lenght{htons(this->_dh_lenght)};

    memcpy(data, &nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(data + pos, &dh_lenght, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(data + pos, this->_dh_key, this->_dh_lenght);
    pos += this->_dh_lenght;

    return pos;
}

size_t 
DHKey::NtoH(unsigned char *ser_data)
{
    size_t pos{0};
    uint32_t nonce{0};
    uint16_t dh_lenght{0};

    memcpy(&nonce, ser_data, sizeof(uint32_t));
    this->_nonce = ntohl(nonce);
    pos += sizeof(uint32_t);
    memcpy(&dh_lenght, ser_data + pos, sizeof(uint16_t));
    this->_dh_lenght = ntohs(dh_lenght);
    pos += sizeof(uint16_t);
    this->setDHKey(ser_data + pos);
    pos += this->_dh_lenght;

    return pos;
}

void
DHKey::print()
{
    // TODO
}

void Digest::setDigest(unsigned char *digest)
{
    this->_digest = new unsigned char[(strlen((char *)digest) + 1)];
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