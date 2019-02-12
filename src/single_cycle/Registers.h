#ifndef __REGISTER_H__
#define __REGISTER_H__

#include <vector>
#include <string>

using namespace std;

class Registers
{
private:
	uint64_t registers[32];

public:
	Registers();
	uint64_t read_reg(int);
	assign_reg(int, uint64_t);
}

#endif
