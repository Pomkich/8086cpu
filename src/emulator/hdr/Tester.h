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
	void ADD_R_OUT_B_Test();
	void ADD_R_OUT_W_Test();
	void ADD_R_IN_B_Test();
	void ADD_R_IN_W_Test();
	void ADD_A_B_Test();
	void ADD_A_W_Test();
	void OR_R_OUT_B_Test();
	void OR_R_OUT_W_Test();
	void OR_R_IN_B_Test();
	void OR_R_IN_W_Test();
	void OR_A_B_Test();
	void OR_A_W_Test();
	void ADC_R_OUT_B_Test();
	void ADC_R_OUT_W_Test();
	void ADC_R_IN_B_Test();
	void ADC_R_IN_W_Test();
	void ADC_A_B_Test();
	void ADC_A_W_Test();
	void SBB_R_OUT_B_Test();
	void SBB_R_OUT_W_Test();
	void SBB_R_IN_B_Test();
	void SBB_R_IN_W_Test();
	void SBB_A_B_Test();
	void SBB_A_W_Test();
	void AND_R_OUT_B_Test();
	void AND_R_OUT_W_Test();
	void AND_R_IN_B_Test();
	void AND_R_IN_W_Test();
	void AND_A_B_Test();
	void AND_A_W_Test();
	void DAA_Test();
	void SUB_R_OUT_B_Test();
	void SUB_R_OUT_W_Test();
	void SUB_R_IN_B_Test();
	void SUB_R_IN_W_Test();
	void SUB_A_B_Test();
	void SUB_A_W_Test();
	void DAS_Test();
	void XOR_R_OUT_B_Test();
	void XOR_R_OUT_W_Test();
	void XOR_R_IN_B_Test();
	void XOR_R_IN_W_Test();
	void XOR_A_B_Test();
	void XOR_A_W_Test();
	void AAA_Test();
	void CMP_R_OUT_B_Test();
	void CMP_R_OUT_W_Test();
	void CMP_R_IN_B_Test();
	void CMP_R_IN_W_Test();
	void CMP_A_B_Test();
	void CMP_A_W_Test();
	void AAS_Test();
	void INC_R_Test();
	void DEC_R_Test();
	void PUSH_R_Test();
	void POP_R_Test();
	void JMP_COND_Test();
	void MOV_R_OUT_B_Test();
	void MOV_R_OUT_W_Test();
	void MOV_R_IN_B_Test();
	void MOV_R_IN_W_Test();
	void MOV_SR_OUT_Test();
	void MOV_SR_IN_Test();
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

