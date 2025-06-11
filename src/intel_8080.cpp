#include "processor.h"


void Processor::Flags::reset() {
    S = 0;
    Z = 0;
    AC = 0;
    CF = 0;
    P = 0;
}

void Processor::Registers::reset() {
    A = 0x00;
    B = 0x00;
    C = 0x00;
    D = 0x00;
    E = 0x00;

    H = 0x00;
    L = 0x00;
}

void Processor::reset() {
    PC = 0x0000;
    SP = 0x0000;

    registers.reset();
    flags.reset();
}

void Processor::execute(int cycles, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data) {
    unsigned char currByte;
    unsigned char counter_increase = 0;

    while (cycles > 0) {
        currByte = memory.data[PC];
        instructions::execute_instruction(currByte, *this, memory);
        /*
        switch(currByte) {      //TODO: move all instructions into own namespace, and as unique functions
            case LDA: {
                log_instruction("LDA");
                registers.A = memory.data[get_next_word(memory)];
                std::cout << "\tAccumulator: " << std::hex << (int)registers.A << "\n";
                counter_increase = 1;
            } break;
            case STA: {
                log_instruction("STA");
                byte adr = get_next_word(memory);
                memory.data[adr] = registers.A;
                std::cout << "\tMemory written: " << std::hex << (int)memory.data[adr] << "\n";
                counter_increase = 1;
            } break;
            case JMP: {
                log_instruction("JMP");
                word adr = get_next_word(memory);
                PC = adr;
            } break;
            case OUT: {
                log_instruction("OUT");
                byte adr = get_next_byte(memory);
                out_data.data[adr] = registers.A;
                counter_increase = 1;
            } break;
            case STC: {
                log_instruction("STC");
                flags.CF = 1;
                counter_increase = 1;
            } break;
            default: {
                std::cerr << (int)currByte << " is an unknown instruction.\n";
                return;
            }
        }
        */
        PC += counter_increase;
        counter_increase = 0;
        cycles -= 1;
    }
}

byte Processor::get_next_byte(MEMORY& memory) {
    PC += 1;
    byte adr = memory.data[PC];
    std::cout << "\tByte retrieved: " << std::hex << (int)adr << "\n";
    return adr;
}

byte& Processor::get_byte_at_ref(MEMORY& memory, byte adr) {
    return memory.data[adr];
}

word Processor::get_next_word(MEMORY& memory) {
    byte lb = memory.data[PC+1];
    byte hb = memory.data[PC+2];
    word adr = (lb << 8) | hb;
    std::cout << "\tWord retrieved: " << std::hex << (int)adr << "\n";
    PC += 2;
    return adr;
}

char Processor::get_register_name_by_code(byte code) {
    switch (code) {
        case 0b0000: {
            return 'B';
        } break;
        case 0b0001: {
            return 'C';
        } break;
        case 0b0010: {
            return 'D';
        } break;
        case 0b0011: {
            return 'E';
        } break;
        case 0b0100: {
            return 'H';
        } break;
        case 0b0101: {
            return 'L';
        } break;
        case 0b0110: {
            return 'M';
        } break;
        case 0b0111: {
            return 'A';
        } break;
        default: {
            std::stringstream err;
            err << (int)code << " is an unkown register code.";
            throw std::runtime_error(err.str());
        }
    }
}

byte& Processor::get_register_by_code(MEMORY& memory, byte code) {
    switch (code) {
        case 0b0000: {
            return registers.B;
        } break;
        case 0b0001: {
            return registers.C;
        } break;
        case 0b0010: {
            return registers.D;
        } break;
        case 0b0011: {
            return registers.E;
        } break;
        case 0b0100: {
            return registers.H;
        } break;
        case 0b0101: {
            return registers.L;
        } break;
        case 0b0110: {
            return get_byte_at_ref(memory, (registers.L << 8) | registers.H);
        } break;
        case 0b0111: {
            return registers.A;
        } break;
        default: {
            std::stringstream err;
            err << (int)code << " is an unkown register code.";
            throw std::runtime_error(err.str());
        }
    }
}

void Processor::log_registers(MEMORY& memory) {
    std::clog << "Current register values:\n";
    std::clog << "A: " << std::hex << (int)registers.A << "\n";
    std::clog << "B: " << std::hex << (int)registers.B << "\n";
    std::clog << "C: " << std::hex << (int)registers.C << "\n";
    std::clog << "D: " << std::hex << (int)registers.D << "\n";
    std::clog << "E: " << std::hex << (int)registers.E << "\n";
    std::clog << "H: " << std::hex << (int)registers.H << "\n";
    std::clog << "L: " << std::hex << (int)registers.L << "\n";
    std::clog << "Memory: " << std::hex << (int)get_byte_at_ref(memory, (registers.L << 8) | registers.H) << "\n\n";
}

void Processor::log_flags() {
    std::clog << "Current flag values:\n";
    std::clog << "S: " << (int)flags.S << "\n";
    std::clog << "Z: " << (int)flags.Z << "\n";
    std::clog << "AC: " << (int)flags.AC << "\n";
    std::clog << "CF: " << (int)flags.CF << "\n";
    std::clog << "P: " << (int)flags.P << "\n\n";
}

void Processor::increase_counter(byte amount) {
    PC += amount;
}

void Processor::halt() {
    std::clog << "Processor halted.\n";
}
