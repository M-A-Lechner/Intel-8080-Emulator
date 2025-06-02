#pragma once

#include <stdexcept>
#include <sstream>
#include <memory>

#include "definitions/datatypes.h"
#include "processor.h"

struct MEMORY;
struct Processor;

namespace instructions {
    extern void execute_instruction(byte opcode, Processor& processor, MEMORY& memory);

    void increase_instruction(byte opcode, Processor& processor, MEMORY& memory);
    void move_instruction(byte opcode, Processor& processor, MEMORY& memory);
}