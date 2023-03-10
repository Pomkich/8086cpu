#pragma once
#include "Constants.h"
#include "cpu8086.h"
#include "Memory.h"
#include <memory>
#include <cassert>


class Tester {
private:
	std::shared_ptr<cpu8086> cpu_pt;
	std::shared_ptr<Memory> mem_pt;

public:
	Tester();

	void RunTests();

	/*** MEMORY TESTS BEGIN ***/
	void MemoryCheckByteWriting();
	void MemoryCheckWordWriting();
	void MemoryCheckBytePlacement();
	/*** MEMORY TESTS END   ***/

	/*** FLAG TESTS BEGIN ***/
	void FlagOTest();
	void FlagSTest();
	void FlagZTest();
	void FlagATest();
	void FlagPTest();
	void FlagCTest();
	/*** FLAG TESTS END   ***/

	/*** OPCODE TESTS BEGIN ***/
	void ADD_R_IN_B_Test();
	void ADD_R_OUT_B_Test();
	void ADD_R_IN_W_Test();
	void ADD_R_OUT_W_Test();
	void ADD_A_B_Test();
	void ADD_A_W_Test();
	void INC_R_Test();
	void DEC_R_Test();
	void PUSH_R_Test();
	void POP_R_Test();
	void MOV_R_OUT_B_Test();
	void MOV_R_OUT_W_Test();
	void MOV_R_IN_B_Test();
	void MOV_R_IN_W_Test();
	void MOV_A_IN_B_Test();
	void MOV_A_IN_W_Test();
	void MOV_A_OUT_B_Test();
	void MOV_A_OUT_W_Test();
	void MOV_R_IMM_B_Test();
	void MOV_R_IMM_W_Test();
	void MOV_MEM_IMM_B_Test();
	void MOV_MEM_IMM_W_Test();
	/*** OPCODE TESTS END   ***/
};

