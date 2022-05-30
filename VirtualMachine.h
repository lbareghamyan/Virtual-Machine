#pragma once
#include <vector>
#include <iostream>
#include <fstream>
struct Instruction 
{
	uint8_t opcode;
	uint8_t op1;
	uint8_t op2;
	uint8_t dest;
};
class VirtualMachine
{
public:
	VirtualMachine(std::string, std::istream*, std::ostream*);
	void run();
	void print();
	
private:
	static const int REG_NUMBER = 8;
	static const int InOutNum = REG_NUMBER + 1; // input output register number

	//8-bit registers
	unsigned char m_reg[REG_NUMBER] = {0};

	int m_counter;

	std::istream* m_in = nullptr;
	std::ostream* m_out = nullptr;

	std::vector<Instruction> code;

	bool readFile(std::string);

	unsigned char ALU(uint8_t,char,char);
	int Conditionals(Instruction);
};
