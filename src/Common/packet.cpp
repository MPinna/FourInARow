/**
 * DESCRIPTION
 * This file it is used to manage serialization / desarialization over defined messages
 */
#include "Messages/Client/auth.hpp"
#include "Messages/Server/auth.hpp"
#include "Messages/Client/enc.hpp"
#include "Messages/Server/enc.hpp"
#include "Messages/header.hpp"
#include "Messages/packet.hpp"

// NOTE: before implementing ser / des class member, check its correctness