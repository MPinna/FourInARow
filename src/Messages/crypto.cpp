#include "../../include/Messages/crypto.hpp"
#include <openssl/evp.h>
#include <openssl/pem.h>

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
    this->_tag = new unsigned char[size];
    memcpy(this->_tag, tag, size);
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

size_t
DHKey::getSize()
{
    return (
        sizeof(this->_dh_lenght) +
        this->_dh_lenght
    );
}

int
DHKey::setKey(EVP_PKEY *key)
{
    int size{EVP_PKEY_size(key)};
    this->_dh_key = new unsigned char[size];
    memcpy(this->_dh_key, key, size);
    this->_dh_lenght = size;
    
    return 1;
}

size_t 
DHKey::HtoN(unsigned char *data)
{
    size_t pos{0};
    uint16_t dh_lenght{htons(this->_dh_lenght)};

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
    uint16_t dh_lenght{0};

    memcpy(&dh_lenght, ser_data + pos, sizeof(uint16_t));
    this->_dh_lenght = ntohs(dh_lenght);
    pos += sizeof(uint16_t);
    this->_dh_key = new unsigned char[dh_lenght];
    memcpy(this->_dh_key, ser_data + pos, dh_lenght);
    pos += dh_lenght;

    return pos;
}

void
DHKey::print()
{
    std::cout << 
        "DH Key length: " << this->_dh_lenght << 
        "\nDH Key: " << this->_dh_key << 
    std::endl;
}

int
DHParams::setParams(DH *params)
{
    BIO *bio = BIO_new(BIO_s_mem());
    if(bio == NULL)
    {    
        std::cerr << "<== DHParams::setParams(BIO_new) failed!" << std::endl;
        return 0;
    }
    if(PEM_write_bio_DHparams(bio, params) < 1)
    {
        std::cerr << "<== DHParams::setParams(PEM_write_bio_DHparams) failed!";
        return 0;
    }
    const int dh_size = BIO_pending(bio);
    this->_params = new unsigned char[dh_size];
    BIO_read(bio, this->_params, dh_size);
    this->_params_length = dh_size;
    BIO_free(bio);

    return 1;
}

size_t
DHParams::getSize()
{
    return (
        sizeof(this->_params_length) +
        this->_params_length
    );
}

size_t
DHParams::HtoN(unsigned char *data)
{
    size_t pos{0};
    uint16_t params_length{htons(this->_params_length)};

    memcpy(data + pos, &params_length, sizeof(params_length));
    pos += sizeof(params_length);
    memcpy(data + pos, this->_params, this->_params_length);
    pos += this->_params_length;

    return pos;
}

size_t
DHParams::NtoH(unsigned char *ser_data)
{
    size_t pos{0};
    uint16_t params_length{0};

    memcpy(&params_length, ser_data + pos, sizeof(params_length));
    this->_params_length = ntohs(params_length);
    pos += sizeof(params_length);
    this->_params = new unsigned char[params_length];
    memcpy(this->_params, ser_data + pos, this->_params_length);
    pos += _params_length;

    return pos;
}


void
DHParams::print()
{
    std::cout <<
        "Params length: " << this->_params_length << 
        "\nDH Params: " << this->_params <<
    std::endl;
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