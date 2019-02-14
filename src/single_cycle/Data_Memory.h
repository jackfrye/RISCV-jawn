#ifndef __DATA_MEMORY_H_
#define __DATA_MEMORY_H_

#include <vector>
#include <map>
#include <bitset>

class Data_Memory
{
public:
	Data_Memory();
	void write_data(long address, unsigned int _data);
	unsigned int read_data(long address);
private:
	map<long, unsigned int> data;
}

#endif