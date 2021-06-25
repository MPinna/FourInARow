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
Packet::incrCounter()
{
    this->header._counter++;
}

void 
Packet::setPayload(unsigned char *data)
{
    size_t size{strlen((char *)data)};
    this->_payload = new unsigned char[(size + 1)];
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

unsigned char *
Packet::getPayload()
{
    return this->_payload;
}

int
Packet::initPacket(size_t type)
{
    int _ret_code{-1};
    this->setType(type);
    assert(this->header._type > 0 || this->header._type < 11);
    if(this->header._type < 0 || this->header._type > 11)
    {
        std::cerr << "Packet::initPacket() Unknown type";
        return -1;
    }
    this->initCounter();
    assert(this->header._counter == 0);
    if(this->header._counter != 0)
    {
        std::cerr << "Packet::initPacket() Unknown type";
        return -1;
    }

    return 1;
}

int
Packet::reallocPayload(unsigned char *data)
{
    size_t size{strlen((char *)data)};
    if(size == 0)
    {
        this->_payload = (unsigned char *)realloc(this->_payload, 1 * sizeof(char));
        if(!this->_payload)
        {
            std::cerr << "Packet::reallocPayload(1): something went wrong =>";
            return -1;
        }
        memset(this->_payload, 0, 1);
        this->header._payload_size = 0;
    }
    else
    {
        this->_payload = (unsigned char *)realloc(this->_payload, size * sizeof(char));
        if(!this->_payload)
        {
            std::cerr << "Packet::reallocPayload(2): something went wrong =>";
            return -1;
        }
        memcpy(this->_payload, data + '\0', size + 1);
        this->header._payload_size = size + 1;
    }

    return 1;
}

size_t
Packet::serialize(unsigned char **buf)
{
    uint16_t type{htons(this->header._type)};
    uint32_t count{htonl(this->header._counter)};
    uint16_t psize{htons(this->header._payload_size)};
    size_t pos{0};
    size_t buf_size{sizeof(struct Header) + this->getPayloadSize()};
    *buf = new unsigned char[buf_size];

    memcpy(*buf, &type, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(*buf + pos, &count, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(*buf + pos, &psize, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(*buf + pos, this->_payload, this->header._payload_size);

    return buf_size;
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

void
Packet::print()
{
    std::cout <<
        "\nType:        " << this->header._type <<
        "\nCounter:     " << this->header._counter <<
        "\nPayload Size:" << this->header._payload_size << 
        "\nPayload:     " << this->_payload <<
    std::endl;
}