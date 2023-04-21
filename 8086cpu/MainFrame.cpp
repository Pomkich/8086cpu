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
	// AX
	auto temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "AX: "),
		1, wxALIGN_CENTER);
	AH_field = new wxTextCtrl(this, wxID_ANY);
	AH_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	AL_field = new wxTextCtrl(this, wxID_ANY);
	AL_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	temp_sizer->Add(AH_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(AL_field, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// BX
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "BX: "),
		1, wxALIGN_CENTER);
	BH_field = new wxTextCtrl(this, wxID_ANY);
	BH_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	BL_field = new wxTextCtrl(this, wxID_ANY);
	BL_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	temp_sizer->Add(BH_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(BL_field, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// CX
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "CX: "),
		1, wxALIGN_CENTER);
	CH_field = new wxTextCtrl(this, wxID_ANY);
	CH_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	CL_field = new wxTextCtrl(this, wxID_ANY);
	CL_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	temp_sizer->Add(CH_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(CL_field, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// DX
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "DX: "),
		1, wxALIGN_CENTER);
	DH_field = new wxTextCtrl(this, wxID_ANY);
	DH_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	DL_field = new wxTextCtrl(this, wxID_ANY);
	DL_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	temp_sizer->Add(DH_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(DL_field, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// CS
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "CS:    "),
		1, wxALIGN_CENTER);
	CS_field = new wxTextCtrl(this, wxID_ANY);
	CS_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	temp_sizer->Add(CS_field, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// IP
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "IP:    "),
		1, wxALIGN_CENTER);
	IP_field = new wxTextCtrl(this, wxID_ANY);
	IP_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	temp_sizer->Add(IP_field, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// SS
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "SS:    "),
		1, wxALIGN_CENTER);
	SS_field = new wxTextCtrl(this, wxID_ANY);
	SS_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	temp_sizer->Add(SS_field, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// SP
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "SP:    "),
		1, wxALIGN_CENTER);
	SP_field = new wxTextCtrl(this, wxID_ANY);
	SP_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	temp_sizer->Add(SP_field, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// BP
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "BP:    "),
		1, wxALIGN_CENTER);
	BP_field = new wxTextCtrl(this, wxID_ANY);
	BP_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	temp_sizer->Add(BP_field, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// SI
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "SI:    "),
		1, wxALIGN_CENTER);
	SI_field = new wxTextCtrl(this, wxID_ANY);
	SI_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	temp_sizer->Add(SI_field, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// DI
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "DI:    "),
		1, wxALIGN_CENTER);
	DI_field = new wxTextCtrl(this, wxID_ANY);
	DI_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	temp_sizer->Add(DI_field, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// DS
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "DS:    "),
		1, wxALIGN_CENTER);
	DS_field = new wxTextCtrl(this, wxID_ANY);
	DS_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	temp_sizer->Add(DS_field, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// ES
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "ES:    "),
		1, wxALIGN_CENTER);
	ES_field = new wxTextCtrl(this, wxID_ANY);
	ES_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	temp_sizer->Add(ES_field, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// REGISTER SIZER END

	// CODE SIZER START
	code_field_sizer->Add(
		new wxStaticText(this, wxID_ANY, "Код программы"),
		0, wxALIGN_CENTER);
	code_editor = new wxTextCtrl(this, wxID_ANY, "");
	code_field_sizer->Add(code_editor, 1, wxEXPAND);
	// CODE SIZER END

	// MEMORY SIZER START
	mem_field_sizer->Add(
		new wxStaticText(this, wxID_ANY, "Память"),
		0, wxALIGN_CENTER);
	mem_dump = new wxTextCtrl(this, wxID_ANY, "");
	mem_field_sizer->Add(mem_dump, 1, wxEXPAND);
	// MEMORY SIZER END


	fields_sizer->Add(reg_field_sizer, 1, wxEXPAND | wxALL, GraphConst::base_border);
	fields_sizer->Add(code_field_sizer, 2, wxEXPAND | wxALL, GraphConst::base_border);
	fields_sizer->Add(mem_field_sizer, 1, wxEXPAND | wxALL, GraphConst::base_border);

	main_sizer->Add(buttons_sizer, 1, wxEXPAND | wxALL, GraphConst::base_border);
	main_sizer->Add(fields_sizer, 7, wxEXPAND | wxALL, GraphConst::base_border);

	SetSizer(main_sizer);
	SetBackgroundColour(wxColour(100, 200, 100));
}