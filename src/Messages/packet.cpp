/**
 * DESCRIPTION
 * This file it is used to manage serialization / desarialization over defined messages
 */
#include "../../include/Messages/packet.hpp"
#include "../../include/Messages/crypto.hpp"

size_t
Header::HtoN(unsigned char *buf)
{
    size_t pos{0};
    uint16_t type{htons(_type)};
    uint32_t count{htonl(_counter)};
    uint16_t psize{htons(_payload_size)};

    memcpy(buf, &type, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(buf + pos, &count, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(buf + pos, &psize, sizeof(uint16_t));
    pos += sizeof(uint16_t);

    return pos;
}

size_t
Header::serialize(unsigned char *buf)
{
    size_t pos{0};
    memcpy(buf, &_type, sizeof(uint16_t));
    pos += sizeof(uint16_t);
    memcpy(buf + pos, &_counter, sizeof(uint32_t));
    pos += sizeof(uint32_t);
    memcpy(buf + pos, &_payload_size, sizeof(uint16_t));
    pos += sizeof(uint16_t);

    return pos;
}

size_t
Header::NtoH(unsigned char *header_buf)
{
    short int pos{0};
    uint16_t dtype{0};
    uint32_t dcount{0};
    uint16_t dpsize{0};

    memcpy(&dtype, header_buf, sizeof(uint16_t));
    this->_type = (ntohs(dtype));
    pos += sizeof(uint16_t);
    memcpy(&dcount, header_buf + pos, sizeof(uint32_t));
    this->_counter = (ntohl(dcount));
    pos += sizeof(uint32_t);
    memcpy(&dpsize, header_buf + pos, sizeof(uint16_t));
    this->_payload_size = (ntohs(dpsize));
    pos += sizeof(uint16_t);

    return pos;
}

Packet::Packet()
    : header{0, 0, 0}, _payload{NULL}
{
}

Packet::~Packet()
{
}

void Packet::reset()
{
    this->header._type = 0;
    this->header._counter = 0;
    this->header._payload_size = 0;
    this->_payload = NULL;
}

/**
 * DESCRIPTION
 * Member class of Packet structure
 */
int Packet::setType(unsigned short int type)
{
    if (type > SIZE_MAX / sizeof(unsigned short int))
    {
        std::cerr << "Packet::setType: SIZE_MAX overcame";
        return -1;
    }
    this->header._type = type;
    return type;
}

int Packet::setPayloadSize(size_t size)
{
    if (size > SIZE_MAX / sizeof(size_t))
    {
        std::cerr << "Packet::setPayloadSize(): SIZE_MAX overcame";
        return -1;
    }
    this->header._payload_size = size;
    return 1;
}

int Packet::setPayload(unsigned char *data, size_t size)
{
    if (size > SIZE_MAX / sizeof(unsigned short int))
    {
        std::cerr << "Packet::setPayload(): SIZE_MAX overcame";
        return -1;
    }
    // this->_payload = new unsigned char[(size) + 1];
    this->_payload = new unsigned char[(size)];
    memcpy(this->_payload, data, size);
    // this->_payload[size + 1] = '\0';
    // this->header._payload_size = size+1;
    this->header._payload_size = size;

    return size;
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

unsigned short int
Packet::getHeaderSize()
{
    return sizeof(struct Header);
}

unsigned char *
Packet::getPayload()
{
    return this->_payload;
}

int Packet::initCounter()
{
    unsigned int seed;
    FILE *urnd;
    urnd = fopen("/dev/urandom", "rb");
    if (!urnd)
    {
        std::cerr << "Header::Header::fopen() failed!" << std::endl;
        return -1;
    }
    if ((fread(&seed, 1, sizeof(seed), urnd)) != sizeof(seed))
    {
        std::cerr << "Header::Header::fread() failed!" << std::endl;
        return -1;
    }
    fclose(urnd);
    srand(seed);
    this->header._counter = rand();
    return 1;
}

int Packet::incCounter()
{
    if (this->header._counter == UINT_MAX)
    {
        std::cerr << "Packet::incCounter() UINT_MAX reached";
        return -1;
    }
    this->header._counter++;
    return 1;
}

int Packet::reallocPayload(unsigned char *data, size_t size)
{
    if(size == 0)
    {
        this->_payload = (unsigned char *)realloc(this->_payload, 1 * sizeof(char));
        if (!this->_payload)
        {
            std::cerr << "Packet::reallocPayload(1): something went wrong =>";
            return -1;
        }
        memset(this->_payload, 0, 1);
        this->header._payload_size = 0;

        return this->header._payload_size;
    }
    else
    {
        this->_payload = (unsigned char *)realloc(this->_payload, size * sizeof(char));
        // this->_payload = (unsigned char *)realloc(this->_payload, size * sizeof(char) + 1);
        if (!this->_payload)
        {
            std::cerr << "Packet::reallocPayload(2): something went wrong =>";
            return -1;
        }
        memcpy(this->_payload, data, size);
        // this->_payload[size+1] = '\0';
        this->header._payload_size = size;
        // this->header._payload_size = size + 1;

        return size+1;
    }
}

/**
 * @param: packet_buf
 * @return: pos
 * DESCRIPTION: the function serialize the packet according to ntoh format and return the position of the buffer, so that the htonESPPacket can continue from this point 
 */
size_t
Packet::htonPacket(unsigned char *packet_buf)
{
    size_t pos{0};

    pos = this->header.HtoN(packet_buf);
    memcpy(packet_buf + pos, this->_payload, this->header._payload_size);
    pos += this->header._payload_size;

    return pos;
}

size_t
Packet::htonPacket(unsigned char **buf)
{
    size_t packet_size{sizeof(struct Header) + this->header._payload_size};
    *buf = new unsigned char[packet_size];
    size_t pos{0};
    pos = this->header.HtoN(*buf);
    memcpy(*buf + pos, this->_payload, this->header._payload_size);
    pos += this->header._payload_size;

    return packet_size;
}

size_t
Packet::serialize(unsigned char **packet_buf)
{
    size_t packet_size{sizeof(struct Header) + this->header._payload_size};
    *packet_buf = new unsigned char[packet_size];
    size_t pos{0};
    pos = this->header.serialize(*packet_buf);
    memcpy(*packet_buf + pos, this->_payload, this->header._payload_size);
    pos += this->header._payload_size;

    return packet_size;
}

size_t
Packet::ntohHeader(unsigned char *ser_data)
{
    return this->header.NtoH(ser_data);
}

int Packet::ntohPayload(unsigned char *ser_data)
{
    return 0;
}

void Packet::print()
{
    std::cout << 
        "\nType: " << this->header._type << 
        "\nCounter: " << this->header._counter << 
        "\nPayload Size: " << this->header._payload_size << 
        "\nPayload: " << this->_payload // TODO cancel this line
    << std::endl;
}

ESP::ESP()
    : tag{0, NULL}
{
}

ESP::~ESP()
{
}

unsigned short int
ESP::getTaglen()
{
    if (this->tag._taglen > USHRT_MAX)
    {
        std::cerr << "AuthPacket::getSigLen() USHRT_MAX";
        return 0;
    }
    return this->tag._taglen;
}

unsigned char *
ESP::getTag()
{
    assert(!this->tag._tag);
    return this->tag._tag;
}

int ESP::setTag(unsigned char *signature, unsigned short int size)
{
    return this->tag.setTag(signature, size);
}

size_t
ESP::getSize()
{
    return this->getHeaderSize() +
           this->getPayloadSize() +
           sizeof(this->tag._taglen) +
           this->tag._taglen;
}

size_t
ESP::getTagSize()
{
    return sizeof(this->tag._taglen) +
           this->tag._taglen;
}

/**
 * @param: authpacket_buf
 * @return: packet_size
 * DESCRIPTION: The function serialize the ESP according to ntoh format and return the entire packet size 
 */
size_t
ESP::HtoN(unsigned char **authpacket_buf)
{
    size_t pos{0}, size{this->getSize()};

    *authpacket_buf = new unsigned char[size];
    pos = this->htonPacket(*authpacket_buf + pos);
    pos = this->tag.HtoN(*authpacket_buf + pos);
    return size;
}

size_t
ESP::serializeTag(unsigned char **buf)
{
    size_t tsize{0};
    *buf = new unsigned char[this->getTaglen() + sizeof(this->tag._taglen)];
    tsize = this->tag.HtoN(*buf);

    return tsize;
}

size_t
ESP::htonTag(unsigned char *buf)
{
    return this->tag.HtoN(buf);
}


size_t
ESP::htonESP(unsigned char *pbuf, size_t psize, unsigned char *tbuf, size_t tsize, unsigned char **ser_buf)
{
    size_t size{psize + tsize + 1};
    size_t pos{0};
    *ser_buf = new unsigned char[size];
    memcpy(*ser_buf, pbuf, psize);
    pos += psize;
    memcpy(*ser_buf, tbuf, tsize);
    *ser_buf[size] = '\0';

    return size;
}


size_t
ESP::ntohESPPacket(unsigned char *ser_data)
{
    return this->tag.NtoH(ser_data);
}

size_t
ESP::ntohTaglen(unsigned char *ser_data)
{
    return this->tag.NtoHtaglen(ser_data);
}

int ESP::printTag()
{
    return this->tag.print();
}