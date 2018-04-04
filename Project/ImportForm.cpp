/*
 *  Copyright (C) 2018  Thales Lima Oliveira <thales@ufu.br>
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

#include "Bus.h"
#include "Capacitor.h"
#include "ImportForm.h"
#include "IndMotor.h"
#include "Inductor.h"
#include "Line.h"
#include "Load.h"
#include "PropertiesData.h"
#include "SyncGenerator.h"
#include "SyncMotor.h"
#include "Transformer.h"
#include "Workspace.h"

ImportForm::ImportForm(wxWindow* parent, Workspace* workspace) : ImportFormBase(parent)
{
    SetInitialSize();

    m_parent = parent;
    m_workspace = workspace;
}

ImportForm::~ImportForm() {}

void ImportForm::OnButtonCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
    if(m_workspace) delete m_workspace;
}

void ImportForm::OnButtonOKClick(wxCommandEvent& event)
{
    if(ImportSelectedFiles())
        EndModal(wxID_OK);
    else {
        // Error message
        wxMessageDialog msgDialog(this, _("It was not possible to import the selected files."), _("Error"),
                                  wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
    }
}

bool ImportForm::ImportSelectedFiles()
{
    ParseAnarede parseAnarede(m_filePickerANAREDELST->GetFileName(), m_filePickerANAREDEPWF->GetFileName());
    if(!parseAnarede.Parse()) return false;
    double scale = 1.25;

    double mvaBasePower = parseAnarede.GetMVAPowerBase();
    auto simProp = m_workspace->GetProperties()->GetSimulationPropertiesData();
    simProp.basePower = mvaBasePower;
    m_workspace->GetProperties()->SetSimulationPropertiesData(simProp);

    std::vector<Element*> elementList;

    std::vector<Bus*> busList;
    std::vector<SyncGenerator*> syncGeneratorList;
    std::vector<SyncMotor*> syncMotorList;
    std::vector<Load*> loadList;
    std::vector<Inductor*> indList;
    std::vector<Capacitor*> capList;
    std::vector<IndMotor*> indMotorList;
    std::vector<Transformer*> transformerList;
    std::vector<Line*> lineList;

    auto components = parseAnarede.GetComponents();
    for(auto it = components.begin(), itEnd = components.end(); it != itEnd; ++it) {
        switch((*it)->type) {
            case ANA_BUS: {
                Bus* bus = new Bus(wxPoint2DDouble((*it)->position.m_x * scale, (*it)->position.m_y * scale));
                bus->SetID((*it)->id);
                bus->SetWidth((*it)->length * scale);
                bus->StartMove(bus->GetPosition());
                if((*it)->rotationID == 0) {
                    bus->Move(bus->GetPosition() + wxPoint2DDouble(bus->GetWidth() / 2, bus->GetHeight() / 2));
                } else if((*it)->rotationID == 1) {
                    for(int i = 0; i < (*it)->rotationID * 2; ++i) bus->Rotate();
                    bus->Move(bus->GetPosition() + wxPoint2DDouble(-bus->GetHeight() / 2, bus->GetWidth() / 2));
                } else if((*it)->rotationID == 2) {
                    for(int i = 0; i < (*it)->rotationID * 2; ++i) bus->Rotate();
                    bus->Move(bus->GetPosition() + wxPoint2DDouble(-bus->GetWidth() / 2, -bus->GetHeight() / 2));
                } else if((*it)->rotationID == 3) {
                    for(int i = 0; i < (*it)->rotationID * 2; ++i) bus->Rotate();
                    bus->Move(bus->GetPosition() + wxPoint2DDouble(-bus->GetHeight() / 2, -bus->GetWidth() / 2));
                }

                // Electrical data
                auto data = bus->GetElectricalData();
                ParseAnarede::BusData* busData = parseAnarede.GetBusDataFromID((*it)->electricalID);
                if(busData) {
                    data.number = busData->id;
                    data.name = busData->busName;
                    switch(busData->type) {
                        case 0:
                        case 3: {
                            data.isVoltageControlled = false;
                            data.slackBus = false;
                        } break;
                        case 1: {
                            data.isVoltageControlled = true;
                            data.slackBus = false;
                        } break;
                        case 2: {
                            data.isVoltageControlled = true;
                            data.slackBus = true;
                        } break;
                        default: {
                            return false;
                        } break;
                    }
                    data.voltage = std::complex<double>(busData->voltage * std::cos(wxDegToRad(busData->angle)),
                                                        busData->voltage * std::sin(wxDegToRad(busData->angle)));
                    data.controlledVoltage = busData->voltage;
                } else
                    return false;

                bus->SetElectricalData(data);
                busList.push_back(bus);
            } break;
            case ANA_GENERATOR: {
                // Find parent bus
                Bus* parentBus = GetBusFromID(busList, (*it)->busConnectionID[0].first);
                wxPoint2DDouble nodePos =
                    parseAnarede.GetNodePositionFromID(parentBus, scale, (*it)->busConnectionNode[0].second);

                ParseAnarede::BusData* busData = parseAnarede.GetBusDataFromID((*it)->busConnectionID[0].second);
                bool isMotor = false;
                if(busData->genPower.real() <= 0.0) isMotor = true;
                Machines* machine = NULL;

                if(!isMotor) {
                    SyncGenerator* syncGenerator = new SyncGenerator();

                    auto data = syncGenerator->GetElectricalData();
                    data.name = _("Generator") + wxT(" (") + busData->busName + wxT(")");
                    data.activePower = busData->genPower.real();
                    data.reactivePower = busData->genPower.imag();
                    data.minReactive = busData->minReactivePower;
                    data.maxReactive = busData->maxReactivePower;

                    syncGenerator->SetElectricalData(data);

                    machine = syncGenerator;
                } else {
                    SyncMotor* syncMotor = new SyncMotor();

                    auto data = syncMotor->GetElectricalData();
                    data.name = _("Synchronous compensator") + wxT(" (") + busData->busName + wxT(")");
                    data.activePower = busData->genPower.real() == 0.0 ? 0.0 : -busData->genPower.real();
                    data.reactivePower = busData->genPower.imag();
                    data.minReactive = busData->minReactivePower;
                    data.maxReactive = busData->maxReactivePower;

                    syncMotor->SetElectricalData(data);

                    machine = syncMotor;
                }

                if(machine) {
                    machine->SetID((*it)->id);
                    machine->AddParent(parentBus, nodePos);

                    machine->StartMove(machine->GetPosition());
                    machine->Move(wxPoint2DDouble((*it)->position.m_x * scale, (*it)->position.m_y * scale));

                    for(int i = 0; i < 2; ++i) machine->Rotate(false);  // Set to ANAREDE default rotation
                    for(int i = 0; i < (*it)->rotationID * 2; ++i) machine->Rotate();

                    if(!isMotor)
                        syncGeneratorList.push_back(static_cast<SyncGenerator*>(machine));
                    else
                        syncMotorList.push_back(static_cast<SyncMotor*>(machine));
                }
            } break;
            case ANA_IND_GENERATOR: {
                // Same as ANA_GENERATOR, but with a transformer connecting the bus
                Bus* parentBus = GetBusFromID(busList, (*it)->busConnectionID[0].first);
                wxPoint2DDouble nodePos =
                    parseAnarede.GetNodePositionFromID(parentBus, scale, (*it)->busConnectionNode[0].second);

                wxPoint2DDouble genPosition((*it)->position.m_x * scale,
                                            (*it)->position.m_y * scale);  // Element position.
                // Calculate the new generator position:
                wxPoint2DDouble offset;
                double angle = wxRadToDeg(std::atan2(genPosition.m_y - nodePos.m_y, genPosition.m_x - nodePos.m_x));

                int tRot = 0;  // Number of transformer rotations
                int bRot = 0;  // Number of bus rotations
                if(angle >= -45 && angle <= 45) {
                    offset = wxPoint2DDouble(75.0, 0.0);
                    tRot = 4;
                    bRot = 2;
                } else if(angle < -45 && angle > -135) {
                    offset = wxPoint2DDouble(0.0, -75.0);
                    tRot = 2;
                } else if(angle <= -135 || angle >= 135) {
                    offset = wxPoint2DDouble(-75.0, 0.0);
                    bRot = 2;
                } else {
                    offset = wxPoint2DDouble(0.0, 75.0);
                    tRot = 6;
                }

                // Auxiliary bus
                Bus* auxBus = new Bus(genPosition);
                for(int i = 0; i < bRot; ++i) auxBus->Rotate();
                // Transformer
                Transformer* transformer = new Transformer();
                transformer->AddParent(auxBus, auxBus->GetPosition());
                transformer->AddParent(parentBus, nodePos);
                for(int i = 0; i < tRot; ++i) transformer->Rotate();
                transformer->StartMove(transformer->GetPosition());
                transformer->Move(genPosition - offset);

                SyncGenerator* syncGenerator = new SyncGenerator();
                syncGenerator->SetID((*it)->id);
                syncGenerator->AddParent(auxBus, auxBus->GetPosition());

                syncGenerator->StartMove(syncGenerator->GetPosition());
                syncGenerator->Move(genPosition + offset);

                for(int i = 0; i < 2; ++i) syncGenerator->Rotate(false);  // Set to ANAREDE default rotation
                for(int i = 0; i < (*it)->rotationID * 2; ++i) syncGenerator->Rotate();

                syncGeneratorList.push_back(syncGenerator);

                busList.push_back(auxBus);
                transformerList.push_back(transformer);
                syncGeneratorList.push_back(syncGenerator);
            } break;
            case ANA_LOAD:
            case ANA_IND_LOAD: {
                Bus* parentBus = GetBusFromID(busList, (*it)->busConnectionID[0].first);
                wxPoint2DDouble nodePos =
                    parseAnarede.GetNodePositionFromID(parentBus, scale, (*it)->busConnectionNode[0].second);

                Load* load = new Load();
                load->SetID((*it)->id);
                load->AddParent(parentBus, nodePos);

                load->StartMove(load->GetPosition());
                load->Move(wxPoint2DDouble((*it)->position.m_x * scale, (*it)->position.m_y * scale));

                for(int i = 0; i < (*it)->rotationID * 2; ++i) load->Rotate();

                loadList.push_back(load);
            } break;
            case ANA_SHUNT:
            case ANA_IND_SHUNT: {
                Bus* parentBus = GetBusFromID(busList, (*it)->busConnectionID[0].first);
                wxPoint2DDouble nodePos =
                    parseAnarede.GetNodePositionFromID(parentBus, scale, (*it)->busConnectionNode[0].second);

                ParseAnarede::BusData* busData = parseAnarede.GetBusDataFromID((*it)->busConnectionID[0].second);
                bool isInductor = false;
                if(busData->shuntReactive <= 0.0) isInductor = true;
                Shunt* shuntElement = NULL;
                if(!isInductor) {
                    Capacitor* cap = new Capacitor();
                    auto data = cap->GetElectricalData();
                    data.name = _("Capacitor") + wxT("(") + busData->busName + wxT(")");
                    data.reactivePower = busData->shuntReactive;
                    cap->SetElectricalData(data);
                    shuntElement = cap;
                } else {
                    Inductor* ind = new Inductor();
                    auto data = ind->GetElectricalData();
                    data.name = _("Inductor") + wxT("(") + busData->busName + wxT(")");
                    data.reactivePower = -busData->shuntReactive;
                    ind->SetElectricalData(data);
                    shuntElement = ind;
                }

                if(shuntElement) {
                    shuntElement->SetID((*it)->id);
                    shuntElement->AddParent(parentBus, nodePos);

                    shuntElement->StartMove(shuntElement->GetPosition());
                    // Offset (ind->GetHeight() / 2 + 10) to adequate the y coordinate
                    shuntElement->Move(wxPoint2DDouble(
                        (*it)->position.m_x * scale, (*it)->position.m_y * scale + shuntElement->GetHeight() / 2 + 10));
                    if((*it)->rotationID != 0) {
                        // Remove offset in position
                        shuntElement->Move(wxPoint2DDouble((*it)->position.m_x * scale, (*it)->position.m_y * scale));
                        // Get the rotated point ralated to the offset
                        wxPoint2DDouble movePt = shuntElement->RotateAtPosition(
                            shuntElement->GetPosition() + wxPoint2DDouble(0, shuntElement->GetHeight() / 2 + 10),
                            (*it)->rotationID * 90.0, true);
                        shuntElement->Move(movePt);

                        for(int i = 0; i < (*it)->rotationID * 2; ++i) shuntElement->Rotate();
                    }

                    if(!isInductor)
                        capList.push_back(static_cast<Capacitor*>(shuntElement));
                    else
                        indList.push_back(static_cast<Inductor*>(shuntElement));
                }
            } break;
            case ANA_MIT: {
                // Find parent bus
                Bus* parentBus = GetBusFromID(busList, (*it)->busConnectionID[0].first);
                wxPoint2DDouble nodePos =
                    parseAnarede.GetNodePositionFromID(parentBus, scale, (*it)->busConnectionNode[0].second);

                IndMotor* indMotor = new IndMotor();
                indMotor->SetID((*it)->id);
                indMotor->AddParent(parentBus, nodePos);

                indMotor->StartMove(indMotor->GetPosition());
                indMotor->Move(wxPoint2DDouble((*it)->position.m_x * scale, (*it)->position.m_y * scale));

                for(int i = 0; i < 2; ++i) indMotor->Rotate(false);  // Set to ANAREDE default rotation
                for(int i = 0; i < (*it)->rotationID * 2; ++i) indMotor->Rotate();

                indMotorList.push_back(indMotor);
            } break;
            case ANA_TRANSFORMER: {
                Bus* parentBus1 = GetBusFromID(busList, (*it)->busConnectionID[0].first);
                Bus* parentBus2 = GetBusFromID(busList, (*it)->busConnectionID[1].first);
                wxPoint2DDouble nodePos1 =
                    parseAnarede.GetNodePositionFromID(parentBus1, scale, (*it)->busConnectionNode[0].second);
                wxPoint2DDouble nodePos2 =
                    parseAnarede.GetNodePositionFromID(parentBus2, scale, (*it)->busConnectionNode[1].second);

                Transformer* transformer = new Transformer();
                transformer->SetID((*it)->id);
                transformer->AddParent(parentBus1, nodePos1);
                transformer->AddParent(parentBus2, nodePos2);

                transformer->StartMove(transformer->GetPosition());
                transformer->Move(wxPoint2DDouble((*it)->position.m_x * scale, (*it)->position.m_y * scale));

                for(int i = 0; i < 2; ++i) transformer->Rotate();  // Set to ANAREDE default rotation
                for(int i = 0; i < (*it)->rotationID * 2; ++i) transformer->Rotate();

                transformerList.push_back(transformer);
            } break;
            default: {
            } break;
        }
    }

    auto powerLines = parseAnarede.GetLines();
    for(auto it = powerLines.begin(), itEnd = powerLines.end(); it != itEnd; ++it) {
        if((*it)->type == ANA_LINE) {
            Bus* parentBus1 = GetBusFromID(busList, (*it)->busConnectionID[0].first);
            Bus* parentBus2 = GetBusFromID(busList, (*it)->busConnectionID[1].first);
            wxPoint2DDouble nodePos1 =
                parseAnarede.GetNodePositionFromID(parentBus1, scale, (*it)->busConnectionNode[0].second);
            wxPoint2DDouble nodePos2 =
                parseAnarede.GetNodePositionFromID(parentBus2, scale, (*it)->busConnectionNode[1].second);

            Line* line = new Line();
            line->SetID((*it)->id);
            line->AddParent(parentBus1, nodePos1);
            for(unsigned int i = 0; i < (*it)->nodesPosition.size(); ++i)
                line->AddPoint(
                    wxPoint2DDouble((*it)->nodesPosition[i].m_x * scale, (*it)->nodesPosition[i].m_y * scale));
            line->AddParent(parentBus2, nodePos2);

            lineList.push_back(line);
        }
    }

    for(auto it = busList.begin(), itEnd = busList.end(); it != itEnd; ++it) elementList.push_back(*it);
    for(auto it = syncGeneratorList.begin(), itEnd = syncGeneratorList.end(); it != itEnd; ++it)
        elementList.push_back(*it);
    for(auto it = syncMotorList.begin(), itEnd = syncMotorList.end(); it != itEnd; ++it) elementList.push_back(*it);
    for(auto it = loadList.begin(), itEnd = loadList.end(); it != itEnd; ++it) elementList.push_back(*it);
    for(auto it = indList.begin(), itEnd = indList.end(); it != itEnd; ++it) elementList.push_back(*it);
    for(auto it = capList.begin(), itEnd = capList.end(); it != itEnd; ++it) elementList.push_back(*it);
    for(auto it = indMotorList.begin(), itEnd = indMotorList.end(); it != itEnd; ++it) elementList.push_back(*it);
    for(auto it = transformerList.begin(), itEnd = transformerList.end(); it != itEnd; ++it) elementList.push_back(*it);
    for(auto it = lineList.begin(), itEnd = lineList.end(); it != itEnd; ++it) elementList.push_back(*it);

    m_workspace->SetElementList(elementList);
    m_workspace->SetName(parseAnarede.GetProjectName());
    return true;
}

Bus* ImportForm::GetBusFromID(std::vector<Bus*> busList, int id)
{
    for(auto it = busList.begin(), itEnd = busList.end(); it != itEnd; ++it) {
        if((*it)->GetID() == id) return *it;
    }
    return NULL;
}

ParseAnarede::ParseAnarede(wxFileName lstFile, wxFileName pwfFile)
{
    m_lstFile = lstFile;
    m_pwfFile = pwfFile;
}

bool ParseAnarede::Parse()
{
    wxTextFile lst(m_lstFile.GetFullPath());
    wxTextFile pwf(m_pwfFile.GetFullPath());
    if(!lst.Open()) return false;
    if(!pwf.Open()) return false;

    // Parse LST file
    for(wxString line = lst.GetFirstLine(); !lst.Eof(); line = lst.GetNextLine()) {
        // Current line
        switch(static_cast<char>(line[0])) {
            case 'C': {  // Component
                int parsePosition = 1;

                Component* component = new Component();

                component->id = wxAtoi(GetLSTLineNextValue(line, parsePosition));
                // Check if is component type is valid
                if(StrToElementType(GetLSTLineNextValue(line, parsePosition), component->type)) {
                    if(component->type == ANA_BUS) {
                        if(!GetLenghtAndRotationFromBusCode(GetLSTLineNextValue(line, parsePosition), component->length,
                                                            component->rotationID))
                            return false;
                    } else {
                        component->rotationID = wxAtoi(GetLSTLineNextValue(line, parsePosition));
                    }
                    parsePosition += 2;  // Jump to position
                    if(!GetLSTLineNextValue(line, parsePosition).ToCDouble(&component->position.m_x)) return false;
                    if(!GetLSTLineNextValue(line, parsePosition).ToCDouble(&component->position.m_y)) return false;
                    parsePosition += 8;  // Jump to electrical ID
                    component->electricalID = wxAtoi(GetLSTLineNextValue(line, parsePosition));

                    // Bus connection IDs
                    if(component->type != ANA_BUS) {
                        int fromBus = wxAtoi(GetLSTLineNextValue(line, parsePosition));  // Origin bus
                        int toBus = 0;
                        // If the component is a transformer, parse the next value (toBus)
                        if(component->type == ANA_TRANSFORMER) {
                            toBus = wxAtoi(GetLSTLineNextValue(line, parsePosition));  // Destiny bus
                        }
                        // Iterate through the already parsed components (the buses is saved first)
                        for(auto it = m_components.begin(), itEnd = m_components.end(); it != itEnd; ++it) {
                            if((*it)->type == ANA_BUS) {
                                if(fromBus == (*it)->electricalID) {
                                    component->busConnectionID[0] = std::make_pair((*it)->id, fromBus);
                                } else if(toBus == (*it)->electricalID) {
                                    component->busConnectionID[1] = std::make_pair((*it)->id, toBus);
                                }
                            }
                        }
                    }
                    m_components.push_back(component);
                }
            } break;
            case 'L': {  // Line
                int parsePosition = 1;

                PowerLine* pLine = new PowerLine();

                pLine->id = wxAtoi(GetLSTLineNextValue(line, parsePosition));
                pLine->type = static_cast<ElementTypeAnarede>(wxAtoi(GetLSTLineNextValue(line, parsePosition)));
                parsePosition += 4;  // Jump to from bus

                int fromBus = wxAtoi(GetLSTLineNextValue(line, parsePosition));  // Origin bus
                int toBus = wxAtoi(GetLSTLineNextValue(line, parsePosition));    // Destiny bus

                // Iterate through the already parsed components
                for(auto it = m_components.begin(), itEnd = m_components.end(); it != itEnd; ++it) {
                    if((*it)->type == ANA_BUS) {
                        if(fromBus == (*it)->id) {
                            pLine->busConnectionID[0] = std::make_pair((*it)->id, (*it)->electricalID);
                        } else if(toBus == (*it)->id) {
                            pLine->busConnectionID[1] = std::make_pair((*it)->id, (*it)->electricalID);
                        }
                    }
                }
                pLine->electricalID = wxAtoi(GetLSTLineNextValue(line, parsePosition));

                m_lines.push_back(pLine);
            } break;
            case 'U': {  // Connection
                // The connection data depends on the component type, so this data definition must be more generic.
                int parsePosition = 1;

                int id = wxAtoi(GetLSTLineNextValue(line, parsePosition));
                (void)id;            // Unused id.
                parsePosition += 5;  // Jump to next relevant data
                int data1 = wxAtoi(GetLSTLineNextValue(line, parsePosition));
                int data2 = wxAtoi(GetLSTLineNextValue(line, parsePosition));
                int data3 = wxAtoi(GetLSTLineNextValue(line, parsePosition));
                int data4 = wxAtoi(GetLSTLineNextValue(line, parsePosition));
                int data5 = wxAtoi(GetLSTLineNextValue(line, parsePosition));
                int data6 = wxAtoi(GetLSTLineNextValue(line, parsePosition));

                // If data5 differs from -1, the connection is power line data with data5's value as its ID, otherwise
                // data1 is the ID of a component.
                if(data5 != -1) {
                    // Iterate through parsed lines
                    for(auto it = m_lines.begin(), itEnd = m_lines.end(); it != itEnd; ++it) {
                        if(data5 == (*it)->id) {
                            (*it)->busConnectionNode[0] = std::make_pair(data1, data2);
                            (*it)->busConnectionNode[1] = std::make_pair(data3, data4);
                            for(int i = 0; i < data6; ++i) {
                                wxPoint2DDouble nodePt;
                                if(!GetLSTLineNextValue(line, parsePosition).ToCDouble(&nodePt.m_x)) return false;
                                if(!GetLSTLineNextValue(line, parsePosition).ToCDouble(&nodePt.m_y)) return false;
                                (*it)->nodesPosition.push_back(nodePt);
                            }
                        }
                    }
                } else {
                    // Iterate through parsed components
                    for(auto it = m_components.begin(), itEnd = m_components.end(); it != itEnd; ++it) {
                        if(data1 == (*it)->id) {
                            if((*it)->type == ANA_BUS) {
                                // If the data1 is a bus ID, the element is a element with different data order.
                                // Find the correct element ID with data3
                                for(auto itDiff = m_components.begin(), itDiffEnd = m_components.end();
                                    itDiff != itDiffEnd; ++itDiff) {
                                    if(data3 == (*itDiff)->id) {
                                        (*itDiff)->busConnectionNode[data4 - 1] = std::make_pair(data1, data2);
                                        break;
                                    }
                                }
                            } else {
                                (*it)->busConnectionNode[data2 - 1] = std::make_pair(data3, data4);
                                break;
                            }
                        }
                    }
                }
            } break;
            default:
                break;
        }
    }
    // Last line

    // Parse PWF file
    for(wxString line = pwf.GetFirstLine(); !pwf.Eof(); line = pwf.GetNextLine()) {
        // Current line
        if(line == "TITU") {  // Title
            m_projectName = pwf.GetNextLine().Trim();
        } else if(line != "") {
            if(line == "FIM") break;
            if(line[0] != '(') {  // Ignore commented line
                wxString exeCode = line;
                wxString data = "";
                for(wxString lineData = pwf.GetNextLine(); lineData != "99999" && !pwf.Eof();
                    lineData = pwf.GetNextLine()) {
                    if(lineData == "FIM") break;
                    if(lineData[0] != '(') {  // Ignore commented line
                        data += lineData + "\n";
                    }
                }
                if(data != "" || data != "FIM") {  // Don't parse empty data
                    if(!ParsePWFExeCode(data, exeCode)) return false;
                }
            }
        }
    }
    // Last line

    return true;
}

wxString ParseAnarede::GetLSTLineNextValue(wxString line, int& currentPos)
{
    // Parse each line column
    wxString strValue = "=";
    for(; currentPos < static_cast<int>(line.length()); ++currentPos) {
        if(line[currentPos] != ' ') {  // Skip the firsts whitespaces
            wxString parsedLine = line.Mid(currentPos);
            strValue = parsedLine.BeforeFirst(' ');  // Get the character before the first whitespace
            currentPos += strValue.length();         // Set the current parse position
            break;
        }
    }
    return strValue;
}

bool ParseAnarede::GetLenghtAndRotationFromBusCode(wxString code, double& lenght, int& rotationID)
{
    // Get code in decimal.
    long longCode;
    if(!code.ToLong(&longCode)) return false;
    // Convert code to hex
    std::stringstream hexCode;
    hexCode << std::hex << longCode;
    // Convert code to string
    wxString hexCodeStr = hexCode.str();
    // Get length on the substring
    wxString lenghtStr = hexCodeStr.Left(hexCodeStr.length() - 1);
    // Convert lenght string (hex) to decimal
    hexCode.str("");
    hexCode << lenghtStr.ToStdString();
    int intLenght;
    hexCode >> std::hex >> intLenght;
    lenght = static_cast<double>(intLenght);
    // Get rotation ID from substring
    wxString rotationIDStr = hexCodeStr.Right(1);
    // Convert rotation ID string to int
    long rotationIDLong;
    if(!rotationIDStr.ToLong(&rotationIDLong)) return false;
    rotationID = static_cast<int>(rotationIDLong);
    // Calculate the true value of lenght
    lenght = 3.0 * lenght - 16.0;
    return true;
}

wxPoint2DDouble ParseAnarede::GetNodePositionFromID(Bus* bus, double scale, int nodeID)
{
    wxPoint2DDouble nodePt;

    double offset = (static_cast<double>(nodeID) - 1.0) * 16.0;
    nodePt = bus->GetPosition() + wxPoint2DDouble(offset * scale, 0) -
             wxPoint2DDouble(bus->GetWidth() / 2, bus->GetHeight() / 2);
    nodePt = bus->RotateAtPosition(nodePt, bus->GetAngle(), true);

    return nodePt;
}

bool ParseAnarede::StrToElementType(wxString strType, ElementTypeAnarede& type)
{
    // Check if type exists, othewise return false
    int typeInt = wxAtoi(strType);
    switch(typeInt) {
        case ANA_BUS:
        case ANA_GENERATOR:
        case ANA_LOAD:
        case ANA_SHUNT:
        case ANA_MIT:
        case ANA_TRANSFORMER:
        case ANA_LINE:
        case ANA_IND_LOAD:
        case ANA_IND_SHUNT:
        case ANA_IND_GENERATOR: {
            type = static_cast<ElementTypeAnarede>(typeInt);
            return true;
        } break;
        default:
            break;
    }
    return false;
}

bool ParseAnarede::ParsePWFExeCode(wxString data, wxString exeCode)
{
    if(exeCode == "DCTE") {
        // Parse data
        wxString code = "";
        wxString value = "";
        for(unsigned int i = 0; i < data.length(); ++i) {
            code = data.Mid(i, 4);
            value = data.Mid(i + 5, 6).Trim(false);
            i += 11;
            if(code == "BASE") {
                if(!value.ToCDouble(&m_mvaBase)) return false;
                break;
            }
        }
    } else if(exeCode == "DBAR") {
        wxString lineData = "";
        while(data != "") {
            wxString rest = "";
            lineData = data.BeforeFirst('\n', &rest);
            data = rest;
            BusData* busData = new BusData();

            if(!GetPWFStructuredData(lineData, 0, 5, busData->id)) return false;
            wxString isOnlineCode = lineData.Mid(6, 1);
            if(isOnlineCode == "L" || isOnlineCode == " ")
                busData->isOnline = true;
            else if(isOnlineCode == "D")
                busData->isOnline = false;
            else
                return false;
            if(!GetPWFStructuredData(lineData, 7, 1, busData->type)) return false;
            if(!GetPWFStructuredData(lineData, 8, 2, busData->voltageBase)) return false;
            busData->busName = lineData.Mid(10, 12).Trim();
            if(!GetPWFStructuredData(lineData, 24, 4, busData->voltage, 1)) return false;
            if(!GetPWFStructuredData(lineData, 28, 4, busData->angle)) return false;
            double pg, qg;
            if(!GetPWFStructuredData(lineData, 32, 5, pg)) return false;
            if(!GetPWFStructuredData(lineData, 37, 5, qg)) return false;
            busData->genPower = std::complex<double>(pg, qg);
            if(!GetPWFStructuredData(lineData, 42, 5, busData->minReactivePower)) return false;
            if(!GetPWFStructuredData(lineData, 47, 5, busData->maxReactivePower)) return false;
            if(!GetPWFStructuredData(lineData, 52, 6, busData->ctrlBusID)) return false;
            double pl, ql;
            if(!GetPWFStructuredData(lineData, 58, 5, pl)) return false;
            if(!GetPWFStructuredData(lineData, 63, 5, ql)) return false;
            busData->loadPower = std::complex<double>(pl, ql);
            if(!GetPWFStructuredData(lineData, 68, 5, busData->shuntReactive)) return false;

            m_busData.push_back(busData);
        }
    } else if(exeCode == "DLIN") {
        wxString lineData = "";
        while(data != "") {
            wxString rest = "";
            lineData = data.BeforeFirst('\n', &rest);
            data = rest;
            BranchData* branchData = new BranchData();

            if(!GetPWFStructuredData(lineData, 15, 2, branchData->id)) return false;
            int from, to;
            if(!GetPWFStructuredData(lineData, 0, 5, from)) return false;
            if(!GetPWFStructuredData(lineData, 10, 5, to)) return false;
            branchData->busConnections = std::make_pair(from, to);

            wxString isOnlineCode = lineData.Mid(6, 1) + lineData.Mid(9, 1) + lineData.Mid(17, 1);
            if(isOnlineCode.Find('D') == wxNOT_FOUND)
                branchData->isOnline = true;
            else
                branchData->isOnline = false;
            if(!GetPWFStructuredData(lineData, 20, 6, branchData->resistance, 4)) return false;
            if(!GetPWFStructuredData(lineData, 26, 6, branchData->indReactance, 4)) return false;
            if(!GetPWFStructuredData(lineData, 32, 6, branchData->capSusceptance, 4)) return false;
            if(!GetPWFStructuredData(lineData, 38, 5, branchData->tap, 2)) return false;
            if(!GetPWFStructuredData(lineData, 53, 5, branchData->phaseShift, 3)) return false;

            m_branchData.push_back(branchData);
        }
    } else if(exeCode == "DCAI") {
        wxString lineData = "";
        while(data != "") {
            wxString rest = "";
            lineData = data.BeforeFirst('\n', &rest);
            data = rest;
            IndElementData* indData = new IndElementData();
            ;
            indData->type = ANA_IND_LOAD;

            if(!GetPWFStructuredData(lineData, 9, 2, indData->id)) return false;
            if(!GetPWFStructuredData(lineData, 0, 5, indData->busConnection)) return false;

            wxString isOnlineCode = lineData.Mid(13, 1);
            if(isOnlineCode == "L" || isOnlineCode == " ")
                indData->isOnline = true;
            else if(isOnlineCode == "D")
                indData->isOnline = false;
            else
                return false;

            double pl, ql;
            if(!GetPWFStructuredData(lineData, 22, 5, pl)) return false;
            if(!GetPWFStructuredData(lineData, 28, 5, ql)) return false;
            indData->power = std::complex<double>(pl, ql);
            // Unities in operation
            if(!GetPWFStructuredData(lineData, 18, 3, indData->numUnits)) return false;

            m_indElementData.push_back(indData);
        }
    } else if(exeCode == "DGEI") {
        wxString lineData = "";
        while(data != "") {
            wxString rest = "";
            lineData = data.BeforeFirst('\n', &rest);
            data = rest;
            IndGenData* genData = new IndGenData();
            genData->type = ANA_IND_GENERATOR;

            if(!GetPWFStructuredData(lineData, 9, 2, genData->id)) return false;
            if(!GetPWFStructuredData(lineData, 0, 5, genData->busConnection)) return false;

            wxString isOnlineCode = lineData.Mid(12, 1);
            if(isOnlineCode == "L" || isOnlineCode == " ")
                genData->isOnline = true;
            else if(isOnlineCode == "D")
                genData->isOnline = false;
            else
                return false;

            double pg, qg;
            if(!GetPWFStructuredData(lineData, 22, 5, pg)) return false;
            if(!GetPWFStructuredData(lineData, 27, 5, qg)) return false;
            genData->power = std::complex<double>(pg, qg);
            // Unities in operation
            if(!GetPWFStructuredData(lineData, 16, 3, genData->numUnits)) return false;
            if(!GetPWFStructuredData(lineData, 32, 5, genData->minReactivePower)) return false;
            if(!GetPWFStructuredData(lineData, 37, 5, genData->maxReactivePower)) return false;
            if(!GetPWFStructuredData(lineData, 42, 6, genData->xt, 2)) return false;
            if(!GetPWFStructuredData(lineData, 49, 5, genData->xd, 4)) return false;
            if(!GetPWFStructuredData(lineData, 54, 5, genData->xq, 4)) return false;
            if(!GetPWFStructuredData(lineData, 59, 5, genData->xl, 4)) return false;
            if(!GetPWFStructuredData(lineData, 69, 5, genData->ratedPower, 3)) return false;

            m_indElementData.push_back(genData);
        }
    }

    return true;
}

bool ParseAnarede::GetPWFStructuredData(wxString data,
                                        unsigned int startPos,
                                        unsigned int dataLenght,
                                        int& value,
                                        int decimalPos)
{
    if(data.length() < startPos || data.length() < (startPos + dataLenght)) {
        value = 0;
        return true;
    }
    wxString strValue = data.Mid(startPos, dataLenght);
    strValue.Replace(' ', '0');
    long lValue = 0;
    if(!strValue.ToLong(&lValue)) return false;
    value = static_cast<int>(lValue);
    return true;
}

bool ParseAnarede::GetPWFStructuredData(wxString data,
                                        unsigned int startPos,
                                        unsigned int dataLenght,
                                        double& value,
                                        int decimalPos)
{
    if(data.length() < startPos || data.length() < (startPos + dataLenght)) {
        value = 0.0;
        return true;
    }
    wxString strValue = data.Mid(startPos, dataLenght);
    if(strValue.Find('-') == wxNOT_FOUND) strValue.Replace(' ', '0');
    if(decimalPos != -1 && strValue.Find('.') == wxNOT_FOUND) strValue.insert(decimalPos, '.');
    if(!strValue.ToCDouble(&value)) return false;
    return true;
}

ParseAnarede::BusData* ParseAnarede::GetBusDataFromID(int id)
{
    for(auto it = m_busData.begin(), itEnd = m_busData.end(); it != itEnd; ++it) {
        if((*it)->id == id) return *it;
    }
    return NULL;
}

ParseAnarede::BranchData* ParseAnarede::GetBranchDataFromID(int id, int fromBus, int toBus) { return NULL; }

ParseAnarede::IndElementData* ParseAnarede::GetIndElementDataFromID(int id, int bus) { return NULL; }

void ParseAnarede::ClearData()
{
    for(auto it = m_components.begin(), itEnd = m_components.end(); it != itEnd; ++it) {
        if(*it) delete *it;
    }
    m_components.clear();
    for(auto it = m_lines.begin(), itEnd = m_lines.end(); it != itEnd; ++it) {
        if(*it) delete *it;
    }
    m_lines.clear();
    for(auto it = m_busData.begin(), itEnd = m_busData.end(); it != itEnd; ++it) {
        if(*it) delete *it;
    }
    m_busData.clear();
    for(auto it = m_branchData.begin(), itEnd = m_branchData.end(); it != itEnd; ++it) {
        if(*it) delete *it;
    }
    m_branchData.clear();
    for(auto it = m_indElementData.begin(), itEnd = m_indElementData.end(); it != itEnd; ++it) {
        if(*it) delete *it;
    }
    m_indElementData.clear();
}