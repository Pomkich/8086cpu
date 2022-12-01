#include "GenReg.h"

reg::reg() {
	X = 0;
	H = 0;
	L = 0;
}

void reg::setX(word new_X) {
	X = new_X;
	H = (X >> 8);
	L = X;
}

void reg::setH(byte new_H) {
	H = new_H;
	X &= 0x00FF;	// �������� �������� � ������� �����
	X |= ((word)H << 8);	// ���������� �����
}

void reg::setL(byte new_L) {
	L = new_L;
	X &= 0xFF00;	// �������� �������� � ������ �����
	X |= L;		// ���������� �����
}

word reg::getX() { return X; };
byte reg::getH() { return H; };
byte reg::getL() { return L; };