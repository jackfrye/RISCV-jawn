#ifndef __IMM_GEN_H__
#define __IMM_GEN_H__

#include "Control.h"
#include <cstdint>

using namespace std;

class Imm_gen 
{
private:
    uint64_t imm_gen_out;

public:
    Imm_gen();
    void set_imm_gen(uint8_t op_code, uint32_t instruction);
    uint64_t get_imm_gen_result() { return imm_gen_out; }
};

#endif
