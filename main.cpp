#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

int main() {
    const int HEADER_LENGTH = 17;

    std::ifstream blend("Cube.blend", std::ios::in | std::ios::binary);

    char* buffer = new char [17 + 1];

    blend.read(buffer, 17);
    buffer[17 + 1] = '\0';

    std::string str = buffer;

    std::cout << "header: " << str << "\n";
    std::cout << "header length: " << str.substr(7, 2) << "\n";
    std::cout << "format version: " << str.substr(10, 2) << "\n";
    std::cout << "blender version: " << str.substr(14, 3) << "\n";
}