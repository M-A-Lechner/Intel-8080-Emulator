#include <iostream>
#include <string>
#include <limits.h>

using namespace std;

#define flag bool
#define byte unsigned char
#define word unsigned short

#define STAX_B 0x02
#define STAX_D 0x12
#define STA 0x32
#define STC 0x37
#define LDA 0x3A
#define INR_A 0x3C
#define DCR_A 0x3D
#define CMC 0x3F
#define MOV_B_A 0x47
#define MOV_M_A 0x77
#define MOV_A_B 0x78
#define MOV_A_M 0x7E
#define JMP 0xC3
#define OUT 0xD3


struct MEMORY {
    byte data[65535]; // 64KiB equivalent to 0xFFFF.
};

struct OUT_DATA {
    byte data[256];
};

struct IN_DATA {
    byte data[256];
};

struct CPU {
    struct Flags {
        flag S;
        flag Z;
        flag AC;
        flag CF;
        flag P;

        void reset() {
            S = 0;
            Z = 0;
            AC = 0;
            CF = 0;
            P = 0;
        }
    };

    struct Registers {
        byte A;
        byte B;
        byte C;
        byte D;
        
        byte H;
        byte L;

        void reset() {
            A = 0x00;
            B = 0x00;
            C = 0x00;
            D = 0x00;

            H = 0x00;
            L = 0x00;
        }
    };

    Registers registers;
    Flags flags;

    word SP;
    word PC;

    void reset() {
        PC = 0x0000;
        SP = 0x0000;

        registers.reset();
        flags.reset();
    }

    void execute(int cycles, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data) {
        unsigned char currByte;
        unsigned char counter_increase = 0;

        while (cycles > 0) {
            currByte = memory.data[PC];
            switch(currByte) {      //TODO: move all instructions into own namespace, and as unique functions
                case STAX_B: {
                    clog << "STAX B\n";
                    word adr = (word)(((registers.B & 0xF) << 8) | registers.C);
                    memory.data[adr] = registers.A;
                    counter_increase = 3;
                } break;
                case STAX_D: {
                    clog << "STAX D\n";
                    word adr = (word)(((registers.D & 0xF) << 8) | registers.E);
                    memory.data[adr] = registers.A;
                    counter_increase = 3;
                } break;
                case LDA: {
                    clog << "LDA\n";
                    registers.A = memory.data[get_next_word(memory)];
                    cout << "\tAccumulator: " << hex << (int)registers.A << "\n";
                    counter_increase = 1;
                } break;
                case STA: {
                    clog << "STA\n";
                    byte adr = get_next_word(memory);
                    memory.data[adr] = registers.A;
                    cout << "\tMemory written: " << hex << (int)memory.data[adr] << "\n";
                    counter_increase = 1;
                } break;
                case JMP: {
                    clog << "JMP\n";
                    word adr = get_next_word(memory);
                    PC = adr;
                } break;
                case OUT: {
                    clog << "OUT\n";
                    byte adr = get_next_byte(memory);
                    out_data.data[adr] = registers.A;
                    counter_increase = 1;
                } break;
                case INR_A: {
                    clog << "INR_A\n";
                    increase_by_one(registers.A);
                    counter_increase = 1;
                } break;
                case DCR_A: {
                    clog << "DCR_A\n";
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
                    clog << "MOV_B_A\n";
                    registers.B = registers.A;
                    counter_increase = 1;
                } break;
                case MOV_M_A: {
                    clog << "MOV_M_A\n";
                    byte adr = get_next_word();
                    memory.data[adr] = registers.A;
                    counter_increase = 1;
                } break;
                case MOV_A_B: {
                    clog << "MOV_A_B\n";
                    registers.A = registers.B;
                    counter_increase = 1;
                } break;
                case MOV_A_M: {
                    clog << "MOV_A_M\n";
                    byte adr = get_next_word();
                    registers.A = memory.data[adr];
                    counter_increase = 1;
                } break;
                case STC: {
                    clog << "STC\n";
                    flags.CF = 1;
                    counter_increase = 1;
                } break;
                case CMC: {
                    clog << "CMC\n";
                    flags.CF = !flags.CF;
                    counter_increase = 1;
                } break;
                default: {
                    cerr << (int)currByte << " is an unknown instruction." << endl;
                    return;
                }
            }
            PC += counter_increase;
            counter_increase = 0;
            cycles -= 1;
        }
    }

    /**
     * Increases given register value by one.
     * Flags affected are: S, Z, AC, P
     */
    void increase_by_one(byte& reg) {
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

    byte get_next_byte(MEMORY& memory) {
        PC += 1;
        byte adr = memory.data[PC];
        cout << "\tByte retrieved: " << hex << (int)adr << endl;
        return adr;
    }

    word get_next_word(MEMORY& memory) {
        byte lb = memory.data[PC+1];
        byte hb = memory.data[PC+2];
        word adr = (lb << 8) | hb;
        cout << "\tWord retrieved: " << hex << (int)adr << endl;
        PC += 2;
        return adr;
    }
};

void printOutData(OUT_DATA& out_data) {
    unsigned char character = out_data.data[0];
    unsigned char index = 1;
    while(character != 0x03 && index != 0) {
        cout << character;
        character = out_data.data[index];
        index += 1; // should have check if index hits 256!
    };
    if (index == 0) {
        cerr << endl << "Output error: Char limit reached!";
    }
    cout << endl << endl << "End of output!" << endl;
}

int main (int argc, char *argv[]) {
    CPU cpu;
    cpu.reset();
    MEMORY memory;

    memory.data[0] = LDA;  //Load first character into A register. <-- This should be done with LDAX instead of altering the source code at runtime!
    memory.data[1] = 0x05;
    memory.data[2] = 0x00; //Address here is variable.
    memory.data[3] = OUT;  //Output character in A register.
    memory.data[4] = 0x00; //Set index of output. Address here is variable.
    memory.data[5] = LDA;  //Load value at 0x0002 into register A.
    memory.data[6] = 0x00;
    memory.data[7] = 0x02;
    memory.data[8] = INR_A; //increment A by one.
    memory.data[9] = STA;   //store current A value at 0x0002.
    memory.data[10] = 0x00;
    memory.data[11] = 0x02;
    memory.data[12] = STA;  //store current A value at 0x0004.
    memory.data[13] = 0x00;
    memory.data[14] = 0x04;
    memory.data[15] = JMP;  //Jump to start of programme 0x0000.
    memory.data[16] = 0x00;
    memory.data[16] = 0x00;


    memory.data[0x0500] = 0x48; //H
    memory.data[0x0501] = 0x45; //E
    memory.data[0x0502] = 0x4C; //L
    memory.data[0x0503] = 0x4C; //L
    memory.data[0x0504] = 0x4F; //O
    memory.data[0x0505] = 0x20; //<space>
    memory.data[0x0506] = 0x57; //W
    memory.data[0x0507] = 0x4F; //O
    memory.data[0x0508] = 0x52; //R
    memory.data[0x0509] = 0x4C; //L
    memory.data[0x050A] = 0x44; //D
    memory.data[0x050B] = 0x03; //<End of text>


    IN_DATA in_data;
    OUT_DATA out_data;

    cpu.execute(12*7, memory, in_data, out_data);
    printOutData(out_data);

    return 0;
}
