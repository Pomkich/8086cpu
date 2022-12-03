#pragma once
#include "Constants.h"
#include <cstring>
#include <iostream>

class Memory {
private:
	byte memory[memory_size];

public:
	Memory();
	byte read(dword address);
	void writeB(dword address, byte val);
	void writeW(dword address, word val);
};