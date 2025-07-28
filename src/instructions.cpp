#include "instructions.h"

namespace instructions {
    std::function<void(little_byte, Processor&, MEMORY&)> instruction_table[256] = {nullptr};

    void execute_instruction(little_byte opcode, Processor& processor, MEMORY& memory) {
        if (auto& handler = instruction_table[opcode])
            handler(opcode, processor, memory);
        else {
            std::stringstream err;
            err << "The program encountered the following illegal instruction: " << (int)opcode << "\nThe program should never have reached this point.\nPlease report this error to the current maintainer.\n";
            throw std::runtime_error(err.str());
        }

        if (processor.DEBUG_MODE) {
            processor.log_registers(memory);
            processor.log_flags();
        }
        //processor.increase_counter();
    }

    void init_instruction_table() {
        // Lambda function to retrieve the reference of a register, depending on the value of opcode.
        auto get_reg = [](little_byte opcode, Processor& processor, MEMORY& memory) -> little_byte& {
            return processor.get_register_by_code(memory, opcode & 0b00000111);
        };

        instruction_table[NOP] = INSTRUCTION_PARAMS { std::clog << "NOP\n"; };
        instruction_table[HLT] = INSTRUCTION_PARAMS { processor.halt(); };

        for (little_byte opcode: mov_opcodes)
            instruction_table[opcode] = INSTRUCTION_PARAMS { move_instruction(opcode, processor, memory); };

        for (little_byte opcode: add_opcodes)
            instruction_table[opcode] = INSTRUCTION_PARAMS { add_instruction(opcode, processor, memory, get_reg(opcode, processor, memory), "ADD"); };

        for (little_byte opcode: adc_opcodes)
            instruction_table[opcode] = INSTRUCTION_PARAMS {
                little_byte& reg = get_reg(opcode, processor, memory);
                little_byte amount = reg + processor.flags.CF;
                processor.flags.CF = 0;
                add_instruction(opcode, processor, memory, amount, "ADC");
            };

        for (little_byte opcode: sub_opcodes)
            instruction_table[opcode] = INSTRUCTION_PARAMS { sub_instruction(opcode, processor, memory, get_reg(opcode, processor, memory), "SUB"); };

        for (little_byte opcode: sbb_opcodes)
            instruction_table[opcode] = INSTRUCTION_PARAMS {
                little_byte& reg = get_reg(opcode, processor, memory);
                little_byte amount = reg + processor.flags.CF;
                processor.flags.CF = 0;
                sub_instruction(opcode, processor, memory, reg, "SBB");
            };

        for (little_byte opcode: ana_opcodes)
            instruction_table[opcode] = INSTRUCTION_PARAMS { logical_and_instruction(opcode, processor, memory, get_reg(opcode, processor, memory)); };

        for (little_byte opcode: xra_opcodes)
            instruction_table[opcode] = INSTRUCTION_PARAMS { logical_xor_instruction(opcode, processor, memory, get_reg(opcode, processor, memory)); };

        for (little_byte opcode: ora_opcodes)
            instruction_table[opcode] = INSTRUCTION_PARAMS { logical_or_instruction(opcode, processor, memory, get_reg(opcode, processor, memory)); };

        for (little_byte opcode: cmp_opcodes)
            instruction_table[opcode] = INSTRUCTION_PARAMS { compare_instruction(opcode, processor, memory, get_reg(opcode, processor, memory)); };

        for (little_byte opcode: inr_opcodes)
            instruction_table[opcode] = INSTRUCTION_PARAMS { inc_dec_instruction(opcode, processor, memory, 1, "INR"); };

        for (little_byte opcode: dcr_opcodes)
            instruction_table[opcode] = INSTRUCTION_PARAMS { inc_dec_instruction(opcode, processor, memory, -1, "DCR"); };

        for (little_byte opcode: ldax_opcodes)
            instruction_table[opcode] = INSTRUCTION_PARAMS { load_instruction(opcode, processor, memory); };

        for (little_byte opcode: stax_opcodes)
            instruction_table[opcode] = INSTRUCTION_PARAMS { store_instruction(opcode, processor, memory); };

        for (little_byte opcode: push_opcodes)
            instruction_table[opcode] = INSTRUCTION_PARAMS { push_instruction(opcode, processor, memory); };

        for (little_byte opcode: pop_opcodes)
            instruction_table[opcode] = INSTRUCTION_PARAMS { pop_instruction(opcode, processor, memory); };

        // Carry Instructions
        instruction_table[CMC] = INSTRUCTION_PARAMS { complement_carry(processor); };
        instruction_table[STC] = INSTRUCTION_PARAMS { set_carry(processor); };
        instruction_table[CMA] = INSTRUCTION_PARAMS { complement_accumulator(processor); };
        
        // Accumulator Rotation Instructions
        instruction_table[RLC] = INSTRUCTION_PARAMS { rotate_accumulator_left(processor); };
        instruction_table[RRC] = INSTRUCTION_PARAMS { rotate_accumulator_right(processor); };
        instruction_table[RAL] = INSTRUCTION_PARAMS { rotate_accumulator_left_carry(processor); };
        instruction_table[RAR] = INSTRUCTION_PARAMS { rotate_accumulator_right_carry(processor); };
    }

    void adjust_value(Processor& processor, MEMORY& memory, little_byte& reg, little_byte amount) {
        processor.flags.AC = (((reg & 0x0F) + amount) > 0x0F);

        reg += amount;
        processor.flags.S = (reg >> 7) & 1;
        processor.flags.Z = (reg == 0) ? 1 : 0;

        little_byte v = reg;
        bool parity = true;
        while (v) {
            parity = !parity;
            v = v & (v - 1);
        }
        processor.flags.P = parity;
    }

    void inc_dec_instruction(little_byte opcode, Processor& processor, MEMORY& memory, signed char amount, std::string instruction_name) {
        little_byte reg_code = ((opcode & 0b00111000) >> 3);
        little_byte& reg = processor.get_register_by_code(memory, reg_code);

        adjust_value(processor, memory, reg, amount);

        std::clog << instruction_name << " " << processor.get_register_name_by_code(reg_code) << "\n";
    }

    void add_instruction(little_byte opcode, Processor& processor, MEMORY& memory, signed char amount, std::string instruction_name) {
        adjust_value(processor, memory, processor.registers.A, amount);
        
        std::clog << instruction_name << " " << processor.get_register_name_by_code((opcode & 0b00000111)) << "\n";
    }

    void sub_instruction(little_byte opcode, Processor& processor, MEMORY& memory, little_byte amount, std::string instruction_name) {
        amount = ~amount + 1;
        adjust_value(processor, memory, processor.registers.A, amount);

        //processor.flags.CF = ~processor.flags.CF;

        std::clog << instruction_name << " " << processor.get_register_name_by_code((opcode & 0b00000111)) << "\n";
    }

    void move_instruction(little_byte opcode, Processor& processor, MEMORY& memory) {
        little_byte src_code = opcode & 0x07;
        little_byte dst_code = (opcode & 0x38) >> 3;
        little_byte& src = processor.get_register_by_code(memory, src_code);
        little_byte& dst = processor.get_register_by_code(memory, dst_code);

        if (!((src_code == dst_code) && (src_code == 0b110))) {
            dst = src;
        } else {
            processor.halt();
        }

        std::clog << "MOV " << processor.get_register_name_by_code(dst_code) << "," << processor.get_register_name_by_code(src_code) << "\n";
    }

    void logical_and_instruction(little_byte opcode, Processor& processor, MEMORY& memory, little_byte value) {
        processor.registers.A = processor.registers.A & value;
        processor.flags.CF = 0;

        std::clog << "ANA " << processor.get_register_name_by_code((opcode & 0b00000111)) << "\n";
    }

    void logical_xor_instruction(little_byte opcode, Processor& processor, MEMORY& memory, little_byte value) {
        processor.registers.A = processor.registers.A ^ value;

        std::clog << "XRA " << processor.get_register_name_by_code((opcode & 0b00000111)) << "\n";
    }

    void logical_or_instruction(little_byte opcode, Processor& processor, MEMORY& memory, little_byte value) {
        processor.registers.A = processor.registers.A || value;

        std::clog << "ORA " << processor.get_register_name_by_code((opcode & 0b00000111)) << "\n";
    }

    void compare_instruction(little_byte opcode, Processor& processor, MEMORY& memory, little_byte value) {
        sub_instruction(opcode, processor, memory, value, "CMP");
    }

    void load_instruction(little_byte opcode, Processor& processor, MEMORY& memory) {
        char loc = 'D';
        word adr = (word)(((processor.registers.D & 0xF) << 8) | processor.registers.E);

        if ((opcode & 0b00010000) >> 4 == 0) {
            adr = (word)(((processor.registers.B & 0xF) << 8) | processor.registers.C);
            loc = 'B';
        }
        processor.registers.A = memory.data[adr];
        std::clog << "LDAX " << loc << "\n";
    }

    void store_instruction(little_byte opcode, Processor& processor, MEMORY& memory) {
        char loc = 'D';
        word adr = (word)(((processor.registers.D & 0xF) << 8) | processor.registers.E);
        
        if ((opcode & 0b00010000) >> 4 == 0) {
            adr = (word)(((processor.registers.B & 0xF) << 8) | processor.registers.C);
            loc = 'B';
        }
        memory.data[adr] = processor.registers.A;
        std::clog << "STAX " << loc << "\n";
    }

    void complement_accumulator(Processor& processor) {
        processor.registers.A = ~processor.registers.A;
        std::clog << "CMA\n";
    }

    void complement_carry(Processor& processor) {
        processor.flags.CF = ~processor.flags.CF;
        std::clog << "CMC\n";
    }

    void set_carry(Processor& processor) {
        processor.flags.CF = 1;
        std::clog << "CMA\n";
    }

    void rotate_accumulator_left(Processor& processor) {
        processor.flags.CF = processor.registers.A >> 7;
        processor.registers.A = std::rotl(processor.registers.A, 1);

        std::clog << "RLC\n";
    }

    void rotate_accumulator_right(Processor& processor) {
        processor.flags.CF = processor.registers.A & 1;
        processor.registers.A = std::rotr(processor.registers.A, 1);

        std::clog << "RRC\n";
    }

    void rotate_accumulator_left_carry(Processor& processor) {
        processor.registers.A = std::rotl(processor.registers.A, 1);

        bool temp = processor.flags.CF;
        processor.flags.CF = processor.registers.A & 1;
        processor.registers.A = (processor.registers.A & ~1) | (temp ? 1 : 0);

        std::clog << "RAL\n";
    }

    void rotate_accumulator_right_carry(Processor& processor) {
        processor.registers.A = std::rotr(processor.registers.A, 1);

        bool temp = processor.flags.CF;
        processor.flags.CF = (processor.registers.A >> 7) & 1;
        processor.registers.A = (processor.registers.A & 0b01111111) | (temp ? 0b10000000 : 0);

        std::clog << "RAR\n";
    }

    void push_instruction(little_byte opcode, Processor& processor, MEMORY& memory) {
        little_byte code = (opcode & 0b00110000) >> 4;
        std::pair<little_byte&, little_byte&> values = processor.get_register_pair(code);

        processor.SP -= 1;
        memory.data[processor.SP] = values.first;
        processor.SP -= 1;
        memory.data[processor.SP] = values.second;

        std::string reg_name = "THIS VALUE SHOULD NOT BE SHOWN";
        switch (code) {
            case (0b00):
                reg_name = "B";
                break;
            case (0b01):
                reg_name = "D";
                break;
            case (0b10):
                reg_name = "H";
                break;
            case (0b11):
                reg_name = "PSW";
                break;
        }

        std::clog << "PUSH " << reg_name << "\n";
    }

    void pop_instruction(little_byte opcode, Processor& processor, MEMORY& memory) {
        little_byte code = (opcode & 0b00110000) >> 4;
        std::pair<little_byte&, little_byte&> regs = processor.get_register_pair(code);

        regs.second = memory.data[processor.SP];
        processor.SP += 1;

        if (code == 0b11) {
            processor.flags.S = (memory.data[processor.SP] & 0b10000000) >> 7;
            processor.flags.Z = (memory.data[processor.SP] & 0b01000000) >> 6;
            processor.flags.AC = (memory.data[processor.SP] & 0b00010000) >> 4;
            processor.flags.P = (memory.data[processor.SP] & 0b00000100) >> 2;
            processor.flags.CF = (memory.data[processor.SP] & 0b00000001);
        } else regs.first = memory.data[processor.SP];

        processor.SP += 1;

        std::string reg_name = "THIS VALUE SHOULD NOT BE SHOWN";
        switch (code) {
            case (0b00):
                reg_name = "B";
                break;
            case (0b01):
                reg_name = "D";
                break;
            case (0b10):
                reg_name = "H";
                break;
            case (0b11):
                reg_name = "PSW";
                break;
        }

        std::clog << "POP " << reg_name << "\n";
    }
}