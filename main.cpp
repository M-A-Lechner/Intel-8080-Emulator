#include <iostream>
#include <string>

using namespace std;

#define flag bool
#define byte unsigned char
#define word unsigned short
#define LDA 0x3A
#define STA 0x32
#define JMP 0xC3
#define OUT 0xD3
#define INR_A 0x3C


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
  //Flags
  flag S;
  flag Z;
  flag AC;
  flag CA;
  flag P;

  byte A, B, C, D, H, L; //Registers to write data to.

  word SP;
  word PC;

  void reset() {
    PC = 0x0000;
    SP = 0x0000;
    A = 0x0000;
  }
  
  void execute(int cycles, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data) {
    unsigned char currByte;
    while (cycles > 0) {
      currByte = memory.data[PC];
      switch(currByte) {
        case LDA: {
            clog << "LDA" << endl;
            A = memory.data[getWordFromAddress(memory)];
            cout << "Accumulator: " << hex << (int)A << endl;
            PC += 1;
          } break;
        case STA: {
            clog << "STA" << endl;
            word adr = getWordFromAddress(memory);
            memory.data[adr] = A;
            cout << "Memory written: " << hex << (int)memory.data[adr] << endl;
            PC += 1;
          } break;
        case JMP: {
            clog << "JMP" << endl;
            word adr = getWordFromAddress(memory);
            PC = adr;
          } break;
        case OUT: {
            clog << "OUT" << endl;
            byte adr = getByteFromAddress(memory);
            out_data.data[adr] = A;
            PC += 1;
          } break;
        case INR_A: {
            clog << "INR_A" << endl;
            A += 1;
            PC += 1;
          } break;
        default:
          {
            cerr << "Unknown value." << endl;
            return;
          }
      }
      cycles -= 1;
    }
  }

  byte getByteFromAddress(MEMORY& memory) {
    PC += 1;
    byte adr = memory.data[PC];
    cout << "Byte retrieved: " << hex << (int)adr << endl;
    return adr;
  }

  word getWordFromAddress(MEMORY& memory) {
    PC += 1;
    byte lb = memory.data[PC];
    PC += 1;
    byte hb = memory.data[PC];
    word adr = (lb << 8) | hb;
    cout << "Word retrieved: " << hex << (int)adr << endl;
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

  memory.data[0] = LDA;  //Load first character into A register.
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
