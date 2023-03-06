#include "Tester.h"

Tester::Tester() {
	cpu_pt = std::make_shared<cpu8086>();
	mem_pt = std::make_shared<Memory>();
	cpu_pt->initMemory(mem_pt);
	cpu_pt->initOpTable();
	cpu_pt->reset();
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
	INC_R_Test();
	DEC_R_Test();
	PUSH_R_Test();
	POP_R_Test();
}

void Tester::MemoryCheckByteWriting() {
	mem_pt->writeB(0x1000, 0x32);
	assert(mem_pt->readB(0x1000), 0x32);
}

void Tester::MemoryCheckWordWriting() {
	mem_pt->writeW(0x1000, 0x3264);
	assert(mem_pt->readW(0x1000), 0x3264);
}

void Tester::MemoryCheckBytePlacement() {
	mem_pt->writeW(0x1000, 0x3264);
	assert(mem_pt->readB(0x1000), 0x64);
	assert(mem_pt->readB(0x1000), 0x32);
}

void Tester::FlagOTest() {
	// overflow occured
	cpu_pt->testFlagO(1, 0);
	assert(cpu_pt->getFlag(Flag::O) == 1);
	cpu_pt->testFlagO(0, 1);
	assert(cpu_pt->getFlag(Flag::O) == 1);
	// overflow didn't occured
	cpu_pt->testFlagO(1, 1);
	assert(cpu_pt->getFlag(Flag::O) == 0);
	cpu_pt->testFlagO(0, 0);
	assert(cpu_pt->getFlag(Flag::O) == 0);
}

void Tester::FlagSTest() {
	// byte values
	cpu_pt->testFlagS(0b10000000, false);
	assert(cpu_pt->getFlag(Flag::S) == true);
	cpu_pt->testFlagS(0b00101100, false);
	assert(cpu_pt->getFlag(Flag::S) == false);
	// word values
	cpu_pt->testFlagS(0xFFFF, true);
	assert(cpu_pt->getFlag(Flag::S) == true);
	cpu_pt->testFlagS(0x0FFF, true);
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
	cpu_pt->testFlagP(0xF0, false);
	assert(cpu_pt->getFlag(Flag::P) == true);
	cpu_pt->testFlagP(0xF1, false);
	assert(cpu_pt->getFlag(Flag::P) == false);

	// word values
	cpu_pt->testFlagP(0xFF00, true);
	assert(cpu_pt->getFlag(Flag::P) == true);
	cpu_pt->testFlagP(0xFF01, true);
	assert(cpu_pt->getFlag(Flag::P) == false);
}

void Tester::FlagCTest() {
	// byte functions
	cpu_pt->testFlagCAddB(0xFF, 0xFE);
	cpu_pt->testFlagAAdd(0xFF, 0xFE);
	assert(cpu_pt->getFlag(Flag::C) == 1);
	assert(cpu_pt->getFlag(Flag::A) == 1);

	cpu_pt->testFlagCSubB(0x00, 0xFF);
	assert(cpu_pt->getFlag(Flag::C) == 1);

	cpu_pt->testFlagCAddB(0xEE, 0xEF);
	assert(cpu_pt->getFlag(Flag::C) == 0);

	cpu_pt->testFlagCSubB(0xEE, 0xEA);
	assert(cpu_pt->getFlag(Flag::C) == 0);

	cpu_pt->testFlagCSubB(0xEE, 0xEF);
	cpu_pt->testFlagASub(0xEE, 0xEF);
	assert(cpu_pt->getFlag(Flag::C) == 1);
	assert(cpu_pt->getFlag(Flag::A) == 1);

	// word functions
	cpu_pt->testFlagCAddW(0xF000, 0x0004);
	assert(cpu_pt->getFlag(Flag::C) == 1);
	cpu_pt->testFlagCSubW(0x0000, 0xF000);
	assert(cpu_pt->getFlag(Flag::C) == 1);
	cpu_pt->testFlagCAddW(0x7000, 0x7AFF);
	assert(cpu_pt->getFlag(Flag::C) == 0);
	cpu_pt->testFlagCSubW(0xFFFF, 0xFF00);
	assert(cpu_pt->getFlag(Flag::C) == 0);
}

void Tester::ADD_R_IN_B_Test() {
	// adding value from memory to register
	cpu_pt->reset();
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

void Tester::ADD_R_OUT_B_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
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

void Tester::ADD_R_IN_W_Test() {
	// adding value from memory to register
	cpu_pt->reset();
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

void Tester::ADD_R_OUT_W_Test() {
	// adding value from register to memory by address
	cpu_pt->reset();
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

void Tester::ADD_A_B_Test() {
	// adding value accumulator byte
	cpu_pt->reset();
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

void Tester::INC_R_Test() {
	cpu_pt->reset();
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
	assert(mem_pt->readB(0x10002) == 0x34 && mem_pt->readB(0x10001) == 0x12);
}

void Tester::POP_R_Test() {
	cpu_pt->reset();
	// initialize code segment
	cpu_pt->CS = 0x4000;
	cpu_pt->IP = 0x0001;
	// initialize stack segment
	cpu_pt->SS = 0x1000;
	cpu_pt->SP = 0x0004;
	mem_pt->writeB(0x40001, 0x58);
	mem_pt->writeB(0x10004, 0x35);
	mem_pt->writeB(0x10003, 0xFF);
	cpu_pt->A.X = 0x1234;
	cpu_pt->clock();

	assert(cpu_pt->A.X == 0xFF35);
}