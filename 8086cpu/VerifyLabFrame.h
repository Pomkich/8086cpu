#pragma once
#include <wx/wx.h>
#include <wx/grid.h>
#include "GraphicConstants.h"
#include "AbstractPresenter.h"
#include "cpu8086.h"
#include "Memory.h"
#include "TestingModule.h"
#include "DescriptionFrame.h"
#include <string>
#include <fstream>
#include <Windows.h>
#include <iostream>

class VerifyLabFrame : public wxFrame, public AbstractPresenter {
private:
	// frame's
	DescriptionFrame* desc_frame;
	// sizers
	wxBoxSizer* main_sizer;
	wxBoxSizer* buttons_sizer;
	wxBoxSizer* fields_sizer;
	wxBoxSizer* reg_field_sizer;
	wxBoxSizer* code_field_sizer;
	wxBoxSizer* mem_field_sizer;
	// кнопки
	wxButton* verify_button;
	wxButton* show_desc_button;
	wxButton* load_lab_button;
	wxButton* load_code_button;
	// регистры
	wxTextCtrl* AH_field, * AL_field;
	wxTextCtrl* BH_field, * BL_field;
	wxTextCtrl* CH_field, * CL_field;
	wxTextCtrl* DH_field, * DL_field;
	wxTextCtrl* CS_field;
	wxTextCtrl* IP_field;
	wxTextCtrl* SS_field;
	wxTextCtrl* SP_field;
	wxTextCtrl* BP_field;
	wxTextCtrl* SI_field;
	wxTextCtrl* DI_field;
	wxTextCtrl* DS_field;
	wxTextCtrl* ES_field;
	wxTextCtrl* O_field, * S_field, * Z_field, * P_field, * C_field, * A_field;
	// код
	wxTextCtrl* code_editor;
	// память
	wxGrid* mem_dump;
	wxTextCtrl* start_address;
	// проверка загрузки лабораторной
	bool lab_loaded;

	// переменная нужна для проверки лабораторной
	std::shared_ptr<bool> running;
	wxString description;
	wxString lab_name;
public:
	VerifyLabFrame();
	void initEmulator();
	void initDescFrame(DescriptionFrame* f);

	void notifyRegChange() override;
	void notifyMemChange() override;
	void notifyStkChange() override {};
	void notifyHalt() override;

	void Render();

private:
	// список регистров, которые будут проверяться в лабораторной
	void OnClose(wxCloseEvent& evt);
	// button handlers
	void OnStartAddressChange(wxCommandEvent& evt);
	void OnVerifyButton(wxCommandEvent& evt);
	void OnShowDescButton(wxCommandEvent& evt);
	void OnLoadCodeButton(wxCommandEvent& evt);
	void OnLoadLabButton(wxCommandEvent& evt);
	void OnByteFieldChange(wxCommandEvent& evt);
	void OnWordFieldChange(wxCommandEvent& evt);
	void OnFlagFieldChange(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};