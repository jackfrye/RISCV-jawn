#include "Stages.h"
#include "Instruction.h"
#include "Registers.h"
#include "Imm_gen.h"

#define BEQ 0
#define BNE 1
#define BLT 2
#define BGT 3

void IF_Stage::tick()
{
	if (end == 1)
	{
		// No instruction to run, return.
		return;
	}
	
	if (PC == instr_mem->last_addr())
	{
		// No instruction to run for next clock cycle since we have reached the last 
		// instruction.
		end = 1;
	}

	/*
	 * Simulate IF stage here
	 * */
	if (stall == 0 && if_flush == 0)
	{
		// Get Instruction
		Instruction &instruction = instr_mem->get_instruction(PC);

		if_id_reg.PC = PC;
		if_id_reg.instruction = instruction.instruction;

		// Increment PC
		// TODO, PC should be incremented or decremented based on instruction
		PC += 4;

		/*
		 * TODO, fix me. Simulate IF Stage here.
		 * Example on how to extract fields have been given.
		 * */
		if_id_reg.valid = 1;

		// For demonstration, I assume all instructions are R-type.
		if_id_reg.WB = 1; 

		if_id_reg.rd_index = (instruction.instruction >> 7) & 31;
		if_id_reg.rs_1_index = (instruction.instruction >> (7 + 5 + 3)) & 31;
		if_id_reg.rs_2_index = (instruction.instruction >> (7 + 5 + 3 + 5)) & 31;

		/*
			Simulator related
		*/
		instruction.begin_exe = core->clk;

		// Initialize end execution time to 5 clock cycles but adjust it
		// if a stall detected.
		instruction.end_exe = core->clk + 4;

		// Push instruction object into queue
		(core->pending_queue).push_back(instruction);
		instr = (core->pending_queue).end();
		instr--;

		/*
	 	* De-bugging
	 	* */
		if (DEBUG)
		{
			cout << "IF : " << instr->raw_instr << " | ";	
		}
	}
}

void ID_Stage::hazard_detection()
{
	/*
	 * TODO, fix me, please modify this function in order to get a complete detection unit.
	 * For demonstration, I assume all instructions are R-type (WB is always set).
	 * */

	/*
	 * (1) EX/MEM.rd = ID/EX.rs1
	 * (2) EX/MEM.rd = ID/EX.rs2
	 * (3) MEM/WB.rd = ID/EX.rs1
	 * (4) MEM/WB.rd = ID/EX.rs2
	 * */

	// Instruction tries to read a register following a load instruction that writes to the 
	// same register
	if( id_ex_reg.mem_read && ( (id_ex_reg.rd_index == if_stage->if_id_reg.rs_1_index) ||  (id_ex_reg.rd_index == if_stage->if_id_reg.rs_2_index) ) )
	{
		if_stage->stall = 1; // Fetching should not proceed.
		stall = 1; // ID should also stall.
		ex_stage->bubble = 1; // EX stage should not accept any new instructions

		instr->end_exe += 1; // The end execution time should be incremented by 1.

		return;
	}

	// Branching control logic
	uint64_t add_sum = id_ex_reg.PC + id_ex_reg.imm_gen_result;
	if (id_ex_reg.jump) { // Jump
        	if_stage->if_flush = 1;
		if_stage->PC = add_sum;
		return;
    	}
	else if(id_ex_reg.jalr)
	{
        	if_stage->if_flush = 1;
		if_stage->PC = add_sum;
	}
    	else if (!id_ex_reg.alu_op2 && id_ex_reg.alu_op1) { // ALUop 01 (BEQ)
		int64_t tmp_result;
		bool is_zero;
		switch (id_ex_reg.funct3) {
		    case BEQ:
		        tmp_result = id_ex_reg.read_data1 - id_ex_reg.read_data2;
		        is_zero = (tmp_result == 0) ? 1 : 0;
		        break;
		    case BNE:
		        tmp_result = id_ex_reg.read_data1 - id_ex_reg.read_data2;
		        is_zero = (tmp_result != 0) ? 1 : 0;
		        break;
		    case BLT:
		        is_zero = (id_ex_reg.read_data1 < id_ex_reg.read_data2) ? 1 : 0;
		        break;
		    case BGT:
		        is_zero = (id_ex_reg.read_data1 >= id_ex_reg.read_data2) ? 1 : 0;
		        break;
		    default:
		        is_zero = 0;
		        break;
		}
		if(is_zero)
		{
			if_stage->if_flush = 1;
			if_stage->PC = add_sum;
			return;
		}
    	}


	if_stage->stall = 0; // No hazard found, fetching proceed.
	stall = 0; // No hazard found, ID stage proceed.
	ex_stage->bubble = 0; // No hazard found, execution proceed.
}

void ID_Stage::tick()
{
        if (end == 1 && stall == 0)
        {
                // Instructions are run out, do nothing.
		return;
        }

        if (if_stage->if_id_reg.valid == 0)
        {
                // IF_ID register is invalid, do nothing.
		return;
        }
	
	/*
	 * FIXME, simulate ID stage here.
         **/
	end = if_stage->end; // end signal is propagated from IF stage
	
	instr = if_stage->instr; // instruction pointer is also propagated from IF stage
	
	id_ex_reg.valid = if_stage->if_id_reg.valid;
	id_ex_reg.WB = if_stage->if_id_reg.WB;
	id_ex_reg.rd_index = if_stage->if_id_reg.rd_index;
	id_ex_reg.rs_1_index = if_stage->if_id_reg.rs_1_index;
	id_ex_reg.rs_2_index = if_stage->if_id_reg.rs_2_index;

	uint8_t op_code = if_stage->if_id_reg.instruction & 0x007F;
	control.set_op_code(op_code);

	id_ex_reg.branch = control.get_branch();
	id_ex_reg.mem_read = control.get_mem_read();
	id_ex_reg.mem_to_reg = control.get_mem_to_reg();
	id_ex_reg.alu_op1 = control.get_alu_op_0();
	id_ex_reg.alu_op2 = control.get_alu_op_1();
	id_ex_reg.mem_write = control.get_mem_write();
	id_ex_reg.alu_src = control.get_alu_src();
	id_ex_reg.reg_write = control.get_reg_write();		
	
	id_ex_reg.PC = if_stage->if_id_reg.PC;

	id_ex_reg.read_data1 = registers.read_reg(if_stage->if_id_reg.rs_1_index);
	id_ex_reg.read_data2 = registers.read_reg(if_stage->if_id_reg.rs_2_index);

	imm_gen.set_imm_gen(op_code, if_stage->if_id_reg.instruction);
    id_ex_reg.imm_gen_result = imm_gen.get_imm_gen_result();

	id_ex_reg.funct7 = (if_stage->if_id_reg.instruction >> 25) & 0x7F;
    id_ex_reg.funct3 = (if_stage->if_id_reg.instruction >> 12) & 0x7;

	id_ex_reg.jalr = control.get_jalr();
    id_ex_reg.jump = control.get_jump();

	hazard_detection();


	/*
	 * De-bugging
	 * */
	if (DEBUG)
	{
		cout << "ID : " << instr->raw_instr;
		
		if (stall)
		{
			cout << " (stalled) ";
		}

		cout << " | ";	
	}
}

void EX_Stage::tick()
{
	if (bubble == 1)
    {
            // A bubble is inserted, do nothing.
            return;
    }

    if (end == 1)
    {
            // Instructions are run out, do nothing.
            return;
    }

    if (id_stage->id_ex_reg.valid == 0)
    {
            // ID_EX register is invalid, do nothing.
            return;
    }
	
	/*
	 * FIXME, simulate EX stage here
         **/
	end = id_stage->end; // end signal is propagated from IF stage

	instr = id_stage->instr; // instruction pointer is also propagated from IF stage

	ex_mem_reg.valid = id_stage->id_ex_reg.valid;
	id_stage->id_ex_reg.valid = 0; // I only allow any unique instruction to be read only 
					// once in order to increase simulator performance.


	/* DATA FORWARDING LOGIC */
	int64_t alu_in1;
	int64_t alu_in2;

	/* ALU input 1 gets from mem stage */
	// ALU DATA 1
	if(ex_mem_reg.reg_write && (ex_mem_reg.rd_index != 0) && (ex_mem_reg.rd_index == id_stage->id_ex_reg.rs_1_index) )
	{
		alu_in1 = mem_stage->mem_wb_reg.rs_1_index;
	}
	else if(mem_stage->mem_wb_reg.reg_write && (mem_stage->mem_wb_reg.rd_index != 0) && !( ex_mem_reg.reg_write && (ex_mem_reg.rd_index != 0) && (ex_mem_reg.rd_index == id_stage->id_ex_reg.rs_1_index) ) && (mem_stage->mem_wb_reg.rd_index == id_stage->id_ex_reg.rs_1_index ) )
	{
		alu_in1 = wb_stage->mux_out;
	}
	else
	{
		alu_in1 = id_stage->id_ex_reg.read_data1;
	}
	//

	// ALU DATA 2
	if(ex_mem_reg.reg_write && (ex_mem_reg.rd_index != 0) && (ex_mem_reg.rd_index == id_stage->id_ex_reg.rs_2_index) )
	{
		alu_in2 = mem_stage->mem_wb_reg.rs_2_index;
	}
	else if(mem_stage->mem_wb_reg.reg_write && (mem_stage->mem_wb_reg.rd_index != 0) && !( ex_mem_reg.reg_write && (ex_mem_reg.rd_index != 0) && (ex_mem_reg.rd_index == id_stage->id_ex_reg.rs_2_index) ) && (mem_stage->mem_wb_reg.rd_index == id_stage->id_ex_reg.rs_2_index ) )
	{
		alu_in2 = wb_stage->mux_out;
	}
	else
	{
		alu_in2 = id_stage->id_ex_reg.read_data2;
	}
	//
		
	ex_mem_reg.WB = id_stage->id_ex_reg.WB;

	ex_mem_reg.rd_index = id_stage->id_ex_reg.rd_index;
	ex_mem_reg.rs_1_index = id_stage->id_ex_reg.rs_1_index;
	ex_mem_reg.rs_2_index = id_stage->id_ex_reg.rs_2_index;

    	ex_mem_reg.read_data2 = id_stage->id_ex_reg.read_data2;
	
	ex_mem_reg.branch = id_stage->id_ex_reg.branch;
	ex_mem_reg.mem_read = id_stage->id_ex_reg.mem_read;
	ex_mem_reg.mem_write = id_stage->id_ex_reg.mem_write;
	ex_mem_reg.reg_write = id_stage->id_ex_reg.reg_write;
	ex_mem_reg.mem_to_reg = id_stage->id_ex_reg.mem_to_reg;

    if (id_stage->id_ex_reg.alu_src)
        alu_in2 = id_stage->id_ex_reg.imm_gen_result;

	alu.set_alu_ops( alu_in1, alu_in2, id_stage->id_ex_reg.alu_op1, id_stage->id_ex_reg.alu_op2, id_stage->id_ex_reg.funct7, id_stage->id_ex_reg.funct3);
	ex_mem_reg.alu_out = alu.get_alu_result();
	ex_mem_reg.alu_zero = alu.get_alu_is_zero();

	ex_mem_reg.add_sum = id_stage->id_ex_reg.PC + id_stage->id_ex_reg.imm_gen_result;

	
	/*
	 * De-bugging
	 * */
	if (DEBUG)
	{
		cout << "EX : " << instr->raw_instr << " | ";	
	}
}

void MEM_Stage::tick()
{
    if (end == 1)
    {
            // Instructions are run out, do nothing.
            return;
    }

    if (ex_stage->ex_mem_reg.valid == 0)
    {
            // EX_MEM register is invalid, do nothing.
            return;
    }
	
	/** FIXME, simulate MEM stage here.  **/
	end = ex_stage->end; // end signal is propagated from IF stage

	instr = ex_stage->instr; // instruction pointer is also propagated from IF stage
	

	mem_wb_reg.reg_write = ex_stage->ex_mem_reg.reg_write;
	mem_wb_reg.mem_to_reg = ex_stage->ex_mem_reg.mem_to_reg;

	if (ex_stage->ex_mem_reg.mem_write) 
	{
		data_memory.write_data(ex_stage->ex_mem_reg.alu_out, ex_stage->ex_mem_reg.read_data2);
	}

    if (ex_stage->ex_mem_reg.mem_read)
    {
        mem_wb_reg.data_mem_read = data_memory.read_data(ex_stage->ex_mem_reg.alu_out);
    }  

	mem_wb_reg.alu_zero = ex_stage->ex_mem_reg.alu_zero;
	mem_wb_reg.alu_out = ex_stage->ex_mem_reg.alu_out;

	mem_wb_reg.valid = ex_stage->ex_mem_reg.valid;
	ex_stage->ex_mem_reg.valid = 0;

	mem_wb_reg.WB = ex_stage->ex_mem_reg.WB;

	mem_wb_reg.rd_index = ex_stage->ex_mem_reg.rd_index;
	mem_wb_reg.rs_1_index = ex_stage->ex_mem_reg.rs_1_index;
	mem_wb_reg.rs_2_index = ex_stage->ex_mem_reg.rs_2_index;


	/*
	 * De-bugging
	 * */
	if (DEBUG)
	{
		cout << "MEM : " << instr->raw_instr << " | ";	
	}
}

void WB_Stage::tick()
{
        if (end == 1)
        {
                // Instructions are run out, do nothing.
                return;
        }

        if (mem_stage->mem_wb_reg.valid == 0)
        {
                // MEM_WB register is invalid, do nothing.
                return;
        }

	
	/*
	 * FIXME, simulate WB stage here.
         **/
	end = mem_stage->end; // end signal is propagated from IF stage
	
	instr = mem_stage->instr; // instruction pointer is also propagated from IF stage
	
	mem_stage->mem_wb_reg.valid = 0; 

	mux_out = (mem_stage->mem_wb_reg.mem_to_reg) ? mem_stage->mem_wb_reg.data_mem_read : mem_stage->mem_wb_reg.alu_out;


	if (mem_stage->mem_wb_reg.reg_write)
	{
		id_stage->registers.assign_reg(mem_stage->mem_wb_reg.rd_index, mux_out);
	}


	if (DEBUG)
	{
        cout << "WB : " << instr->raw_instr << " | ";
	}	
}

