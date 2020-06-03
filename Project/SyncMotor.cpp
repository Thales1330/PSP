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
void SyncMotor::DrawDCSymbol(wxGraphicsContext* gc) const
{
    DrawDCArc(m_position, 12, 30, 330, 10, gc);
}
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
    if(data.useMachineBase) { machineBasePower = GetValueFromUnit(data.nominalPower, data.nominalPowerUnit); }

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

rapidxml::xml_node<>* SyncMotor::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
    auto elementNode = XMLParser::AppendNode(doc, elementListNode, "SyncMotor");
    XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

    SaveCADProperties(doc, elementNode);

    auto electricalProp = XMLParser::AppendNode(doc, elementNode, "ElectricalProperties");
    auto isOnline = XMLParser::AppendNode(doc, electricalProp, "IsOnline");
    XMLParser::SetNodeValue(doc, isOnline, m_online);
    auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
    XMLParser::SetNodeValue(doc, name, m_electricalData.name);
    auto nominalPower = XMLParser::AppendNode(doc, electricalProp, "NominalPower");
    XMLParser::SetNodeValue(doc, nominalPower, m_electricalData.nominalPower);
    XMLParser::SetNodeAttribute(doc, nominalPower, "UnitID", m_electricalData.nominalPowerUnit);
    auto activePower = XMLParser::AppendNode(doc, electricalProp, "ActivePower");
    XMLParser::SetNodeValue(doc, activePower, m_electricalData.activePower);
    XMLParser::SetNodeAttribute(doc, activePower, "UnitID", m_electricalData.activePowerUnit);
    auto reactivePower = XMLParser::AppendNode(doc, electricalProp, "ReactivePower");
    XMLParser::SetNodeValue(doc, reactivePower, m_electricalData.reactivePower);
    XMLParser::SetNodeAttribute(doc, reactivePower, "UnitID", m_electricalData.reactivePowerUnit);
    auto haveMaxReactive = XMLParser::AppendNode(doc, electricalProp, "HaveMaxReactive");
    XMLParser::SetNodeValue(doc, haveMaxReactive, m_electricalData.haveMaxReactive);
    auto maxReactive = XMLParser::AppendNode(doc, electricalProp, "MaxReactive");
    XMLParser::SetNodeValue(doc, maxReactive, m_electricalData.maxReactive);
    XMLParser::SetNodeAttribute(doc, maxReactive, "UnitID", m_electricalData.maxReactiveUnit);
    auto haveMinReactive = XMLParser::AppendNode(doc, electricalProp, "HaveMinReactive");
    XMLParser::SetNodeValue(doc, haveMinReactive, m_electricalData.haveMinReactive);
    auto minReactive = XMLParser::AppendNode(doc, electricalProp, "MinReactive");
    XMLParser::SetNodeValue(doc, minReactive, m_electricalData.minReactive);
    XMLParser::SetNodeAttribute(doc, minReactive, "UnitID", m_electricalData.minReactiveUnit);
    auto useMachineBase = XMLParser::AppendNode(doc, electricalProp, "UseMachineBase");
    XMLParser::SetNodeValue(doc, useMachineBase, m_electricalData.useMachineBase);

    auto fault = XMLParser::AppendNode(doc, electricalProp, "Fault");
    auto positiveResistance = XMLParser::AppendNode(doc, fault, "PositiveResistance");
    XMLParser::SetNodeValue(doc, positiveResistance, m_electricalData.positiveResistance);
    auto positiveReactance = XMLParser::AppendNode(doc, fault, "PositiveReactance");
    XMLParser::SetNodeValue(doc, positiveReactance, m_electricalData.positiveReactance);
    auto negativeResistance = XMLParser::AppendNode(doc, fault, "NegativeResistance");
    XMLParser::SetNodeValue(doc, negativeResistance, m_electricalData.negativeResistance);
    auto negativeReactance = XMLParser::AppendNode(doc, fault, "NegativeReactance");
    XMLParser::SetNodeValue(doc, negativeReactance, m_electricalData.negativeReactance);
    auto zeroResistance = XMLParser::AppendNode(doc, fault, "ZeroResistance");
    XMLParser::SetNodeValue(doc, zeroResistance, m_electricalData.zeroResistance);
    auto zeroReactance = XMLParser::AppendNode(doc, fault, "ZeroReactance");
    XMLParser::SetNodeValue(doc, zeroReactance, m_electricalData.zeroReactance);
    auto groundResistance = XMLParser::AppendNode(doc, fault, "GroundResistance");
    XMLParser::SetNodeValue(doc, groundResistance, m_electricalData.groundResistance);
    auto groundReactance = XMLParser::AppendNode(doc, fault, "GroundReactance");
    XMLParser::SetNodeValue(doc, groundReactance, m_electricalData.groundReactance);
    auto groundNeutral = XMLParser::AppendNode(doc, fault, "GroundNeutral");
    XMLParser::SetNodeValue(doc, groundNeutral, m_electricalData.groundNeutral);

    return elementNode;
}

bool SyncMotor::OpenElement(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList)
{
    if(!OpenCADProperties(elementNode, parentList)) return false;

    auto electricalProp = elementNode->first_node("ElectricalProperties");
    if(!electricalProp) return false;

    SetOnline(XMLParser::GetNodeValueInt(electricalProp, "IsOnline"));
    m_electricalData.name = electricalProp->first_node("Name")->value();
    m_electricalData.nominalPower = XMLParser::GetNodeValueDouble(electricalProp, "NominalPower");
    m_electricalData.nominalPowerUnit =
        static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "NominalPower", "UnitID"));
    m_electricalData.activePower = XMLParser::GetNodeValueDouble(electricalProp, "ActivePower");
    m_electricalData.activePowerUnit =
        static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "ActivePower", "UnitID"));
    m_electricalData.reactivePower = XMLParser::GetNodeValueDouble(electricalProp, "ReactivePower");
    m_electricalData.reactivePowerUnit =
        static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "ReactivePower", "UnitID"));
    m_electricalData.haveMaxReactive = XMLParser::GetNodeValueInt(electricalProp, "HaveMaxReactive");
    m_electricalData.maxReactive = XMLParser::GetNodeValueDouble(electricalProp, "MaxReactive");
    m_electricalData.maxReactiveUnit =
        static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "MaxReactive", "UnitID"));
    m_electricalData.haveMinReactive = XMLParser::GetNodeValueInt(electricalProp, "HaveMinReactive");
    m_electricalData.minReactive = XMLParser::GetNodeValueDouble(electricalProp, "MinReactive");
    m_electricalData.minReactiveUnit =
        static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "MinReactive", "UnitID"));
    m_electricalData.useMachineBase = XMLParser::GetNodeValueInt(electricalProp, "UseMachineBase");

    auto fault = electricalProp->first_node("Fault");
    if(!fault) return false;
    m_electricalData.positiveResistance = XMLParser::GetNodeValueDouble(fault, "PositiveResistance");
    m_electricalData.positiveReactance = XMLParser::GetNodeValueDouble(fault, "PositiveReactance");
    m_electricalData.negativeResistance = XMLParser::GetNodeValueDouble(fault, "NegativeResistance");
    m_electricalData.negativeReactance = XMLParser::GetNodeValueDouble(fault, "NegativeReactance");
    m_electricalData.zeroResistance = XMLParser::GetNodeValueDouble(fault, "ZeroResistance");
    m_electricalData.zeroReactance = XMLParser::GetNodeValueDouble(fault, "ZeroReactance");
    m_electricalData.groundResistance = XMLParser::GetNodeValueDouble(fault, "GroundResistance");
    m_electricalData.groundReactance = XMLParser::GetNodeValueDouble(fault, "GroundReactance");
    m_electricalData.groundNeutral = XMLParser::GetNodeValueInt(fault, "GroundNeutral");

    m_inserted = true;

    return true;
}
