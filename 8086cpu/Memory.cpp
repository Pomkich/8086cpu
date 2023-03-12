#include "Memory.h"

Memory::Memory() {
	memset(memory, 0, memory_size);
}

void Memory::reset() {
	memset(memory, 0, memory_size);
	if (presenter != nullptr) {
		presenter->notifyMemChange();
		presenter->notifyStkChange();
	}
}

void Memory::initPresenter(std::shared_ptr<ConsolePresenter> p_pres) {
	presenter = p_pres;
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
	if (presenter != nullptr) {
		presenter->notifyMemChange();
	}
}

void Memory::writeW(dword address, word val) {
	// NOTE: следует добавить проверки на выход за пределы
	memory[address] = (val & 0x00FF);		// нижняя часть слова
	memory[address + 1] = (val >> 8);		// верхняя часть слова
	if (presenter != nullptr) {
		presenter->notifyMemChange();
	}
}

void Memory::writeStack(dword address, word val) {
	memory[address] = (val & 0x00FF);
	memory[address - 1] = (val >> 8);
	if (presenter != nullptr) {
		presenter->notifyStkChange();
	}
}