
#include <vector>
#include <map>
#include <bitset>
#include <iostream>

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

void Data_Memory::print_mem(ofstream *out)
{
	for(map<long, unsigned int>::iterator it = data.begin(); it != data.end(); it++)
	{
		bitset<32> x1(it->second);
		*out << it->first << ": " << x1 << endl;
	}
}
