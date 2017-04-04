#include "ConstantForm.h"
#include "Constant.h"

ConstantForm::ConstantForm(wxWindow* parent, Constant* constant) : ConstantFormBase(parent)
{
    m_parent = parent;
    m_constant = constant;

    m_textCtrlValue->SetValue(m_constant->StringFromDouble(m_constant->GetValue()));
}

ConstantForm::~ConstantForm() {}
void ConstantForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

bool ConstantForm::ValidateData()
{
    double value;
    if(!m_constant->DoubleFromString(this, m_textCtrlValue->GetValue(), value,
                                     _("Value entered incorrectly in the field \"Constant value\".")))
        return false;

    m_constant->SetValue(value);
    return true;
}
