#pragma once 
#include "GenReg.h"
#include "Constants.h"

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
};