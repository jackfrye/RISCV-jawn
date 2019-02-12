#ifndef __REGISTER_H__
#define __REGISTER_H__

#include <vector>
#include <string>

using namespace std;

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

#endif
