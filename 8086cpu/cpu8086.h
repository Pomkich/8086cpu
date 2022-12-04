#pragma once 
#include "Constants.h"
#include <map>
#include <functional>
#include <memory>
#include "Memory.h"

static enum class Flag { O = 11, D = 10, I = 9, T = 8, S = 7, Z = 6, A = 4, P = 2, C = 0 };	// bits of flags register

// замена текста препроцессором для более удобного
// обращения к верхней и нижней частей регистров
#define L low_high[0]
#define H low_high[1]

union reg {
	word X;
	byte low_high[2];
};

class cpu8086 {
public:	// private
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

	byte opcode;		// переменная хранящая команду
	dword instr_adr;	// переменная хранящая физический адрес команды
	dword stack_adr;	// переменная хранящая физический адрес стека

	// указатель на физическое адресное пространство
	std::shared_ptr<Memory> memory;

public:
	cpu8086();
	void reset();	// сброс
	void clock();	// выполнение одной команды
	void initOpTable();
	void initMemory(std::shared_ptr<Memory> mem);

public: // private
	// фунции для проверки флагов
	void testFlagZ(word& src_op);
	void testFlagSB(byte& src_op);	// для работы с байтами
	void testFlagSW(word& src_op);	// для работы со словами
	void testFlagP(byte val);
	void testFlagCAddB(byte prev_val, byte& src_op);	// для работы с байтами
	void testFlagCSubB(byte prev_val, byte& src_op);	
	void testFlagCAddW(word prev_val, word& src_op);	// для работы со словами
	void testFlagCSubW(word prev_val, word& src_op);
	void testFlagAAdd(word prev_val, word& src_op);
	void testFlagASub(word prev_val, word& src_op);
	void testFlagO(bool prev_sig_bit, bool now_sig_bit);

	// функции работы с флагами
	bool getFlag(Flag f);
	void setFlag(Flag f);
	void remFlag(Flag f);

	// функции-помощники адресации
	word fetchEA(byte MOD, byte RM, word disp);	// получение эффективного адреса в зависимости от типа адресации
	byte& getRegB(byte reg);	// декодирование 8-битного регистра
	word& getRegW(byte reg);	// декодирование 16-битного регистра

private:
	void ADD_R_IN_B();		// сложение в байтовый регистр
	void ADD_R_OUT_B();		// сложение из байтового регистра
	void ADD_R_IN_W();		// сложение в 16-битный регистр
	void ADD_R_OUT_W();		// сложение из 16-битного регистра
	void INC_R(word& rgs);
	void DEC_R(word& rgs);
	void PUSH_R(word& rgs);
	void POP_R(word& rgs);
};