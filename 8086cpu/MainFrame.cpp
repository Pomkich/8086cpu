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
	buttons_sizer->Add(button, 1, wxALIGN_CENTER | wxALL, GraphConst::base_border);

	// REGISTER SIZER START
	reg_field_sizer->Add(
		new wxStaticText(this, wxID_ANY, "Регистры"),
		0, wxALIGN_CENTER);
	auto temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "AX: "),
		0, wxALIGN_CENTER);
	AH_field = new wxTextCtrl(this, wxID_ANY);
	AH_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	AL_field = new wxTextCtrl(this, wxID_ANY);
	AL_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	temp_sizer->Add(AH_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(AL_field, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);

	// code sizer
	code_field_sizer->Add(
		new wxStaticText(this, wxID_ANY, "Код программы"),
		0, wxALIGN_CENTER);
	code_editor = new wxTextCtrl(this, wxID_ANY, "");
	code_field_sizer->Add(code_editor, 1, wxEXPAND);


	// memory sizer
	mem_field_sizer->Add(
		new wxStaticText(this, wxID_ANY, "Память"),
		0, wxALIGN_CENTER);
	mem_dump = new wxTextCtrl(this, wxID_ANY, "");
	mem_field_sizer->Add(mem_dump, 1, wxEXPAND);

	fields_sizer->Add(reg_field_sizer, 1, wxEXPAND | wxALL, GraphConst::base_border);
	fields_sizer->Add(code_field_sizer, 2, wxEXPAND | wxALL, GraphConst::base_border);
	fields_sizer->Add(mem_field_sizer, 1, wxEXPAND | wxALL, GraphConst::base_border);

	main_sizer->Add(buttons_sizer, 1, wxEXPAND | wxALL, GraphConst::base_border);
	main_sizer->Add(fields_sizer, 7, wxEXPAND | wxALL, GraphConst::base_border);

	SetSizer(main_sizer);
	SetBackgroundColour(wxColour(100, 200, 100));
}