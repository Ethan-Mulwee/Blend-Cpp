#ifndef BLENDIO_BYTE_BUFFER
#define BLENDIO_BYTE_BUFFER

#include <cstdint>

namespace blendio {
    struct BlendByteBuffer {
        const char* data;
        uint64_t size;
    };
    
    BlendByteBuffer ReadBlendFileBytes(const char* path);
}


#endif