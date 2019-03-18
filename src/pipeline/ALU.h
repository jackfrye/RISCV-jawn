#ifndef __ALU_H__
#define __ALU_H__

#include <cstdint>

using namespace std;

class Algo_Logic_Unit
{
private:
    int64_t alu_result;
    bool is_zero;

public:
    Algo_Logic_Unit();

    void set_alu_ops(int64_t op_1, int64_t op_2, bool alu_op_0, bool alu_op_1, uint8_t funct7, uint8_t funct3);
    
    int64_t get_alu_result() { return alu_result; }
    bool get_alu_is_zero() { return is_zero; }
};

#endif
