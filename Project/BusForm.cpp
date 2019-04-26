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

#include "BusForm.h"
#include "Bus.h"

BusForm::BusForm(wxWindow* parent, Bus* bus) : BusFormBase(parent)
{
    m_choiceFaultType->SetString(0, _("Three-phase"));
    m_choiceFaultType->SetString(1, _("Line-to-line"));
    m_choiceFaultType->SetString(2, _("Double line-to-ground"));
    m_choiceFaultType->SetString(3, _("Line-to-ground"));

    SetSize(GetBestSize());

    m_parent = parent;
    m_bus = bus;

    m_textCtrlName->SetValue(bus->GetElectricalData().name);
    m_textCtrlNomVoltage->SetValue(bus->StringFromDouble(bus->GetElectricalData().nominalVoltage));

    if(bus->GetElectricalData().nominalVoltageUnit == UNIT_V)
        m_choiceNomVoltage->SetSelection(0);
    else
        m_choiceNomVoltage->SetSelection(1);

    m_checkBoxCtrlVoltage->SetValue(bus->GetElectricalData().isVoltageControlled);
    m_textCtrlCtrlVoltage->SetValue(bus->StringFromDouble(bus->GetElectricalData().controlledVoltage));
    m_choiceCtrlVoltage->SetSelection(bus->GetElectricalData().controlledVoltageUnitChoice);
    m_checkBoxSlackBus->SetValue(bus->GetElectricalData().slackBus);

    m_checkBoxFault->SetValue(bus->GetElectricalData().hasFault);
    switch(bus->GetElectricalData().faultType) {
        case FAULT_THREEPHASE: {
            m_choiceFaultType->SetSelection(0);
        } break;
        case FAULT_2LINE: {
            m_choiceFaultType->SetSelection(1);
        } break;
        case FAULT_2LINE_GROUND: {
            m_choiceFaultType->SetSelection(2);
        } break;
        case FAULT_LINE_GROUND: {
            m_choiceFaultType->SetSelection(3);
        } break;
        default:
            break;
    }
    switch(bus->GetElectricalData().faultLocation) {
        case FAULT_LINE_A: {
            m_choiceFaultPlace->SetSelection(0);
        } break;
        case FAULT_LINE_B: {
            m_choiceFaultPlace->SetSelection(1);
        } break;
        case FAULT_LINE_C: {
            m_choiceFaultPlace->SetSelection(2);
        } break;
        default:
            break;
    }
    m_textCtrlFaultResistance->SetValue(bus->StringFromDouble(bus->GetElectricalData().faultResistance));
    m_textCtrlFaultReactance->SetValue(bus->StringFromDouble(bus->GetElectricalData().faultReactance));

    m_checkBoxPlotData->SetValue(bus->GetElectricalData().plotBus);
    m_checkBoxStabFault->SetValue(bus->GetElectricalData().stabHasFault);
    m_textCtrlStabFaultTime->SetValue(bus->StringFromDouble(bus->GetElectricalData().stabFaultTime));
    m_textCtrlStabFaultLength->SetValue(bus->StringFromDouble(bus->GetElectricalData().stabFaultLength));
    m_textCtrlStabFaultResistance->SetValue(bus->StringFromDouble(bus->GetElectricalData().stabFaultResistance));
    m_textCtrlStabFaultReactance->SetValue(bus->StringFromDouble(bus->GetElectricalData().stabFaultReactance));

    EnableCtrlVoltageFields(bus->GetElectricalData().isVoltageControlled);
    EnableFaultFields(bus->GetElectricalData().hasFault);
    EnableStabFaultFields(bus->GetElectricalData().stabHasFault);
    
    m_checkBoxPlotPQData->SetValue(bus->GetElectricalData().plotPQData);
}

BusForm::~BusForm() {}
void BusForm::OnButtonCancelClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
void BusForm::OnButtonOKClick(wxCommandEvent& event)
{
    BusElectricalData data = m_bus->GetElectricalData();
    data.name = m_textCtrlName->GetValue();
    if(!m_bus->DoubleFromString(m_parent, m_textCtrlNomVoltage->GetValue(), data.nominalVoltage,
                                _("Value entered incorrectly in the field \"Rated voltage\".")))
        return;
    data.nominalVoltageUnit = m_choiceNomVoltage->GetSelection() == 0 ? UNIT_V : UNIT_kV;
    data.isVoltageControlled = m_checkBoxCtrlVoltage->GetValue();
    if(data.isVoltageControlled) {
        if(!m_bus->DoubleFromString(m_parent, m_textCtrlCtrlVoltage->GetValue(), data.controlledVoltage,
                                    _("Value entered incorrectly in the field \"Controlled voltage\".")))
            return;
        data.controlledVoltageUnitChoice = m_choiceCtrlVoltage->GetSelection();
    }
    data.slackBus = m_checkBoxSlackBus->GetValue();

    data.hasFault = m_checkBoxFault->GetValue();
    switch(m_choiceFaultType->GetSelection()) {
        case 0: {
            data.faultType = FAULT_THREEPHASE;
        } break;
        case 1: {
            data.faultType = FAULT_2LINE;
        } break;
        case 2: {
            data.faultType = FAULT_2LINE_GROUND;
        } break;
        case 3: {
            data.faultType = FAULT_LINE_GROUND;
        } break;
    }

    switch(m_choiceFaultPlace->GetSelection()) {
        case 0: {
            data.faultLocation = FAULT_LINE_A;
        } break;
        case 1: {
            data.faultLocation = FAULT_LINE_B;
        } break;
        case 2: {
            data.faultLocation = FAULT_LINE_C;
        } break;
    }

    if(!m_bus->DoubleFromString(m_parent, m_textCtrlFaultResistance->GetValue(), data.faultResistance,
                                _("Value entered incorrectly in the field \"Fault resistance\".")))
        return;

    if(!m_bus->DoubleFromString(m_parent, m_textCtrlFaultReactance->GetValue(), data.faultReactance,
                                _("Value entered incorrectly in the field \"Fault reactance\".")))
        return;

    data.plotBus = m_checkBoxPlotData->GetValue();
    data.stabHasFault = m_checkBoxStabFault->GetValue();

    if(!m_bus->DoubleFromString(m_parent, m_textCtrlStabFaultTime->GetValue(), data.stabFaultTime,
                                _("Value entered incorrectly in the field \"Time\".")))
        return;

    if(!m_bus->DoubleFromString(m_parent, m_textCtrlStabFaultLength->GetValue(), data.stabFaultLength,
                                _("Value entered incorrectly in the field \"Fault lenght\".")))
        return;

    if(!m_bus->DoubleFromString(m_parent, m_textCtrlStabFaultResistance->GetValue(), data.stabFaultResistance,
                                _("Value entered incorrectly in the field \"Fault resistence (stability)\".")))
        return;

    if(!m_bus->DoubleFromString(m_parent, m_textCtrlStabFaultReactance->GetValue(), data.stabFaultReactance,
                                _("Value entered incorrectly in the field \"Fault reactance (stability)\".")))
        return;
        
    data.plotPQData = m_checkBoxPlotPQData->GetValue();

    m_bus->SetElectricalData(data);

    if(data.stabHasFault)
        m_bus->SetDynamicEvent(true);
    else
        m_bus->SetDynamicEvent(false);

    EndModal(wxID_OK);
}

void BusForm::OnNominalVoltageChoice(wxCommandEvent& event) { UpdateChoiceBoxes(); }
void BusForm::OnFaultTypeChoice(wxCommandEvent& event) { UpdateChoiceBoxes(); }
void BusForm::OnControlledVoltageClick(wxCommandEvent& event)
{
    EnableCtrlVoltageFields(m_checkBoxCtrlVoltage->GetValue());
}
void BusForm::OnInsertFaultClick(wxCommandEvent& event) { EnableFaultFields(m_checkBoxFault->GetValue()); }
void BusForm::OnInsertStabFaultClick(wxCommandEvent& event) { EnableStabFaultFields(m_checkBoxStabFault->GetValue()); }
void BusForm::EnableCtrlVoltageFields(bool enable)
{
    m_textCtrlCtrlVoltage->Enable(enable);
    m_choiceCtrlVoltage->Enable(enable);

    UpdateChoiceBoxes();
}

void BusForm::EnableFaultFields(bool enable)
{
    m_choiceFaultType->Enable(enable);
    m_choiceFaultPlace->Enable(enable);
    m_textCtrlFaultReactance->Enable(enable);
    m_textCtrlFaultResistance->Enable(enable);
    m_staticTextPU_1->Enable(enable);
    m_staticTextPU_2->Enable(enable);

    UpdateChoiceBoxes();
}

void BusForm::EnableStabFaultFields(bool enable)
{
    m_textCtrlStabFaultTime->Enable(enable);
    m_textCtrlStabFaultLength->Enable(enable);
    m_staticTextS_1->Enable(enable);
    m_staticTextS_2->Enable(enable);
    m_textCtrlStabFaultReactance->Enable(enable);
    m_textCtrlStabFaultResistance->Enable(enable);
    m_staticTextPU_3->Enable(enable);
    m_staticTextPU_4->Enable(enable);
}

void BusForm::UpdateChoiceBoxes()
{
    switch(m_choiceFaultType->GetSelection()) {
        case 0:  // three-phase
        {
            m_choiceFaultPlace->Enable(false);
        } break;
        case 1:  // line-to-line
        case 2:  // double line-to-line
        {
            if(m_checkBoxFault->GetValue()) m_choiceFaultPlace->Enable(true);
            m_choiceFaultPlace->SetString(0, _("Lines AB"));
            m_choiceFaultPlace->SetString(1, _("Lines BC"));
            m_choiceFaultPlace->SetString(2, _("Lines CA"));
        } break;
        case 3:  // line-to-ground
        {
            if(m_checkBoxFault->GetValue()) m_choiceFaultPlace->Enable(true);
            m_choiceFaultPlace->SetString(0, _("Line A"));
            m_choiceFaultPlace->SetString(1, _("Line B"));
            m_choiceFaultPlace->SetString(2, _("Line C"));
        } break;
        default:
            break;
    }
    switch(m_choiceNomVoltage->GetSelection()) {
        case 0: {
            m_choiceCtrlVoltage->SetString(1, _("V"));
        } break;
        case 1: {
            m_choiceCtrlVoltage->SetString(1, _("kV"));
        } break;
        default:
            break;
    }
}
