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

#include "SyncMachineForm.h"
#include "SyncMotor.h"

SyncMotor::SyncMotor() : Machines() {}
SyncMotor::SyncMotor(wxString name) : Machines() { m_electricalData.name = name; }
SyncMotor::~SyncMotor() {}
void SyncMotor::DrawSymbol() const { DrawArc(m_position, 12, 30, 330, 10, GL_LINE_STRIP); }
bool SyncMotor::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_ELEMENT, _("Edit Synchronous Condenser"));
    GeneralMenuItens(menu);

    return true;
}

bool SyncMotor::ShowForm(wxWindow* parent, Element* element)
{
    SyncMachineForm* syncMotorForm = new SyncMachineForm(parent, this);
    syncMotorForm->SetTitle(_("Synchronous Condenser"));
    if(syncMotorForm->ShowModal() == wxID_OK) {
        syncMotorForm->Destroy();
        return true;
    }

    syncMotorForm->Destroy();
    return false;
}

SyncMotorElectricalData SyncMotor::GetPUElectricalData(double systemPowerBase)
{
    SyncMotorElectricalData data = m_electricalData;
    double machineBasePower = 1.0;
    if(data.useMachineBase) {
        machineBasePower = GetValueFromUnit(data.nominalPower, data.nominalPowerUnit);
    }

    // Active power
    double activePower = GetValueFromUnit(data.activePower, data.activePowerUnit);
    if(!m_online) activePower = 0.0;
    if(data.activePowerUnit == UNIT_PU) {
        if(data.useMachineBase) data.activePower = (activePower * machineBasePower) / systemPowerBase;
    } else {
        data.activePower = activePower / systemPowerBase;
    }
    data.activePowerUnit = UNIT_PU;

    // Reactive power
    double reactivePower = GetValueFromUnit(data.reactivePower, data.reactivePowerUnit);
    if(!m_online) reactivePower = 0.0;
    if(data.reactivePowerUnit == UNIT_PU) {
        if(data.useMachineBase) data.reactivePower = (reactivePower * machineBasePower) / systemPowerBase;
    } else {
        data.reactivePower = reactivePower / systemPowerBase;
    }
    data.reactivePowerUnit = UNIT_PU;

    // Max reactive power
    double maxReactive = GetValueFromUnit(data.maxReactive, data.maxReactiveUnit);
    if(data.maxReactiveUnit == UNIT_PU) {
        if(data.useMachineBase) data.maxReactive = (maxReactive * machineBasePower) / systemPowerBase;
    } else {
        data.maxReactive = maxReactive / systemPowerBase;
    }
    data.maxReactiveUnit = UNIT_PU;

    // Min reactive power
    double minReactive = GetValueFromUnit(data.minReactive, data.minReactiveUnit);
    if(data.minReactiveUnit == UNIT_PU) {
        if(data.useMachineBase) data.minReactive = (minReactive * machineBasePower) / systemPowerBase;
    } else {
        data.minReactive = minReactive / systemPowerBase;
    }
    data.minReactiveUnit = UNIT_PU;

    double baseVoltage = GetValueFromUnit(data.nominalVoltage, data.nominalVoltageUnit);
    double systemBaseImpedance = (baseVoltage * baseVoltage) / systemPowerBase;
    double machineBaseImpedance = (baseVoltage * baseVoltage) / machineBasePower;

    // Fault data
    if(data.useMachineBase) {
        data.positiveResistance = (data.positiveResistance * machineBaseImpedance) / systemBaseImpedance;
        data.positiveReactance = (data.positiveReactance * machineBaseImpedance) / systemBaseImpedance;
        data.negativeResistance = (data.negativeResistance * machineBaseImpedance) / systemBaseImpedance;
        data.negativeReactance = (data.negativeReactance * machineBaseImpedance) / systemBaseImpedance;
        data.zeroResistance = (data.zeroResistance * machineBaseImpedance) / systemBaseImpedance;
        data.zeroReactance = (data.zeroReactance * machineBaseImpedance) / systemBaseImpedance;
        data.groundResistance = (data.groundResistance * machineBaseImpedance) / systemBaseImpedance;
        data.groundReactance = (data.groundReactance * machineBaseImpedance) / systemBaseImpedance;
    }

    if(!m_online) {
        data.faultCurrent[0] = std::complex<double>(0, 0);
        data.faultCurrent[1] = std::complex<double>(0, 0);
        data.faultCurrent[2] = std::complex<double>(0, 0);
    }

    return data;
}

void SyncMotor::SetNominalVoltage(std::vector<double> nominalVoltage, std::vector<ElectricalUnit> nominalVoltageUnit)
{
    if(nominalVoltage.size() > 0) {
        m_electricalData.nominalVoltage = nominalVoltage[0];
        m_electricalData.nominalVoltageUnit = nominalVoltageUnit[0];
    }
}

Element* SyncMotor::GetCopy()
{
    SyncMotor* copy = new SyncMotor();
    *copy = *this;
    return copy;
}

wxString SyncMotor::GetTipText() const
{
    wxString tipText = m_electricalData.name;
    tipText += "\n";
    double activePower = m_electricalData.activePower;
    if(!m_online) activePower = 0.0;
    tipText += _("\nP = ") + wxString::FromDouble(activePower, 5);
    switch(m_electricalData.activePowerUnit) {
        case UNIT_PU: {
            tipText += _(" p.u.");
        } break;
        case UNIT_W: {
            tipText += _(" W");
        } break;
        case UNIT_kW: {
            tipText += _(" kW");
        } break;
        case UNIT_MW: {
            tipText += _(" MW");
        } break;
        default:
            break;
    }
    double reactivePower = m_electricalData.reactivePower;
    if(!m_online) reactivePower = 0.0;
    tipText += _("\nQ = ") + wxString::FromDouble(reactivePower, 5);
    switch(m_electricalData.reactivePowerUnit) {
        case UNIT_PU: {
            tipText += _(" p.u.");
        } break;
        case UNIT_VAr: {
            tipText += _(" VAr");
        } break;
        case UNIT_kVAr: {
            tipText += _(" kVAr");
        } break;
        case UNIT_MVAr: {
            tipText += _(" MVAr");
        } break;
        default:
            break;
    }

    return tipText;
}
