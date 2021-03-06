#include "Control.h"

using namespace std;

Control::Control()
{
	op_code = 0;
}

Control::Control(uint8_t _op_code)
{
	op_code = _op_code;
	if(op_code == R_FORMAT) // R-Format
	{
        jalr = 0;
        jump = 0;
		alu_src = 0;
		mem_to_reg = 0;
		reg_write = 1;
		mem_read = 1;
		mem_write = 0;
		branch = 0;
		alu_op_1 = 1;
		alu_op_0 = 0;
	}
	else if(op_code == LOAD) // load double word
	{
        jalr = 0;
        jump = 0;
		alu_src = 1;
		mem_to_reg = 1;
		reg_write = 1;
		mem_read = 1;
		mem_write = 0;
		branch = 0;
		alu_op_1 = 0;
		alu_op_0 = 0;
	}
	else if(op_code == STORE) // store double word
	{
        jalr = 0;
        jump = 0;
		alu_src = 1;
		mem_to_reg = 0;
		reg_write = 0;
		mem_read = 0;
		mem_write = 1;
		branch = 0;
		alu_op_1 = 0;
		alu_op_0 = 0;
	}
	else if(op_code == BRANCH) // branch if equal
	{
        jalr = 0;
        jump = 0;
		alu_src = 0;
		mem_to_reg = -1;
		reg_write = 0;
		mem_read = 0;
		mem_write = 0;
		branch = 1;
		alu_op_1 = 0;
		alu_op_0 = 1;
	}
    else if(op_code == I_FORMAT) // I-Format
    {
        jalr = 0;
        jump = 0;
		alu_src = 1;
		mem_to_reg = 0;
		reg_write = 1;
		mem_read = 1;
		mem_write = 0;
		branch = 0;
		alu_op_1 = 1;
		alu_op_0 = 0;
    }
    else if(op_code == JAL)
    {
        jalr = 0;
        jump = 1;
		alu_src = 0;
		mem_to_reg = 0;
		reg_write = 0;
		mem_read = 0;
		mem_write = 0;
		branch = 0;
		alu_op_1 = 0;
		alu_op_0 = 0;
    }
    else if(op_code == JALR)
    {
        jalr = 1;
        jump = 0;
		alu_src = 0;
		mem_to_reg = 0;
		reg_write = 0;
		mem_read = 0;
		mem_write = 0;
		branch = 0;
		alu_op_1 = 0;
		alu_op_0 = 0;
    }
	else
	{
        jump = 0;
		alu_src = 0;
		mem_to_reg = 0;
		reg_write = 0;
		mem_read = 0;
		mem_write = 0;
		branch = 0;
		alu_op_1 = 0;
		alu_op_0 = 0;
	}
}

void Control::set_op_code(uint8_t _op_code)
{
	op_code = _op_code;
	if(op_code == R_FORMAT) // R-Format
	{
        jalr = 0;
        jump = 0;
		alu_src = 0;
		mem_to_reg = 0;
		reg_write = 1;
		mem_read = 1;
		mem_write = 0;
		branch = 0;
		alu_op_1 = 1;
		alu_op_0 = 0;
	}
	else if(op_code == LOAD) // load double word
	{
        jalr = 0;
        jump = 0;
		alu_src = 1;
		mem_to_reg = 1;
		reg_write = 1;
		mem_read = 1;
		mem_write = 0;
		branch = 0;
		alu_op_1 = 0;
		alu_op_0 = 0;
	}
	else if(op_code == STORE) // store double word
	{
        jalr = 0;
        jump = 0;
		alu_src = 1;
		mem_to_reg = 0;
		reg_write = 0;
		mem_read = 0;
		mem_write = 1;
		branch = 0;
		alu_op_1 = 0;
		alu_op_0 = 0;
	}
	else if(op_code == BRANCH) // branch if equal
	{
        jalr = 0;
        jump = 0;
		alu_src = 0;
		mem_to_reg = -1;
		reg_write = 0;
		mem_read = 0;
		mem_write = 0;
		branch = 1;
		alu_op_1 = 0;
		alu_op_0 = 1;
	}
    else if(op_code == I_FORMAT) // I-Format
    {
        jalr = 0;
        jump = 0;
		alu_src = 1;
		mem_to_reg = 0;
		reg_write = 1;
		mem_read = 1;
		mem_write = 0;
		branch = 0;
		alu_op_1 = 1;
		alu_op_0 = 0;
    }
    else if(op_code == JAL)
    {
        jalr = 0;
        jump = 1;
		alu_src = 0;
		mem_to_reg = 0;
		reg_write = 0;
		mem_read = 0;
		mem_write = 0;
		branch = 1;
		alu_op_1 = 1;
		alu_op_0 = 1;
    }
    else if(op_code == JALR)
    {
        jalr = 1;
        jump = 0;
		alu_src = 0;
		mem_to_reg = 0;
		reg_write = 0;
		mem_read = 0;
		mem_write = 0;
		branch = 1;
		alu_op_1 = 1;
		alu_op_0 = 1;
    }
	else
	{
		alu_src = 0;
		mem_to_reg = 0;
		reg_write = 0;
		mem_read = 0;
		mem_write = 0;
		branch = 0;
		alu_op_1 = 0;
		alu_op_0 = 0;
	}
}

bool Control::get_jalr()
{
    return jalr;
}

bool Control::get_jump()
{
    return jump;
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

bool Control::get_alu_op_0()
{
	return alu_op_0;
}

bool Control::get_alu_op_1()
{
	return alu_op_1;
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
