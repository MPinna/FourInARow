/** DESCRIPTION:
 * This file it is used to implement serialization / deserialization class member for all used messages (both for client side and server side)
 */
#include "../../include/Messages/Client/auth.hpp"
#include "../../include/Messages/Server/auth.hpp"
#include "../../include/Messages/Client/enc.hpp"
#include "../../include/Messages/Server/enc.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>


/**
 * SECTION: 
 * Authentication client messages 
 */
void
ClientHello::setUsername(char *user)
{   
    if(strlen(user) > 16)
    {
        std::cout << "setUsername() error: max lenght = 16 char" << std::endl;
        return;
    }
    memcpy(this->_username, user + '\0', USERNAME_LENGHT_16);
}

void
ClientHello::serialize(unsigned char * to_ser)
{
    unsigned short int pos{0};
    uint16_t port_num{htons(this->_port_number)};
    uint32_t nonce{htonl(this->_nonce)};

    memcpy(to_ser + pos, this->_username, USERNAME_LENGHT_16);
    pos += USERNAME_LENGHT_16;
    memcpy(to_ser + pos, &port_num, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(to_ser + pos, &nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    this->digest.serialize(to_ser + pos);
    pos += sizeof(this->digest);
    this->signature.serialize(to_ser + pos);
}

void
ClientHello::deserialize(unsigned char *ser_buf)
{
    unsigned short int pos{0};
    uint16_t port_num{0};
    uint32_t nonce{0};

    this->setUsername((char *)ser_buf);
    pos += USERNAME_LENGHT_16;
    memcpy(&port_num, ser_buf + pos, sizeof(uint16_t));
    this->_port_number = ntohs(port_num);
    pos += sizeof(uint16_t);
    memcpy(&nonce, ser_buf + pos, sizeof(uint32_t));
    this->_nonce = ntohl(nonce);
    pos += sizeof(uint32_t);
    this->digest.deserialize(ser_buf + pos);
    pos += sizeof(this->digest);
    this->signature.deserialize(ser_buf + pos);
}

void
ClientResponse::serialize(unsigned char *to_ser_buf)
{
    unsigned short int pos{0};
    uint32_t opp_nonce{htonl(this->_opp_nonce)};

    memcpy(to_ser_buf, &opp_nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    this->dh_key.serialize(to_ser_buf + pos);
    pos += sizeof(this->dh_key);
    this->digest.serialize(to_ser_buf + pos);
    pos += sizeof(this->digest);
    this->signature.serialize(to_ser_buf + pos);
}

void
ClientResponse::deserialize(unsigned char *ser_buf)
{
    unsigned short int pos{0};
    uint32_t opp_nonce{0};

    memcpy(&opp_nonce, ser_buf, sizeof(uint32_t));
    this->_opp_nonce = ntohl(opp_nonce);
    pos += sizeof(uint32_t);
    this->dh_key.deserialize(ser_buf + pos);
    pos += sizeof(this->dh_key);
    this->digest.deserialize(ser_buf + pos);
    pos += sizeof(this->digest);
    this->signature.deserialize(ser_buf + pos);
}

void
PeerHello::setUsername(unsigned char *name)
{
    memcpy(this->_username, name+'\0', strlen((char *)name));
}

void
PeerHello::serialize(unsigned char *to_ser_buf)
{
    unsigned short int pos{0};
    uint32_t nonce{htonl(this->_nonce)};

    memcpy(to_ser_buf, this->_username, USERNAME_LENGHT_16);
    pos += USERNAME_LENGHT_16;
    memcpy(to_ser_buf + pos, &nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    this->digest.serialize(to_ser_buf + pos);
    pos += sizeof(this->digest);
    this->signature.serialize(to_ser_buf + pos);
}

void
PeerHello::deserialize(unsigned char *ser_buf)
{
    unsigned short int pos{0};
    uint32_t nonce{0};

    this->setUsername(ser_buf);
    pos += USERNAME_LENGHT_16;
    memcpy(&nonce, ser_buf + pos, sizeof(uint32_t));
    this->_nonce = ntohl(nonce);
    pos += sizeof(uint32_t);
    this->digest.deserialize(ser_buf + pos);
    pos += sizeof(this->digest);
    this->signature.deserialize(ser_buf + pos);
}

void
PeerChallenge::serialize(unsigned char *to_ser_buf)
{
    unsigned short int pos{0};
    uint32_t opp_nonce{htonl(this->_opp_nonce)}, nonce{htonl(this->_nonce)};

    memcpy(to_ser_buf + pos, &nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(to_ser_buf + pos, &opp_nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    this->dh_key.serialize(to_ser_buf + pos);
    pos += sizeof(this->dh_key);
    this->digest.serialize(to_ser_buf + pos);
    pos += sizeof(this->digest);
    this->signature.serialize(to_ser_buf + pos);
}

void
PeerChallenge::deserialize(unsigned char *ser_buf)
{
    unsigned short int pos{0};
    uint32_t opp_nonce{0}, nonce{0};

    memcpy(&nonce, ser_buf, sizeof(uint32_t));
    this->_nonce = ntohl(nonce);
    pos += sizeof(uint32_t);
    memcpy(&opp_nonce, ser_buf, sizeof(uint32_t));
    this->_opp_nonce = ntohl(opp_nonce);
    pos += sizeof(uint32_t);
    this->dh_key.deserialize(ser_buf + pos);
    pos += sizeof(this->dh_key);
    this->digest.deserialize(ser_buf + pos);
    pos += sizeof(this->digest);
    this->signature.deserialize(ser_buf + pos);
}

void
PeerResponse::serialize(unsigned char *to_ser_buf)
{
    unsigned short int pos{0};
    uint32_t opp_nonce{htonl(this->_opp_nonce)};
    uint16_t signal{-1};

    memcpy(to_ser_buf + pos, &opp_nonce, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(to_ser_buf + pos, &signal, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    this->dh_key.serialize(to_ser_buf + pos);
    pos += sizeof(this->dh_key);
    this->digest.serialize(to_ser_buf + pos);
    pos += sizeof(this->digest);
    this->signature.serialize(to_ser_buf + pos);
}

void
PeerResponse::deserialize(unsigned char *ser_buf)
{
    unsigned short int pos{0};
    uint32_t opp_nonce{0};
    uint16_t signal{0};

    memcpy(&opp_nonce, ser_buf, sizeof(uint32_t));
    this->_opp_nonce = ntohl(opp_nonce);
    pos += sizeof(uint32_t);
    memcpy(&signal, ser_buf + pos, sizeof(uint16_t));
    this->_signal = ntohs(signal);
    pos += sizeof(uint16_t);
    this->dh_key.deserialize(ser_buf + pos);
    pos += sizeof(this->dh_key);
    this->digest.deserialize(ser_buf + pos);
    pos += sizeof(this->digest);
    this->signature.deserialize(ser_buf + pos);
}

/**
 * SECTION
 * Authentication server messages 
 */