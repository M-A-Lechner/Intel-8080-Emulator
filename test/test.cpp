// compound assignment operators
#include <iostream>
#include <cassert>
#include <functional>


#include "../src/processor.h"
#include "../src/definitions/opcodes.h"
#include "../src/definitions/datatypes.h"
#include "../src/memory.h"
using namespace std;


#define assertm(exp, msg) assert((void(msg), exp))



struct Processor;
struct MEMORY;
struct IN_DATA;
struct OUT_DATA;

void reset_all_memory(MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data);

void test_ana_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data);
void test_add_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data);
void test_sub_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data);
void test_ldax_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data);

void test_rlc_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data);
void test_rrc_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data);
void test_ral_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data);
void test_rar_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data);


int main () {
    instructions::init_instruction_table();
    Processor cpu;
    cpu.reset();
    IN_DATA in_data;
    OUT_DATA out_data;
    MEMORY memory;

    test_ana_instruction(cpu, memory, in_data, out_data);
    test_add_instruction(cpu, memory, in_data, out_data);
    test_sub_instruction(cpu, memory, in_data, out_data);
    test_ldax_instruction(cpu, memory, in_data, out_data);

    test_rlc_instruction(cpu, memory, in_data, out_data);
    test_rrc_instruction(cpu, memory, in_data, out_data);
    test_ral_instruction(cpu, memory, in_data, out_data);
    test_rar_instruction(cpu, memory, in_data, out_data);
}

void reset_all_memory(MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data) {
    memory.reset();
    in_data.reset();
    out_data.reset();
}

void test_ana_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data) {
    reset_all_memory(memory, in_data, out_data);

    processor.reset();

    processor.registers.A = 0b00110011;
    processor.registers.B = 0b00001111;

    memory.data[0] = ANA_B;
    memory.data[1] = HLT;

    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.A == 0b00000011, "Test instruction ANA successful.");
}

void test_add_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data) {
    reset_all_memory(memory, in_data, out_data);

    processor.reset();

    processor.registers.A = 0b00001000;

    memory.data[0] = ADD_A;
    memory.data[1] = HLT;

    processor.execute(memory, in_data, out_data);

    std::cout << (int)processor.registers.A << "\n";

    assertm(processor.registers.A == 0b00010000, "Test instruction ADD successful.");
}

void test_sub_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data) {
    reset_all_memory(memory, in_data, out_data);

    processor.reset();

    processor.registers.A = 0b00111110;

    memory.data[0] = SUB_A;
    memory.data[1] = HLT;

    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.A == 0b00000000, "Test instruction SUB successful: Value of A register.");
    assertm(processor.flags.CF == 0, "Test instruction SUB successful: Value of Carry flag.");
}

void test_ldax_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();

    processor.registers.B = 0b00000000;
    processor.registers.C = 0b11111111;

    little_byte target_val = 0b00111100;
    word adr = (word)(((processor.registers.B & 0xF) << 8) | processor.registers.C);

    memory.data[0] = LDAX_B;
    memory.data[1] = HLT;
    memory.data[adr] = target_val;

    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.A == target_val, "Test instruction LDAX successful.");
}

// Accumulator Rotation Instructions
void test_rlc_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();

    processor.registers.A = 0b11110010;

    memory.data[0] = RLC;
    memory.data[1] = HLT;
    
    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.A == 0b11100101, "Test instruction RLC successful: Value of A register.");
    assertm(processor.flags.CF == true, "Test instruction RLC successful: Value of Carry flag.");
}

void test_rrc_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();

    processor.registers.A = 0b11110010;

    memory.data[0] = RRC;
    memory.data[1] = HLT;
    
    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.A == 0b01111001, "Test instruction RRC successful: Value of A register.");
    assertm(processor.flags.CF == false, "Test instruction RRC successful: Value of Carry flag.");
}

void test_ral_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();

    processor.registers.A = 0b11110010;

    memory.data[0] = RAL;
    memory.data[1] = HLT;
    
    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.A == 0b11100100, "Test instruction RAL successful: Value of A register.");
    assertm(processor.flags.CF == true, "Test instruction RAL successful: Value of Carry flag.");
}

void test_rar_instruction(Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();

    processor.registers.A = 0b11110010;

    memory.data[0] = RAR;
    memory.data[1] = HLT;
    
    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.A == 0b01111001, "Test instruction RAR successful: Value of A register.");
    assertm(processor.flags.CF == false, "Test instruction RAR successful: Value of Carry flag.");
}