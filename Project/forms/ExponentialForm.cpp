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

#include "ExponentialForm.h"
#include "../elements/controlElement/Exponential.h"

ExponentialForm::ExponentialForm(wxWindow* parent, Exponential* exponential) : ExponentialFormBase(parent)
{
    SetSize(GetBestSize());
    
    wxString expSymbol = wxString::FromUTF8("\xF0\x9D\x91\x92");
    wxString superscriptCapitalB = wxString::FromUTF8("\xE1\xB4\xAE");
    wxString superscriptSmallX = wxString::FromUTF8("\xCB\xA3");
    m_staticTextExp->SetLabel("y = A" + expSymbol + superscriptCapitalB + superscriptSmallX);
    
    wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    font.SetPointSize(14);
    m_staticTextExp->SetFont(font);
    
    m_parent = parent;
    m_exponential = exponential;

    double a, b;
    m_exponential->GetValues(a, b);
    m_textCtrlAValue->SetValue(m_exponential->StringFromDouble(a));
    m_textCtrlBValue->SetValue(m_exponential->StringFromDouble(b));
    
    SetInitialSize();
    Layout();
}

ExponentialForm::~ExponentialForm() {}
void ExponentialForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

bool ExponentialForm::ValidateData()
{
    double a, b;
    if(!m_exponential->DoubleFromString(this, m_textCtrlAValue->GetValue(), a,
                                        _("Value entered incorrectly in the field \"A value\".")))
        return false;
    if(!m_exponential->DoubleFromString(this, m_textCtrlBValue->GetValue(), b,
                                        _("Value entered incorrectly in the field \"B value\".")))
        return false;
    m_exponential->SetValues(a, b);
    return true;
}
