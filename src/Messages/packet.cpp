/**
 * DESCRIPTION
 * This file it is used to manage serialization / desarialization over defined messages
 */
#include "../../include/Messages/packet.hpp"
#include "../../include/Messages/crypto.hpp"

Header::Header()
{
    _type = 0;
    _counter = 0;
    _payload_size = 0;
}

Header::~Header()
{
}

// DESCRIPTION: Constructor
Packet::Packet()
    : _payload{NULL}
{
}

Packet::~Packet()
{
}

/**
 * DESCRIPTION
 * Member class of Packet structure
 */
void
Packet::setType(unsigned short int type)
{
    this->header._type = type;
}

void
Packet::initCounter()
{
    this->header._counter = 0;
}

void
Packet::incCounter()
{
    this->header._counter++;
}

// void 
// Packet::setPayloadSize(char *payload)
// {
//     this->header._payload_size = strlen(payload);
// }

// void
// Packet::setPayloadSize(unsigned short int size)
// {
//     this->header._payload_size = size;
// }

void
Packet::setPayload(unsigned char *data, size_t size)
{
    // this->_payload = new unsigned char[(size + 1)];
    this->_payload = (unsigned char *)realloc(this->_payload, size);
    memcpy(this->_payload, data + '\0', size + 1);
    this->header._payload_size = size + 1;
}

unsigned short int
Packet::getType()
{
    return this->header._type;
}

unsigned int
Packet::getCounter()
{
    return this->header._counter;
}

unsigned short int
Packet::getPayloadSize()
{
    return this->header._payload_size;
}

void
Packet::serialize(unsigned char *to_ser_buf)
{
    uint16_t type{htons(this->header._type)};
    uint32_t count{htonl(this->header._counter)};
    uint16_t psize{htons(this->header._payload_size)};
    size_t pos{0};

    memcpy(to_ser_buf, &type, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(to_ser_buf + pos, &count, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(to_ser_buf + pos, &psize, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(to_ser_buf + pos, this->_payload, this->header._payload_size);
}

void 
Packet::deserializeHeader(unsigned char *ser_buf)
{
    short int pos{0};
    uint16_t dtype{0};
    uint32_t dcount{0};
    uint16_t dpsize{0};

    memcpy(&dtype, ser_buf, sizeof(uint16_t));
    this->header._type = (ntohs(dtype));
    pos += sizeof(uint16_t);
    memcpy(&dcount, ser_buf + pos, sizeof(uint32_t));
    this->header._counter = (ntohl(dcount));
    pos += sizeof(uint32_t);
    memcpy(&dpsize, ser_buf + pos, sizeof(uint16_t));
    this->header._payload_size = (ntohs(dpsize));
}