#include "MainFrame.h"
#include <iostream>

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "8086 emulator") {
	SetSize(800, 600);

	main_sizer = new wxBoxSizer(wxVERTICAL);
	buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
	fields_sizer = new wxBoxSizer(wxHORIZONTAL);
	reg_field_sizer = new wxBoxSizer(wxVERTICAL);
	code_field_sizer = new wxBoxSizer(wxVERTICAL);
	mem_field_sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* reg_hint, * code_hint, * mem_hint;
	reg_hint = new wxStaticText(this, wxID_ANY, "Регистры");
	code_hint = new wxStaticText(this, wxID_ANY, "Код программы");
	mem_hint = new wxStaticText(this, wxID_ANY, "Дамп памяти");

	wxButton* test_button1 = new wxButton(this, wxID_ANY, "start", wxDefaultPosition, wxDefaultSize, 0);
	wxButton* test_button2 = new wxButton(this, wxID_ANY, "stop", wxDefaultPosition, wxDefaultSize, 0);

	buttons_sizer->Add(test_button1, 0, wxALL, 5);
	buttons_sizer->Add(test_button2, 0, wxALL, 5);

	reg_field_sizer->Add(reg_hint, 0, wxALIGN_CENTER, 5);
	code_field_sizer->Add(code_hint, 0, wxALIGN_CENTER, 5);
	mem_field_sizer->Add(mem_hint, 0, wxALIGN_CENTER, 5);

	AX_label = new wxStaticText(this, wxID_ANY, "AX:");
	AH_field = new wxTextCtrl(this, wxID_ANY);
	AL_field = new wxTextCtrl(this, wxID_ANY);

	wxBoxSizer* reg_sizer = new wxBoxSizer(wxHORIZONTAL);
	reg_sizer->Add(AX_label, 0, wxALIGN_CENTER, 5);
	reg_sizer->Add(AH_field, 0, wxALIGN_CENTER, 5);
	reg_sizer->Add(AL_field, 0, wxALIGN_CENTER, 5);

	reg_field_sizer->Add(reg_sizer);

	fields_sizer->Add(reg_field_sizer, 2, wxALIGN_CENTER | wxALIGN_TOP, 20);
	fields_sizer->Add(code_field_sizer, 2, wxALIGN_CENTER | wxALIGN_TOP, 20);
	fields_sizer->Add(mem_field_sizer, 2, wxALIGN_CENTER | wxALIGN_TOP, 20);

	main_sizer->Add(buttons_sizer, 1, wxALIGN_CENTER, 5);
	main_sizer->Add(fields_sizer, 1, wxALIGN_CENTER, 5);

	SetSizer(main_sizer);
}