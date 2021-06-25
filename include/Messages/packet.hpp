/**
 * DESCRIPTION
 * The Packet structure define the generic structure of a packet, made up by:
 *  - header (type, count, payload size)
 *  - payload(which is used to carry out the information)
 * The Packet class member is in charge of:
 *  - set the payload size (given an array of char)
 *  - manage serialization / deserialization for both:
 *      - header
 *      - payload
 */
#ifndef _PACKET_H_
#define _PACKET_H_
// #define NDEBUG
#include "header.hpp"
#include <cassert>

class Packet
{
private:
    Header          header;
    unsigned char * _payload;
public:
    // DESCRIPTION: Constructor
    Packet();
    ~Packet();
    // DESCRIPTION: Setter / Getter
    void setType(unsigned short int type);
    void initCounter();
    void incrCounter();
    void setPayload(unsigned char * data);
    unsigned short int  getType();
    unsigned int        getCounter();
    unsigned short int  getPayloadSize();
    unsigned char *     getPayload();
    // DESCRIPTION: member methods
    int initPacket(size_t type);
    int reallocPayload(unsigned char *data);
    size_t serialize(unsigned char **buf);
    void deserializeHeader(unsigned char * ser_buf);
    void print();
};
#endif