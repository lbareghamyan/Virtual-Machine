#include <iostream>
#include "VirtualMachine.h"
void createBinaryFile(std::string filename)
{
	std::ofstream fs(filename, std::ios::out | std::ios::binary);
	char code[] = { 0,0,9,0 };
	for (int i = 0; i < 20; i++)
	{
		fs.write((char*)&code[i], sizeof(char));
	}
	fs.close();
}

int main(int argc, char* argv[])
{
	if (argc < 2) 
	{
		std::cerr << "Not enough arguments are provided" << std::endl;
		return 1;
	}
	//createBinaryFile("exe.bin");
	std::string filename(argv[1]);
	VirtualMachine v(filename, &std::cin, &std::cout);
	v.run();
	v.print();
	return 0;
}