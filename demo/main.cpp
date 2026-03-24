#include "blend_file_reader.h"
#include "blend_file_intermediate.h"
#include <cstdlib>
#include <cstring>

int main() {

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