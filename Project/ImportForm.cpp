#include "ImportForm.h"
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
    return true;
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
                component.type = static_cast<ElementTypeAnarede>(wxAtoi(GetLSTLineNextValue(line, parsePosition)));

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
                            if((*it).type == ANA_TRANSFORMER) {
                                if(data2 == 1) {  // Primary
                                    (*it).busConnectionNode[0] = std::make_pair(data3, data4);
                                } else if(data2 == 2) {  // Secondary
                                    (*it).busConnectionNode[1] = std::make_pair(data3, data4);
                                }
                            } else {
                                (*it).busConnectionNode[0] = std::make_pair(data3, data4);
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

    for(auto it = m_components.begin(), itEnd = m_components.end(); it != itEnd; ++it) {
        wxString comp = wxString::Format("ID = %d\n", (*it).id);
        comp += wxString::Format("TIPO = %d\n", (*it).type);
        comp += wxString::Format("TAMANHO = %f\n", (*it).length);
        comp += wxString::Format("ROTACAO = %d\n", (*it).rotationID);
        comp += wxString::Format("POS = (%f , %f)\n", (*it).position.m_x, (*it).position.m_y);
        comp += wxString::Format("ID ELETRICO = %d\n", (*it).electricalID);
        for(int i = 0; i < 2; ++i) {
            comp += wxString::Format("BARRA[%d]<IDg, IDe> = <%d , %d>\n", i, (*it).busConnectionID[i].first,
                                     (*it).busConnectionID[i].second);
        }
        for(int i = 0; i < 2; ++i) {
            comp += wxString::Format("NODE[%d]<IDg, IDp> = <%d , %d>\n", i, (*it).busConnectionNode[i].first,
                                     (*it).busConnectionNode[i].second);
        }
        wxMessageBox(comp);
    }
    /*for(auto it = m_lines.begin(), itEnd = m_lines.end(); it != itEnd; ++it) {
        wxString comp = wxString::Format("ID = %d\n", (*it).id);
        comp += wxString::Format("TIPO = %d\n", (*it).type);
        comp += wxString::Format("ID ELETRICO = %d\n", (*it).electricalID);
        for(int i = 0; i < 2; ++i) {
            comp += wxString::Format("BARRA[%d]<IDg, IDe> = <%d , %d>\n", i, (*it).busConnectionID[i].first,
                                     (*it).busConnectionID[i].second);
        }
        for(int i = 0; i < 2; ++i) {
            comp += wxString::Format("NODE[%d]<IDg, IDp> = <%d , %d>\n", i, (*it).busConnectionNode[i].first,
                                     (*it).busConnectionNode[i].second);
        }
        for(unsigned int i = 0; i < (*it).nodesPosition.size(); ++i) {
            comp += wxString::Format("NODEPos[%d] = (%f , %f)\n", i, (*it).nodesPosition[i].m_x,
                                     (*it).nodesPosition[i].m_y);
        }
        wxMessageBox(comp);
    }*/
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
    long longCode;
    if(!code.ToLong(&longCode)) return false;
    std::bitset<22> bit(longCode);
    wxString binStr(bit.to_string());
    wxString lenghtStr = binStr.Left(20);
    wxString rotationIDStr = binStr.Right(2);
    std::bitset<20> lenghtBit(lenghtStr.ToStdString());
    std::bitset<2> rotationIDBit(rotationIDStr.ToStdString());
    lenght = ((static_cast<double>(lenghtBit.to_ulong()) / 16.0) - 1) * 50.0 + 30.0;
    rotationID = static_cast<int>(rotationIDBit.to_ulong());
    return true;
}
