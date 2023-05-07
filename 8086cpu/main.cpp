//#include <wx/wx.h>
//#include "EmulatorApp.h"

//wxIMPLEMENT_APP(EmulatorApp);

#include "Tester.h"
#include "cpu8086.h"
#include "Memory.h"
#include "AbstractPresenter.h"
#include "ConsolePresenter.h"
#include "TestingModule.h"

using namespace std;


class FakePresenter : public AbstractPresenter {
public:
	void notifyMemChange() override {};
	void notifyRegChange() override {};
	void notifyStkChange() override {};
	void Render() override {};

	FakePresenter(std::shared_ptr<bool> r) { running = r; *running = true; }
	void notifyHalt() { *running = false; }

	std::shared_ptr<bool> running;
};

int main() {
	std::shared_ptr<cpu8086> cpu_pt = std::make_shared<cpu8086>();
	std::shared_ptr<Memory> mem_pt = std::make_shared<Memory>();
	std::shared_ptr<bool> running = std::make_shared<bool>();
	FakePresenter* pres = new FakePresenter(running);
	cpu_pt->initOpTable();
	cpu_pt->initMemory(mem_pt);
	cpu_pt->initPresenter(pres);
	mem_pt->initPresenter(pres);
	*running = true;
	
	cpu_pt->setRegVal(RegId::CS, 0x1000);

	mem_pt->writeB(0x10000, 0x03);
	mem_pt->writeB(0x10001, 0xC3);
	mem_pt->writeB(0x10002, 0xF4);	// HLT

	std::list<RegId> regs = { RegId::AX, RegId::BX };

	//GenerateLab(cpu_pt, mem_pt, running, regs, 100, 103, "lab1", "");

	if (VerifyLab(cpu_pt, mem_pt, running, "lab1.json")) {
		std::cout << "lab done" << std::endl;
	}
	else {
		std::cout << "lab not done" << std::endl;
	}


	return 0;
}