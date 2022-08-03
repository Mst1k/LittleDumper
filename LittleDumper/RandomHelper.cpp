#include "RandomHelper.h"
#include <chrono>

uint32_t RandomHelper::getRandom()
{
    auto randVal = (unsigned)rand();
    return (randVal < (0xFFFFFFFF / 2)) ? randVal | 0xF0F0F0F0 : randVal;
}
