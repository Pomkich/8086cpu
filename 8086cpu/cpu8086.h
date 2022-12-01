#pragma once 
#include "Constants.h"
#include <map>
#include <functional>

static enum class Flag { O = 11, D = 10, I = 9, T = 8, S = 7, Z = 6, A = 4, P = 2, C = 0 };	// bits of flags register

// ������ ������ �������������� ��� ����� ��������
// ��������� � ������� � ������ ������ ���������
#define L HL[0]
#define H HL[1]

union reg {
	word X;
	byte HL[2];
};

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

	// ������� ������
	std::map<byte, std::function<void()>> opcode_table;

	// ���������� ��� ������ � ���������� �������
	byte opcode;

public:
	cpu8086();
	void reset();	// �����
	void clock();	// ���������� ����� �������
	void initOpTable();

	// ������ ��� �������� ������
	void testFlagZ(word& src_op);
	void testFlagS(byte& src_op);	// ��� ������ � �������
	void testFlagS(word& src_op);	// ��� ������ �� �������
	void testFlagP(byte val);
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

private:
	void INC_R(word& rgs);
};