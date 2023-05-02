#include "CreateLabFrame.h"

wxBEGIN_EVENT_TABLE(CreateLabFrame, wxFrame)
	EVT_BUTTON(GraphConst::ButtonsIDs::LOAD, CreateLabFrame::OnLoadButton)
	EVT_TEXT_ENTER(GraphConst::FieldIDs::START_ADDRESS, CreateLabFrame::OnStartAddressChange)
	EVT_CLOSE(CreateLabFrame::OnClose)
	EVT_TEXT(GraphConst::FieldIDs::AH, CreateLabFrame::OnByteFieldChange)
	EVT_TEXT(GraphConst::FieldIDs::AL, CreateLabFrame::OnByteFieldChange)
	EVT_TEXT(GraphConst::FieldIDs::BH, CreateLabFrame::OnByteFieldChange)
	EVT_TEXT(GraphConst::FieldIDs::BL, CreateLabFrame::OnByteFieldChange)
	EVT_TEXT(GraphConst::FieldIDs::CH, CreateLabFrame::OnByteFieldChange)
	EVT_TEXT(GraphConst::FieldIDs::CL, CreateLabFrame::OnByteFieldChange)
	EVT_TEXT(GraphConst::FieldIDs::DH, CreateLabFrame::OnByteFieldChange)
	EVT_TEXT(GraphConst::FieldIDs::DL, CreateLabFrame::OnByteFieldChange)
wxEND_EVENT_TABLE()

// вспомогательные функции
static std::string int_to_hex(dword i) {
	std::stringstream stream;
	stream << std::hex << i;
	return stream.str();
}

static int hex_to_int(std::string hexVal) {
	int len = hexVal.size();
	int base = 1;
	int dec_val = 0;

	for (int i = len - 1; i >= 0; i--) {
		if (hexVal[i] >= '0' && hexVal[i] <= '9') {
			dec_val += (int(hexVal[i]) - 48) * base;
			base = base * 16;
		}
		else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
			dec_val += (int(hexVal[i]) - 55) * base;
			base = base * 16;
		}
	}
	return dec_val;
}

CreateLabFrame::CreateLabFrame() : wxFrame(nullptr, wxID_ANY, "8086 emulator") {
	SetSize(GraphConst::screen_width, GraphConst::screen_height);

	main_sizer = new wxBoxSizer(wxVERTICAL);
	buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
	fields_sizer = new wxBoxSizer(wxHORIZONTAL);
	reg_field_sizer = new wxBoxSizer(wxVERTICAL);
	code_field_sizer = new wxBoxSizer(wxVERTICAL);
	mem_field_sizer = new wxBoxSizer(wxVERTICAL);

	// BUTTON SIZER START
	generate_button = new wxButton(this, GraphConst::ButtonsIDs::CLOCK, "Сгенерировать");
	load_button = new wxButton(this, GraphConst::ButtonsIDs::LOAD, "Загрузить");
	buttons_sizer->Add(generate_button, 1, wxALIGN_CENTER | wxALL, GraphConst::base_border);
	buttons_sizer->Add(load_button, 1, wxALIGN_CENTER | wxALL, GraphConst::base_border);
	// BUTTON SIZER END

	// REGISTER SIZER START
	reg_field_sizer->Add(
		new wxStaticText(this, wxID_ANY, "Регистры"),
		0, wxALIGN_CENTER | wxALL, GraphConst::base_border);
	// AX
	auto temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "AX: "),
		1, wxALIGN_CENTER);
	AH_field = new wxTextCtrl(this, GraphConst::FieldIDs::AH, "00");
	AH_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	AL_field = new wxTextCtrl(this, GraphConst::FieldIDs::AL, "00");
	AL_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	AX_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::AX, "");
	temp_sizer->Add(AH_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(AL_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(AX_check, 0, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// BX
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "BX: "),
		1, wxALIGN_CENTER);
	BH_field = new wxTextCtrl(this, GraphConst::FieldIDs::BH, "00");
	BH_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	BL_field = new wxTextCtrl(this, GraphConst::FieldIDs::BL, "00");
	BL_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	BX_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::BX, "");
	temp_sizer->Add(BH_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(BL_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(BX_check, 0, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// CX
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "CX: "),
		1, wxALIGN_CENTER);
	CH_field = new wxTextCtrl(this, GraphConst::FieldIDs::CH, "00");
	CH_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	CL_field = new wxTextCtrl(this, GraphConst::FieldIDs::CL, "00");
	CL_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	CX_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::CX, "");
	temp_sizer->Add(CH_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(CL_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(CX_check, 0, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// DX
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "DX: "),
		1, wxALIGN_CENTER);
	DH_field = new wxTextCtrl(this, GraphConst::FieldIDs::DH, "00");
	DH_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	DL_field = new wxTextCtrl(this, GraphConst::FieldIDs::DL, "00");
	DL_field->SetMinSize(wxSize(GraphConst::byte_field_width, GraphConst::byte_field_height));
	DX_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::DX, "");
	temp_sizer->Add(DH_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(DL_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(DX_check, 0, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// CS
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "CS:    "),
		1, wxALIGN_CENTER);
	CS_field = new wxTextCtrl(this, wxID_ANY, "0000");
	CS_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	CS_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::CS, "");
	temp_sizer->Add(CS_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(CS_check, 0, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// IP
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "IP:    "),
		1, wxALIGN_CENTER);
	IP_field = new wxTextCtrl(this, wxID_ANY, "0000");
	IP_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	IP_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::IP, "");
	temp_sizer->Add(IP_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(IP_check, 0, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// SS
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "SS:    "),
		1, wxALIGN_CENTER);
	SS_field = new wxTextCtrl(this, wxID_ANY, "0000");
	SS_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	SS_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::SS, "");
	temp_sizer->Add(SS_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(SS_check, 0, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// SP
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "SP:    "),
		1, wxALIGN_CENTER);
	SP_field = new wxTextCtrl(this, wxID_ANY, "0000");
	SP_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	SP_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::SP, "");
	temp_sizer->Add(SP_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(SP_check, 0, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// BP
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "BP:    "),
		1, wxALIGN_CENTER);
	BP_field = new wxTextCtrl(this, wxID_ANY, "0000");
	BP_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	BP_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::BP, "");
	temp_sizer->Add(BP_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(BP_check, 0, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// SI
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "SI:    "),
		1, wxALIGN_CENTER);
	SI_field = new wxTextCtrl(this, wxID_ANY, "0000");
	SI_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	SI_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::SI, "");
	temp_sizer->Add(SI_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(SI_check, 0, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// DI
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "DI:    "),
		1, wxALIGN_CENTER);
	DI_field = new wxTextCtrl(this, wxID_ANY, "0000");
	DI_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	DI_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::DI, "");
	temp_sizer->Add(DI_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(DI_check, 0, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// DS
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "DS:    "),
		1, wxALIGN_CENTER);
	DS_field = new wxTextCtrl(this, wxID_ANY, "0000");
	DS_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	DS_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::DS, "");
	temp_sizer->Add(DS_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(DS_check, 0, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// ES
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "ES:    "),
		1, wxALIGN_CENTER);
	ES_field = new wxTextCtrl(this, wxID_ANY, "0000");
	ES_field->SetMinSize(wxSize(GraphConst::word_field_width, GraphConst::word_field_height));
	ES_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::ES, "");
	temp_sizer->Add(ES_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(ES_check, 0, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// flag register
	reg_field_sizer->Add(new wxStaticText(this, wxID_ANY, "Регистр флагов"), 0, wxALIGN_CENTER);
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, " O "), 1, wxALIGN_CENTER);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, " S "), 1, wxALIGN_CENTER);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, " Z "), 1, wxALIGN_CENTER);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, " P "), 1, wxALIGN_CENTER);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, " C "), 1, wxALIGN_CENTER);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, " A "), 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	O_field = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(GraphConst::flag_field_width, GraphConst::flag_field_height));
	S_field = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(GraphConst::flag_field_width, GraphConst::flag_field_height));
	Z_field = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(GraphConst::flag_field_width, GraphConst::flag_field_height));
	P_field = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(GraphConst::flag_field_width, GraphConst::flag_field_height));
	C_field = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(GraphConst::flag_field_width, GraphConst::flag_field_height));
	A_field = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(GraphConst::flag_field_width, GraphConst::flag_field_height));
	temp_sizer->Add(O_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(S_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(Z_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(P_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(C_field, 1, wxALIGN_CENTER);
	temp_sizer->Add(A_field, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);

	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	O_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::O, "");
	S_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::S, "");
	Z_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::Z, "");
	P_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::P, "");
	C_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::C, "");
	A_check = new wxCheckBox(this, (int)GraphConst::CheckboxIDs::A, "");
	temp_sizer->Add(O_check, 1, wxALIGN_CENTER);
	temp_sizer->Add(S_check, 1, wxALIGN_CENTER);
	temp_sizer->Add(Z_check, 1, wxALIGN_CENTER);
	temp_sizer->Add(P_check, 1, wxALIGN_CENTER);
	temp_sizer->Add(C_check, 1, wxALIGN_CENTER);
	temp_sizer->Add(A_check, 1, wxALIGN_CENTER);
	reg_field_sizer->Add(temp_sizer, 0, wxALIGN_CENTER);
	// REGISTER SIZER END

	// CODE SIZER START
	code_field_sizer->Add(
		new wxStaticText(this, wxID_ANY, "Код программы"),
		0, wxALIGN_CENTER | wxALL, GraphConst::base_border);
	code_editor = new wxTextCtrl(this, GraphConst::CODE_FIELD, wxEmptyString,
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	code_field_sizer->Add(code_editor, 1, wxEXPAND);
	// CODE SIZER END

	// MEMORY SIZER START
	mem_field_sizer->Add(
		new wxStaticText(this, wxID_ANY, "Память"),
		0, wxALIGN_CENTER);
	start_address = new wxTextCtrl(this, GraphConst::FieldIDs::START_ADDRESS, "00000",
		wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	mem_field_sizer->Add(new wxStaticText(this, wxID_ANY, "Начальный адрес"),
		0, wxALIGN_CENTER | wxALL, GraphConst::base_border);
	mem_field_sizer->Add(start_address,
		0, wxALIGN_CENTER | wxALL, GraphConst::base_border);

	mem_dump = new wxGrid(this, wxID_ANY);
	mem_dump->CreateGrid(GraphConst::memory_rows, GraphConst::memory_cols);
	for (int i = 0; i < GraphConst::memory_cols; i++) {
		mem_dump->SetColLabelValue(i, "00");
	}
	for (int i = 0; i < GraphConst::memory_rows; i++) {
		mem_dump->SetRowLabelValue(i, "0000" + int_to_hex(i));
	}
	mem_dump->SetRowLabelSize(GraphConst::row_label_size);
	mem_dump->SetColLabelSize(GraphConst::col_label_size);
	mem_dump->AutoSizeColumns();
	mem_dump->HideColLabels();
	mem_field_sizer->Add(mem_dump, 0, wxEXPAND);
	// MEMORY SIZER END


	fields_sizer->Add(reg_field_sizer, 1, wxEXPAND | wxALL, GraphConst::base_border);
	fields_sizer->Add(code_field_sizer, 2, wxEXPAND | wxALL, GraphConst::base_border);
	fields_sizer->Add(mem_field_sizer, 2, wxEXPAND | wxALL, GraphConst::base_border);

	main_sizer->Add(buttons_sizer, 1, wxEXPAND | wxALL, GraphConst::base_border);
	main_sizer->Add(fields_sizer, 7, wxEXPAND | wxALL, GraphConst::base_border);

	SetSizer(main_sizer);
	SetBackgroundColour(wxColour(100, 200, 100));
	CreateStatusBar();
}

void CreateLabFrame::initEmulator() {
	cpu_pt = std::make_shared<cpu8086>();
	mem_pt = std::make_shared<Memory>();
	cpu_pt->initOpTable();
	cpu_pt->initMemory(mem_pt);
	cpu_pt->initPresenter(this);
	cpu_pt->setRegVal(RegId::CS, 0x1000);
	cpu_pt->setRegVal(RegId::DS, 0x2000);
	cpu_pt->setRegVal(RegId::SS, 0x3000);
	mem_pt->initPresenter(this);

	Render();
}

void CreateLabFrame::Render() {
	notifyRegChange();
	notifyMemChange();
}

void CreateLabFrame::notifyRegChange() {
	AH_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::AH)));
	AL_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::AL)));
	BH_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::BH)));
	BL_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::BL)));
	CH_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::CH)));
	CL_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::CL)));
	DH_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::DH)));
	DL_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::DL)));
	CS_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::CS)));
	IP_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::IP)));
	SS_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::SS)));
	SP_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::SP)));
	BP_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::BP)));
	SI_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::SI)));
	DI_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::DI)));
	DS_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::DS)));
	ES_field->SetValue(int_to_hex(cpu_pt->getRegVal(RegId::ES)));

	O_field->SetValue(std::to_string(cpu_pt->getFlag(Flag::O)));
	S_field->SetValue(std::to_string(cpu_pt->getFlag(Flag::S)));
	Z_field->SetValue(std::to_string(cpu_pt->getFlag(Flag::Z)));
	P_field->SetValue(std::to_string(cpu_pt->getFlag(Flag::P)));
	C_field->SetValue(std::to_string(cpu_pt->getFlag(Flag::C)));
	A_field->SetValue(std::to_string(cpu_pt->getFlag(Flag::A)));
}

void CreateLabFrame::notifyMemChange() {
	int address = 0;
	address = hex_to_int(start_address->GetValue().ToStdString());

	// перезапись левых ячеек-подсказок
	for (int row = 0; row < GraphConst::memory_rows; row++) {
		std::string value = int_to_hex(address + row * 0x10);
		// заполнение до 5 символов
		value.insert(0, 5 - value.length(), '0');
		mem_dump->SetRowLabelValue(row, value);
	}

	// заполнение ячеек памяти
	for (int row = 0; row < GraphConst::memory_rows; row++) {
		for (int col = 0; col < GraphConst::memory_cols; col++) {
			int value = mem_pt->readB(address + (row * GraphConst::memory_cols) + col);
			if (value < 0x10) {
				mem_dump->SetCellValue(row, col, "0" + int_to_hex(value));
			}
			else {
				mem_dump->SetCellValue(row, col, int_to_hex(value));
			}
		}
	}
}

void CreateLabFrame::OnClose(wxCloseEvent& evt) {
	Show(false);
}

void CreateLabFrame::OnStartAddressChange(wxCommandEvent& evt) {
	start_address->SetValue(evt.GetString().MakeUpper());
	notifyMemChange();
	wxLogStatus(start_address->GetValue());
}

// загрузить исходный код программы
void CreateLabFrame::OnLoadButton(wxCommandEvent& evt) {
	wxFileDialog
		openFileDialog(this, _("Загрузить исходный код"), "", "",
			"ASM files (*.asm)|*.asm", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

	// открываем файл в режиме чтения
	std::ifstream stream(openFileDialog.GetPath().ToStdString());
	std::string line;
	code_editor->Clear();
	// читаем файл и записываем исходный код
	while (std::getline(stream, line)) {
		code_editor->AppendText(line);
		code_editor->AppendText('\n');
	}
	stream.close();
}

void CreateLabFrame::OnSandboxButton(wxCommandEvent& evt) {
	wxLogStatus("sandbox");
}

void CreateLabFrame::OnReferenceButton(wxCommandEvent& evt) {
	wxLogStatus("ref");
}

void CreateLabFrame::OnCreateLabButton(wxCommandEvent& evt) {
	wxLogStatus("create lab");
}

void CreateLabFrame::OnTestLabButton(wxCommandEvent& evt) {
	wxLogStatus("test lab");
}

void CreateLabFrame::OnByteFieldChange(wxCommandEvent& evt) {
	wxLogStatus(evt.GetString());
}

void CreateLabFrame::OnWordFieldChange(wxCommandEvent& evt) {
	wxLogStatus(evt.GetString());
}

void CreateLabFrame::OnFlagFieldChange(wxCommandEvent& evt) {
	wxLogStatus(evt.GetString());
}