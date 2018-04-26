#pragma once
class SimpleInterpreter
{
private:
	unsigned char * memory;		//Memory layout: [working memory - input and output--->]...[<---program memory][registers]
	size_t memorySize;			//Default 1024 bytes
	int architecture;			//Number of bytes a "pointer" is wide - usually 16
	size_t registerSize;		//Number of bytes the "registers" occupy at the end of the memory
public:
	SimpleInterpreter(int memorySize, int architecture);
	~SimpleInterpreter();
	void setInstructionPointer(size_t value);
	char* parseProgram(unsigned char * program, size_t programSize, unsigned char * input, size_t inputSize);
};

