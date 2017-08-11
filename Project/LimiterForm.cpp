#include "LimiterForm.h"
#include "Limiter.h"

LimiterForm::LimiterForm(wxWindow* parent, Limiter* limiter) : LimiterFormBase(parent)
{
    SetSize(GetBestSize());
    
    m_limiter = limiter;
    m_parent = parent;

    m_textCtrlUpLimit->SetValue(m_limiter->StringFromDouble(m_limiter->GetUpLimit()));
    m_textCtrlLowLimit->SetValue(m_limiter->StringFromDouble(m_limiter->GetLowLimit()));
}

LimiterForm::~LimiterForm() {}
void LimiterForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

bool LimiterForm::ValidateData()
{
    double upLimit;
    double lowLimit;

    if(!m_limiter->DoubleFromString(this, m_textCtrlUpLimit->GetValue(), upLimit,
                                    _("Value entered incorrectly in the field \"Upper limit\".")))
        return false;
    if(!m_limiter->DoubleFromString(this, m_textCtrlLowLimit->GetValue(), lowLimit,
                                    _("Value entered incorrectly in the field \"Lower limit\".")))
        return false;

    m_limiter->SetUpLimit(upLimit);
    m_limiter->SetLowLimit(lowLimit);
    return true;
}