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

#define MOV_B_B 0X40
#define MOV_B_C 0X41
#define MOV_B_D 0X42
#define MOV_B_E 0X43
#define MOV_B_H 0X44
#define MOV_B_L 0X45
#define MOV_B_M 0X46
#define MOV_B_A 0X47
#define MOV_C_B 0X48
#define MOV_C_C 0X49
#define MOV_C_D 0X4A
#define MOV_C_E 0X4B
#define MOV_C_H 0X4C
#define MOV_C_L 0X4D
#define MOV_C_M 0X4E
#define MOV_C_A 0X4F
#define MOV_D_B 0X50
#define MOV_D_C 0X51
#define MOV_D_D 0X52
#define MOV_D_E 0X53
#define MOV_D_H 0X54
#define MOV_D_L 0X55
#define MOV_D_M 0X56
#define MOV_D_A 0X57
#define MOV_E_B 0X58
#define MOV_E_C 0X59
#define MOV_E_D 0X5A
#define MOV_E_E 0X5B
#define MOV_E_H 0X5C
#define MOV_E_L 0X5D
#define MOV_E_M 0X5E
#define MOV_E_A 0X5F
#define MOV_H_B 0X60
#define MOV_H_C 0X61
#define MOV_H_D 0X62
#define MOV_H_E 0X63
#define MOV_H_H 0X64
#define MOV_H_L 0X65
#define MOV_H_M 0X66
#define MOV_H_A 0X67
#define MOV_L_B 0X68
#define MOV_L_C 0X69
#define MOV_L_D 0X6A
#define MOV_L_E 0X6B
#define MOV_L_H 0X6C
#define MOV_L_L 0X6D
#define MOV_L_M 0X6E
#define MOV_L_A 0X6F
#define MOV_M_B 0X70
#define MOV_M_C 0X71
#define MOV_M_D 0X72
#define MOV_M_E 0X73
#define MOV_M_H 0X74
#define MOV_M_L 0X75
#define HLT 0X76
#define MOV_M_A 0X77
#define MOV_A_B 0X78
#define MOV_A_C 0X79
#define MOV_A_D 0X7A
#define MOV_A_E 0X7B
#define MOV_A_H 0X7C
#define MOV_A_L 0X7D
#define MOV_A_M 0X7E
#define MOV_A_A 0X7F
#define ADD_B 0x80
#define ADD_C 0x81
#define ADD_D 0x82
#define ADD_H 0x83
#define ADD_L 0x84
#define ADD_M 0x85
#define ADD_A 0x86

#define SUB_B 0x90
#define SUB_C 0x91
#define SUB_D 0x92
#define SUB_H 0x93
#define SUB_L 0x94
#define SUB_M 0x95
#define SUB_A 0x96

#define JMP 0xC3
#define OUT 0xD3