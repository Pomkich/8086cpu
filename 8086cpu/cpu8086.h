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

// ������ ������ �������������� ��� ����� ��������
// ��������� � ������� � ������ ������ ���������
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
	// �������� ������ ����������
	reg A, B, C, D;
	// �������� ���������� � ��������
	word SP, BP, SI, DI;
	// �������� ������ ���������
	word CS, DS, SS, ES;
	// ��������� �� ����������
	word IP;
	// ������� ������
	word flag_reg;

	// ������� ������
	std::map<byte, std::function<void()>> opcode_table;

	byte opcode;		// ���������� �������� �������
	dword address;			// ����������� �����

	// ��������� �� ���������� �������� ������������
	std::shared_ptr<Memory> memory;
	// ��������� �� �����������
	AbstractPresenter* presenter;

	friend class Tester;

public:
	cpu8086();
	void reset();	// �����
	void clock();	// ���������� ����� �������
	void initOpTable();
	void initMemory(std::shared_ptr<Memory> mem);
	void initPresenter(AbstractPresenter* p_pres);
	void loadTestProgram();
	// ������� ������ � ����������
	word getRegVal(RegId reg_id);
	void setRegVal(RegId reg_id, word value);	
	// ������� ������ � �������
	bool getFlag(Flag f);
	void setFlag(Flag f);
	void remFlag(Flag f);

private:
	// ������ ��� �������� ������
	void testFlagZ(word new_val);
	void testFlagS(word new_val, OpType type);
	void testFlagP(byte val);
	void testFlagCAdd(word prev_val, word new_val);	// ��� ������ �� �������
	void testFlagCSub(word prev_val, word new_val);
	void testFlagAAdd(word prev_val, word new_val);
	void testFlagASub(word prev_val, word new_val);
	void testFlagO(word prev_val, word new_val, OpType type);

	// �������-��������� ���������
	word fetchEA(byte mod, byte rm, word disp);	// ��������� ������������ ������ � ����������� �� ���� ���������
	word fetchDisp(byte mod, byte rm);			// ��������� ��������
	byte fetchCodeByte();	// ��������� ���������� ����� � �������� ����
	word fetchCodeWord();	// ��������� ���������� ����� � �������� ����
	void fetchModRegRm(byte& mod, byte& reg, byte& rm);	// ��������� ����� �� �����
	byte& getRegB(byte reg);	// ������������� 8-������� ��������
	word& getRegW(byte reg);	// ������������� 16-������� ��������
	word& getSegReg(byte reg);	// ������������� ����������� ��������

private:
	void ADD_R_OUT_B();		// �������� �� ��������� ���������
	void ADD_R_OUT_W();		// �������� �� 16-������� ��������
	void ADD_R_IN_B();		// �������� � �������� �������
	void ADD_R_IN_W();		// �������� � 16-������ ������
	void ADD_A_B();
	void ADD_A_W();
	void OR_R_OUT_B();		// ���������� �������� �� ��������� ���������
	void OR_R_OUT_W();		// ���������� �������� �� 16-������� ��������
	void OR_R_IN_B();		// ���������� �������� � �������� �������
	void OR_R_IN_W();		// ���������� �������� � 16-������ ������
	void OR_A_B();
	void OR_A_W();
	void ADC_R_OUT_B();		// �������� ������ � C �����
	void ADC_R_OUT_W();
	void ADC_R_IN_B();
	void ADC_R_IN_W();
	void ADC_A_B();
	void ADC_A_W();
	void SBB_R_OUT_B();		// ��������� ������ � C �����
	void SBB_R_OUT_W();
	void SBB_R_IN_B();
	void SBB_R_IN_W();
	void SBB_A_B();
	void SBB_A_W();
	void AND_R_OUT_B();		// ���������� ��������� �� ��������� ���������
	void AND_R_OUT_W();		// ���������� ��������� �� 16-������� ��������
	void AND_R_IN_B();		// ���������� ��������� � �������� �������
	void AND_R_IN_W();		// ���������� ��������� � 16-������ ������
	void AND_A_B();
	void AND_A_W();
	void DAA();				// decimal adjust for addition
	void SUB_R_OUT_B();		// ���������
	void SUB_R_OUT_W();
	void SUB_R_IN_B();
	void SUB_R_IN_W();
	void SUB_A_B();
	void SUB_A_W();
	void DAS();				// decimal adjust for substraction
	void XOR_R_OUT_B();		// ���������� ����������
	void XOR_R_OUT_W();		
	void XOR_R_IN_B();		
	void XOR_R_IN_W();		
	void XOR_A_B();
	void XOR_A_W();
	void AAA();				// ASCII adjust for addition
	void CMP_R_OUT_B();		// ���������
	void CMP_R_OUT_W();
	void CMP_R_IN_B();
	void CMP_R_IN_W();
	void CMP_A_B();
	void CMP_A_W();
	void AAS();				// ASCII adjust for substraction
	void INC_R(word& reg);	// ��������� ��������
	void DEC_R(word& reg);	// ��������� ��������
	void PUSH_R(word& reg);	// push 16 ������� �������� � ����
	void POP_R(word& reg);	// pop 16 ������� �������� �� �����
	void JMP_COND(std::function<bool()> condition);	// ������� �� �������, ������� - ��������
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