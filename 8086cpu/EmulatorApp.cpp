#include "EmulatorApp.h"
#include "MainFrame.h"
#include "CreateLabFrame.h"
#include "VerifyLabFrame.h"
#include "DescriptionFrame.h"

bool EmulatorApp::OnInit() {
	MainFrame* frame = new MainFrame();
	CreateLabFrame* lab_frame = new CreateLabFrame();
	VerifyLabFrame* verify_frame = new VerifyLabFrame();
	DescriptionFrame* desc_frame = new DescriptionFrame();
	lab_frame->initEmulator();
	verify_frame->initEmulator();
	frame->initEmulator();
	frame->initLabFrame(lab_frame);
	frame->initVerifyFrame(verify_frame);
	frame->initDescFrame(desc_frame);
	frame->Show();
	return true;
}