#include "DescriptionDialog.h"

DescriptionDialog::DescriptionDialog(
	wxWindow* parent, wxWindowID id,
	const wxString& title,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name) : wxDialog(parent, id, title, pos, size, style, name) {

	description = new wxTextCtrl(this, wxID_ANY, "Empty description", 
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	description->SetMinSize(wxSize(GraphConst::lab_width, GraphConst::description_height));
	lab_name = new wxTextCtrl(this, wxID_ANY, "Empty name",
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	lab_name->SetMinSize(wxSize(GraphConst::lab_width, GraphConst::lab_height));
	main_sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "Название"));
	temp_sizer->Add(lab_name, 1, wxEXPAND);
	main_sizer->Add(temp_sizer);

	temp_sizer = new wxBoxSizer(wxHORIZONTAL);
	temp_sizer->Add(new wxStaticText(this, wxID_ANY, "Описание"));
	temp_sizer->Add(description, 1, wxEXPAND);
	main_sizer->Add(temp_sizer);

	wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
	main_sizer->Add(buttonSizer);

	SetSizer(main_sizer);
	Fit();
}

wxString DescriptionDialog::GetDescription() {
	return description->GetValue();
}

wxString DescriptionDialog::GetLabName() {
	return lab_name->GetValue();
}

void DescriptionDialog::SetDescription(wxString desc) {
	description->SetValue(desc);
}

void DescriptionDialog::SetLabName(wxString lb_nm) {
	lab_name->SetValue(lb_nm);
}