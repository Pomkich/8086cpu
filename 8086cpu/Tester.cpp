#include "Tester.h"

Tester::Tester() {
	cpu_pt = std::make_shared<cpu8086>();
	mem_pt = std::make_shared<Memory>();
	cpu_pt->initMemory(mem_pt);
	cpu_pt->initOpTable();
	cpu_pt->reset();
	mem_pt->reset();
}

void Tester::RunTests() {
	MemoryCheckByteWriting();
	MemoryCheckWordWriting();
	MemoryCheckBytePlacement();

	FlagOTest();
	FlagSTest();
	FlagZTest();
	FlagATest();
	FlagPTest();
	FlagCTest();

	ADD_R_IN_B_Test();
	ADD_R_OUT_B_Test();
	ADD_R_IN_W_Test();
	ADD_R_OUT_W_Test();
	ADD_A_B_Test();
	ADD_A_W_Test();
	OR_R_IN_B_Test();
	OR_R_OUT_B_Test();
	OR_R_IN_W_Test();
	OR_R_OUT_W_Test();
	OR_A_B_Test();
	OR_A_W_Test();
	ADC_R_OUT_B_Test();
	ADC_R_OUT_W_Test();
	ADC_R_IN_B_Test();
	ADC_R_IN_W_Test();
	ADC_A_B_Test();
	ADC_A_W_Test();
	SBB_R_OUT_B_Test();
	SBB_R_OUT_W_Test();
	SBB_R_IN_B_Test();
	SBB_R_IN_W_Test();
	SBB_A_B_Test();
	SBB_A_W_Test();
	AND_R_OUT_B_Test();
	AND_R_OUT_W_Test();
	AND_R_IN_B_Test();
	AND_R_IN_W_Test();
	AND_A_B_Test();
	AND_A_W_Test();
	DAA_Test();
	SUB_R_OUT_B_Test();
	SUB_R_OUT_W_Test();
	SUB_R_IN_B_Test();
	SUB_R_IN_W_Test();
	SUB_A_B_Test();
	SUB_A_W_Test();
	DAS_Test();
	XOR_R_OUT_B_Test();
	XOR_R_OUT_W_Test();
	XOR_R_IN_B_Test();
	XOR_R_IN_W_Test();
	XOR_A_B_Test();
	XOR_A_W_Test();
	CMP_R_OUT_B_Test();
	CMP_R_OUT_W_Test();
	CMP_R_IN_B_Test();
	CMP_R_IN_W_Test();
	CMP_A_B_Test();
	CMP_A_W_Test();
	INC_R_Test();
	DEC_R_Test();
	PUSH_R_Test();
	POP_R_Test();
	MOV_R_OUT_B_Test();
	MOV_R_OUT_W_Test();
	MOV_R_IN_B_Test();
	MOV_R_IN_W_Test();
	MOV_SR_OUT_Test();
	MOV_SR_IN_Test();
	MOV_A_IN_B_Test();
	MOV_A_IN_W_Test();
	MOV_A_OUT_B_Test();
	MOV_A_OUT_W_Test();
	MOV_R_IMM_B_Test();
	MOV_R_IMM_W_Test();
	MOV_MEM_IMM_B_Test();
	MOV_MEM_IMM_W_Test();
}

void Tester::MemoryCheckByteWriting() {
	mem_pt->reset();
	mem_pt->writeB(0x10000, 0x32);
	assert(mem_pt->readB(0x10000) == 0x32);
}

void Tester::MemoryCheckWordWriting() {
	mem_pt->reset();
	mem_pt->writeW(0x10000, 0x3264);
	assert(mem_pt->readW(0x10000) == 0x3264);
}

void Tester::MemoryCheckBytePlacement() {
	mem_pt->reset();
	mem_pt->writeW(0x10000, 0x3264);
	assert(mem_pt->readB(0x10000) == 0x64);
	assert(mem_pt->readB(0x10001) == 0x32);
}

void Tester::FlagOTest() {
	// overflow occured byte
	cpu_pt->testFlagO(0x7F, 0x80, OpType::Byte);
	assert(cpu_pt->getFlag(Flag::O) == 1);
	cpu_pt->testFlagO(0x80, 0x7F, OpType::Byte);
	assert(cpu_pt->getFlag(Flag::O) == 1);
	// overflow occured word
	cpu_pt->testFlagO(0x7FFF, 0x8000, OpType::Word);
	assert(cpu_pt->getFlag(Flag::O) == 1);
	cpu_pt->testFlagO(0x8000, 0x7FFF, OpType::Word);
	assert(cpu_pt->getFlag(Flag::O) == 1);

	// overflow didn't occured byte
	cpu_pt->testFlagO(0x32, 0x44, OpType::Byte);
	assert(cpu_pt->getFlag(Flag::O) == 0);
	cpu_pt->testFlagO(0x91, 0x85, OpType::Byte);
	assert(cpu_pt->getFlag(Flag::O) == 0);
}

void Tester::FlagSTest() {
	// byte values
	cpu_pt->testFlagS(0b10000000, OpType::Byte);
	assert(cpu_pt->getFlag(Flag::S) == true);
	cpu_pt->testFlagS(0b00101100, OpType::Byte);
	assert(cpu_pt->getFlag(Flag::S) == false);
	// word values
	cpu_pt->testFlagS(0xFFFF, OpType::Word);
	assert(cpu_pt->getFlag(Flag::S) == true);
	cpu_pt->testFlagS(0x0FFF, OpType::Word);
	assert(cpu_pt->getFlag(Flag::S) == false);
}

void Tester::FlagZTest() {
	cpu_pt->testFlagZ(0x0000);
	assert(cpu_pt->getFlag(Flag::Z) == true);
	cpu_pt->testFlagZ(0x1234);
	assert(cpu_pt->getFlag(Flag::Z) == false);
}

void Tester::FlagATest() {
	// borrow from high nimble
	cpu_pt->testFlagAAdd(0xFF0F, 0xFF10);
	assert(cpu_pt->getFlag(Flag::A) == 1);
	cpu_pt->testFlagAAdd(0xFF0F, 0x0000);
	assert(cpu_pt->getFlag(Flag::A) == 1);
	cpu_pt->testFlagAAdd(0xFF0F, 0x0E0F);
	assert(cpu_pt->getFlag(Flag::A) == 0);
	cpu_pt->testFlagAAdd(0xFF03, 0xFF06);
	assert(cpu_pt->getFlag(Flag::A) == 0);
	// carry out from low nimble
	cpu_pt->testFlagASub(0xFFF0, 0xFF0F);
	assert(cpu_pt->getFlag(Flag::A) == 1);
	cpu_pt->testFlagASub(0xFF30, 0xFF10);
	assert(cpu_pt->getFlag(Flag::A) == 0);
	cpu_pt->testFlagASub(0xFF0F, 0x000E);
	assert(cpu_pt->getFlag(Flag::A) == 0);
	cpu_pt->testFlagASub(0xFF30, 0x0000);
	assert(cpu_pt->getFlag(Flag::A) == 0);
}

void Tester::FlagPTest() {
	// byte values
	cpu_pt->testFlagP(0xF0);
	assert(cpu_pt->getFlag(Flag::P) == true);
	cpu_pt->testFlagP(0xF1);
	assert(cpu_pt->getFlag(Flag::P) == false);

	// word values
	cpu_pt->testFlagP(0xFF00);
	assert(cpu_pt->getFlag(Flag::P) == true);
	cpu_pt->testFlagP(0xFF01);
	assert(cpu_pt->getFlag(Flag::P) == false);
}

void Tester::FlagCTest() {
	// byte functions
	cpu_pt->testFlagCAdd(0xFF, 0xFE);
	cpu_pt->testFlagAAdd(0xFF, 0xFE);
	assert(cpu_pt->getFlag(Flag::C) == 1);
	assert(cpu_pt->getFlag(Flag::A) == 1);

	cpu_pt->testFlagCSub(0x00, 0xFF);
	assert(cpu_pt->getFlag(Flag::C) == 1);

	cpu_pt->testFlagCAdd(0xEE, 0xEF);
	assert(cpu_pt->getFlag(Flag::C) == 0);

	cpu_pt->testFlagCSub(0xEE, 0xEA);
	assert(cpu_pt->getFlag(Flag::C) == 0);

	cpu_pt->testFlagCSub(0xEE, 0xEF);
	cpu_pt->testFlagASub(0xEE, 0xEF);
	assert(cpu_pt->getFlag(Flag::C) == 1);
	assert(cpu_pt->getFlag(Flag::A) == 1);

	// word functions
	cpu_pt->testFlagCAdd(0xF000, 0x0004);
	assert(cpu_pt->getFlag(Flag::C) == 1);
	cpu_pt->testFlagCSub(0x0000, 0xF000);
	assert(cpu_pt->getFlag(Flag::C) == 1);
	cpu_pt->testFlagCAdd(0x7000, 0x7AFF);
	assert(cpu_pt->getFlag(Flag::C) == 0);
	cpu_pt->testFlagCSub(0xFFFF, 0xFF00);
	assert(cpu_pt->getFlag(Flag::C) == 0);
}

void Tester::ADD_R_OUT_B_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x00);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x35);	// value in memory: 35
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readB(0x01064) == 0x67);
}

void Tester::ADD_R_OUT_W_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x3211;
	// initialize memory
	mem_pt->writeB(0x10000, 0x01);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x1212);
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readW(0x01064) == 0x4423);
}

void Tester::ADD_R_IN_B_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x02);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x35);	// value in memory: 35
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x67);

	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->B.L = 0x32;
	cpu_pt->C.L = 0x58;
	// initialize memory
	mem_pt->writeB(0x10000, 0x02);	// opcode: ADD
	mem_pt->writeB(0x10001, 0xD9);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x8A);
}

void Tester::ADD_R_IN_W_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x1000;
	// initialize memory
	mem_pt->writeB(0x10000, 0x03);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x3523);
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0x4523);
	
	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->C.X = 0x1234;
	cpu_pt->A.X = 0x4321;
	// initialize memory
	mem_pt->writeB(0x10000, 0x03);	// opcode: ADD
	mem_pt->writeB(0x10001, 0xC8);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0x5555);
}

void Tester::ADD_A_B_Test() {
	// adding value accumulator byte
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x04);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x35);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.L == 0x67);
}

void Tester::ADD_A_W_Test() {
	// adding value accumulator word
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.X = 0x3211;
	// initialize memory
	mem_pt->writeB(0x10000, 0x05);		// opcode: ADD
	mem_pt->writeW(0x10001, 0x3511);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.X == 0x6722);
}

void Tester::OR_R_OUT_B_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x08);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x35);	// value in memory: 35
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readB(0x01064) == 0x37);
}

void Tester::OR_R_OUT_W_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x3211;
	// initialize memory
	mem_pt->writeB(0x10000, 0x09);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x1212);
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readW(0x01064) == 0x3213);
}

void Tester::OR_R_IN_B_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x0A);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x35);	// value in memory: 35
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x37);

	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->B.L = 0x32;
	cpu_pt->C.L = 0x58;
	// initialize memory
	mem_pt->writeB(0x10000, 0x0A);	// opcode: ADD
	mem_pt->writeB(0x10001, 0xD9);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x7A);
}

void Tester::OR_R_IN_W_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x1000;
	// initialize memory
	mem_pt->writeB(0x10000, 0x0B);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x3523);
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0x3523);
	assert(cpu_pt->getFlag(Flag::C) == false);

	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->C.X = 0x1234;
	cpu_pt->A.X = 0x4321;
	// initialize memory
	mem_pt->writeB(0x10000, 0x0B);	// opcode: ADD
	mem_pt->writeB(0x10001, 0xC8);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0x5335);
}

void Tester::OR_A_B_Test() {
	// adding value accumulator byte
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x0C);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x35);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.L == 0x37);
}

void Tester::OR_A_W_Test() {
	// adding value accumulator word
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.X = 0x3211;
	// initialize memory
	mem_pt->writeB(0x10000, 0x0D);		// opcode: ADD
	mem_pt->writeW(0x10001, 0x3511);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.X == 0x3711);
}

void Tester::ADC_R_OUT_B_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	cpu_pt->setFlag(Flag::C);
	// initialize memory
	mem_pt->writeB(0x10000, 0x10);	// opcode: ADC
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x35);	// value in memory: 35
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readB(0x01064) == 0x68);
}

void Tester::ADC_R_OUT_W_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x3211;
	cpu_pt->setFlag(Flag::C);
	// initialize memory
	mem_pt->writeB(0x10000, 0x11);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x1212);
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readW(0x01064) == 0x4424);
}

void Tester::ADC_R_IN_B_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x12);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x35);	// value in memory: 35
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x67);

	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->B.L = 0x32;
	cpu_pt->C.L = 0x58;
	cpu_pt->setFlag(Flag::C);
	// initialize memory
	mem_pt->writeB(0x10000, 0x12);	// opcode: ADD
	mem_pt->writeB(0x10001, 0xD9);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x8B);
}

void Tester::ADC_R_IN_W_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x1000;
	// initialize memory
	mem_pt->writeB(0x10000, 0x13);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x3523);
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0x4523);

	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->C.X = 0x1234;
	cpu_pt->A.X = 0x4321;
	// initialize memory
	mem_pt->writeB(0x10000, 0x13);	// opcode: ADD
	mem_pt->writeB(0x10001, 0xC8);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0x5555);
}

void Tester::ADC_A_B_Test() {
	// adding value accumulator byte
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.L = 0x32;
	cpu_pt->setFlag(Flag::C);
	// initialize memory
	mem_pt->writeB(0x10000, 0x14);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x35);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.L == 0x68);
}

void Tester::ADC_A_W_Test() {
	// adding value accumulator word
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.X = 0x3211;
	cpu_pt->setFlag(Flag::C);
	// initialize memory
	mem_pt->writeB(0x10000, 0x15);		// opcode: ADD
	mem_pt->writeW(0x10001, 0x3511);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.X == 0x6723);
}

void Tester::SBB_R_OUT_B_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	cpu_pt->setFlag(Flag::C);
	// initialize memory
	mem_pt->writeB(0x10000, 0x18);	// opcode: SBB
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x31);	// value in memory: 31
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readB(0x01064) == 0xFE);

	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	cpu_pt->setFlag(Flag::C);
	// initialize memory
	mem_pt->writeB(0x10000, 0x18);	// opcode: SBB
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x34);	// value in memory: 31
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readB(0x01064) == 0x01);
}

void Tester::SBB_R_OUT_W_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x3211;
	cpu_pt->setFlag(Flag::C);
	// initialize memory
	mem_pt->writeB(0x10000, 0x19);	// opcode: SBB
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x1212);
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readW(0x01064) == 0xE000);
}

void Tester::SBB_R_IN_B_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x42;
	// initialize memory
	mem_pt->writeB(0x10000, 0x1A);	// opcode: SBB
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x35);	// value in memory: 35
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x0D);

	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->B.L = 0x98;
	cpu_pt->C.L = 0x58;
	cpu_pt->setFlag(Flag::C);
	// initialize memory
	mem_pt->writeB(0x10000, 0x1A);	// opcode: SBB
	mem_pt->writeB(0x10001, 0xD9);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x3F);
}

void Tester::SBB_R_IN_W_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x1000;
	cpu_pt->setFlag(Flag::C);
	// initialize memory
	mem_pt->writeB(0x10000, 0x1B);	// opcode: SBB
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x3523);
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0xDADC);

	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->C.X = 0x1234;
	cpu_pt->A.X = 0x4321;
	// initialize memory
	mem_pt->writeB(0x10000, 0x1B);	// opcode: SBB
	mem_pt->writeB(0x10001, 0xC8);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0xCF13);
}

void Tester::SBB_A_B_Test() {
	// adding value accumulator byte
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.L = 0x32;
	cpu_pt->setFlag(Flag::C);
	// initialize memory
	mem_pt->writeB(0x10000, 0x1C);	// opcode: SBB
	mem_pt->writeB(0x10001, 0x12);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.L == 0x1F);
}

void Tester::SBB_A_W_Test() {
	// adding value accumulator word
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.X = 0x3211;
	cpu_pt->setFlag(Flag::C);
	// initialize memory
	mem_pt->writeB(0x10000, 0x1D);		// opcode: ADD
	mem_pt->writeW(0x10001, 0x3511);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.X == 0xFCFF);
}

void Tester::AND_R_OUT_B_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x20);	// opcode: AND
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x35);	// value in memory: 35
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readB(0x01064) == 0x30);
}

void Tester::AND_R_OUT_W_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x3211;
	// initialize memory
	mem_pt->writeB(0x10000, 0x21);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x1212);
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readW(0x01064) == 0x1210);
}

void Tester::AND_R_IN_B_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x22);	// opcode: AND
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x35);	// value in memory: 35
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x30);

	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->B.L = 0x32;
	cpu_pt->C.L = 0x58;
	// initialize memory
	mem_pt->writeB(0x10000, 0x22);	// opcode: AND
	mem_pt->writeB(0x10001, 0xD9);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x10);
}

void Tester::AND_R_IN_W_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x1000;
	// initialize memory
	mem_pt->writeB(0x10000, 0x23);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x3523);
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0x1000);

	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->C.X = 0x1234;
	cpu_pt->A.X = 0x4321;
	// initialize memory
	mem_pt->writeB(0x10000, 0x23);	// opcode: ADD
	mem_pt->writeB(0x10001, 0xC8);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0x0220);
}

void Tester::AND_A_B_Test() {
	// adding value accumulator byte
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x24);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x35);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.L == 0x30);
}

void Tester::AND_A_W_Test() {
	// adding value accumulator word
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.X = 0x3211;
	// initialize memory
	mem_pt->writeB(0x10000, 0x25);		// opcode: ADD
	mem_pt->writeW(0x10001, 0x3511);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.X == 0x3011);
}

void Tester::DAA_Test() {
	// adding value accumulator word
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.L = 0;
	// initialize memory
	mem_pt->writeB(0x10000, 0x04);		// opcode: ADD
	mem_pt->writeB(0x10001, 0xE0);		// opcode: 
	mem_pt->writeB(0x10002, 0x27);		// opcode: DAA
	// run opcodes
	cpu_pt->clock();
	cpu_pt->clock();

	assert(cpu_pt->A.L == 0x40);
}

void Tester::SUB_R_OUT_B_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x28);	// opcode: SBB
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x31);	// value in memory: 31
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readB(0x01064) == 0xFF);

	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x28);	// opcode: SBB
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x34);	// value in memory: 31
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readB(0x01064) == 0x02);
}

void Tester::SUB_R_OUT_W_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x3211;
	// initialize memory
	mem_pt->writeB(0x10000, 0x29);	// opcode: SBB
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x1212);
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readW(0x01064) == 0xE001);
}

void Tester::SUB_R_IN_B_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x42;
	// initialize memory
	mem_pt->writeB(0x10000, 0x2A);	// opcode: SBB
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x35);	// value in memory: 35
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x0D);

	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->B.L = 0x98;
	cpu_pt->C.L = 0x58;
	// initialize memory
	mem_pt->writeB(0x10000, 0x2A);	// opcode: SBB
	mem_pt->writeB(0x10001, 0xD9);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x40);
}

void Tester::SUB_R_IN_W_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x1000;
	// initialize memory
	mem_pt->writeB(0x10000, 0x2B);	// opcode: SBB
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x3523);
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0xDADD);

	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->C.X = 0x1234;
	cpu_pt->A.X = 0x4321;
	// initialize memory
	mem_pt->writeB(0x10000, 0x2B);	// opcode: SBB
	mem_pt->writeB(0x10001, 0xC8);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0xCF13);
}

void Tester::SUB_A_B_Test() {
	// adding value accumulator byte
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x2C);	// opcode: SBB
	mem_pt->writeB(0x10001, 0x12);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.L == 0x20);
}

void Tester::SUB_A_W_Test() {
	// adding value accumulator word
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.X = 0x3211;
	// initialize memory
	mem_pt->writeB(0x10000, 0x1D);		// opcode: ADD
	mem_pt->writeW(0x10001, 0x3511);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.X == 0xFD00);
}

void Tester::DAS_Test() {
	// adding value accumulator word
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.L = 0;
	// initialize memory
	mem_pt->writeB(0x10000, 0x2C);		// opcode: SUB
	mem_pt->writeB(0x10001, 0xE0);
	mem_pt->writeB(0x10002, 0x2F);		// opcode: DAS
	// run opcodes
	cpu_pt->clock();
	cpu_pt->clock();

	assert(cpu_pt->A.L == 0xC0);
}

void Tester::XOR_R_OUT_B_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x30);	// opcode: XOR
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x35);	// value in memory: 35
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readB(0x01064) == 0x07);
	assert(cpu_pt->getFlag(Flag::A) == true);
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x30);	// opcode: XOR
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x32);	// value in memory: 35
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readB(0x01064) == 0x00);
	assert(cpu_pt->getFlag(Flag::A) == false);
}

void Tester::XOR_R_OUT_W_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x3211;
	// initialize memory
	mem_pt->writeB(0x10000, 0x31);	// opcode: XOR
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x1212);
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readW(0x01064) == 0x2003);
}

void Tester::XOR_R_IN_B_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x32);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x35);	// value in memory: 35
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x07);

	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->B.L = 0x32;
	cpu_pt->C.L = 0x58;
	// initialize memory
	mem_pt->writeB(0x10000, 0x32);	// opcode: XOR
	mem_pt->writeB(0x10001, 0xD9);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x6A);
}

void Tester::XOR_R_IN_W_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x1000;
	// initialize memory
	mem_pt->writeB(0x10000, 0x33);	// opcode: XOR
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x3523);
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0x2523);

	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->C.X = 0x1234;
	cpu_pt->A.X = 0x4321;
	// initialize memory
	mem_pt->writeB(0x10000, 0x33);	// opcode: ADD
	mem_pt->writeB(0x10001, 0xC8);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0x5115);
}

void Tester::XOR_A_B_Test() {
	// adding value accumulator byte
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x34);	// opcode: ADD
	mem_pt->writeB(0x10001, 0x35);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.L == 0x07);
}

void Tester::XOR_A_W_Test() {
	// adding value accumulator word
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.X = 0x3211;
	// initialize memory
	mem_pt->writeB(0x10000, 0x35);		// opcode: ADD
	mem_pt->writeW(0x10001, 0x3511);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.X == 0x0700);
}

void Tester::CMP_R_OUT_B_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x38);	// opcode: CMP
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x32);	// value in memory: 32
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readB(0x01064) == 0x32);
	assert(cpu_pt->getFlag(Flag::Z) == true);

	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x38);	// opcode: SBB
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x34);	// value in memory: 31
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readB(0x01064) == 0x34);
}

void Tester::CMP_R_OUT_W_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x3211;
	// initialize memory
	mem_pt->writeB(0x10000, 0x39);	// opcode: CMP
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x1212);
	// run opcode
	cpu_pt->clock();
	assert(mem_pt->readW(0x01064) == 0x1212);
}

void Tester::CMP_R_IN_B_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->B.L = 0x42;
	// initialize memory
	mem_pt->writeB(0x10000, 0x3A);	// opcode: CMP
	mem_pt->writeB(0x10001, 0x1E);	// MOD: 00, REG: 011, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeB(0x01064, 0x35);	// value in memory: 35
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x42);

	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->B.L = 0x98;
	cpu_pt->C.L = 0x58;
	// initialize memory
	mem_pt->writeB(0x10000, 0x3A);	// opcode: CMP
	mem_pt->writeB(0x10001, 0xD9);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->B.L == 0x98);
}

void Tester::CMP_R_IN_W_Test() {
	// adding value from memory to register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->DS = 0x0100;
	cpu_pt->C.X = 0x1000;
	// initialize memory
	mem_pt->writeB(0x10000, 0x3B);	// opcode: CMP
	mem_pt->writeB(0x10001, 0x0E);	// MOD: 00, REG: 001, R/M: 110
	mem_pt->writeB(0x10002, 0x64);	// displacement low
	mem_pt->writeB(0x10003, 0x00);	// displacement high
	mem_pt->writeW(0x01064, 0x3523);
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0x1000);

	// adding value to register from register
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->C.X = 0x1234;
	cpu_pt->A.X = 0x4321;
	// initialize memory
	mem_pt->writeB(0x10000, 0x3B);	// opcode: CMP
	mem_pt->writeB(0x10001, 0xC8);	// MOD: 11, REG: 011, R/M: 001
	// run opcode
	cpu_pt->clock();
	assert(cpu_pt->C.X == 0x1234);
}

void Tester::CMP_A_B_Test() {
	// adding value accumulator byte
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.L = 0x32;
	// initialize memory
	mem_pt->writeB(0x10000, 0x3C);	// opcode: CMP
	mem_pt->writeB(0x10001, 0x12);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.L == 0x32);
}

void Tester::CMP_A_W_Test() {
	// adding value accumulator word
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->A.X = 0x3211;
	// initialize memory
	mem_pt->writeB(0x10000, 0x3D);		// opcode: CMP
	mem_pt->writeW(0x10001, 0x3511);
	// run opcode
	cpu_pt->clock();

	assert(cpu_pt->A.X == 0x3211);
	assert(cpu_pt->getFlag(Flag::C) == true);
}


void Tester::INC_R_Test() {
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->B.X = 0x35;
	// initialize memory
	mem_pt->writeB(0x10000, 0x41);
	// run command
	cpu_pt->clock();
	assert(cpu_pt->B.X == 0x36);
}

void Tester::DEC_R_Test() {
	cpu_pt->reset();
	mem_pt->reset();
	// initialize registers
	cpu_pt->CS = 0x1000;
	cpu_pt->IP = 0x0000;
	cpu_pt->B.X = 0x35;
	// initialize memory
	mem_pt->writeB(0x10000, 0x49);
	// run command
	cpu_pt->clock();
	assert(cpu_pt->B.X == 0x34);
}

void Tester::PUSH_R_Test() {
	cpu_pt->reset();
	mem_pt->reset();
	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize stack segment
	cpu_pt->SS = 0x1000;
	cpu_pt->SP = 0x0004;
	mem_pt->writeB(0x40001, 0x50);
	cpu_pt->A.X = 0x1234;
	cpu_pt->clock();
	// stack grows down
	assert(mem_pt->readB(0x10003) == 0x12 && mem_pt->readB(0x10002) == 0x34);
}

void Tester::POP_R_Test() {
	cpu_pt->reset();
	mem_pt->reset();
	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize stack segment
	cpu_pt->SS = 0x1000;
	cpu_pt->SP = 0x0004;
	mem_pt->writeB(0x40001, 0x58);
	mem_pt->writeB(0x10005, 0xFF);
	mem_pt->writeB(0x10004, 0x35);
	cpu_pt->A.X = 0x1234;
	cpu_pt->clock();

	assert(cpu_pt->A.X == 0xFF35);
}

void Tester::MOV_R_OUT_B_Test() {
	cpu_pt->reset();
	mem_pt->reset();

	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize data segment
	cpu_pt->DS = 0x1000;
	// initialize memory
	mem_pt->writeB(0x40001, 0x88);
	mem_pt->writeB(0x40002, 0x1E);
	mem_pt->writeB(0x40003, 0x32);
	mem_pt->writeB(0x40004, 0x00);
	cpu_pt->B.L = 0x11;
	cpu_pt->clock();

	assert(mem_pt->readB(0x10032) == 0x11);
}

void Tester::MOV_R_OUT_W_Test() {
	cpu_pt->reset();
	mem_pt->reset();

	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize data segment
	cpu_pt->DS = 0x1000;
	// initialize memory
	mem_pt->writeB(0x40001, 0x89);
	mem_pt->writeB(0x40002, 0x1E);
	mem_pt->writeB(0x40003, 0x32);
	mem_pt->writeB(0x40004, 0x11);
	cpu_pt->B.X = 0x1234;
	cpu_pt->clock();

	assert(mem_pt->readW(0x11132) == 0x1234);
}

void Tester::MOV_R_IN_B_Test()  {
	// memory mode test
	cpu_pt->reset();
	mem_pt->reset();

	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize data segment
	cpu_pt->DS = 0x1000;
	// initialize memory
	mem_pt->writeB(0x40001, 0x8A);
	mem_pt->writeB(0x40002, 0x1E);
	mem_pt->writeB(0x40003, 0x32);
	mem_pt->writeB(0x40004, 0x00);
	mem_pt->writeB(0x10032, 0x64);
	cpu_pt->clock();

	assert(cpu_pt->B.L == 0x64);

	// register mode test
	cpu_pt->reset();
	mem_pt->reset();

	cpu_pt->B.L = 0x33;
	cpu_pt->A.L = 0x11;

	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize memory
	mem_pt->writeB(0x40001, 0x8A);
	mem_pt->writeB(0x40002, 0xD8);
	cpu_pt->clock();

	assert(cpu_pt->B.L == 0x11);
}

void Tester::MOV_R_IN_W_Test() {
	// memory mode test
	cpu_pt->reset();
	mem_pt->reset();

	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize data segment
	cpu_pt->DS = 0x1000;
	// initialize memory
	mem_pt->writeB(0x40001, 0x8B);
	mem_pt->writeB(0x40002, 0x1E);
	mem_pt->writeB(0x40003, 0x32);
	mem_pt->writeB(0x40004, 0x11);
	mem_pt->writeW(0x11132, 0x6412);
	cpu_pt->clock();

	assert(cpu_pt->B.X == 0x6412);

	// register mode test
	cpu_pt->reset();
	mem_pt->reset();

	cpu_pt->B.X = 0x3311;
	cpu_pt->A.X = 0x1133;

	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize memory
	mem_pt->writeB(0x40001, 0x8B);
	mem_pt->writeB(0x40002, 0xD8);
	cpu_pt->clock();

	assert(cpu_pt->B.X == 0x1133);
}

void Tester::MOV_SR_OUT_Test() {
	// memory mode test
	cpu_pt->reset();
	mem_pt->reset();
	cpu_pt->SS = 0x1111;
	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize data segment
	cpu_pt->DS = 0x1000;
	// initialize memory
	mem_pt->writeB(0x40001, 0x8C);
	mem_pt->writeB(0x40002, 0x16);
	mem_pt->writeB(0x40003, 0x64);
	mem_pt->writeB(0x40004, 0x00);
	cpu_pt->clock();

	assert(mem_pt->readW(0x10064) == 0x1111);

	// register mode test
	cpu_pt->reset();
	mem_pt->reset();
	cpu_pt->SS = 0x1111;

	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize memory
	mem_pt->writeB(0x40001, 0x8C);
	mem_pt->writeB(0x40002, 0xD0);
	cpu_pt->clock();

	assert(cpu_pt->A.X == 0x1111);
}

void Tester::MOV_SR_IN_Test() {
	// memory mode test
	cpu_pt->reset();
	mem_pt->reset();
	
	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize data segment
	cpu_pt->DS = 0x1000;
	// initialize memory
	mem_pt->writeB(0x40001, 0x8E);
	mem_pt->writeB(0x40002, 0x16);
	mem_pt->writeB(0x40003, 0x64);
	mem_pt->writeB(0x40004, 0x00);
	mem_pt->writeW(0x10064, 0x3211);
	cpu_pt->clock();

	assert(cpu_pt->SS == 0x3211);

	// register mode test
	cpu_pt->reset();
	mem_pt->reset();
	cpu_pt->A.X = 0x1111;

	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize memory
	mem_pt->writeB(0x40001, 0x8E);
	mem_pt->writeB(0x40002, 0xD0);
	cpu_pt->clock();

	assert(cpu_pt->SS == 0x1111);
}

void Tester::MOV_A_IN_B_Test() {
	cpu_pt->reset();
	mem_pt->reset();
	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize data segment
	cpu_pt->DS = 0x1000;
	// initialize memory
	mem_pt->writeB(0x40001, 0xA0);
	mem_pt->writeW(0x40002, 0x0AAA);
	mem_pt->writeB(0x10AAA, 0x32);
	cpu_pt->clock();

	assert(cpu_pt->A.L == 0x32);
}

void Tester::MOV_A_IN_W_Test() {
	cpu_pt->reset();
	mem_pt->reset();
	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize data segment
	cpu_pt->DS = 0x1000;
	// initialize memory
	mem_pt->writeB(0x40001, 0xA1);
	mem_pt->writeW(0x40002, 0x0AAA);
	mem_pt->writeW(0x10AAA, 0x3211);
	cpu_pt->clock();

	assert(cpu_pt->A.X == 0x3211);
}

void Tester::MOV_A_OUT_B_Test() {
	cpu_pt->reset();
	mem_pt->reset();
	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize data segment
	cpu_pt->DS = 0x1000;
	// initialize memory
	mem_pt->writeB(0x40001, 0xA2);
	mem_pt->writeW(0x40002, 0x0AAA);
	mem_pt->writeB(0x10AAA, 0x32);

	cpu_pt->A.L = 0x64;
	cpu_pt->clock();

	assert(mem_pt->readB(0x10AAA) == 0x64);
}

void Tester::MOV_A_OUT_W_Test() {
	cpu_pt->reset();
	mem_pt->reset();
	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize data segment
	cpu_pt->DS = 0x1000;
	// initialize memory
	mem_pt->writeB(0x40001, 0xA3);
	mem_pt->writeW(0x40002, 0x0AAA);
	mem_pt->writeW(0x10AAA, 0x0032);

	cpu_pt->A.X = 0x6432;
	cpu_pt->clock();

	assert(mem_pt->readW(0x10AAA) == 0x6432);
}


void Tester::MOV_R_IMM_B_Test() {
	cpu_pt->reset();
	mem_pt->reset();
	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	mem_pt->writeB(0x40001, 0xB0);
	mem_pt->writeB(0x40002, 0x12);
	cpu_pt->clock();

	assert(cpu_pt->A.L == 0x12);
}

void Tester::MOV_R_IMM_W_Test() {
	cpu_pt->reset();
	mem_pt->reset();
	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	mem_pt->writeB(0x40001, 0xBB);
	mem_pt->writeW(0x40002, 0x0A12);
	cpu_pt->clock();

	assert(cpu_pt->B.X == 0x0A12);
}

void Tester::MOV_MEM_IMM_B_Test() {
	cpu_pt->reset();
	mem_pt->reset();
	// initialize data segment
	cpu_pt->DS = 0x1000;

	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	mem_pt->writeB(0x40001, 0xC6);
	mem_pt->writeB(0x40002, 0x06);	// 00 - mod; 000 - reg; 110 - rm
	mem_pt->writeB(0x40003, 0x10);
	mem_pt->writeB(0x40004, 0x00);
	mem_pt->writeB(0x40005, 0x74);

	cpu_pt->clock();

	assert(mem_pt->readB(0x10010) == 0x74);
}

void Tester::MOV_MEM_IMM_W_Test() {
	cpu_pt->reset();
	mem_pt->reset();
	// initialize data segment
	cpu_pt->DS = 0x1000;

	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	mem_pt->writeB(0x40001, 0xC7);
	mem_pt->writeB(0x40002, 0x06);	// 00 - mod; 000 - reg; 110 - rm
	mem_pt->writeB(0x40003, 0x10);
	mem_pt->writeB(0x40004, 0x00);
	mem_pt->writeB(0x40005, 0x74);
	mem_pt->writeB(0x40006, 0x32);

	cpu_pt->clock();

	assert(mem_pt->readW(0x10010) == 0x3274);
}
