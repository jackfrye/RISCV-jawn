#include "ALU.h"
#include <iostream>
#include <bitset>

using namespace std;

#define BEQ 0
#define BNE 1
#define BLT 2
#define BGT 3

Algo_Logic_Unit::Algo_Logic_Unit()
{
}

void Algo_Logic_Unit::set_alu_ops(int64_t op_1, int64_t op_2, bool alu_op_0, bool alu_op_1, uint8_t funct7, uint8_t funct3)
{
    int64_t tmp_result;

    if (!alu_op_1 && !alu_op_0) { // ALUop 00 (Load/Store)
        tmp_result = op_1 + op_2;
    }
    else if (alu_op_1 && alu_op_0) { // Jump
        is_zero = 1;
    }
    else if (!alu_op_1 && alu_op_0) { // ALUop 01 (BEQ)
        switch (funct3) {
            case BEQ:
                tmp_result = op_1 - op_2;
                is_zero = (tmp_result == 0) ? 1 : 0;
                break;
            case BNE:
                tmp_result = op_1 - op_2;
                is_zero = (tmp_result != 0) ? 1 : 0;
                break;
            case BLT:
                is_zero = (op_1 < op_2) ? 1 : 0;
                break;
            case BGT:
                is_zero = (op_1 >= op_2) ? 1 : 0;
                break;
            default:
                is_zero = 0;
                break;
        }
    }
    else if (alu_op_1 && !alu_op_0) { // ALUop 10 (R-type or I-type)
        switch (funct3) {
            case 0x0: tmp_result = op_1  + op_2; break; 
            case 0x1: tmp_result = op_1 << op_2; break;
            case 0x4: tmp_result = op_1  ^ op_2; break;
            case 0x5: tmp_result = op_1 >> op_2; break;
            case 0x6: tmp_result = op_1  | op_2; break;
            case 0x7: tmp_result = op_1  & op_2; break;
            default:  tmp_result = -1;           break;
        }
    }
    alu_result = tmp_result;
}

