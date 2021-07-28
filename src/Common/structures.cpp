#include "../../include/Utils/structures.hpp"

void
SecurityCheck::updateFields()
{
    _next = _received + 1;
    _expected = _next + 1;
}