#pragma once
#include <wx/wx.h>

class DescriptionDialog : public wxDialog {
private:
	wxBoxSizer* main_sizer;
	wxTextCtrl* description;
	wxTextCtrl* lab_name;

public:
	DescriptionDialog(wxWindow* parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos,
		const wxSize& size,
		long style,
		const wxString& name);

	wxString GetDescription();
	wxString GetLabName();
	void SetDescription(wxString desc);
	void SetLabName(wxString lb_nm);
};