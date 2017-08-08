#include "GeneralPropertiesForm.h"

GeneralPropertiesForm::GeneralPropertiesForm(wxWindow* parent)
    : GeneralPropertiesFormBase(parent)
{
}

GeneralPropertiesForm::~GeneralPropertiesForm()
{
}

void GeneralPropertiesForm::OnButtonOKClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

bool GeneralPropertiesForm::ValidateData()
{
    return true;
}
