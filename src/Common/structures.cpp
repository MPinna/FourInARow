#include "../../include/Utils/structures.hpp"

void
SecurityCheck::updateFields()
{
    _expected = _received + 1;
}