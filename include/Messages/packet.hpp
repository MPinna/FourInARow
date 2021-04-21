/**
 * SECTION
 * The packetHeader.h file define the structure of the packet header used to exchange message in the Four-in-a-row game
 */
#ifndef _PACKET_H_
#define _PACKET_H_
class Packet
{
private:
    /* data size = 9 Bytes */
    unsigned short int  _type; // TODO check allpacket type
    unsigned int        _counter;
    unsigned short int  _payload_size; // To allocate buffer space
    unsigned char *     _payload;

public:
    // SECTION Default Constructor
    Packet();
    Packet(unsigned short int type);
    Packet(unsigned short int type, unsigned int count);
    Packet(unsigned short int type, unsigned int count, unsigned char *payload);
    
    // SECTION: Setter
    void set_type(short unsigned int type);
    void set_count(unsigned int count);
    void set_csize(char *payload);
    void set_isize(unsigned short int size);
    void set_payl(char *payload);
    
    // SECTION: Getter
    short unsigned int get_type();
    unsigned int get_count();
    unsigned int get_psize();
    unsigned char *get_payload();
    
    // SECTION: Member methods
    void serialize(char *to_ser_buf);
    void deserialize(char *to_deser_buf);
} __attribute__((packed));
#endif