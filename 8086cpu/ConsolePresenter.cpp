#include "ConsolePresenter.h"

ConsolePresenter::ConsolePresenter(std::shared_ptr<cpu8086> p_cpu, std::shared_ptr<Memory> p_mem) {
	cpu_pt = p_cpu;
	mem_pt = p_mem;
}

void ConsolePresenter::notifyRegChange() {
	system("clear");
	Render();
}

void ConsolePresenter::notifyMemChange() {
	system("clear");
	Render();
}

void ConsolePresenter::notifyStkChange() {
	system("clear");
	Render();
}

void ConsolePresenter::Render() {
	// registers segment
	std::cout << std::hex;
	std::cout << "AX: " << cpu_pt->A.X << std::endl;
	std::cout << "BX: " << cpu_pt->B.X << std::endl;
	std::cout << "CX: " << cpu_pt->C.X << std::endl;
	std::cout << "DX: " << cpu_pt->D.X << std::endl;
	std::cout << "CS: " << cpu_pt->CS << std::endl;
	std::cout << "IP: " << cpu_pt->IP << std::endl;
	std::cout << "SS: " << cpu_pt->SS << std::endl;
	std::cout << "SP: " << cpu_pt->SP << std::endl;
	std::cout << "BP: " << cpu_pt->BP << std::endl;
	std::cout << "SI: " << cpu_pt->SI << std::endl;
	std::cout << "DI: " << cpu_pt->DI << std::endl;
	std::cout << "DS: " << cpu_pt->DS << std::endl;
	std::cout << "ES: " << cpu_pt->ES << std::endl;
}