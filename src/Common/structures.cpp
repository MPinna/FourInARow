#include "../../include/Utils/structures.hpp"

void
SecurityCheck::updateFields()
{
    _sent = _received + 1;
    _expected = _sent + 1;
}