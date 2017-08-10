#include "GeneralPropertiesForm.h"
#include "PropertiesData.h"

GeneralPropertiesForm::GeneralPropertiesForm(wxWindow* parent, PropertiesData* properties)
    : GeneralPropertiesFormBase(parent)
{
    m_properties = properties;
}

GeneralPropertiesForm::~GeneralPropertiesForm() {}
void GeneralPropertiesForm::OnButtonOKClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

bool GeneralPropertiesForm::ValidateData() { return true; }
