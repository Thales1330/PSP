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

#include "GainForm.h"
#include "../elements/controlElement/Gain.h"

GainForm::GainForm(wxWindow* parent, Gain* gain) : GainFormBase(parent)
{
    SetSize(GetBestSize());

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
