#include "SimpleInterpreter.h"
#include <iostream>
#include <exception>

#define DEFAULT_MEMORY_SIZE 1024
#define DEFAULT_ARCHITECTURE 16

SimpleInterpreter::SimpleInterpreter(int memorySize = DEFAULT_MEMORY_SIZE, int architecture = DEFAULT_ARCHITECTURE) : memorySize(memorySize), architecture(architecture)
{
	memory = new unsigned char[memorySize];
	registerSize = (architecture/8) * 1;
}


SimpleInterpreter::~SimpleInterpreter()
{
	delete[] memory;
}

void SimpleInterpreter::setInstructionPointer(size_t value) {
	int byteArch = architecture / 8;
	memcpy(memory - byteArch, &value, byteArch);
}

char * SimpleInterpreter::parseProgram(unsigned char * program, size_t programSize, unsigned char * input, size_t inputSize)
{
	if (inputSize > memorySize) throw new std::exception("Input too large.");
	if (programSize > memorySize) throw new std::exception("Program too large.");
	if (programSize + inputSize > memorySize) throw new std::exception("Program + Input too large.");

	memset(memory, 0, memorySize);
	memcpy(memory, input, inputSize);
	memcpy(memory+memorySize-registerSize-programSize, program, programSize);
	setInstructionPointer(memorySize-registerSize-programSize);

	return (char*)memory;
}
