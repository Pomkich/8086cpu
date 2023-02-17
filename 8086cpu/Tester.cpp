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
