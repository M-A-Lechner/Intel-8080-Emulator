#include <iostream>
#include <string>
#include <limits.h>

#include "definitions/datatypes.h"
#include "definitions/opcodes.h"
#include "processor.h"



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
    Processor cpu;
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
