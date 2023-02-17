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