/**
 * DESCRIPTION 
 * The protocol section is responsible to 
 *  - manage the structure of the packet sent over the network between client and server or peer-to-peer. 
 *  - manage all generic settings used to set-up the SSL connection
 */
#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_
#include "Messages/packet.hpp"

/**
 * DESCRIPTION:
 * This structure is used to manage authentication packet (e.g. packet with require digital signature)
 */
struct auth_protocol 
{
    Packet          data;
    unsigned char * _signature;
}__attribute__((packed));

/**
 * DESCRIPTION:
 * This structure is used to manage AEAD packet (e.g. a form of encryption which simultaneously assure the confidentiality and authenticity of data)
 */
struct enc_protocol
{
    Packet data;
    unsigned char * _tag;
}__attribute__((packed));
#endif