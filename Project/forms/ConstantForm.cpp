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

#include "ConstantForm.h"
#include "../elements/controlElement/Constant.h"

ConstantForm::ConstantForm(wxWindow* parent, Constant* constant) : ConstantFormBase(parent)
{
    SetSize(GetBestSize());

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
