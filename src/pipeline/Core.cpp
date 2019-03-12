#include "Core.h"

Core::Core(const string &fname, ofstream *out) : out(out), 
						clk(0) 
{
	/* Initialize global core components */
	registers = new Registers();
	control = new Control();
	alu = new Algo_Logic_Unit();
    	imm_gen = new Imm_gen();
    	data_memory = new Data_Memory();
	registers->assign_reg(0,0);

	/*
	 * Initialize Stages
	 * */
	if_stage = (new IF_Stage(fname, this));	
	id_stage = (new ID_Stage(registers, control, imm_gen));
	ex_stage = (new EX_Stage());
	mem_stage = (new MEM_Stage());
	wb_stage = (new WB_Stage());

	wb_stage->mem_stage = mem_stage;
	wb_stage->id_stage = id_stage;

	mem_stage->ex_stage = ex_stage;
	
	ex_stage->id_stage = id_stage;
	ex_stage->if_stage = if_stage;
	
	id_stage->if_stage = if_stage;
	id_stage->ex_stage = ex_stage;
	id_stage->mem_stage = mem_stage;
}	

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
	if (DEBUG)
	{
		cout << "clk: " << clk << " : ";
	}

	wb_stage->tick();
	mem_stage->tick();
	ex_stage->tick();
	id_stage->tick();	
	if_stage->tick();
	
	if (DEBUG)
	{
		cout << endl;
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

