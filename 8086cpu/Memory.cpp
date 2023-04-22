#include "Memory.h"

Memory::Memory() {
	memset(memory, 0, memory_size);
	presenter = new EmptyPresenter();
}

void Memory::reset() {
	memset(memory, 0, memory_size);
	presenter->notifyMemChange();
	presenter->notifyStkChange();
}

void Memory::initPresenter(AbstractPresenter* p_pres) {
	presenter = p_pres;
}

byte Memory::readB(dword address) {
	return memory[address];
}

word Memory::readW(dword address) {
	return ((memory[address + 1] << 8) | memory[address]);
}

void Memory::writeB(dword address, byte val) {
	memory[address] = val;
	presenter->notifyMemChange();
}

void Memory::writeW(dword address, word val) {
	// NOTE: ������� �������� �������� �� ����� �� �������
	memory[address] = (val & 0x00FF);		// ������ ����� �����
	memory[address + 1] = (val >> 8);		// ������� ����� �����
	presenter->notifyMemChange();
}