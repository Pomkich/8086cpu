#pragma once 
#include "Constants.h"
#include <map>
#include <functional>

static enum class Flag { O = 11, D = 10, I = 9, T = 8, S = 7, Z = 6, A = 4, P = 2, C = 0 };	// bits of flags register

// замена текста препроцессором для более удобного
// обращения к верхней и нижней частей регистров
#define L HL[0]
#define H HL[1]

union reg {
	word X;
	byte HL[2];
};

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

	// таблица команд
	std::map<byte, std::function<void()>> opcode_table;

	// переменная для выбора и выполнения команды
	byte opcode;

public:
	cpu8086();
	void reset();	// сброс
	void clock();	// выполнение одной команды
	void initOpTable();

	// фунции для проверки флагов
	void testFlagZ(word& src_op);
	void testFlagS(byte& src_op);	// для работы с байтами
	void testFlagS(word& src_op);	// для работы со словами
	void testFlagP(byte val);
	void testFlagCAdd(byte prev_val, byte& src_op);	// для работы с байтами
	void testFlagCSub(byte prev_val, byte& src_op);	
	void testFlagCAdd(word prev_val, word& src_op);	// для работы со словами
	void testFlagCSub(word prev_val, word& src_op);
	void testFlagAAdd(word prev_val, word& src_op);
	void testFlagASub(word prev_val, word& src_op);
	void testFlagO(bool prev_sig_bit, bool now_sig_bit);

	// функции работы с флагами
	bool getFlag(Flag f);
	void setFlag(Flag f);
	void remFlag(Flag f);

private:
	void INC_R(word& rgs);
};