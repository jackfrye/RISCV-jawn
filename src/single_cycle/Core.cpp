#include "Core.h"

Core::Core(const string &fname, ofstream *out) : out(out), 
						clk(0), 
						PC(0),
						instr_mem(new Instruction_Memory(fname))
{
	registers = new Registers();
	control = new Control();
	alu = new Algo_Logic_Unit();
    imm_gen = new Imm_gen();
}

/*
	TODO - Add more functions and modify tick() to simulate single-cycle RISC-V architecture
*/
bool Core::tick()
{
	/*
		Step One: Serving pending instructions
	*/
	if (pending_queue.size() > 0)
	{
		serve_pending_instrs();
	}
	
	/*
		Step Two: Where simulation happens
	*/
	if (PC <= instr_mem->last_addr())
	{
		// Get Instruction
		Instruction &instruction = instr_mem->get_instruction(PC);

		// Increment PC
		// TODO, PC should be incremented or decremented based on instruction
		read_register1 = (instruction.instruction >> 15) & 0x1F;
		read_register2 = (instruction.instruction >> 20) & 0x1F;
		write_register = (instruction.instruction >> 7) & 0x1F;
		read_data1 = registers->read_reg((int)read_register1);
		read_data2 = registers->read_reg((int)read_register2);


		op_code = instruction.instruction & 0x00FF;
		control->set_op_code(op_code);
		
		branch = control->get_branch();
		mem_read = control->get_mem_read();
		mem_to_reg = control->get_mem_to_reg();
		alu_op_0 = control->get_alu_op_0();
		alu_op_1 = control->get_alu_op_1();
		mem_write = control->get_mem_write();
		alu_src = control->get_alu_src();
		reg_write = control->get_reg_write();

        funct7 = (instruction.instruction >> 25) & 0x7F;
        funct3 = (instruction.instruction >> 12) & 0x7;

        imm_gen->set_imm_gen(op_code, instruction.instruction);
        imm_gen_result = imm_gen->get_imm_gen_result();

        if (alu_src) {
            read_data2 = imm_gen_result;
        }

		alu->set_alu_ops( read_data1, read_data2, alu_op_0, alu_op_1, funct7, funct3);

		PC += 4;

		/*
			Step Three: Simulator related
		*/
		instruction.begin_exe = clk;
		
		// Single-cycle always takes one clock cycle to complete
		instruction.end_exe = clk + 1; 
	
		pending_queue.push_back(instruction);
	}

	clk++;

	/*
		Step Four: Should we shut down simulator
	*/
	if (pending_queue.size() == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Core::serve_pending_instrs()
{
	list<Instruction>::iterator instr = pending_queue.begin();

	if (instr->end_exe <= clk)
	{
		printStats(instr);
		
		pending_queue.erase(instr);	
	}
}

void Core::printStats(list<Instruction>::iterator &ite)
{
	*out << ite->raw_instr << " => ";
	*out << "Core ID: " << id << "; ";
	*out << "Begin Exe: " << ite->begin_exe << "; ";
	*out << "End Exe: " << ite->end_exe << endl;
}

