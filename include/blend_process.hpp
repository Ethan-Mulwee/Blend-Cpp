#ifndef BLEND_FILE_PROCESS
#define BLEND_FILE_PROCESS

#include "blend_data_blocks.hpp"

/* TODO: handle pointer mapping better, either remap all the pointers at once 
or do something else to ensure you can still access other datablock data */
void InterpretDataBlocks(const DataBlockList& list);

// void LogMesh(BlendFileReader& blend, FileDataBlockNode* node, Mesh& mesh);

#endif