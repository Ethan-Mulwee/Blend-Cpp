#include "blend_process.hpp"
#include "blend_types/sdna_types_gen.h"
#include "blend_types/attribute_types.h"

#include <vector>
#include <iostream>

void InterpretDataBlocks(const DataBlockList& list, const SDNA& sdna) {
    DataBlockNode* node = list.first;
    std::vector<Mesh> meshes = {};
    std::vector<FileGlobal> file_globals = {};
    auto map = GetPointerMapping(list);
    int uncovered_cases = 0;
    while(node) {
        const DataBlock& block = node->block;
        const DataBlockHeader& header= block.header;
        // switch on type index
        switch (header.SDNA_type_index) {

            case 0: { // raw_data
                // std::cout << "raw_data found \n";
                break;
            }

            case 1: { // byte, char, uint_8, etc. (this is never used by a datablock)
                // std::cout << "byte found \n";
                break;
            }

            case 170:  { // FileGlobal
                // std::cout << "FileGlobal found \n";
                FileGlobal file_global = *reinterpret_cast<FileGlobal*>(block.data);
                file_globals.push_back(file_global);
                break;
            }

            case 321: { // Mesh
                std::cout << "Found a mesh \n";
                Mesh mesh; 
                mesh = *reinterpret_cast<Mesh*>(block.data);
                meshes.push_back(mesh);
                LogMesh(block, sdna, map);
                std::cout << "vertices: " << mesh.totvert << "\n";
                break;
            }

            default:
                uncovered_cases++;
                break;
        }

        
        node = node->next;
    }
    std::cout << "Interpret missed " << uncovered_cases << " data blocks" << "\n";
}

const char* TypeNameOfDataBlock(DataBlockNode* node, const SDNA& sdna) {
    short data_type_index = sdna.structs[node->block.header.SDNA_type_index]->type_index;
    const char* data_type = sdna.types[data_type_index];
    return data_type;
}

void LogMesh(const DataBlock& block, const SDNA& sdna, std::map<void*, DataBlockNode*>& map) {
    Mesh mesh = *(Mesh*)block.data;
    std::cout << "SDNA index: " << block.header.SDNA_type_index << "\n";
    std::cout << "Mesh Name: " << mesh.id.name << "\n";
    std::cout << "Vertex Count: " << mesh.totvert << "\n";
    std::cout << "Edge Count: " << mesh.totedge << "\n";
    std::cout << "Face Count: " << mesh.totpoly << "\n";
    std::cout << "Corner Count: " << mesh.totloop << "\n";
    std::cout << "Number of Attributes:" << mesh.attribute_storage.dna_attributes_num << "\n";

    DataBlockNode* poly_offset_indices_data_block = map[(void*)mesh.poly_offset_indices];
    std::cout << "face_offset_indices Type: " << TypeNameOfDataBlock(poly_offset_indices_data_block, sdna) << "\n";
    std::cout << "face_offset_indices Byte Length: " << block.header.byte_length << "\n";
    // int* poly_offset_indices = (int*)blend.GetRawDataAddress(poly_offset_indices_data_block->data_offset);
    int* poly_offset_indices = (int*)poly_offset_indices_data_block->block.data;
    /* See DNA_mesh_types.h faces() */

    /* From the developer docs https://developer.blender.org/docs/features/objects/mesh/mesh/ 
    OffsetIndices is a general abstraction for splitting a larger array into many contiguous groups. 
    Every group is represented by a single integer-- the first index of the elements in the group. 
    The end of the group is simply the next integer in the offsets array. For example, the face offsets 
    [0,3,7,10,14] encode a triangle, a quad, a triangle, and a quad, in that order. */

    /* i.e 7 numbers encode 6 faces explaining the + 1 */
    std::cout << "{";
    for (int poly_offset_indice_idx = 0; poly_offset_indice_idx < mesh.totpoly + 1; poly_offset_indice_idx++) {
        std::cout << poly_offset_indices[poly_offset_indice_idx] << ", ";
    }
    std::cout << "}\n";

    // Attribute* attributes = (Attribute*)map[(void*)mesh.attribute_storage.dna_attributes]->block.data;
    Attribute* attributes = MapPointer(mesh.attribute_storage.dna_attributes, map);
    
    std::cout << "Mesh Attributes: \n";
    for (int attribute_idx = 0; attribute_idx < mesh.attribute_storage.dna_attributes_num; attribute_idx++) {
        Attribute& attribute = attributes[attribute_idx];
        std::cout << "    Name: " << MapPointer(attribute.name, map) << "\n";
        std::cout << "    Type: " << attribute.data_type << "\n";
        std::cout << "    Domain: " << (int)attribute.domain << "\n";

        DataBlockNode* attribute_array_data = MapPointerToBlock(attribute.data, map);
        const char* attribute_array_data_type = TypeNameOfDataBlock(attribute_array_data, sdna);
        // AttributeArray attribute_array = ReadDataBlock<AttributeArray>(blend, attribute_array_data, 0);
        AttributeArray attribute_array = *(AttributeArray*)attribute_array_data->block.data;

        std::cout << "    Data Type: " << attribute_array_data_type << " {\n";
        std::cout << "        Size: " << attribute_array.size << "\n";
        
        // Read raw attribute data bytes
        DataBlockNode* raw_data_block = MapPointerToBlock(attribute_array.data, map);
        const char* raw_data_type = TypeNameOfDataBlock(raw_data_block, sdna);
        raw_data* data = (raw_data*)raw_data_block->block.data;
        uint32_t byte_length = raw_data_block->block.header.byte_length;
        uint32_t number_structs = raw_data_block->block.header.struct_number;
        std::cout << "        Data Type: " << raw_data_type << "\n";
        std::cout << "        Byte Length: " << byte_length << "\n";
        std::cout << "        Number of Structs: " << number_structs << "\n";
        
        for (int array_idx = 0; array_idx < attribute_array.size; array_idx++) {
            switch ((AttrType)attribute.data_type) {
                case AttrType::Float3: {
                    float x, y, z;
                    int data_idx = array_idx * sizeof(float) * 3;
                    x = *reinterpret_cast<float*>(&data[data_idx]);
                    y = *reinterpret_cast<float*>(&data[data_idx + sizeof(float)]);
                    z = *reinterpret_cast<float*>(&data[data_idx + (sizeof(float) * 2)]);
                    
                    std::cout << "        {" << x << ", " << y << ", " << z << "}\n";
                    break;
                }

                case AttrType::Int32_2D: {
                    int32_t a, b;
                    int data_idx = array_idx * sizeof(int32_t) * 2;
                    a = *reinterpret_cast<int32_t*>(&data[data_idx]);
                    b = *reinterpret_cast<int32_t*>(&data[data_idx + sizeof(int32_t)]);

                    std::cout << "        {" << a << ", " << b << "}\n";
                    break;
                }

                case AttrType::Int32: {
                    int32_t i;
                    int data_idx = array_idx * sizeof(int32_t);
                    i = *reinterpret_cast<int32_t*>(&data[data_idx]);

                    std::cout << "        {" << i << "}\n";
                    break;
                }
                
                case AttrType::Bool: {
                    bool b;
                    b = *reinterpret_cast<bool*>(&data[array_idx]);

                    if (b) {
                        std::cout << "        {true}\n";
                    } else {
                        std::cout << "        {false}\n";
                    }
                }

                default:
                    break;
            }
        }


        std::cout << "    }\n";
    }
}