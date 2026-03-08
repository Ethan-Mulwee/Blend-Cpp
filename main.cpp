#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <exception>


struct BlendFile {
    char* header;
    int header_length;

    int format_version;
    int blender_version;
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


    return result;
}

int main() {

    BlendFile blendFile = ReadBlendFile("Cube.blend");

    std::cout << "header: " << blendFile.header << "\n";
    std::cout << "header length: " << blendFile.header_length << "\n";
    std::cout << "format version: " << blendFile.format_version << "\n";
    std::cout << "blender version: " << blendFile.blender_version << "\n";
}