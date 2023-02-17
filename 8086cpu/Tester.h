#pragma once
#include "Constants.h"
#include "cpu8086.h"
#include "Memory.h"
#include <memory>
#include <cassert>


class Tester {
private:
	std::shared_ptr<cpu8086> cpu_pt;
	std::shared_ptr<Memory> mem_pt;

public:
	Tester();

	void RunTests();

	/*** MEMORY TESTS BEGIN ***/
	void MemoryCheckByteWriting();
	void MemoryCheckWordWriting();
	void MemoryCheckBytePlacement();
	/*** MEMORY TESTS END   ***/

	/*** FLAG TESTS BEGIN ***/
	void FlagOTest();
	void FlagSTest();
	void FlagZTest();
	void FlagATest();
	void FlagPTest();
	void FlagCTest();
	/*** FLAG TESTS END   ***/

	/*** OPCODE TESTS BEGIN ***/
	/*** OPCODE TESTS END   ***/
};

