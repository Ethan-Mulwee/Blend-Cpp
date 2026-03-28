#include "core/blend_data_blocks.hpp"

#include <cstring>
#include <iostream>

DataBlockList ParseDataBlocks(const BlendByteBuffer& buffer) {
    DataBlockList result;

    // skip the header
    uint64_t data_index = 17;

    while (data_index < buffer.size) {
        const char* header_address = &buffer.data[data_index];
        // interpret data as a DataBlockHeader
        DataBlockHeader block_header = *(DataBlockHeader*)header_address;
        data_index += sizeof(DataBlockHeader);

        size_t data_size = block_header.byte_length;
        const char* data_address = buffer.data + data_index;
        char* data = new char[data_size];
        memcpy(data, data_address, data_size);

        data_index += data_size;

        DataBlock data_block {
            .header = block_header,
            .data = data
        };

        result.add(data_block);
    }

    return result;
}

std::map<void*, DataBlockNode*> GetPointerMapping(const DataBlockList& list) {
    std::map<void*, DataBlockNode*> map;
    
    DataBlockNode* node = list.first;
    while(node) {
        map.insert({(void*)node->block.header.old_pointer, node});

        node = node->next;
    }

    return map;
}