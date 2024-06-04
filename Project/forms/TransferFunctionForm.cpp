/*
 *  Copyright (C) 2017  Thales Lima Oliveira <thales@ufu.br>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "TransferFunctionForm.h"
#include "../elements/controlElement/TransferFunction.h"

TransferFunctionForm::TransferFunctionForm(wxWindow* parent, TransferFunction* transferFunction)
    : TransferFunctionFormBase(parent)
{
    SetSize(GetBestSize());

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
