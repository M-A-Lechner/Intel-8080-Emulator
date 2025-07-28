#define TEST_PARAMS Processor processor, MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data

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

void test_ana_instruction(TEST_PARAMS);
void test_add_instruction(TEST_PARAMS);
void test_sub_instruction(TEST_PARAMS);
void test_ldax_instruction(TEST_PARAMS);

void test_rlc_instruction(TEST_PARAMS);
void test_rrc_instruction(TEST_PARAMS);
void test_ral_instruction(TEST_PARAMS);
void test_rar_instruction(TEST_PARAMS);

void test_push_instruction(TEST_PARAMS);
void test_pop_instruction(TEST_PARAMS);

void test_dad_instruction(TEST_PARAMS);
void test_increment_register_pair_instruction(TEST_PARAMS);
void test_decrement_register_pair_instruction(TEST_PARAMS);
void test_exchange_registers_instruction(TEST_PARAMS);
void test_exchange_stack_instruction(TEST_PARAMS);
void test_load_sp_from_h_l_instruction(TEST_PARAMS);

int main () {
    instructions::init_instruction_table();
    Processor cpu;
    cpu.reset();
    cpu.DEBUG_MODE = true;
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

    test_push_instruction(cpu, memory, in_data, out_data);
    test_pop_instruction(cpu, memory, in_data, out_data);

    test_dad_instruction(cpu, memory, in_data, out_data);
    test_increment_register_pair_instruction(cpu, memory, in_data, out_data);
    test_decrement_register_pair_instruction(cpu, memory, in_data, out_data);
    test_exchange_registers_instruction(cpu, memory, in_data, out_data);
    test_exchange_stack_instruction(cpu, memory, in_data, out_data);
    test_load_sp_from_h_l_instruction(cpu, memory, in_data, out_data);
}

void reset_all_memory(MEMORY& memory, IN_DATA& in_data, OUT_DATA& out_data) {
    memory.reset();
    in_data.reset();
    out_data.reset();
}

void test_ana_instruction(TEST_PARAMS) {
    reset_all_memory(memory, in_data, out_data);

    processor.reset();

    processor.registers.A = 0b00110011;
    processor.registers.B = 0b00001111;

    memory.data[0] = ANA_B;
    memory.data[1] = HLT;

    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.A == 0b00000011, "Test instruction ANA successful.");
}

void test_add_instruction(TEST_PARAMS) {
    reset_all_memory(memory, in_data, out_data);

    processor.reset();

    processor.registers.A = 0b00001000;

    memory.data[0] = ADD_A;
    memory.data[1] = HLT;

    processor.execute(memory, in_data, out_data);

    std::cout << (int)processor.registers.A << "\n";

    assertm(processor.registers.A == 0b00010000, "Test instruction ADD successful.");
}

void test_sub_instruction(TEST_PARAMS) {
    reset_all_memory(memory, in_data, out_data);

    processor.reset();

    processor.registers.A = 0b00111110;

    memory.data[0] = SUB_A;
    memory.data[1] = HLT;

    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.A == 0b00000000, "Test instruction SUB successful: Value of A register.");
    assertm(processor.flags.CF == 0, "Test instruction SUB successful: Value of Carry flag.");
}

void test_ldax_instruction(TEST_PARAMS) {
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
void test_rlc_instruction(TEST_PARAMS) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();

    processor.registers.A = 0b11110010;

    memory.data[0] = RLC;
    memory.data[1] = HLT;
    
    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.A == 0b11100101, "Test instruction RLC successful: Value of A register.");
    assertm(processor.flags.CF == true, "Test instruction RLC successful: Value of Carry flag.");
}

void test_rrc_instruction(TEST_PARAMS) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();

    processor.registers.A = 0b11110010;

    memory.data[0] = RRC;
    memory.data[1] = HLT;
    
    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.A == 0b01111001, "Test instruction RRC successful: Value of A register.");
    assertm(processor.flags.CF == false, "Test instruction RRC successful: Value of Carry flag.");
}

void test_ral_instruction(TEST_PARAMS) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();

    processor.registers.A = 0b11110010;

    memory.data[0] = RAL;
    memory.data[1] = HLT;
    
    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.A == 0b11100100, "Test instruction RAL successful: Value of A register.");
    assertm(processor.flags.CF == true, "Test instruction RAL successful: Value of Carry flag.");
}

void test_rar_instruction(TEST_PARAMS) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();

    processor.registers.A = 0b11110010;

    memory.data[0] = RAR;
    memory.data[1] = HLT;
    
    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.A == 0b01111001, "Test instruction RAR successful: Value of A register.");
    assertm(processor.flags.CF == false, "Test instruction RAR successful: Value of Carry flag.");
}

void test_push_instruction(TEST_PARAMS) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();

    processor.registers.B = 0x8F;
    processor.registers.C = 0x9D;

    processor.SP = 0x3A2C;

    memory.data[0] = PUSH_B;
    memory.data[1] = HLT;

    processor.execute(memory, in_data, out_data);

    assertm(memory.data[0x3A2B] == 0x8F, "Test instruction PUSH successful: Value of B register stored in correct stack location.");
    assertm(memory.data[0x3A2A] == 0x9D, "Test instruction PUSH successful: Value of C register stored in correct stack location.");
    assertm(processor.SP == 0x3A2A, "Test instruction PUSH successful: Value of Stack Pointer.");
}

void test_pop_instruction(TEST_PARAMS) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();

    processor.registers.B = 0x8F;
    processor.registers.C = 0x9D;

    processor.SP = 0x3A2C;

    memory.data[0] = POP_B;
    memory.data[1] = HLT;

    memory.data[0x3A2C] = 0x00;
    memory.data[0x3A2D] = 0x10;

    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.B == 0x10, "Test instruction POP successful: Value of B register.");
    assertm(processor.registers.C == 0x00, "Test instruction POP successful: Value of C register.");
    assertm(processor.SP == 0x3A2E, "Test instruction POP successful: Value of Stack Pointer.");
}

void test_dad_instruction(TEST_PARAMS) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();

    processor.flags.CF = true;

    processor.registers.B = 0x33;
    processor.registers.C = 0x9F;

    processor.registers.H = 0xA1;
    processor.registers.L = 0x7B;

    memory.data[0] = DAD_B;
    memory.data[1] = HLT;

    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.H == 0xD5, "Test instruction DAD successful: Value of H register.");
    assertm(processor.registers.L == 0x1A, "Test instruction DAD successful: Value of L register.");
    assertm(processor.flags.CF == 0, "Test instruction DAD successful: Value of Carry flag.");
}

void test_increment_register_pair_instruction(TEST_PARAMS) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();

    processor.registers.B = 0x33;
    processor.registers.C = 0xFF;

    memory.data[0] = INX_B;
    memory.data[1] = HLT;

    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.B == 0x34, "Test instruction INX successful: Value of B register.");
    assertm(processor.registers.C == 0x00, "Test instruction INX successful: Value of C register.");
}

void test_decrement_register_pair_instruction(TEST_PARAMS) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();

    processor.registers.B = 0x98;
    processor.registers.C = 0x00;

    memory.data[0] = DCX_B;
    memory.data[1] = HLT;

    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.B == 0x97, "Test instruction DCX successful: Value of B register.");
    assertm(processor.registers.C == 0xFF, "Test instruction DCX successful: Value of C register.");
}

void test_exchange_registers_instruction(TEST_PARAMS) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();

    processor.registers.D = 0x33;
    processor.registers.E = 0x55;
    
    processor.registers.H = 0x00;
    processor.registers.L = 0xFF;

    memory.data[0] = XCHG;
    memory.data[1] = HLT;

    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.D == 0x00, "Test instruction XCHG successful: Value of D register.");
    assertm(processor.registers.E == 0xFF, "Test instruction XCHG successful: Value of E register.");
    assertm(processor.registers.H == 0x33, "Test instruction XCHG successful: Value of H register.");
    assertm(processor.registers.L == 0x55, "Test instruction XCHG successful: Value of L register.");
}

void test_exchange_stack_instruction(TEST_PARAMS) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();
    
    processor.registers.H = 0x0B;
    processor.registers.L = 0x3C;

    processor.SP = 0x10AD;

    memory.data[0] = XTHL;
    memory.data[1] = HLT;

    memory.data[0x10AD] = 0xF0;
    memory.data[0x10AE] = 0x0D;

    processor.execute(memory, in_data, out_data);

    assertm(processor.registers.H == 0x0D, "Test instruction XTHL successful: Value of H register.");
    assertm(processor.registers.L == 0xF0, "Test instruction XTHL successful: Value of L register.");
    assertm(memory.data[0x10AD] == 0x3C, "Test instruction XTHL successful: Value at memory location 0x10AD.");
    assertm(memory.data[0x10AE] == 0x0B, "Test instruction XTHL successful: Value at memory location 0x10AE.");
}

void test_load_sp_from_h_l_instruction(TEST_PARAMS) {
    reset_all_memory(memory, in_data, out_data);
    processor.reset();
    
    processor.registers.H = 0x50;
    processor.registers.L = 0x6C;

    memory.data[0] = SPHL;
    memory.data[1] = HLT;

    processor.execute(memory, in_data, out_data);

    assertm(processor.SP == 0x506C, "Test instruction SPHL successful: Value of Stack Pointer.");
}