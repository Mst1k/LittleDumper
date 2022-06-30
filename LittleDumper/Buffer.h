#pragma once

#include <cstdint>

struct Buffer {
    unsigned char* mBuff;
    uint64_t mSize;

    Buffer();
    ~Buffer();
};