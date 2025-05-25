#pragma once

#include <iostream>
#include <string>
#include <limits.h>

#include "definitions/datatypes.h"
#include "definitions/opcodes.h"


struct MEMORY {
    byte data[65535]; // 64KiB equivalent to 0xFFFF.
};

struct OUT_DATA {
    byte data[256];
};

struct IN_DATA {
    byte data[256];
};


struct Processor {
    struct Flags {
        flag S;
        flag Z;
        flag AC;
        flag CF;
        flag P;

        void reset();
    };

    struct Registers {
        byte A;
        byte B;
        byte C;
        byte D;
        byte E;
        
        byte H;
        byte L;

        void reset();
    };

    Registers registers;
    Flags flags;

    word SP;
    word PC;

    void reset();

    void execute(int cycles, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data);

    /**
     * Increases given register value by one.
     * Flags affected are: S, Z, AC, P
     */
    void increase_by_one(byte& reg);

    byte get_next_byte(MEMORY& memory);

    word get_next_word(MEMORY& memory);

    void log_instruction(std::string instruction_name);
};