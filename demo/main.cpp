// #include "blend_file_reader.h"
// #include "blend_file_intermediate.h"
// #include <cstdlib>
// #include <cstring>

#include <iostream>

// TODO: get rid of monolithic blend file structs, instead split into multiple functions and structs
// 1. function for reading and returning the data and info of the raw bytes of the file
// #include "core/blend_byte_buffer.hpp"

// 2. function for parsing out the list of data blocks, with the data copied and split into individual buffers for each block
// #include "core/blend_data_blocks.hpp"

// 3. function for reading SDNA data block
// #include "core/blend_sdna.hpp"

// #include "generated_code.h"
#include "blendio_core.hpp"
#include "blendio_process.hpp"

// 4. function for sorting the data blocks into lists by type
// #include "blend_process.hpp"
// 5. function for processing the pointers contain within the data block data to remap them to the correct buffers


using namespace blendio;

int main() {
    // BlendFileReader blend_file_reader = ReadBlendFile("Cube.blend");
    // BlendFileIntermediate blend_file_intermediate = SplitDataBlocks(blend_file_reader);
    // InterpretDataBlocks(blend_file_intermediate);


    BlendByteBuffer blend_bytes = ReadBlendFileBytes("Reference.blend");
    DataBlockList block_list = ParseDataBlocks(blend_bytes);
    SDNA sdna = ParseSDNA(block_list);
    // for (int i = 0; i < sdna.members_num; i++) {
    //     std::cout << sdna.members[i] << "\n";
    // }
    auto mapping = GetPointerMapping(block_list);
    InterpretDataBlocks(block_list, sdna);


    return 0;
}