
#include "Control.h"

using namespace std;

Control::Control()
{
	op_code = 0;
}

Control::Control(uint8_t _op_code)
{
	op_code = _op_code;
	branch = (op_code >> 6) & 0x1;
	mem_read = (op_code >> 5 ) & 0x1;
	mem_to_reg = (op_code >> 4) & 0x1;
	alu_op = (op_code >> 3 ) & 0x1;
	mem_write = (op_code >> 2 ) & 0x1;
	alu_src = (op_code >> 1 ) & 0x1;
	reg_write = (op_code >> 0 ) & 0x1; 
}

void Control::set_op_code(uint8_t _op_code)
{
	op_code = _op_code;
	branch = (op_code >> 6) & 0x1;
	mem_read = (op_code >> 5 ) & 0x1;
	mem_to_reg = (op_code >> 4) & 0x1;
	alu_op = (op_code >> 3 ) & 0x1;
	mem_write = (op_code >> 2 ) & 0x1;
	alu_src = (op_code >> 1 ) & 0x1;
	reg_write = (op_code >> 0 ) & 0x1; 
}

bool Control::get_branch()
{
	return branch;
}

bool Control::Control::get_mem_read()
{
	return mem_read;
}

bool Control::get_mem_to_reg()
{
	return mem_to_reg;
}

bool Control::get_alu_op()
{
	return alu_op;
}

bool Control::get_mem_write()
{
	return mem_write;
}

bool Control::get_alu_src()
{
	return alu_src;
}

bool Control::get_reg_write()
{
	return reg_write;
}
