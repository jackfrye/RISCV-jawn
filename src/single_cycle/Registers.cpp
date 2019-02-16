#include "Registers.h"
#include <iostream>

Registers::Registers()
{
}

uint64_t Registers::read_reg(int index)
{
	return registers[index];
}

void Registers::assign_reg(int index, uint64_t value)
{
	registers[index] = value;
}

void Registers::printRegs(ofstream *out)
{
	for(int i = 0; i < 32; i++)
	{
		*out << "Reg " << i << ": " << registers[i] << endl;
	}
}
