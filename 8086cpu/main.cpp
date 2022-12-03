#include <iostream>
#include "cpu8086.h"

using namespace std;

int main() {
	std::shared_ptr<Memory> mem = std::make_shared<Memory>();
	cpu8086 c;
	c.initOpTable();
	c.initMemory(mem);
	c.reset();

	mem->write(0xFFFF0, 0x40);
	mem->write(0xFFFF1, 0x40);
	std::cout << c.A.X << std::endl;
	c.clock();
	c.clock();
	std::cout << c.A.X << std::endl;


	return 0;
}