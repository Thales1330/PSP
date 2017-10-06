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