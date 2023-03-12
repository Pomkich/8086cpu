#include <iostream>
#include "Tester.h"
#include "cpu8086.h"
#include "Memory.h"
#include "AbstractPresenter.h"
#include "ConsolePresenter.h"

using namespace std;

int main() {
	Tester test;
	test.RunTests();

	std::shared_ptr<cpu8086> cpu_pt = std::make_shared<cpu8086>();
	std::shared_ptr<Memory> mem_pt = std::make_shared<Memory>();
	std::shared_ptr<ConsolePresenter> presenter_pt = std::make_shared<ConsolePresenter>(cpu_pt, mem_pt);
	cpu_pt->initMemory(mem_pt);
	cpu_pt->initPresenter(presenter_pt);
	cpu_pt->initOpTable();

	mem_pt->initPresenter(presenter_pt);

	// инициализация сегментов
	cpu_pt->CS = 0x1000;
	cpu_pt->DS = 0x2000;
	// загрузка программы в память
	mem_pt->writeB(0x10000, 0xB8);
	mem_pt->writeB(0x10001, 0x32);
	mem_pt->writeB(0x10002, 0x00);
	mem_pt->writeB(0x10003, 0xBB);
	mem_pt->writeB(0x10004, 0x64);
	mem_pt->writeB(0x10005, 0x00);
	mem_pt->writeB(0x10006, 0x03);
	mem_pt->writeB(0x10007, 0xC3);
	mem_pt->writeB(0x10008, 0xA3);
	mem_pt->writeB(0x10009, 0x64);
	mem_pt->writeB(0x10010, 0x00);
	// всего загружено 4 команды, 4 раза выполняем цикл
	for (int i = 0; i < 4; i++) {
		cpu_pt->clock();
	}
	

	return 0;
}