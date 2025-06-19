#pragma once

#define NOP 0x00;

#define STAX_B 0x02

#define INR_B 0x04
#define DCR_B 0x05




#define LDAX_B 0x0A

#define INR_C 0x0C
#define DCR_C 0x0D



#define STAX_D 0x12

#define INR_D 0x14
#define DCR_D 0x15




#define LDAX_D 0x1A

#define INR_E 0x1C
#define DCR_E 0x1D






#define INR_H 0x24
#define DCR_H 0x25






#define INR_L 0x2C
#define DCR_L 0x2D

#define CMA 0x2F

#define STA 0x32

#define INR_M 0x34
#define DCR_M 0x35

#define STC 0x37


#define LDA 0x3A

#define INR_A 0x3C
#define DCR_A 0x3D

#define CMC 0x3F

#define MOV_B_B 0x40
#define MOV_B_C 0x41
#define MOV_B_D 0x42
#define MOV_B_E 0x43
#define MOV_B_H 0x44
#define MOV_B_L 0x45
#define MOV_B_M 0x46
#define MOV_B_A 0x47
#define MOV_C_B 0x48
#define MOV_C_C 0x49
#define MOV_C_D 0x4A
#define MOV_C_E 0x4B
#define MOV_C_H 0x4C
#define MOV_C_L 0x4D
#define MOV_C_M 0x4E
#define MOV_C_A 0x4F
#define MOV_D_B 0x50
#define MOV_D_C 0x51
#define MOV_D_D 0x52
#define MOV_D_E 0x53
#define MOV_D_H 0x54
#define MOV_D_L 0x55
#define MOV_D_M 0x56
#define MOV_D_A 0x57
#define MOV_E_B 0x58
#define MOV_E_C 0x59
#define MOV_E_D 0x5A
#define MOV_E_E 0x5B
#define MOV_E_H 0x5C
#define MOV_E_L 0x5D
#define MOV_E_M 0x5E
#define MOV_E_A 0x5F
#define MOV_H_B 0x60
#define MOV_H_C 0x61
#define MOV_H_D 0x62
#define MOV_H_E 0x63
#define MOV_H_H 0x64
#define MOV_H_L 0x65
#define MOV_H_M 0x66
#define MOV_H_A 0x67
#define MOV_L_B 0x68
#define MOV_L_C 0x69
#define MOV_L_D 0x6A
#define MOV_L_E 0x6B
#define MOV_L_H 0x6C
#define MOV_L_L 0x6D
#define MOV_L_M 0x6E
#define MOV_L_A 0x6F
#define MOV_M_B 0x70
#define MOV_M_C 0x71
#define MOV_M_D 0x72
#define MOV_M_E 0x73
#define MOV_M_H 0x74
#define MOV_M_L 0x75
#define HLT 0x76
#define MOV_M_A 0x77
#define MOV_A_B 0x78
#define MOV_A_C 0x79
#define MOV_A_D 0x7A
#define MOV_A_E 0x7B
#define MOV_A_H 0x7C
#define MOV_A_L 0x7D
#define MOV_A_M 0x7E
#define MOV_A_A 0x7F
#define ADD_B 0x80
#define ADD_C 0x81
#define ADD_D 0x82
#define ADD_E 0x83
#define ADD_H 0x84
#define ADD_L 0x85
#define ADD_M 0x86
#define ADD_A 0x87








#define SUB_B 0x90
#define SUB_C 0x91
#define SUB_D 0x92
#define SUB_E 0x93
#define SUB_H 0x94
#define SUB_L 0x95
#define SUB_M 0x96
#define SUB_A 0x97

#define ANA_B 0xA0
#define ANA_C 0xA1
#define ANA_D 0xA2
#define ANA_E 0xA3
#define ANA_H 0xA4
#define ANA_L 0xA5
#define ANA_M 0xA6
#define ANA_A 0xA7
#define XRA_B 0xA8
#define XRA_C 0xA9
#define XRA_D 0xAA
#define XRA_E 0xAB
#define XRA_H 0xAC
#define XRA_L 0xAD
#define XRA_M 0xAE
#define XRA_A 0xAF



















#define JMP 0xC3















#define OUT 0xD3