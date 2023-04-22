#pragma once
#include "Constants.h"
#include <cstring>
#include <iostream>
#include "AbstractPresenter.h"
#include "ConsolePresenter.h"

class ConsolePresenter;

class Memory {
private:
	byte memory[memory_size];
	AbstractPresenter* presenter;

public:
	Memory();
	void reset();
	void initPresenter(AbstractPresenter* p_pres);

	byte readB(dword address);
	word readW(dword address);
	void writeB(dword address, byte val);
	void writeW(dword address, word val);
};