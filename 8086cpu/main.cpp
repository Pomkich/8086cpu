#include <iostream>
#include "cpu8086.h"

using namespace std;

int main() {
	cpu8086 c;
	c.reset();

	std::cout << c.getFlag(Flag::O);


	return 0;
}