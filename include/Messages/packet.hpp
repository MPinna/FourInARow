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
#include <openssl/evp.h>
#include "header.hpp"
#include "crypto.hpp"
#include <cassert>

/** DESCRIPTION
 *  The packet class it is used to contain the header of a packet within its payload. This class has built-in functionalities to serialize data according to Network-to-Host and Host-to-Network of linux socket. Such that data can be sent over a TCP socket and interpreted from another client (a peer) or Server
 */
class Packet
{
private:
    Header              header;
public:
    // DESCRIPTION: Constructor
    unsigned char *     _payload;
    Packet();
    ~Packet();
    
    // DESCRIPTION: Setter / Getter
    int setType(unsigned short int type);
    int setPayloadSize(size_t size);
    int setPayload(unsigned char *data, size_t size);
    unsigned short int  getType();
    unsigned int        getCounter();
    unsigned short int  getPayloadSize();
    unsigned short int  getHeaderSize();
    unsigned char *     getPayload();
    
    // DESCRIPTION: member methods
    int     initCounter();
    int     incCounter();
    void    print();
    void    reset();
    int     reallocPayload(unsigned char *data);
    size_t  serialize(unsigned char **buf);
    size_t  htonPacket(unsigned char *buf);
    size_t  hostToNet(unsigned char **buf);
    size_t  ntohHeader(unsigned char *ser_data);
    int     ntohPayload(unsigned char *ser_data);
};

/** DESCRIPTION
 * The esp class, extends the Packet class to manage a generic 'tag' which is attached within a packet. Note: we talk about a generic tag because the tag can assume different meaning:
 * - Can represent the Signature of such packet
 * - Can contain the Auth Data used during the exchanged message with DH-Key
 */
class ESP : public Packet{
    private:
        Tag tag;
    public:
        ESP();
        ~ESP();
        // DESCRIPTION setter / getter
        unsigned short int  getTaglen();
        unsigned char *     getTag();
        int setTag(unsigned char *tag, unsigned short int size);
        // DESCRIPTION Class members
        size_t  getESPPacketSize();
        size_t  HtoN(unsigned char **buf);
        size_t  serializeTag(unsigned char **buf);
        size_t  htonESP(
            unsigned char *pbuf, size_t psize, 
            unsigned char *tbuf, size_t tsize, 
            unsigned char **ser_buf
        );
        size_t  ntohESPPacket(unsigned char *ser_data);
        size_t  ntohTaglen(unsigned char *ser_data);
        int     tagPrint();
};
#endif