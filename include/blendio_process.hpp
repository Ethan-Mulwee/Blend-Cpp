#ifndef BLENDIO_FILE_PROCESS
#define BLENDIO_FILE_PROCESS

#include "blendio_core.hpp"
#include <map>


namespace blendio {
    void InterpretDataBlocks(const DataBlockList& list, const SDNA& sdna);
    
    void LogMesh(const DataBlock& block, const SDNA& sdna, std::map<void*, DataBlockNode*>& map);
}

#endif