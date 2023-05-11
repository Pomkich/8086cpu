#pragma once

#include <wx/wx.h>
#include "GraphicConstants.h"

class DescriptionFrame : public wxFrame {
private:
	wxBoxSizer* main_sizer;
	wxTextCtrl* description;
	wxTextCtrl* lab_name;

public:
	DescriptionFrame();

	void SetDescription(wxString desc);
	void SetLabName(wxString lb_nm);
	void OnClose(wxCloseEvent& evt);

	wxDECLARE_EVENT_TABLE();
};