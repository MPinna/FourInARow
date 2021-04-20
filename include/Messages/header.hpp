/**
 * SECTION
 * The packetHeader.h file define the structure of the packet header used to exchange message in the Four-in-a-row game
 */
#ifndef _PACKETHEADER_H_
#define _PACKETHEADER_H_
class Header
{
private:
    /* data size = 9 Bytes */
    unsigned short int  _type; // TODO check allpacket type
    unsigned int        _counter;
    unsigned int        _payload_size;
    unsigned char *     _payload;

public:
    // SECTION Default Constructor
    Header();
    Header(unsigned short int type);
    Header(unsigned short int type, unsigned int count);
    Header(unsigned short int type, unsigned int count, unsigned char *payload);
    // SECTION: Setter
    void set_type(short unsigned int type);
    void set_count(unsigned int count);
    void set_csize(char *payload);
    void set_isize(unsigned int size);
    void set_payl(char *payload);
    // SECTION: Getter
    short unsigned int get_type();
    unsigned int get_count();
    unsigned int get_psize();
    unsigned char *get_payload();
    // SECTION: Member methods
    void serialize(Header *packet, char *ser_buf);
    void deserilize(char *ser_buf, Header *packet);
} __attribute__((packed));
#endif