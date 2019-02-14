#ifndef __ALU_H__
#define __ALU_H__

#include <cstdint>

using namespace std;

class Algo_Logic_Unit
{
private:
    uint64_t op_1;
    uint64_t op_2;

    bool alu_op_0;
    bool alu_op_1;

    uint8_t funct7; 
    uint8_t funct3;

    uint64_t alu_result;

public:
    Algo_Logic_Unit();

    void set_alu_ops(uint64_t op_1, uint64_t op_2, bool alu_op_0, bool alu_op_1, uint8_t funct7, uint8_t funct3);
    
    uint64_t get_alu_result() { return alu_result; }
};

#endif
