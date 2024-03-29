#pragma once
#include <wx/wx.h>
#include <wx/grid.h>
#include "GraphicConstants.h"
#include "AbstractPresenter.h"
#include "cpu8086.h"
#include "Memory.h"
#include "CreateLabFrame.h"
#include "VerifyLabFrame.h"
#include "DescriptionFrame.h"
#include <string>
#include <fstream>
#include <Windows.h>
#include <iostream>

class MainFrame : public wxFrame, public AbstractPresenter {
private:
	CreateLabFrame* lab_frame;
	VerifyLabFrame* verify_frame;
	DescriptionFrame* desc_frame;
	// sizers
	wxBoxSizer* main_sizer;
	wxBoxSizer* buttons_sizer;
	wxBoxSizer* fields_sizer;
	wxBoxSizer* reg_field_sizer;
	wxBoxSizer* code_field_sizer;
	wxBoxSizer* mem_field_sizer;
	// ����
	wxMenuBar* menu;
	wxMenu* labs;
	wxMenu* emulator;
	wxMenuItem* sandbox;
	wxMenuItem* reference;
	wxMenuItem* createLab;
	wxMenuItem* testLab;
	// ������
	wxButton* clock_button;
	wxButton* run_button;
	wxButton* compile_button;
	wxButton* stop_button;
	wxButton* load_button;
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
	wxTextCtrl* O_field, * S_field, * Z_field, * P_field, * C_field, * A_field;
	// ���
	wxTextCtrl* code_editor;
	// ������
	wxGrid* mem_dump;
	wxTextCtrl* start_address;

	std::shared_ptr<bool> running;
public:
	MainFrame();
	void initEmulator();
	void initLabFrame(CreateLabFrame* second_frame);
	void initVerifyFrame(VerifyLabFrame* third_frame);
	void initDescFrame(DescriptionFrame* d_f);

	void notifyRegChange() override;
	void notifyMemChange() override;
	void notifyStkChange() override {};
	void notifyHalt() override;

	void Render();

private:
	void OnClose(wxCloseEvent& evt);
	// button handlers
	void OnStartAddressChange(wxCommandEvent& evt);
	void OnClockButton(wxCommandEvent& evt);
	void OnCompileButton(wxCommandEvent& evt);
	void OnLoadButton(wxCommandEvent& evt);
	void OnRunButton(wxCommandEvent& evt);
	void OnSandboxButton(wxCommandEvent& evt);
	void OnReferenceButton(wxCommandEvent& evt);
	void OnCreateLabButton(wxCommandEvent& evt);
	void OnTestLabButton(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};