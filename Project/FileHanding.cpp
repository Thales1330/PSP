/*
 *  Copyright (C) 2017  Thales Lima Oliveira <thales@ufu.br>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "FileHanding.h"

FileHanding::~FileHanding() {}
FileHanding::FileHanding(Workspace* workspace) { m_workspace = workspace; }
FileHanding::FileHanding(ControlEditor* controlEditor) { m_controlEditor = controlEditor; }
FileHanding::FileHanding() {}
void FileHanding::SaveProject(wxFileName path)
{
    // Erase the file (if exists or not) and write the initial data
    std::ofstream writeProjectsFile(path.GetFullPath());
    writeProjectsFile.close();

    rapidxml::xml_document<> doc;
    rapidxml::file<> xmlFile(path.GetFullPath().mb_str());
    doc.parse<0>(xmlFile.data());

    rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
    rapidxml::xml_attribute<>* ver = doc.allocate_attribute("version", "1.0");
    rapidxml::xml_attribute<>* encoding = doc.allocate_attribute("encoding", "utf-8");
    decl->append_attribute(ver);
    decl->append_attribute(encoding);
    doc.append_node(decl);

    rapidxml::xml_node<>* rootNode = doc.allocate_node(rapidxml::node_element, "Project");
    doc.append_node(rootNode);

    rapidxml::xml_node<>* projectNameNode = XMLParser::AppendNode(doc, rootNode, "Name");
    XMLParser::SetNodeValue(doc, projectNameNode, path.GetName());

    //{ Simulation properties
    PropertiesData* properties = m_workspace->GetProperties();
    auto propertiesNode = XMLParser::AppendNode(doc, rootNode, "Properties");

    SimulationData simulationData = properties->GetSimulationPropertiesData();
    auto simulationPropNode = XMLParser::XMLParser::AppendNode(doc, propertiesNode, "SimulationProperties");

    auto generalPropNode = XMLParser::AppendNode(doc, simulationPropNode, "General");
    auto basePower = XMLParser::AppendNode(doc, generalPropNode, "BasePower");
    XMLParser::SetNodeValue(doc, basePower, simulationData.basePower);
    XMLParser::SetNodeAttribute(doc, basePower, "UnitID", simulationData.basePowerUnit);
    auto contCalc = XMLParser::AppendNode(doc, generalPropNode, "ContinuousCalculation");
    auto contCalcFault = XMLParser::AppendNode(doc, contCalc, "Fault");
    XMLParser::SetNodeValue(doc, contCalcFault, simulationData.faultAfterPowerFlow);
    auto contCalcSCPower = XMLParser::AppendNode(doc, contCalc, "SCPower");
    XMLParser::SetNodeValue(doc, contCalcSCPower, simulationData.scPowerAfterPowerFlow);

    auto powerFlowPropNode = XMLParser::AppendNode(doc, simulationPropNode, "PowerFlow");
    auto solutionMethod = XMLParser::AppendNode(doc, powerFlowPropNode, "SolutionMethod");
    XMLParser::SetNodeValue(doc, solutionMethod, simulationData.powerFlowMethod);
    auto accFactor = XMLParser::AppendNode(doc, powerFlowPropNode, "AccFactor");
    XMLParser::SetNodeValue(doc, accFactor, simulationData.accFator);
    auto pfTolerance = XMLParser::AppendNode(doc, powerFlowPropNode, "Tolerance");
    XMLParser::SetNodeValue(doc, pfTolerance, simulationData.powerFlowTolerance);
    auto pfMaxIter = XMLParser::AppendNode(doc, powerFlowPropNode, "MaxIterations");
    XMLParser::SetNodeValue(doc, pfMaxIter, simulationData.powerFlowMaxIterations);

    auto stabilityPropNode = XMLParser::AppendNode(doc, simulationPropNode, "Stability");
    auto timeStep = XMLParser::AppendNode(doc, stabilityPropNode, "TimeStep");
    XMLParser::SetNodeValue(doc, timeStep, simulationData.timeStep);
    auto simTime = XMLParser::AppendNode(doc, stabilityPropNode, "SimulationTime");
    XMLParser::SetNodeValue(doc, simTime, simulationData.stabilitySimulationTime);
    auto freq = XMLParser::AppendNode(doc, stabilityPropNode, "Frequency");
    XMLParser::SetNodeValue(doc, freq, simulationData.stabilityFrequency);
    auto stabTolerance = XMLParser::AppendNode(doc, stabilityPropNode, "Tolerance");
    XMLParser::SetNodeValue(doc, stabTolerance, simulationData.stabilityTolerance);
    auto stabTMaxIter = XMLParser::AppendNode(doc, stabilityPropNode, "MaxIterations");
    XMLParser::SetNodeValue(doc, stabTMaxIter, simulationData.stabilityMaxIterations);
    auto controlRatio = XMLParser::AppendNode(doc, stabilityPropNode, "ControlStepRatio");
    XMLParser::SetNodeValue(doc, controlRatio, simulationData.controlTimeStepRatio);
    auto plotStep = XMLParser::AppendNode(doc, stabilityPropNode, "PlotStep");
    XMLParser::SetNodeValue(doc, plotStep, simulationData.plotTime);
    auto useCOI = XMLParser::AppendNode(doc, stabilityPropNode, "UseCOI");
    XMLParser::SetNodeValue(doc, useCOI, simulationData.useCOI);

    auto zipPropNode = XMLParser::AppendNode(doc, simulationPropNode, "ZIPLoad");
    auto useCompLoads = XMLParser::AppendNode(doc, zipPropNode, "UseCompositeLoad");
    XMLParser::SetNodeValue(doc, useCompLoads, simulationData.useCompLoads);
    auto activePowerComp = XMLParser::AppendNode(doc, zipPropNode, "ActivePowerComposition");
    auto pz = XMLParser::AppendNode(doc, activePowerComp, "ConstantImpedance");
    XMLParser::SetNodeValue(doc, pz, simulationData.constImpedanceActive);
    auto pi = XMLParser::AppendNode(doc, activePowerComp, "ConstantCurrent");
    XMLParser::SetNodeValue(doc, pi, simulationData.constCurrentActive);
    auto pp = XMLParser::AppendNode(doc, activePowerComp, "ConstantPower");
    XMLParser::SetNodeValue(doc, pp, simulationData.constPowerActive);
    auto reactivePowerComp = XMLParser::AppendNode(doc, zipPropNode, "ReactivePowerComposition");
    auto qz = XMLParser::AppendNode(doc, reactivePowerComp, "ConstantImpedance");
    XMLParser::SetNodeValue(doc, qz, simulationData.constImpedanceReactive);
    auto qi = XMLParser::AppendNode(doc, reactivePowerComp, "ConstantCurrent");
    XMLParser::SetNodeValue(doc, qi, simulationData.constCurrentReactive);
    auto qp = XMLParser::AppendNode(doc, reactivePowerComp, "ConstantPower");
    XMLParser::SetNodeValue(doc, qp, simulationData.constPowerReactive);
    auto undervoltageLim = XMLParser::AppendNode(doc, zipPropNode, "UndervoltageLimit");
    auto uvi = XMLParser::AppendNode(doc, undervoltageLim, "ConstantCurrent");
    XMLParser::SetNodeValue(doc, uvi, simulationData.underVoltageConstCurrent);
    auto uvp = XMLParser::AppendNode(doc, undervoltageLim, "ConstantPower");
    XMLParser::SetNodeValue(doc, uvp, simulationData.underVoltageConstPower);

    //}

    auto elementsNode = XMLParser::AppendNode(doc, rootNode, "Elements");

    // Save all the data
    ElectricCalculation allElements;
    allElements.GetElementsFromList(m_workspace->GetElementList());
    int elementID = 0;

    //{ Buses
    auto busesNode = XMLParser::AppendNode(doc, elementsNode, "BusList");
    auto busList = allElements.GetBusList();
    for(auto it = busList.begin(), itEnd = busList.end(); it != itEnd; ++it) {
        (*it)->SetID(elementID);
        (*it)->SaveElement(doc, busesNode);
        elementID++;
    }
    //}

    //{ Capacitor
    auto capacitorsNode = XMLParser::AppendNode(doc, elementsNode, "CapacitorList");
    auto capacitorList = allElements.GetCapacitorList();
    elementID = 0;
    for(auto it = capacitorList.begin(), itEnd = capacitorList.end(); it != itEnd; ++it) {
        (*it)->SetID(elementID);
        (*it)->SaveElement(doc, capacitorsNode);
        elementID++;
    }
    //}

    //{ IndMotor
    auto indMotorsNode = XMLParser::AppendNode(doc, elementsNode, "IndMotorList");
    auto indMotorList = allElements.GetIndMotorList();
    elementID = 0;
    for(auto it = indMotorList.begin(), itEnd = indMotorList.end(); it != itEnd; ++it) {
        (*it)->SetID(elementID);
        (*it)->SaveElement(doc, indMotorsNode);
        elementID++;
    }
    //}

    //{ Inductor
    auto inductorsNode = XMLParser::AppendNode(doc, elementsNode, "InductorList");
    auto inductorList = allElements.GetInductorList();
    for(int i = 0; i < (int)inductorList.size(); i++) {
        Inductor* inductor = inductorList[i];
        auto inductorNode = XMLParser::AppendNode(doc, inductorsNode, "Inductor");
        XMLParser::SetNodeAttribute(doc, inductorNode, "ID", i);
        auto cadProp = XMLParser::AppendNode(doc, inductorNode, "CADProperties");
        auto position = XMLParser::AppendNode(doc, cadProp, "Position");
        auto posX = XMLParser::AppendNode(doc, position, "X");
        XMLParser::SetNodeValue(doc, posX, inductor->GetPosition().m_x);
        auto posY = XMLParser::AppendNode(doc, position, "Y");
        XMLParser::SetNodeValue(doc, posY, inductor->GetPosition().m_y);
        auto size = XMLParser::AppendNode(doc, cadProp, "Size");
        auto width = XMLParser::AppendNode(doc, size, "Width");
        XMLParser::SetNodeValue(doc, width, inductor->GetWidth());
        auto height = XMLParser::AppendNode(doc, size, "Height");
        XMLParser::SetNodeValue(doc, height, inductor->GetHeight());
        auto angle = XMLParser::AppendNode(doc, cadProp, "Angle");
        XMLParser::SetNodeValue(doc, angle, inductor->GetAngle());
        auto nodePos = XMLParser::AppendNode(doc, cadProp, "NodePosition");
        auto nodePosX = XMLParser::AppendNode(doc, nodePos, "X");
        XMLParser::SetNodeValue(doc, nodePosX, inductor->GetPointList()[0].m_x);
        auto nodePosY = XMLParser::AppendNode(doc, nodePos, "Y");
        XMLParser::SetNodeValue(doc, nodePosY, inductor->GetPointList()[0].m_y);
        auto parentID = XMLParser::AppendNode(doc, cadProp, "ParentID");
        Bus* parent = static_cast<Bus*>(inductor->GetParentList()[0]);
        if(parent) XMLParser::SetNodeValue(doc, parentID, parent->GetElectricalData().number);

        InductorElectricalData data = inductor->GetElectricalData();
        auto electricalProp = XMLParser::AppendNode(doc, inductorNode, "ElectricalProperties");
        auto isOnline = XMLParser::AppendNode(doc, electricalProp, "IsOnline");
        XMLParser::SetNodeValue(doc, isOnline, inductor->IsOnline());
        auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
        XMLParser::SetNodeValue(doc, name, data.name);
        auto reactivePower = XMLParser::AppendNode(doc, electricalProp, "ReactivePower");
        XMLParser::SetNodeValue(doc, reactivePower, data.reactivePower);
        XMLParser::SetNodeAttribute(doc, reactivePower, "UnitID", data.reactivePowerUnit);

        auto switchingList = XMLParser::AppendNode(doc, electricalProp, "SwitchingList");
        SwitchingData swData = inductor->GetSwitchingData();
        for(int j = 0; j < (int)swData.swType.size(); j++) {
            auto switching = XMLParser::AppendNode(doc, switchingList, "Switching");
            XMLParser::SetNodeAttribute(doc, switching, "ID", j);
            auto swType = XMLParser::AppendNode(doc, switching, "Type");
            XMLParser::SetNodeValue(doc, swType, swData.swType[j]);
            auto swTime = XMLParser::AppendNode(doc, switching, "Time");
            XMLParser::SetNodeValue(doc, swTime, swData.swTime[j]);
        }
    }  //}

    //{ Line
    auto linesNode = XMLParser::AppendNode(doc, elementsNode, "LineList");
    auto lineList = allElements.GetLineList();
    for(int i = 0; i < (int)lineList.size(); i++) {
        Line* line = lineList[i];
        auto lineNode = XMLParser::AppendNode(doc, linesNode, "Line");
        XMLParser::SetNodeAttribute(doc, lineNode, "ID", i);
        auto cadProp = XMLParser::AppendNode(doc, lineNode, "CADProperties");
        auto nodeList = XMLParser::AppendNode(doc, cadProp, "NodeList");
        auto ptList = line->GetPointList();
        int nodeID = 0;
        for(int j = 0; j < (int)ptList.size(); j++) {
            if((j != 1) && (j != (int)ptList.size() - 2)) {
                auto nodePos = XMLParser::AppendNode(doc, nodeList, "Node");
                XMLParser::SetNodeAttribute(doc, nodePos, "ID", nodeID);
                auto nodePosX = XMLParser::AppendNode(doc, nodePos, "X");
                XMLParser::SetNodeValue(doc, nodePosX, ptList[j].m_x);
                auto nodePosY = XMLParser::AppendNode(doc, nodePos, "Y");
                XMLParser::SetNodeValue(doc, nodePosY, ptList[j].m_y);
                nodeID++;
            }
        }

        auto parentIDList = XMLParser::AppendNode(doc, cadProp, "ParentIDList");
        for(int j = 0; j < (int)line->GetParentList().size(); j++) {
            Bus* parent = static_cast<Bus*>(line->GetParentList()[j]);
            if(parent) {
                auto parentID = XMLParser::AppendNode(doc, parentIDList, "ParentID");
                XMLParser::SetNodeAttribute(doc, parentID, "ID", j);
                XMLParser::SetNodeValue(doc, parentID, parent->GetElectricalData().number);
            }
        }

        LineElectricalData data = line->GetElectricalData();
        auto electricalProp = XMLParser::AppendNode(doc, lineNode, "ElectricalProperties");
        auto isOnline = XMLParser::AppendNode(doc, electricalProp, "IsOnline");
        XMLParser::SetNodeValue(doc, isOnline, line->IsOnline());
        auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
        XMLParser::SetNodeValue(doc, name, data.name);
        auto nominalVoltage = XMLParser::AppendNode(doc, electricalProp, "NominalVoltage");
        XMLParser::SetNodeValue(doc, nominalVoltage, data.nominalVoltage);
        XMLParser::SetNodeAttribute(doc, nominalVoltage, "UnitID", data.nominalVoltageUnit);
        auto nominalPower = XMLParser::AppendNode(doc, electricalProp, "NominalPower");
        XMLParser::SetNodeValue(doc, nominalPower, data.nominalPower);
        XMLParser::SetNodeAttribute(doc, nominalPower, "UnitID", data.nominalPowerUnit);
        auto resistance = XMLParser::AppendNode(doc, electricalProp, "Resistance");
        XMLParser::SetNodeValue(doc, resistance, data.resistance);
        XMLParser::SetNodeAttribute(doc, resistance, "UnitID", data.resistanceUnit);
        auto indReactance = XMLParser::AppendNode(doc, electricalProp, "IndReactance");
        XMLParser::SetNodeValue(doc, indReactance, data.indReactance);
        XMLParser::SetNodeAttribute(doc, indReactance, "UnitID", data.indReactanceUnit);
        auto capSusceptance = XMLParser::AppendNode(doc, electricalProp, "CapSusceptance");
        XMLParser::SetNodeValue(doc, capSusceptance, data.capSusceptance);
        XMLParser::SetNodeAttribute(doc, capSusceptance, "UnitID", data.capSusceptanceUnit);
        auto lineSize = XMLParser::AppendNode(doc, electricalProp, "LineSize");
        XMLParser::SetNodeValue(doc, lineSize, data.lineSize);
        auto useLinePower = XMLParser::AppendNode(doc, electricalProp, "UseLinePower");
        XMLParser::SetNodeValue(doc, useLinePower, data.useLinePower);

        auto fault = XMLParser::AppendNode(doc, electricalProp, "Fault");
        auto zeroResistance = XMLParser::AppendNode(doc, fault, "ZeroResistance");
        XMLParser::SetNodeValue(doc, zeroResistance, data.zeroResistance);
        auto zeroIndReactance = XMLParser::AppendNode(doc, fault, "ZeroIndReactance");
        XMLParser::SetNodeValue(doc, zeroIndReactance, data.zeroIndReactance);
        auto zeroCapSusceptance = XMLParser::AppendNode(doc, fault, "ZeroCapSusceptance");
        XMLParser::SetNodeValue(doc, zeroCapSusceptance, data.zeroCapSusceptance);

        auto switchingList = XMLParser::AppendNode(doc, electricalProp, "SwitchingList");
        SwitchingData swData = line->GetSwitchingData();
        for(int j = 0; j < (int)swData.swType.size(); j++) {
            auto switching = XMLParser::AppendNode(doc, switchingList, "Switching");
            XMLParser::SetNodeAttribute(doc, switching, "ID", j);
            auto swType = XMLParser::AppendNode(doc, switching, "Type");
            XMLParser::SetNodeValue(doc, swType, swData.swType[j]);
            auto swTime = XMLParser::AppendNode(doc, switching, "Time");
            XMLParser::SetNodeValue(doc, swTime, swData.swTime[j]);
        }
    }  //}

    //{ Load
    auto loadsNode = XMLParser::AppendNode(doc, elementsNode, "LoadList");
    auto loadList = allElements.GetLoadList();
    for(int i = 0; i < (int)loadList.size(); i++) {
        Load* load = loadList[i];
        auto loadNode = XMLParser::AppendNode(doc, loadsNode, "Load");
        XMLParser::SetNodeAttribute(doc, loadNode, "ID", i);
        auto cadProp = XMLParser::AppendNode(doc, loadNode, "CADProperties");
        auto position = XMLParser::AppendNode(doc, cadProp, "Position");
        auto posX = XMLParser::AppendNode(doc, position, "X");
        XMLParser::SetNodeValue(doc, posX, load->GetPosition().m_x);
        auto posY = XMLParser::AppendNode(doc, position, "Y");
        XMLParser::SetNodeValue(doc, posY, load->GetPosition().m_y);
        auto size = XMLParser::AppendNode(doc, cadProp, "Size");
        auto width = XMLParser::AppendNode(doc, size, "Width");
        XMLParser::SetNodeValue(doc, width, load->GetWidth());
        auto height = XMLParser::AppendNode(doc, size, "Height");
        XMLParser::SetNodeValue(doc, height, load->GetHeight());
        auto angle = XMLParser::AppendNode(doc, cadProp, "Angle");
        XMLParser::SetNodeValue(doc, angle, load->GetAngle());
        auto nodePos = XMLParser::AppendNode(doc, cadProp, "NodePosition");
        auto nodePosX = XMLParser::AppendNode(doc, nodePos, "X");
        XMLParser::SetNodeValue(doc, nodePosX, load->GetPointList()[0].m_x);
        auto nodePosY = XMLParser::AppendNode(doc, nodePos, "Y");
        XMLParser::SetNodeValue(doc, nodePosY, load->GetPointList()[0].m_y);
        auto parentID = XMLParser::AppendNode(doc, cadProp, "ParentID");
        Bus* parent = static_cast<Bus*>(load->GetParentList()[0]);
        if(parent) XMLParser::SetNodeValue(doc, parentID, parent->GetElectricalData().number);

        LoadElectricalData data = load->GetElectricalData();
        auto electricalProp = XMLParser::AppendNode(doc, loadNode, "ElectricalProperties");
        auto isOnline = XMLParser::AppendNode(doc, electricalProp, "IsOnline");
        XMLParser::SetNodeValue(doc, isOnline, load->IsOnline());
        auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
        XMLParser::SetNodeValue(doc, name, data.name);
        auto activePower = XMLParser::AppendNode(doc, electricalProp, "ActivePower");
        XMLParser::SetNodeValue(doc, activePower, data.activePower);
        XMLParser::SetNodeAttribute(doc, activePower, "UnitID", data.activePowerUnit);
        auto reactivePower = XMLParser::AppendNode(doc, electricalProp, "ReactivePower");
        XMLParser::SetNodeValue(doc, reactivePower, data.reactivePower);
        XMLParser::SetNodeAttribute(doc, reactivePower, "UnitID", data.reactivePowerUnit);
        auto loadType = XMLParser::AppendNode(doc, electricalProp, "LoadType");
        XMLParser::SetNodeValue(doc, loadType, data.loadType);

        auto stability = XMLParser::AppendNode(doc, electricalProp, "Stability");
        auto plotLoad = XMLParser::AppendNode(doc, stability, "PlotLoad");
        XMLParser::SetNodeValue(doc, plotLoad, data.plotLoad);
        auto useCompLoad = XMLParser::AppendNode(doc, stability, "UseCompositeLoad");
        XMLParser::SetNodeValue(doc, useCompLoad, data.useCompLoad);
        auto activePowerCompl = XMLParser::AppendNode(doc, stability, "ActivePowerComposition");
        auto pzl = XMLParser::AppendNode(doc, activePowerCompl, "ConstantImpedance");
        XMLParser::SetNodeValue(doc, pzl, data.constImpedanceActive);
        auto pil = XMLParser::AppendNode(doc, activePowerCompl, "ConstantCurrent");
        XMLParser::SetNodeValue(doc, pil, data.constCurrentActive);
        auto ppl = XMLParser::AppendNode(doc, activePowerCompl, "ConstantPower");
        XMLParser::SetNodeValue(doc, ppl, data.constPowerActive);
        auto reactivePowerCompl = XMLParser::AppendNode(doc, stability, "ReactivePowerComposition");
        auto qzl = XMLParser::AppendNode(doc, reactivePowerCompl, "ConstantImpedance");
        XMLParser::SetNodeValue(doc, qzl, data.constImpedanceReactive);
        auto qil = XMLParser::AppendNode(doc, reactivePowerCompl, "ConstantCurrent");
        XMLParser::SetNodeValue(doc, qil, data.constCurrentReactive);
        auto qpl = XMLParser::AppendNode(doc, reactivePowerCompl, "ConstantPower");
        XMLParser::SetNodeValue(doc, qpl, data.constPowerReactive);

        auto switchingList = XMLParser::AppendNode(doc, electricalProp, "SwitchingList");
        SwitchingData swData = load->GetSwitchingData();
        for(int j = 0; j < (int)swData.swType.size(); j++) {
            auto switching = XMLParser::AppendNode(doc, switchingList, "Switching");
            XMLParser::SetNodeAttribute(doc, switching, "ID", j);
            auto swType = XMLParser::AppendNode(doc, switching, "Type");
            XMLParser::SetNodeValue(doc, swType, swData.swType[j]);
            auto swTime = XMLParser::AppendNode(doc, switching, "Time");
            XMLParser::SetNodeValue(doc, swTime, swData.swTime[j]);
        }
    }  //}

    //{ SyncGenerator
    auto syncGeneratorsNode = XMLParser::AppendNode(doc, elementsNode, "SyncGeneratorList");
    auto syncGeneratorList = allElements.GetSyncGeneratorList();
    for(int i = 0; i < (int)syncGeneratorList.size(); i++) {
        SyncGenerator* syncGenerator = syncGeneratorList[i];
        auto syncGeneratorNode = XMLParser::AppendNode(doc, syncGeneratorsNode, "SyncGenerator");
        XMLParser::SetNodeAttribute(doc, syncGeneratorNode, "ID", i);
        auto cadProp = XMLParser::AppendNode(doc, syncGeneratorNode, "CADProperties");
        auto position = XMLParser::AppendNode(doc, cadProp, "Position");
        auto posX = XMLParser::AppendNode(doc, position, "X");
        XMLParser::SetNodeValue(doc, posX, syncGenerator->GetPosition().m_x);
        auto posY = XMLParser::AppendNode(doc, position, "Y");
        XMLParser::SetNodeValue(doc, posY, syncGenerator->GetPosition().m_y);
        auto size = XMLParser::AppendNode(doc, cadProp, "Size");
        auto width = XMLParser::AppendNode(doc, size, "Width");
        XMLParser::SetNodeValue(doc, width, syncGenerator->GetWidth());
        auto height = XMLParser::AppendNode(doc, size, "Height");
        XMLParser::SetNodeValue(doc, height, syncGenerator->GetHeight());
        auto angle = XMLParser::AppendNode(doc, cadProp, "Angle");
        XMLParser::SetNodeValue(doc, angle, syncGenerator->GetAngle());
        auto nodePos = XMLParser::AppendNode(doc, cadProp, "NodePosition");
        auto nodePosX = XMLParser::AppendNode(doc, nodePos, "X");
        XMLParser::SetNodeValue(doc, nodePosX, syncGenerator->GetPointList()[0].m_x);
        auto nodePosY = XMLParser::AppendNode(doc, nodePos, "Y");
        XMLParser::SetNodeValue(doc, nodePosY, syncGenerator->GetPointList()[0].m_y);
        auto parentID = XMLParser::AppendNode(doc, cadProp, "ParentID");
        Bus* parent = static_cast<Bus*>(syncGenerator->GetParentList()[0]);
        if(parent) XMLParser::SetNodeValue(doc, parentID, parent->GetElectricalData().number);

        SyncGeneratorElectricalData data = syncGenerator->GetElectricalData();
        auto electricalProp = XMLParser::AppendNode(doc, syncGeneratorNode, "ElectricalProperties");
        auto isOnline = XMLParser::AppendNode(doc, electricalProp, "IsOnline");
        XMLParser::SetNodeValue(doc, isOnline, syncGenerator->IsOnline());
        auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
        XMLParser::SetNodeValue(doc, name, data.name);
        auto nominalPower = XMLParser::AppendNode(doc, electricalProp, "NominalPower");
        XMLParser::SetNodeValue(doc, nominalPower, data.nominalPower);
        XMLParser::SetNodeAttribute(doc, nominalPower, "UnitID", data.nominalPowerUnit);
        auto nominalVoltage = XMLParser::AppendNode(doc, electricalProp, "NominalVoltage");
        XMLParser::SetNodeValue(doc, nominalVoltage, data.nominalVoltage);
        XMLParser::SetNodeAttribute(doc, nominalVoltage, "UnitID", data.nominalVoltageUnit);
        auto activePower = XMLParser::AppendNode(doc, electricalProp, "ActivePower");
        XMLParser::SetNodeValue(doc, activePower, data.activePower);
        XMLParser::SetNodeAttribute(doc, activePower, "UnitID", data.activePowerUnit);
        auto reactivePower = XMLParser::AppendNode(doc, electricalProp, "ReactivePower");
        XMLParser::SetNodeValue(doc, reactivePower, data.reactivePower);
        XMLParser::SetNodeAttribute(doc, reactivePower, "UnitID", data.reactivePowerUnit);
        auto haveMaxReactive = XMLParser::AppendNode(doc, electricalProp, "HaveMaxReactive");
        XMLParser::SetNodeValue(doc, haveMaxReactive, data.haveMaxReactive);
        auto maxReactive = XMLParser::AppendNode(doc, electricalProp, "MaxReactive");
        XMLParser::SetNodeValue(doc, maxReactive, data.maxReactive);
        XMLParser::SetNodeAttribute(doc, maxReactive, "UnitID", data.maxReactiveUnit);
        auto haveMinReactive = XMLParser::AppendNode(doc, electricalProp, "HaveMinReactive");
        XMLParser::SetNodeValue(doc, haveMinReactive, data.haveMinReactive);
        auto minReactive = XMLParser::AppendNode(doc, electricalProp, "MinReactive");
        XMLParser::SetNodeValue(doc, minReactive, data.minReactive);
        XMLParser::SetNodeAttribute(doc, minReactive, "UnitID", data.minReactiveUnit);
        auto useMachineBase = XMLParser::AppendNode(doc, electricalProp, "UseMachineBase");
        XMLParser::SetNodeValue(doc, useMachineBase, data.useMachineBase);

        auto fault = XMLParser::AppendNode(doc, electricalProp, "Fault");
        auto positiveResistance = XMLParser::AppendNode(doc, fault, "PositiveResistance");
        XMLParser::SetNodeValue(doc, positiveResistance, data.positiveResistance);
        auto positiveReactance = XMLParser::AppendNode(doc, fault, "PositiveReactance");
        XMLParser::SetNodeValue(doc, positiveReactance, data.positiveReactance);
        auto negativeResistance = XMLParser::AppendNode(doc, fault, "NegativeResistance");
        XMLParser::SetNodeValue(doc, negativeResistance, data.negativeResistance);
        auto negativeReactance = XMLParser::AppendNode(doc, fault, "NegativeReactance");
        XMLParser::SetNodeValue(doc, negativeReactance, data.negativeReactance);
        auto zeroResistance = XMLParser::AppendNode(doc, fault, "ZeroResistance");
        XMLParser::SetNodeValue(doc, zeroResistance, data.zeroResistance);
        auto zeroReactance = XMLParser::AppendNode(doc, fault, "ZeroReactance");
        XMLParser::SetNodeValue(doc, zeroReactance, data.zeroReactance);
        auto groundResistance = XMLParser::AppendNode(doc, fault, "GroundResistance");
        XMLParser::SetNodeValue(doc, groundResistance, data.groundResistance);
        auto groundReactance = XMLParser::AppendNode(doc, fault, "GroundReactance");
        XMLParser::SetNodeValue(doc, groundReactance, data.groundReactance);
        auto groundNeutral = XMLParser::AppendNode(doc, fault, "GroundNeutral");
        XMLParser::SetNodeValue(doc, groundNeutral, data.groundNeutral);

        auto stability = XMLParser::AppendNode(doc, electricalProp, "Stability");
        auto plotSyncMachine = XMLParser::AppendNode(doc, stability, "PlotSyncMachine");
        XMLParser::SetNodeValue(doc, plotSyncMachine, data.plotSyncMachine);
        auto inertia = XMLParser::AppendNode(doc, stability, "Inertia");
        XMLParser::SetNodeValue(doc, inertia, data.inertia);
        auto damping = XMLParser::AppendNode(doc, stability, "Damping");
        XMLParser::SetNodeValue(doc, damping, data.damping);
        auto useAVR = XMLParser::AppendNode(doc, stability, "UseAVR");
        XMLParser::SetNodeValue(doc, useAVR, data.useAVR);
        auto useSpeedGovernor = XMLParser::AppendNode(doc, stability, "UseSpeedGovernor");
        XMLParser::SetNodeValue(doc, useSpeedGovernor, data.useSpeedGovernor);
        auto armResistance = XMLParser::AppendNode(doc, stability, "ArmResistance");
        XMLParser::SetNodeValue(doc, armResistance, data.armResistance);
        auto potierReactance = XMLParser::AppendNode(doc, stability, "PotierReactance");
        XMLParser::SetNodeValue(doc, potierReactance, data.potierReactance);
        auto satFactor = XMLParser::AppendNode(doc, stability, "SatFactor");
        XMLParser::SetNodeValue(doc, satFactor, data.satFactor);
        auto syncXd = XMLParser::AppendNode(doc, stability, "SyncXd");
        XMLParser::SetNodeValue(doc, syncXd, data.syncXd);
        auto syncXq = XMLParser::AppendNode(doc, stability, "SyncXq");
        XMLParser::SetNodeValue(doc, syncXq, data.syncXq);
        auto transXd = XMLParser::AppendNode(doc, stability, "TransXd");
        XMLParser::SetNodeValue(doc, transXd, data.transXd);
        auto transXq = XMLParser::AppendNode(doc, stability, "TransXq");
        XMLParser::SetNodeValue(doc, transXq, data.transXq);
        auto transTd0 = XMLParser::AppendNode(doc, stability, "TransTd0");
        XMLParser::SetNodeValue(doc, transTd0, data.transTd0);
        auto transTq0 = XMLParser::AppendNode(doc, stability, "TransTq0");
        XMLParser::SetNodeValue(doc, transTq0, data.transTq0);
        auto subXd = XMLParser::AppendNode(doc, stability, "SubXd");
        XMLParser::SetNodeValue(doc, subXd, data.subXd);
        auto subXq = XMLParser::AppendNode(doc, stability, "SubXq");
        XMLParser::SetNodeValue(doc, subXq, data.subXq);
        auto subTd0 = XMLParser::AppendNode(doc, stability, "SubTd0");
        XMLParser::SetNodeValue(doc, subTd0, data.subTd0);
        auto subTq0 = XMLParser::AppendNode(doc, stability, "SubTq0");
        XMLParser::SetNodeValue(doc, subTq0, data.subTq0);

        auto avr = XMLParser::AppendNode(doc, stability, "AVR");
        if(data.avr) SaveControlElements(doc, avr, data.avr);

        auto speedGov = XMLParser::AppendNode(doc, stability, "SpeedGovernor");
        if(data.speedGov) SaveControlElements(doc, speedGov, data.speedGov);

        auto switchingList = XMLParser::AppendNode(doc, electricalProp, "SwitchingList");
        SwitchingData swData = syncGenerator->GetSwitchingData();
        for(int j = 0; j < (int)swData.swType.size(); j++) {
            auto switching = XMLParser::AppendNode(doc, switchingList, "Switching");
            XMLParser::SetNodeAttribute(doc, switching, "ID", j);
            auto swType = XMLParser::AppendNode(doc, switching, "Type");
            XMLParser::SetNodeValue(doc, swType, swData.swType[j]);
            auto swTime = XMLParser::AppendNode(doc, switching, "Time");
            XMLParser::SetNodeValue(doc, swTime, swData.swTime[j]);
        }
    }  //}

    //{ SyncMotor
    auto syncMotorsNode = XMLParser::AppendNode(doc, elementsNode, "SyncMotorList");
    auto syncMotorList = allElements.GetSyncMotorList();
    for(int i = 0; i < (int)syncMotorList.size(); i++) {
        SyncMotor* syncMotor = syncMotorList[i];
        auto syncMotorNode = XMLParser::AppendNode(doc, syncMotorsNode, "SyncMotor");
        XMLParser::SetNodeAttribute(doc, syncMotorNode, "ID", i);
        auto cadProp = XMLParser::AppendNode(doc, syncMotorNode, "CADProperties");
        auto position = XMLParser::AppendNode(doc, cadProp, "Position");
        auto posX = XMLParser::AppendNode(doc, position, "X");
        XMLParser::SetNodeValue(doc, posX, syncMotor->GetPosition().m_x);
        auto posY = XMLParser::AppendNode(doc, position, "Y");
        XMLParser::SetNodeValue(doc, posY, syncMotor->GetPosition().m_y);
        auto size = XMLParser::AppendNode(doc, cadProp, "Size");
        auto width = XMLParser::AppendNode(doc, size, "Width");
        XMLParser::SetNodeValue(doc, width, syncMotor->GetWidth());
        auto height = XMLParser::AppendNode(doc, size, "Height");
        XMLParser::SetNodeValue(doc, height, syncMotor->GetHeight());
        auto angle = XMLParser::AppendNode(doc, cadProp, "Angle");
        XMLParser::SetNodeValue(doc, angle, syncMotor->GetAngle());
        auto nodePos = XMLParser::AppendNode(doc, cadProp, "NodePosition");
        auto nodePosX = XMLParser::AppendNode(doc, nodePos, "X");
        XMLParser::SetNodeValue(doc, nodePosX, syncMotor->GetPointList()[0].m_x);
        auto nodePosY = XMLParser::AppendNode(doc, nodePos, "Y");
        XMLParser::SetNodeValue(doc, nodePosY, syncMotor->GetPointList()[0].m_y);
        auto parentID = XMLParser::AppendNode(doc, cadProp, "ParentID");
        Bus* parent = static_cast<Bus*>(syncMotor->GetParentList()[0]);
        if(parent) XMLParser::SetNodeValue(doc, parentID, parent->GetElectricalData().number);

        SyncMotorElectricalData data = syncMotor->GetElectricalData();
        auto electricalProp = XMLParser::AppendNode(doc, syncMotorNode, "ElectricalProperties");
        auto isOnline = XMLParser::AppendNode(doc, electricalProp, "IsOnline");
        XMLParser::SetNodeValue(doc, isOnline, syncMotor->IsOnline());
        auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
        XMLParser::SetNodeValue(doc, name, data.name);
        auto nominalPower = XMLParser::AppendNode(doc, electricalProp, "NominalPower");
        XMLParser::SetNodeValue(doc, nominalPower, data.nominalPower);
        XMLParser::SetNodeAttribute(doc, nominalPower, "UnitID", data.nominalPowerUnit);
        // auto nominalVoltage = XMLParser::AppendNode(doc, electricalProp, "NominalVoltage");
        // XMLParser::SetNodeValue(doc, nominalVoltage, data.nominalVoltage);
        // XMLParser::SetNodeAttribute(doc, nominalVoltage, "UnitID", data.nominalVoltageUnit);
        auto activePower = XMLParser::AppendNode(doc, electricalProp, "ActivePower");
        XMLParser::SetNodeValue(doc, activePower, data.activePower);
        XMLParser::SetNodeAttribute(doc, activePower, "UnitID", data.activePowerUnit);
        auto reactivePower = XMLParser::AppendNode(doc, electricalProp, "ReactivePower");
        XMLParser::SetNodeValue(doc, reactivePower, data.reactivePower);
        XMLParser::SetNodeAttribute(doc, reactivePower, "UnitID", data.reactivePowerUnit);
        auto haveMaxReactive = XMLParser::AppendNode(doc, electricalProp, "HaveMaxReactive");
        XMLParser::SetNodeValue(doc, haveMaxReactive, data.haveMaxReactive);
        auto maxReactive = XMLParser::AppendNode(doc, electricalProp, "MaxReactive");
        XMLParser::SetNodeValue(doc, maxReactive, data.maxReactive);
        XMLParser::SetNodeAttribute(doc, maxReactive, "UnitID", data.maxReactiveUnit);
        auto haveMinReactive = XMLParser::AppendNode(doc, electricalProp, "HaveMinReactive");
        XMLParser::SetNodeValue(doc, haveMinReactive, data.haveMinReactive);
        auto minReactive = XMLParser::AppendNode(doc, electricalProp, "MinReactive");
        XMLParser::SetNodeValue(doc, minReactive, data.minReactive);
        XMLParser::SetNodeAttribute(doc, minReactive, "UnitID", data.minReactiveUnit);
        auto useMachineBase = XMLParser::AppendNode(doc, electricalProp, "UseMachineBase");
        XMLParser::SetNodeValue(doc, useMachineBase, data.useMachineBase);

        auto fault = XMLParser::AppendNode(doc, electricalProp, "Fault");
        auto positiveResistance = XMLParser::AppendNode(doc, fault, "PositiveResistance");
        XMLParser::SetNodeValue(doc, positiveResistance, data.positiveResistance);
        auto positiveReactance = XMLParser::AppendNode(doc, fault, "PositiveReactance");
        XMLParser::SetNodeValue(doc, positiveReactance, data.positiveReactance);
        auto negativeResistance = XMLParser::AppendNode(doc, fault, "NegativeResistance");
        XMLParser::SetNodeValue(doc, negativeResistance, data.negativeResistance);
        auto negativeReactance = XMLParser::AppendNode(doc, fault, "NegativeReactance");
        XMLParser::SetNodeValue(doc, negativeReactance, data.negativeReactance);
        auto zeroResistance = XMLParser::AppendNode(doc, fault, "ZeroResistance");
        XMLParser::SetNodeValue(doc, zeroResistance, data.zeroResistance);
        auto zeroReactance = XMLParser::AppendNode(doc, fault, "ZeroReactance");
        XMLParser::SetNodeValue(doc, zeroReactance, data.zeroReactance);
        auto groundResistance = XMLParser::AppendNode(doc, fault, "GroundResistance");
        XMLParser::SetNodeValue(doc, groundResistance, data.groundResistance);
        auto groundReactance = XMLParser::AppendNode(doc, fault, "GroundReactance");
        XMLParser::SetNodeValue(doc, groundReactance, data.groundReactance);
        auto groundNeutral = XMLParser::AppendNode(doc, fault, "GroundNeutral");
        XMLParser::SetNodeValue(doc, groundNeutral, data.groundNeutral);

        // To future use...
        /*auto stability = XMLParser::AppendNode(doc, electricalProp, "Stability");
        auto plotSyncMachine = XMLParser::AppendNode(doc, stability, "PlotSyncMotor");
        XMLParser::SetNodeValue(doc, plotSyncMachine, data.plotSyncMachine);
        auto inertia = XMLParser::AppendNode(doc, stability, "Inertia");
        XMLParser::SetNodeValue(doc, inertia, data.inertia);
        auto damping = XMLParser::AppendNode(doc, stability, "Damping");
        XMLParser::SetNodeValue(doc, damping, data.damping);
        auto useAVR = XMLParser::AppendNode(doc, stability, "UseAVR");
        XMLParser::SetNodeValue(doc, useAVR, data.useAVR);
        auto armResistance = XMLParser::AppendNode(doc, stability, "ArmResistance");
        XMLParser::SetNodeValue(doc, armResistance, data.armResistance);
        auto potierReactance = XMLParser::AppendNode(doc, stability, "PotierReactance");
        XMLParser::SetNodeValue(doc, potierReactance, data.potierReactance);
        auto satFactor = XMLParser::AppendNode(doc, stability, "SatFactor");
        XMLParser::SetNodeValue(doc, satFactor, data.satFactor);
        auto syncXd = XMLParser::AppendNode(doc, stability, "SyncXd");
        XMLParser::SetNodeValue(doc, syncXd, data.syncXd);
        auto syncXq = XMLParser::AppendNode(doc, stability, "SyncXq");
        XMLParser::SetNodeValue(doc, syncXq, data.syncXq);
        auto transXd = XMLParser::AppendNode(doc, stability, "TransXd");
        XMLParser::SetNodeValue(doc, transXd, data.transXd);
        auto transXq = XMLParser::AppendNode(doc, stability, "TransXq");
        XMLParser::SetNodeValue(doc, transXq, data.transXq);
        auto transTd0 = XMLParser::AppendNode(doc, stability, "TransTd0");
        XMLParser::SetNodeValue(doc, transTd0, data.transTd0);
        auto transTq0 = XMLParser::AppendNode(doc, stability, "TransTq0");
        XMLParser::SetNodeValue(doc, transTq0, data.transTq0);
        auto subXd = XMLParser::AppendNode(doc, stability, "SubXd");
        XMLParser::SetNodeValue(doc, subXd, data.subXd);
        auto subXq = XMLParser::AppendNode(doc, stability, "SubXq");
        XMLParser::SetNodeValue(doc, subXq, data.subXq);
        auto subTd0 = XMLParser::AppendNode(doc, stability, "SubTd0");
        XMLParser::SetNodeValue(doc, subTd0, data.subTd0);
        auto subTq0 = XMLParser::AppendNode(doc, stability, "SubTq0");
        XMLParser::SetNodeValue(doc, subTq0, data.subTq0);

        auto switchingList = XMLParser::AppendNode(doc, electricalProp, "SwitchingList");
        SwitchingData swData = syncGenerator->GetSwitchingData();
        for(int j = 0; j < (int)swData.swType.size(); j++) {
            auto switching = XMLParser::AppendNode(doc, switchingList, "Switching");
            XMLParser::SetNodeAttribute(doc, switching, "ID", j);
            auto swType = XMLParser::AppendNode(doc, switching, "Type");
            XMLParser::SetNodeValue(doc, swType, swData.swType[j]);
            auto swTime = XMLParser::AppendNode(doc, switching, "Time");
            XMLParser::SetNodeValue(doc, swTime, swData.swTime[j]);
        }*/
    }  //}

    //{ Transfomer
    auto transformersNode = XMLParser::AppendNode(doc, elementsNode, "TransformerList");
    auto transformerList = allElements.GetTransformerList();
    for(int i = 0; i < (int)transformerList.size(); i++) {
        Transformer* transfomer = transformerList[i];
        auto transformerNode = XMLParser::AppendNode(doc, transformersNode, "Transfomer");
        XMLParser::SetNodeAttribute(doc, transformerNode, "ID", i);
        auto cadProp = XMLParser::AppendNode(doc, transformerNode, "CADProperties");
        auto position = XMLParser::AppendNode(doc, cadProp, "Position");
        auto posX = XMLParser::AppendNode(doc, position, "X");
        XMLParser::SetNodeValue(doc, posX, transfomer->GetPosition().m_x);
        auto posY = XMLParser::AppendNode(doc, position, "Y");
        XMLParser::SetNodeValue(doc, posY, transfomer->GetPosition().m_y);
        auto size = XMLParser::AppendNode(doc, cadProp, "Size");
        auto width = XMLParser::AppendNode(doc, size, "Width");
        XMLParser::SetNodeValue(doc, width, transfomer->GetWidth());
        auto height = XMLParser::AppendNode(doc, size, "Height");
        XMLParser::SetNodeValue(doc, height, transfomer->GetHeight());
        auto angle = XMLParser::AppendNode(doc, cadProp, "Angle");
        XMLParser::SetNodeValue(doc, angle, transfomer->GetAngle());
        auto nodeList = XMLParser::AppendNode(doc, cadProp, "NodeList");
        auto nodePos1 = XMLParser::AppendNode(doc, nodeList, "Node");
        XMLParser::SetNodeAttribute(doc, nodePos1, "ID", 0);
        auto nodePosX1 = XMLParser::AppendNode(doc, nodePos1, "X");
        XMLParser::SetNodeValue(doc, nodePosX1, transfomer->GetPointList()[0].m_x);
        auto nodePosY1 = XMLParser::AppendNode(doc, nodePos1, "Y");
        XMLParser::SetNodeValue(doc, nodePosY1, transfomer->GetPointList()[0].m_y);
        auto nodePos2 = XMLParser::AppendNode(doc, nodeList, "Node");
        XMLParser::SetNodeAttribute(doc, nodePos2, "ID", 1);
        auto nodePosX2 = XMLParser::AppendNode(doc, nodePos2, "X");
        XMLParser::SetNodeValue(doc, nodePosX2, transfomer->GetPointList()[transfomer->GetPointList().size() - 1].m_x);
        auto nodePosY2 = XMLParser::AppendNode(doc, nodePos2, "Y");
        XMLParser::SetNodeValue(doc, nodePosY2, transfomer->GetPointList()[transfomer->GetPointList().size() - 1].m_y);

        auto parentIDList = XMLParser::AppendNode(doc, cadProp, "ParentIDList");
        for(int j = 0; j < (int)transfomer->GetParentList().size(); j++) {
            Bus* parent = static_cast<Bus*>(transfomer->GetParentList()[j]);
            if(parent) {
                auto parentID = XMLParser::AppendNode(doc, parentIDList, "ParentID");
                XMLParser::SetNodeAttribute(doc, parentID, "ID", j);
                XMLParser::SetNodeValue(doc, parentID, parent->GetElectricalData().number);
            }
        }

        TransformerElectricalData data = transfomer->GetElectricalData();
        auto electricalProp = XMLParser::AppendNode(doc, transformerNode, "ElectricalProperties");
        auto isOnline = XMLParser::AppendNode(doc, electricalProp, "IsOnline");
        XMLParser::SetNodeValue(doc, isOnline, transfomer->IsOnline());
        auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
        XMLParser::SetNodeValue(doc, name, data.name);
        auto primaryNominalVoltage = XMLParser::AppendNode(doc, electricalProp, "PrimaryNominalVoltage");
        XMLParser::SetNodeValue(doc, primaryNominalVoltage, data.primaryNominalVoltage);
        XMLParser::SetNodeAttribute(doc, primaryNominalVoltage, "UnitID", data.primaryNominalVoltageUnit);
        auto secondaryNominalVoltage = XMLParser::AppendNode(doc, electricalProp, "SecondaryNominalVoltage");
        XMLParser::SetNodeValue(doc, secondaryNominalVoltage, data.secondaryNominalVoltage);
        XMLParser::SetNodeAttribute(doc, secondaryNominalVoltage, "UnitID", data.secondaryNominalVoltageUnit);
        auto nominalPower = XMLParser::AppendNode(doc, electricalProp, "NominalPower");
        XMLParser::SetNodeValue(doc, nominalPower, data.nominalPower);
        XMLParser::SetNodeAttribute(doc, nominalPower, "UnitID", data.nominalPowerUnit);
        auto resistance = XMLParser::AppendNode(doc, electricalProp, "Resistance");
        XMLParser::SetNodeValue(doc, resistance, data.resistance);
        XMLParser::SetNodeAttribute(doc, resistance, "UnitID", data.resistanceUnit);
        auto indReactance = XMLParser::AppendNode(doc, electricalProp, "IndReactance");
        XMLParser::SetNodeValue(doc, indReactance, data.indReactance);
        XMLParser::SetNodeAttribute(doc, indReactance, "UnitID", data.indReactanceUnit);
        auto connection = XMLParser::AppendNode(doc, electricalProp, "Connection");
        XMLParser::SetNodeValue(doc, connection, data.connection);
        auto turnsRatio = XMLParser::AppendNode(doc, electricalProp, "TurnsRatio");
        XMLParser::SetNodeValue(doc, turnsRatio, data.turnsRatio);
        auto phaseShift = XMLParser::AppendNode(doc, electricalProp, "PhaseShift");
        XMLParser::SetNodeValue(doc, phaseShift, data.phaseShift);
        auto useTransformerPower = XMLParser::AppendNode(doc, electricalProp, "UseTransfomerPower");
        XMLParser::SetNodeValue(doc, useTransformerPower, data.useTransformerPower);

        auto fault = XMLParser::AppendNode(doc, electricalProp, "Fault");
        auto zeroResistance = XMLParser::AppendNode(doc, fault, "ZeroResistance");
        XMLParser::SetNodeValue(doc, zeroResistance, data.zeroResistance);
        auto zeroIndReactance = XMLParser::AppendNode(doc, fault, "ZeroIndReactance");
        XMLParser::SetNodeValue(doc, zeroIndReactance, data.zeroIndReactance);
        auto primaryGrndResistance = XMLParser::AppendNode(doc, fault, "PrimaryGrndResistance");
        XMLParser::SetNodeValue(doc, primaryGrndResistance, data.primaryGrndResistance);
        auto primaryGrndReactance = XMLParser::AppendNode(doc, fault, "PrimaryGrndReactance");
        XMLParser::SetNodeValue(doc, primaryGrndReactance, data.primaryGrndReactance);
        auto secondaryGrndResistance = XMLParser::AppendNode(doc, fault, "SecondaryGrndResistance");
        XMLParser::SetNodeValue(doc, secondaryGrndResistance, data.secondaryGrndResistance);
        auto secondaryGrndReactance = XMLParser::AppendNode(doc, fault, "SecondaryGrndReactance");
        XMLParser::SetNodeValue(doc, secondaryGrndReactance, data.secondaryGrndReactance);

        auto switchingList = XMLParser::AppendNode(doc, electricalProp, "SwitchingList");
        SwitchingData swData = transfomer->GetSwitchingData();
        for(int j = 0; j < (int)swData.swType.size(); j++) {
            auto switching = XMLParser::AppendNode(doc, switchingList, "Switching");
            XMLParser::SetNodeAttribute(doc, switching, "ID", j);
            auto swType = XMLParser::AppendNode(doc, switching, "Type");
            XMLParser::SetNodeValue(doc, swType, swData.swType[j]);
            auto swTime = XMLParser::AppendNode(doc, switching, "Time");
            XMLParser::SetNodeValue(doc, swTime, swData.swTime[j]);
        }
    }  //}

    //{ Text
    auto textsNode = XMLParser::AppendNode(doc, elementsNode, "TextList");
    auto textList = m_workspace->GetTextList();
    for(int i = 0; i < (int)textList.size(); i++) {
        Text* text = textList[i];
        auto textNode = XMLParser::AppendNode(doc, textsNode, "Text");
        XMLParser::SetNodeAttribute(doc, textNode, "ID", i);
        auto cadProp = XMLParser::AppendNode(doc, textNode, "CADProperties");
        auto position = XMLParser::AppendNode(doc, cadProp, "Position");
        auto posX = XMLParser::AppendNode(doc, position, "X");
        XMLParser::SetNodeValue(doc, posX, text->GetPosition().m_x);
        auto posY = XMLParser::AppendNode(doc, position, "Y");
        XMLParser::SetNodeValue(doc, posY, text->GetPosition().m_y);
        auto size = XMLParser::AppendNode(doc, cadProp, "Size");
        auto width = XMLParser::AppendNode(doc, size, "Width");
        XMLParser::SetNodeValue(doc, width, text->GetWidth());
        auto height = XMLParser::AppendNode(doc, size, "Height");
        XMLParser::SetNodeValue(doc, height, text->GetHeight());
        auto angle = XMLParser::AppendNode(doc, cadProp, "Angle");
        XMLParser::SetNodeValue(doc, angle, text->GetAngle());
        auto textProperties = XMLParser::AppendNode(doc, textNode, "TextProperties");
        auto elementType = XMLParser::AppendNode(doc, textProperties, "ElementType");
        XMLParser::SetNodeValue(doc, elementType, text->GetElementType());
        auto elementNumber = XMLParser::AppendNode(doc, textProperties, "ElementNumber");
        XMLParser::SetNodeValue(doc, elementNumber, text->GetElementNumber());
        auto dataType = XMLParser::AppendNode(doc, textProperties, "DataType");
        XMLParser::SetNodeValue(doc, dataType, text->GetDataType());
        auto dataUnit = XMLParser::AppendNode(doc, textProperties, "DataUnit");
        XMLParser::SetNodeValue(doc, dataUnit, text->GetUnit());
        auto direction = XMLParser::AppendNode(doc, textProperties, "Direction");
        XMLParser::SetNodeValue(doc, direction, text->GetDirection());
        auto decimalPlaces = XMLParser::AppendNode(doc, textProperties, "DecimalPlaces");
        XMLParser::SetNodeValue(doc, decimalPlaces, text->GetDecimalPlaces());
    }
    //}

    std::ofstream writeXML(path.GetFullPath());
    writeXML << doc;
    writeXML.close();
}

bool FileHanding::OpenProject(wxFileName path)
{
    rapidxml::xml_document<> doc;
    rapidxml::file<> xmlFile(path.GetFullPath().mb_str());

    doc.parse<0>(xmlFile.data());

    auto projectNode = doc.first_node("Project");
    if(!projectNode) return false;
    auto nameNode = projectNode->first_node("Name");
    if(!nameNode) return false;
    m_workspace->SetName(nameNode->value());

    PropertiesData* propData = m_workspace->GetProperties();
    SimulationData simData = propData->GetSimulationPropertiesData();

    //{ Properties data
    auto propertiesNode = projectNode->first_node("Properties");
    if(propertiesNode) {
        auto simPropertiesNode = propertiesNode->first_node("SimulationProperties");
        if(simPropertiesNode) {
            // General
            auto general = simPropertiesNode->first_node("General");
            simData.basePower = XMLParser::GetNodeValueDouble(general, "BasePower");
            simData.basePowerUnit =
                static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(general, "BasePower", "UnitID"));
            auto contCalc = general->first_node("ContinuousCalculation");
            simData.faultAfterPowerFlow = XMLParser::GetNodeValueInt(contCalc, "Fault");
            simData.scPowerAfterPowerFlow = XMLParser::GetNodeValueInt(contCalc, "SCPower");

            // Power flow
            auto powerFlow = simPropertiesNode->first_node("PowerFlow");
            simData.powerFlowMethod =
                static_cast<PowerFlowMethod>(XMLParser::GetNodeValueInt(powerFlow, "SolutionMethod"));
            simData.accFator = XMLParser::GetNodeValueDouble(powerFlow, "AccFactor");
            simData.powerFlowTolerance = XMLParser::GetNodeValueDouble(powerFlow, "Tolerance");
            simData.powerFlowMaxIterations = XMLParser::GetNodeValueInt(powerFlow, "MaxIterations");

            // Stability
            auto stability = simPropertiesNode->first_node("Stability");
            simData.timeStep = XMLParser::GetNodeValueDouble(stability, "TimeStep");
            simData.stabilitySimulationTime = XMLParser::GetNodeValueDouble(stability, "SimulationTime");
            simData.stabilityFrequency = XMLParser::GetNodeValueDouble(stability, "Frequency");
            simData.stabilityTolerance = XMLParser::GetNodeValueDouble(stability, "Tolerance");
            simData.stabilityMaxIterations = XMLParser::GetNodeValueDouble(stability, "MaxIterations");
            simData.controlTimeStepRatio = XMLParser::GetNodeValueInt(stability, "ControlStepRatio");
            simData.plotTime = XMLParser::GetNodeValueDouble(stability, "PlotStep");
            simData.useCOI = XMLParser::GetNodeValueInt(stability, "UseCOI");

            // ZIP load
            auto compLoads = simPropertiesNode->first_node("ZIPLoad");
            simData.useCompLoads = XMLParser::GetNodeValueInt(compLoads, "UseCompositeLoad");
            auto activePowerComp = compLoads->first_node("ActivePowerComposition");
            simData.constImpedanceActive = XMLParser::GetNodeValueDouble(activePowerComp, "ConstantImpedance");
            simData.constCurrentActive = XMLParser::GetNodeValueDouble(activePowerComp, "ConstantCurrent");
            simData.constPowerActive = XMLParser::GetNodeValueDouble(activePowerComp, "ConstantPower");
            auto reactivePowerComp = compLoads->first_node("ReactivePowerComposition");
            simData.constImpedanceReactive = XMLParser::GetNodeValueDouble(reactivePowerComp, "ConstantImpedance");
            simData.constCurrentReactive = XMLParser::GetNodeValueDouble(reactivePowerComp, "ConstantCurrent");
            simData.constPowerReactive = XMLParser::GetNodeValueDouble(reactivePowerComp, "ConstantPower");
            auto uvLimit = compLoads->first_node("UndervoltageLimit");
            simData.underVoltageConstCurrent = XMLParser::GetNodeValueDouble(uvLimit, "ConstantCurrent");
            simData.underVoltageConstPower = XMLParser::GetNodeValueDouble(uvLimit, "ConstantPower");
        }
    }
    //}

    propData->SetSimulationPropertiesData(simData);

    // Open elements
    auto elementsNode = projectNode->first_node("Elements");
    if(!elementsNode) return false;
    std::vector<Element*> elementList;
    // Save lists individually to get parents
    std::vector<Bus*> busList;
    std::vector<Capacitor*> capacitorList;
    std::vector<IndMotor*> indMotorList;
    std::vector<Inductor*> inductorList;
    std::vector<Line*> lineList;
    std::vector<Load*> loadList;
    std::vector<SyncGenerator*> syncGeneratorList;
    std::vector<SyncMotor*> syncMotorList;
    std::vector<Transformer*> transformerList;
    std::vector<Text*> textList;
    
    // List of parents
    std::vector<Element*> parentList;

    //{ Bus
    auto busListNode = elementsNode->first_node("BusList");
    if(!busListNode) return false;
    auto busNode = busListNode->first_node("Bus");
    while(busNode) {
        Bus* bus = new Bus();
        
        if(!bus->OpenElement(busNode)) return false;
        elementList.push_back(bus);
        busList.push_back(bus);
        parentList.push_back(bus);
        
        busNode = busNode->next_sibling("Bus");
    }  //}

    //{ Capacitor
    auto capacitorListNode = elementsNode->first_node("CapacitorList");
    if(!capacitorListNode) return false;
    auto capacitorNode = capacitorListNode->first_node("Capacitor");
    while(capacitorNode) {
        Capacitor* capacitor = new Capacitor();
        
        if(!capacitor->OpenElement(capacitorNode, parentList)) return false;
        elementList.push_back(capacitor);
        capacitorList.push_back(capacitor);

        capacitorNode = capacitorNode->next_sibling("Capacitor");
    }  //}

    //{ IndMotor
    auto indMotorListNode = elementsNode->first_node("IndMotorList");
    if(!indMotorListNode) return false;
    auto indMotorNode = indMotorListNode->first_node("IndMotor");
    while(indMotorNode) {
        IndMotor* indMotor = new IndMotor();
        
        if(!indMotor->OpenElement(indMotorNode, parentList)) return false;
        elementList.push_back(indMotor);
        indMotorList.push_back(indMotor);
        
        indMotorNode = indMotorNode->next_sibling("IndMotor");
    }  //}

    //{ Inductor
    auto inductorListNode = elementsNode->first_node("InductorList");
    if(!inductorListNode) return false;
    auto inductorNode = inductorListNode->first_node("Inductor");
    while(inductorNode) {
        Inductor* inductor = new Inductor();

        auto cadPropNode = inductorNode->first_node("CADProperties");
        if(!cadPropNode) return false;

        auto position = cadPropNode->first_node("Position");
        double posX = XMLParser::GetNodeValueDouble(position, "X");
        double posY = XMLParser::GetNodeValueDouble(position, "Y");
        auto size = cadPropNode->first_node("Size");
        double width = XMLParser::GetNodeValueDouble(size, "Width");
        double height = XMLParser::GetNodeValueDouble(size, "Height");
        double angle = XMLParser::GetNodeValueDouble(cadPropNode, "Angle");
        auto nodePosition = cadPropNode->first_node("NodePosition");
        double nodePosX = XMLParser::GetNodeValueDouble(nodePosition, "X");
        double nodePosY = XMLParser::GetNodeValueDouble(nodePosition, "Y");
        int parentID = XMLParser::GetNodeValueInt(cadPropNode, "ParentID");
        if(parentID == -1) {
            // If the element has no parent, create a temporary one, remove and delete.
            Bus* parent = new Bus(wxPoint2DDouble(nodePosX, nodePosY));
            inductor->AddParent(parent, wxPoint2DDouble(nodePosX, nodePosY));
            inductor->StartMove(inductor->GetPosition());
            inductor->Move(wxPoint2DDouble(posX, posY));
            inductor->RemoveParent(parent);
            delete parent;
        } else {
            Bus* parent = busList[parentID];
            inductor->AddParent(parent, wxPoint2DDouble(nodePosX, nodePosY));
            inductor->StartMove(inductor->GetPosition());
            inductor->Move(wxPoint2DDouble(posX, posY));
        }
        inductor->SetWidth(width);
        inductor->SetHeight(height);

        int numRot = angle / inductor->GetRotationAngle();
        bool clockwise = true;
        if(numRot < 0) {
            numRot = std::abs(numRot);
            clockwise = false;
        }
        for(int i = 0; i < numRot; i++) inductor->Rotate(clockwise);

        auto electricalProp = inductorNode->first_node("ElectricalProperties");
        if(!electricalProp) return false;

        inductor->SetOnline(XMLParser::GetNodeValueInt(electricalProp, "IsOnline"));
        InductorElectricalData data = inductor->GetElectricalData();
        data.name = electricalProp->first_node("Name")->value();
        data.reactivePower = XMLParser::GetNodeValueDouble(electricalProp, "ReactivePower");
        data.reactivePowerUnit =
            (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "ReactivePower", "UnitID");

        SwitchingData swData;
        auto switchingList = electricalProp->first_node("SwitchingList");
        if(!switchingList) return false;
        auto swNode = switchingList->first_node("Switching");
        while(swNode) {
            swData.swType.push_back((SwitchingType)XMLParser::GetNodeValueInt(swNode, "Type"));
            swData.swTime.push_back(XMLParser::GetNodeValueDouble(swNode, "Time"));
            swNode = swNode->next_sibling("Switching");
        }
        inductor->SetSwitchingData(swData);

        inductor->SetElectricalData(data);

        if(swData.swTime.size() != 0) inductor->SetDynamicEvent(true);

        elementList.push_back(inductor);
        inductorList.push_back(inductor);
        inductorNode = inductorNode->next_sibling("Inductor");
    }  //}

    //{ Line
    auto lineListNode = elementsNode->first_node("LineList");
    if(!lineListNode) return false;
    auto lineNode = lineListNode->first_node("Line");
    while(lineNode) {
        Line* line = new Line();

        auto cadPropNode = lineNode->first_node("CADProperties");
        if(!cadPropNode) return false;

        // Get nodes points
        std::vector<wxPoint2DDouble> ptsList;
        auto nodePosList = cadPropNode->first_node("NodeList");
        if(!nodePosList) return false;
        auto nodePos = nodePosList->first_node("Node");
        while(nodePos) {
            double nodePosX = XMLParser::GetNodeValueDouble(nodePos, "X");
            double nodePosY = XMLParser::GetNodeValueDouble(nodePos, "Y");
            ptsList.push_back(wxPoint2DDouble(nodePosX, nodePosY));
            nodePos = nodePos->next_sibling("Node");
        }

        // Get parents IDs
        auto parentIDList = cadPropNode->first_node("ParentIDList");
        if(!parentIDList) return false;
        auto parentNode = parentIDList->first_node("ParentID");
        long parentID[2] = {-1, -1};
        while(parentNode) {
            long index = 0;
            wxString(parentNode->first_attribute("ID")->value()).ToLong(&index);
            wxString(parentNode->value()).ToCLong(&parentID[index]);
            parentNode = parentNode->next_sibling("ParentID");
        }

        std::vector<wxPoint2DDouble> nodePtsList;            // List of node points
        nodePtsList.push_back(ptsList[0]);                   // First point on the list
        nodePtsList.push_back(ptsList[ptsList.size() - 1]);  // Last point on the list

        // List of dummy buses to set not connected nodes properly
        std::vector<Bus*> dummyBusList;
        for(unsigned int i = 0; i < nodePtsList.size(); ++i) {
            if(parentID[i] == -1)  // No parent connected
            {
                Bus* dummyBus = new Bus(nodePtsList[i]);
                dummyBusList.push_back(dummyBus);
                line->AddParent(dummyBus, nodePtsList[i]);
            } else {  // Parent connected (necessarily a bus, get from bus list)
                line->AddParent(busList[parentID[i]], nodePtsList[i]);
            }
        }

        // Add the others nodes (if exists)
        std::vector<wxPoint2DDouble> midPts;
        for(unsigned int i = 1; i < ptsList.size() - 1; i++) midPts.push_back(ptsList[i]);
        std::vector<wxPoint2DDouble> edgesPts = line->GetPointList();
        edgesPts.insert(edgesPts.begin() + 2, midPts.begin(), midPts.end());
        line->SetPointList(edgesPts);

        // Remove dummy buses
        for(auto it = dummyBusList.begin(), itEnd = dummyBusList.end(); it != itEnd; ++it) {
            line->RemoveParent(*it);
            delete *it;
        }
        dummyBusList.clear();

        auto electricalProp = lineNode->first_node("ElectricalProperties");
        if(!electricalProp) return false;

        line->SetOnline(XMLParser::GetNodeValueInt(electricalProp, "IsOnline"));
        LineElectricalData data = line->GetElectricalData();
        data.name = electricalProp->first_node("Name")->value();
        data.nominalVoltage = XMLParser::GetNodeValueDouble(electricalProp, "NominalVoltage");
        data.nominalVoltageUnit =
            (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "NominalVoltage", "UnitID");
        data.nominalPower = XMLParser::GetNodeValueDouble(electricalProp, "NominalPower");
        data.nominalPowerUnit =
            (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "NominalPower", "UnitID");
        data.resistance = XMLParser::GetNodeValueDouble(electricalProp, "Resistance");
        data.resistanceUnit = (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "Resistance", "UnitID");
        data.indReactance = XMLParser::GetNodeValueDouble(electricalProp, "IndReactance");
        data.indReactanceUnit =
            (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "IndReactance", "UnitID");
        data.capSusceptance = XMLParser::GetNodeValueDouble(electricalProp, "CapSusceptance");
        data.capSusceptanceUnit =
            (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "CapSusceptance", "UnitID");
        data.lineSize = XMLParser::GetNodeValueDouble(electricalProp, "LineSize");
        data.useLinePower = XMLParser::GetNodeValueInt(electricalProp, "UseLinePower");

        auto fault = electricalProp->first_node("Fault");
        data.zeroResistance = XMLParser::GetNodeValueDouble(fault, "ZeroResistance");
        data.zeroIndReactance = XMLParser::GetNodeValueDouble(fault, "ZeroIndReactance");
        data.zeroCapSusceptance = XMLParser::GetNodeValueDouble(fault, "ZeroCapSusceptance");

        SwitchingData swData;
        auto switchingList = electricalProp->first_node("SwitchingList");
        if(!switchingList) return false;
        auto swNode = switchingList->first_node("Switching");
        while(swNode) {
            swData.swType.push_back((SwitchingType)XMLParser::GetNodeValueInt(swNode, "Type"));
            swData.swTime.push_back(XMLParser::GetNodeValueDouble(swNode, "Time"));
            swNode = swNode->next_sibling("Switching");
        }
        line->SetSwitchingData(swData);

        line->SetElectricalData(data);

        if(swData.swTime.size() != 0) line->SetDynamicEvent(true);

        elementList.push_back(line);
        lineList.push_back(line);
        lineNode = lineNode->next_sibling("Line");
    }  //}

    //{ Load
    auto loadListNode = elementsNode->first_node("LoadList");
    if(!loadListNode) return false;
    auto loadNode = loadListNode->first_node("Load");
    while(loadNode) {
        Load* load = new Load();

        auto cadPropNode = loadNode->first_node("CADProperties");
        if(!cadPropNode) return false;

        auto position = cadPropNode->first_node("Position");
        double posX = XMLParser::GetNodeValueDouble(position, "X");
        double posY = XMLParser::GetNodeValueDouble(position, "Y");
        auto size = cadPropNode->first_node("Size");
        double width = XMLParser::GetNodeValueDouble(size, "Width");
        double height = XMLParser::GetNodeValueDouble(size, "Height");
        double angle = XMLParser::GetNodeValueDouble(cadPropNode, "Angle");
        auto nodePosition = cadPropNode->first_node("NodePosition");
        double nodePosX = XMLParser::GetNodeValueDouble(nodePosition, "X");
        double nodePosY = XMLParser::GetNodeValueDouble(nodePosition, "Y");
        int parentID = XMLParser::GetNodeValueInt(cadPropNode, "ParentID");
        if(parentID == -1) {
            // If the element has no parent, create a temporary one, remove and delete.
            Bus* parent = new Bus(wxPoint2DDouble(nodePosX, nodePosY));
            load->AddParent(parent, wxPoint2DDouble(nodePosX, nodePosY));
            load->StartMove(load->GetPosition());
            load->Move(wxPoint2DDouble(posX, posY));
            load->RemoveParent(parent);
            delete parent;
        } else {
            Bus* parent = busList[parentID];
            load->AddParent(parent, wxPoint2DDouble(nodePosX, nodePosY));
            load->StartMove(load->GetPosition());
            load->Move(wxPoint2DDouble(posX, posY));
        }
        load->SetWidth(width);
        load->SetHeight(height);

        int numRot = angle / load->GetRotationAngle();
        bool clockwise = true;
        if(numRot < 0) {
            numRot = std::abs(numRot);
            clockwise = false;
        }
        for(int i = 0; i < numRot; i++) load->Rotate(clockwise);

        auto electricalProp = loadNode->first_node("ElectricalProperties");
        if(!electricalProp) return false;

        load->SetOnline(XMLParser::GetNodeValueInt(electricalProp, "IsOnline"));
        LoadElectricalData data = load->GetElectricalData();
        data.name = electricalProp->first_node("Name")->value();
        data.activePower = XMLParser::GetNodeValueDouble(electricalProp, "ActivePower");
        data.activePowerUnit = (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "ActivePower", "UnitID");
        data.reactivePower = XMLParser::GetNodeValueDouble(electricalProp, "ReactivePower");
        data.reactivePowerUnit =
            (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "ReactivePower", "UnitID");
        data.loadType = (LoadType)XMLParser::GetNodeValueInt(electricalProp, "LoadType");
        // Stability
        auto stability = electricalProp->first_node("Stability");
        if(stability) {
            data.plotLoad = XMLParser::GetNodeValueInt(stability, "PlotLoad");
            data.useCompLoad = XMLParser::GetNodeValueInt(stability, "UseCompositeLoad");
            auto activePowerComp = stability->first_node("ActivePowerComposition");
            data.constImpedanceActive = XMLParser::GetNodeValueDouble(activePowerComp, "ConstantImpedance");
            data.constCurrentActive = XMLParser::GetNodeValueDouble(activePowerComp, "ConstantCurrent");
            data.constPowerActive = XMLParser::GetNodeValueDouble(activePowerComp, "ConstantPower");
            auto reactivePowerComp = stability->first_node("ReactivePowerComposition");
            data.constImpedanceReactive = XMLParser::GetNodeValueDouble(reactivePowerComp, "ConstantImpedance");
            data.constCurrentReactive = XMLParser::GetNodeValueDouble(reactivePowerComp, "ConstantCurrent");
            data.constPowerReactive = XMLParser::GetNodeValueDouble(reactivePowerComp, "ConstantPower");
        }

        SwitchingData swData;
        auto switchingList = electricalProp->first_node("SwitchingList");
        if(!switchingList) return false;
        auto swNode = switchingList->first_node("Switching");
        while(swNode) {
            swData.swType.push_back((SwitchingType)XMLParser::GetNodeValueInt(swNode, "Type"));
            swData.swTime.push_back(XMLParser::GetNodeValueDouble(swNode, "Time"));
            swNode = swNode->next_sibling("Switching");
        }
        load->SetSwitchingData(swData);

        load->SetElectricalData(data);

        if(swData.swTime.size() != 0) load->SetDynamicEvent(true);

        elementList.push_back(load);
        loadList.push_back(load);
        loadNode = loadNode->next_sibling("Load");
    }  //}

    //{ SyncGenerator
    auto syncGeneratorListNode = elementsNode->first_node("SyncGeneratorList");
    if(!syncGeneratorListNode) return false;
    auto syncGeneratorNode = syncGeneratorListNode->first_node("SyncGenerator");
    while(syncGeneratorNode) {
        SyncGenerator* syncGenerator = new SyncGenerator();

        auto cadPropNode = syncGeneratorNode->first_node("CADProperties");
        if(!cadPropNode) return false;

        auto position = cadPropNode->first_node("Position");
        double posX = XMLParser::GetNodeValueDouble(position, "X");
        double posY = XMLParser::GetNodeValueDouble(position, "Y");
        auto size = cadPropNode->first_node("Size");
        double width = XMLParser::GetNodeValueDouble(size, "Width");
        double height = XMLParser::GetNodeValueDouble(size, "Height");
        double angle = XMLParser::GetNodeValueDouble(cadPropNode, "Angle");
        auto nodePosition = cadPropNode->first_node("NodePosition");
        double nodePosX = XMLParser::GetNodeValueDouble(nodePosition, "X");
        double nodePosY = XMLParser::GetNodeValueDouble(nodePosition, "Y");
        int parentID = XMLParser::GetNodeValueInt(cadPropNode, "ParentID");
        if(parentID == -1) {
            // If the element has no parent, create a temporary one, remove and delete.
            Bus* parent = new Bus(wxPoint2DDouble(nodePosX, nodePosY));
            syncGenerator->AddParent(parent, wxPoint2DDouble(nodePosX, nodePosY));
            syncGenerator->StartMove(syncGenerator->GetPosition());
            syncGenerator->Move(wxPoint2DDouble(posX, posY));
            syncGenerator->RemoveParent(parent);
            delete parent;
        } else {
            Bus* parent = busList[parentID];
            syncGenerator->AddParent(parent, wxPoint2DDouble(nodePosX, nodePosY));
            syncGenerator->StartMove(syncGenerator->GetPosition());
            syncGenerator->Move(wxPoint2DDouble(posX, posY));
        }
        syncGenerator->SetWidth(width);
        syncGenerator->SetHeight(height);

        int numRot = angle / syncGenerator->GetRotationAngle();
        bool clockwise = true;
        if(numRot < 0) {
            numRot = std::abs(numRot);
            clockwise = false;
        }
        for(int i = 0; i < numRot; i++) syncGenerator->Rotate(clockwise);

        auto electricalProp = syncGeneratorNode->first_node("ElectricalProperties");
        if(!electricalProp) return false;

        syncGenerator->SetOnline(XMLParser::GetNodeValueInt(electricalProp, "IsOnline"));
        SyncGeneratorElectricalData data = syncGenerator->GetElectricalData();
        data.name = electricalProp->first_node("Name")->value();
        data.nominalPower = XMLParser::GetNodeValueDouble(electricalProp, "NominalPower");
        data.nominalPowerUnit =
            (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "NominalPower", "UnitID");
        data.nominalVoltage = XMLParser::GetNodeValueDouble(electricalProp, "NominalVoltage");
        data.nominalVoltageUnit =
            (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "NominalVoltage", "UnitID");
        data.activePower = XMLParser::GetNodeValueDouble(electricalProp, "ActivePower");
        data.activePowerUnit = (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "ActivePower", "UnitID");
        data.reactivePower = XMLParser::GetNodeValueDouble(electricalProp, "ReactivePower");
        data.reactivePowerUnit =
            (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "ReactivePower", "UnitID");
        data.haveMaxReactive = XMLParser::GetNodeValueInt(electricalProp, "HaveMaxReactive");
        data.maxReactive = XMLParser::GetNodeValueDouble(electricalProp, "MaxReactive");
        data.maxReactiveUnit = (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "MaxReactive", "UnitID");
        data.haveMinReactive = XMLParser::GetNodeValueInt(electricalProp, "HaveMinReactive");
        data.minReactive = XMLParser::GetNodeValueDouble(electricalProp, "MinReactive");
        data.minReactiveUnit = (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "MinReactive", "UnitID");
        data.useMachineBase = XMLParser::GetNodeValueInt(electricalProp, "UseMachineBase");

        auto fault = electricalProp->first_node("Fault");
        if(!fault) return false;
        data.positiveResistance = XMLParser::GetNodeValueDouble(fault, "PositiveResistance");
        data.positiveReactance = XMLParser::GetNodeValueDouble(fault, "PositiveReactance");
        data.negativeResistance = XMLParser::GetNodeValueDouble(fault, "NegativeResistance");
        data.negativeReactance = XMLParser::GetNodeValueDouble(fault, "NegativeReactance");
        data.zeroResistance = XMLParser::GetNodeValueDouble(fault, "ZeroResistance");
        data.zeroReactance = XMLParser::GetNodeValueDouble(fault, "ZeroReactance");
        data.groundResistance = XMLParser::GetNodeValueDouble(fault, "GroundResistance");
        data.groundReactance = XMLParser::GetNodeValueDouble(fault, "GroundReactance");
        data.groundNeutral = XMLParser::GetNodeValueInt(fault, "GroundNeutral");

        auto stability = electricalProp->first_node("Stability");
        if(!stability) return false;
        data.plotSyncMachine = XMLParser::GetNodeValueInt(stability, "PlotSyncMachine");
        data.inertia = XMLParser::GetNodeValueDouble(stability, "Inertia");
        data.damping = XMLParser::GetNodeValueDouble(stability, "Damping");
        data.useAVR = XMLParser::GetNodeValueInt(stability, "UseAVR");
        data.useSpeedGovernor = XMLParser::GetNodeValueInt(stability, "UseSpeedGovernor");
        data.armResistance = XMLParser::GetNodeValueDouble(stability, "ArmResistance");
        data.potierReactance = XMLParser::GetNodeValueDouble(stability, "PotierReactance");
        data.satFactor = XMLParser::GetNodeValueDouble(stability, "SatFactor");
        data.syncXd = XMLParser::GetNodeValueDouble(stability, "SyncXd");
        data.syncXq = XMLParser::GetNodeValueDouble(stability, "SyncXq");
        data.transXd = XMLParser::GetNodeValueDouble(stability, "TransXd");
        data.transXq = XMLParser::GetNodeValueDouble(stability, "TransXq");
        data.transTd0 = XMLParser::GetNodeValueDouble(stability, "TransTd0");
        data.transTq0 = XMLParser::GetNodeValueDouble(stability, "TransTq0");
        data.subXd = XMLParser::GetNodeValueDouble(stability, "SubXd");
        data.subXq = XMLParser::GetNodeValueDouble(stability, "SubXq");
        data.subTd0 = XMLParser::GetNodeValueDouble(stability, "SubTd0");
        data.subTq0 = XMLParser::GetNodeValueDouble(stability, "SubTq0");

        auto avr = stability->first_node("AVR");
        if(!avr) return false;
        if(!OpenControlElements(doc, avr, data.avr)) return false;

        auto speedGov = stability->first_node("SpeedGovernor");
        if(!speedGov) return false;
        if(!OpenControlElements(doc, speedGov, data.speedGov)) return false;

        SwitchingData swData;
        auto switchingList = electricalProp->first_node("SwitchingList");
        if(!switchingList) return false;
        auto swNode = switchingList->first_node("Switching");
        while(swNode) {
            swData.swType.push_back((SwitchingType)XMLParser::GetNodeValueInt(swNode, "Type"));
            swData.swTime.push_back(XMLParser::GetNodeValueDouble(swNode, "Time"));
            swNode = swNode->next_sibling("Switching");
        }
        syncGenerator->SetSwitchingData(swData);

        syncGenerator->SetElectricalData(data);

        if(swData.swTime.size() != 0) syncGenerator->SetDynamicEvent(true);

        elementList.push_back(syncGenerator);
        syncGeneratorList.push_back(syncGenerator);
        syncGeneratorNode = syncGeneratorNode->next_sibling("SyncGenerator");
    }  //}

    //{ SyncMotor
    auto syncMotorListNode = elementsNode->first_node("SyncMotorList");
    if(!syncMotorListNode) return false;
    auto syncMotorNode = syncMotorListNode->first_node("SyncMotor");
    while(syncMotorNode) {
        SyncMotor* syncMotor = new SyncMotor();

        auto cadPropNode = syncMotorNode->first_node("CADProperties");
        if(!cadPropNode) return false;

        auto position = cadPropNode->first_node("Position");
        double posX = XMLParser::GetNodeValueDouble(position, "X");
        double posY = XMLParser::GetNodeValueDouble(position, "Y");
        auto size = cadPropNode->first_node("Size");
        double width = XMLParser::GetNodeValueDouble(size, "Width");
        double height = XMLParser::GetNodeValueDouble(size, "Height");
        double angle = XMLParser::GetNodeValueDouble(cadPropNode, "Angle");
        auto nodePosition = cadPropNode->first_node("NodePosition");
        double nodePosX = XMLParser::GetNodeValueDouble(nodePosition, "X");
        double nodePosY = XMLParser::GetNodeValueDouble(nodePosition, "Y");
        int parentID = XMLParser::GetNodeValueInt(cadPropNode, "ParentID");
        if(parentID == -1) {
            // If the element has no parent, create a temporary one, remove and delete.
            Bus* parent = new Bus(wxPoint2DDouble(nodePosX, nodePosY));
            syncMotor->AddParent(parent, wxPoint2DDouble(nodePosX, nodePosY));
            syncMotor->StartMove(syncMotor->GetPosition());
            syncMotor->Move(wxPoint2DDouble(posX, posY));
            syncMotor->RemoveParent(parent);
            delete parent;
        } else {
            Bus* parent = busList[parentID];
            syncMotor->AddParent(parent, wxPoint2DDouble(nodePosX, nodePosY));
            syncMotor->StartMove(syncMotor->GetPosition());
            syncMotor->Move(wxPoint2DDouble(posX, posY));
        }
        syncMotor->SetWidth(width);
        syncMotor->SetHeight(height);

        int numRot = angle / syncMotor->GetRotationAngle();
        bool clockwise = true;
        if(numRot < 0) {
            numRot = std::abs(numRot);
            clockwise = false;
        }
        for(int i = 0; i < numRot; i++) syncMotor->Rotate(clockwise);

        auto electricalProp = syncMotorNode->first_node("ElectricalProperties");
        if(!electricalProp) return false;

        syncMotor->SetOnline(XMLParser::GetNodeValueInt(electricalProp, "IsOnline"));
        SyncMotorElectricalData data = syncMotor->GetElectricalData();
        data.name = electricalProp->first_node("Name")->value();
        data.nominalPower = XMLParser::GetNodeValueDouble(electricalProp, "NominalPower");
        data.nominalPowerUnit =
            (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "NominalPower", "UnitID");
        // data.nominalVoltage = XMLParser::GetNodeValueDouble(electricalProp, "NominalVoltage");
        // data.nominalVoltageUnit = (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "NominalVoltage",
        // "UnitID");
        data.activePower = XMLParser::GetNodeValueDouble(electricalProp, "ActivePower");
        data.activePowerUnit = (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "ActivePower", "UnitID");
        data.reactivePower = XMLParser::GetNodeValueDouble(electricalProp, "ReactivePower");
        data.reactivePowerUnit =
            (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "ReactivePower", "UnitID");
        data.haveMaxReactive = XMLParser::GetNodeValueInt(electricalProp, "HaveMaxReactive");
        data.maxReactive = XMLParser::GetNodeValueDouble(electricalProp, "MaxReactive");
        data.maxReactiveUnit = (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "MaxReactive", "UnitID");
        data.haveMinReactive = XMLParser::GetNodeValueInt(electricalProp, "HaveMinReactive");
        data.minReactive = XMLParser::GetNodeValueDouble(electricalProp, "MinReactive");
        data.minReactiveUnit = (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "MinReactive", "UnitID");
        data.useMachineBase = XMLParser::GetNodeValueInt(electricalProp, "UseMachineBase");

        auto fault = electricalProp->first_node("Fault");
        if(!fault) return false;
        data.positiveResistance = XMLParser::GetNodeValueDouble(fault, "PositiveResistance");
        data.positiveReactance = XMLParser::GetNodeValueDouble(fault, "PositiveReactance");
        data.negativeResistance = XMLParser::GetNodeValueDouble(fault, "NegativeResistance");
        data.negativeReactance = XMLParser::GetNodeValueDouble(fault, "NegativeReactance");
        data.zeroResistance = XMLParser::GetNodeValueDouble(fault, "ZeroResistance");
        data.zeroReactance = XMLParser::GetNodeValueDouble(fault, "ZeroReactance");
        data.groundResistance = XMLParser::GetNodeValueDouble(fault, "GroundResistance");
        data.groundReactance = XMLParser::GetNodeValueDouble(fault, "GroundReactance");
        data.groundNeutral = XMLParser::GetNodeValueInt(fault, "GroundNeutral");

        /*SwitchingData swData;
        auto switchingList = electricalProp->first_node("SwitchingList");
        if(!switchingList) return false;
        auto swNode = switchingList->first_node("Switching");
        while(swNode) {
            swData.swType.push_back((SwitchingType)XMLParser::GetNodeValueInt(swNode, "Type"));
            swData.swTime.push_back(XMLParser::GetNodeValueDouble(swNode, "Time"));
            swNode = swNode->next_sibling("Switching");
        }
        syncMotor->SetSwitchingData(swData);*/

        syncMotor->SetElectricalData(data);
        elementList.push_back(syncMotor);
        syncMotorList.push_back(syncMotor);
        syncMotorNode = syncMotorNode->next_sibling("SyncMotor");
    }  //}

    //{ Transformer
    auto transformerListNode = elementsNode->first_node("TransformerList");
    if(!transformerListNode) return false;
    auto transfomerNode = transformerListNode->first_node("Transfomer");
    while(transfomerNode) {
        Transformer* transformer = new Transformer();

        auto cadPropNode = transfomerNode->first_node("CADProperties");
        if(!cadPropNode) return false;

        auto position = cadPropNode->first_node("Position");
        double posX = XMLParser::GetNodeValueDouble(position, "X");
        double posY = XMLParser::GetNodeValueDouble(position, "Y");
        auto size = cadPropNode->first_node("Size");
        double width = XMLParser::GetNodeValueDouble(size, "Width");
        double height = XMLParser::GetNodeValueDouble(size, "Height");
        double angle = XMLParser::GetNodeValueDouble(cadPropNode, "Angle");

        // Get nodes points
        std::vector<wxPoint2DDouble> ptsList;
        auto nodePosList = cadPropNode->first_node("NodeList");
        if(!nodePosList) return false;
        auto nodePos = nodePosList->first_node("Node");
        while(nodePos) {
            double nodePosX = XMLParser::GetNodeValueDouble(nodePos, "X");
            double nodePosY = XMLParser::GetNodeValueDouble(nodePos, "Y");
            ptsList.push_back(wxPoint2DDouble(nodePosX, nodePosY));
            nodePos = nodePos->next_sibling("Node");
        }

        // Get parents IDs
        auto parentIDList = cadPropNode->first_node("ParentIDList");
        if(!parentIDList) return false;
        auto parentNode = parentIDList->first_node("ParentID");
        long parentID[2] = {-1, -1};
        while(parentNode) {
            long index = 0;
            wxString(parentNode->first_attribute("ID")->value()).ToLong(&index);
            wxString(parentNode->value()).ToCLong(&parentID[index]);
            parentNode = parentNode->next_sibling("ParentID");
        }

        // Set parents (if have)
        Bus *parent1, *parent2;
        if(parentID[0] == -1) {
            parent1 = new Bus(ptsList[0]);
            transformer->AddParent(parent1, ptsList[0]);
        } else {
            parent1 = busList[parentID[0]];
            transformer->AddParent(parent1, ptsList[0]);
        }
        if(parentID[1] == -1) {
            parent2 = new Bus(ptsList[ptsList.size() - 1]);
            transformer->AddParent(parent2, ptsList[ptsList.size() - 1]);
        } else {
            parent2 = busList[parentID[1]];
            transformer->AddParent(parent2, ptsList[ptsList.size() - 1]);
        }

        transformer->StartMove(transformer->GetPosition());
        transformer->Move(wxPoint2DDouble(posX, posY));

        if(parentID[0] == -1) {
            transformer->RemoveParent(parent1);
            delete parent1;
        }
        if(parentID[1] == -1) {
            transformer->RemoveParent(parent2);
            delete parent2;
        }

        transformer->SetWidth(width);
        transformer->SetHeight(height);

        int numRot = angle / transformer->GetRotationAngle();
        bool clockwise = true;
        if(numRot < 0) {
            numRot = std::abs(numRot);
            clockwise = false;
        }
        for(int i = 0; i < numRot; i++) transformer->Rotate(clockwise);

        auto electricalProp = transfomerNode->first_node("ElectricalProperties");
        if(!electricalProp) return false;

        transformer->SetOnline(XMLParser::GetNodeValueInt(electricalProp, "IsOnline"));
        TransformerElectricalData data = transformer->GetElectricalData();
        data.name = electricalProp->first_node("Name")->value();
        data.primaryNominalVoltage = XMLParser::GetNodeValueDouble(electricalProp, "PrimaryNominalVoltage");
        data.primaryNominalVoltageUnit =
            (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "PrimaryNominalVoltage", "UnitID");
        data.secondaryNominalVoltage = XMLParser::GetNodeValueDouble(electricalProp, "SecondaryNominalVoltage");
        data.secondaryNominalVoltageUnit =
            (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "SecondaryNominalVoltage", "UnitID");
        data.nominalPower = XMLParser::GetNodeValueDouble(electricalProp, "NominalPower");
        data.nominalPowerUnit =
            (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "NominalPower", "UnitID");
        data.resistance = XMLParser::GetNodeValueDouble(electricalProp, "Resistance");
        data.resistanceUnit = (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "Resistance", "UnitID");
        data.indReactance = XMLParser::GetNodeValueDouble(electricalProp, "IndReactance");
        data.indReactanceUnit =
            (ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "IndReactance", "UnitID");
        data.connection = (TransformerConnection)XMLParser::GetNodeValueInt(electricalProp, "Connection");
        data.turnsRatio = XMLParser::GetNodeValueDouble(electricalProp, "TurnsRatio");
        data.phaseShift = XMLParser::GetNodeValueDouble(electricalProp, "PhaseShift");
        data.useTransformerPower = XMLParser::GetNodeValueInt(electricalProp, "UseTransfomerPower");

        auto fault = electricalProp->first_node("Fault");
        data.zeroResistance = XMLParser::GetNodeValueDouble(fault, "ZeroResistance");
        data.zeroIndReactance = XMLParser::GetNodeValueDouble(fault, "ZeroIndReactance");
        data.primaryGrndResistance = XMLParser::GetNodeValueDouble(fault, "PrimaryGrndResistance");
        data.primaryGrndReactance = XMLParser::GetNodeValueDouble(fault, "PrimaryGrndReactance");
        data.secondaryGrndResistance = XMLParser::GetNodeValueDouble(fault, "SecondaryGrndResistance");
        data.secondaryGrndReactance = XMLParser::GetNodeValueDouble(fault, "SecondaryGrndReactance");

        SwitchingData swData;
        auto switchingList = electricalProp->first_node("SwitchingList");
        if(!switchingList) return false;
        auto swNode = switchingList->first_node("Switching");
        while(swNode) {
            swData.swType.push_back((SwitchingType)XMLParser::GetNodeValueInt(swNode, "Type"));
            swData.swTime.push_back(XMLParser::GetNodeValueDouble(swNode, "Time"));
            swNode = swNode->next_sibling("Switching");
        }
        transformer->SetSwitchingData(swData);

        transformer->SetElectricaData(data);

        if(swData.swTime.size() != 0) transformer->SetDynamicEvent(true);

        elementList.push_back(transformer);
        transformerList.push_back(transformer);
        transfomerNode = transfomerNode->next_sibling("Transfomer");
    }  //}

    m_workspace->SetElementList(elementList);

    //{ Text
    auto textListNode = elementsNode->first_node("TextList");
    if(!textListNode) return false;
    auto textNode = textListNode->first_node("Text");
    while(textNode) {
        auto cadPropNode = textNode->first_node("CADProperties");
        if(!cadPropNode) return false;

        auto position = cadPropNode->first_node("Position");
        double posX = XMLParser::GetNodeValueDouble(position, "X");
        double posY = XMLParser::GetNodeValueDouble(position, "Y");
        auto size = cadPropNode->first_node("Size");
        double width = XMLParser::GetNodeValueDouble(size, "Width");
        double height = XMLParser::GetNodeValueDouble(size, "Height");
        double angle = XMLParser::GetNodeValueDouble(cadPropNode, "Angle");

        Text* text = new Text(wxPoint2DDouble(posX, posY));

        text->SetWidth(width);
        text->SetHeight(height);

        auto textProperties = textNode->first_node("TextProperties");
        if(!textProperties) return false;

        text->SetElementType((ElementType)XMLParser::GetNodeValueDouble(textProperties, "ElementType"));
        text->SetDataType((DataType)XMLParser::GetNodeValueDouble(textProperties, "DataType"));
        text->SetUnit((ElectricalUnit)XMLParser::GetNodeValueDouble(textProperties, "DataUnit"));
        text->SetDirection(XMLParser::GetNodeValueDouble(textProperties, "Direction"));
        text->SetDecimalPlaces(XMLParser::GetNodeValueDouble(textProperties, "DecimalPlaces"));

        text->SetElementNumber(XMLParser::GetNodeValueInt(textProperties, "ElementNumber"));
        switch(text->GetElementType()) {
            case TYPE_NONE:
                break;
            case TYPE_BUS: {
                Bus* bus = busList[text->GetElementNumber()];
                text->SetElement(bus);
            } break;
            case TYPE_CAPACITOR: {
                Capacitor* capacitor = capacitorList[text->GetElementNumber()];
                text->SetElement(capacitor);
            } break;
            case TYPE_IND_MOTOR: {
                IndMotor* indMotor = indMotorList[text->GetElementNumber()];
                text->SetElement(indMotor);
            } break;
            case TYPE_INDUCTOR: {
                Inductor* inductor = inductorList[text->GetElementNumber()];
                text->SetElement(inductor);
            } break;
            case TYPE_LINE: {
                Line* line = lineList[text->GetElementNumber()];
                text->SetElement(line);
            } break;
            case TYPE_LOAD: {
                Load* load = loadList[text->GetElementNumber()];
                text->SetElement(load);
            } break;
            case TYPE_SYNC_GENERATOR: {
                SyncGenerator* syncGenerator = syncGeneratorList[text->GetElementNumber()];
                text->SetElement(syncGenerator);
            } break;
            case TYPE_SYNC_MOTOR: {
                SyncMotor* syncMotor = syncMotorList[text->GetElementNumber()];
                text->SetElement(syncMotor);
            } break;
            case TYPE_TRANSFORMER: {
                Transformer* transformer = transformerList[text->GetElementNumber()];
                text->SetElement(transformer);
            } break;
        }

        int numRot = angle / text->GetRotationAngle();
        bool clockwise = true;
        if(numRot < 0) {
            numRot = std::abs(numRot);
            clockwise = false;
        }
        for(int i = 0; i < numRot; i++) text->Rotate(clockwise);

        textList.push_back(text);
        textNode = textNode->next_sibling("Text");
    }  //}

    m_workspace->SetTextList(textList);
    return true;
}

void FileHanding::SaveControl(wxFileName path)
{
    // Same process present in SaveProject():
    std::ofstream writeProjectsFile(path.GetFullPath());
    writeProjectsFile.close();

    rapidxml::xml_document<> doc;
    rapidxml::file<> xmlFile(path.GetFullPath().mb_str());
    doc.parse<0>(xmlFile.data());

    rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
    rapidxml::xml_attribute<>* ver = doc.allocate_attribute("version", "1.0");
    rapidxml::xml_attribute<>* encoding = doc.allocate_attribute("encoding", "utf-8");
    decl->append_attribute(ver);
    decl->append_attribute(encoding);
    doc.append_node(decl);

    rapidxml::xml_node<>* rootNode = doc.allocate_node(rapidxml::node_element, "Control");
    doc.append_node(rootNode);

    rapidxml::xml_node<>* projectNameNode = XMLParser::AppendNode(doc, rootNode, "Name");
    XMLParser::SetNodeValue(doc, projectNameNode, path.GetName());

    auto elementsNode = XMLParser::AppendNode(doc, rootNode, "ControlElements");
    SaveControlElements(doc, elementsNode);
    std::ofstream writeXML(path.GetFullPath());
    writeXML << doc;
    writeXML.close();
}

bool FileHanding::OpenControl(wxFileName path,
                              std::vector<ControlElement*>& ctrlElementList,
                              std::vector<ConnectionLine*>& ctrlConnectionList)
{
    rapidxml::xml_document<> doc;
    rapidxml::file<> xmlFile(path.GetFullPath().mb_str());

    doc.parse<0>(xmlFile.data());

    auto projectNode = doc.first_node("Control");
    if(!projectNode) return false;
    // auto nameNode = projectNode->first_node("Name");
    // if(!nameNode) return false;
    // m_controlEditor->SetName(nameNode->value());

    // Open elements
    auto elementsNode = projectNode->first_node("ControlElements");
    if(!elementsNode) return false;

    // auto elementsNode = XMLParser::AppendNode(doc, rootNode, "ControlElements");
    ControlElementContainer* ctrlElementContainer = new ControlElementContainer();
    if(!OpenControlElements(doc, elementsNode, ctrlElementContainer)) return false;
    ctrlElementList = ctrlElementContainer->GetControlElementsList();
    ctrlConnectionList = ctrlElementContainer->GetConnectionLineList();
    return true;
}

void FileHanding::SaveControlElements(rapidxml::xml_document<>& doc,
                                      rapidxml::xml_node<>* elementsNode,
                                      ControlElementContainer* ctrlContainer)
{
    if(!ctrlContainer) {
        ctrlContainer = new ControlElementContainer();
        ctrlContainer->FillContainer(m_controlEditor);
    }

    //{ Constant
    auto constsNode = XMLParser::AppendNode(doc, elementsNode, "ConstantList");
    auto constList = ctrlContainer->GetConstantList();
    for(auto it = constList.begin(), itEnd = constList.end(); it != itEnd; ++it) {
        (*it)->SaveElement(doc, constsNode);
    }  //}

    //{ Exponential
    auto expsNode = XMLParser::AppendNode(doc, elementsNode, "ExponentialList");
    auto expList = ctrlContainer->GetExponentialList();
    for(auto it = expList.begin(), itEnd = expList.end(); it != itEnd; ++it) { (*it)->SaveElement(doc, expsNode); }  //}

    //{ Gain
    auto gainsNode = XMLParser::AppendNode(doc, elementsNode, "GainList");
    auto gainList = ctrlContainer->GetGainList();
    for(auto it = gainList.begin(), itEnd = gainList.end(); it != itEnd; ++it) {
        (*it)->SaveElement(doc, gainsNode);
    }  //}

    //{ IO
    auto iosNode = XMLParser::AppendNode(doc, elementsNode, "IOList");
    auto ioList = ctrlContainer->GetIOControlList();
    for(auto it = ioList.begin(), itEnd = ioList.end(); it != itEnd; ++it) { (*it)->SaveElement(doc, iosNode); }  //}

    //{ Limiter
    auto limitersNode = XMLParser::AppendNode(doc, elementsNode, "LimiterList");
    auto limiterList = ctrlContainer->GetLimiterList();
    for(auto it = limiterList.begin(), itEnd = limiterList.end(); it != itEnd; ++it) {
        (*it)->SaveElement(doc, limitersNode);
    }  //}

    //{ Multiplier
    auto multipliersNode = XMLParser::AppendNode(doc, elementsNode, "MultiplierList");
    auto multiplierList = ctrlContainer->GetMultiplierList();
    for(auto it = multiplierList.begin(), itEnd = multiplierList.end(); it != itEnd; ++it) {
        (*it)->SaveElement(doc, multipliersNode);
    }  //}

    //{ Divider
    auto dividersNode = XMLParser::AppendNode(doc, elementsNode, "DividerList");
    auto dividersList = ctrlContainer->GetDividerList();
    for(auto it = dividersList.begin(), itEnd = dividersList.end(); it != itEnd; ++it) {
        (*it)->SaveElement(doc, dividersNode);
    }  //}

    //{ Rate limiter
    auto rateLimitersNode = XMLParser::AppendNode(doc, elementsNode, "RateLimiterList");
    auto rateLimiterList = ctrlContainer->GetRateLimiterList();
    for(auto it = rateLimiterList.begin(), itEnd = rateLimiterList.end(); it != itEnd; ++it) {
        (*it)->SaveElement(doc, rateLimitersNode);
    }  //}

    //{ Sum
    auto sumsNode = XMLParser::AppendNode(doc, elementsNode, "SumList");
    auto sumList = ctrlContainer->GetSumList();
    for(auto it = sumList.begin(), itEnd = sumList.end(); it != itEnd; ++it) { (*it)->SaveElement(doc, sumsNode); }  //}

    //{ Math expression
    auto mathExprsNode = XMLParser::AppendNode(doc, elementsNode, "MathExprList");
    auto mathExprList = ctrlContainer->GetMathExprList();
    for(auto it = mathExprList.begin(), itEnd = mathExprList.end(); it != itEnd; ++it) {
        (*it)->SaveElement(doc, mathExprsNode);
    }  //}

    //{ Transfer function
    auto tfsNode = XMLParser::AppendNode(doc, elementsNode, "TransferFunctionList");
    auto tfList = ctrlContainer->GetTFList();
    for(auto it = tfList.begin(), itEnd = tfList.end(); it != itEnd; ++it) { (*it)->SaveElement(doc, tfsNode); }  //}

    //{ Connection line
    auto cLinesNode = XMLParser::AppendNode(doc, elementsNode, "ConnectionList");
    auto connLineList = ctrlContainer->GetConnectionLineList();
    for(auto it = connLineList.begin(), itEnd = connLineList.end(); it != itEnd; ++it) {
        ConnectionLine* cLine = *it;
        auto cLineNode = XMLParser::AppendNode(doc, cLinesNode, "Connection");
        XMLParser::SetNodeAttribute(doc, cLineNode, "ID", cLine->GetID());

        // CAD properties
        auto cadProp = XMLParser::AppendNode(doc, cLineNode, "CADProperties");
        auto offset = XMLParser::AppendNode(doc, cadProp, "Offset");
        XMLParser::SetNodeValue(doc, offset, cLine->GetOffset());

        // Parent list
        auto parentsNode = XMLParser::AppendNode(doc, cLineNode, "ParentList");
        auto parentList = cLine->GetParentList();
        int nodeIndex = 0;
        for(auto itP = parentList.begin(), itPEnd = parentList.end(); itP != itPEnd; ++itP) {
            Element* parent = *itP;
            auto parentNode = XMLParser::AppendNode(doc, parentsNode, "Parent");
            auto elementID = XMLParser::AppendNode(doc, parentNode, "ElementID");
            XMLParser::SetNodeValue(doc, elementID, parent->GetID());
            auto nodeID = XMLParser::AppendNode(doc, parentNode, "NodeID");
            XMLParser::SetNodeValue(doc, nodeID, cLine->GetNodeList()[nodeIndex]->GetID());
            nodeIndex++;
        }

        auto parentLine = XMLParser::AppendNode(doc, cLineNode, "ParentLine");
        if(cLine->GetParentLine()) {
            ConnectionLine* parent = cLine->GetParentLine();
            XMLParser::SetNodeAttribute(doc, parentLine, "ID", parent->GetID());
        } else {
            XMLParser::SetNodeAttribute(doc, parentLine, "ID", -1);
        }
    }  //}
}

bool FileHanding::OpenControlElements(rapidxml::xml_document<>& doc,
                                      rapidxml::xml_node<>* elementsNode,
                                      ControlElementContainer* ctrlContainer)
{
    std::vector<ControlElement*> elementList;
    std::vector<ConnectionLine*> connectionList;

    //{ Constant
    auto constListNode = elementsNode->first_node("ConstantList");
    if(constListNode) {
        auto constNode = constListNode->first_node("Constant");
        while(constNode) {
            int id = XMLParser::GetAttributeValueInt(constNode, "ID");
            Constant* constant = new Constant(id);

            if(!constant->OpenElement(constNode)) return false;
            elementList.push_back(constant);

            constNode = constNode->next_sibling("Constant");
        }
    }
    //}

    //{ Exponential
    auto expListNode = elementsNode->first_node("ExponentialList");
    if(expListNode) {
        auto expNode = expListNode->first_node("Exponential");
        while(expNode) {
            int id = XMLParser::GetAttributeValueInt(expNode, "ID");
            Exponential* exponential = new Exponential(id);

            if(!exponential->OpenElement(expNode)) return false;
            elementList.push_back(exponential);

            expNode = expNode->next_sibling("Exponential");
        }
    }
    //}

    //{ Gain
    auto gainListNode = elementsNode->first_node("GainList");
    if(gainListNode) {
        auto gainNode = gainListNode->first_node("Gain");
        while(gainNode) {
            int id = XMLParser::GetAttributeValueInt(gainNode, "ID");
            Gain* gain = new Gain(id);

            if(!gain->OpenElement(gainNode)) return false;
            elementList.push_back(gain);

            gainNode = gainNode->next_sibling("Gain");
        }
    }
    //}

    //{ IO
    auto ioListNode = elementsNode->first_node("IOList");
    if(ioListNode) {
        auto ioNode = ioListNode->first_node("IO");
        while(ioNode) {
            int id = XMLParser::GetAttributeValueInt(ioNode, "ID");
            int ioFlags = XMLParser::GetNodeValueInt(ioNode, "IOFlags");

            IOControl* io = new IOControl(ioFlags, id);

            if(!io->OpenElement(ioNode)) return false;
            elementList.push_back(io);

            ioNode = ioNode->next_sibling("IO");
        }
    }
    //}

    //{ Limiter
    auto limiterListNode = elementsNode->first_node("LimiterList");
    if(limiterListNode) {
        auto limiterNode = limiterListNode->first_node("Limiter");
        while(limiterNode) {
            int id = XMLParser::GetAttributeValueInt(limiterNode, "ID");
            Limiter* limiter = new Limiter(id);

            if(!limiter->OpenElement(limiterNode)) return false;
            elementList.push_back(limiter);

            limiterNode = limiterNode->next_sibling("Limiter");
        }
    }
    //}

    //{ Multiplier
    auto multiplierListNode = elementsNode->first_node("MultiplierList");
    if(multiplierListNode) {
        auto multiplierNode = multiplierListNode->first_node("Multiplier");
        while(multiplierNode) {
            int id = XMLParser::GetAttributeValueInt(multiplierNode, "ID");
            Multiplier* multiplier = new Multiplier(id);

            if(!multiplier->OpenElement(multiplierNode)) return false;
            elementList.push_back(multiplier);

            multiplierNode = multiplierNode->next_sibling("Multiplier");
        }
    }
    //}

    //{ Divider
    auto dividerListNode = elementsNode->first_node("DividerList");
    if(dividerListNode) {
        auto dividerNode = dividerListNode->first_node("Divider");
        while(dividerNode) {
            int id = XMLParser::GetAttributeValueInt(dividerNode, "ID");
            Divider* divider = new Divider(id);

            if(!divider->OpenElement(dividerNode)) return false;
            elementList.push_back(divider);

            dividerNode = dividerNode->next_sibling("Divider");
        }
    }
    //}

    //{ Rate limiter
    auto rateLimiterListNode = elementsNode->first_node("RateLimiterList");
    if(rateLimiterListNode) {
        auto rateLimiterNode = rateLimiterListNode->first_node("RateLimiter");
        while(rateLimiterNode) {
            int id = XMLParser::GetAttributeValueInt(rateLimiterNode, "ID");
            RateLimiter* limiter = new RateLimiter(id);

            if(!limiter->OpenElement(rateLimiterNode)) return false;
            elementList.push_back(limiter);

            rateLimiterNode = rateLimiterNode->next_sibling("RateLimiter");
        }
    }
    //}

    //{ Sum
    auto sumListNode = elementsNode->first_node("SumList");
    if(sumListNode) {
        auto sumNode = sumListNode->first_node("Sum");
        while(sumNode) {
            int id = XMLParser::GetAttributeValueInt(sumNode, "ID");
            Sum* sum = new Sum(id);

            if(!sum->OpenElement(sumNode)) return false;
            elementList.push_back(sum);

            sumNode = sumNode->next_sibling("Sum");
        }
    }
    //}

    //{ Math expression
    auto mathListNode = elementsNode->first_node("MathExprList");
    if(mathListNode) {
        auto mathExprNode = mathListNode->first_node("MathExpr");
        while(mathExprNode) {
            int id = XMLParser::GetAttributeValueInt(mathExprNode, "ID");
            MathExpression* mathExpr = new MathExpression(id);

            if(!mathExpr->OpenElement(mathExprNode)) return false;
            elementList.push_back(mathExpr);

            mathExprNode = mathExprNode->next_sibling("MathExpr");
        }
    }
    //}

    //{ Transfer function
    auto tfListNode = elementsNode->first_node("TransferFunctionList");
    if(tfListNode) {
        auto tfNode = tfListNode->first_node("TransferFunction");
        while(tfNode) {
            int id = XMLParser::GetAttributeValueInt(tfNode, "ID");
            TransferFunction* tf = new TransferFunction(id);

            if(!tf->OpenElement(tfNode)) return false;
            elementList.push_back(tf);

            tfNode = tfNode->next_sibling("TransferFunction");
        }
    }
    //}

    //{ Connection line
    auto connectionListNode = elementsNode->first_node("ConnectionList");
    if(connectionListNode) {
        auto connNode = connectionListNode->first_node("Connection");
        while(connNode) {
            ConnectionLine* cLine = NULL;
            int id = XMLParser::GetAttributeValueInt(connNode, "ID");

            auto cadPropNode = connNode->first_node("CADProperties");
            if(!cadPropNode) return false;
            double offset = XMLParser::GetNodeValueDouble(cadPropNode, "Offset");

            auto parentList = connNode->first_node("ParentList");
            if(!parentList) return false;

            auto parentNode = parentList->first_node("Parent");
            bool firstNode = true;
            while(parentNode) {
                int elementID = XMLParser::GetNodeValueInt(parentNode, "ElementID");
                int nodeID = XMLParser::GetNodeValueInt(parentNode, "NodeID");

                ControlElement* element = ControlElement::GetControlElementFromID(elementList, elementID);
                Node* node = element->GetNodeList()[nodeID];

                if(firstNode) cLine = new ConnectionLine(node, id);
                cLine->AddParent(element);
                element->AddChild(cLine);
                if(!firstNode) cLine->AppendNode(node, element);

                if(firstNode) firstNode = false;
                parentNode = parentNode->next_sibling("Parent");
            }

            auto parentLine = connNode->first_node("ParentLine");
            if(!parentLine) return false;
            int parentLineID = XMLParser::GetAttributeValueInt(parentLine, "ID");
            if(parentLineID != -1) {
                for(auto it = connectionList.begin(), itEnd = connectionList.end(); it != itEnd; ++it) {
                    ConnectionLine* parent = *it;
                    if(parent->GetID() == parentLineID) {
                        cLine->SetParentLine(parent);
                        parent->AddChild(cLine);
                    }
                }
            }

            cLine->SetOffset(offset);
            cLine->UpdatePoints();
            connectionList.push_back(cLine);
            connNode = connNode->next_sibling("Connection");
        }
    }  //}

    ctrlContainer->FillContainer(elementList, connectionList);
    return true;
}
