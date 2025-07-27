#include <iostream>
#include <string>
#include <limits.h>

#include "definitions/datatypes.h"
#include "definitions/opcodes.h"
#include "argument_parsing/argument_parser.h"
#include "processor.h"
#include "instructions.h"



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
    MEMORY memory{};
    IN_DATA in_data{};
    OUT_DATA out_data{};

    std::string filename = "";

    Argument_Parser arg_p{};
    arg_p.add_argument("help", std::vector<std::string>{"-h", "--help"}, false, true);
    arg_p.add_argument("file", std::vector<std::string>{"-f", "--file"}, true, false);
    arg_p.add_argument("debug", std::vector<std::string>{"-d", "--debug"}, false, false);

    auto args = arg_p.parse_list(argc, argv);

    if (args.contains("help")) {
        std::cout << "Intel 8080 Emulator Version 0.61\n\n";
        std::cout << "Useage: main.out <file> | main.out [flags][files...]\n\n";
        std::cout << "Arguments:\n";
        std::cout << "  -h --help           Shows this screen.\n";
        std::cout << "  -f --file <file>    Required! Specify binary file for the program to read. If no other flags are set, this flag can be omitted, and only the file specified.\n";
        std::cout << "  -d --debug          Enables debug mode. Shows detailed information on what instructions were run and what the state of the emulator was.\n";
        return 0;
    }
    if (args.contains("file")) {
        filename = args["file"];
        
    } else {
        throw std::runtime_error("An input file is required to run this program. Please specify a binary file as such: -f <filename>");
    }
    if (args.contains("debug")) {
        cpu.DEBUG_MODE = true;
    }
    
    memory.load_file(filename);

    instructions::init_instruction_table();

    cpu.reset();
    cpu.execute(memory, in_data, out_data);

    return 0;
}
