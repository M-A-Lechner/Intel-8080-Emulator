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
        switch(currByte) {      //TODO: move all instructions into own namespace, and as unique functions
            case STAX_B: {
                log_instruction("STAX B");
                word adr = (word)(((registers.B & 0xF) << 8) | registers.C);
                memory.data[adr] = registers.A;
                counter_increase = 3;
            } break;
            case STAX_D: {
                log_instruction("STAX D");
                word adr = (word)(((registers.D & 0xF) << 8) | registers.E);
                memory.data[adr] = registers.A;
                counter_increase = 3;
            } break;
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
            case INR_A: {
                log_instruction("INR_A");
                increase_by_one(registers.A);
                counter_increase = 1;
            } break;
            case DCR_A: {
                log_instruction("DCR_A");
                if (flags.CF == 1 && registers.A == 0x00) {
                    registers.A == UCHAR_MAX;
                    flags.CF = 0;
                } else {
                    registers.A -= 1;
                }
                flags.S = (registers.A >> 7) & 1;
                counter_increase = 1;
            } break;
            case MOV_B_A: {
                log_instruction("MOV_B_A");
                registers.B = registers.A;
                counter_increase = 1;
            } break;
            case MOV_M_A: {
                log_instruction("MOV_M_A\n");
                byte adr = get_next_word(memory);
                memory.data[adr] = registers.A;
                counter_increase = 1;
            } break;
            case MOV_A_B: {
                log_instruction("MOV_A_B");
                registers.A = registers.B;
                counter_increase = 1;
            } break;
            case MOV_A_M: {
                log_instruction("MOV_A_M");
                byte adr = get_next_word(memory);
                registers.A = memory.data[adr];
                counter_increase = 1;
            } break;
            case STC: {
                log_instruction("STC");
                flags.CF = 1;
                counter_increase = 1;
            } break;
            case CMC: {
                log_instruction("CMC");
                flags.CF = !flags.CF;
                counter_increase = 1;
            } break;
            default: {
                std::cerr << (int)currByte << " is an unknown instruction.\n";
                return;
            }
        }
        PC += counter_increase;
        counter_increase = 0;
        cycles -= 1;
    }
}

void Processor::increase_by_one(byte& reg) {
    flags.AC = (((reg & 0x0F) + 1) > 0x0F);

    reg += 1;
    flags.S = (reg >> 7) & 1;
    flags.Z = (reg == 0) ? 1 : 0;

    unsigned char v = reg;
    bool parity = true;
    while (v) {
        parity = !parity;
        v = v & (v - 1);
    }
    flags.P = parity;
}

byte Processor::get_next_byte(MEMORY& memory) {
    PC += 1;
    byte adr = memory.data[PC];
    std::cout << "\tByte retrieved: " << std::hex << (int)adr << "\n";
    return adr;
}

word Processor::get_next_word(MEMORY& memory) {
    byte lb = memory.data[PC+1];
    byte hb = memory.data[PC+2];
    word adr = (lb << 8) | hb;
    std::cout << "\tWord retrieved: " << std::hex << (int)adr << "\n";
    PC += 2;
    return adr;
}

void Processor::log_instruction(std::string instruction_name) {
    std::clog << instruction_name << "\n";
}
