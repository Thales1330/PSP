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
    std::ofstream writeProjectsFile(path.GetFullPath().mb_str());
    writeProjectsFile.close();

    rapidxml::xml_document<> doc;
    rapidxml::file<> xmlFile(path.GetFullPath());
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
    XMLParser::SetNodeAttribute(doc, basePower, "UnitID", static_cast<int>(simulationData.basePowerUnit));
    auto contCalc = XMLParser::AppendNode(doc, generalPropNode, "ContinuousCalculation");
    auto contCalcFault = XMLParser::AppendNode(doc, contCalc, "Fault");
    XMLParser::SetNodeValue(doc, contCalcFault, simulationData.faultAfterPowerFlow);
    auto contCalcSCPower = XMLParser::AppendNode(doc, contCalc, "SCPower");
    XMLParser::SetNodeValue(doc, contCalcSCPower, simulationData.scPowerAfterPowerFlow);
    auto contCalcTHD = XMLParser::AppendNode(doc, contCalc, "HarmonicDistortion");
    XMLParser::SetNodeValue(doc, contCalcTHD, simulationData.harmDistortionAfterPowerFlow);

    auto powerFlowPropNode = XMLParser::AppendNode(doc, simulationPropNode, "PowerFlow");
    auto solutionMethod = XMLParser::AppendNode(doc, powerFlowPropNode, "SolutionMethod");
    XMLParser::SetNodeValue(doc, solutionMethod, simulationData.powerFlowMethod);
    auto accFactor = XMLParser::AppendNode(doc, powerFlowPropNode, "AccFactor");
    XMLParser::SetNodeValue(doc, accFactor, simulationData.accFator);
    auto pfTolerance = XMLParser::AppendNode(doc, powerFlowPropNode, "Tolerance");
    XMLParser::SetNodeValue(doc, pfTolerance, simulationData.powerFlowTolerance);
    auto pfMaxIter = XMLParser::AppendNode(doc, powerFlowPropNode, "MaxIterations");
    XMLParser::SetNodeValue(doc, pfMaxIter, simulationData.powerFlowMaxIterations);
    auto pfSlackAngle = XMLParser::AppendNode(doc, powerFlowPropNode, "SlackAngle");
    XMLParser::SetNodeValue(doc, pfSlackAngle, simulationData.initAngle);

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
    elementID = 0;
    for(auto it = inductorList.begin(), itEnd = inductorList.end(); it != itEnd; ++it) {
        (*it)->SetID(elementID);
        (*it)->SaveElement(doc, inductorsNode);
        elementID++;
    }
    //}

    //{ Line
    auto linesNode = XMLParser::AppendNode(doc, elementsNode, "LineList");
    auto lineList = allElements.GetLineList();
    elementID = 0;
    for(auto it = lineList.begin(), itEnd = lineList.end(); it != itEnd; ++it) {
        (*it)->SetID(elementID);
        (*it)->SaveElement(doc, linesNode);
        elementID++;
    }
    //}

    //{ Load
    auto loadsNode = XMLParser::AppendNode(doc, elementsNode, "LoadList");
    auto loadList = allElements.GetLoadList();
    elementID = 0;
    for(auto it = loadList.begin(), itEnd = loadList.end(); it != itEnd; ++it) {
        (*it)->SetID(elementID);
        (*it)->SaveElement(doc, loadsNode);
        elementID++;
    }
    //}

    //{ SyncGenerator
    auto syncGeneratorsNode = XMLParser::AppendNode(doc, elementsNode, "SyncGeneratorList");
    auto syncGeneratorList = allElements.GetSyncGeneratorList();
    elementID = 0;
    for(auto it = syncGeneratorList.begin(), itEnd = syncGeneratorList.end(); it != itEnd; ++it) {
        (*it)->SetID(elementID);
        auto elementNode = (*it)->SaveElement(doc, syncGeneratorsNode);

        // Save controls
        auto data = (*it)->GetElectricalData();
        auto electricalProp = elementNode->first_node("ElectricalProperties");
        auto stability = electricalProp->first_node("Stability");

        auto avr = XMLParser::AppendNode(doc, stability, "AVR");
        if(data.avr) SaveControlElements(doc, avr, data.avr);

        auto speedGov = XMLParser::AppendNode(doc, stability, "SpeedGovernor");
        if(data.speedGov) SaveControlElements(doc, speedGov, data.speedGov);

        elementID++;
    }
    //}

    //{ SyncMotor
    auto syncMotorsNode = XMLParser::AppendNode(doc, elementsNode, "SyncMotorList");
    auto syncMotorList = allElements.GetSyncMotorList();
    elementID = 0;
    for(auto it = syncMotorList.begin(), itEnd = syncMotorList.end(); it != itEnd; ++it) {
        (*it)->SetID(elementID);
        (*it)->SaveElement(doc, syncMotorsNode);
        elementID++;
    }
    //}

    //{ Transfomer
    auto transformersNode = XMLParser::AppendNode(doc, elementsNode, "TransformerList");
    auto transformerList = allElements.GetTransformerList();
    elementID = 0;
    for(auto it = transformerList.begin(), itEnd = transformerList.end(); it != itEnd; ++it) {
        (*it)->SetID(elementID);
        (*it)->SaveElement(doc, transformersNode);
        elementID++;
    }
    //}

    //{ HarmCurrent
    auto harmCurrentNode = XMLParser::AppendNode(doc, elementsNode, "HarmCurrentList");
    auto harmCurrentList = allElements.GetHarmCurrentList();
    elementID = 0;
    for(auto it = harmCurrentList.begin(), itEnd = harmCurrentList.end(); it != itEnd; ++it) {
        (*it)->SetID(elementID);
        (*it)->SaveElement(doc, harmCurrentNode);
        elementID++;
    }
    //}

    //{ EMTElement
    auto emtElementNode = XMLParser::AppendNode(doc, elementsNode, "EMTElementList");
    auto emtElementList = allElements.GetEMTElementList();
    elementID = 0;
    for (auto it = emtElementList.begin(), itEnd = emtElementList.end(); it != itEnd; ++it) {
        (*it)->SetID(elementID);
        (*it)->SaveElement(doc, emtElementNode);
        elementID++;
    }
    //}

    //{ Text
    auto textsNode = XMLParser::AppendNode(doc, elementsNode, "TextList");
    auto textList = m_workspace->GetTextList();
    elementID = 0;
    for(auto it = textList.begin(), itEnd = textList.end(); it != itEnd; ++it) {
        (*it)->SetID(elementID);
        (*it)->SaveElement(doc, textsNode);
        elementID++;
    }
    //}

    std::ofstream writeXML(path.GetFullPath().mb_str());
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
            int harmDistortionAfterPowerFlow = XMLParser::GetNodeValueInt(contCalc, "HarmonicDistortion");
            simData.harmDistortionAfterPowerFlow = harmDistortionAfterPowerFlow != -1 ? harmDistortionAfterPowerFlow : false;

            // Power flow
            auto powerFlow = simPropertiesNode->first_node("PowerFlow");
            simData.powerFlowMethod =
                static_cast<PowerFlowMethod>(XMLParser::GetNodeValueInt(powerFlow, "SolutionMethod"));
            simData.accFator = XMLParser::GetNodeValueDouble(powerFlow, "AccFactor");
            simData.powerFlowTolerance = XMLParser::GetNodeValueDouble(powerFlow, "Tolerance");
            simData.powerFlowMaxIterations = XMLParser::GetNodeValueInt(powerFlow, "MaxIterations");
            simData.initAngle = XMLParser::GetNodeValueDouble(powerFlow, "SlackAngle");

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
    std::vector<HarmCurrent*> harmCurrentList;
    std::vector<EMTElement*> emtElementList;
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

        if(!inductor->OpenElement(inductorNode, parentList)) return false;
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

        if(!line->OpenElement(lineNode, parentList)) return false;
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

        if(!load->OpenElement(loadNode, parentList)) return false;
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

        if(!syncGenerator->OpenElement(syncGeneratorNode, parentList)) return false;

        // Open controls.
        auto data = syncGenerator->GetElectricalData();
        auto electricalProp = syncGeneratorNode->first_node("ElectricalProperties");
        auto stability = electricalProp->first_node("Stability");

        auto avr = stability->first_node("AVR");
        if(!avr) return false;
        if(!OpenControlElements(doc, avr, data.avr)) return false;

        auto speedGov = stability->first_node("SpeedGovernor");
        if(!speedGov) return false;
        if(!OpenControlElements(doc, speedGov, data.speedGov)) return false;

        syncGenerator->SetElectricalData(data);

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

        if(!syncMotor->OpenElement(syncMotorNode, parentList)) return false;
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

        if(!transformer->OpenElement(transfomerNode, parentList)) return false;
        elementList.push_back(transformer);
        transformerList.push_back(transformer);

        transfomerNode = transfomerNode->next_sibling("Transfomer");
    }  //}

    //{ HarmCurrent
    auto harmCurrentListNode = elementsNode->first_node("HarmCurrentList");
    if(harmCurrentListNode) {
        auto harmCurrentNode = harmCurrentListNode->first_node("HarmCurrent");
        while(harmCurrentNode) {
            HarmCurrent* harmCurrent = new HarmCurrent();

            if(!harmCurrent->OpenElement(harmCurrentNode, parentList)) return false;
            elementList.push_back(harmCurrent);
            harmCurrentList.push_back(harmCurrent);

            harmCurrentNode = harmCurrentNode->next_sibling("HarmCurrent");
        }
    }  //}
    //{ HarmCurrent
    auto emtElementListNode = elementsNode->first_node("EMTElementList");
    if (emtElementListNode) {
        auto emtElementNode = emtElementListNode->first_node("EMTElement");
        while (emtElementNode) {
            EMTElement* emtElement = new EMTElement();

            if (!emtElement->OpenElement(emtElementNode, parentList)) return false;
            elementList.push_back(emtElement);
            emtElementList.push_back(emtElement);

            emtElementNode = emtElementNode->next_sibling("EMTElement");
        }
    }  //}

    m_workspace->SetElementList(elementList);

    //{ Text
    auto textListNode = elementsNode->first_node("TextList");
    if(!textListNode) return false;
    auto textNode = textListNode->first_node("Text");
    while(textNode) {
        Text* text = new Text();

        if(!text->OpenElement(textNode)) return true;

        switch(text->GetElementTypeText()) {
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
            case TYPE_HARMCURRENT: {
                HarmCurrent* harmCurrent = harmCurrentList[text->GetElementNumber()];
                text->SetElement(harmCurrent);
            } break;
        }

        textList.push_back(text);
        textNode = textNode->next_sibling("Text");
    }  //}

    m_workspace->SetTextList(textList);
    return true;
}

void FileHanding::SaveControl(wxFileName path)
{
    // Same process present in SaveProject():
    std::ofstream writeProjectsFile(path.GetFullPath().mb_str());
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
    std::ofstream writeXML(path.GetFullPath().mb_str());
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
            ConnectionLine* cLine = nullptr;
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
