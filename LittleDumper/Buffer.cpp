#include "Buffer.h"

Buffer::Buffer()
    : mBuff(nullptr)
    , mSize(0)
{}

Buffer::~Buffer()
{
    if (mBuff)
        delete[] mBuff;
}
