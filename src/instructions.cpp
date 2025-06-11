#include <iostream>

#include "instructions.h"

namespace instructions {
    void execute_instruction(byte opcode, Processor& processor, MEMORY& memory) {
        
        switch (byte operation_area = opcode >> 6) {
            
            // 0b00
            case 0: {
                switch (opcode & 0b00000111) {
                    case (0b000): {
                        std::clog << "NOP\n";
                    } break;
                    case (0b010): {
                        if ((opcode & 0b00001000) >> 3 == 1)
                            load_instruction(opcode, processor, memory);
                        else store_instruction(opcode, processor, memory);
                    } break;
                    case (0b100): {
                        inc_dec_instruction(opcode, processor, memory, 1, "INR");
                    } break;
                    case (0b101): {
                        inc_dec_instruction(opcode, processor, memory, -1, "DCR");
                    } break;
                    case (0b111): {
                        byte subcode = opcode & 0b00111111;
                        if (subcode == 0b00111111)
                            complement_carry(processor);
                        else if ((subcode >> 3) == 0b101) {
                            complement_accumulator(processor);
                        }
                    } break;
                    default: {
                        std::stringstream err;
                        err << "The program encountered the following illegal instruction: " << opcode << "\n";
                        throw std::runtime_error(err.str());
                    } break;
                };
            } break;

            // 0b01 describes all MOV instructions
            case 1: {
                move_instruction(opcode, processor, memory);
            } break;

            // 0b10
            case 2: {
                byte reg_code = (opcode & 0b00000111);
                byte& reg = processor.get_register_by_code(memory, reg_code);
                switch ((opcode & 0b00111000) >> 3) {
                    case (0b000): {
                        add_sub_instruction(opcode, processor, memory, reg, "ADD");
                    } break;
                    case (0b001): {

                    } break;
                    case (0b010): {
                        add_sub_instruction(opcode, processor, memory, -reg, "SUB");
                    } break;
                    case (0b011): {

                    } break;
                    case (0b100): {

                    } break;
                    case (0b101): {

                    } break;
                    case (0b110): {

                    } break;
                    case (0b111): {

                    } break;
                    default: {

                    } break;
                };
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

    void adjust_value(Processor& processor, MEMORY& memory, byte& reg, byte amount) {
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
    }

    void inc_dec_instruction(byte opcode, Processor& processor, MEMORY& memory, signed char amount, std::string instruction_name) {
        byte reg_code = ((opcode & 0b00111000) >> 3);
        byte& reg = processor.get_register_by_code(memory, reg_code);

        test_instruction(processor, memory, reg, amount);

        std::clog << instruction_name << " " << processor.get_register_name_by_code(reg_code) << "\n";
    }

    void add_sub_instruction(byte opcode, Processor& processor, MEMORY& memory, signed char amount, std::string instruction_name) {
        test_instruction(processor, memory, processor.registers.A, amount);
        
        std::clog << instruction_name << " " << processor.get_register_name_by_code((opcode & 0b00000111)) << "\n";
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

    void load_instruction(byte opcode, Processor& processor, MEMORY& memory) {
        char loc = 'D';
        word adr = (word)(((processor.registers.D & 0xF) << 8) | processor.registers.E);

        if ((opcode & 0b00010000) >> 4 == 0) {
            adr = (word)(((processor.registers.B & 0xF) << 8) | processor.registers.C);
            loc = 'B';
        }
        processor.registers.A = memory.data[adr];
        std::clog << "LDAX " << loc << "\n";
    }

    void store_instruction(byte opcode, Processor& processor, MEMORY& memory) {
        char loc = 'D';
        word adr = (word)(((processor.registers.D & 0xF) << 8) | processor.registers.E);
        
        if ((opcode & 0b00010000) >> 4 == 0) {
            adr = (word)(((processor.registers.B & 0xF) << 8) | processor.registers.C);
            loc = 'B';
        }
        memory.data[adr] = processor.registers.A;
        std::clog << "STAX " << loc << "\n";
        //std::clog << "Contents of memory location " << adr << ": " << (int)memory.data[adr] << "\n";
    }

    void complement_accumulator(Processor& processor) {
        processor.registers.A = ~processor.registers.A;
        std::clog << "CMA\n";
    }

    void complement_carry(Processor& processor) {
        processor.flags.CF = ~processor.flags.CF;
        std::clog << "CMC\n";
    }
}