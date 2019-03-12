#include "Stages.h"

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
	if (stall == 0)
	{
		// Get Instruction
		Instruction &instruction = instr_mem->get_instruction(PC);

		// Increment PC
		// TODO, PC should be incremented or decremented based on instruction
		if_id_reg.addr = PC;
		PC += 4;

		/*
		 * TODO, fix me. Simulate IF Stage here.
		 * Example on how to extract fields have been given.
		 * */
		if_id_reg.valid = 1;
		if_id_reg.instr = instruction.instruction;

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
	if (ex_stage->ex_mem_reg.valid == 1)
	{
		if (ex_stage->ex_mem_reg.rd_index == id_ex_reg.rs_1_index || 
			ex_stage->ex_mem_reg.rd_index == id_ex_reg.rs_2_index)
		{
			if_stage->stall = 1; // Fetching should not proceed.
			stall = 1; // ID should also stall.
			ex_stage->bubble = 1; // EX stage should not accept any new instructions

			instr->end_exe += 1; // The end execution time should be incremented by 1.

			return;
		}
	}
	else if (mem_stage->mem_wb_reg.valid == 1)
	{
		if (mem_stage->mem_wb_reg.rd_index == id_ex_reg.rs_1_index || 
			mem_stage->mem_wb_reg.rd_index == id_ex_reg.rs_2_index)
		{
			if_stage->stall = 1; // Fetching should not proceed.
			stall = 1; // ID should also stall.
			ex_stage->bubble = 1; // EX stage should not accept any new instructions

			instr->end_exe += 1; // The end execution time should be incremented by 1.
			
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
	id_ex_reg.rd_index = (if_stage->if_id_reg.instr >> 7) & 31;
	id_ex_reg.rs_1_index = (if_stage->if_id_reg.instr >> (7 + 5 + 3)) & 31;
	id_ex_reg.rs_2_index = (if_stage->if_id_reg.instr >> (7 + 5 + 3 + 5)) & 31;
	id_ex_reg.data1 = regs->read_reg((int)id_ex_reg.rs_1_index);
	id_ex_reg.data2 = regs->read_reg((int)id_ex_reg.rs_2_index);
	id_ex_reg.addr = if_stage->if_id_reg.addr;
	
	uint8_t op_code = if_stage->if_id_reg.instr & 0x007F;
	control->set_op_code(op_code);
        imm_gen->set_imm_gen(op_code, if_stage->if_id_reg.instr);
        imm_gen_result = imm_gen->get_imm_gen_result();	
	
        id_ex_reg.jalr = control->get_jalr();
        id_ex_reg.jump = control->get_jump();
	id_ex_reg.branch = control->get_branch();
	id_ex_reg.mem_read = control->get_mem_read();
	id_ex_reg.mem_to_reg = control->get_mem_to_reg();
	id_ex_reg.alu_op_0 = control->get_alu_op_0();
	id_ex_reg.alu_op_1 = control->get_alu_op_1();
	id_ex_reg.mem_write = control->get_mem_write();
	id_ex_reg.alu_src = control->get_alu_src();
	id_ex_reg.imm_gen = imm_gen_result;
	id_ex_reg.funct7 = (if_stage->if_id_reg.instr >> 25) & 0x7F;
	id_ex_reg.funct3 = (if_stage->if_id_reg.instr >> 12) & 0x7;


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

        if (id_stage->id_ex_reg.jump) {

            //registers->assign_reg(write_register, PC+4);
            ex_mem_reg.write_reg_addr = id_stage->id_ex_reg.imm_gen;
	    
        }
        if (id_stage->id_ex_reg.jalr) {
            PC = registers->read_reg(read_register1);
            PC += registers->read_reg(write_register);
            PC += imm_gen_result;
        }

	int64_t mux_read_data2;
        else {
            // ALU Mux
            if (id_stage->id_ex_reg.alu_src) {
                mux_read_data2 = id_ex_reg.imm_gen;
            }
            else
            {
                mux_read_data2 = id_ex_reg.data2;
            }


            alu->set_alu_ops( id_stage->id_ex_reg.data1, mux_read_data2, id_stage->id_ex_reg.alu_op_0, id_stage->id_ex_reg.alu_op_1, id_stage->id_ex_reg.funct7, id_stage->id_ex_reg.funct3);

	ex_mem_reg.valid = 1;
	ex_mem_reg.branch = id_stage->id_ex_reg.branch;
	ex_mem_reg.mem_read = id_stage->id_ex_reg.mem_read;
	ex_mem_reg.mem_write = id_stage->id_ex_reg.mem_write;
	ex_mem_reg.reg_write = id_stage->id_ex_reg.reg_write;
	ex_mem_reg.mem_to_reg = id_stage->id_ex_reg.mem_to_reg;
	ex_mem_reg.add_sum = id_stage->id_ex_reg.addr + id_stage->id_ex_reg.imm_gen;
	ex_mem_reg.alu_zero = alu->get_alu_is_zero();
	ex_mem_reg.alu_out = alu->get_alu_result();
	ex_mem_reg.write_reg_addr = id_stage->id_ex_reg.rd_index;
	ex_mem_reg.read_reg2 = id_stage->id_ex_reg.data2;
	}



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
	
	/*
	 * FIXME, simulate MEM stage here.
         **/
	end = ex_stage->end; // end signal is propagated from IF stage

	instr = ex_stage->instr; // instruction pointer is also propagated from IF stage
	
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

	if (DEBUG)
	{
		cout << "WB : " << instr->raw_instr << " | ";
	}	
}

