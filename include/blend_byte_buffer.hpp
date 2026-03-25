#ifndef BLEND_BYTE_BUFFER
#define BLEND_BYTE_BUFFER

#include <cstdint>

struct BlendByteBuffer {
    const char* data;
    uint64_t size;
};

BlendByteBuffer ReadBlendFileBytes(const char* path);

#endif