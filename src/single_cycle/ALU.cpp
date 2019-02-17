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

void Algo_Logic_Unit::set_alu_ops(uint64_t op_1, uint64_t op_2, bool alu_op_0, bool alu_op_1, uint8_t funct7, uint8_t funct3)
{
    uint64_t tmp_result;

    if (!alu_op_1 && !alu_op_0) { // ALUop 00 (Load/Store)
        tmp_result = op_1 + op_2;
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
	else if(funct3 == 0x1)
	{
		tmp_result = op_1 << op_2;
	}
	else if(funct3 == 0x5)
	{
		tmp_result = op_1 >> op_2;
	}
        else {
            tmp_result = -1;
        }
    }
    alu_result = tmp_result;
}

