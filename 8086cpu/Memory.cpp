#include "Memory.h"

Memory::Memory() {
	memset(memory, 0, memory_size);
}

byte Memory::readB(dword address) {
	return memory[address];
}

word Memory::readW(dword address) {
	return ((memory[address + 1] << 8) | memory[address]);
}

word Memory::readStack(dword address) {
	return ((memory[address - 1] << 8) | memory[address]);
}

void Memory::writeB(dword address, byte val) {
	memory[address] = val;
}

void Memory::writeW(dword address, word val) {
	// NOTE: следует добавить проверки на выход за пределы
	memory[address] = (val & 0x00FF);		// нижняя часть слова
	memory[address + 1] = (val >> 8);		// верхняя часть слова
}

void Memory::writeStack(dword address, word val) {
	memory[address] = (val & 0x00FF);
	memory[address - 1] = (val >> 8);
}