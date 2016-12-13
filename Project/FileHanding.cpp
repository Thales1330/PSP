#include "FileHanding.h"

FileHanding::~FileHanding() {}

FileHanding::FileHanding(Workspace* workspace) { m_workspace = workspace; }

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
    } //}

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
        auto parentID = AppendNode(doc, cadProp, "ParentID");
        Bus* parent = (Bus*)capacitor->GetParentList()[0];
        if(parent) SetNodeValue(doc, parentID, parent->GetEletricalData().number);

        CapacitorElectricalData data = capacitor->GetElectricalData();
        auto electricalProp = AppendNode(doc, capacitorNode, "ElectricalProperties");
        auto isOnline = AppendNode(doc, electricalProp, "IsOnline");
        SetNodeValue(doc, isOnline, capacitor->IsOnline());
        auto name = AppendNode(doc, capacitorNode, "Name");
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
    } //}

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
        auto parentID = AppendNode(doc, cadProp, "ParentID");
        SetNodeValue(doc, parentID, ((Bus*)indMotor->GetParentList()[0])->GetEletricalData().number);

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
    } //}

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
        Bus* parent = (Bus*)inductor->GetParentList()[0];
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
    } //}

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
            Bus* parent = (Bus*)line->GetParentList()[j];
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
    } //}

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
        Bus* parent = (Bus*)load->GetParentList()[0];
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
    } //}

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
        Bus* parent = (Bus*)syncGenerator->GetParentList()[0];
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
    } //}

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
        Bus* parent = (Bus*)syncMotor->GetParentList()[0];
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
    } //}

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
            Bus* parent = (Bus*)transfomer->GetParentList()[j];
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
    } //}
    
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

void FileHanding::OpenProject(wxFileName path) {}

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
