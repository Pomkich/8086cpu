#pragma once 
#include "GenReg.h"
#include "Constants.h"

static enum class Flag { O = 11, D = 10, I = 9, T = 8, S = 7, Z = 6, A = 4, P = 2, C = 0 };	// bits of flags register

class cpu8086 {
public:
	// регистры общего назначения
	reg A, B, C, D;
	// регистры указателей и индексов
	word SP, BP, SI, DI;
	// регистры начала сегментов
	word CS, DS, SS, ES;
	// указатель на инструкцию
	word IP;
	// регистр флагов
	word flag_reg;

	cpu8086() {

	}
public:
	// фунции для проверки флагов
	void testFlagZ(byte& src_op);
	void testFlagS(byte& src_op);	// для работы с байтами
	void testFlagS(word& src_op);	// для работы со словами
	void testFlagP(byte& src_op);
	void testFlagCAdd(word prev_val, word& src_op);
	void testFlagCSub(word prev_val, word& src_op);
	void testFlagAAdd(word prev_val, word& src_op);
	void testFlagASub(word prev_val, word& src_op);

	// функции работы с флагами
	bool getFlag(Flag f);
	void setFlag(Flag f);
	void remFlag(Flag f);
};