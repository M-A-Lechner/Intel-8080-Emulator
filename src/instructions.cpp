#include <iostream>

#include "instructions.h"

namespace instructions {
    void execute_instruction(byte opcode, Processor& processor, MEMORY& memory) {
        switch (byte operation_area = opcode >> 6) {
            
            // 0b00
            case 0: {
                byte instruction_code = opcode & 0b00000111;
                switch (instruction_code) {
                    case (4): {
                        add_sub_instruction(opcode, processor, memory, 1, "INR");
                    } break;
                    case (5): {
                        add_sub_instruction(opcode, processor, memory, -1, "DCR");
                    } break;
                    default: {
                        std::stringstream err;
                        err << "The program encountered the following illegal instruction: " << opcode << "\n";
                        throw std::runtime_error(err.str());
                    }
                };
            } break;

            // 0b01 describes all MOV instructions
            case 1: {
                move_instruction(opcode, processor, memory);
            } break;

            // 0b10
            case 2: {

            } break;

            // 0b11
            case 3: {

            } break;
            default: {
                std::stringstream err;
                err << "The program encountered the following illegal instruction: " << opcode << "\n";
                throw std::runtime_error(err.str());
            }
        };

        processor.log_registers(memory);
        processor.log_flags();
        processor.increase_counter();
    }

    void add_sub_instruction(byte opcode, Processor& processor, MEMORY& memory, signed char amount, std::string instruction_name) {
        byte reg_code = ((opcode & 0b00111000) >> 3);
        byte& reg = processor.get_register_by_code(memory, reg_code);

        processor.flags.AC = (((reg & 0x0F) + amount) > 0x0F);

        reg += amount;
        processor.flags.S = (reg >> 7) & 1;
        processor.flags.Z = (reg == 0) ? 1 : 0;

        byte v = reg;
        bool parity = true;
        while (v) {
            parity = !parity;
            v = v & (v - 1);
        }
        processor.flags.P = parity;
        std::clog << instruction_name << " " << processor.get_register_name_by_code(reg_code) << "\n";
    }

    void move_instruction(byte opcode, Processor& processor, MEMORY& memory) {
        byte src_code = opcode & 0x07;
        byte dst_code = (opcode & 0x38) >> 3;
        byte& src = processor.get_register_by_code(memory, src_code);
        byte& dst = processor.get_register_by_code(memory, dst_code);

        if (!((src_code == dst_code) && (src_code == 0b110))) {
            dst = src;
        } else {
            processor.halt();
        }

        std::clog << "MOV " << processor.get_register_name_by_code(dst_code) << "," << processor.get_register_name_by_code(src_code) << "\n";
    }
}