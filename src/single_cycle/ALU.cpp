#include "ALU.h"
#include <iostream>
#include <bitset>

using namespace std;

Algo_Logic_Unit::Algo_Logic_Unit()
{
}

void Algo_Logic_Unit::set_alu_ops(uint64_t op_1, uint64_t op_2, bool alu_op_0, bool alu_op_1, uint8_t funct7, uint8_t funct3)
{
    uint64_t tmp_result;

	cout << "op_1: " << op_1 << endl;
	cout << "op_2: " << op_2 << endl;
	cout << "alu_op_0: " << alu_op_0 << endl;
	cout << "alu_op_1: " << alu_op_1 << endl;
	bitset<8> f7(funct7);
	cout << "funct7: " << f7 << endl;
	bitset<8> f3(funct3);
	cout << "funct3: " << f3 << endl;

    if (!alu_op_1 && !alu_op_0) { // ALUop 00 (Load/Store)
        tmp_result = op_1 + op_2;
    }
    else if (!alu_op_1 && alu_op_0) { // ALUop 01 (BEQ)
        tmp_result = op_1 - op_2;
        is_zero = (tmp_result == 0) ? 1 : 0;
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
    cout << "ALU RESULTS: " << alu_result << endl;
	
}

