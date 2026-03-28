#ifndef BLENDIO_DATA_BLOCKS
#define BLENDIO_DATA_BLOCKS

#include "blendio_byte_buffer.hpp"

// TODO: use a more efficent structure
#include <map>

namespace blendio {
    /* Aka BHead or LargeBHead8 in blender source */
    struct DataBlockHeader {
        int32_t code;
        // SDNAnr
        int32_t SDNA_type_index;
        uint64_t old_pointer;
        // len
        int64_t byte_length;
        // nr
        int64_t struct_number;
    };
    
    struct DataBlock {
        DataBlockHeader header;
        char* data;
    };
    
    struct DataBlockNode {
        DataBlockNode *next = nullptr, *perv = nullptr;
        DataBlock block;
    };
    
    struct DataBlockList {
        DataBlockNode* first = nullptr;
        DataBlockNode* last = nullptr;
    
        inline void add(DataBlock block) {
            DataBlockNode* node = new DataBlockNode();
            node->block = block;
            if (first) {
                node->perv = last;
                last->next = node;
                last = node;
            } else {
                first = node;
                last = node;
            }
        }
    };
    
    DataBlockList ParseDataBlocks(const BlendByteBuffer& buffer);
    
    std::map<void*, DataBlockNode*> GetPointerMapping(const DataBlockList& list);
    
    template <typename T>
    T* MapPointer(T* ptr, std::map<void*, DataBlockNode*>& map) {
        DataBlockNode* data_block_node = map[ptr];
        T* mapped_pointer = (T*)data_block_node->block.data;
        return mapped_pointer;
    }
    
    template <typename T>
    DataBlockNode* MapPointerToBlock(T* ptr, std::map<void*, DataBlockNode*>& map) {
        DataBlockNode* data_block_node = map[ptr];
        return data_block_node;
    }
}

#endif