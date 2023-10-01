#include "DescriptionFrame.h"

wxBEGIN_EVENT_TABLE(DescriptionFrame, wxFrame)
	EVT_CLOSE(DescriptionFrame::OnClose)
wxEND_EVENT_TABLE()

DescriptionFrame::DescriptionFrame() : wxFrame(nullptr, wxID_ANY, "8086 emulator") {
	main_sizer = new wxBoxSizer(wxVERTICAL);
	lab_name = new wxTextCtrl(this, wxID_ANY, "", 
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
	description = new wxTextCtrl(this, wxID_ANY, "",
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

	lab_name->SetMinSize(wxSize(GraphConst::lab_width, GraphConst::lab_height));
	description->SetMinSize(wxSize(GraphConst::lab_width, GraphConst::description_height));

	main_sizer->Add(lab_name, 0, wxEXPAND);
	main_sizer->Add(description, 1, wxEXPAND);

	SetSizer(main_sizer);
	Fit();
}

void DescriptionFrame::SetDescription(wxString desc) {
	description->SetValue(desc);
}

void DescriptionFrame::SetLabName(wxString lb_nm) {
	lab_name->SetValue(lb_nm);
}

void DescriptionFrame::OnClose(wxCloseEvent& evt) {
	Show(false);
}