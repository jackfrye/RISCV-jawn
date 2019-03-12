#ifndef __REGISTERS_H__
#define __REGISTERS_H__

#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Registers
{
private:
	int64_t registers[32] = {0};

public:
	Registers();
	int64_t read_reg(int);
	void assign_reg(int, int64_t);
	void printRegs(ofstream *out);
};

#endif
