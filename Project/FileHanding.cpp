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
        auto nominalVoltageUnit = AppendNode(doc, electricalProp, "NominalVoltageUnit");
        SetNodeValue(doc, nominalVoltageUnit, data.nominalVoltageUnit);
        auto isVoltageControlled = AppendNode(doc, electricalProp, "IsVoltageControlled");
        SetNodeValue(doc, isVoltageControlled, data.isVoltageControlled);
        auto controlledVoltage = AppendNode(doc, electricalProp, "ControlledVoltage");
        SetNodeValue(doc, controlledVoltage, data.controlledVoltage);
        auto controlledVoltageUnitChoice = AppendNode(doc, electricalProp, "ControlledVoltageUnitChoice");
        SetNodeValue(doc, controlledVoltageUnitChoice, data.controlledVoltageUnitChoice);
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
        auto reactivePowerUnit = AppendNode(doc, electricalProp, "ReactivePowerUnit");
        SetNodeValue(doc, reactivePowerUnit, data.reactivePowerUnit);

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
        auto activePowerUnit = AppendNode(doc, electricalProp, "ActivePowerUnit");
        SetNodeValue(doc, activePowerUnit, data.activePowerUnit);
        auto reactivePower = AppendNode(doc, electricalProp, "ReactivePower");
        SetNodeValue(doc, reactivePower, data.reactivePower);
        auto reactivePowerUnit = AppendNode(doc, electricalProp, "ReactivePowerUnit");
        SetNodeValue(doc, reactivePowerUnit, data.reactivePowerUnit);
    } //}

    //{ Inductor
    auto inductorsNode = AppendNode(doc, elementsNode, "InductorList");
    auto inductorList = allElements.GetInductorList();
    for(int i = 0; i < (int)inductorList.size(); i++) {
        Inductor* inductor = inductorList[i];
        auto inductorNode = AppendNode(doc, inductorsNode, "Inductor");
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
        auto reactivePowerUnit = AppendNode(doc, electricalProp, "ReactivePowerUnit");
        SetNodeValue(doc, reactivePowerUnit, data.reactivePowerUnit);

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
        auto nominalVoltageUnit = AppendNode(doc, electricalProp, "NominalVoltageUnit");
        SetNodeValue(doc, nominalVoltageUnit, data.nominalVoltageUnit);
        auto nominalPower = AppendNode(doc, electricalProp, "NominalPower");
        SetNodeValue(doc, nominalPower, data.nominalPower);
        auto nominalPowerUnit = AppendNode(doc, electricalProp, "NominalPowerUnit");
        SetNodeValue(doc, nominalPowerUnit, data.nominalPowerUnit);
        auto resistance = AppendNode(doc, electricalProp, "Resistance");
        SetNodeValue(doc, resistance, data.resistance);
        auto resistanceUnit = AppendNode(doc, electricalProp, "ResistanceUnit");
        SetNodeValue(doc, resistanceUnit, data.resistanceUnit);
        auto indReactance = AppendNode(doc, electricalProp, "IndReactance");
        SetNodeValue(doc, indReactance, data.indReactance);
        auto indReactanceUnit = AppendNode(doc, electricalProp, "IndReactanceUnit");
        SetNodeValue(doc, indReactanceUnit, data.indReactanceUnit);
        auto capSusceptance = AppendNode(doc, electricalProp, "CapSusceptance");
        SetNodeValue(doc, capSusceptance, data.capSusceptance);
        auto capSusceptanceUnit = AppendNode(doc, electricalProp, "CapSusceptanceUnit");
        SetNodeValue(doc, capSusceptanceUnit, data.capSusceptanceUnit);
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
        auto activePowerUnit = AppendNode(doc, electricalProp, "ActivePowerUnit");
        SetNodeValue(doc, activePowerUnit, data.activePowerUnit);
        auto reactivePower = AppendNode(doc, electricalProp, "ReactivePower");
        SetNodeValue(doc, reactivePower, data.reactivePower);
        auto reactivePowerUnit = AppendNode(doc, electricalProp, "ReactivePowerUnit");
        SetNodeValue(doc, reactivePowerUnit, data.reactivePowerUnit);
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
    auto syncGeneratorList = allElements.GetLoadList();
    for(int i = 0; i < (int)syncGeneratorList.size(); i++) {
        SyncGenerator* syncGenerator = syncGeneratorList[i];
        auto syncGeneratorNode = AppendNode(doc, syncGeneratorsNode, "SyncGenerator");
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
        
        /*wxString name = "";
        double nominalPower = 100.0;
        ElectricalUnit nominalPowerUnit = UNIT_MVA;
        double nominalVoltage = 13.8;
        ElectricalUnit nominalVoltageUnit = UNIT_kV;
        double activePower = 100.0;
        ElectricalUnit activePowerUnit = UNIT_MW;
        double reactivePower = 0.0;
        ElectricalUnit reactivePowerUnit = UNIT_MVAr;
        bool haveMaxReactive = false;
        double maxReactive = 9999.0;
        ElectricalUnit maxReactiveUnit = UNIT_MVAr;
        bool haveMinReactive = false;
        double minReactive = -9999.0;
        ElectricalUnit minReactiveUnit = UNIT_MVAr;
        bool useMachineBase = false;

        // Fault
        double positiveResistance = 0.0;
        double positiveReactance = 1.0;
        double negativeResistance = 0.0;
        double negativeReactance = 1.0;
        double zeroResistance = 0.0;
        double zeroReactance = 1.0;
        double groundResistance = 0.0;
        double groundReactance = 0.0;
        bool groundNeutral = true;
        // p.u. fault data
        std::complex<double> faultCurrent[3] = { std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 0.0),
            std::complex<double>(0.0, 0.0) };

        // Stability
        bool plotSyncMachine = false;
        double inertia = 0.0;
        double damping = 0.0;
        bool useAVR = false;
        bool useSpeedGovernor = false;

        double armResistance = 0.0;
        double potierReactance = 0.0;
        double satFactor = 0.0;

        double syncXd = 0.0;
        double syncXq = 0.0;
        double transXd = 1.0;
        double transXq = 0.0;
        double transTd0 = 0.0;
        double transTq0 = 0.0;
        double subXd = 0.0;
        double subXq = 0.0;
        double subTd0 = 0.0;
        double subTq0 = 0.0;*/

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
