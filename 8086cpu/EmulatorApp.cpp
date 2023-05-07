#include "EmulatorApp.h"
#include "MainFrame.h"
#include "CreateLabFrame.h"
#include "VerifyLabFrame.h"

bool EmulatorApp::OnInit() {
	MainFrame* frame = new MainFrame();
	CreateLabFrame* lab_frame = new CreateLabFrame();
	VerifyLabFrame* verify_frame = new VerifyLabFrame();
	lab_frame->initEmulator();
	verify_frame->initEmulator();
	frame->initEmulator();
	frame->initLabFrame(lab_frame);
	frame->initVerifyFrame(verify_frame);
	frame->Show();
	return true;
}