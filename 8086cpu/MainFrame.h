#pragma once
#include <wx/wx.h>
#include <wx/grid.h>
#include "GraphicConstants.h"
#include "AbstractPresenter.h"
#include "cpu8086.h"
#include "Memory.h"
#include <string>

class MainFrame : public wxFrame, public AbstractPresenter {
private:
	wxPanel* panel;
	wxButton* cycle_button;
	// sizers
	wxBoxSizer* main_sizer;
	wxBoxSizer* buttons_sizer;
	wxBoxSizer* fields_sizer;
	wxBoxSizer* reg_field_sizer;
	wxBoxSizer* code_field_sizer;
	wxBoxSizer* mem_field_sizer;
	// кнопки
	wxButton* clock_button;
	wxButton* run_button;
	wxButton* stop_button;
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

public:
	MainFrame();
	void initEmulator();

	void notifyRegChange() override;
	void notifyMemChange() override;
	void notifyStkChange() override {};

	void Render();

private:

	// button handlers
	void OnStartAddressChange(wxCommandEvent& evt);
	void OnClockButton(wxCommandEvent& evt);
	void OnLoadButton(wxCommandEvent& evt);
	void OnByteFieldChange(wxCommandEvent& evt);
	void OnWordFieldChange(wxCommandEvent& evt);
	void OnFlagFieldChange(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};