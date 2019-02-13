#ifndef __CORE_H__
#define __CORE_H__

#include <fstream>
#include <iostream>
#include <string>
#include <list>

#include "Instruction_Memory.h"
#include "Instruction.h"
#include "Registers.h"
#include "Control.h"

using namespace std;

class Core
{
public:
	Core(const string &fname, ofstream *out);

	bool tick(); // FALSE means all the instructions are exhausted

	int id; // Each core has its own ID

	void printInstrs()
	{
		cout << "Core " << id << " : " << endl;

		instr_mem->printInstr();
	}

private:

	ofstream *out; // Output file

	unsigned long long int clk;

	/*
		Group One: Design Components Here, an instruction memory has already been
		designed for you.
	*/
	long PC;

	Instruction_Memory *instr_mem;
	Registers *registers;

	/* Registers signals */
	uint8_t read_register1;
	uint8_t read_register2;
	uint8_t write_register;
	uint8_t write_data;
	uint32_t read_data1;
	uint32_t read_data2;

	/* Control signals */
	bool branch;
	bool mem_read;
	bool mem_to_reg;
	bool alu_op;
	bool mem_write;
	bool alu_src;
	bool reg_write;
	uint8_t op_code;

	Control *control;

	/*
		Group Two: Simulator Related
	*/
	list<Instruction> pending_queue;

	void serve_pending_instrs();

	void printStats(list<Instruction>::iterator &ite);
};

#endif
