#include <iostream>
#include <cstring>

using namespace std;

class Brain {
public:
	const size_t memorySize = 50;
	unsigned char* memory;
	Brain() { memory = new unsigned char[memorySize]; }
	~Brain() { delete[] memory; }

	void outputMemory(){
		char* newchar = new char[memorySize+1];
		memcpy(newchar, memory, memorySize);
		for (int i = 0; i < memorySize; i++) {
			newchar[i] = (newchar[i] == '\0' ? '0' : newchar[i]);
			newchar[i] = (newchar[i] == -1 ? 'e' : newchar[i]);
		}
		newchar[memorySize] = '\0';

		for (int i = 0; i < memorySize; i++) {
			printf("%02X ", (unsigned char)newchar[i]);
			if (i % 5 == 4) cout << endl;
		}
		cout << endl;
		delete[] newchar;
	}

	void randomizeMemory() {
		for (int i = 0; i < memorySize; i++) {
			memory[i] = rand() % 256;
		}
	}
};

class SimulatedComputer {
public:
	string input;
	string output;
	Brain * brain;
	size_t currentBrainMemoryOffset = 0;
	size_t currentInputOffset = 0;
	size_t inputLength;

	SimulatedComputer(string input, Brain * brain) {
		currentBrainMemoryOffset = 0;
		currentInputOffset = 0;
		this->input = input;
		inputLength = input.length();
		this->brain = brain;
		output = "";
	}

	size_t addressBufferToPointer(unsigned char * addressBuffer, size_t addressSize) {
		size_t ptr = 0;
		if (addressSize > sizeof(size_t)) return -1;
		for (size_t i = 0; i < addressSize; i++) {
			ptr <<= 8;
			ptr += addressBuffer[i];
		}
		return ptr;
	}

	size_t readAddress(unsigned char * &address, size_t offset) {
		size_t addressSize = 1;
		while (brain->memory[currentBrainMemoryOffset] == 0xFF) {
			addressSize *= 2;
			if (offset + addressSize >= brain->memorySize) { address = nullptr; return 0; }
			offset++;
		}
		address = new unsigned char[addressSize];
		memcpy(address, brain->memory, addressSize);
		return addressSize;
	}

	char readByte() {
		char toReturn = input.at(currentInputOffset);
		currentInputOffset++;
		return toReturn;
	}

	void writeByte(char byte) {
		output += byte;
	}

	string execute() {
		while (currentBrainMemoryOffset < brain->memorySize) {
			if (brain->memory[currentBrainMemoryOffset] == 0) {
				currentBrainMemoryOffset++;
			}
			else if (brain->memory[currentBrainMemoryOffset] == 1) {
				if (currentBrainMemoryOffset + 1 >= brain->memorySize) break;
				unsigned char * address;
				size_t addressSize = readAddress(address, currentBrainMemoryOffset + 1);
				if (addressSize == 0) break;
				size_t add = addressBufferToPointer(address, addressSize);
				brain->memory[add] = readByte();
				currentBrainMemoryOffset += 1 + (addressSize*(addressSize + 1)) / 2;
			}
			else if (brain->memory[currentBrainMemoryOffset] == 2) {
				if (currentBrainMemoryOffset + 1 >= brain->memorySize) break;
				unsigned char * address;
				size_t addressSize = readAddress(address, currentBrainMemoryOffset + 1);
				if (addressSize == 0) break;
				size_t add = addressBufferToPointer(address, addressSize);
				writeByte(brain->memory[add]);
				currentBrainMemoryOffset += 1 + (addressSize*(addressSize+1))/2;
			}
			else currentBrainMemoryOffset++;
		}
		return output;
	}
};

int main() {
	Brain brain;

	int maxi = 50;
	for (int i = 0; i < maxi; i++) {
		brain.randomizeMemory();
		SimulatedComputer * pc = new SimulatedComputer("apple", &brain);
		string output = pc->execute();
		delete pc;
		if (output.length() == 0) { maxi++; cout << maxi << "\r";  continue; }
		cout << output.c_str() << endl;
	}

	return 0;
}
