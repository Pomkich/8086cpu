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

}