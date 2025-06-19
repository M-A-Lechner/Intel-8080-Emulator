#include "memory.h"


void MEMORY::load_file(std::string filename) {
    word address = 0x0000;

    std::ifstream in_file;
    in_file.open(filename);

    byte next_byte = 0;

    while (in_file >> std::hex >> next_byte) {
        
        std::clog << "Current byte: " << std::hex << next_byte << "\n";
        data[address] = next_byte;
        address += 1;
    }

    in_file.close();
}
