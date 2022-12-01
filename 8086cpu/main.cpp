#include <iostream>
#include "cpu8086.h"

using namespace std;

int main() {
	cpu8086 c;
	c.initOpTable();
	c.reset();

	c.opcode_table[0x44]();
	std::cout << c.SP << std::endl;


	return 0;
}