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

    rapidxml::xml_node<>* projectNameNode = AppendNode(doc, rootNode, "Name");
    SetNodeValue(doc, projectNameNode, path.GetName());

    auto elementsNode = AppendNode(doc, rootNode, "Elements");

    // Save all the data
    ElectricCalculation allElements;
    allElements.GetElementsFromList(m_workspace->GetElementList());

    //{ Buses
    auto busesNode = AppendNode(doc, elementsNode, "BusList");
    auto busList = allElements.GetBusList();
    for(int i = 0; i < (int)busList.size(); i++) {
        Bus* bus = busList[i];
        auto busNode = AppendNode(doc, busesNode, "Bus");
        SetNodeAttribute(doc, busNode, "ID", i);
        auto cadProp = AppendNode(doc, busNode, "CADProperties");
        auto position = AppendNode(doc, cadProp, "Position");
        auto posX = AppendNode(doc, position, "X");
        SetNodeValue(doc, posX, bus->GetPosition().m_x);
        auto posY = AppendNode(doc, position, "Y");
        SetNodeValue(doc, posY, bus->GetPosition().m_y);
        auto size = AppendNode(doc, cadProp, "Size");
        auto width = AppendNode(doc, size, "Width");
        SetNodeValue(doc, width, bus->GetWidth());
        auto height = AppendNode(doc, size, "Height");
        SetNodeValue(doc, height, bus->GetHeight());
        auto angle = AppendNode(doc, cadProp, "Angle");
        SetNodeValue(doc, angle, bus->GetAngle());

        BusElectricalData data = bus->GetEletricalData();
        auto electricalProp = AppendNode(doc, busNode, "ElectricalProperties");
        auto name = AppendNode(doc, electricalProp, "Name");
        SetNodeValue(doc, name, data.name);
        auto nominalVoltage = AppendNode(doc, electricalProp, "NominalVoltage");
        SetNodeValue(doc, nominalVoltage, data.nominalVoltage);
        SetNodeAttribute(doc, nominalVoltage, "UnitID", data.nominalVoltageUnit);
        auto isVoltageControlled = AppendNode(doc, electricalProp, "IsVoltageControlled");
        SetNodeValue(doc, isVoltageControlled, data.isVoltageControlled);
        auto controlledVoltage = AppendNode(doc, electricalProp, "ControlledVoltage");
        SetNodeValue(doc, controlledVoltage, data.controlledVoltage);
        SetNodeAttribute(doc, controlledVoltage, "Choice", data.controlledVoltageUnitChoice);
        auto slackBus = AppendNode(doc, electricalProp, "SlackBus");
        SetNodeValue(doc, slackBus, data.slackBus);

        auto fault = AppendNode(doc, electricalProp, "Fault");
        auto hasFault = AppendNode(doc, fault, "HasFault");
        SetNodeValue(doc, hasFault, data.hasFault);
        auto faultType = AppendNode(doc, fault, "Type");
        SetNodeValue(doc, faultType, data.faultType);
        auto faultLocation = AppendNode(doc, fault, "Location");
        SetNodeValue(doc, faultLocation, data.faultLocation);
        auto faultResistance = AppendNode(doc, fault, "Resistance");
        SetNodeValue(doc, faultResistance, data.faultResistance);
        auto faultReactance = AppendNode(doc, fault, "Reactance");
        SetNodeValue(doc, faultReactance, data.faultReactance);

        auto stability = AppendNode(doc, electricalProp, "Stability");
        auto plotBus = AppendNode(doc, stability, "Plot");
        SetNodeValue(doc, plotBus, data.plotBus);
        auto stabHasFault = AppendNode(doc, stability, "HasFault");
        SetNodeValue(doc, stabHasFault, data.stabHasFault);
        auto stabFaultTime = AppendNode(doc, stability, "FaultTime");
        SetNodeValue(doc, stabFaultTime, data.stabFaultTime);
        auto stabFaultLength = AppendNode(doc, stability, "FaultLength");
        SetNodeValue(doc, stabFaultLength, data.stabFaultLength);
        auto stabFaultResistance = AppendNode(doc, stability, "FaultResistance");
        SetNodeValue(doc, stabFaultResistance, data.stabFaultResistance);
        auto stabFaultReactance = AppendNode(doc, stability, "FaultReactance");
        SetNodeValue(doc, stabFaultReactance, data.stabFaultReactance);

        data.number = i;
        bus->SetElectricalData(data);
    }  //}

    //{ Capacitor
    auto capacitorsNode = AppendNode(doc, elementsNode, "CapacitorList");
    auto capacitorList = allElements.GetCapacitorList();
    for(int i = 0; i < (int)capacitorList.size(); i++) {
        Capacitor* capacitor = capacitorList[i];
        auto capacitorNode = AppendNode(doc, capacitorsNode, "Capacitor");
        SetNodeAttribute(doc, capacitorNode, "ID", i);
        auto cadProp = AppendNode(doc, capacitorNode, "CADProperties");
        auto position = AppendNode(doc, cadProp, "Position");
        auto posX = AppendNode(doc, position, "X");
        SetNodeValue(doc, posX, capacitor->GetPosition().m_x);
        auto posY = AppendNode(doc, position, "Y");
        SetNodeValue(doc, posY, capacitor->GetPosition().m_y);
        auto size = AppendNode(doc, cadProp, "Size");
        auto width = AppendNode(doc, size, "Width");
        SetNodeValue(doc, width, capacitor->GetWidth());
        auto height = AppendNode(doc, size, "Height");
        SetNodeValue(doc, height, capacitor->GetHeight());
        auto angle = AppendNode(doc, cadProp, "Angle");
        SetNodeValue(doc, angle, capacitor->GetAngle());
        auto nodePos = AppendNode(doc, cadProp, "NodePosition");
        auto nodePosX = AppendNode(doc, nodePos, "X");
        SetNodeValue(doc, nodePosX, capacitor->GetPointList()[0].m_x);
        auto nodePosY = AppendNode(doc, nodePos, "Y");
        SetNodeValue(doc, nodePosY, capacitor->GetPointList()[0].m_y);
        auto parentID = AppendNode(doc, cadProp, "ParentID");
        Bus* parent = static_cast<Bus*>(capacitor->GetParentList()[0]);
        if(parent) SetNodeValue(doc, parentID, parent->GetEletricalData().number);

        CapacitorElectricalData data = capacitor->GetElectricalData();
        auto electricalProp = AppendNode(doc, capacitorNode, "ElectricalProperties");
        auto isOnline = AppendNode(doc, electricalProp, "IsOnline");
        SetNodeValue(doc, isOnline, capacitor->IsOnline());
        auto name = AppendNode(doc, electricalProp, "Name");
        SetNodeValue(doc, name, data.name);
        auto reactivePower = AppendNode(doc, electricalProp, "ReactivePower");
        SetNodeValue(doc, reactivePower, data.reactivePower);
        SetNodeAttribute(doc, reactivePower, "UnitID", data.reactivePowerUnit);

        auto switchingList = AppendNode(doc, electricalProp, "SwitchingList");
        SwitchingData swData = capacitor->GetSwitchingData();
        for(int j = 0; j < (int)swData.swType.size(); j++) {
            auto switching = AppendNode(doc, switchingList, "Switching");
            SetNodeAttribute(doc, switching, "ID", j);
            auto swType = AppendNode(doc, switching, "Type");
            SetNodeValue(doc, swType, swData.swType[j]);
            auto swTime = AppendNode(doc, switching, "Time");
            SetNodeValue(doc, swTime, swData.swTime[j]);
        }
    }  //}

    //{ IndMotor
    auto indMotorsNode = AppendNode(doc, elementsNode, "IndMotorList");
    auto indMotorList = allElements.GetIndMotorList();
    for(int i = 0; i < (int)indMotorList.size(); i++) {
        IndMotor* indMotor = indMotorList[i];
        auto indMotorNode = AppendNode(doc, indMotorsNode, "IndMotor");
        SetNodeAttribute(doc, indMotorNode, "ID", i);
        auto cadProp = AppendNode(doc, indMotorNode, "CADProperties");
        auto position = AppendNode(doc, cadProp, "Position");
        auto posX = AppendNode(doc, position, "X");
        SetNodeValue(doc, posX, indMotor->GetPosition().m_x);
        auto posY = AppendNode(doc, position, "Y");
        SetNodeValue(doc, posY, indMotor->GetPosition().m_y);
        auto size = AppendNode(doc, cadProp, "Size");
        auto width = AppendNode(doc, size, "Width");
        SetNodeValue(doc, width, indMotor->GetWidth());
        auto height = AppendNode(doc, size, "Height");
        SetNodeValue(doc, height, indMotor->GetHeight());
        auto angle = AppendNode(doc, cadProp, "Angle");
        SetNodeValue(doc, angle, indMotor->GetAngle());
        auto nodePos = AppendNode(doc, cadProp, "NodePosition");
        auto nodePosX = AppendNode(doc, nodePos, "X");
        SetNodeValue(doc, nodePosX, indMotor->GetPointList()[0].m_x);
        auto nodePosY = AppendNode(doc, nodePos, "Y");
        SetNodeValue(doc, nodePosY, indMotor->GetPointList()[0].m_y);
        auto parentID = AppendNode(doc, cadProp, "ParentID");
        Bus* parent = static_cast<Bus*>(indMotor->GetParentList()[0]);
        if(parent) SetNodeValue(doc, parentID, parent->GetEletricalData().number);

        IndMotorElectricalData data = indMotor->GetElectricalData();
        auto electricalProp = AppendNode(doc, indMotorNode, "ElectricalProperties");
        auto isOnline = AppendNode(doc, electricalProp, "IsOnline");
        SetNodeValue(doc, isOnline, indMotor->IsOnline());
        auto name = AppendNode(doc, electricalProp, "Name");
        SetNodeValue(doc, name, data.name);
        auto activePower = AppendNode(doc, electricalProp, "ActivePower");
        SetNodeValue(doc, activePower, data.activePower);
        SetNodeAttribute(doc, activePower, "UnitID", data.activePowerUnit);
        auto reactivePower = AppendNode(doc, electricalProp, "ReactivePower");
        SetNodeValue(doc, reactivePower, data.reactivePower);
        SetNodeAttribute(doc, reactivePower, "UnitID", data.reactivePowerUnit);
    }  //}

    //{ Inductor
    auto inductorsNode = AppendNode(doc, elementsNode, "InductorList");
    auto inductorList = allElements.GetInductorList();
    for(int i = 0; i < (int)inductorList.size(); i++) {
        Inductor* inductor = inductorList[i];
        auto inductorNode = AppendNode(doc, inductorsNode, "Inductor");
        SetNodeAttribute(doc, inductorNode, "ID", i);
        auto cadProp = AppendNode(doc, inductorNode, "CADProperties");
        auto position = AppendNode(doc, cadProp, "Position");
        auto posX = AppendNode(doc, position, "X");
        SetNodeValue(doc, posX, inductor->GetPosition().m_x);
        auto posY = AppendNode(doc, position, "Y");
        SetNodeValue(doc, posY, inductor->GetPosition().m_y);
        auto size = AppendNode(doc, cadProp, "Size");
        auto width = AppendNode(doc, size, "Width");
        SetNodeValue(doc, width, inductor->GetWidth());
        auto height = AppendNode(doc, size, "Height");
        SetNodeValue(doc, height, inductor->GetHeight());
        auto angle = AppendNode(doc, cadProp, "Angle");
        SetNodeValue(doc, angle, inductor->GetAngle());
        auto nodePos = AppendNode(doc, cadProp, "NodePosition");
        auto nodePosX = AppendNode(doc, nodePos, "X");
        SetNodeValue(doc, nodePosX, inductor->GetPointList()[0].m_x);
        auto nodePosY = AppendNode(doc, nodePos, "Y");
        SetNodeValue(doc, nodePosY, inductor->GetPointList()[0].m_y);
        auto parentID = AppendNode(doc, cadProp, "ParentID");
        Bus* parent = static_cast<Bus*>(inductor->GetParentList()[0]);
        if(parent) SetNodeValue(doc, parentID, parent->GetEletricalData().number);

        InductorElectricalData data = inductor->GetElectricalData();
        auto electricalProp = AppendNode(doc, inductorNode, "ElectricalProperties");
        auto isOnline = AppendNode(doc, electricalProp, "IsOnline");
        SetNodeValue(doc, isOnline, inductor->IsOnline());
        auto name = AppendNode(doc, electricalProp, "Name");
        SetNodeValue(doc, name, data.name);
        auto reactivePower = AppendNode(doc, electricalProp, "ReactivePower");
        SetNodeValue(doc, reactivePower, data.reactivePower);
        SetNodeAttribute(doc, reactivePower, "UnitID", data.reactivePowerUnit);

        auto switchingList = AppendNode(doc, electricalProp, "SwitchingList");
        SwitchingData swData = inductor->GetSwitchingData();
        for(int j = 0; j < (int)swData.swType.size(); j++) {
            auto switching = AppendNode(doc, switchingList, "Switching");
            SetNodeAttribute(doc, switching, "ID", j);
            auto swType = AppendNode(doc, switching, "Type");
            SetNodeValue(doc, swType, swData.swType[j]);
            auto swTime = AppendNode(doc, switching, "Time");
            SetNodeValue(doc, swTime, swData.swTime[j]);
        }
    }  //}

    //{ Line
    auto linesNode = AppendNode(doc, elementsNode, "LineList");
    auto lineList = allElements.GetLineList();
    for(int i = 0; i < (int)lineList.size(); i++) {
        Line* line = lineList[i];
        auto lineNode = AppendNode(doc, linesNode, "Line");
        SetNodeAttribute(doc, lineNode, "ID", i);
        auto cadProp = AppendNode(doc, lineNode, "CADProperties");
        auto nodeList = AppendNode(doc, cadProp, "NodeList");
        auto ptList = line->GetPointList();
        int nodeID = 0;
        for(int j = 0; j < (int)ptList.size(); j++) {
            if((j != 1) && (j != (int)ptList.size() - 2)) {
                auto nodePos = AppendNode(doc, nodeList, "Node");
                SetNodeAttribute(doc, nodePos, "ID", nodeID);
                auto nodePosX = AppendNode(doc, nodePos, "X");
                SetNodeValue(doc, nodePosX, ptList[j].m_x);
                auto nodePosY = AppendNode(doc, nodePos, "Y");
                SetNodeValue(doc, nodePosY, ptList[j].m_y);
                nodeID++;
            }
        }

        auto parentIDList = AppendNode(doc, cadProp, "ParentIDList");
        for(int j = 0; j < (int)line->GetParentList().size(); j++) {
            Bus* parent = static_cast<Bus*>(line->GetParentList()[j]);
            if(parent) {
                auto parentID = AppendNode(doc, parentIDList, "ParentID");
                SetNodeAttribute(doc, parentID, "ID", j);
                SetNodeValue(doc, parentID, parent->GetEletricalData().number);
            }
        }

        LineElectricalData data = line->GetElectricalData();
        auto electricalProp = AppendNode(doc, lineNode, "ElectricalProperties");
        auto isOnline = AppendNode(doc, electricalProp, "IsOnline");
        SetNodeValue(doc, isOnline, line->IsOnline());
        auto name = AppendNode(doc, electricalProp, "Name");
        SetNodeValue(doc, name, data.name);
        auto nominalVoltage = AppendNode(doc, electricalProp, "NominalVoltage");
        SetNodeValue(doc, nominalVoltage, data.nominalVoltage);
        SetNodeAttribute(doc, nominalVoltage, "UnitID", data.nominalVoltageUnit);
        auto nominalPower = AppendNode(doc, electricalProp, "NominalPower");
        SetNodeValue(doc, nominalPower, data.nominalPower);
        SetNodeAttribute(doc, nominalPower, "UnitID", data.nominalPowerUnit);
        auto resistance = AppendNode(doc, electricalProp, "Resistance");
        SetNodeValue(doc, resistance, data.resistance);
        SetNodeAttribute(doc, resistance, "UnitID", data.resistanceUnit);
        auto indReactance = AppendNode(doc, electricalProp, "IndReactance");
        SetNodeValue(doc, indReactance, data.indReactance);
        SetNodeAttribute(doc, indReactance, "UnitID", data.indReactanceUnit);
        auto capSusceptance = AppendNode(doc, electricalProp, "CapSusceptance");
        SetNodeValue(doc, capSusceptance, data.capSusceptance);
        SetNodeAttribute(doc, capSusceptance, "UnitID", data.capSusceptanceUnit);
        auto lineSize = AppendNode(doc, electricalProp, "LineSize");
        SetNodeValue(doc, lineSize, data.lineSize);
        auto useLinePower = AppendNode(doc, electricalProp, "UseLinePower");
        SetNodeValue(doc, useLinePower, data.useLinePower);

        auto fault = AppendNode(doc, electricalProp, "Fault");
        auto zeroResistance = AppendNode(doc, fault, "ZeroResistance");
        SetNodeValue(doc, zeroResistance, data.zeroResistance);
        auto zeroIndReactance = AppendNode(doc, fault, "ZeroIndReactance");
        SetNodeValue(doc, zeroIndReactance, data.zeroIndReactance);
        auto zeroCapSusceptance = AppendNode(doc, fault, "ZeroCapSusceptance");
        SetNodeValue(doc, zeroCapSusceptance, data.zeroCapSusceptance);

        auto switchingList = AppendNode(doc, electricalProp, "SwitchingList");
        SwitchingData swData = line->GetSwitchingData();
        for(int j = 0; j < (int)swData.swType.size(); j++) {
            auto switching = AppendNode(doc, switchingList, "Switching");
            SetNodeAttribute(doc, switching, "ID", j);
            auto swType = AppendNode(doc, switching, "Type");
            SetNodeValue(doc, swType, swData.swType[j]);
            auto swTime = AppendNode(doc, switching, "Time");
            SetNodeValue(doc, swTime, swData.swTime[j]);
        }
    }  //}

    //{ Load
    auto loadsNode = AppendNode(doc, elementsNode, "LoadList");
    auto loadList = allElements.GetLoadList();
    for(int i = 0; i < (int)loadList.size(); i++) {
        Load* load = loadList[i];
        auto loadNode = AppendNode(doc, loadsNode, "Load");
        SetNodeAttribute(doc, loadNode, "ID", i);
        auto cadProp = AppendNode(doc, loadNode, "CADProperties");
        auto position = AppendNode(doc, cadProp, "Position");
        auto posX = AppendNode(doc, position, "X");
        SetNodeValue(doc, posX, load->GetPosition().m_x);
        auto posY = AppendNode(doc, position, "Y");
        SetNodeValue(doc, posY, load->GetPosition().m_y);
        auto size = AppendNode(doc, cadProp, "Size");
        auto width = AppendNode(doc, size, "Width");
        SetNodeValue(doc, width, load->GetWidth());
        auto height = AppendNode(doc, size, "Height");
        SetNodeValue(doc, height, load->GetHeight());
        auto angle = AppendNode(doc, cadProp, "Angle");
        SetNodeValue(doc, angle, load->GetAngle());
        auto nodePos = AppendNode(doc, cadProp, "NodePosition");
        auto nodePosX = AppendNode(doc, nodePos, "X");
        SetNodeValue(doc, nodePosX, load->GetPointList()[0].m_x);
        auto nodePosY = AppendNode(doc, nodePos, "Y");
        SetNodeValue(doc, nodePosY, load->GetPointList()[0].m_y);
        auto parentID = AppendNode(doc, cadProp, "ParentID");
        Bus* parent = static_cast<Bus*>(load->GetParentList()[0]);
        if(parent) SetNodeValue(doc, parentID, parent->GetEletricalData().number);

        LoadElectricalData data = load->GetElectricalData();
        auto electricalProp = AppendNode(doc, loadNode, "ElectricalProperties");
        auto isOnline = AppendNode(doc, electricalProp, "IsOnline");
        SetNodeValue(doc, isOnline, load->IsOnline());
        auto name = AppendNode(doc, electricalProp, "Name");
        SetNodeValue(doc, name, data.name);
        auto activePower = AppendNode(doc, electricalProp, "ActivePower");
        SetNodeValue(doc, activePower, data.activePower);
        SetNodeAttribute(doc, activePower, "UnitID", data.activePowerUnit);
        auto reactivePower = AppendNode(doc, electricalProp, "ReactivePower");
        SetNodeValue(doc, reactivePower, data.reactivePower);
        SetNodeAttribute(doc, reactivePower, "UnitID", data.reactivePowerUnit);
        auto loadType = AppendNode(doc, electricalProp, "LoadType");
        SetNodeValue(doc, loadType, data.loadType);

        auto switchingList = AppendNode(doc, electricalProp, "SwitchingList");
        SwitchingData swData = load->GetSwitchingData();
        for(int j = 0; j < (int)swData.swType.size(); j++) {
            auto switching = AppendNode(doc, switchingList, "Switching");
            SetNodeAttribute(doc, switching, "ID", j);
            auto swType = AppendNode(doc, switching, "Type");
            SetNodeValue(doc, swType, swData.swType[j]);
            auto swTime = AppendNode(doc, switching, "Time");
            SetNodeValue(doc, swTime, swData.swTime[j]);
        }
    }  //}

    //{ SyncGenerator
    auto syncGeneratorsNode = AppendNode(doc, elementsNode, "SyncGeneratorList");
    auto syncGeneratorList = allElements.GetSyncGeneratorList();
    for(int i = 0; i < (int)syncGeneratorList.size(); i++) {
        SyncGenerator* syncGenerator = syncGeneratorList[i];
        auto syncGeneratorNode = AppendNode(doc, syncGeneratorsNode, "SyncGenerator");
        SetNodeAttribute(doc, syncGeneratorNode, "ID", i);
        auto cadProp = AppendNode(doc, syncGeneratorNode, "CADProperties");
        auto position = AppendNode(doc, cadProp, "Position");
        auto posX = AppendNode(doc, position, "X");
        SetNodeValue(doc, posX, syncGenerator->GetPosition().m_x);
        auto posY = AppendNode(doc, position, "Y");
        SetNodeValue(doc, posY, syncGenerator->GetPosition().m_y);
        auto size = AppendNode(doc, cadProp, "Size");
        auto width = AppendNode(doc, size, "Width");
        SetNodeValue(doc, width, syncGenerator->GetWidth());
        auto height = AppendNode(doc, size, "Height");
        SetNodeValue(doc, height, syncGenerator->GetHeight());
        auto angle = AppendNode(doc, cadProp, "Angle");
        SetNodeValue(doc, angle, syncGenerator->GetAngle());
        auto nodePos = AppendNode(doc, cadProp, "NodePosition");
        auto nodePosX = AppendNode(doc, nodePos, "X");
        SetNodeValue(doc, nodePosX, syncGenerator->GetPointList()[0].m_x);
        auto nodePosY = AppendNode(doc, nodePos, "Y");
        SetNodeValue(doc, nodePosY, syncGenerator->GetPointList()[0].m_y);
        auto parentID = AppendNode(doc, cadProp, "ParentID");
        Bus* parent = static_cast<Bus*>(syncGenerator->GetParentList()[0]);
        if(parent) SetNodeValue(doc, parentID, parent->GetEletricalData().number);

        SyncGeneratorElectricalData data = syncGenerator->GetElectricalData();
        auto electricalProp = AppendNode(doc, syncGeneratorNode, "ElectricalProperties");
        auto isOnline = AppendNode(doc, electricalProp, "IsOnline");
        SetNodeValue(doc, isOnline, syncGenerator->IsOnline());
        auto name = AppendNode(doc, electricalProp, "Name");
        SetNodeValue(doc, name, data.name);
        auto nominalPower = AppendNode(doc, electricalProp, "NominalPower");
        SetNodeValue(doc, nominalPower, data.nominalPower);
        SetNodeAttribute(doc, nominalPower, "UnitID", data.nominalPowerUnit);
        auto nominalVoltage = AppendNode(doc, electricalProp, "NominalVoltage");
        SetNodeValue(doc, nominalVoltage, data.nominalVoltage);
        SetNodeAttribute(doc, nominalVoltage, "UnitID", data.nominalVoltageUnit);
        auto activePower = AppendNode(doc, electricalProp, "ActivePower");
        SetNodeValue(doc, activePower, data.activePower);
        SetNodeAttribute(doc, activePower, "UnitID", data.activePowerUnit);
        auto reactivePower = AppendNode(doc, electricalProp, "ReactivePower");
        SetNodeValue(doc, reactivePower, data.reactivePower);
        SetNodeAttribute(doc, reactivePower, "UnitID", data.reactivePowerUnit);
        auto haveMaxReactive = AppendNode(doc, electricalProp, "HaveMaxReactive");
        SetNodeValue(doc, haveMaxReactive, data.haveMaxReactive);
        auto maxReactive = AppendNode(doc, electricalProp, "MaxReactive");
        SetNodeValue(doc, maxReactive, data.maxReactive);
        SetNodeAttribute(doc, maxReactive, "UnitID", data.maxReactiveUnit);
        auto haveMinReactive = AppendNode(doc, electricalProp, "HaveMinReactive");
        SetNodeValue(doc, haveMinReactive, data.haveMinReactive);
        auto minReactive = AppendNode(doc, electricalProp, "MinReactive");
        SetNodeValue(doc, minReactive, data.minReactive);
        SetNodeAttribute(doc, minReactive, "UnitID", data.minReactiveUnit);
        auto useMachineBase = AppendNode(doc, electricalProp, "UseMachineBase");
        SetNodeValue(doc, useMachineBase, data.useMachineBase);

        auto fault = AppendNode(doc, electricalProp, "Fault");
        auto positiveResistance = AppendNode(doc, fault, "PositiveResistance");
        SetNodeValue(doc, positiveResistance, data.positiveResistance);
        auto positiveReactance = AppendNode(doc, fault, "PositiveReactance");
        SetNodeValue(doc, positiveReactance, data.positiveReactance);
        auto negativeResistance = AppendNode(doc, fault, "NegativeResistance");
        SetNodeValue(doc, negativeResistance, data.negativeResistance);
        auto negativeReactance = AppendNode(doc, fault, "NegativeReactance");
        SetNodeValue(doc, negativeReactance, data.negativeReactance);
        auto zeroResistance = AppendNode(doc, fault, "ZeroResistance");
        SetNodeValue(doc, zeroResistance, data.zeroResistance);
        auto zeroReactance = AppendNode(doc, fault, "ZeroReactance");
        SetNodeValue(doc, zeroReactance, data.zeroReactance);
        auto groundResistance = AppendNode(doc, fault, "GroundResistance");
        SetNodeValue(doc, groundResistance, data.groundResistance);
        auto groundReactance = AppendNode(doc, fault, "GroundReactance");
        SetNodeValue(doc, groundReactance, data.groundReactance);
        auto groundNeutral = AppendNode(doc, fault, "GroundNeutral");
        SetNodeValue(doc, groundNeutral, data.groundNeutral);

        auto stability = AppendNode(doc, electricalProp, "Stability");
        auto plotSyncMachine = AppendNode(doc, stability, "PlotSyncMachine");
        SetNodeValue(doc, plotSyncMachine, data.plotSyncMachine);
        auto inertia = AppendNode(doc, stability, "Inertia");
        SetNodeValue(doc, inertia, data.inertia);
        auto damping = AppendNode(doc, stability, "Damping");
        SetNodeValue(doc, damping, data.damping);
        auto useAVR = AppendNode(doc, stability, "UseAVR");
        SetNodeValue(doc, useAVR, data.useAVR);
        auto useSpeedGovernor = AppendNode(doc, stability, "UseSpeedGovernor");
        SetNodeValue(doc, useSpeedGovernor, data.useSpeedGovernor);
        auto armResistance = AppendNode(doc, stability, "ArmResistance");
        SetNodeValue(doc, armResistance, data.armResistance);
        auto potierReactance = AppendNode(doc, stability, "PotierReactance");
        SetNodeValue(doc, potierReactance, data.potierReactance);
        auto satFactor = AppendNode(doc, stability, "SatFactor");
        SetNodeValue(doc, satFactor, data.satFactor);
        auto syncXd = AppendNode(doc, stability, "SyncXd");
        SetNodeValue(doc, syncXd, data.syncXd);
        auto syncXq = AppendNode(doc, stability, "SyncXq");
        SetNodeValue(doc, syncXq, data.syncXq);
        auto transXd = AppendNode(doc, stability, "TransXd");
        SetNodeValue(doc, transXd, data.transXd);
        auto transXq = AppendNode(doc, stability, "TransXq");
        SetNodeValue(doc, transXq, data.transXq);
        auto transTd0 = AppendNode(doc, stability, "TransTd0");
        SetNodeValue(doc, transTd0, data.transTd0);
        auto transTq0 = AppendNode(doc, stability, "TransTq0");
        SetNodeValue(doc, transTq0, data.transTq0);
        auto subXd = AppendNode(doc, stability, "SubXd");
        SetNodeValue(doc, subXd, data.subXd);
        auto subXq = AppendNode(doc, stability, "SubXq");
        SetNodeValue(doc, subXq, data.subXq);
        auto subTd0 = AppendNode(doc, stability, "SubTd0");
        SetNodeValue(doc, subTd0, data.subTd0);
        auto subTq0 = AppendNode(doc, stability, "SubTq0");
        SetNodeValue(doc, subTq0, data.subTq0);

        auto switchingList = AppendNode(doc, electricalProp, "SwitchingList");
        SwitchingData swData = syncGenerator->GetSwitchingData();
        for(int j = 0; j < (int)swData.swType.size(); j++) {
            auto switching = AppendNode(doc, switchingList, "Switching");
            SetNodeAttribute(doc, switching, "ID", j);
            auto swType = AppendNode(doc, switching, "Type");
            SetNodeValue(doc, swType, swData.swType[j]);
            auto swTime = AppendNode(doc, switching, "Time");
            SetNodeValue(doc, swTime, swData.swTime[j]);
        }
    }  //}

    //{ SyncMotor
    auto syncMotorsNode = AppendNode(doc, elementsNode, "SyncMotorList");
    auto syncMotorList = allElements.GetSyncMotorList();
    for(int i = 0; i < (int)syncMotorList.size(); i++) {
        SyncMotor* syncMotor = syncMotorList[i];
        auto syncMotorNode = AppendNode(doc, syncMotorsNode, "SyncMotor");
        SetNodeAttribute(doc, syncMotorNode, "ID", i);
        auto cadProp = AppendNode(doc, syncMotorNode, "CADProperties");
        auto position = AppendNode(doc, cadProp, "Position");
        auto posX = AppendNode(doc, position, "X");
        SetNodeValue(doc, posX, syncMotor->GetPosition().m_x);
        auto posY = AppendNode(doc, position, "Y");
        SetNodeValue(doc, posY, syncMotor->GetPosition().m_y);
        auto size = AppendNode(doc, cadProp, "Size");
        auto width = AppendNode(doc, size, "Width");
        SetNodeValue(doc, width, syncMotor->GetWidth());
        auto height = AppendNode(doc, size, "Height");
        SetNodeValue(doc, height, syncMotor->GetHeight());
        auto angle = AppendNode(doc, cadProp, "Angle");
        SetNodeValue(doc, angle, syncMotor->GetAngle());
        auto nodePos = AppendNode(doc, cadProp, "NodePosition");
        auto nodePosX = AppendNode(doc, nodePos, "X");
        SetNodeValue(doc, nodePosX, syncMotor->GetPointList()[0].m_x);
        auto nodePosY = AppendNode(doc, nodePos, "Y");
        SetNodeValue(doc, nodePosY, syncMotor->GetPointList()[0].m_y);
        auto parentID = AppendNode(doc, cadProp, "ParentID");
        Bus* parent = static_cast<Bus*>(syncMotor->GetParentList()[0]);
        if(parent) SetNodeValue(doc, parentID, parent->GetEletricalData().number);

        SyncMotorElectricalData data = syncMotor->GetElectricalData();
        auto electricalProp = AppendNode(doc, syncMotorNode, "ElectricalProperties");
        auto isOnline = AppendNode(doc, electricalProp, "IsOnline");
        SetNodeValue(doc, isOnline, syncMotor->IsOnline());
        auto name = AppendNode(doc, electricalProp, "Name");
        SetNodeValue(doc, name, data.name);
        auto nominalPower = AppendNode(doc, electricalProp, "NominalPower");
        SetNodeValue(doc, nominalPower, data.nominalPower);
        SetNodeAttribute(doc, nominalPower, "UnitID", data.nominalPowerUnit);
        // auto nominalVoltage = AppendNode(doc, electricalProp, "NominalVoltage");
        // SetNodeValue(doc, nominalVoltage, data.nominalVoltage);
        // SetNodeAttribute(doc, nominalVoltage, "UnitID", data.nominalVoltageUnit);
        auto activePower = AppendNode(doc, electricalProp, "ActivePower");
        SetNodeValue(doc, activePower, data.activePower);
        SetNodeAttribute(doc, activePower, "UnitID", data.activePowerUnit);
        auto reactivePower = AppendNode(doc, electricalProp, "ReactivePower");
        SetNodeValue(doc, reactivePower, data.reactivePower);
        SetNodeAttribute(doc, reactivePower, "UnitID", data.reactivePowerUnit);
        auto haveMaxReactive = AppendNode(doc, electricalProp, "HaveMaxReactive");
        SetNodeValue(doc, haveMaxReactive, data.haveMaxReactive);
        auto maxReactive = AppendNode(doc, electricalProp, "MaxReactive");
        SetNodeValue(doc, maxReactive, data.maxReactive);
        SetNodeAttribute(doc, maxReactive, "UnitID", data.maxReactiveUnit);
        auto haveMinReactive = AppendNode(doc, electricalProp, "HaveMinReactive");
        SetNodeValue(doc, haveMinReactive, data.haveMinReactive);
        auto minReactive = AppendNode(doc, electricalProp, "MinReactive");
        SetNodeValue(doc, minReactive, data.minReactive);
        SetNodeAttribute(doc, minReactive, "UnitID", data.minReactiveUnit);
        auto useMachineBase = AppendNode(doc, electricalProp, "UseMachineBase");
        SetNodeValue(doc, useMachineBase, data.useMachineBase);

        auto fault = AppendNode(doc, electricalProp, "Fault");
        auto positiveResistance = AppendNode(doc, fault, "PositiveResistance");
        SetNodeValue(doc, positiveResistance, data.positiveResistance);
        auto positiveReactance = AppendNode(doc, fault, "PositiveReactance");
        SetNodeValue(doc, positiveReactance, data.positiveReactance);
        auto negativeResistance = AppendNode(doc, fault, "NegativeResistance");
        SetNodeValue(doc, negativeResistance, data.negativeResistance);
        auto negativeReactance = AppendNode(doc, fault, "NegativeReactance");
        SetNodeValue(doc, negativeReactance, data.negativeReactance);
        auto zeroResistance = AppendNode(doc, fault, "ZeroResistance");
        SetNodeValue(doc, zeroResistance, data.zeroResistance);
        auto zeroReactance = AppendNode(doc, fault, "ZeroReactance");
        SetNodeValue(doc, zeroReactance, data.zeroReactance);
        auto groundResistance = AppendNode(doc, fault, "GroundResistance");
        SetNodeValue(doc, groundResistance, data.groundResistance);
        auto groundReactance = AppendNode(doc, fault, "GroundReactance");
        SetNodeValue(doc, groundReactance, data.groundReactance);
        auto groundNeutral = AppendNode(doc, fault, "GroundNeutral");
        SetNodeValue(doc, groundNeutral, data.groundNeutral);

        // To future use...
        /*auto stability = AppendNode(doc, electricalProp, "Stability");
        auto plotSyncMachine = AppendNode(doc, stability, "PlotSyncMotor");
        SetNodeValue(doc, plotSyncMachine, data.plotSyncMachine);
        auto inertia = AppendNode(doc, stability, "Inertia");
        SetNodeValue(doc, inertia, data.inertia);
        auto damping = AppendNode(doc, stability, "Damping");
        SetNodeValue(doc, damping, data.damping);
        auto useAVR = AppendNode(doc, stability, "UseAVR");
        SetNodeValue(doc, useAVR, data.useAVR);
        auto armResistance = AppendNode(doc, stability, "ArmResistance");
        SetNodeValue(doc, armResistance, data.armResistance);
        auto potierReactance = AppendNode(doc, stability, "PotierReactance");
        SetNodeValue(doc, potierReactance, data.potierReactance);
        auto satFactor = AppendNode(doc, stability, "SatFactor");
        SetNodeValue(doc, satFactor, data.satFactor);
        auto syncXd = AppendNode(doc, stability, "SyncXd");
        SetNodeValue(doc, syncXd, data.syncXd);
        auto syncXq = AppendNode(doc, stability, "SyncXq");
        SetNodeValue(doc, syncXq, data.syncXq);
        auto transXd = AppendNode(doc, stability, "TransXd");
        SetNodeValue(doc, transXd, data.transXd);
        auto transXq = AppendNode(doc, stability, "TransXq");
        SetNodeValue(doc, transXq, data.transXq);
        auto transTd0 = AppendNode(doc, stability, "TransTd0");
        SetNodeValue(doc, transTd0, data.transTd0);
        auto transTq0 = AppendNode(doc, stability, "TransTq0");
        SetNodeValue(doc, transTq0, data.transTq0);
        auto subXd = AppendNode(doc, stability, "SubXd");
        SetNodeValue(doc, subXd, data.subXd);
        auto subXq = AppendNode(doc, stability, "SubXq");
        SetNodeValue(doc, subXq, data.subXq);
        auto subTd0 = AppendNode(doc, stability, "SubTd0");
        SetNodeValue(doc, subTd0, data.subTd0);
        auto subTq0 = AppendNode(doc, stability, "SubTq0");
        SetNodeValue(doc, subTq0, data.subTq0);

        auto switchingList = AppendNode(doc, electricalProp, "SwitchingList");
        SwitchingData swData = syncGenerator->GetSwitchingData();
        for(int j = 0; j < (int)swData.swType.size(); j++) {
            auto switching = AppendNode(doc, switchingList, "Switching");
            SetNodeAttribute(doc, switching, "ID", j);
            auto swType = AppendNode(doc, switching, "Type");
            SetNodeValue(doc, swType, swData.swType[j]);
            auto swTime = AppendNode(doc, switching, "Time");
            SetNodeValue(doc, swTime, swData.swTime[j]);
        }*/
    }  //}

    //{ Transfomer
    auto transformersNode = AppendNode(doc, elementsNode, "TransformerList");
    auto transformerList = allElements.GetTransformerList();
    for(int i = 0; i < (int)transformerList.size(); i++) {
        Transformer* transfomer = transformerList[i];
        auto transformerNode = AppendNode(doc, transformersNode, "Transfomer");
        SetNodeAttribute(doc, transformerNode, "ID", i);
        auto cadProp = AppendNode(doc, transformerNode, "CADProperties");
        auto position = AppendNode(doc, cadProp, "Position");
        auto posX = AppendNode(doc, position, "X");
        SetNodeValue(doc, posX, transfomer->GetPosition().m_x);
        auto posY = AppendNode(doc, position, "Y");
        SetNodeValue(doc, posY, transfomer->GetPosition().m_y);
        auto size = AppendNode(doc, cadProp, "Size");
        auto width = AppendNode(doc, size, "Width");
        SetNodeValue(doc, width, transfomer->GetWidth());
        auto height = AppendNode(doc, size, "Height");
        SetNodeValue(doc, height, transfomer->GetHeight());
        auto angle = AppendNode(doc, cadProp, "Angle");
        SetNodeValue(doc, angle, transfomer->GetAngle());
        auto nodeList = AppendNode(doc, cadProp, "NodeList");
        auto nodePos1 = AppendNode(doc, nodeList, "Node");
        SetNodeAttribute(doc, nodePos1, "ID", 0);
        auto nodePosX1 = AppendNode(doc, nodePos1, "X");
        SetNodeValue(doc, nodePosX1, transfomer->GetPointList()[0].m_x);
        auto nodePosY1 = AppendNode(doc, nodePos1, "Y");
        SetNodeValue(doc, nodePosY1, transfomer->GetPointList()[0].m_y);
        auto nodePos2 = AppendNode(doc, nodeList, "Node");
        SetNodeAttribute(doc, nodePos2, "ID", 1);
        auto nodePosX2 = AppendNode(doc, nodePos2, "X");
        SetNodeValue(doc, nodePosX2, transfomer->GetPointList()[transfomer->GetPointList().size() - 1].m_x);
        auto nodePosY2 = AppendNode(doc, nodePos2, "Y");
        SetNodeValue(doc, nodePosY2, transfomer->GetPointList()[transfomer->GetPointList().size() - 1].m_y);

        auto parentIDList = AppendNode(doc, cadProp, "ParentIDList");
        for(int j = 0; j < (int)transfomer->GetParentList().size(); j++) {
            Bus* parent = static_cast<Bus*>(transfomer->GetParentList()[j]);
            if(parent) {
                auto parentID = AppendNode(doc, parentIDList, "ParentID");
                SetNodeAttribute(doc, parentID, "ID", j);
                SetNodeValue(doc, parentID, parent->GetEletricalData().number);
            }
        }

        TransformerElectricalData data = transfomer->GetElectricalData();
        auto electricalProp = AppendNode(doc, transformerNode, "ElectricalProperties");
        auto isOnline = AppendNode(doc, electricalProp, "IsOnline");
        SetNodeValue(doc, isOnline, transfomer->IsOnline());
        auto name = AppendNode(doc, electricalProp, "Name");
        SetNodeValue(doc, name, data.name);
        auto primaryNominalVoltage = AppendNode(doc, electricalProp, "PrimaryNominalVoltage");
        SetNodeValue(doc, primaryNominalVoltage, data.primaryNominalVoltage);
        SetNodeAttribute(doc, primaryNominalVoltage, "UnitID", data.primaryNominalVoltageUnit);
        auto secondaryNominalVoltage = AppendNode(doc, electricalProp, "SecondaryNominalVoltage");
        SetNodeValue(doc, secondaryNominalVoltage, data.secondaryNominalVoltage);
        SetNodeAttribute(doc, secondaryNominalVoltage, "UnitID", data.secondaryNominalVoltageUnit);
        auto nominalPower = AppendNode(doc, electricalProp, "NominalPower");
        SetNodeValue(doc, nominalPower, data.nominalPower);
        SetNodeAttribute(doc, nominalPower, "UnitID", data.nominalPowerUnit);
        auto resistance = AppendNode(doc, electricalProp, "Resistance");
        SetNodeValue(doc, resistance, data.resistance);
        SetNodeAttribute(doc, resistance, "UnitID", data.resistanceUnit);
        auto indReactance = AppendNode(doc, electricalProp, "IndReactance");
        SetNodeValue(doc, indReactance, data.indReactance);
        SetNodeAttribute(doc, indReactance, "UnitID", data.indReactanceUnit);
        auto connection = AppendNode(doc, electricalProp, "Connection");
        SetNodeValue(doc, connection, data.connection);
        auto turnsRatio = AppendNode(doc, electricalProp, "TurnsRatio");
        SetNodeValue(doc, turnsRatio, data.turnsRatio);
        auto phaseShift = AppendNode(doc, electricalProp, "PhaseShift");
        SetNodeValue(doc, phaseShift, data.phaseShift);
        auto useTransformerPower = AppendNode(doc, electricalProp, "UseTransfomerPower");
        SetNodeValue(doc, useTransformerPower, data.useTransformerPower);

        auto fault = AppendNode(doc, electricalProp, "Fault");
        auto zeroResistance = AppendNode(doc, fault, "ZeroResistance");
        SetNodeValue(doc, zeroResistance, data.zeroResistance);
        auto zeroIndReactance = AppendNode(doc, fault, "ZeroIndReactance");
        SetNodeValue(doc, zeroIndReactance, data.zeroIndReactance);
        auto primaryGrndResistance = AppendNode(doc, fault, "PrimaryGrndResistance");
        SetNodeValue(doc, primaryGrndResistance, data.primaryGrndResistance);
        auto primaryGrndReactance = AppendNode(doc, fault, "PrimaryGrndReactance");
        SetNodeValue(doc, primaryGrndReactance, data.primaryGrndReactance);
        auto secondaryGrndResistance = AppendNode(doc, fault, "SecondaryGrndResistance");
        SetNodeValue(doc, secondaryGrndResistance, data.secondaryGrndResistance);
        auto secondaryGrndReactance = AppendNode(doc, fault, "SecondaryGrndReactance");
        SetNodeValue(doc, secondaryGrndReactance, data.secondaryGrndReactance);

        auto switchingList = AppendNode(doc, electricalProp, "SwitchingList");
        SwitchingData swData = transfomer->GetSwitchingData();
        for(int j = 0; j < (int)swData.swType.size(); j++) {
            auto switching = AppendNode(doc, switchingList, "Switching");
            SetNodeAttribute(doc, switching, "ID", j);
            auto swType = AppendNode(doc, switching, "Type");
            SetNodeValue(doc, swType, swData.swType[j]);
            auto swTime = AppendNode(doc, switching, "Time");
            SetNodeValue(doc, swTime, swData.swTime[j]);
        }
    }  //}

    //{ Text
    auto textsNode = AppendNode(doc, elementsNode, "TextList");
    auto textList = m_workspace->GetTextList();
    for(int i = 0; i < (int)textList.size(); i++) {
        Text* text = textList[i];
        auto textNode = AppendNode(doc, textsNode, "Text");
        SetNodeAttribute(doc, textNode, "ID", i);
        auto cadProp = AppendNode(doc, textNode, "CADProperties");
        auto position = AppendNode(doc, cadProp, "Position");
        auto posX = AppendNode(doc, position, "X");
        SetNodeValue(doc, posX, text->GetPosition().m_x);
        auto posY = AppendNode(doc, position, "Y");
        SetNodeValue(doc, posY, text->GetPosition().m_y);
        auto size = AppendNode(doc, cadProp, "Size");
        auto width = AppendNode(doc, size, "Width");
        SetNodeValue(doc, width, text->GetWidth());
        auto height = AppendNode(doc, size, "Height");
        SetNodeValue(doc, height, text->GetHeight());
        auto angle = AppendNode(doc, cadProp, "Angle");
        SetNodeValue(doc, angle, text->GetAngle());
        auto textProperties = AppendNode(doc, textNode, "TextProperties");
        auto elementType = AppendNode(doc, textProperties, "ElementType");
        SetNodeValue(doc, elementType, text->GetElementType());
        auto elementNumber = AppendNode(doc, textProperties, "ElementNumber");
        SetNodeValue(doc, elementNumber, text->GetElementNumber());
        auto dataType = AppendNode(doc, textProperties, "DataType");
        SetNodeValue(doc, dataType, text->GetDataType());
        auto dataUnit = AppendNode(doc, textProperties, "DataUnit");
        SetNodeValue(doc, dataUnit, text->GetUnit());
        auto direction = AppendNode(doc, textProperties, "Direction");
        SetNodeValue(doc, direction, text->GetDirection());
        auto decimalPlaces = AppendNode(doc, textProperties, "DecimalPlaces");
        SetNodeValue(doc, decimalPlaces, text->GetDecimalPlaces());
    }
    //}

    std::ofstream writeXML(path.GetFullPath());
    writeXML << doc;
    writeXML.close();
}

bool FileHanding::OpenProject(wxFileName path)
{
    rapidxml::xml_document<> doc;
    rapidxml::file<> xmlFile(path.GetFullPath());

    doc.parse<0>(xmlFile.data());

    auto projectNode = doc.first_node("Project");
    if(!projectNode) return false;
    auto nameNode = projectNode->first_node("Name");
    if(!nameNode) return false;
    m_workspace->SetName(nameNode->value());

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

    //{ Bus
    auto busListNode = elementsNode->first_node("BusList");
    if(!busListNode) return false;
    auto busNode = busListNode->first_node("Bus");
    while(busNode) {
        auto cadPropNode = busNode->first_node("CADProperties");
        if(!cadPropNode) return false;

        auto position = cadPropNode->first_node("Position");
        double posX = GetNodeValueDouble(position, "X");
        double posY = GetNodeValueDouble(position, "Y");
        Bus* bus = new Bus(wxPoint2DDouble(posX, posY));

        auto size = cadPropNode->first_node("Size");
        double width = GetNodeValueDouble(size, "Width");
        double height = GetNodeValueDouble(size, "Height");
        double angle = GetNodeValueDouble(cadPropNode, "Angle");
        bus->SetWidth(width);
        bus->SetHeight(height);
        bus->SetPosition(bus->GetPosition());  // Update bus rectangle.
        int numRot = angle / bus->GetRotationAngle();
        bool clockwise = true;
        if(numRot < 0) {
            numRot = std::abs(numRot);
            clockwise = false;
        }
        for(int i = 0; i < numRot; i++) bus->Rotate(clockwise);

        BusElectricalData data = bus->GetEletricalData();
        auto electricalProp = busNode->first_node("ElectricalProperties");
        if(!electricalProp) return false;

        data.name = electricalProp->first_node("Name")->value();
        data.nominalVoltage = GetNodeValueDouble(electricalProp, "NominalVoltage");
        data.nominalVoltageUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "NominalVoltage", "UnitID");
        data.isVoltageControlled = GetNodeValueInt(electricalProp, "IsVoltageControlled");
        data.controlledVoltage = GetNodeValueDouble(electricalProp, "ControlledVoltage");
        data.controlledVoltageUnitChoice = GetAttributeValueInt(electricalProp, "ControlledVoltage", "Choice");
        data.slackBus = GetNodeValueInt(electricalProp, "SlackBus");
        auto fault = electricalProp->first_node("Fault");
        data.hasFault = GetNodeValueInt(fault, "HasFault");
        data.faultType = (FaultData)GetNodeValueInt(fault, "Type");
        data.faultLocation = (FaultData)GetNodeValueInt(fault, "Location");
        data.faultResistance = GetNodeValueDouble(fault, "Resistance");
        data.faultReactance = GetNodeValueDouble(fault, "Reactance");
        auto stability = electricalProp->first_node("Stability");
        data.plotBus = GetNodeValueInt(stability, "Plot");
        data.stabHasFault = GetNodeValueInt(stability, "HasFault");
        data.stabFaultTime = GetNodeValueDouble(stability, "FaultTime");
        data.stabFaultLength = GetNodeValueDouble(stability, "FaultLength");
        data.stabFaultResistance = GetNodeValueDouble(stability, "FaultResistance");
        data.stabFaultReactance = GetNodeValueDouble(stability, "FaultReactance");

        bus->SetElectricalData(data);
        elementList.push_back(bus);
        busList.push_back(bus);
        busNode = busNode->next_sibling("Bus");
    }  //}

    //{ Capacitor
    auto capacitorListNode = elementsNode->first_node("CapacitorList");
    if(!capacitorListNode) return false;
    auto capacitorNode = capacitorListNode->first_node("Capacitor");
    while(capacitorNode) {
        Capacitor* capacitor = new Capacitor();

        auto cadPropNode = capacitorNode->first_node("CADProperties");
        if(!cadPropNode) return false;

        auto position = cadPropNode->first_node("Position");
        double posX = GetNodeValueDouble(position, "X");
        double posY = GetNodeValueDouble(position, "Y");
        auto size = cadPropNode->first_node("Size");
        double width = GetNodeValueDouble(size, "Width");
        double height = GetNodeValueDouble(size, "Height");
        double angle = GetNodeValueDouble(cadPropNode, "Angle");
        auto nodePosition = cadPropNode->first_node("NodePosition");
        double nodePosX = GetNodeValueDouble(nodePosition, "X");
        double nodePosY = GetNodeValueDouble(nodePosition, "Y");
        int parentID = GetNodeValueInt(cadPropNode, "ParentID");
        if(parentID == -1) {
            // If the element has no parent, create a temporary one, remove and delete.
            Bus* parent = new Bus(wxPoint2DDouble(nodePosX, nodePosY));
            capacitor->AddParent(parent, wxPoint2DDouble(nodePosX, nodePosY));
            capacitor->StartMove(capacitor->GetPosition());
            capacitor->Move(wxPoint2DDouble(posX, posY));
            capacitor->RemoveParent(parent);
            delete parent;
        } else {
            Bus* parent = busList[parentID];
            capacitor->AddParent(parent, wxPoint2DDouble(nodePosX, nodePosY));
            capacitor->StartMove(capacitor->GetPosition());
            capacitor->Move(wxPoint2DDouble(posX, posY));
        }
        capacitor->SetWidth(width);
        capacitor->SetHeight(height);

        int numRot = angle / capacitor->GetRotationAngle();
        bool clockwise = true;
        if(numRot < 0) {
            numRot = std::abs(numRot);
            clockwise = false;
        }
        for(int i = 0; i < numRot; i++) capacitor->Rotate(clockwise);

        auto electricalProp = capacitorNode->first_node("ElectricalProperties");
        if(!electricalProp) return false;

        capacitor->SetOnline(GetNodeValueInt(electricalProp, "IsOnline"));
        CapacitorElectricalData data = capacitor->GetElectricalData();
        data.name = electricalProp->first_node("Name")->value();
        data.reactivePower = GetNodeValueDouble(electricalProp, "ReactivePower");
        data.reactivePowerUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "ReactivePower", "UnitID");

        SwitchingData swData;
        auto switchingList = electricalProp->first_node("SwitchingList");
        if(!switchingList) return false;
        auto swNode = switchingList->first_node("Switching");
        while(swNode) {
            swData.swType.push_back((SwitchingType)GetNodeValueInt(swNode, "Type"));
            swData.swTime.push_back(GetNodeValueDouble(swNode, "Time"));
            swNode = swNode->next_sibling("Switching");
        }
        capacitor->SetSwitchingData(swData);

        capacitor->SetElectricalData(data);
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

        auto cadPropNode = indMotorNode->first_node("CADProperties");
        if(!cadPropNode) return false;

        auto position = cadPropNode->first_node("Position");
        double posX = GetNodeValueDouble(position, "X");
        double posY = GetNodeValueDouble(position, "Y");
        auto size = cadPropNode->first_node("Size");
        double width = GetNodeValueDouble(size, "Width");
        double height = GetNodeValueDouble(size, "Height");
        double angle = GetNodeValueDouble(cadPropNode, "Angle");
        auto nodePosition = cadPropNode->first_node("NodePosition");
        double nodePosX = GetNodeValueDouble(nodePosition, "X");
        double nodePosY = GetNodeValueDouble(nodePosition, "Y");
        int parentID = GetNodeValueInt(cadPropNode, "ParentID");
        if(parentID == -1) {
            // If the element has no parent, create a temporary one, remove and delete.
            Bus* parent = new Bus(wxPoint2DDouble(nodePosX, nodePosY));
            indMotor->AddParent(parent, wxPoint2DDouble(nodePosX, nodePosY));
            indMotor->StartMove(indMotor->GetPosition());
            indMotor->Move(wxPoint2DDouble(posX, posY));
            indMotor->RemoveParent(parent);
            delete parent;
        } else {
            Bus* parent = busList[parentID];
            indMotor->AddParent(parent, wxPoint2DDouble(nodePosX, nodePosY));
            indMotor->StartMove(indMotor->GetPosition());
            indMotor->Move(wxPoint2DDouble(posX, posY));
        }
        indMotor->SetWidth(width);
        indMotor->SetHeight(height);

        int numRot = angle / indMotor->GetRotationAngle();
        bool clockwise = true;
        if(numRot < 0) {
            numRot = std::abs(numRot);
            clockwise = false;
        }
        for(int i = 0; i < numRot; i++) indMotor->Rotate(clockwise);

        auto electricalProp = indMotorNode->first_node("ElectricalProperties");
        if(!electricalProp) return false;

        indMotor->SetOnline(GetNodeValueInt(electricalProp, "IsOnline"));
        IndMotorElectricalData data = indMotor->GetElectricalData();
        data.name = electricalProp->first_node("Name")->value();
        data.activePower = GetNodeValueDouble(electricalProp, "ActivePower");
        data.activePowerUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "ActivePower", "UnitID");
        data.reactivePower = GetNodeValueDouble(electricalProp, "ReactivePower");
        data.reactivePowerUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "ReactivePower", "UnitID");

        indMotor->SetElectricalData(data);
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
        double posX = GetNodeValueDouble(position, "X");
        double posY = GetNodeValueDouble(position, "Y");
        auto size = cadPropNode->first_node("Size");
        double width = GetNodeValueDouble(size, "Width");
        double height = GetNodeValueDouble(size, "Height");
        double angle = GetNodeValueDouble(cadPropNode, "Angle");
        auto nodePosition = cadPropNode->first_node("NodePosition");
        double nodePosX = GetNodeValueDouble(nodePosition, "X");
        double nodePosY = GetNodeValueDouble(nodePosition, "Y");
        int parentID = GetNodeValueInt(cadPropNode, "ParentID");
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

        inductor->SetOnline(GetNodeValueInt(electricalProp, "IsOnline"));
        InductorElectricalData data = inductor->GetElectricalData();
        data.name = electricalProp->first_node("Name")->value();
        data.reactivePower = GetNodeValueDouble(electricalProp, "ReactivePower");
        data.reactivePowerUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "ReactivePower", "UnitID");

        SwitchingData swData;
        auto switchingList = electricalProp->first_node("SwitchingList");
        if(!switchingList) return false;
        auto swNode = switchingList->first_node("Switching");
        while(swNode) {
            swData.swType.push_back((SwitchingType)GetNodeValueInt(swNode, "Type"));
            swData.swTime.push_back(GetNodeValueDouble(swNode, "Time"));
            swNode = swNode->next_sibling("Switching");
        }
        inductor->SetSwitchingData(swData);

        inductor->SetElectricalData(data);
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
            double nodePosX = GetNodeValueDouble(nodePos, "X");
            double nodePosY = GetNodeValueDouble(nodePos, "Y");
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
            line->AddParent(parent1, ptsList[0]);
        } else {
            parent1 = busList[parentID[0]];
            line->AddParent(parent1, ptsList[0]);
        }
        if(parentID[1] == -1) {
            parent2 = new Bus(ptsList[ptsList.size() - 1]);
            line->AddParent(parent2, ptsList[ptsList.size() - 1]);
        } else {
            parent2 = busList[parentID[1]];
            line->AddParent(parent2, ptsList[ptsList.size() - 1]);
        }

        // Add the others nodes (if have)
        std::vector<wxPoint2DDouble> midPts;
        for(int i = 1; i < (int)ptsList.size() - 1; i++) midPts.push_back(ptsList[i]);
        std::vector<wxPoint2DDouble> edgesPts = line->GetPointList();
        edgesPts.insert(edgesPts.begin() + 2, midPts.begin(), midPts.end());
        line->SetPointList(edgesPts);

        if(parentID[0] == -1) {
            line->RemoveParent(parent1);
            delete parent1;
        }
        if(parentID[1] == -1) {
            line->RemoveParent(parent2);
            delete parent2;
        }

        auto electricalProp = lineNode->first_node("ElectricalProperties");
        if(!electricalProp) return false;

        line->SetOnline(GetNodeValueInt(electricalProp, "IsOnline"));
        LineElectricalData data = line->GetElectricalData();
        data.name = electricalProp->first_node("Name")->value();
        data.nominalVoltage = GetNodeValueDouble(electricalProp, "NominalVoltage");
        data.nominalVoltageUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "NominalVoltage", "UnitID");
        data.nominalPower = GetNodeValueDouble(electricalProp, "NominalPower");
        data.nominalPowerUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "NominalPower", "UnitID");
        data.resistance = GetNodeValueDouble(electricalProp, "Resistance");
        data.resistanceUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "Resistance", "UnitID");
        data.indReactance = GetNodeValueDouble(electricalProp, "IndReactance");
        data.indReactanceUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "IndReactance", "UnitID");
        data.capSusceptance = GetNodeValueDouble(electricalProp, "CapSusceptance");
        data.capSusceptanceUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "CapSusceptance", "UnitID");
        data.lineSize = GetNodeValueDouble(electricalProp, "LineSize");
        data.useLinePower = GetNodeValueInt(electricalProp, "UseLinePower");

        auto fault = electricalProp->first_node("Fault");
        data.zeroResistance = GetNodeValueDouble(fault, "ZeroResistance");
        data.zeroIndReactance = GetNodeValueDouble(fault, "ZeroIndReactance");
        data.zeroCapSusceptance = GetNodeValueDouble(fault, "ZeroCapSusceptance");

        SwitchingData swData;
        auto switchingList = electricalProp->first_node("SwitchingList");
        if(!switchingList) return false;
        auto swNode = switchingList->first_node("Switching");
        while(swNode) {
            swData.swType.push_back((SwitchingType)GetNodeValueInt(swNode, "Type"));
            swData.swTime.push_back(GetNodeValueDouble(swNode, "Time"));
            swNode = swNode->next_sibling("Switching");
        }
        line->SetSwitchingData(swData);

        line->SetElectricalData(data);
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
        double posX = GetNodeValueDouble(position, "X");
        double posY = GetNodeValueDouble(position, "Y");
        auto size = cadPropNode->first_node("Size");
        double width = GetNodeValueDouble(size, "Width");
        double height = GetNodeValueDouble(size, "Height");
        double angle = GetNodeValueDouble(cadPropNode, "Angle");
        auto nodePosition = cadPropNode->first_node("NodePosition");
        double nodePosX = GetNodeValueDouble(nodePosition, "X");
        double nodePosY = GetNodeValueDouble(nodePosition, "Y");
        int parentID = GetNodeValueInt(cadPropNode, "ParentID");
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

        load->SetOnline(GetNodeValueInt(electricalProp, "IsOnline"));
        LoadElectricalData data = load->GetElectricalData();
        data.name = electricalProp->first_node("Name")->value();
        data.activePower = GetNodeValueDouble(electricalProp, "ActivePower");
        data.activePowerUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "ActivePower", "UnitID");
        data.reactivePower = GetNodeValueDouble(electricalProp, "ReactivePower");
        data.reactivePowerUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "ReactivePower", "UnitID");
        data.loadType = (LoadType)GetNodeValueInt(electricalProp, "LoadType");

        SwitchingData swData;
        auto switchingList = electricalProp->first_node("SwitchingList");
        if(!switchingList) return false;
        auto swNode = switchingList->first_node("Switching");
        while(swNode) {
            swData.swType.push_back((SwitchingType)GetNodeValueInt(swNode, "Type"));
            swData.swTime.push_back(GetNodeValueDouble(swNode, "Time"));
            swNode = swNode->next_sibling("Switching");
        }
        load->SetSwitchingData(swData);

        load->SetElectricalData(data);
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
        double posX = GetNodeValueDouble(position, "X");
        double posY = GetNodeValueDouble(position, "Y");
        auto size = cadPropNode->first_node("Size");
        double width = GetNodeValueDouble(size, "Width");
        double height = GetNodeValueDouble(size, "Height");
        double angle = GetNodeValueDouble(cadPropNode, "Angle");
        auto nodePosition = cadPropNode->first_node("NodePosition");
        double nodePosX = GetNodeValueDouble(nodePosition, "X");
        double nodePosY = GetNodeValueDouble(nodePosition, "Y");
        int parentID = GetNodeValueInt(cadPropNode, "ParentID");
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

        syncGenerator->SetOnline(GetNodeValueInt(electricalProp, "IsOnline"));
        SyncGeneratorElectricalData data = syncGenerator->GetElectricalData();
        data.name = electricalProp->first_node("Name")->value();
        data.nominalPower = GetNodeValueDouble(electricalProp, "NominalPower");
        data.nominalPowerUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "NominalPower", "UnitID");
        data.nominalVoltage = GetNodeValueDouble(electricalProp, "NominalVoltage");
        data.nominalVoltageUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "NominalVoltage", "UnitID");
        data.activePower = GetNodeValueDouble(electricalProp, "ActivePower");
        data.activePowerUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "ActivePower", "UnitID");
        data.reactivePower = GetNodeValueDouble(electricalProp, "ReactivePower");
        data.reactivePowerUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "ReactivePower", "UnitID");
        data.haveMaxReactive = GetNodeValueInt(electricalProp, "HaveMaxReactive");
        data.maxReactive = GetNodeValueDouble(electricalProp, "MaxReactive");
        data.maxReactiveUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "MaxReactive", "UnitID");
        data.haveMinReactive = GetNodeValueInt(electricalProp, "HaveMinReactive");
        data.minReactive = GetNodeValueDouble(electricalProp, "MinReactive");
        data.minReactiveUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "MinReactive", "UnitID");
        data.useMachineBase = GetNodeValueInt(electricalProp, "UseMachineBase");

        auto fault = electricalProp->first_node("Fault");
        if(!fault) return false;
        data.positiveResistance = GetNodeValueDouble(fault, "PositiveResistance");
        data.positiveReactance = GetNodeValueDouble(fault, "PositiveReactance");
        data.negativeResistance = GetNodeValueDouble(fault, "NegativeResistance");
        data.negativeReactance = GetNodeValueDouble(fault, "NegativeReactance");
        data.zeroResistance = GetNodeValueDouble(fault, "ZeroResistance");
        data.zeroReactance = GetNodeValueDouble(fault, "ZeroReactance");
        data.groundResistance = GetNodeValueDouble(fault, "GroundResistance");
        data.groundReactance = GetNodeValueDouble(fault, "GroundReactance");
        data.groundNeutral = GetNodeValueInt(fault, "GroundNeutral");

        auto stability = electricalProp->first_node("Stability");
        if(!stability) return false;
        data.plotSyncMachine = GetNodeValueInt(stability, "PlotSyncMachine");
        data.inertia = GetNodeValueDouble(stability, "Inertia");
        data.damping = GetNodeValueDouble(stability, "Damping");
        data.useAVR = GetNodeValueInt(stability, "UseAVR");
        data.useSpeedGovernor = GetNodeValueInt(stability, "UseSpeedGovernor");
        data.armResistance = GetNodeValueDouble(stability, "ArmResistance");
        data.potierReactance = GetNodeValueDouble(stability, "PotierReactance");
        data.satFactor = GetNodeValueDouble(stability, "SatFactor");
        data.syncXd = GetNodeValueDouble(stability, "SyncXd");
        data.syncXq = GetNodeValueDouble(stability, "SyncXq");
        data.transXd = GetNodeValueDouble(stability, "TransXd");
        data.transXq = GetNodeValueDouble(stability, "TransXq");
        data.transTd0 = GetNodeValueDouble(stability, "TransTd0");
        data.transTq0 = GetNodeValueDouble(stability, "TransTq0");
        data.subXd = GetNodeValueDouble(stability, "SubXd");
        data.subXq = GetNodeValueDouble(stability, "SubXq");
        data.subTd0 = GetNodeValueDouble(stability, "SubTd0");
        data.subTq0 = GetNodeValueDouble(stability, "SubTq0");

        SwitchingData swData;
        auto switchingList = electricalProp->first_node("SwitchingList");
        if(!switchingList) return false;
        auto swNode = switchingList->first_node("Switching");
        while(swNode) {
            swData.swType.push_back((SwitchingType)GetNodeValueInt(swNode, "Type"));
            swData.swTime.push_back(GetNodeValueDouble(swNode, "Time"));
            swNode = swNode->next_sibling("Switching");
        }
        syncGenerator->SetSwitchingData(swData);

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

        auto cadPropNode = syncMotorNode->first_node("CADProperties");
        if(!cadPropNode) return false;

        auto position = cadPropNode->first_node("Position");
        double posX = GetNodeValueDouble(position, "X");
        double posY = GetNodeValueDouble(position, "Y");
        auto size = cadPropNode->first_node("Size");
        double width = GetNodeValueDouble(size, "Width");
        double height = GetNodeValueDouble(size, "Height");
        double angle = GetNodeValueDouble(cadPropNode, "Angle");
        auto nodePosition = cadPropNode->first_node("NodePosition");
        double nodePosX = GetNodeValueDouble(nodePosition, "X");
        double nodePosY = GetNodeValueDouble(nodePosition, "Y");
        int parentID = GetNodeValueInt(cadPropNode, "ParentID");
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

        syncMotor->SetOnline(GetNodeValueInt(electricalProp, "IsOnline"));
        SyncMotorElectricalData data = syncMotor->GetElectricalData();
        data.name = electricalProp->first_node("Name")->value();
        data.nominalPower = GetNodeValueDouble(electricalProp, "NominalPower");
        data.nominalPowerUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "NominalPower", "UnitID");
        // data.nominalVoltage = GetNodeValueDouble(electricalProp, "NominalVoltage");
        // data.nominalVoltageUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "NominalVoltage", "UnitID");
        data.activePower = GetNodeValueDouble(electricalProp, "ActivePower");
        data.activePowerUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "ActivePower", "UnitID");
        data.reactivePower = GetNodeValueDouble(electricalProp, "ReactivePower");
        data.reactivePowerUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "ReactivePower", "UnitID");
        data.haveMaxReactive = GetNodeValueInt(electricalProp, "HaveMaxReactive");
        data.maxReactive = GetNodeValueDouble(electricalProp, "MaxReactive");
        data.maxReactiveUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "MaxReactive", "UnitID");
        data.haveMinReactive = GetNodeValueInt(electricalProp, "HaveMinReactive");
        data.minReactive = GetNodeValueDouble(electricalProp, "MinReactive");
        data.minReactiveUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "MinReactive", "UnitID");
        data.useMachineBase = GetNodeValueInt(electricalProp, "UseMachineBase");

        auto fault = electricalProp->first_node("Fault");
        if(!fault) return false;
        data.positiveResistance = GetNodeValueDouble(fault, "PositiveResistance");
        data.positiveReactance = GetNodeValueDouble(fault, "PositiveReactance");
        data.negativeResistance = GetNodeValueDouble(fault, "NegativeResistance");
        data.negativeReactance = GetNodeValueDouble(fault, "NegativeReactance");
        data.zeroResistance = GetNodeValueDouble(fault, "ZeroResistance");
        data.zeroReactance = GetNodeValueDouble(fault, "ZeroReactance");
        data.groundResistance = GetNodeValueDouble(fault, "GroundResistance");
        data.groundReactance = GetNodeValueDouble(fault, "GroundReactance");
        data.groundNeutral = GetNodeValueInt(fault, "GroundNeutral");

        /*SwitchingData swData;
        auto switchingList = electricalProp->first_node("SwitchingList");
        if(!switchingList) return false;
        auto swNode = switchingList->first_node("Switching");
        while(swNode) {
            swData.swType.push_back((SwitchingType)GetNodeValueInt(swNode, "Type"));
            swData.swTime.push_back(GetNodeValueDouble(swNode, "Time"));
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
        double posX = GetNodeValueDouble(position, "X");
        double posY = GetNodeValueDouble(position, "Y");
        auto size = cadPropNode->first_node("Size");
        double width = GetNodeValueDouble(size, "Width");
        double height = GetNodeValueDouble(size, "Height");
        double angle = GetNodeValueDouble(cadPropNode, "Angle");

        // Get nodes points
        std::vector<wxPoint2DDouble> ptsList;
        auto nodePosList = cadPropNode->first_node("NodeList");
        if(!nodePosList) return false;
        auto nodePos = nodePosList->first_node("Node");
        while(nodePos) {
            double nodePosX = GetNodeValueDouble(nodePos, "X");
            double nodePosY = GetNodeValueDouble(nodePos, "Y");
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

        transformer->SetOnline(GetNodeValueInt(electricalProp, "IsOnline"));
        TransformerElectricalData data = transformer->GetElectricalData();
        data.name = electricalProp->first_node("Name")->value();
        data.primaryNominalVoltage = GetNodeValueDouble(electricalProp, "PrimaryNominalVoltage");
        data.primaryNominalVoltageUnit =
            (ElectricalUnit)GetAttributeValueInt(electricalProp, "PrimaryNominalVoltage", "UnitID");
        data.secondaryNominalVoltage = GetNodeValueDouble(electricalProp, "SecondaryNominalVoltage");
        data.secondaryNominalVoltageUnit =
            (ElectricalUnit)GetAttributeValueInt(electricalProp, "SecondaryNominalVoltage", "UnitID");
        data.nominalPower = GetNodeValueDouble(electricalProp, "NominalPower");
        data.nominalPowerUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "NominalPower", "UnitID");
        data.resistance = GetNodeValueDouble(electricalProp, "Resistance");
        data.resistanceUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "Resistance", "UnitID");
        data.indReactance = GetNodeValueDouble(electricalProp, "IndReactance");
        data.indReactanceUnit = (ElectricalUnit)GetAttributeValueInt(electricalProp, "IndReactance", "UnitID");
        data.connection = (TransformerConnection)GetNodeValueInt(electricalProp, "Connection");
        data.turnsRatio = GetNodeValueDouble(electricalProp, "TurnsRatio");
        data.phaseShift = GetNodeValueDouble(electricalProp, "PhaseShift");
        data.useTransformerPower = GetNodeValueInt(electricalProp, "UseTransfomerPower");

        auto fault = electricalProp->first_node("Fault");
        data.zeroResistance = GetNodeValueDouble(fault, "ZeroResistance");
        data.zeroIndReactance = GetNodeValueDouble(fault, "ZeroIndReactance");
        data.primaryGrndResistance = GetNodeValueDouble(fault, "PrimaryGrndResistance");
        data.primaryGrndReactance = GetNodeValueDouble(fault, "PrimaryGrndReactance");
        data.secondaryGrndResistance = GetNodeValueDouble(fault, "SecondaryGrndResistance");
        data.secondaryGrndReactance = GetNodeValueDouble(fault, "SecondaryGrndReactance");

        SwitchingData swData;
        auto switchingList = electricalProp->first_node("SwitchingList");
        if(!switchingList) return false;
        auto swNode = switchingList->first_node("Switching");
        while(swNode) {
            swData.swType.push_back((SwitchingType)GetNodeValueInt(swNode, "Type"));
            swData.swTime.push_back(GetNodeValueDouble(swNode, "Time"));
            swNode = swNode->next_sibling("Switching");
        }
        transformer->SetSwitchingData(swData);

        transformer->SetElectricaData(data);
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
        double posX = GetNodeValueDouble(position, "X");
        double posY = GetNodeValueDouble(position, "Y");
        auto size = cadPropNode->first_node("Size");
        double width = GetNodeValueDouble(size, "Width");
        double height = GetNodeValueDouble(size, "Height");
        double angle = GetNodeValueDouble(cadPropNode, "Angle");

        Text* text = new Text(wxPoint2DDouble(posX, posY));

        text->SetWidth(width);
        text->SetHeight(height);

        auto textProperties = textNode->first_node("TextProperties");
        if(!textProperties) return false;

        text->SetElementType((ElementType)GetNodeValueDouble(textProperties, "ElementType"));
        text->SetDataType((DataType)GetNodeValueDouble(textProperties, "DataType"));
        text->SetUnit((ElectricalUnit)GetNodeValueDouble(textProperties, "DataUnit"));
        text->SetDirection(GetNodeValueDouble(textProperties, "Direction"));
        text->SetDecimalPlaces(GetNodeValueDouble(textProperties, "DecimalPlaces"));

        text->SetElementNumber(GetNodeValueInt(textProperties, "ElementNumber"));
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
    rapidxml::file<> xmlFile(path.GetFullPath());
    doc.parse<0>(xmlFile.data());

    rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
    rapidxml::xml_attribute<>* ver = doc.allocate_attribute("version", "1.0");
    rapidxml::xml_attribute<>* encoding = doc.allocate_attribute("encoding", "utf-8");
    decl->append_attribute(ver);
    decl->append_attribute(encoding);
    doc.append_node(decl);

    rapidxml::xml_node<>* rootNode = doc.allocate_node(rapidxml::node_element, "Control");
    doc.append_node(rootNode);

    rapidxml::xml_node<>* projectNameNode = AppendNode(doc, rootNode, "Name");
    SetNodeValue(doc, projectNameNode, path.GetName());

    auto elementsNode = AppendNode(doc, rootNode, "ControlElements");
    SaveControlElements(doc, elementsNode);
    std::ofstream writeXML(path.GetFullPath());
    writeXML << doc;
    writeXML.close();
}

bool FileHanding::OpenControl(wxFileName path) { return false; }
void FileHanding::SaveControlElements(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementsNode)
{
    ControlElementContainer ctrlContainer;
    ctrlContainer.FillContainer(m_controlEditor);

    //{ Connection line
    auto cLinesNode = AppendNode(doc, elementsNode, "ConnectionList");
    auto connLineList = ctrlContainer.GetConnectionLineList();
    for(auto it = connLineList.begin(), itEnd = connLineList.end(); it != itEnd; ++it) {
        ConnectionLine* cLine = *it;
        auto cLineNode = AppendNode(doc, cLinesNode, "Connection");
        SetNodeAttribute(doc, cLineNode, "ID", cLine->GetID());

        // CAD properties
        auto cadProp = AppendNode(doc, cLineNode, "CADProperties");
        auto position1 = AppendNode(doc, cadProp, "Position1");
        auto posX1 = AppendNode(doc, position1, "X");
        SetNodeValue(doc, posX1, cLine->GetPointList()[0].m_x);
        auto posY1 = AppendNode(doc, position1, "Y");
        SetNodeValue(doc, posY1, cLine->GetPointList()[0].m_y);
        auto position2 = AppendNode(doc, cadProp, "Position2");
        auto posX2 = AppendNode(doc, position2, "X");
        SetNodeValue(doc, posX2, cLine->GetPointList()[5].m_x);
        auto posY2 = AppendNode(doc, position2, "Y");
        SetNodeValue(doc, posY2, cLine->GetPointList()[5].m_y);
        auto offset = AppendNode(doc, cadProp, "Offset");
        SetNodeValue(doc, offset, cLine->GetOffset());

        // Child list
        auto childsNode = AppendNode(doc, cLineNode, "ChildList");
        auto childList = cLine->GetLineChildList();
        for(auto itC = childList.begin(), itCEnd = childList.end(); itC != itCEnd; ++itC) {
            ConnectionLine* childLine = *itC;
            auto childNode = AppendNode(doc, childsNode, "Child");
            SetNodeAttribute(doc, childNode, "ID", childLine->GetID());
        }

        // Parent list
        auto parentsNode = AppendNode(doc, cLineNode, "ParentList");
        auto parentList = cLine->GetParentList();
        for(auto itP = parentList.begin(), itPEnd = parentList.end(); itP != itPEnd; ++itP) {
            Element* parent = *itP;
            auto parentNode = AppendNode(doc, parentsNode, "Parent");
            SetNodeAttribute(doc, parentNode, "ID", parent->GetID());
        }

        auto parentLine = AppendNode(doc, cLineNode, "ParentLine");
        if(cLine->GetParentLine()) {
            ConnectionLine* parent = cLine->GetParentLine();
            SetNodeAttribute(doc, parentLine, "ID", parent->GetID());
        } else {
            SetNodeAttribute(doc, parentLine, "ID", -1);
        }
    }  //}

    //{ Constant
    auto constsNode = AppendNode(doc, elementsNode, "ConstantList");
    auto constList = ctrlContainer.GetConstantList();
    for(auto it = constList.begin(), itEnd = constList.end(); it != itEnd; ++it) {
        Constant* constant = *it;
        auto constNode = AppendNode(doc, constsNode, "Constant");
        SetNodeAttribute(doc, constNode, "ID", constant->GetID());
        auto cadProp = AppendNode(doc, constNode, "CADProperties");
        auto position = AppendNode(doc, cadProp, "Position");
        auto posX = AppendNode(doc, position, "X");
        SetNodeValue(doc, posX, constant->GetPosition().m_x);
        auto posY = AppendNode(doc, position, "Y");
        SetNodeValue(doc, posY, constant->GetPosition().m_y);
        auto size = AppendNode(doc, cadProp, "Size");
        auto width = AppendNode(doc, size, "Width");
        SetNodeValue(doc, width, constant->GetWidth());
        auto height = AppendNode(doc, size, "Height");
        SetNodeValue(doc, height, constant->GetHeight());
        auto angle = AppendNode(doc, cadProp, "Angle");
        SetNodeValue(doc, angle, constant->GetAngle());

        // Nodes
        auto nodeList = AppendNode(doc, constNode, "NodeList");
        SaveControlNodes(doc, nodeList, constant->GetNodeList());

        // Control properties
        auto value = AppendNode(doc, constNode, "Value");
        SetNodeValue(doc, value, constant->GetValue());
    }  //}

    //{ Exponential
    auto expsNode = AppendNode(doc, elementsNode, "ExponentialList");
    auto expList = ctrlContainer.GetExponentialList();
    for(auto it = expList.begin(), itEnd = expList.end(); it != itEnd; ++it) {
        Exponential* exponential = *it;
        auto expNode = AppendNode(doc, expsNode, "Exponential");
        SetNodeAttribute(doc, expNode, "ID", exponential->GetID());
        auto cadProp = AppendNode(doc, expNode, "CADProperties");
        auto position = AppendNode(doc, cadProp, "Position");
        auto posX = AppendNode(doc, position, "X");
        SetNodeValue(doc, posX, exponential->GetPosition().m_x);
        auto posY = AppendNode(doc, position, "Y");
        SetNodeValue(doc, posY, exponential->GetPosition().m_y);
        auto size = AppendNode(doc, cadProp, "Size");
        auto width = AppendNode(doc, size, "Width");
        SetNodeValue(doc, width, exponential->GetWidth());
        auto height = AppendNode(doc, size, "Height");
        SetNodeValue(doc, height, exponential->GetHeight());
        auto angle = AppendNode(doc, cadProp, "Angle");
        SetNodeValue(doc, angle, exponential->GetAngle());

        // Nodes
        auto nodeList = AppendNode(doc, expNode, "NodeList");
        SaveControlNodes(doc, nodeList, exponential->GetNodeList());

        // Control properties
        double a, b;
        exponential->GetValues(a, b);
        auto value = AppendNode(doc, expNode, "Value");
        auto aValue = AppendNode(doc, value, "A");
        SetNodeValue(doc, aValue, a);
        auto bValue = AppendNode(doc, value, "B");
        SetNodeValue(doc, bValue, b);
    }  //}

    //{ Gain
    auto gainsNode = AppendNode(doc, elementsNode, "GainList");
    auto gainList = ctrlContainer.GetGainList();
    for(auto it = gainList.begin(), itEnd = gainList.end(); it != itEnd; ++it) {
        Gain* gain = *it;
        auto gainNode = AppendNode(doc, gainsNode, "Gain");
        SetNodeAttribute(doc, gainNode, "ID", gain->GetID());
        auto cadProp = AppendNode(doc, gainNode, "CADProperties");
        auto position = AppendNode(doc, cadProp, "Position");
        auto posX = AppendNode(doc, position, "X");
        SetNodeValue(doc, posX, gain->GetPosition().m_x);
        auto posY = AppendNode(doc, position, "Y");
        SetNodeValue(doc, posY, gain->GetPosition().m_y);
        auto size = AppendNode(doc, cadProp, "Size");
        auto width = AppendNode(doc, size, "Width");
        SetNodeValue(doc, width, gain->GetWidth());
        auto height = AppendNode(doc, size, "Height");
        SetNodeValue(doc, height, gain->GetHeight());
        auto angle = AppendNode(doc, cadProp, "Angle");
        SetNodeValue(doc, angle, gain->GetAngle());

        // Nodes
        auto nodeList = AppendNode(doc, gainNode, "NodeList");
        SaveControlNodes(doc, nodeList, gain->GetNodeList());

        // Control properties
        auto value = AppendNode(doc, gainNode, "Value");
        SetNodeValue(doc, value, gain->GetValue());
    }  //}
}

void FileHanding::SaveControlNodes(rapidxml::xml_document<>& doc,
                                   rapidxml::xml_node<>* nodesN,
                                   std::vector<Node*> nodeList)
{
    for(auto it = nodeList.begin(), itEnd = nodeList.end(); it != itEnd; ++it) {
        Node* node = *it;
        auto nodeN = AppendNode(doc, nodesN, "Node");
        auto nodePosition = AppendNode(doc, nodeN, "Position");
        auto posNodeX = AppendNode(doc, nodePosition, "X");
        SetNodeValue(doc, posNodeX, node->GetPosition().m_x);
        auto posNodeY = AppendNode(doc, nodePosition, "Y");
        SetNodeValue(doc, posNodeY, node->GetPosition().m_y);
        auto angle = AppendNode(doc, nodeN, "Angle");
        SetNodeValue(doc, angle, node->GetAngle());
        auto isConnected = AppendNode(doc, nodeN, "IsConnected");
        SetNodeValue(doc, isConnected, node->IsConnected());
        auto nodeType = AppendNode(doc, nodeN, "Type");
        SetNodeValue(doc, nodeType, node->GetNodeType());
    }
}

void FileHanding::SaveControlChildren(rapidxml::xml_document<>& doc,
                                      rapidxml::xml_node<>* childrenNode,
                                      std::vector<Node*> childList)

    rapidxml::xml_node<>* FileHanding::AppendNode(rapidxml::xml_document<>& doc,
                                                  rapidxml::xml_node<>* parentNode,
                                                  const char* name,
                                                  rapidxml::node_type nodeType)
{
    rapidxml::xml_node<>* node = doc.allocate_node(nodeType, name);
    parentNode->append_node(node);
    return node;
}

void FileHanding::SetNodeValue(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, wxString value)
{
    node->value(doc.allocate_string(value.mb_str()));
}

void FileHanding::SetNodeValue(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, int value)
{
    node->value(doc.allocate_string(wxString::Format("%d", value)));
}

void FileHanding::SetNodeValue(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, double value)
{
    node->value(doc.allocate_string(wxString::FromCDouble(value, 13).mb_str()));
}

void FileHanding::SetNodeAttribute(rapidxml::xml_document<>& doc,
                                   rapidxml::xml_node<>* node,
                                   const char* atrName,
                                   wxString value)
{
    node->append_attribute(doc.allocate_attribute(atrName, doc.allocate_string(value.mb_str())));
}

void FileHanding::SetNodeAttribute(rapidxml::xml_document<>& doc,
                                   rapidxml::xml_node<>* node,
                                   const char* atrName,
                                   int value)
{
    node->append_attribute(doc.allocate_attribute(atrName, doc.allocate_string(wxString::Format("%d", value))));
}

void FileHanding::SetNodeAttribute(rapidxml::xml_document<>& doc,
                                   rapidxml::xml_node<>* node,
                                   const char* atrName,
                                   double value)
{
    node->append_attribute(
        doc.allocate_attribute(atrName, doc.allocate_string(wxString::FromCDouble(value, 13).mb_str())));
}

double FileHanding::GetNodeValueDouble(rapidxml::xml_node<>* parent, const char* nodeName)
{
    double dValue = 0.0;
    if(parent) {
        auto node = parent->first_node(nodeName);
        if(node) wxString(node->value()).ToCDouble(&dValue);
    }
    return dValue;
}

int FileHanding::GetNodeValueInt(rapidxml::xml_node<>* parent, const char* nodeName)
{
    long iValue = -1;
    if(parent) {
        auto node = parent->first_node(nodeName);
        if(node) wxString(node->value()).ToCLong(&iValue);
    }
    return (int)iValue;
}

int FileHanding::GetAttributeValueInt(rapidxml::xml_node<>* parent, const char* nodeName, const char* atrName)
{
    long iValue = -1;
    if(parent) {
        auto node = parent->first_node(nodeName);
        if(node) {
            auto atr = node->first_attribute(atrName);
            if(atr) wxString(atr->value()).ToCLong(&iValue);
        }
    }
    return (int)iValue;
}
