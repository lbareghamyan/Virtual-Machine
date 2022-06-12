#include "VirtualMachine.h"

VirtualMachine::VirtualMachine(std::string filePath, std::istream* in, std::ostream* out)
	:m_counter(0), m_in(in), m_out(out)
{
	readFile(filePath);
}

void VirtualMachine::run()
{
	
	for (int i = 0; i < code.size(); i = m_counter)
	{
		uint8_t a = 0, b = 0;
		//check if 7th or 6th bits are set for immediate values
		if ((code[i].opcode >> 7 & 1))
		{
			a = code[i].op1;
			code[i].opcode = code[i].opcode >> 7 & 0;
		}
		else if (code[i].op1 == InOutNum)//check if op1 is the input reg
		{
			int k;
			*m_in >> k;
			a = k;
		}
		else if (code[i].op1 >= 0 && code[i].op1 <= REG_NUMBER)//if op1 is a register
		{
			a = m_reg[code[i].op1];
		}
		if ((code[i].opcode >> 6 & 1))
		{
			b = code[i].op2;
			code[i].opcode = code[i].opcode >> 6 & 0;
		}
		else if (code[i].op2 == InOutNum)//check if op2 is the input reg
		{
			int k;
			*m_in >> k;
			b = k;
		}
		else if (code[i].op2 >= 0 && code[i].op2 <= REG_NUMBER)//if op2 is a register
		{
			b = m_reg[code[i].op2];
		}
		if (((code[i].opcode) >> 5) & 1)//check if 5th(consditional) bit is set
		{
			m_counter = Conditionals({ code[i].opcode, a, b, code[i].dest });
		}
		else
		{
			if (code[i].dest < REG_NUMBER)
			{
				m_reg[code[i].dest] = ALU(code[i].opcode, a, b);
			}
			else if (code[i].dest == InOutNum)//check if dest is the output register
			{
				(*m_out) << (int)ALU(code[i].opcode, a, b) << std::endl;
			}
			
		}
		m_counter++;
	}
}

void VirtualMachine::print()
{
	for (int i = 0; i < REG_NUMBER; i++)
	{
		std::cout << "R" << i << "\t";
	}
	std::cout << "Counter";
	std::cout << std::endl;
	for (int i = 0; i < REG_NUMBER; i++)
	{
		std::cout << (short)m_reg[i] << "\t";
	}
	std::cout << m_counter * 4 << std::endl;
	
}

bool VirtualMachine::readFile(std::string filePath)
{

	std::ifstream binaryFile;
	binaryFile.open(filePath, std::ios::in | std::ios::binary);

	if (!binaryFile.is_open())
	{
		//cannot open the file error
		throw std::runtime_error("Could not open the file!");
		return false;
	}
	uint8_t temp[4];
	do {

		binaryFile.read((char*)&temp, 4 * sizeof(uint8_t));
		Instruction instruction{ temp[3],temp[2],temp[1],temp[0] };
		code.push_back(instruction); 

	} while (binaryFile.good());
	if (binaryFile.bad() || !binaryFile.eof()) 
	{
		throw std::runtime_error("Fatal error accured!");
		exit(1);
	}
	binaryFile.close();
	return true;
}

// op1 and op2 are immediate values
unsigned char VirtualMachine::ALU(uint8_t opcode, char op1, char op2)
{
	switch (opcode)
	{
	case 0://add
		return op1 + op2;
	case 1://sub
		return op1 - op2;
	case 2://and
		return op1 & op2;
	case 3://or
		return op1 | op2;
	case 4://not, ignores 2nd parameter
		return ~op1;
	case 5://xor
		return op1 ^ op2;
	default:
		throw std::invalid_argument("Unkown ALU instruction!");
		return ' ';
		break;
	}
}

int VirtualMachine::Conditionals(Instruction inst)
{
	switch (inst.opcode)
	{
	case 32:
		if (inst.op1 == inst.op2)
		{
			return inst.dest - 1;//change counter value
		}
		break;
	case 33:
		if (inst.op1 != inst.op2)
		{
			return inst.dest - 1;//change counter value
		}
		break;
	case 34:
		if (inst.op1 < inst.op2)
		{
			return inst.dest - 1;//change counter value
		}
		break;
	case 35:
		if (inst.op1 <= inst.op2)
		{
			return inst.dest - 1;//change counter value
		}
		break;
	case 36:
		if (inst.op1 > inst.op2)
		{
			return inst.dest - 1;//change counter value
		}
		break;
	case 37:
		if (inst.op1 >= inst.op2)
		{
			return inst.dest - 1;//change counter value
		}
		break;
	default:
		throw std::invalid_argument("Unkown Conditional instruction!");
		break;
	}
	return m_counter;
}
