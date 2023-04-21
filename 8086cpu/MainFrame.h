#pragma once
#include <wx/wx.h>
#include "GraphicConstants.h"

class MainFrame : public wxFrame {
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
	// ��������
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
	// ���
	wxTextCtrl* code_editor;
	// ������
	wxTextCtrl* mem_dump;

public:
	MainFrame();
};