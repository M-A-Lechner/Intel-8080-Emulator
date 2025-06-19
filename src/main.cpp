#include <iostream>
#include <string>
#include <limits.h>

#include "definitions/datatypes.h"
#include "definitions/opcodes.h"
#include "processor.h"



void printOutData(OUT_DATA& out_data) {
    unsigned char character = out_data.data[0];
    unsigned char index = 1;
    while(character != 0x03 && index != 0) {
        std::cout << character;
        character = out_data.data[index];
        index += 1; // should have check if index hits 256!
    };
    if (index == 0) {
        std::cerr << "\nOutput error: Char limit reached!";
    }
    std::cout << "\n\nEnd of output!\n";
}

int main (int argc, char *argv[]) {
    Processor cpu{};
    cpu.reset();
    MEMORY memory{};

    std::string filename = "test_binary.bin";

    std::cout << argc << "\n";


    if (argc == 2)
        filename.assign(argv[1]);
    else if (argc > 2)
        throw std::runtime_error("Error: More than one argument given.");

    memory.load_file(filename);


    IN_DATA in_data{};
    OUT_DATA out_data{};

    cpu.execute(12*7, memory, in_data, out_data);
    //printOutData(out_data);

    return 0;
}
