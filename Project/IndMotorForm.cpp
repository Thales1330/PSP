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

#include "IndMotorForm.h"
#include "IndMotor.h"

IndMotorForm::IndMotorForm(wxWindow* parent, IndMotor* indMotor) : IndMotorFormBase(parent)
{
    SetSize(GetBestSize());
    m_buttonStabButton->Enable(false);
    IndMotorElectricalData data = indMotor->GetElectricalData();

    m_textCtrlName->SetValue(data.name);

    m_textCtrlActivePower->SetValue(IndMotor::StringFromDouble(data.activePower));
    switch(data.activePowerUnit) {
        case UNIT_PU: {
            m_choiceActivePower->SetSelection(0);
        } break;
        case UNIT_W: {
            m_choiceActivePower->SetSelection(1);
        } break;
        case UNIT_kW: {
            m_choiceActivePower->SetSelection(2);
        } break;
        case UNIT_MW: {
            m_choiceActivePower->SetSelection(3);
        } break;
        default:
            break;
    }

    m_textCtrlReactivePower->SetValue(IndMotor::StringFromDouble(data.reactivePower));
    switch(data.reactivePowerUnit) {
        case UNIT_PU: {
            m_choiceReactivePower->SetSelection(0);
        } break;
        case UNIT_VAr: {
            m_choiceReactivePower->SetSelection(1);
        } break;
        case UNIT_kVAr: {
            m_choiceReactivePower->SetSelection(2);
        } break;
        case UNIT_MVAr: {
            m_choiceReactivePower->SetSelection(3);
        } break;
        default:
            break;
    }

    m_parent = parent;
    m_indMotor = indMotor;
}

IndMotorForm::~IndMotorForm() {}
void IndMotorForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}
void IndMotorForm::OnStabilityButtonClick(wxCommandEvent& event)
{
    // TODO: Induction motor stability form
}

bool IndMotorForm::ValidateData()
{
    IndMotorElectricalData data;

    data.name = m_textCtrlName->GetValue();

    if(!m_indMotor->DoubleFromString(m_parent, m_textCtrlActivePower->GetValue(), data.activePower,
                                     _("Value entered incorrectly in the field \"Active power\".")))
        return false;
    switch(m_choiceActivePower->GetSelection()) {
        case 0: {
            data.activePowerUnit = UNIT_PU;
        } break;
        case 1: {
            data.activePowerUnit = UNIT_W;
        } break;
        case 2: {
            data.activePowerUnit = UNIT_kW;
        } break;
        case 3: {
            data.activePowerUnit = UNIT_MW;
        } break;
    }

    if(!m_indMotor->DoubleFromString(m_parent, m_textCtrlReactivePower->GetValue(), data.reactivePower,
                                     _("Value entered incorrectly in the field \"Reactive power\".")))
        return false;
    switch(m_choiceReactivePower->GetSelection()) {
        case 0: {
            data.reactivePowerUnit = UNIT_PU;
        } break;
        case 1: {
            data.reactivePowerUnit = UNIT_VAr;
        } break;
        case 2: {
            data.reactivePowerUnit = UNIT_kVAr;
        } break;
        case 3: {
            data.reactivePowerUnit = UNIT_MVAr;
        } break;
    }

    m_indMotor->SetElectricalData(data);
    return true;
}
