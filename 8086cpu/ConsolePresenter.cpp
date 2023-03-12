#include "ConsolePresenter.h"

ConsolePresenter::ConsolePresenter(std::shared_ptr<cpu8086> p_cpu, std::shared_ptr<Memory> p_mem) {
	cpu_pt = p_cpu;
	mem_pt = p_mem;
}

void ConsolePresenter::notifyRegChange() {
	system("cls");
	Render();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void ConsolePresenter::notifyMemChange() {
	system("cls");
	Render();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void ConsolePresenter::notifyStkChange() {
	system("cls");
	Render();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void ConsolePresenter::Render() {
	// registers segment
	dword stack_begin = ((dword)cpu_pt->getRegVal(RegId::SS) << 4) + cpu_pt->getRegVal(RegId::SP);
	dword data_begin = cpu_pt->getRegVal(RegId::DS) << 4;
	dword code_begin = ((dword)cpu_pt->getRegVal(RegId::CS) << 4);

	std::cout << std::hex;
	std::cout << "registers" << "\t\t" << "stack dump" << std::endl;
	std::cout << "AX: " << cpu_pt->getRegVal(RegId::AX) << "    \t\t" << mem_pt->readW(stack_begin) << std::endl;
	std::cout << "BX: " << cpu_pt->getRegVal(RegId::BX) << "    \t\t" << mem_pt->readW(stack_begin + 1) << std::endl;
	std::cout << "CX: " << cpu_pt->getRegVal(RegId::CX) << "    \t\t" << mem_pt->readW(stack_begin + 2) << std::endl;
	std::cout << "DX: " << cpu_pt->getRegVal(RegId::DX) << "    \t\t" << mem_pt->readW(stack_begin + 3) << std::endl;
	std::cout << "CS: " << cpu_pt->getRegVal(RegId::CS) << "    \t\t" << mem_pt->readW(stack_begin + 4) << std::endl;
	std::cout << "IP: " << cpu_pt->getRegVal(RegId::IP) << "    \t\t" << mem_pt->readW(stack_begin + 5) << std::endl;
	std::cout << "SS: " << cpu_pt->getRegVal(RegId::SS) << "    \t\t" << mem_pt->readW(stack_begin + 6) << std::endl;
	std::cout << "SP: " << cpu_pt->getRegVal(RegId::SP) << "    \t\t" << mem_pt->readW(stack_begin + 7) << std::endl;
	std::cout << "BP: " << cpu_pt->getRegVal(RegId::BP) << "    \t\t" << mem_pt->readW(stack_begin + 8) << std::endl;
	std::cout << "SI: " << cpu_pt->getRegVal(RegId::SI) << "    \t\t" << mem_pt->readW(stack_begin + 9) << std::endl;
	std::cout << "DI: " << cpu_pt->getRegVal(RegId::DI) << "    \t\t" << mem_pt->readW(stack_begin + 10) << std::endl;
	std::cout << "DS: " << cpu_pt->getRegVal(RegId::DS) << "    \t\t" << mem_pt->readW(stack_begin + 11) << std::endl;
	std::cout << "ES: " << cpu_pt->getRegVal(RegId::ES) << "    \t\t" << mem_pt->readW(stack_begin + 12) << std::endl;

	std::cout << "memory dump" << std::endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			std::cout << (int)mem_pt->readB(data_begin + (i * 8 + j)) << " ";
		}
		std::cout << std::endl;
	}

	std::cout << "code dump" << std::endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			std::cout << (int)mem_pt->readB(code_begin + (i * 8 + j)) << " ";
		}
		std::cout << std::endl;
	}
}