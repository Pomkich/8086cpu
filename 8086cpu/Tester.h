#pragma once
#include "Constants.h"
#include "cpu8086.h"
#include "Memory.h"
#include <memory>


class Tester {
private:
	std::shared_ptr<cpu8086> cpu_pt;
	std::shared_ptr<cpu8086> mem_pt;

public:
	Tester();

	void RunTests();

	/*** MEMORY TESTS BEGIN ***/
	/*** MEMORY TESTS END   ***/

	/*** FLAG TESTS BEGIN ***/
	/*** FLAG TESTS END   ***/

	/*** OPCODE TESTS BEGIN ***/
	/*** OPCODE TESTS END   ***/
};

