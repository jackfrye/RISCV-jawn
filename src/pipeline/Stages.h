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
#include "Imm_gen.h"
#include "ALU.h"
#include "Data_Memory.h"

using namespace std;

class Core;

class IF_Stage;
class ID_Stage;
class EX_Stage;
class MEM_Stage;
class WB_Stage;


struct if_id_reg
{
        int valid; // Is content inside register valid?

        int WB; // Is WB required?

        int rd_index;
        int rs_1_index;
        int rs_2_index;

	long PC;
	int instruction;

};


struct id_ex_reg
{
        int valid; // Is content inside register valid?

        int WB; // Is WB required?

        int rd_index;
        int rs_1_index;
	int rs_2_index;
	
	/* Ex signals */
	bool alu_op1;
	bool alu_op2;
	bool alu_src;

	/* Mem signals */
	bool branch;
	bool mem_read;
	bool mem_write;

	/* WB signals */
	bool reg_write;
	bool mem_to_reg;

	long PC;	

	int64_t read_data1;
	int64_t read_data2;

    int64_t imm_gen_result;

	uint8_t funct7;
    uint8_t funct3;

	bool jump;
	bool jalr;
	
};


struct ex_mem_reg
{
    int valid; // Is content inside register valid?

    int WB; // Is WB required?

    int rd_index;
    int rs_1_index;
    int rs_2_index;

	/* Mem signals */
	bool branch;
	bool mem_read;
	bool mem_write;

	/* WB signals */
	bool reg_write;
	bool mem_to_reg;

	/* Jump address */
	int64_t add_sum;

	/* ALU Signals */
	bool alu_zero;
    int64_t alu_out;

	/* Memory Write Data/Register2 Data */
	int64_t read_data2;
	
};

struct mem_wb_reg
{
    int valid; // Is content inside register valid?

    int WB; // Is WB required?

    int rd_index;
    int rs_1_index;
    int rs_2_index;

	/* WB signals */
	bool reg_write;
	bool mem_to_reg;

	int data_mem_read;

	bool alu_zero;
	int64_t alu_out;
	
};


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
        if_flush = false;
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
	bool if_flush;
        long PC;

        Instruction_Memory *instr_mem;

        /*
         * TODO, define your IF/ID register here.
         * */
        /*
	 * Here shows the prototype of an in-complete IF/ID register. You should 
	 * extend it further to get a complete IF/ID register.
	 * */

        
	struct if_id_reg if_id_reg;
};

class ID_Stage
{
public:
        ID_Stage() : stall(0), end(0)
        {
                id_ex_reg.valid = 0;		
                id_ex_reg.mem_to_reg = 0;
        }

	/* Modules from Single Cycle */
	Registers registers;
	Control control;
	Imm_gen imm_gen;

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
         * TODO, define your ID/EX register here.
         * */
        /*
         * Here shows the prototype of an in-complete ID/EX register. You should extend it further
	 * to get a complete ID/EX register.
	 * */
        struct id_ex_reg id_ex_reg;
};

class EX_Stage
{
public:
    EX_Stage() : bubble(0), end(0)
	{
        ex_mem_reg.valid = 0;
        ex_mem_reg.alu_out = 0;
        ex_mem_reg.read_data2 = 0;
    }

	Algo_Logic_Unit alu; 

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
	MEM_Stage *mem_stage;
	WB_Stage *wb_stage;

	/*
	 * TODO, design components of EX stage here.
	 * */

        /*
         * TODO, define your EX/MEM register here.
         * */
        /*
	 * Here shows the prototype of an in-complete EX/MEM register. Extend it further to a 
	 * complete EX/MEM register.
	 * */

        struct ex_mem_reg ex_mem_reg;
};

class MEM_Stage
{
public:
    MEM_Stage() : end(0)
    {
        mem_wb_reg.valid = 0;
        mem_wb_reg.data_mem_read = 0;
        mem_wb_reg.mem_to_reg = 0;
        mem_wb_reg.alu_out = 0;
    }

	Data_Memory data_memory;

    void tick();

    /*
     * Important signals.
     * */
    list<Instruction>::iterator instr; // Points to the instruction currently in the stage
        
	int end; // All instructions are exhausted?
	
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

    struct mem_wb_reg mem_wb_reg;
};

class WB_Stage
{
public:
        WB_Stage() : end(0)
        {
		int64_t mux_out = 0;
        }

	int64_t mux_out;
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
