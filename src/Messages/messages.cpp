/** DESCRIPTION:
 * This file it is used to implement serialization / deserialization class member for all used messages (both for client side and server side)
 */
#include "../../include/Messages/Client/auth.hpp"
#include "../../include/Messages/Server/auth.hpp"
#include "../../include/Messages/Client/enc.hpp"
#include "../../include/Messages/Server/enc.hpp"
#include "../../include/Utils/strinops.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>


/**
 * SECTION: 
 * Client authentication messages 
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
 * Server authentication messages 
 */
void
ServerHello::serialize(unsigned char *to_ser_buf)
{
    this->_certificate.serialize(to_ser_buf);
}

void
ServerHello::deserialize(unsigned char *ser_buf)
{
    this->_certificate.deserialize(ser_buf);
}

void
ServerChallenge::serialize(unsigned char *to_ser_buf)
{
    unsigned short int pos{0};
    uint32_t dh_param_p{htonl(this->_dh_param_p)}, dh_param_g{htonl(this->_dh_param_g)};

    memcpy(to_ser_buf + pos, &dh_param_p, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(to_ser_buf + pos, &dh_param_g, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    this->dh_key.serialize(to_ser_buf + pos);
    pos += sizeof(this->dh_key);
    this->digest.serialize(to_ser_buf + pos);
    pos += sizeof(this->digest);
    this->signature.serialize(to_ser_buf + pos);
}

void
ServerChallenge::deserialize(unsigned char *ser_buf)
{
    unsigned short int pos{0};
    uint32_t dh_param_g{0}, dh_param_p{0};

    memcpy(&dh_param_p, ser_buf, sizeof(uint32_t));
    this->_dh_param_p = ntohl(dh_param_p);
    pos += sizeof(uint32_t);
    memcpy(&dh_param_g, ser_buf + pos, sizeof(uint32_t));
    this->_dh_param_g = ntohs(dh_param_g);
    pos += sizeof(uint32_t);
    this->dh_key.deserialize(ser_buf + pos);
    pos += sizeof(this->dh_key);
    this->digest.deserialize(ser_buf + pos);
    pos += sizeof(this->digest);
    this->signature.deserialize(ser_buf + pos);
}

/**
 * SECTION
 * Client encrypted messages
 */
void
PlayerList::serialize(unsigned char *to_ser_buf)
{
    unsigned short int pos{0};

    memcpy(to_ser_buf, this->_randomness, RANDOMNESS_LENGTH_16);
    pos += RANDOMNESS_LENGTH_16;
    this->tag.serialize(to_ser_buf + pos);
}

void
PlayerList::deserialize(unsigned char *ser_buf)
{
    unsigned short int pos{0};

    memcpy(this->_randomness, ser_buf, RANDOMNESS_LENGTH_16);
    pos += RANDOMNESS_LENGTH_16;
    this->tag.deserialize(ser_buf + pos);
}

void
ChallengeRequest::serialize(unsigned char *to_ser_buf)
{
    unsigned short int pos{0};

    writeInto(to_ser_buf, this->_opp_username);
    pos += USERNAME_LENGHT_16;
    this->tag.serialize(to_ser_buf + pos);
}

void
ChallengeRequest::deserialize(unsigned char *ser_buf)
{
    unsigned short int pos{0};

    memcpy(this->_opp_username, ser_buf, USERNAME_LENGHT_16);
    pos += USERNAME_LENGHT_16;
    this->tag.deserialize(ser_buf + pos);
}

void
GameMove::serialize(unsigned char *to_ser_buf)
{
    unsigned short int pos{0};
    uint16_t column_num{htons(this->_column_num)};

    memcpy(to_ser_buf, &column_num, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(to_ser_buf + pos, this->_randomness, RANDOMNESS_LENGTH_16);
    pos += RANDOMNESS_LENGTH_16;
    this->tag.serialize(to_ser_buf + pos);
}

void
GameMove::deserialize(unsigned char *ser_buf)
{
    unsigned short int pos{0};
    uint16_t column_num{0};

    memcpy(&column_num, ser_buf, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(this->_randomness, ser_buf + pos, RANDOMNESS_LENGTH_16);
    pos += RANDOMNESS_LENGTH_16;
    this->tag.deserialize(ser_buf + pos);
}


void
GameEnd::serialize(unsigned char *to_ser_buf)
{
    unsigned short int pos{0};
    uint16_t signal{htons(this->_signal)};

    memcpy(to_ser_buf, &signal, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(to_ser_buf + pos, this->_randomness, RANDOMNESS_LENGTH_16);
    pos += RANDOMNESS_LENGTH_16;
    this->tag.serialize(to_ser_buf + pos);
}

void
GameEnd::deserialize(unsigned char *ser_buf)
{
    unsigned short int pos{0};
    uint16_t signal{0};

    memcpy(&signal, ser_buf, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(this->_randomness, ser_buf + pos, RANDOMNESS_LENGTH_16);
    pos += RANDOMNESS_LENGTH_16;
    this->tag.deserialize(ser_buf + pos);
}

/**
 * SECTION
 * Server encrypted messages
 */
void
ClientList::addUser(char *name)
{
    User tmp;
    tmp.setName(name);
    this->users.emplace_back(tmp);
    this->_n_of_elem++;
}

void
ClientList::print()
{
    for(const auto& elem : this->users)
    {
        unsigned short int reminder = USERNAME_LENGHT_16 - strlen(elem._name);
        char format[reminder] = "";
        std::cout << 
            "|" << elem._name << format << "\t| " << "status: " << elem._status << "|" <<
            "\n----------------------------" <<
        std::endl;
    }
}

char *
ClientList::serialize()
{
    uint16_t n_of_elem{htons(this->_n_of_elem)};
    char *ser_buf = new char[(this->_n_of_elem*USERNAME_LENGHT_16)+(this->_n_of_elem*sizeof(uint16_t))];
    unsigned short int pos{0};

    memcpy(ser_buf, &n_of_elem, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    for(const auto& elem : this->users)
    {
        uint16_t status{htons(elem._status)};
        memcpy(ser_buf + pos, elem._name, USERNAME_LENGHT_16);
        pos += USERNAME_LENGHT_16;
        memcpy(ser_buf + pos, &status, sizeof(uint16_t));
        pos += sizeof(uint16_t);
    }
    return ser_buf;
} 

void
ClientList::deserialize(char *ser_buf)
{
    unsigned short int pos{0};
    uint16_t n_of_elem{0};

    memcpy(&n_of_elem, ser_buf, sizeof(uint16_t));
    this->_n_of_elem = ntohs(n_of_elem);
    pos += sizeof(uint16_t);
    for (size_t i = 0; i < this->_n_of_elem; i++)
    {
        uint16_t status{0};
        User tmp;
        memcpy(tmp._name, ser_buf + pos, USERNAME_LENGHT_16);
        pos += USERNAME_LENGHT_16;
        memcpy(&status, ser_buf + pos, sizeof(uint16_t));
        tmp._status = ntohs(status);
        pos += sizeof(uint16_t);
        this->users.push_back(tmp);
    }
}