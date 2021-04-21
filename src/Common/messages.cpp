#include "include/Messages/packet.hpp"
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <netinet/in.h>

Packet::Packet()
    : _type{0}, _counter{0}, _payload_size{0}, _payload{}
{
}

Packet::Packet(unsigned short int type)
    : _type{type}, _counter{0}, _payload_size{0}, _payload{}
{
}

Packet::Packet(unsigned short int type, unsigned int count)
    : _type{type}, _counter{count}, _payload_size{0}, _payload{}
{
}

Packet::Packet(unsigned short int type, unsigned int count, unsigned char *payload)
    : _type{type}, _counter{count}, _payload_size{strlen((char *)payload)}, _payload{payload}
{
}

// SECTION: Setter
void 
Packet::set_type(short unsigned int type) 
{
    this->_type = type;
}

void 
Packet::set_count(unsigned int count) 
{
    this->_counter = count; 
}

void 
Packet::set_csize(char *payload) 
{
    this->_payload_size = strlen(payload) + 1; // TOCHECK va qua ?
}

void 
Packet::set_isize(unsigned short int size) 
{
    this->_payload_size = size;
}

void
Packet::set_payl(char *payload)
{
    this->_payload = (unsigned char *)malloc(strlen(payload) + 1);
    memcpy(this->_payload, payload + '\0', strlen(payload)); // TOCHECK va qua?
}

// SECTION: Getter
short unsigned int 
Packet::get_type()
{
    return this->_type; 
}
unsigned int 
Packet::get_count()
{
    return this->_counter; 
}
unsigned int 
Packet::get_psize()
{
    return this->_payload_size; 
}
unsigned char *
Packet::get_payload()
{
    return this->_payload;
}

/**
 * SECTION Class members
 */
void Packet::serialize(char *to_ser_buf)
{
    short int pos{0};
    uint16_t type{htons(this->_type)}, psize{htons(this->_payload_size)};
    uint32_t count{htonl(this->_counter)};
    memcpy(to_ser_buf, &type, sizeof(type));
    pos += sizeof(type);
    memcpy(to_ser_buf + pos, &count, sizeof(count));
    pos += sizeof(count);
    memcpy(to_ser_buf + pos, &psize, sizeof(psize));
    pos += sizeof(psize);
    memcpy(to_ser_buf + pos, this->_payload, this->_payload_size);
}

void Packet::deserialize(char *to_deser_buf)
{
    short int pos{0};
    u_int16_t dtype, dpsize;
    u_int32_t dcount;

    memcpy(&dtype, to_deser_buf, sizeof(dtype));
    this->set_type(ntohs(dtype));
    pos += sizeof(dtype);
    memcpy(&dcount, to_deser_buf + pos, sizeof(dcount));
    this->set_count(ntohl(dcount));
    pos += sizeof(dcount);
    memcpy(&dpsize, to_deser_buf + pos, sizeof(dpsize));
    this->set_isize(ntohl(dpsize));
    pos += sizeof(dpsize);
    char tmp_msg[ntohl(dpsize)];
    memcpy(tmp_msg, to_deser_buf + pos, this->get_psize());
    this->set_payl(tmp_msg);
}