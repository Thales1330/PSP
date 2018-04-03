#include "Bus.h"
#include "Capacitor.h"
#include "ImportForm.h"
#include "IndMotor.h"
#include "Inductor.h"
#include "Line.h"
#include "Load.h"
#include "SyncGenerator.h"
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

    std::vector<Element*> elementList;

    std::vector<Bus*> busList;
    std::vector<SyncGenerator*> syncGeneratorList;
    std::vector<Load*> loadList;
    std::vector<Inductor*> indList;
    std::vector<IndMotor*> indMotorList;
    std::vector<Transformer*> transformerList;
    std::vector<Line*> lineList;

    auto components = parseAnarede.GetComponents();
    for(auto it = components.begin(), itEnd = components.end(); it != itEnd; ++it) {
        switch((*it).type) {
            case ANA_BUS: {
                Bus* bus = new Bus(wxPoint2DDouble((*it).position.m_x * scale, (*it).position.m_y * scale));
                bus->SetID((*it).id);
                bus->SetWidth((*it).length * scale);
                // bus->SetPosition(bus->GetPosition());  // Update bus rect
                bus->StartMove(bus->GetPosition());
                if((*it).rotationID == 0) {
                    bus->Move(bus->GetPosition() + wxPoint2DDouble(bus->GetWidth() / 2, bus->GetHeight() / 2));
                } else if((*it).rotationID == 1) {
                    for(int i = 0; i < (*it).rotationID * 2; ++i) bus->Rotate();
                    bus->Move(bus->GetPosition() + wxPoint2DDouble(-bus->GetHeight() / 2, bus->GetWidth() / 2));
                } else if((*it).rotationID == 2) {
                    for(int i = 0; i < (*it).rotationID * 2; ++i) bus->Rotate();
                    bus->Move(bus->GetPosition() + wxPoint2DDouble(-bus->GetWidth() / 2, -bus->GetHeight() / 2));
                } else if((*it).rotationID == 3) {
                    for(int i = 0; i < (*it).rotationID * 2; ++i) bus->Rotate();
                    bus->Move(bus->GetPosition() + wxPoint2DDouble(-bus->GetHeight() / 2, -bus->GetWidth() / 2));
                }

                busList.push_back(bus);
            } break;
            case ANA_GENERATOR:
            case ANA_IND_GENERATOR: {
                // Find parent bus
                Bus* parentBus = GetBusFromID(busList, (*it).busConnectionID[0].first);
                wxPoint2DDouble nodePos =
                    parseAnarede.GetNodePositionFromID(parentBus, scale, (*it).busConnectionNode[0].second);

                SyncGenerator* syncGenerator = new SyncGenerator();
                syncGenerator->SetID((*it).id);
                syncGenerator->AddParent(parentBus, nodePos);

                syncGenerator->StartMove(syncGenerator->GetPosition());
                syncGenerator->Move(wxPoint2DDouble((*it).position.m_x * scale, (*it).position.m_y * scale));

                for(int i = 0; i < 2; ++i) syncGenerator->Rotate(false);  // Set to ANAREDE default rotation
                for(int i = 0; i < (*it).rotationID * 2; ++i) syncGenerator->Rotate();

                syncGeneratorList.push_back(syncGenerator);
            } break;
            case ANA_LOAD:
            case ANA_IND_LOAD: {
                Bus* parentBus = GetBusFromID(busList, (*it).busConnectionID[0].first);
                wxPoint2DDouble nodePos =
                    parseAnarede.GetNodePositionFromID(parentBus, scale, (*it).busConnectionNode[0].second);

                Load* load = new Load();
                load->SetID((*it).id);
                load->AddParent(parentBus, nodePos);

                load->StartMove(load->GetPosition());
                load->Move(wxPoint2DDouble((*it).position.m_x * scale, (*it).position.m_y * scale));

                for(int i = 0; i < (*it).rotationID * 2; ++i) load->Rotate();

                loadList.push_back(load);
            } break;
            case ANA_SHUNT:
            case ANA_IND_SHUNT: {
                Bus* parentBus = GetBusFromID(busList, (*it).busConnectionID[0].first);
                wxPoint2DDouble nodePos =
                    parseAnarede.GetNodePositionFromID(parentBus, scale, (*it).busConnectionNode[0].second);

                Inductor* ind = new Inductor();
                ind->SetID((*it).id);
                ind->AddParent(parentBus, nodePos);

                ind->StartMove(ind->GetPosition());
                // Offset (ind->GetHeight() / 2 + 10) to adequate the y coordinate
                ind->Move(wxPoint2DDouble((*it).position.m_x * scale,
                                          (*it).position.m_y * scale + ind->GetHeight() / 2 + 10));
                if((*it).rotationID != 0) {
                    // Remove offset in position
                    ind->Move(wxPoint2DDouble((*it).position.m_x * scale, (*it).position.m_y * scale));
                    // Get the rotated point ralated to the offset
                    wxPoint2DDouble movePt =
                        ind->RotateAtPosition(ind->GetPosition() + wxPoint2DDouble(0, ind->GetHeight() / 2 + 10),
                                              (*it).rotationID * 90.0, true);
                    ind->Move(movePt);

                    for(int i = 0; i < (*it).rotationID * 2; ++i) ind->Rotate();
                }

                indList.push_back(ind);
            } break;
            case ANA_MIT: {
                // Find parent bus
                Bus* parentBus = GetBusFromID(busList, (*it).busConnectionID[0].first);
                wxPoint2DDouble nodePos =
                    parseAnarede.GetNodePositionFromID(parentBus, scale, (*it).busConnectionNode[0].second);

                IndMotor* indMotor = new IndMotor();
                indMotor->SetID((*it).id);
                indMotor->AddParent(parentBus, nodePos);

                indMotor->StartMove(indMotor->GetPosition());
                indMotor->Move(wxPoint2DDouble((*it).position.m_x * scale, (*it).position.m_y * scale));

                for(int i = 0; i < 2; ++i) indMotor->Rotate(false);  // Set to ANAREDE default rotation
                for(int i = 0; i < (*it).rotationID * 2; ++i) indMotor->Rotate();

                indMotorList.push_back(indMotor);
            } break;
            case ANA_TRANSFORMER: {
                Bus* parentBus1 = GetBusFromID(busList, (*it).busConnectionID[0].first);
                Bus* parentBus2 = GetBusFromID(busList, (*it).busConnectionID[1].first);
                wxPoint2DDouble nodePos1 =
                    parseAnarede.GetNodePositionFromID(parentBus1, scale, (*it).busConnectionNode[0].second);
                wxPoint2DDouble nodePos2 =
                    parseAnarede.GetNodePositionFromID(parentBus2, scale, (*it).busConnectionNode[1].second);

                Transformer* transformer = new Transformer();
                transformer->SetID((*it).id);
                transformer->AddParent(parentBus1, nodePos1);
                transformer->AddParent(parentBus2, nodePos2);

                transformer->StartMove(transformer->GetPosition());
                transformer->Move(wxPoint2DDouble((*it).position.m_x * scale, (*it).position.m_y * scale));

                for(int i = 0; i < 2; ++i) transformer->Rotate();  // Set to ANAREDE default rotation
                for(int i = 0; i < (*it).rotationID * 2; ++i) transformer->Rotate();

                transformerList.push_back(transformer);
            } break;
            default: {
            } break;
        }
    }

    auto powerLines = parseAnarede.GetLines();
    for(auto it = powerLines.begin(), itEnd = powerLines.end(); it != itEnd; ++it) {
        if((*it).type == ANA_LINE) {
            Bus* parentBus1 = GetBusFromID(busList, (*it).busConnectionID[0].first);
            Bus* parentBus2 = GetBusFromID(busList, (*it).busConnectionID[1].first);
            wxPoint2DDouble nodePos1 =
                parseAnarede.GetNodePositionFromID(parentBus1, scale, (*it).busConnectionNode[0].second);
            wxPoint2DDouble nodePos2 =
                parseAnarede.GetNodePositionFromID(parentBus2, scale, (*it).busConnectionNode[1].second);

            Line* line = new Line();
            line->SetID((*it).id);
            line->AddParent(parentBus1, nodePos1);
            for(unsigned int i = 0; i < (*it).nodesPosition.size(); ++i)
                line->AddPoint(wxPoint2DDouble((*it).nodesPosition[i].m_x * scale, (*it).nodesPosition[i].m_y * scale));
            line->AddParent(parentBus2, nodePos2);

            lineList.push_back(line);
        }
    }

    for(auto it = busList.begin(), itEnd = busList.end(); it != itEnd; ++it) elementList.push_back(*it);
    for(auto it = syncGeneratorList.begin(), itEnd = syncGeneratorList.end(); it != itEnd; ++it)
        elementList.push_back(*it);
    for(auto it = loadList.begin(), itEnd = loadList.end(); it != itEnd; ++it) elementList.push_back(*it);
    for(auto it = indList.begin(), itEnd = indList.end(); it != itEnd; ++it) elementList.push_back(*it);
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

                Component component;

                component.id = wxAtoi(GetLSTLineNextValue(line, parsePosition));
                // Check if is component type is valid
                if(StrToElementType(GetLSTLineNextValue(line, parsePosition), component.type)) {
                    if(component.type == ANA_BUS) {
                        if(!GetLenghtAndRotationFromBusCode(GetLSTLineNextValue(line, parsePosition), component.length,
                                                            component.rotationID))
                            return false;
                    } else {
                        component.rotationID = wxAtoi(GetLSTLineNextValue(line, parsePosition));
                    }
                    parsePosition += 2;  // Jump to position
                    if(!GetLSTLineNextValue(line, parsePosition).ToCDouble(&component.position.m_x)) return false;
                    if(!GetLSTLineNextValue(line, parsePosition).ToCDouble(&component.position.m_y)) return false;
                    parsePosition += 8;  // Jump to electrical ID
                    component.electricalID = wxAtoi(GetLSTLineNextValue(line, parsePosition));

                    // Bus connection IDs
                    if(component.type != ANA_BUS) {
                        int fromBus = wxAtoi(GetLSTLineNextValue(line, parsePosition));  // Origin bus
                        int toBus = 0;
                        // If the component is a transformer, parse the next value (toBus)
                        if(component.type == ANA_TRANSFORMER) {
                            toBus = wxAtoi(GetLSTLineNextValue(line, parsePosition));  // Destiny bus
                        }
                        // Iterate through the already parsed components (the buses is saved first)
                        for(auto it = m_components.begin(), itEnd = m_components.end(); it != itEnd; ++it) {
                            if((*it).type == ANA_BUS) {
                                if(fromBus == (*it).electricalID) {
                                    component.busConnectionID[0] = std::make_pair((*it).id, fromBus);
                                } else if(toBus == (*it).electricalID) {
                                    component.busConnectionID[1] = std::make_pair((*it).id, toBus);
                                }
                            }
                        }
                    }
                    m_components.push_back(component);
                }
            } break;
            case 'L': {  // Line
                int parsePosition = 1;

                PowerLine pLine;

                pLine.id = wxAtoi(GetLSTLineNextValue(line, parsePosition));
                pLine.type = static_cast<ElementTypeAnarede>(wxAtoi(GetLSTLineNextValue(line, parsePosition)));
                parsePosition += 4;  // Jump to from bus

                int fromBus = wxAtoi(GetLSTLineNextValue(line, parsePosition));  // Origin bus
                int toBus = wxAtoi(GetLSTLineNextValue(line, parsePosition));    // Destiny bus

                // Iterate through the already parsed components
                for(auto it = m_components.begin(), itEnd = m_components.end(); it != itEnd; ++it) {
                    if((*it).type == ANA_BUS) {
                        if(fromBus == (*it).id) {
                            pLine.busConnectionID[0] = std::make_pair((*it).id, (*it).electricalID);
                        } else if(toBus == (*it).id) {
                            pLine.busConnectionID[1] = std::make_pair((*it).id, (*it).electricalID);
                        }
                    }
                }
                pLine.electricalID = wxAtoi(GetLSTLineNextValue(line, parsePosition));

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
                        if(data5 == (*it).id) {
                            (*it).busConnectionNode[0] = std::make_pair(data1, data2);
                            (*it).busConnectionNode[1] = std::make_pair(data3, data4);
                            for(int i = 0; i < data6; ++i) {
                                wxPoint2DDouble nodePt;
                                if(!GetLSTLineNextValue(line, parsePosition).ToCDouble(&nodePt.m_x)) return false;
                                if(!GetLSTLineNextValue(line, parsePosition).ToCDouble(&nodePt.m_y)) return false;
                                (*it).nodesPosition.push_back(nodePt);
                            }
                        }
                    }
                } else {
                    // Iterate through parsed components
                    for(auto it = m_components.begin(), itEnd = m_components.end(); it != itEnd; ++it) {
                        if(data1 == (*it).id) {
                            if((*it).type == ANA_BUS) {
                                // If the data1 is a bus ID, the element is a element with different data order.
                                // Find the correct element ID with data3
                                for(auto itDiff = m_components.begin(), itDiffEnd = m_components.end();
                                    itDiff != itDiffEnd; ++itDiff) {
                                    if(data3 == (*itDiff).id) {
                                        (*itDiff).busConnectionNode[data4 - 1] = std::make_pair(data1, data2);
                                        break;
                                    }
                                }
                            } else {
                                (*it).busConnectionNode[data2 - 1] = std::make_pair(data3, data4);
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
            BusData busData;
            if(!GetPWFStructuredData(lineData, 0, 5, busData.id)) return false;
            wxString isOnlineCode = lineData.Mid(6, 1);
            if(isOnlineCode == "L")
                busData.isOnline = true;
            else if(isOnlineCode == "D")
                busData.isOnline = false;
            else
                return false;
            if(!GetPWFStructuredData(lineData, 7, 1, busData.type)) return false;
            if(!GetPWFStructuredData(lineData, 8, 2, busData.voltageBase)) return false;
            busData.busName = lineData.Mid(10, 12).Trim();
            if(!GetPWFStructuredData(lineData, 24, 4, busData.voltage, 1)) return false;
            if(!GetPWFStructuredData(lineData, 28, 4, busData.angle)) return false;
            double pg, qg;
            if(!GetPWFStructuredData(lineData, 32, 5, pg)) return false;
            if(!GetPWFStructuredData(lineData, 37, 5, qg)) return false;
            busData.genPower = std::complex<double>(pg, qg);
            if(!GetPWFStructuredData(lineData, 42, 5, busData.minReactivePower)) return false;
            if(!GetPWFStructuredData(lineData, 47, 5, busData.maxReactivePower)) return false;
            if(!GetPWFStructuredData(lineData, 52, 6, busData.ctrlBusID)) return false;
            double pl, ql;
            if(!GetPWFStructuredData(lineData, 58, 5, pl)) return false;
            if(!GetPWFStructuredData(lineData, 63, 5, ql)) return false;
            busData.loadPower = std::complex<double>(pl, ql);
            if(!GetPWFStructuredData(lineData, 68, 5, busData.shuntReactive)) return false;

            wxMessageBox(wxString::Format("ID = %d\nonline = %d\ntype = %d\nbaseVoltage = %d\nName = \"%s\"\nTensao = "
                                          "%f\nAngulo = %f\nPg = %f\nQg = %f\nminQ = %f\nmaxQ = %f\nctrlBus = %d\nPl = "
                                          "%f\nQl = %f\nshuntMVA = %f",
                                          busData.id, busData.isOnline, busData.type, busData.voltageBase,
                                          busData.busName, busData.voltage, busData.angle, busData.genPower.real(),
                                          busData.genPower.imag(), busData.minReactivePower, busData.maxReactivePower,
                                          busData.ctrlBusID, busData.loadPower.real(), busData.loadPower.imag(),
                                          busData.shuntReactive));
        }
    }
    return true;
}

bool ParseAnarede::GetPWFStructuredData(wxString data, int startPos, int dataLenght, int& value, int decimalPos)
{
    wxString strValue = data.Mid(startPos, dataLenght);
    strValue.Replace(' ', '0');
    long lValue = 0;
    if(!strValue.ToLong(&lValue)) return false;
    value = static_cast<int>(lValue);
    return true;
}

bool ParseAnarede::GetPWFStructuredData(wxString data, int startPos, int dataLenght, double& value, int decimalPos)
{
    wxString strValue = data.Mid(startPos, dataLenght);
    if(strValue.Find('-') == wxNOT_FOUND) strValue.Replace(' ', '0');
    if(decimalPos != -1) strValue.insert(decimalPos, '.');
    if(!strValue.ToCDouble(&value)) return false;
    return true;
}
