#include "TestingModule.h"

void GenerateLab(
	std::shared_ptr<cpu8086> cpu_pt,
	std::shared_ptr<Memory> mem_pt,
	std::shared_ptr<bool> running, 
	std::list<RegId> regs, int mem_start, int mem_end, std::string deskription) {
	// �������� �������� � ����
	// ������� ����:
	//		������������� ������� ������ ��� ��������� � ������
	//		�������� ������� ������
	//		��������� ���������� ���������
	//		��������� � �������� �������� ������
	// ������� ��� ���� �����

	word cs_copy = cpu_pt->getRegVal(RegId::CS);
	word ip_copy = cpu_pt->getRegVal(RegId::IP);

	for (int test = 0; test < 10; test++) {
		// ��������������� �������� ��������� (���� ������ ����)
		cpu_pt->setRegVal(RegId::CS, cs_copy);
		cpu_pt->setRegVal(RegId::IP, ip_copy);
		// ���������� ��������� ������ � ��������
		std::cout << "input:" << std::endl;
		dword value = 0;
		for (auto id : regs) {
			value = rand() % 65000;	// ~2^16
			cpu_pt->setRegVal(id, value);
			std::cout << value << std::endl;	// ������ ������� ������
		}
		// ���������� ��������� ������ � ������
		std::cout << "memory: " << std::endl;
		for (int address = mem_start; address < mem_end; address++) {
			mem_pt->writeB(address, rand() % 255);
			std::cout << mem_pt->readB(address);	// ������ � ����
		}

		// ��������� ���������
		*running = true;
		while (*running) {
			cpu_pt->clock();
		}

		std::cout << "out: " << std::endl;
		// ������ �������� �������� ���������
		for (auto id : regs) {
			value = cpu_pt->getRegVal(id);
			std::cout << value << std::endl;
		}
		// ������ �������� �������� ������
		std::cout << "memory: " << std::endl;
		for (int address = mem_start; address < mem_end; address++) {
			std::cout << mem_pt->readB(address);	// ������ � ����
		}
	}

}

void VerifyLab(std::string lab_file_name) {

}