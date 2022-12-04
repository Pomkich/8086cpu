#pragma once 
#include "Constants.h"
#include <map>
#include <functional>
#include <memory>
#include "Memory.h"

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
	dword instr_adr;	// ���������� �������� ���������� ����� �������
	dword stack_adr;	// ���������� �������� ���������� ����� �����

	// ��������� �� ���������� �������� ������������
	std::shared_ptr<Memory> memory;

public:
	cpu8086();
	void reset();	// �����
	void clock();	// ���������� ����� �������
	void initOpTable();
	void initMemory(std::shared_ptr<Memory> mem);

public: // private
	// ������ ��� �������� ������
	void testFlagZ(word& src_op);
	void testFlagSB(byte& src_op);	// ��� ������ � �������
	void testFlagSW(word& src_op);	// ��� ������ �� �������
	void testFlagP(byte val);
	void testFlagCAddB(byte prev_val, byte& src_op);	// ��� ������ � �������
	void testFlagCSubB(byte prev_val, byte& src_op);	
	void testFlagCAddW(word prev_val, word& src_op);	// ��� ������ �� �������
	void testFlagCSubW(word prev_val, word& src_op);
	void testFlagAAdd(word prev_val, word& src_op);
	void testFlagASub(word prev_val, word& src_op);
	void testFlagO(bool prev_sig_bit, bool now_sig_bit);

	// ������� ������ � �������
	bool getFlag(Flag f);
	void setFlag(Flag f);
	void remFlag(Flag f);

	// �������-��������� ���������
	word fetchEA(byte MOD, byte RM, word disp);	// ��������� ������������ ������ � ����������� �� ���� ���������
	byte& getRegB(byte reg);	// ������������� 8-������� ��������
	word& getRegW(byte reg);	// ������������� 16-������� ��������

private:
	void ADD_R_IN_B();		// �������� � �������� �������
	void ADD_R_OUT_B();		// �������� �� ��������� ��������
	void ADD_R_IN_W();		// �������� � 16-������ �������
	void ADD_R_OUT_W();		// �������� �� 16-������� ��������
	void INC_R(word& rgs);
	void DEC_R(word& rgs);
	void PUSH_R(word& rgs);
	void POP_R(word& rgs);
};