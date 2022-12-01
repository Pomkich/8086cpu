#pragma once 
#include "GenReg.h"
#include "Constants.h"

static enum class Flag { O = 11, D = 10, I = 9, T = 8, S = 7, Z = 6, A = 4, P = 2, C = 0 };	// bits of flags register

class cpu8086 {
public:
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

public:
	cpu8086();
	void reset();

	// ������ ��� �������� ������
	void testFlagZ(byte& src_op);
	void testFlagS(byte& src_op);	// ��� ������ � �������
	void testFlagS(word& src_op);	// ��� ������ �� �������
	void testFlagP(byte& src_op);
	void testFlagCAdd(byte prev_val, byte& src_op);	// ��� ������ � �������
	void testFlagCSub(byte prev_val, byte& src_op);	
	void testFlagCAdd(word prev_val, word& src_op);	// ��� ������ �� �������
	void testFlagCSub(word prev_val, word& src_op);
	void testFlagAAdd(word prev_val, word& src_op);
	void testFlagASub(word prev_val, word& src_op);
	void testFlagO(bool prev_sig_bit, bool now_sig_bit);

	// ������� ������ � �������
	bool getFlag(Flag f);
	void setFlag(Flag f);
	void remFlag(Flag f);
};