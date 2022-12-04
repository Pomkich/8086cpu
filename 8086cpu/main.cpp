#include <iostream>
#include "cpu8086.h"

using namespace std;

int main() {
	std::shared_ptr<Memory> mem = std::make_shared<Memory>();
	cpu8086 c;
	c.initOpTable();
	c.initMemory(mem);
	c.reset();

	return 0;
}