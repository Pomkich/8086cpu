#pragma once
#include "Constants.h"

// класс регистра общего назначения
class reg {
private:
	word X;
	byte H;
	byte L;

public:
	reg();

	void setX(word new_X);
	void setH(byte new_H);
	void setL(byte new_L);

	word getX();
	byte getH();
	byte getL();
};