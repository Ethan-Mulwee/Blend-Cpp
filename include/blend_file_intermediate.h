#ifndef BLEND_FILE_INTERMEDIATE
#define BLEND_FILE_INTERMEDIATE

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <exception>
#include <cstdint>
#include <map>
#include <vector>

#include "sdna_structs.h"
#include "attribute_types.h"
#include "blend_file_reader.h"


/* 
 * This is a inbetween step that provides easy editting that be read from a blend file
 * or written to a blend file
 */

/* Structure designed to simplify reading and writing blend files  */
struct BlendFileIntermediate {
    DataBlockList split_data_blocks;

    std::vector<FileGlobal*> file_global_blocks;
    std::vector<Mesh*> mesh_blocks;
};

BlendFileIntermediate SplitDataBlocks(BlendFileReader& blend);

/* TODO: handle pointer mapping better, either remap all the pointers at once 
or do something else to ensure you can still access other datablock data */
void InterpretDataBlocks(BlendFileIntermediate& blend);

void LogMesh(BlendFileReader& blend, FileDataBlockNode* node, Mesh& mesh);

#endif