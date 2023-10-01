#pragma once
#include <wx/wx.h>
#include <wx/grid.h>
#include "GraphicConstants.h"
#include "AbstractPresenter.h"
#include "cpu8086.h"
#include "Memory.h"
#include "TestingModule.h"
#include "DescriptionDialog.h"
#include <string>
#include <fstream>
#include <Windows.h>
#include <iostream>

class CreateLabFrame : public wxFrame, public AbstractPresenter {
private:
	// sizers
	wxBoxSizer* main_sizer;
	wxBoxSizer* buttons_sizer;
	wxBoxSizer* fields_sizer;
	wxBoxSizer* reg_field_sizer;
	wxBoxSizer* code_field_sizer;
	wxBoxSizer* mem_field_sizer;
	// кнопки
	wxButton* generate_button;
	wxButton* add_description;
	wxButton* load_button;
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
	// границы проверяемой памяти
	wxTextCtrl* block_start;
	wxTextCtrl* block_end;

	// checkbox'ы 
	wxCheckBox* AX_check;
	wxCheckBox* BX_check;
	wxCheckBox* CX_check;
	wxCheckBox* DX_check;

	// переменная нужна для программы при генерации теста
	std::shared_ptr<bool> running;
	wxString description;
	wxString lab_name;
public:
	CreateLabFrame();
	void initEmulator();

	void notifyRegChange() override;
	void notifyMemChange() override;
	void notifyStkChange() override {};
	void notifyHalt() override;

	void Render();

private:
	// список регистров, которые будут проверяться в лабораторной
	std::list<RegId> GetRegistersForCheck();
	void OnClose(wxCloseEvent& evt);
	// button handlers
	void OnStartAddressChange(wxCommandEvent& evt);
	void OnGenerateButton(wxCommandEvent& evt);
	void OnAddDescButton(wxCommandEvent& evt);
	void OnLoadButton(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};