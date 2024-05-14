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

#include "Workspace.h"
#include "ControlElementContainer.h"
#include "SyncGenerator.h"
#include "SyncMachineForm.h"

SyncGenerator::SyncGenerator() : Machines() {
    m_elementType = TYPE_SYNC_GENERATOR;
    Init();
}
SyncGenerator::SyncGenerator(wxString name) : Machines()
{
    m_elementType = TYPE_SYNC_GENERATOR;
    Init();
    m_electricalData.name = name;
}

SyncGenerator::~SyncGenerator() {}
void SyncGenerator::Init()
{
    int numPtsSine = 10;
    double mx = 15.0;
    double my = 10.0;
    double pi = 3.14159265359;

    for(int i = 0; i <= numPtsSine; i++) {
        double x = (2.0 * pi / double(numPtsSine)) * double(i) - pi;
        double y = std::sin(x);
        m_sinePts.push_back(wxPoint2DDouble((x / pi) * mx, y * my));
    }

    m_electricalData.avr = new ControlElementContainer();
    m_electricalData.speedGov = new ControlElementContainer();
}

void SyncGenerator::DrawSymbol() const
{
    // Draw sine.
    std::vector<wxPoint2DDouble> sinePts;
    for(int i = 0; i < (int)m_sinePts.size(); i++) { sinePts.push_back(m_sinePts[i] + m_position); }
    DrawLine(sinePts);
}

void SyncGenerator::DrawDCSymbol(wxGraphicsContext* gc) const
{
	// Draw sine.
	std::vector<wxPoint2DDouble> sinePts;
	for (unsigned int i = 0; i < m_sinePts.size(); i++) { sinePts.push_back(m_sinePts[i] + m_position); }
	gc->DrawLines(sinePts.size(), &sinePts[0]);
}

bool SyncGenerator::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_ELEMENT, _("Edit Generator"));

    wxMenu* textMenu = new wxMenu();

    textMenu->Append(ID_TXT_NAME, _("Name"));
    textMenu->Append(ID_TXT_ACTIVE_POWER, _("Active power"));
    textMenu->Append(ID_TXT_REACTIVE_POWER, _("Reactive power"));
    textMenu->Append(ID_TXT_FAULTCURRENT, _("Fault current"));
    textMenu->SetClientData(menu.GetClientData());
    menu.AppendSubMenu(textMenu, _("Add text"));

    GeneralMenuItens(menu);
    return true;
}

bool SyncGenerator::ShowForm(wxWindow* parent, Element* element)
{
    Workspace* ws = static_cast<Workspace*>(parent);
    SyncMachineForm* generatorForm = new SyncMachineForm(parent, this, ws->GetSharedGLContext());
    generatorForm->SetTitle(_("Generator"));
    if(generatorForm->ShowModal() == wxID_OK) {
        generatorForm->Destroy();
        return true;
    }

    generatorForm->Destroy();
    return false;
}

SyncGeneratorElectricalData SyncGenerator::GetPUElectricalData(double systemPowerBase)
{
    SyncGeneratorElectricalData data = m_electricalData;
    double machineBasePower = 1.0;
    if(data.useMachineBase) { machineBasePower = GetValueFromUnit(data.nominalPower, data.nominalPowerUnit); }

    // Active power
    double activePower = GetValueFromUnit(data.activePower, data.activePowerUnit);
    if(!m_online) activePower = 0.0;
    if(data.activePowerUnit == ElectricalUnit::UNIT_PU) {
        if(data.useMachineBase) data.activePower = (activePower * machineBasePower) / systemPowerBase;
    } else {
        data.activePower = activePower / systemPowerBase;
    }
    data.activePowerUnit = ElectricalUnit::UNIT_PU;

    // Reactive power
    double reactivePower = GetValueFromUnit(data.reactivePower, data.reactivePowerUnit);
    if(!m_online) reactivePower = 0.0;
    if(data.reactivePowerUnit == ElectricalUnit::UNIT_PU) {
        if(data.useMachineBase) data.reactivePower = (reactivePower * machineBasePower) / systemPowerBase;
    } else {
        data.reactivePower = reactivePower / systemPowerBase;
    }
    data.reactivePowerUnit = ElectricalUnit::UNIT_PU;

    // Max reactive power
    double maxReactive = GetValueFromUnit(data.maxReactive, data.maxReactiveUnit);
    if(data.maxReactiveUnit == ElectricalUnit::UNIT_PU) {
        if(data.useMachineBase) data.maxReactive = (maxReactive * machineBasePower) / systemPowerBase;
    } else {
        data.maxReactive = maxReactive / systemPowerBase;
    }
    data.maxReactiveUnit = ElectricalUnit::UNIT_PU;

    // Min reactive power
    double minReactive = GetValueFromUnit(data.minReactive, data.minReactiveUnit);
    if(data.minReactiveUnit == ElectricalUnit::UNIT_PU) {
        if(data.useMachineBase) data.minReactive = (minReactive * machineBasePower) / systemPowerBase;
    } else {
        data.minReactive = minReactive / systemPowerBase;
    }
    data.minReactiveUnit = ElectricalUnit::UNIT_PU;

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

void SyncGenerator::SetNominalVoltage(std::vector<double> nominalVoltage,
                                      std::vector<ElectricalUnit> nominalVoltageUnit)
{
    if(nominalVoltage.size() > 0) {
        m_electricalData.nominalVoltage = nominalVoltage[0];
        m_electricalData.nominalVoltageUnit = nominalVoltageUnit[0];
    }
}

Element* SyncGenerator::GetCopy()
{
    SyncGenerator* copy = new SyncGenerator();
    *copy = *this;
    auto data = copy->GetElectricalData();

    // Copy AVR
    std::vector<ConnectionLine*> cLineList;
    std::vector<ControlElement*> elementList;
    m_electricalData.avr->GetContainerCopy(elementList, cLineList);

    ControlElementContainer* avrCopy = new ControlElementContainer();
    avrCopy->FillContainer(elementList, cLineList);
    data.avr = avrCopy;

    // Copy Speed Governor
    cLineList.clear();
    elementList.clear();
    m_electricalData.speedGov->GetContainerCopy(elementList, cLineList);

    ControlElementContainer* speedGovCopy = new ControlElementContainer();
    speedGovCopy->FillContainer(elementList, cLineList);
    data.speedGov = speedGovCopy;

    copy->SetElectricalData(data);
    return copy;
}

wxString SyncGenerator::GetTipText() const
{
    wxString tipText = m_electricalData.name;
    tipText += "\n";
    double activePower = m_electricalData.activePower;
    if(!m_online) activePower = 0.0;
    tipText += _("\nP = ") + wxString::FromDouble(activePower, 5);
    switch(m_electricalData.activePowerUnit) {
        case ElectricalUnit::UNIT_PU: {
            tipText += _(" p.u.");
        } break;
        case ElectricalUnit::UNIT_W: {
            tipText += _(" W");
        } break;
        case ElectricalUnit::UNIT_kW: {
            tipText += _(" kW");
        } break;
        case ElectricalUnit::UNIT_MW: {
            tipText += _(" MW");
        } break;
        default:
            break;
    }
    double reactivePower = m_electricalData.reactivePower;
    if(!m_online) reactivePower = 0.0;
    tipText += _("\nQ = ") + wxString::FromDouble(reactivePower, 5);
    switch(m_electricalData.reactivePowerUnit) {
        case ElectricalUnit::UNIT_PU: {
            tipText += _(" p.u.");
        } break;
        case ElectricalUnit::UNIT_var: {
            tipText += _(" VAr");
        } break;
        case ElectricalUnit::UNIT_kvar: {
            tipText += _(" kVAr");
        } break;
        case ElectricalUnit::UNIT_Mvar: {
            tipText += _(" MVAr");
        } break;
        default:
            break;
    }

    return tipText;
}

bool SyncGenerator::GetPlotData(ElementPlotData& plotData, PlotStudy study)
{
    if (study == PlotStudy::STABILITY) {
        if (!m_electricalData.plotSyncMachine) return false;
        plotData.SetName(m_electricalData.name);
        plotData.SetCurveType(ElementPlotData::CurveType::CT_SYNC_GENERATOR);

        std::vector<double> absTerminalVoltage, activePower, reactivePower;
        for (unsigned int i = 0; i < m_electricalData.terminalVoltageVector.size(); ++i) {
            activePower.push_back(std::real(m_electricalData.electricalPowerVector[i]));
            reactivePower.push_back(std::imag(m_electricalData.electricalPowerVector[i]));
        }
        plotData.AddData(m_electricalData.terminalVoltageVector, _("Terminal voltage"));
        plotData.AddData(activePower, _("Active power"));
        plotData.AddData(reactivePower, _("Reactive power"));
        plotData.AddData(m_electricalData.mechanicalPowerVector, _("Mechanical power"));
        plotData.AddData(m_electricalData.freqVector, _("Frequency"));
        plotData.AddData(m_electricalData.fieldVoltageVector, _("Field voltage"));
        plotData.AddData(m_electricalData.deltaVector, _("Delta"));
        return true;
    }
    return false;
}

rapidxml::xml_node<>* SyncGenerator::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
    auto elementNode = XMLParser::AppendNode(doc, elementListNode, "SyncGenerator");
    XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

    SaveCADProperties(doc, elementNode);
    auto electricalProp = XMLParser::AppendNode(doc, elementNode, "ElectricalProperties");
    auto isOnline = XMLParser::AppendNode(doc, electricalProp, "IsOnline");
    XMLParser::SetNodeValue(doc, isOnline, m_online);
    auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
    XMLParser::SetNodeValue(doc, name, m_electricalData.name);
    auto nominalPower = XMLParser::AppendNode(doc, electricalProp, "NominalPower");
    XMLParser::SetNodeValue(doc, nominalPower, m_electricalData.nominalPower);
    XMLParser::SetNodeAttribute(doc, nominalPower, "UnitID", static_cast<int>(m_electricalData.nominalPowerUnit));
    auto nominalVoltage = XMLParser::AppendNode(doc, electricalProp, "NominalVoltage");
    XMLParser::SetNodeValue(doc, nominalVoltage, m_electricalData.nominalVoltage);
    XMLParser::SetNodeAttribute(doc, nominalVoltage, "UnitID", static_cast<int>(m_electricalData.nominalVoltageUnit));
    auto activePower = XMLParser::AppendNode(doc, electricalProp, "ActivePower");
    XMLParser::SetNodeValue(doc, activePower, m_electricalData.activePower);
    XMLParser::SetNodeAttribute(doc, activePower, "UnitID", static_cast<int>(m_electricalData.activePowerUnit));
    auto reactivePower = XMLParser::AppendNode(doc, electricalProp, "ReactivePower");
    XMLParser::SetNodeValue(doc, reactivePower, m_electricalData.reactivePower);
    XMLParser::SetNodeAttribute(doc, reactivePower, "UnitID", static_cast<int>(m_electricalData.reactivePowerUnit));
    auto haveMaxReactive = XMLParser::AppendNode(doc, electricalProp, "HaveMaxReactive");
    XMLParser::SetNodeValue(doc, haveMaxReactive, m_electricalData.haveMaxReactive);
    auto maxReactive = XMLParser::AppendNode(doc, electricalProp, "MaxReactive");
    XMLParser::SetNodeValue(doc, maxReactive, m_electricalData.maxReactive);
    XMLParser::SetNodeAttribute(doc, maxReactive, "UnitID", static_cast<int>(m_electricalData.maxReactiveUnit));
    auto haveMinReactive = XMLParser::AppendNode(doc, electricalProp, "HaveMinReactive");
    XMLParser::SetNodeValue(doc, haveMinReactive, m_electricalData.haveMinReactive);
    auto minReactive = XMLParser::AppendNode(doc, electricalProp, "MinReactive");
    XMLParser::SetNodeValue(doc, minReactive, m_electricalData.minReactive);
    XMLParser::SetNodeAttribute(doc, minReactive, "UnitID", static_cast<int>(m_electricalData.minReactiveUnit));
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

    auto stability = XMLParser::AppendNode(doc, electricalProp, "Stability");
    auto plotSyncMachine = XMLParser::AppendNode(doc, stability, "PlotSyncMachine");
    XMLParser::SetNodeValue(doc, plotSyncMachine, m_electricalData.plotSyncMachine);
    auto inertia = XMLParser::AppendNode(doc, stability, "Inertia");
    XMLParser::SetNodeValue(doc, inertia, m_electricalData.inertia);
    auto damping = XMLParser::AppendNode(doc, stability, "Damping");
    XMLParser::SetNodeValue(doc, damping, m_electricalData.damping);
    auto useAVR = XMLParser::AppendNode(doc, stability, "UseAVR");
    XMLParser::SetNodeValue(doc, useAVR, m_electricalData.useAVR);
    auto useSpeedGovernor = XMLParser::AppendNode(doc, stability, "UseSpeedGovernor");
    XMLParser::SetNodeValue(doc, useSpeedGovernor, m_electricalData.useSpeedGovernor);
    auto armResistance = XMLParser::AppendNode(doc, stability, "ArmResistance");
    XMLParser::SetNodeValue(doc, armResistance, m_electricalData.armResistance);
    auto potierReactance = XMLParser::AppendNode(doc, stability, "PotierReactance");
    XMLParser::SetNodeValue(doc, potierReactance, m_electricalData.potierReactance);
    auto satFactor = XMLParser::AppendNode(doc, stability, "SatFactor");
    XMLParser::SetNodeValue(doc, satFactor, m_electricalData.satFactor);
    auto syncXd = XMLParser::AppendNode(doc, stability, "SyncXd");
    XMLParser::SetNodeValue(doc, syncXd, m_electricalData.syncXd);
    auto syncXq = XMLParser::AppendNode(doc, stability, "SyncXq");
    XMLParser::SetNodeValue(doc, syncXq, m_electricalData.syncXq);
    auto transXd = XMLParser::AppendNode(doc, stability, "TransXd");
    XMLParser::SetNodeValue(doc, transXd, m_electricalData.transXd);
    auto transXq = XMLParser::AppendNode(doc, stability, "TransXq");
    XMLParser::SetNodeValue(doc, transXq, m_electricalData.transXq);
    auto transTd0 = XMLParser::AppendNode(doc, stability, "TransTd0");
    XMLParser::SetNodeValue(doc, transTd0, m_electricalData.transTd0);
    auto transTq0 = XMLParser::AppendNode(doc, stability, "TransTq0");
    XMLParser::SetNodeValue(doc, transTq0, m_electricalData.transTq0);
    auto subXd = XMLParser::AppendNode(doc, stability, "SubXd");
    XMLParser::SetNodeValue(doc, subXd, m_electricalData.subXd);
    auto subXq = XMLParser::AppendNode(doc, stability, "SubXq");
    XMLParser::SetNodeValue(doc, subXq, m_electricalData.subXq);
    auto subTd0 = XMLParser::AppendNode(doc, stability, "SubTd0");
    XMLParser::SetNodeValue(doc, subTd0, m_electricalData.subTd0);
    auto subTq0 = XMLParser::AppendNode(doc, stability, "SubTq0");
    XMLParser::SetNodeValue(doc, subTq0, m_electricalData.subTq0);

    SaveSwitchingData(doc, electricalProp);
    return elementNode;
}

bool SyncGenerator::OpenElement(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList)
{
    if(!OpenCADProperties(elementNode, parentList)) return false;

    auto electricalProp = elementNode->first_node("ElectricalProperties");
    if(!electricalProp) return false;

    SetOnline(XMLParser::GetNodeValueInt(electricalProp, "IsOnline"));
    m_electricalData.name = electricalProp->first_node("Name")->value();
    m_electricalData.nominalPower = XMLParser::GetNodeValueDouble(electricalProp, "NominalPower");
    m_electricalData.nominalPowerUnit =
        static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "NominalPower", "UnitID"));
    m_electricalData.nominalVoltage = XMLParser::GetNodeValueDouble(electricalProp, "NominalVoltage");
    m_electricalData.nominalVoltageUnit =
        static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "NominalVoltage", "UnitID"));
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

    auto stability = electricalProp->first_node("Stability");
    if(!stability) return false;
    m_electricalData.plotSyncMachine = XMLParser::GetNodeValueInt(stability, "PlotSyncMachine");
    m_electricalData.inertia = XMLParser::GetNodeValueDouble(stability, "Inertia");
    m_electricalData.damping = XMLParser::GetNodeValueDouble(stability, "Damping");
    m_electricalData.useAVR = XMLParser::GetNodeValueInt(stability, "UseAVR");
    m_electricalData.useSpeedGovernor = XMLParser::GetNodeValueInt(stability, "UseSpeedGovernor");
    m_electricalData.armResistance = XMLParser::GetNodeValueDouble(stability, "ArmResistance");
    m_electricalData.potierReactance = XMLParser::GetNodeValueDouble(stability, "PotierReactance");
    m_electricalData.satFactor = XMLParser::GetNodeValueDouble(stability, "SatFactor");
    m_electricalData.syncXd = XMLParser::GetNodeValueDouble(stability, "SyncXd");
    m_electricalData.syncXq = XMLParser::GetNodeValueDouble(stability, "SyncXq");
    m_electricalData.transXd = XMLParser::GetNodeValueDouble(stability, "TransXd");
    m_electricalData.transXq = XMLParser::GetNodeValueDouble(stability, "TransXq");
    m_electricalData.transTd0 = XMLParser::GetNodeValueDouble(stability, "TransTd0");
    m_electricalData.transTq0 = XMLParser::GetNodeValueDouble(stability, "TransTq0");
    m_electricalData.subXd = XMLParser::GetNodeValueDouble(stability, "SubXd");
    m_electricalData.subXq = XMLParser::GetNodeValueDouble(stability, "SubXq");
    m_electricalData.subTd0 = XMLParser::GetNodeValueDouble(stability, "SubTd0");
    m_electricalData.subTq0 = XMLParser::GetNodeValueDouble(stability, "SubTq0");

    if(!OpenSwitchingData(electricalProp)) return false;
    if(m_swData.swTime.size() != 0) SetDynamicEvent(true);

    m_inserted = true;
    return true;
}

void SyncGenerator::SavePlotData()
{
    m_electricalData.terminalVoltageVector.emplace_back(std::abs(m_electricalData.terminalVoltage));
    m_electricalData.electricalPowerVector.emplace_back(m_electricalData.electricalPower);
    m_electricalData.mechanicalPowerVector.emplace_back(m_electricalData.pm);
    m_electricalData.freqVector.emplace_back(m_electricalData.speed / (2.0f * M_PI));
    m_electricalData.fieldVoltageVector.emplace_back(m_electricalData.fieldVoltage);
    m_electricalData.deltaVector.emplace_back(wxRadToDeg(m_electricalData.delta));
}
