#include "BusForm.h"

BusForm::BusForm(wxWindow* parent)
    : BusFormBase(parent)
{
	SetSize(GetBestSize());
}

BusForm::~BusForm()
{
}

void BusForm::OnButtonCancelClick(wxCommandEvent& event)
{
}
void BusForm::OnButtonOKClick(wxCommandEvent& event)
{
}
