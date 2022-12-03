#include "Memory.h"

Memory::Memory() {
	memset(memory, 0, memory_size);
}

byte Memory::read(dword address) {
	return memory[address];
}

void Memory::writeB(dword address, byte val) {
	memory[address] = val;
}

void Memory::writeW(dword address, word val) {
	// NOTE: ������� �������� �������� �� ����� �� �������
	memory[address] = (val & 0x00FF);		// ������ ����� �����
	memory[address + 1] = (val >> 8);		// ������� ����� �����
}