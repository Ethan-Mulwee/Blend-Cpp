#include "blend_file_reader.h"
#include "blend_file_intermediate.h"
#include <cstdlib>
#include <cstring>

int main() {
    // TODO: get rid of monolithic blend file structs, instead split into multiple functions and structs
    // 1. function for reading and returning the data and info of the raw bytes of the file
    // 2. function for parsing out the list of data blocks, with the data copied and split into individual buffers for each block
    // 3. function for sorting the data blocks into lists by type
    // 4. function for processing the pointers contain within the data block data to remap them to the correct buffers
    BlendFileReader blend_file_reader = ReadBlendFile("Cube.blend");
    // ExtractSDNATypesToHeaderFile(blend_file);

    
    // LogBlendFileHeader(blend_file);
    
    // LogDataBlocks(blend_file);
    
    
    /* Attempt to read mesh data */
    /* See DNA_mesh_types.h struct Mesh */

    BlendFileIntermediate blend_file_intermediate = SplitDataBlocks(blend_file_reader);
    InterpretDataBlocks(blend_file_intermediate);


    return 0;
}