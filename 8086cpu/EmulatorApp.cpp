#include "EmulatorApp.h"
#include "MainFrame.h"

bool EmulatorApp::OnInit() {
	MainFrame* frame = new MainFrame();
	frame->Show();
	return true;
}