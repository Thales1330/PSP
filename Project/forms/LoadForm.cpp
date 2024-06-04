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

#include "LoadForm.h"
#include "SwitchingForm.h"
#include "../elements/powerElement/Load.h"

LoadForm::LoadForm(wxWindow* parent, Load* load) : LoadFormBase(parent)
{
    SetSize(GetBestSize());
    LoadElectricalData data = load->GetElectricalData();

    m_textCtrlName->SetValue(data.name);

    m_textCtrlActivePower->SetValue(Load::StringFromDouble(data.activePower));
    switch(data.activePowerUnit) {
        case ElectricalUnit::UNIT_PU: {
            m_choiceActivePower->SetSelection(0);
        } break;
        case ElectricalUnit::UNIT_W: {
            m_choiceActivePower->SetSelection(1);
        } break;
        case ElectricalUnit::UNIT_kW: {
            m_choiceActivePower->SetSelection(2);
        } break;
        case ElectricalUnit::UNIT_MW: {
            m_choiceActivePower->SetSelection(3);
        } break;
        default:
            break;
    }

    m_textCtrlReactivePower->SetValue(Load::StringFromDouble(data.reactivePower));
    switch(data.reactivePowerUnit) {
        case ElectricalUnit::UNIT_PU: {
            m_choiceReactivePower->SetSelection(0);
        } break;
        case ElectricalUnit::UNIT_var: {
            m_choiceReactivePower->SetSelection(1);
        } break;
        case ElectricalUnit::UNIT_kvar: {
            m_choiceReactivePower->SetSelection(2);
        } break;
        case ElectricalUnit::UNIT_Mvar: {
            m_choiceReactivePower->SetSelection(3);
        } break;
        default:
            break;
    }

    switch(data.loadType) {
        case CONST_POWER: {
            m_choiceType->SetSelection(0);
        } break;
        case CONST_IMPEDANCE: {
            m_choiceType->SetSelection(1);
        } break;
    }

    m_checkBoxPlotData->SetValue(data.plotLoad);

    m_checkBoxUseCompLoad->SetValue(data.useCompLoad);

    m_textCtrlActivePowerImp->SetValue(Element::StringFromDouble(data.constImpedanceActive));
    m_textCtrlActivePowerCur->SetValue(Element::StringFromDouble(data.constCurrentActive));
    m_textCtrlActivePowerPow->SetValue(Element::StringFromDouble(data.constPowerActive));
    m_textCtrlReactivePowerImp->SetValue(Element::StringFromDouble(data.constImpedanceReactive));
    m_textCtrlReactivePowerCur->SetValue(Element::StringFromDouble(data.constCurrentReactive));
    m_textCtrlReactivePowerPow->SetValue(Element::StringFromDouble(data.constPowerReactive));

    m_parent = parent;
    m_load = load;
    
    UpdateZIPLoadFieldStatus();
}

LoadForm::~LoadForm() {}
void LoadForm::OnOnButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

void LoadForm::OnStabilityButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) {
        SwitchingForm swForm(m_parent, m_load);
        swForm.SetTitle(_("Load: Switching"));
        swForm.ShowModal();
        EndModal(wxID_OK);
    }
}

bool LoadForm::ValidateData()
{
    LoadElectricalData data;

    data.name = m_textCtrlName->GetValue();

    if(!m_load->DoubleFromString(m_parent, m_textCtrlActivePower->GetValue(), data.activePower,
                                 _("Value entered incorrectly in the field \"Active power\".")))
        return false;
    switch(m_choiceActivePower->GetSelection()) {
        case 0: {
            data.activePowerUnit = ElectricalUnit::UNIT_PU;
        } break;
        case 1: {
            data.activePowerUnit = ElectricalUnit::UNIT_W;
        } break;
        case 2: {
            data.activePowerUnit = ElectricalUnit::UNIT_kW;
        } break;
        case 3: {
            data.activePowerUnit = ElectricalUnit::UNIT_MW;
        } break;
    }

    if(!m_load->DoubleFromString(m_parent, m_textCtrlReactivePower->GetValue(), data.reactivePower,
                                 _("Value entered incorrectly in the field \"Reactive power\".")))
        return false;
    switch(m_choiceReactivePower->GetSelection()) {
        case 0: {
            data.reactivePowerUnit = ElectricalUnit::UNIT_PU;
        } break;
        case 1: {
            data.reactivePowerUnit = ElectricalUnit::UNIT_var;
        } break;
        case 2: {
            data.reactivePowerUnit = ElectricalUnit::UNIT_kvar;
        } break;
        case 3: {
            data.reactivePowerUnit = ElectricalUnit::UNIT_Mvar;
        } break;
    }

    switch(m_choiceType->GetSelection()) {
        case 0: {
            data.loadType = CONST_POWER;
        } break;
        case 1: {
            data.loadType = CONST_IMPEDANCE;
        } break;
    }

    data.plotLoad = m_checkBoxPlotData->GetValue();

    data.useCompLoad = m_checkBoxUseCompLoad->GetValue();

    if(!Element::DoubleFromString(
           this, m_textCtrlActivePowerImp->GetValue(), data.constImpedanceActive,
           _("Value entered incorrectly in the field \"Constant impedance portion of active power\".")))
        return false;
    if(!Element::DoubleFromString(
           this, m_textCtrlActivePowerCur->GetValue(), data.constCurrentActive,
           _("Value entered incorrectly in the field \"Constant current portion of active power\".")))
        return false;
    if(!Element::DoubleFromString(
           this, m_textCtrlActivePowerPow->GetValue(), data.constPowerActive,
           _("Value entered incorrectly in the field \"Constant power portion of active power\".")))
        return false;
    if(!Element::DoubleFromString(
           this, m_textCtrlReactivePowerImp->GetValue(), data.constImpedanceReactive,
           _("Value entered incorrectly in the field \"Constant impedance portion of reactive power\".")))
        return false;
    if(!Element::DoubleFromString(
           this, m_textCtrlReactivePowerCur->GetValue(), data.constCurrentReactive,
           _("Value entered incorrectly in the field \"Constant current portion of reactive power\".")))
        return false;
    if(!Element::DoubleFromString(
           this, m_textCtrlReactivePowerPow->GetValue(), data.constPowerReactive,
           _("Value entered incorrectly in the field \"Constant power portion of reactive power\".")))
        return false;

    double sum = data.constImpedanceActive + data.constCurrentActive + data.constPowerActive;
    if(sum > 100.01 || sum < 99.99) {
        wxMessageDialog msgDialog(this, _("The sum of active power load composition must be 100%."), _("Error"),
                                  wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
        return false;
    }
    sum = data.constImpedanceReactive + data.constCurrentReactive + data.constPowerReactive;
    if(sum > 100.01 || sum < 99.99) {
        wxMessageDialog msgDialog(this, _("The sum of reactive power load composition must be 100%."), _("Error"),
                                  wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
        return false;
    }

    m_load->SetElectricalData(data);
    return true;
}

void LoadForm::UpdateZIPLoadFieldStatus()
{
    m_textCtrlActivePowerImp->Enable(m_checkBoxUseCompLoad->GetValue());
    m_textCtrlActivePowerCur->Enable(m_checkBoxUseCompLoad->GetValue());
    m_textCtrlActivePowerPow->Enable(m_checkBoxUseCompLoad->GetValue());
    m_textCtrlReactivePowerImp->Enable(m_checkBoxUseCompLoad->GetValue());
    m_textCtrlReactivePowerCur->Enable(m_checkBoxUseCompLoad->GetValue());
    m_textCtrlReactivePowerPow->Enable(m_checkBoxUseCompLoad->GetValue());
}
