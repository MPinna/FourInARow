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
#include "header.hpp"

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
    void incCounter();
    // void setPayloadSize(char * payload);
    // void setPayloadSize(unsigned short int size);
    void setPayload(unsigned char * data, size_t size);
    unsigned short int  getType();
    unsigned int        getCounter();
    unsigned short int  getPayloadSize();
    // DESCRIPTION: member methods
    void serialize(unsigned char *to_ser_buf);
    void deserializeHeader(unsigned char * ser_buf);
};
#endif