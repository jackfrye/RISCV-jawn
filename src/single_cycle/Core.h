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
#include "ALU.h"
#include "Imm_gen.h"
#include "Data_Memory.h"

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

	void printMem()
	{
		cout << "Core " << id << " : " << endl;
		data_memory->print_mem(out);
	}

	void printRegs()
	{
		registers->printRegs(out);
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
	uint64_t read_data1;
	uint64_t read_data2;
	int64_t mux_read_data2;

	/* Control signals */
	bool branch;
	bool mem_read;
	bool mem_to_reg;
	bool alu_op_0;
	bool alu_op_1;
	bool mem_write;
	bool alu_src;
	bool reg_write;
	uint8_t op_code;

    uint8_t funct7;
    uint8_t funct3;

	Control *control;

    Imm_gen *imm_gen;
    int64_t imm_gen_result;

	Algo_Logic_Unit *alu;
    bool alu_zero;
    uint64_t alu_out;

    Data_Memory *data_memory;
    unsigned int data_mem_read;

	/*
		Group Two: Simulator Related
	*/
	list<Instruction> pending_queue;

	void serve_pending_instrs();

	void printStats(list<Instruction>::iterator &ite);
};

#endif
