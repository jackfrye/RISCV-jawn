#include "ALU.h"

#define AND       0x0
#define OR        0x1
#define ADD       0x2
#define SUBTRACT  0x6

Algo_Logic_Unit::Algo_Logic_Unit()
{
}

void Algo_Logic_Unit::set_alu_ops(uint32_t op_1, uint32_t op_2, uint8_t alu_control, bool alu_op)
{
    uint32_t tmp_result;

    switch (alu_control) {
        case AND:
            tmp_result = op_1 & op_2;
            break;
        case OR:
            tmp_result = op_1 | op_2;
            break;
        case ADD:
            tmp_result = op_1 + op_2;
            break;
        case SUBTRACT:
            tmp_result = op_1 - op_2;
            break;
        default:
            break;
    }

    this->alu_result = (alu_op) ? tmp_result : 0;
}

