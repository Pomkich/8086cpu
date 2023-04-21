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

	auto button = new wxButton(this, wxID_ANY, "button");
	buttons_sizer->Add(button, 1, wxALIGN_CENTER | wxALL, 5);

	fields_sizer->Add(reg_field_sizer, 1, wxEXPAND | wxALL, 5);
	fields_sizer->Add(code_field_sizer, 2, wxEXPAND | wxALL, 5);
	fields_sizer->Add(mem_field_sizer, 1, wxEXPAND | wxALL, 5);

	main_sizer->Add(buttons_sizer, 1, wxEXPAND | wxALL, 5);
	main_sizer->Add(fields_sizer, 7, wxEXPAND | wxALL, 5);

	SetSizer(main_sizer);
	SetBackgroundColour(wxColour(100, 200, 100));
}