#include <iostream>
#include "VirtualMachine.h"

int main(int argc, char* argv[])
{
	if (argc < 2) 
	{
		std::cerr << "Not enough arguments are provided" << std::endl;
		return 1;
	}
	std::string filename(argv[1]);
	VirtualMachine v(filename, &std::cin, &std::cout);
	v.run();
	v.print();
	return 0;
}