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
	std::shared_ptr<AbstractPresenter> presenter_pt = std::make_shared<ConsolePresenter>(cpu_pt, mem_pt);
	cpu_pt->initMemory(mem_pt);
	cpu_pt->initPresenter(presenter_pt);
	cpu_pt->initOpTable();

	mem_pt->initPresenter(presenter_pt);


	// ������������� ���������
	cpu_pt->setRegVal(RegId::CS, 0x1000);
	cpu_pt->setRegVal(RegId::DS, 0x2000);
	cpu_pt->setRegVal(RegId::SS, 0x3000);
	// �������� ��������� � ������
	mem_pt->writeB(0x10000, 0xB9);		// opcode: sub
	mem_pt->writeB(0x10001, 0x21);
	mem_pt->writeB(0x10002, 0x00);
	mem_pt->writeB(0x10003, 0x83);
	mem_pt->writeB(0x10004, 0xC1);
	mem_pt->writeB(0x10005, 0xE0);
	mem_pt->writeB(0x10006, 0x06);
	mem_pt->writeB(0x10007, 0x00);
	mem_pt->writeB(0x10008, 0x00);
	mem_pt->writeB(0x10009, 0xE8);
	mem_pt->writeB(0x1000A, 0x03);
	// ����� ��������� 4 �������, 4 ���� ��������� ����
	for (int i = 0; i < 2; i++) {
		cpu_pt->clock();
	}
	

	return 0;
}