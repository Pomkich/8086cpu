#include "ConsolePresenter.h"

ConsolePresenter::ConsolePresenter(std::shared_ptr<cpu8086> p_cpu, std::shared_ptr<Memory> p_mem) {
	cpu_pt = p_cpu;
	mem_pt = p_mem;
}

void ConsolePresenter::notifyRegChange() {
	system("cls");
	Render();
	//std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void ConsolePresenter::notifyMemChange() {
	system("cls");
	Render();
	//std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void ConsolePresenter::notifyStkChange() {
	system("cls");
	Render();
	//std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void ConsolePresenter::Render() {
	// registers segment
	std::cout << std::hex;
	std::cout << "AX: " << cpu_pt->getRegVal(RegId::AX) << std::endl;
	std::cout << "BX: " << cpu_pt->getRegVal(RegId::BX) << std::endl;
	std::cout << "CX: " << cpu_pt->getRegVal(RegId::CX) << std::endl;
	std::cout << "DX: " << cpu_pt->getRegVal(RegId::DX) << std::endl;
	std::cout << "CS: " << cpu_pt->getRegVal(RegId::CS) << std::endl;
	std::cout << "IP: " << cpu_pt->getRegVal(RegId::IP) << std::endl;
	std::cout << "SS: " << cpu_pt->getRegVal(RegId::SS) << std::endl;
	std::cout << "SP: " << cpu_pt->getRegVal(RegId::SP) << std::endl;
	std::cout << "BP: " << cpu_pt->getRegVal(RegId::BP) << std::endl;
	std::cout << "SI: " << cpu_pt->getRegVal(RegId::SI) << std::endl;
	std::cout << "DI: " << cpu_pt->getRegVal(RegId::DI) << std::endl;
	std::cout << "DS: " << cpu_pt->getRegVal(RegId::DS) << std::endl;
	std::cout << "ES: " << cpu_pt->getRegVal(RegId::ES) << std::endl;
}