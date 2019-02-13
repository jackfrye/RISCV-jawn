#include "Registers.h"

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
