#ifndef __DATA_MEMORY_H_
#define __DATA_MEMORY_H_

#include <vector>
#include <map>
#include <bitset>
#include <fstream>

using namespace std;

class Data_Memory
{
public:
	Data_Memory();
	void write_data(long address, int _data);
	int read_data(long address);
	void print_mem(ofstream *out);
private:
	map<long, int> data;
};

#endif
