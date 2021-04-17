#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_
#include "Messages/p2pChallRes.hpp"
#include "Messages/p2pMessages.hpp"
#include "Messages/packetHeader.hpp"
#include "Messages/clientServerChallRes.hpp"
#include "Messages/clientServerMessages.hpp"

/**
 * @data: structure containing data header
 * @serilized_packet: buffer on unsigned char used to serialize data
 * @return: void
 */
void serializeHeader(header* data, unsigned char *serialized_packet);

#endif