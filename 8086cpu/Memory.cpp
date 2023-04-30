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

void Memory::loadProgram(int start_address, std::string file_name) {
	std::ifstream program(file_name, std::ios::binary);
	program.seekg(0, std::ios::end);
	size_t length = program.tellg();
	program.seekg(0, std::ios::beg);
	program.unsetf(std::ios::skipws);
	for (int i = start_address; i < length + start_address; i++) {
		program >> memory[i];
	}
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
	// NOTE: следует добавить проверки на выход за пределы
	memory[address] = (val & 0x00FF);		// нижняя часть слова
	memory[address + 1] = (val >> 8);		// верхняя часть слова
	presenter->notifyMemChange();
}