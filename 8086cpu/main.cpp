#include <iostream>
#include "cpu8086.h"

using namespace std;

int main() {
	cpu8086 c;
	c.initOpTable();
	c.reset();

	c.opcode_table[0x40]();
	std::cout << c.A.X << std::endl;


	return 0;
}