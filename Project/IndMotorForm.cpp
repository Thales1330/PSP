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

#include "IndMotor.h"
#include "IndMotorForm.h"
#include "SwitchingForm.h"

IndMotorForm::IndMotorForm(wxWindow* parent, IndMotor* indMotor) : IndMotorFormBase(parent)
{
    SetSize(GetBestSize());
    m_staticTextw->SetLabel(static_cast<wxString>(L'\u03C9') + wxT(" +"));
    m_staticTextw2->SetLabel(static_cast<wxString>(L'\u03C9') + static_cast<wxString>(L'\u00B2'));
    IndMotorElectricalData data = indMotor->GetElectricalData();

    m_textCtrlName->SetValue(data.name);

    m_textCtrlnominalPower->SetValue(IndMotor::StringFromDouble(data.ratedPower));
    switch(data.activePowerUnit) {
        case UNIT_VA: {
            m_choiceNominalPower->SetSelection(0);
        } break;
        case UNIT_kVA: {
            m_choiceNominalPower->SetSelection(1);
        } break;
        case UNIT_MVA: {
            m_choiceNominalPower->SetSelection(2);
        } break;
        default:
            break;
    }

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

    m_checkBoxUseMachinePower->SetValue(data.useMachinePowerAsBase);

    // Stability
    m_textCtrlInertia->SetValue(IndMotor::StringFromDouble(data.inertia));
    m_textCtrlStatorResistence->SetValue(IndMotor::StringFromDouble(data.r1));
    m_textCtrlStatorReactance->SetValue(IndMotor::StringFromDouble(data.x1));
    m_textCtrlRotorResistence->SetValue(IndMotor::StringFromDouble(data.r2));
    m_textCtrlRotorReactance->SetValue(IndMotor::StringFromDouble(data.x2));
    m_textCtrlMagnetizingReactance->SetValue(IndMotor::StringFromDouble(data.xm));

    m_textCtrlA->SetValue(IndMotor::StringFromDouble(data.aw));
    m_textCtrlB->SetValue(IndMotor::StringFromDouble(data.bw));
    m_textCtrlC->SetValue(IndMotor::StringFromDouble(data.cw));

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
    if(ValidateData()) {
        SwitchingForm swForm(m_parent, m_indMotor);
        swForm.SetTitle(_("Induction motor: Switching"));
        swForm.ShowModal();
        EndModal(wxID_OK);
    }
}

bool IndMotorForm::ValidateData()
{
    IndMotorElectricalData data;

    data.name = m_textCtrlName->GetValue();

    if(!m_indMotor->DoubleFromString(m_parent, m_textCtrlnominalPower->GetValue(), data.ratedPower,
                                     _("Value entered incorrectly in the field \"Rated power\".")))
        return false;
    switch(m_choiceNominalPower->GetSelection()) {
        case 0: {
            data.activePowerUnit = UNIT_VA;
        } break;
        case 1: {
            data.activePowerUnit = UNIT_kVA;
        } break;
        case 2: {
            data.activePowerUnit = UNIT_MVA;
        } break;
    }

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

    data.useMachinePowerAsBase = m_checkBoxUseMachinePower->GetValue();

    // Stability
    if(!m_indMotor->DoubleFromString(m_parent, m_textCtrlInertia->GetValue(), data.inertia,
                                     _("Value entered incorrectly in the field \"Inertia\".")))
        return false;
    if(!m_indMotor->DoubleFromString(m_parent, m_textCtrlStatorResistence->GetValue(), data.r1,
                                     _("Value entered incorrectly in the field \"Stator resistence\".")))
        return false;
    if(!m_indMotor->DoubleFromString(m_parent, m_textCtrlStatorReactance->GetValue(), data.x1,
                                     _("Value entered incorrectly in the field \"Stator reactance\".")))
        return false;
    if(!m_indMotor->DoubleFromString(m_parent, m_textCtrlRotorResistence->GetValue(), data.r2,
                                     _("Value entered incorrectly in the field \"Rotor resistence\".")))
        return false;
    if(!m_indMotor->DoubleFromString(m_parent, m_textCtrlRotorReactance->GetValue(), data.x2,
                                     _("Value entered incorrectly in the field \"Rotor reactance\".")))
        return false;
    if(!m_indMotor->DoubleFromString(m_parent, m_textCtrlMagnetizingReactance->GetValue(), data.xm,
                                     _("Value entered incorrectly in the field \"Magnetizing reactance\".")))
        return false;

    if(!m_indMotor->DoubleFromString(m_parent, m_textCtrlA->GetValue(), data.aw,
                                     _("Value entered incorrectly in the field \"Constant torque\".")))
        return false;
    if(!m_indMotor->DoubleFromString(m_parent, m_textCtrlB->GetValue(), data.bw,
                                     _("Value entered incorrectly in the field \"Linear torque\".")))
        return false;
    if(!m_indMotor->DoubleFromString(m_parent, m_textCtrlC->GetValue(), data.cw,
                                     _("Value entered incorrectly in the field \"Quadratic torque\".")))
        return false;

    /*double sum = data.aw + data.bw + data.cw;
    if(sum > 1.01 || sum < 0.99) {
        wxString errorMsg = _("The sum of the portions of the torque must be unitary");
        wxMessageDialog msgDialog(m_parent, errorMsg, _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
        return false;
    }*/

    m_indMotor->SetElectricalData(data);
    return true;
}
