#include "Memory.h"

Memory::Memory() {
	memset(memory, 0, memory_size);
}

byte Memory::read(dword address) {
	return memory[address];
}

void Memory::write(dword address, byte val) {
	memory[address] = val;
}