#include <iostream>
#include "cpu8086.h"

using namespace std;

int main() {
	cpu8086 c;

	c.A.setH(0x12);
	c.A.setL(0x34);
	std::cout << std::hex << c.A.getX();
	std::cout << std::endl;
	std::cout << std::hex << (int)c.A.getH();
	std::cout << std::endl;
	std::cout << std::hex << (int)c.A.getL();

	c.A.setX(0x3123);
	std::cout << std::endl;
	std::cout << std::hex << c.A.getX();
	std::cout << std::endl;
	std::cout << std::hex << (int)c.A.getH();
	std::cout << std::endl;
	std::cout << std::hex << (int)c.A.getL();

	return 0;
}