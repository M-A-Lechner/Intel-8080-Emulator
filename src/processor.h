#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <limits.h>
#include <memory>
#include <array>
#include <utility>

#include "definitions/datatypes.h"
#include "definitions/opcodes.h"
#include "instructions.h"
#include "memory.h"

struct OUT_DATA {
    std::array<little_byte, 256> data;

    void reset();
};

struct IN_DATA {
    std::array<little_byte, 256> data;

    void reset();
};


struct Processor {
    struct Flags {
        flag S;
        flag Z;
        flag AC;
        flag CF;
        flag P;
        little_byte flags;

        void reset();
    };

    struct Registers {
        little_byte A;
        little_byte B;
        little_byte C;
        little_byte D;
        little_byte E;
        
        little_byte H;
        little_byte L;

        void reset();
    };

    bool DEBUG_MODE = false;

    Registers registers;
    Flags flags;

    word SP;
    word PC;
    flag HALT;

    void reset();

    void execute(MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data);

    /**
     * Increases given register value by one.
     * Flags affected are: S, Z, AC, P
     */
    void increase_by_one(little_byte& reg);

    little_byte get_next_byte(MEMORY& memory);

    word get_next_word(MEMORY& memory);

    char get_register_name_by_code(little_byte code);

    little_byte& get_register_by_code(MEMORY& memory, little_byte code);

    little_byte& get_byte_at_ref(MEMORY& memory, little_byte adr);

    word get_register_value_pair(little_byte code);

    std::pair<little_byte&, little_byte&> get_register_pair(little_byte code);

    void log_registers(MEMORY& memory);

    void log_flags();

    void increase_counter(little_byte amount = 1);

    void halt();
};