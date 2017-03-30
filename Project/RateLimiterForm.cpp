#include "RateLimiterForm.h"
#include "RateLimiter.h"

RateLimiterForm::RateLimiterForm(wxWindow* parent, RateLimiter* rateLimiter) : RateLimiterFormBase(parent)
{
    m_rateLimiter = rateLimiter;
    m_parent = parent;

    m_textCtrlUpLimit->SetValue(m_rateLimiter->StringFromDouble(m_rateLimiter->GetUpLimit()));
    m_textCtrlLowLimit->SetValue(m_rateLimiter->StringFromDouble(m_rateLimiter->GetLowLimit()));
}

RateLimiterForm::~RateLimiterForm() {}
bool RateLimiterForm::ValidateData()
{
    double upLimit;
    double lowLimit;

    if(!m_rateLimiter->DoubleFromString(this, m_textCtrlUpLimit->GetValue(), upLimit,
                                        _("Value entered incorrectly in the field \"Upper limit\".")))
        return false;
    if(!m_rateLimiter->DoubleFromString(this, m_textCtrlLowLimit->GetValue(), lowLimit,
                                        _("Value entered incorrectly in the field \"Lower limit\".")))
        return false;

    m_rateLimiter->SetUpLimit(upLimit);
    m_rateLimiter->SetLowLimit(lowLimit);
    
    return true;
}

void RateLimiterForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}
