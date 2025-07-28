#pragma once

#define INSTRUCTION_PARAMS [get_reg](little_byte opcode, Processor& processor, MEMORY& memory)

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <memory>
#include <array>
#include <algorithm>
#include <ranges>
#include <bit>
#include <functional>
#include <utility>

#include "definitions/datatypes.h"
#include "definitions/opcodes.h"
#include "processor.h"

struct MEMORY;
struct Processor;


constexpr auto mov_opcodes = std::to_array({MOV_B_B, MOV_B_C, MOV_B_D, MOV_B_E, MOV_B_H, MOV_B_L, MOV_B_M, MOV_B_A, MOV_C_B, MOV_C_C, MOV_C_D, MOV_C_E, MOV_C_H, MOV_C_L, MOV_C_M, MOV_C_A, MOV_D_B, MOV_D_C, MOV_D_D, MOV_D_E, MOV_D_H, MOV_D_L, MOV_D_M, MOV_D_A, MOV_E_B, MOV_E_C, MOV_E_D, MOV_E_E, MOV_E_H, MOV_E_L, MOV_E_M, MOV_E_A, MOV_H_B, MOV_H_C, MOV_H_D, MOV_H_E, MOV_H_H, MOV_H_L, MOV_H_M, MOV_H_A, MOV_L_B, MOV_L_C, MOV_L_D, MOV_L_E, MOV_L_H, MOV_L_L, MOV_L_M, MOV_L_A, MOV_M_B, MOV_M_C, MOV_M_D, MOV_M_E, MOV_M_H, MOV_M_L, MOV_M_A, MOV_A_B, MOV_A_C, MOV_A_D, MOV_A_E, MOV_A_H, MOV_A_L, MOV_A_M, MOV_A_A});
constexpr auto add_opcodes = std::to_array({ADD_B, ADD_C, ADD_D, ADD_E, ADD_H, ADD_L, ADD_M, ADD_A});
constexpr auto adc_opcodes = std::to_array({ADC_B, ADC_C, ADC_D, ADC_E, ADC_H, ADC_L, ADC_M, ADC_A});
constexpr auto sub_opcodes = std::to_array({SUB_B, SUB_C, SUB_D, SUB_E, SUB_H, SUB_L, SUB_M, SUB_A});
constexpr auto sbb_opcodes = std::to_array({SBB_B, SBB_C, SBB_D, SBB_E, SBB_H, SBB_L, SBB_M, SBB_A});

constexpr auto ana_opcodes = std::to_array({ANA_B, ANA_C, ANA_D, ANA_E, ANA_H, ANA_L, ANA_M, ANA_A});
constexpr auto xra_opcodes = std::to_array({XRA_B, XRA_C, XRA_D, XRA_E, XRA_H, XRA_L, XRA_M, XRA_A});
constexpr auto ora_opcodes = std::to_array({ORA_B, ORA_C, ORA_D, ORA_E, ORA_H, ORA_L, ORA_M, ORA_A});
constexpr auto cmp_opcodes = std::to_array({CMP_B, CMP_C, CMP_D, CMP_E, CMP_H, CMP_L, CMP_M, CMP_A});

constexpr auto inr_opcodes = std::to_array({INR_B, INR_C, INR_D, INR_E, INR_H, INR_L, INR_M, INR_A});
constexpr auto dcr_opcodes = std::to_array({DCR_B, DCR_C, DCR_D, DCR_E, DCR_H, DCR_L, DCR_M, DCR_A});

constexpr auto ldax_opcodes = std::to_array({LDAX_B, LDAX_D});
constexpr auto stax_opcodes = std::to_array({STAX_B, STAX_D});

constexpr auto push_opcodes = std::to_array({PUSH_B, PUSH_D, PUSH_H, PUSH_PSW});
constexpr auto pop_opcodes = std::to_array({POP_B, POP_D, POP_H, POP_PSW});

constexpr auto dad_opcodes = std::to_array({DAD_B, DAD_D, DAD_H, DAD_SP});
constexpr auto inx_opcodes = std::to_array({INX_B, INX_D, INX_H, INX_SP});
constexpr auto dcx_opcodes = std::to_array({DCX_B, DCX_D, DCX_H, DCX_SP});


namespace instructions {
    extern std::function<void(little_byte, Processor&, MEMORY&)> instruction_table[256];
    extern void execute_instruction(little_byte opcode, Processor& processor, MEMORY& memory);

    extern void init_instruction_table();

    little_byte get_two_bit_code(little_byte opcode);
    std::string get_reg_name_from_code(little_byte code);

    void adjust_value(Processor& processor, MEMORY& memory, little_byte& reg, little_byte amount);

    void inc_dec_instruction(little_byte opcode, Processor& processor, MEMORY& memory, signed char amount, std::string instruction_name);
    void add_instruction(little_byte opcode, Processor& processor, MEMORY& memory, signed char amount, std::string instruction_name);
    void sub_instruction(little_byte opcode, Processor& processor, MEMORY& memory, little_byte amount, std::string instruction_name);
    void move_instruction(little_byte opcode, Processor& processor, MEMORY& memory);

    void logical_and_instruction(little_byte opcode, Processor& processor, MEMORY& memory, little_byte value);
    void logical_xor_instruction(little_byte opcode, Processor& processor, MEMORY& memory, little_byte value);
    void logical_or_instruction(little_byte opcode, Processor& processor, MEMORY& memory, little_byte value);
    void compare_instruction(little_byte opcode, Processor& processor, MEMORY& memory, little_byte value);

    void load_instruction(little_byte opcode, Processor& processor, MEMORY& memory);
    void store_instruction(little_byte opcode, Processor& processor, MEMORY& memory);

    void complement_accumulator(Processor& processor);
    void complement_carry(Processor& processor);
    void set_carry(Processor& processor);

    // Rotate Accumulator Instructions
    void rotate_accumulator_left(Processor& processor);
    void rotate_accumulator_right(Processor& processor);
    void rotate_accumulator_left_carry(Processor& processor);
    void rotate_accumulator_right_carry(Processor& processor);

    // Register Pair Instructions
    void push_instruction(little_byte opcode, Processor& processor, MEMORY& memory);
    void pop_instruction(little_byte opcode, Processor& processor, MEMORY& memory);

    void double_add_instruction(little_byte opcode, Processor& processor);
    void increment_register_pair_instruction(little_byte opcode, Processor& processor);
    void decrement_register_pair_instruction(little_byte opcode, Processor& processor);

    void exchange_registers_instruction(Processor& processor);
    void exchange_stack_instruction(Processor& processor, MEMORY& memory);
    void load_sp_from_h_l_instruction(Processor& processor);
}