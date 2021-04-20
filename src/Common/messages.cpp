#include "include/Messages/header.hpp"
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <netinet/in.h>

Header::Header()
    : _type{0}, _counter{0}, _payload_size{0}, _payload{}
{
}

Header::Header(unsigned short int type)
    : _type{type}, _counter{0}, _payload_size{0}, _payload{}
{
}

Header::Header(unsigned short int type, unsigned int count)
    : _type{type}, _counter{count}, _payload_size{0}, _payload{}
{
}

Header::Header(unsigned short int type, unsigned int count, unsigned char *payload)
    : _type{type}, _counter{count}, _payload_size{strlen((char *)payload)}, _payload{payload}
{
}

// SECTION: Setter
void 
Header::set_type(short unsigned int type) 
{
    this->_type = type;
}

void 
Header::set_count(unsigned int count) 
{
    this->_counter = count; 
}

void 
Header::set_csize(char *payload) 
{
    this->_payload_size = strlen(payload) + 1; 
}

void 
Header::set_isize(unsigned int size) 
{
    this->_payload_size = size;
}

void
Header::set_payl(char *payload)
{
    this->_payload = (unsigned char *)malloc(strlen(payload) + 1);
    memcpy(this->_payload, payload + '\0', strlen(payload));
}

// SECTION: Getter
short unsigned int 
Header::get_type()
{
    return this->_type; 
}
unsigned int 
Header::get_count()
{
    return this->_counter; 
}
unsigned int 
Header::get_psize()
{
    return this->_payload_size; 
}
unsigned char *
Header::get_payload()
{
    return this->_payload;
}

// SECTION Class members
void Header::serialize(Header *packet, char *buf)
{
    short int pos{0};
    uint16_t type{htons(packet->get_type())};
    uint32_t count{htonl(packet->get_count())}, psize{htonl(packet->get_psize())};
    memcpy(buf, &type, sizeof(type));
    pos += sizeof(type);
    memcpy(buf + pos, &count, sizeof(count));
    pos += sizeof(count);
    memcpy(buf + pos, &psize, sizeof(psize));
    pos += sizeof(psize);
    memcpy(buf + pos, packet->get_payload(), packet->get_psize());
}

void Header::deserilize(char *buf, Header *packet)
{
    short int pos{0};
    u_int16_t dtype;
    u_int32_t dcount, dpsize;

    memcpy(&dtype, buf, sizeof(dtype));
    packet->set_type(ntohs(dtype));
    pos += sizeof(dtype);
    memcpy(&dcount, buf + pos, sizeof(dcount));
    packet->set_count(ntohl(dcount));
    pos += sizeof(dcount);
    memcpy(&dpsize, buf + pos, sizeof(dpsize));
    packet->set_isize(ntohl(dpsize));
    pos += sizeof(dpsize);
    char tmp_msg[ntohl(dpsize)];
    memcpy(tmp_msg, buf + pos, packet->get_psize());
    packet->set_payl(tmp_msg);
}