#include "Imm_gen.h"

Imm_gen::Imm_gen() 
{
}

void Imm_gen::set_imm_gen(uint8_t op_code, uint32_t instruction)
{
    int64_t tmp;

    switch (op_code) {
        case LOAD:     
            tmp = (instruction >> 20) & 0xFFF; // Instruction[31:20]

            if (tmp & 800) {
                tmp |= ~0xFFF;
            }
            break;
        case STORE:    
            tmp  =  (instruction >> 7)  & 0x1F;       // Instruction[11:7]
            tmp |= ((instruction >> 25) & 0x3F) << 5; // Instruction[31:25]

            if (tmp & 800) {
                tmp |= ~0xFFF;
            }
            break;
        case JAL:
            tmp  = ((instruction << 31) & 0x1)   << 20;  // Instruction[31]
            tmp |= ((instruction << 12) & 0x3FF) << 10;  // Instruction[21:12]
            tmp |= ((instruction << 22) & 0x1)   <<  9;  // Instruction[22]
            tmp |= ((instruction << 23) & 0x7F);         // Instruction[30:23]

            if (tmp & 0x80000) {
                tmp |= ~0xFFFFF;
            }
            tmp = tmp << 1;
            break;
        case JALR:
            tmp = (instruction << 20) & 0xFFF; // Instruction[31:20]

            if (tmp & 800) {
                tmp |= ~0xFFF;
            }
            break;
        case BRANCH:
            tmp  = ((instruction >> 31) & 0x1)  << 11; // Instruction[31]
            tmp |= ((instruction >> 7)  & 0x1)  << 10; // Instruction[7]
            tmp |= ((instruction >> 25) & 0x3F) << 4;  // Instruction[30:25]
            tmp |= ((instruction >> 8)  & 0xF);        // Instruction[11:8]
    
            if (tmp & 0x800) {
                tmp |= ~0xFFF;
            }
            tmp = tmp << 1;

            break;
        case I_FORMAT:
            uint8_t funct3;

            funct3 = (instruction >> 12) & 0x7;

            // For immediate instructions SLLI, SRLI, and SRAI
            if (funct3 == 0x1 || funct3 == 0x5)
                tmp = (instruction >> 20) & 0x1F;

            tmp = (instruction >> 20) & 0xFFF; // Instruction[31:20]

            if (tmp & 800) {
                tmp |= ~0xFFF;
            }
            break;
        default:
            tmp = 0;
            break;
    }

    imm_gen_out = tmp;
}
