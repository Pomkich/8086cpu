#include <iostream>
#include "cpu8086.h"

using namespace std;

int main() {
	std::shared_ptr<Memory> mem = std::make_shared<Memory>();
	cpu8086 c;
	c.initOpTable();
	c.initMemory(mem);
	c.reset();

	c.A.X = 132;
	mem->writeB(0xFFFF0, 0x01);
	mem->writeB(0xFFFF1, 0x06);
	mem->writeB(0xFFFF2, 0x64);
	mem->writeB(0xFFFF3, 0x00);
	mem->writeB(0x00064, 123);
	c.clock();
	std::cout << mem->readW(0x00064) << std::endl;

	return 0;
}