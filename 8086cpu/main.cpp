#include <iostream>
#include "cpu8086.h"

using namespace std;

int main() {
	std::shared_ptr<Memory> mem = std::make_shared<Memory>();
	cpu8086 c;
	c.initOpTable();
	c.initMemory(mem);
	c.reset();

	c.A.X = 0x1234;
	c.SS = 0x1000;
	c.SP = 0x100;

	mem->writeB(0xFFFF0, 0x50);
	mem->writeB(0xFFFF1, 0x58);
	c.clock();
	c.A.X = 0;
	std::cout << std::hex << c.A.X << std::endl;
	c.clock();
	std::cout << std::hex << c.A.X << std::endl;

	return 0;
}