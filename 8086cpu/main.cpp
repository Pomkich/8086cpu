#include <iostream>
#include "Tester.h"
#include "cpu8086.h"
#include "ConsolePresenter.h"

using namespace std;

int main() {
	Tester test;
	test.RunTests();

	/*std::shared_ptr<cpu8086> cpu_pt;
	std::shared_ptr<Memory> mem_pt;
	cpu_pt->initMemory(mem_pt);
	cpu_pt->initOpTable();

	ConsolePresenter pres(cpu_pt, mem_pt);*/
	

	return 0;
}