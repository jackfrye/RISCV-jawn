#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <string>

#define R_FORMAT 0x33
#define LOAD     0x03
#define STORE    0x23
#define BRANCH   0x63
#define I_FORMAT 0x13
#define JAL      0x6F
#define JALR     0x67

class Control
{
private:
    bool jump; // jump hack
	bool branch;
	bool mem_read;
	bool mem_to_reg;
	bool alu_op_0;
	bool alu_op_1;
	bool mem_write;
	bool alu_src;
	bool reg_write;
	uint8_t op_code;

public:
	Control();
	Control(uint8_t);
	void set_op_code(uint8_t);
    bool get_jump();
	bool get_branch();
	bool get_mem_read();
	bool get_mem_to_reg();
	bool get_alu_op_0();
	bool get_alu_op_1();
	bool get_mem_write();
	bool get_alu_src();
	bool get_reg_write();

};

#endif
