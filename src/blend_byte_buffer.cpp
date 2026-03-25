#include "blend_byte_buffer.hpp"

#include <fstream>

BlendByteBuffer ReadBlendFileBytes(const char* path) {
    BlendByteBuffer buffer;

    std::ifstream file_reader(path, std::ios::in | std::ios::binary);

    if (!file_reader) {
        throw std::runtime_error("Failed to open file");
    }

    /* --------------------------- READ RAW FILE DATA --------------------------- */

    file_reader.seekg(0, std::ios::end);
    size_t data_length = file_reader.tellg();

    file_reader.seekg(0, std::ios::beg);
    
    char* data = new char[data_length];
    file_reader.read(data, data_length);
    buffer.data = data;
    buffer.size = data_length;

    file_reader.close();

    return buffer;
}