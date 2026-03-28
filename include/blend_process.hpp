#ifndef BLENDIO_FILE_PROCESS
#define BLENDIO_FILE_PROCESS

#include "blendio_core.hpp"
#include <map>

/* TODO: handle pointer mapping better, either remap all the pointers at once 
or do something else to ensure you can still access other datablock data */
void InterpretDataBlocks(const DataBlockList& list, const SDNA& sdna);

void LogMesh(const DataBlock& block, const SDNA& sdna, std::map<void*, DataBlockNode*>& map);

#endif