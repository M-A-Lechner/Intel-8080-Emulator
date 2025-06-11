#pragma once

#include <stdexcept>
#include <sstream>
#include <string>
#include <memory>

#include "definitions/datatypes.h"
#include "processor.h"

struct MEMORY;
struct Processor;

namespace instructions {
    extern void execute_instruction(byte opcode, Processor& processor, MEMORY& memory);

    void adjust_value(Processor& processor, MEMORY& memory, byte& reg, byte amount);

    void inc_dec_instruction(byte opcode, Processor& processor, MEMORY& memory, signed char amount, std::string instruction_name);
    void add_sub_instruction(byte opcode, Processor& processor, MEMORY& memory, signed char amount, std::string instruction_name);
    void move_instruction(byte opcode, Processor& processor, MEMORY& memory);

    void load_instruction(byte opcode, Processor& processor, MEMORY& memory);
    void store_instruction(byte opcode, Processor& processor, MEMORY& memory);

    void complement_accumulator(Processor& processor);
    void complement_carry(Processor& processor);
}