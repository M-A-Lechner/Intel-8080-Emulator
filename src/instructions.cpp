#include <stdexcept>
#include <sstream>

#include "defitnitions/datatypes.h"

namespace instructions {
    void execute_instruction(byte opcode) {
        byte operation_area = opcode >> 6;

        switch (operation_area) {

            // 0b00
            case 0: {

            } break;

            // 0b01 describes all MOV instructions
            case 1: {
                move_instruction(opcode);
            } break;

            // 0b10
            case 2: {

            } break;

            // 0b11
            case 3: {

            } break;
            default: {
                std::stringstream error_description = "The program encountered the following illegal instruction: " << opcode << "\n";
                throw std::runtime_error(error_description.str());
            }
        };
    }

    void move_instruction(byte opcode) {
        byte src_code = opcode & 0x07;
        byte dst_code = opcode & 0x38;
    }
}