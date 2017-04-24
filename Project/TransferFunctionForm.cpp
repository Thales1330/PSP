#include "TransferFunctionForm.h"
#include "TransferFunction.h"

TransferFunctionForm::TransferFunctionForm(wxWindow* parent, TransferFunction* transferFunction)
    : TransferFunctionFormBase(parent)
{
    m_parent = parent;
    m_tf = transferFunction;
    LoadTFData();
}

TransferFunctionForm::~TransferFunctionForm() {}
void TransferFunctionForm::OnCancelClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
void TransferFunctionForm::OnOKClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

void TransferFunctionForm::LoadTFData()
{
    auto num = m_tf->GetNumerator();
    auto den = m_tf->GetDenominator();

    wxString numStr = "";
    for(auto it = num.begin(), itEnd = num.end(); it != itEnd; ++it) {
        double value = *it;
        if(it == num.begin())
            numStr = m_tf->StringFromDouble(value, 0);
        else
            numStr += " " + m_tf->StringFromDouble(value, 0);
    }
    m_textCtrlNumerator->SetValue(numStr);

    wxString denStr = "";
    for(auto it = den.begin(), itEnd = den.end(); it != itEnd; ++it) {
        double value = *it;
        if(it == den.begin())
            denStr = m_tf->StringFromDouble(value, 0);
        else
            denStr += " " + m_tf->StringFromDouble(value, 0);
    }
    m_textCtrlDenominator->SetValue(denStr);
}

bool TransferFunctionForm::ValidateData()
{
    wxString num = m_textCtrlNumerator->GetValue();
    std::vector<double> numerator;
    while(num != "") {
        wxString rest;
        wxString strValue = num.BeforeFirst(' ', &rest);
        num = rest;
        double value = 0;
        if(!m_tf->DoubleFromString(this, strValue, value,
                                   _("Value entered incorrectly in the field \"Numerator parameters\".")))
            return false;
        numerator.push_back(value);
    }

    wxString den = m_textCtrlDenominator->GetValue();
    std::vector<double> denominator;
    while(den != "") {
        wxString rest;
        wxString strValue = den.BeforeFirst(' ', &rest);
        den = rest;
        double value = 0;
        if(!m_tf->DoubleFromString(this, strValue, value,
                                   _("Value entered incorrectly in the field \"Denominator parameters\".")))
            return false;
        denominator.push_back(value);
    }
    m_tf->SetNumerator(numerator);
    m_tf->SetDenominator(denominator);
    m_tf->UpdateTFText();
    return true;
}
