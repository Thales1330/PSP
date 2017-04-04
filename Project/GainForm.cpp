#include "GainForm.h"
#include "Gain.h"

GainForm::GainForm(wxWindow* parent, Gain* gain) : GainFormBase(parent)
{
    m_parent = parent;
    m_gain = gain;

    m_textCtrlValue->SetValue(m_gain->StringFromDouble(m_gain->GetValue()));
}

GainForm::~GainForm() {}
void GainForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

bool GainForm::ValidateData()
{
    double value;
    if(!m_gain->DoubleFromString(this, m_textCtrlValue->GetValue(), value,
                                 _("Value entered incorrectly in the field \"Gain value\".")))
        return false;

    m_gain->SetValue(value);
    return true;
}
