#pragma once 
#include "Constants.h"
#include <map>
#include <functional>
#include <memory>
#include "Memory.h"
#include "AbstractPresenter.h"
#include "ConsolePresenter.h"

class ConsolePresenter;

static enum class Flag { O = 11, D = 10, I = 9, T = 8, S = 7, Z = 6, A = 4, P = 2, C = 0 };	// bits of flags register

// ������ ������ �������������� ��� ����� ��������
// ��������� � ������� � ������ ������ ���������
#define L low_high[0]
#define H low_high[1]

union reg {
	word X;
	byte low_high[2];
};

class cpu8086 {
public:	// private
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
	std::shared_ptr<ConsolePresenter> presenter;

	friend class Tester;

public:
	cpu8086();
	void reset();	// �����
	void clock();	// ���������� ����� �������
	void initOpTable();
	void initMemory(std::shared_ptr<Memory> mem);
	void initPresenter(std::shared_ptr<ConsolePresenter> p_pres);

public: // private
	// ������ ��� �������� ������
	void testFlagZ(word src_op);
	void testFlagSB(byte src_op);
	void testFlagSW(word src_op);
	void testFlagPB(byte val);
	void testFlagPW(word val);
	void testFlagCAddB(byte prev_val, byte src_op);	// ��� ������ �� �������
	void testFlagCSubB(byte prev_val, byte src_op);
	void testFlagCAddW(word prev_val, word src_op);	// ��� ������ �� �������
	void testFlagCSubW(word prev_val, word src_op);
	void testFlagAAdd(word prev_val, word src_op);
	void testFlagASub(word prev_val, word src_op);
	void testFlagO(bool prev_sig_bit, bool now_sig_bit);

	// ������� ������ � �������
	bool getFlag(Flag f);
	void setFlag(Flag f);
	void remFlag(Flag f);

	// �������-��������� ���������
	word fetchEA(byte MOD, byte RM, word disp);	// ��������� ������������ ������ � ����������� �� ���� ���������
	word fetchDisp(byte mod, byte rm);	// ��������� ��������
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
	void INC_R(word& reg);
	void DEC_R(word& reg);
	void PUSH_R(word& reg);
	void POP_R(word& reg);
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
};