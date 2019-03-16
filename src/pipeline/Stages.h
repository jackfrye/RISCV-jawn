#ifndef __STAGES_H__
#define __STAGES_H__

#include <fstream>
#include <iostream>
#include <string>
#include <list>

#include "Core.h"

#include "Instruction_Memory.h"
#include "Instruction.h"
#include "Registers.h"
#include "Control.h"
#include "ALU.h"
#include "Imm_gen.h"
#include "Data_Memory.h"

using namespace std;

class Core;

class IF_Stage;
class ID_Stage;
class EX_Stage;
class MEM_Stage;
class WB_Stage;

/*
 * The following classes are for demonstration only. You should modify it based on the requirements.
 * */
class IF_Stage
{
public:
        IF_Stage(const string &fname, Core* core) : instr_mem(new Instruction_Memory(fname)),
							core(core),
							PC(0),
							stall(0),
							end(0)
	{
		// Initially, IF/ID Register is invalid.
		if_id_reg.valid = 0;

	}

        void tick();

        /*
         * Important signals.
         * */
        list<Instruction>::iterator instr; // Points to the instruction currently in the stage

        int stall; // Is the stage stalled?
        int end; // All instructions are exhausted?

        /*
         * Related Class
         * */
        Core *core;

	/*
         * TODO, Design components of IF stage here
         * */
        long PC;

        Instruction_Memory *instr_mem;

        /*
         * TODO, define your IF/ID register here.
         * */
        /*
	 * Here shows the prototype of an in-complete IF/ID register. You should 
	 * extend it further to get a complete IF/ID register.
	 * */
        struct Register
        {
                int valid; // Is content inside register valid?
		long addr;
		unsigned int instr;
	};
        
	Register if_id_reg;
};

class ID_Stage
{
public:
        ID_Stage() : stall(0), end(0)
        {
                id_ex_reg.valid = 0;
		regs = new Registers();
		control = new Control();
		imm_gen = new Imm_gen();
        }

	ID_Stage(Registers *_regs, Control *_control, Imm_gen *_imm_gen) : stall(0), end(0)
        {
		regs = _regs;
		control = _control;
		imm_gen = _imm_gen;
                id_ex_reg.valid = 0;
        }
        void tick();

        /*
         * Hazard detection unit: stall ID and IF stages, meanwhile, insert bubbles to
         * EX stage.
         * */
        void hazard_detection();

        /*
         * Important signals.
         * */
        list<Instruction>::iterator instr; // Points to the instruction currently in the stage

        int stall; // Is the stage stalled?
	int end; // All instructions are exhausted?

        /*
         * Related Class
	 *
	 * Hazard detection unit needs access to IF and EX stage.
         * */
        IF_Stage *if_stage;
        EX_Stage *ex_stage;
	MEM_Stage *mem_stage;


	/*
	 * TODO, design components of ID stage here.
	 * */
	Registers *regs;
	Control *control;
	Imm_gen *imm_gen;
        /*
         * TODO, define your ID/EX register here.
         * */
        /*
         * Here shows the prototype of an in-complete ID/EX register. You should extend it further
	 * to get a complete ID/EX register.
	 * */
        struct Register
        {
                int valid; // Is content inside register valid?

                bool branch;
		bool mem_read;
		bool mem_to_reg;
		bool alu_op_0;
		bool alu_op_1;
		bool mem_write;
		bool alu_src;
		bool reg_write;

		uint64_t data1;
		uint64_t data2;
                int rd_index;
                int rs_1_index;
		int rs_2_index;
		uint64_t imm_gen;
		long addr;

		bool jump;
		bool jalr;

	    	uint8_t funct7;
    		uint8_t funct3;
	};
        Register id_ex_reg;
};

class EX_Stage
{
public:
        EX_Stage() : bubble(0), end(0)
	{
                ex_mem_reg.valid = 0;
		alu = new Algo_Logic_Unit();
        }

        void tick();

        /*
         * Important signals.
	*/
        list<Instruction>::iterator instr; // Points to the instruction currently in the stage
        
	int bubble; // A bubble is inserted?
        int end; // All instructions are exhausted?

	/*
         * Related Class
         * */
	IF_Stage *if_stage;
        ID_Stage *id_stage;

	/*
	 * TODO, design components of EX stage here.
	 * */
	Algo_Logic_Unit *alu;

        /*
         * TODO, define your EX/MEM register here.
         * */
        /*
	 * Here shows the prototype of an in-complete EX/MEM register. Extend it further to a 
	 * complete EX/MEM register.
	 * */
        struct Register
        {
                int valid; // Is content inside register valid?

                uint64_t alu_out;
		int write_reg_addr;
		bool alu_zero;
		int read_reg2;

		bool branch;
		bool mem_read;
		bool mem_to_reg;
		bool mem_write;
		bool reg_write;

		long add_sum;
        };
        Register ex_mem_reg;
};

class MEM_Stage
{
public:
        MEM_Stage() : end(0)
        {
                mem_wb_reg.valid = 0;
		data_mem = new Data_Memory();
        }

        void tick();

        /*
         * Important signals.
         * */
        list<Instruction>::iterator instr; // Points to the instruction currently in the stage
        
	int end; // All instructions are exhausted?
	Data_Memory *data_mem;
	/*
         * Related Class
         * */
	IF_Stage *if_stage;
        EX_Stage *ex_stage;
	

	/*
	 * TODO, design component of MEM stage here.
	 * */

        /*
         * TODO, define your MEM/WB register here.
         * */
        /*
	 * Here shows the prototype of an in-complete MEM/WB register. Extend it further to get
	 * a complete MEM/WB register.
	 * */
        struct Register
        {
                int valid; // Is content inside register valid?
		uint64_t alu_out;
		unsigned int data_mem_read;
		bool reg_write;
		bool mem_to_reg;
		int write_reg_addr;
                
        };
        Register mem_wb_reg;
};

class WB_Stage
{
public:
        WB_Stage() : end(0)
        {

        }

        void tick();

        /*
         * Important signals.
         * */
        list<Instruction>::iterator instr; // Points to the instruction currently in the stage
        
	int end; // All instructions are exhausted?

        /*
         * Related Class
         * */
        MEM_Stage *mem_stage;
	ID_Stage *id_stage;

	/*
	 * TODO, you should write to the register file defined in ID stage. Do you know why?
	 * */
};

#endif
