#include "EmulatorApp.h"
#include "MainFrame.h"
#include "CreateLabFrame.h"

bool EmulatorApp::OnInit() {
	MainFrame* frame = new MainFrame();
	CreateLabFrame* lab_frame = new CreateLabFrame();
	lab_frame->initEmulator();
	frame->initEmulator();
	frame->initLabFrame(lab_frame);
	frame->Show();
	return true;
}