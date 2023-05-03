#pragma once 
#include "Constants.h"
#include <map>
#include <functional>
#include <memory>
#include "Memory.h"
#include "AbstractPresenter.h"
#include "ConsolePresenter.h"
#include <sstream>

class ConsolePresenter;

// замена текста препроцессором для более удобного
// обращения к верхней и нижней частей регистров
#define _ITERATOR_DEBUG_LEVEL 0
#define _CONTAINER_DEBUG_LEVEL 0
#define L low_high[0]
#define H low_high[1]

union reg {
	word X;
	byte low_high[2];
};

class cpu8086 {
private:
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
	dword address;			// вычисленный адрес

	// указатель на физическое адресное пространство
	std::shared_ptr<Memory> memory;
	// указатель на презентатор
	AbstractPresenter* presenter;

	friend class Tester;

public:
	cpu8086();
	void reset();	// сброс
	void clock();	// выполнение одной команды
	void initOpTable();
	void initMemory(std::shared_ptr<Memory> mem);
	void initPresenter(AbstractPresenter* p_pres);
	void loadTestProgram();
	// функции работы с регистрами
	word getRegVal(RegId reg_id);
	void setRegVal(RegId reg_id, word value);	
	// функции работы с флагами
	bool getFlag(Flag f);
	void setFlag(Flag f);
	void remFlag(Flag f);

private:
	// фунции для проверки флагов
	void testFlagZ(word new_val);
	void testFlagS(word new_val, OpType type);
	void testFlagP(byte val);
	void testFlagCAdd(word prev_val, word new_val);	// для работы со словами
	void testFlagCSub(word prev_val, word new_val);
	void testFlagAAdd(word prev_val, word new_val);
	void testFlagASub(word prev_val, word new_val);
	void testFlagO(word prev_val, word new_val, OpType type);

	// функции-помощники адресации
	word fetchEA(byte mod, byte rm, word disp);	// получение эффективного адреса в зависимости от типа адресации
	word fetchDisp(byte mod, byte rm);			// получение смещения
	byte fetchCodeByte();	// получение следующего байта в сегменте кода
	word fetchCodeWord();	// получение следующего слова в сегменте кода
	void fetchModRegRm(byte& mod, byte& reg, byte& rm);	// выделение полей из байта
	byte& getRegB(byte reg);	// декодирование 8-битного регистра
	word& getRegW(byte reg);	// декодирование 16-битного регистра
	word& getSegReg(byte reg);	// декодирование сегментного регистра

private:
	void ADD_R_OUT_B();		// сложение из байтового регистрар
	void ADD_R_OUT_W();		// сложение из 16-битного регистра
	void ADD_R_IN_B();		// сложение в байтовый регистр
	void ADD_R_IN_W();		// сложение в 16-битный регист
	void ADD_A_B();
	void ADD_A_W();
	void OR_R_OUT_B();		// логическое сложение из байтового регистрар
	void OR_R_OUT_W();		// логическое сложение из 16-битного регистра
	void OR_R_IN_B();		// логическое сложение в байтовый регистр
	void OR_R_IN_W();		// логическое сложение в 16-битный регист
	void OR_A_B();
	void OR_A_W();
	void ADC_R_OUT_B();		// сложение вместе с C битом
	void ADC_R_OUT_W();
	void ADC_R_IN_B();
	void ADC_R_IN_W();
	void ADC_A_B();
	void ADC_A_W();
	void SBB_R_OUT_B();		// вычитание вместе с C битом
	void SBB_R_OUT_W();
	void SBB_R_IN_B();
	void SBB_R_IN_W();
	void SBB_A_B();
	void SBB_A_W();
	void AND_R_OUT_B();		// логическое умножение из байтового регистрар
	void AND_R_OUT_W();		// логическое умножение из 16-битного регистра
	void AND_R_IN_B();		// логическое умножение в байтовый регистр
	void AND_R_IN_W();		// логическое умножение в 16-битный регист
	void AND_A_B();
	void AND_A_W();
	void DAA();				// decimal adjust for addition
	void SUB_R_OUT_B();		// вычитание
	void SUB_R_OUT_W();
	void SUB_R_IN_B();
	void SUB_R_IN_W();
	void SUB_A_B();
	void SUB_A_W();
	void DAS();				// decimal adjust for substraction
	void XOR_R_OUT_B();		// логическое исключение
	void XOR_R_OUT_W();		
	void XOR_R_IN_B();		
	void XOR_R_IN_W();		
	void XOR_A_B();
	void XOR_A_W();
	void AAA();				// ASCII adjust for addition
	void CMP_R_OUT_B();		// сравнение
	void CMP_R_OUT_W();
	void CMP_R_IN_B();
	void CMP_R_IN_W();
	void CMP_A_B();
	void CMP_A_W();
	void AAS();				// ASCII adjust for substraction
	void INC_R(word& reg);	// инкремент регистра
	void DEC_R(word& reg);	// декремент регистра
	void PUSH_R(word& reg);	// push 16 битного регистра в стек
	void POP_R(word& reg);	// pop 16 битного регистра из стека
	void JMP_COND(std::function<bool()> condition);	// переход по условию, условие - аргумент
	void IMMED_B();
	void IMMED_W();
	void IMMED_B_SX();
	void IMMED_W_SX();
	void MOV_R_OUT_B();
	void MOV_R_OUT_W();
	void MOV_R_IN_B();
	void MOV_R_IN_W();
	void MOV_SR_OUT();
	void MOV_SR_IN();
	void MOV_A_IN_B();
	void MOV_A_IN_W();
	void MOV_A_OUT_B();
	void MOV_A_OUT_W();
	void MOV_R_IMM_B(byte& reg);
	void MOV_R_IMM_W(word& reg);
	void MOV_MEM_IMM_B();
	void MOV_MEM_IMM_W();
	void HLT();
};