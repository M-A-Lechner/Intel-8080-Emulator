#include "instructions.h"

namespace instructions {
    void execute_instruction(byte opcode, Processor& processor, MEMORY& memory) {
        byte reg_code = (opcode & 0b00000111);
        byte& reg = processor.get_register_by_code(memory, reg_code);

        if (opcode == NOP) std::clog << "NOP\n";

        else if (opcode == HLT) processor.halt();

        else if (std::ranges::binary_search(mov_opcodes, opcode)) move_instruction(opcode, processor, memory);

        else if (std::ranges::binary_search(add_opcodes, opcode)) add_instruction(opcode, processor, memory, reg, "ADD");

        else if (std::ranges::binary_search(adc_opcodes, opcode)) {
            byte amount = reg + processor.flags.CF;
            processor.flags.CF = 0;
            add_instruction(opcode, processor, memory, amount, "ADC");
        }

        else if (std::ranges::binary_search(sub_opcodes, opcode)) sub_instruction(opcode, processor, memory, reg, "SUB");

        else if (std::ranges::binary_search(sbb_opcodes, opcode)) {
            byte amount = reg + processor.flags.CF;
            processor.flags.CF = 0;
            sub_instruction(opcode, processor, memory, reg, "SBB");
        }


        else if (std::ranges::binary_search(ana_opcodes, opcode)) logical_and_instruction(opcode, processor, memory, reg);

        else if (std::ranges::binary_search(xra_opcodes, opcode)) logical_xor_instruction(opcode, processor, memory, reg);

        else if (std::ranges::binary_search(ora_opcodes, opcode)) logical_or_instruction(opcode, processor, memory, reg);

        else if (std::ranges::binary_search(cmp_opcodes, opcode)) compare_instruction(opcode, processor, memory, reg);


        else if (std::ranges::binary_search(ldax_opcodes, opcode)) load_instruction(opcode, processor, memory);

        else if (std::ranges::binary_search(stax_opcodes, opcode)) store_instruction(opcode, processor, memory);


        else if (std::ranges::binary_search(inr_opcodes, opcode)) inc_dec_instruction(opcode, processor, memory, 1, "INR");

        else if (std::ranges::binary_search(dcr_opcodes, opcode)) inc_dec_instruction(opcode, processor, memory, -1, "DCR");

        else if (opcode == CMC) complement_carry(processor);

        else if (opcode == STC) set_carry(processor);

        else if (opcode == CMA) complement_accumulator(processor);

        else {
            std::stringstream err;
            err << "The program encountered the following illegal instruction: " << opcode << "\nThe program should never have reached this point.\nPlease report this error to the current maintainer.\n";
            throw std::runtime_error(err.str());
        }

        if (processor.DEBUG_MODE) {
            processor.log_registers(memory);
            processor.log_flags();
        }
        //processor.increase_counter();
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

        adjust_value(processor, memory, reg, amount);

        std::clog << instruction_name << " " << processor.get_register_name_by_code(reg_code) << "\n";
    }

    void add_instruction(byte opcode, Processor& processor, MEMORY& memory, signed char amount, std::string instruction_name) {
        adjust_value(processor, memory, processor.registers.A, amount);
        
        std::clog << instruction_name << " " << processor.get_register_name_by_code((opcode & 0b00000111)) << "\n";
    }

    void sub_instruction(byte opcode, Processor& processor, MEMORY& memory, byte amount, std::string instruction_name) {
        amount = ~amount + 1;
        adjust_value(processor, memory, processor.registers.A, amount);

        //processor.flags.CF = ~processor.flags.CF;

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

    void logical_and_instruction(byte opcode, Processor& processor, MEMORY& memory, byte value) {
        processor.registers.A = processor.registers.A & value;
        processor.flags.CF = 0;

        std::clog << "ANA " << processor.get_register_name_by_code((opcode & 0b00000111)) << "\n";
    }

    void logical_xor_instruction(byte opcode, Processor& processor, MEMORY& memory, byte value) {
        processor.registers.A = processor.registers.A ^ value;

        std::clog << "XRA " << processor.get_register_name_by_code((opcode & 0b00000111)) << "\n";
    }

    void logical_or_instruction(byte opcode, Processor& processor, MEMORY& memory, byte value) {
        processor.registers.A = processor.registers.A || value;

        std::clog << "ORA " << processor.get_register_name_by_code((opcode & 0b00000111)) << "\n";
    }

    void compare_instruction(byte opcode, Processor& processor, MEMORY& memory, byte value) {
        sub_instruction(opcode, processor, memory, value, "CMP");
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
}