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

#include "RateLimiterForm.h"
#include "../elements/controlElement/RateLimiter.h"

RateLimiterForm::RateLimiterForm(wxWindow* parent, RateLimiter* rateLimiter) : RateLimiterFormBase(parent)
{
    SetSize(GetBestSize());

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
