
#include <vector>
#include <map>
#include <bitset>

#include "Data_Memory.h"

using namespace std;

Data_Memory::Data_Memory()
{
}

void Data_Memory::write_data(long address, unsigned int _data)
{
	data[address] = _data;
}

unsigned int Data_Memory::read_data(long address)
{

	return data.find(address)->second;
}
