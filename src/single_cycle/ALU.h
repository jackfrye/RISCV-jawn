#ifndef __ALU_H__
#define __ALU_H__

#include <cstdint>

using namespace std;

class Algo_Logic_Unit
{
private:
    uint32_t alu_result;

public:
    Algo_Logic_Unit();

    void set_alu_ops(uint32_t op_1, uint32_t op_2, uint8_t alu_control, bool alu_op);
    
    uint32_t get_alu_result() { return alu_result; }
};

#endif
