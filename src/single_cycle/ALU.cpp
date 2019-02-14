#include "ALU.h"

Algo_Logic_Unit::Algo_Logic_Unit()
{
    this->op_1 = 0;
    this->op_2 = 0;

    this->alu_op_0 = 0;
    this->alu_op_1 = 0;

    this->funct7 = 0; 
    this->funct3 = 0;

    this->alu_result = 0;
}

void Algo_Logic_Unit::set_alu_ops(uint64_t op_1, uint64_t op_2, bool alu_op_0, bool alu_op_1, uint8_t funct7, uint8_t funct3)
{
    uint64_t tmp_result;

    if (!alu_op_1 && !alu_op_0) { // ALUop 00 (Load/Store)
        tmp_result = op_1 + op_2;
    }
    else if (!alu_op_1 && alu_op_0) { // ALUop 01 (BEQ)
        tmp_result = op_1 - op_2;
    }
    else if (alu_op_1 && !alu_op_0) { // ALUop 10 (R-type)
        if (funct7 == 0x20) {
            tmp_result = op_1 - op_2;
        }
        else if (!funct7 && !funct3) {
            tmp_result = op_1 + op_2;
        }
        else if (funct3 == 0x7) {
            tmp_result = op_1 & op_2;
        }
        else if (funct3 == 0x6) {
            tmp_result = op_1 | op_2;
        }
        else {
            tmp_result = 0;
        }
    }
    alu_result = tmp_result;
}

