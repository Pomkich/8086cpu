#pragma once
#include "Constants.h"
#include <cstring>

class Memory {
private:
	byte memory[memory_size];

public:
	Memory();
	byte read(dword address);
	void write(dword address, byte val);
};