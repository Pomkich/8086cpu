#pragma once
#include "Constants.h"
#include <cstring>
#include <iostream>

class Memory {
private:
	byte memory[memory_size];

public:
	Memory();
	byte readB(dword address);
	word readW(dword address);
	word readStack(dword address);
	void writeB(dword address, byte val);
	void writeW(dword address, word val);
	void writeStack(dword address, word val);
};