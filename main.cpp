#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <exception>


// Aka BHead or LargeBHead8 in blender source
struct BlockHeader {
    int32_t code;
    int32_t SDNAnr;
    uint64_t old_pointer;
    int64_t len;
    int64_t nr;
};

struct BlendFile {
    char* header;
    int header_length;
    int format_version;
    int blender_version;

    BlockHeader test_header;
    BlockHeader test_header2;
};



BlendFile ReadBlendFile(const char* path) {
    BlendFile result;

    std::ifstream file(path, std::ios::in | std::ios::binary);

    /* ------------------------------- READ HEADER ------------------------------ */
    // hardcoded because this isn't really expected to change
    int header_length = 17;
    char* header = new char[header_length + 1];
    header[header_length + 1] = '\0';

    file.read(header, 17);

    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    char format_version_cstr[2] = {header[10], header[11]};
    int format_version = atoi(format_version_cstr);

    char blender_version_cstr[4] = {header[14], header[15], header[16], header[17]};
    int blender_version = atoi(blender_version_cstr);

    /* ------------------------------- SET HEADER ------------------------------- */
    result.header = header;
    result.header_length = header_length;
    result.format_version = format_version;
    result.blender_version = blender_version;

    /* ---------------------------- READ BLOCK HEADER --------------------------- */
    BlockHeader block_header;

    file.read((char*)&block_header, sizeof(BlockHeader));
    result.test_header = block_header;

    // advance to next block
    file.seekg(block_header.len, std::ios::seekdir::_S_cur);

    BlockHeader block_header2;
    file.read((char*)&block_header2, sizeof(BlockHeader));
    result.test_header2 = block_header2;


    return result;
}

void Int32ToChar(char a[], int32_t n) {
    memcpy(a, &n, sizeof(int32_t));
}

int main() {

    BlendFile blendFile = ReadBlendFile("Cube.blend");

    std::cout << "header: " << blendFile.header << "\n";
    std::cout << "length: " << blendFile.header_length << "\n";
    std::cout << "format version: " << blendFile.format_version << "\n";
    std::cout << "blender version: " << blendFile.blender_version << "\n";

    std::cout << "\nblock:\n";
    char code[sizeof(int32_t) + 1];
    code[sizeof(int32_t)] = '\0';
    Int32ToChar(code, blendFile.test_header.code);
    std::cout << "code: " << code << "\n";
    std::cout << "SDNA struct type: " << blendFile.test_header.SDNAnr << "\n";
    std::cout << "old pointer: " << blendFile.test_header.old_pointer << "\n";
    std::cout << "byte length: " << blendFile.test_header.len << "\n";
    std::cout << "number of structs: " << blendFile.test_header.nr << "\n";

    std::cout << "\nblock:\n";
    char code2[sizeof(int32_t) + 1];
    code[sizeof(int32_t)] = '\0';
    Int32ToChar(code2, blendFile.test_header2.code);
    std::cout << "code: " << code2 << "\n";
    std::cout << "SDNA struct type: " << blendFile.test_header2.SDNAnr << "\n";
    std::cout << "old pointer: " << blendFile.test_header2.old_pointer << "\n";
    std::cout << "byte length: " << blendFile.test_header2.len << "\n";
    std::cout << "number of structs: " << blendFile.test_header2.nr << "\n";
}