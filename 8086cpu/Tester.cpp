#include "Tester.h"

Tester::Tester() {
	cpu_pt = std::make_shared<cpu8086>();
	mem_pt = cpu_pt->getMemPt();
}

void Tester::RunTests() {

}