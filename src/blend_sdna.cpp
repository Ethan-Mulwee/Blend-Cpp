#include "blend_sdna.hpp"
#include <stdexcept>
#include <cstring>
#include <fstream>

template <typename T>
T ReadRawDataAs(const char* data, uint64_t data_index) {
    return *((T*)&data[data_index]);
}

void Int32ToChar(char a[], int32_t n) {
    memcpy(a, &n, sizeof(int32_t));
}

int32_t CharToInt32(char a, char b, char c, char d) {
    return (int32_t(d) << 24 | int32_t(c) << 16 | int32_t(b) << 8 | int32_t(a));
}

size_t PadTo4(size_t index) {
    return (size_t)(((index) + 3) & ~3);
    // return index + 2;
}

/* See dna_utils.cc */
int DNA_member_array_num(const char *str) {
    int result = 1;
    int current = 0;
    while (true) {
        char c = *str++;
        switch(c) {
            case '\0':
                return result;
            case '[':
                current = 0;
                break;
            case ']':
                result *= current;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                current = current * 10 + (c - '0');
                break;
            default:
                break;
        }
    }
}

SDNA ParseSDNA(const DataBlockList& blocks) {
    SDNA sdna;

    sdna.types = nullptr;
    sdna.types_size = nullptr;
    sdna.types_alignment = nullptr;
    sdna.structs = nullptr;
    sdna.members = nullptr;
    sdna.members_array_num = nullptr;

    const DataBlock& sdna_block = blocks.last->perv->block;
    const char* data = sdna_block.data;
    uint64_t data_index = 0;

    if (ReadRawDataAs<int>(data, data_index) != CharToInt32('S', 'D', 'N', 'A')) {
        throw std::runtime_error("Error reading SDNA header");
    }

    data_index += sizeof(int);

    /* --------------------------------- Members -------------------------------- */

    /* Initalize member name array */
    if (ReadRawDataAs<int>(data, data_index) == CharToInt32('N', 'A', 'M', 'E')) {
        data_index += sizeof(int);
        sdna.members_num = ReadRawDataAs<int>(data, data_index);
        data_index += sizeof(int);
        sdna.members_num_alloc = sdna.members_num;

        sdna.members = new const char*[sdna.members_num];
        // Zero new array
        memset(sdna.members, 0, sdna.members_num * sizeof(const char*));

        sdna.members_array_num = new short[sdna.members_num];
    } else {
        throw std::runtime_error("Error reading SDNA NAME header");
    }

    /* Find member names */
    for (int member_index = 0; member_index < sdna.members_num; member_index++) {
        const char *member_pointer = &data[data_index];
        sdna.members[member_index] = member_pointer;

        /* Keep going until you find null terminator */
        while(*member_pointer) {
            member_pointer++;
            data_index++;
        }
        /* Go to next member name */
        data_index++;
    }

    data_index = PadTo4(data_index); 

    /* Find array numbers */
    for (int member_index = 0; member_index < sdna.members_num; member_index++) {
        sdna.members_array_num[member_index] = DNA_member_array_num(sdna.members[member_index]); 
    }

    /* ---------------------------------- Types --------------------------------- */

    /* Initalize type name arrays */
    if (ReadRawDataAs<int>(data, data_index) == CharToInt32('T', 'Y', 'P', 'E')) {
        data_index += sizeof(int);

        sdna.types_num = ReadRawDataAs<int>(data, data_index);
        data_index += sizeof(int);

        sdna.types = new const char*[sdna.types_num];
        // Zero new array
        memset(sdna.types, 0, sdna.types_num * sizeof(const char*));
    } else {
        throw std::runtime_error("Error reading SDNA TYPE header");
    }

    /* Find type names */
    for (int type_index = 0; type_index < sdna.types_num; type_index++) {
        const char* type_pointer = &data[data_index];
        sdna.types[type_index] = type_pointer;
        
        /* Keep going until you find null terminator */
        while (*type_pointer) {
            type_pointer++;  
            data_index++;
        }
        /* Go to next type name */
        data_index++;
    }

    data_index = PadTo4(data_index);

    /* --------------------------- Type Length Arrray --------------------------- */
    /* Array is already in the data memory properly so no need to loop over this one */
    short* type_length_pointer;
    if (ReadRawDataAs<int>(data, data_index) == CharToInt32('T', 'L', 'E', 'N')) {
        data_index += sizeof(int);
        type_length_pointer = (short*)&data[data_index];

        sdna.types_size = type_length_pointer;
        data_index += sdna.types_num * sizeof(short);
    } else {
        throw std::runtime_error("Error reading SDNA TYPE LENGTH (TLEN) header");
    }

    /* prevent BUS error? honestly not sure why this is here */
    if (sdna.types_num & 1) {
        data_index += sizeof(short);
    }

    /* ------------------------------ Struct Array ------------------------------ */
    if (ReadRawDataAs<int>(data, data_index) == CharToInt32('S', 'T', 'R', 'C')) {
        data_index += sizeof(int);
        sdna.structs_num = ReadRawDataAs<int>(data, data_index);
        data_index += sizeof(int);

        sdna.structs = new SDNA_Struct*[sdna.structs_num];
        memset(sdna.structs, 0, sdna.structs_num * sizeof(SDNA_Struct));
    } else {
        throw std::runtime_error("Error reading SDNA STRCUT ARRAY (STRC) header");
    }

    /* 
     * Blender does a check to ensure the same struct index isn't used twice here 
     * but I'm not going to bother at least for now
     */

    short* struct_pointer = (short*)&data[data_index];
    for (int struct_index = 0; struct_index < sdna.structs_num; struct_index++) {
        SDNA_Struct *struct_info = (SDNA_Struct*)struct_pointer;
        sdna.structs[struct_index] = struct_info;

        struct_pointer += 2 + (sizeof(SDNA_StructMember) / sizeof(short)) * struct_info->members_num;
    }

    /* 
     * Here pointer_size is normally calculated but since this is for blender 5.0 
     * 64bit or 8 bytes is assumed by this library 
     */

    /* ----------------------------- Type Alignment ----------------------------- */
    sdna.types_alignment = new int[sdna.types_num];
    for (int type_index = 0; type_index < sdna.types_num; type_index++) {
       sdna.types_alignment[type_index] = int(16UL);
    }

    return sdna;
}

void WriteSDNA(const SDNA& sdna, const char* path) {
    std::fstream file(path, std::ios::out);

    file << "// file autogenerated by WriteSDNA() in blend_sdna.cpp \n\n";

    file << "#include <cstdint>\n\n";

    /* forward declaration to avoid ordering issues */
    for (int i = 0; i < sdna.structs_num; i++) {
        SDNA_Struct* struct_pointer = sdna.structs[i];
        file << "struct " << sdna.types[struct_pointer->type_index] << ";\n";
    }

    file << "\n";

    /* These members have size zero and are almost exclusively just named pointers */
    for (int i = 0; i < sdna.types_num; i++) {
        const char* type = sdna.types[i];
        if ((strcmp(type, "void") == 0) || (strcmp(type, "bool") == 0) || (strcmp(type, "raw_data") == 0)) {
            continue;
        }
        if (sdna.types_size[i] == 0) {
            file << "typedef void " << type << ";\n";
        }
    }
    file << "typedef unsigned short ushort;\n";
    file << "typedef unsigned char uchar;\n";

    file << "\n";

    /* TODO: sorting system to avoid ordering issues */

    for (int i = 0; i < sdna.structs_num; i++) {
        SDNA_Struct* struct_pointer = sdna.structs[i];
        file << "struct " << sdna.types[struct_pointer->type_index] << " { \n";
        for (int member_index = 0; member_index < struct_pointer->members_num; member_index++) {
            SDNA_StructMember member = struct_pointer->members[member_index];
            
            file << "\t" << sdna.types[member.type_index] << " ";
            file << "" << sdna.members[member.member_index] << ";\n";
        }
        file << "}; \n\n";
    }

    file.close();
}