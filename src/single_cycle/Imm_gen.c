#include "Imm_gen.h"

Imm_gen::Imm_gen()
{
}

void Imm_gen::Imm_gen(uint8_t op_code, uint32_t instruction)
{
    uint64_t tmp;

    switch (op_code) {
        case LOAD:     
            tmp = (instruction >> 20) & 0xFFF; // Instruction[31:20]
            break;
        case STORE:    
            tmp  =  (instruction >> 7)  & 0x1F;       // Instruction[11:7]
            tmp |= ((instruction >> 25) & 0x3F) << 5; // Instruction[31:25]
            break;
        case BRANCH:
            tmp  = ((instruction >> 31) & 0x1)  << 11; // Instruction[31]
            tmp |= ((instruction >> 7)  & 0x1)  << 10; // Instruction[7]
            tmp |= ((instruction >> 25) & 0x3F) << 4;  // Instruction[30:25]
            tmp |= ((instruction >> 8)  & 0x7);        // Instruction[11:8]
            break;
        case I_FORMAT:
            uint8_t funct3;

            funct3 = (instruction >> 12) & 0x7;

            // For immediate instructions SLLI, SRLI, and SRAI
            if (funct3 == 0x1 || funct3 == 0x5)
                tmp = (instruction >> 20) & 0x1F;

            tmp = (instruction >> 20) & 0xFFF; // Instruction[31:20]
            break;
        default:
            tmp = 0;
            break;
    }
    imm_gen_out = tmp;
}
