#include "Line.h"

Line::Line()
    : Branch()
{
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 3; j++) {
            m_electricaData.faultCurrent[i][j] = std::complex<double>(0.0, 0.0);
        }
    }
}

Line::Line(wxString name)
    : Branch()
{
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 3; j++) {
            m_electricaData.faultCurrent[i][j] = std::complex<double>(0.0, 0.0);
        }
    }
    m_electricaData.name = name;
}
Line::~Line() {}
bool Line::Contains(wxPoint2DDouble position) const
{
    if(PointToLineDistance(position) < 5.0) {
        return true;
    }
    return false;
}

void Line::Draw(wxPoint2DDouble translation, double scale) const
{
    OpenGLColour elementColour;
    if(m_online)
        elementColour = m_onlineElementColour;
    else
        elementColour = m_offlineElementColour;

    std::vector<wxPoint2DDouble> pointList = m_pointList;
    if(!m_inserted && pointList.size() > 0) {
        wxPoint2DDouble secondPoint = m_position;
        if(pointList.size() > 2) {
            secondPoint = pointList[2];
        }
        pointList[1] = GetSwitchPoint(m_parentList[0], pointList[0], secondPoint);
        pointList.push_back(m_position);
    }

    // Line selected (Layer 1).
    if(m_selected) {
        glLineWidth(1.5 + m_borderSize * 2.0);
        glColor4dv(m_selectionColour.GetRGBA());
        DrawLine(pointList);

        // Draw nodes selection.
        if(pointList.size() > 0) {
            DrawCircle(pointList[0], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
            if(m_inserted) {
                DrawCircle(pointList[pointList.size() - 1], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
            }
        }
    }

    // Draw line (Layer 2)
    glLineWidth(1.5);
    glColor4dv(elementColour.GetRGBA());
    DrawLine(pointList);

    if(m_inserted) {
        DrawSwitches();
        DrawPowerFlowPts();
    }

    // Draw nodes.
    if(pointList.size() > 0) {
        glColor4dv(elementColour.GetRGBA());
        DrawCircle(pointList[0], 5.0, 10, GL_POLYGON);
        if(m_inserted) {
            DrawCircle(pointList[pointList.size() - 1], 5.0, 10, GL_POLYGON);
        }
    }

    // Draw pickboxes (Layer 3).
    if(m_showPickbox) {
        glPushMatrix();
        glLoadIdentity();

        for(int i = 2; i < (int)m_pointList.size() - 2; i++) {
            DrawPickbox(WorldToScreen(m_pointList[i], translation, scale));
        }

        glPopMatrix();
    }
}

void Line::Move(wxPoint2DDouble position)
{
    if(!m_parentList[0]) {
        m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
        UpdateSwitchesPosition();
        UpdatePowerFlowArrowsPosition();
    }
    if(!m_parentList[1]) {
        m_pointList[m_pointList.size() - 1] = m_movePts[m_pointList.size() - 1] + position - m_moveStartPt;
        UpdateSwitchesPosition();
        UpdatePowerFlowArrowsPosition();
    }

    if(!m_parentList[0] && !m_parentList[1]) {
        for(int i = 2; i < (int)m_pointList.size() - 2; i++) {
            m_pointList[i] = m_movePts[i] + position - m_moveStartPt;
        }
    }
}

bool Line::AddParent(Element* parent, wxPoint2DDouble position)
{
    if(parent) {
        // First bus.
        if(m_parentList.size() == 0) {
            m_position = position;
            m_parentList.push_back(parent);
            parent->AddChild(this);
            wxPoint2DDouble parentPt =
                parent->RotateAtPosition(position, -parent->GetAngle());       // Rotate click to horizontal position.
            parentPt.m_y = parent->GetPosition().m_y;                          // Centralize on bus.
            parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle()); // Rotate back.
            m_pointList.push_back(parentPt);                                   // First point
            m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_position));

            wxRect2DDouble genRect(0, 0, 0, 0);
            m_switchRect.push_back(genRect);
            UpdateSwitches();

            Bus* parentBus = static_cast<Bus*>(parent);
            m_electricaData.nominalVoltage = parentBus->GetElectricalData().nominalVoltage;
            m_electricaData.nominalVoltageUnit = parentBus->GetElectricalData().nominalVoltageUnit;

            return false;
        }
        // Second bus.
        else if(parent != m_parentList[0]) {
            Bus* parentBus = static_cast<Bus*>(parent);
            if(m_electricaData.nominalVoltage != parentBus->GetElectricalData().nominalVoltage ||
                m_electricaData.nominalVoltageUnit != parentBus->GetElectricalData().nominalVoltageUnit) {
                wxMessageDialog msgDialog(NULL, _("Unable to connect two buses with different nominal voltages.\n"
                                                  "Use a transformer or edit the bus properties."),
                    _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
                msgDialog.ShowModal();
                return false;
            }

            m_parentList.push_back(parent);
            parent->AddChild(this);
            wxPoint2DDouble parentPt =
                parent->RotateAtPosition(position, -parent->GetAngle());       // Rotate click to horizontal position.
            parentPt.m_y = parent->GetPosition().m_y;                          // Centralize on bus.
            parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle()); // Rotate back.

            // Set first switch point.
            wxPoint2DDouble secondPoint = parentPt;
            if(m_pointList.size() > 2) {
                secondPoint = m_pointList[2];
            }
            m_pointList[1] = GetSwitchPoint(m_parentList[0], m_pointList[0], secondPoint);

            // Set the second switch point.
            m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_pointList[m_pointList.size() - 1]));

            m_pointList.push_back(parentPt); // Last point.

            wxRect2DDouble genRect(0, 0, 0, 0);
            m_switchRect.push_back(genRect);
            UpdateSwitches();

            m_inserted = true;
            UpdatePowerFlowArrowsPosition();
            return true;
        }
    }
    return false;
}
bool Line::Intersects(wxRect2DDouble rect) const
{
    for(auto it = m_pointList.begin(); it != m_pointList.end(); ++it) {
        if(rect.Contains(*it)) return true;
    }
    return false;
}
void Line::MovePickbox(wxPoint2DDouble position)
{
    if(m_activePickboxID == ID_PB_NONE) return;

    for(int i = 2; i < (int)m_pointList.size() - 2; i++) {
        if(m_activePickboxID == i) {
            m_pointList[i] = m_movePts[i] + position - m_moveStartPt;
            UpdateSwitchesPosition();
            UpdatePowerFlowArrowsPosition();
        }
    }
}
bool Line::PickboxContains(wxPoint2DDouble position)
{
    for(int i = 2; i < (int)m_pointList.size() - 2; i++) {
        wxRect2DDouble rect(m_pointList[i].m_x - 5.0, m_pointList[i].m_y - 5.0, 10.0, 10.0);
        if(rect.Contains(position)) {
            m_activePickboxID = i;
            return true;
        }
    }
    return false;
}

void Line::AddPoint(wxPoint2DDouble point)
{
    if(m_parentList.size() != 0) {
        m_pointList.push_back(point);
    }
}

void Line::StartMove(wxPoint2DDouble position)
{
    m_moveStartPt = position;
    m_movePts = m_pointList;
}

void Line::MoveNode(Element* parent, wxPoint2DDouble position)
{
    if(parent) {
        // First bus.
        if(parent == m_parentList[0]) {
            m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
        }
        // Second bus.
        else if(parent == m_parentList[1]) {
            m_pointList[m_pointList.size() - 1] = m_movePts[m_pointList.size() - 1] + position - m_moveStartPt;
        }

        // If the line is selected, move all the points, except the switches and buses points.
        if(m_selected) {
            for(int i = 2; i < (int)m_pointList.size() - 1; i++) {
                m_pointList[i] = m_movePts[i] + position - m_moveStartPt;
            }
        }
    } else {
        // If parent is setted to NULL for the firts time, remove the parent child
        if(m_activeNodeID == 1) {
            m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
            if(m_parentList[0]) {
                m_parentList[0]->RemoveChild(this);
                m_parentList[0] = NULL;
                m_online = false;
            }
        } else if(m_activeNodeID == 2) {
            m_pointList[m_pointList.size() - 1] = m_movePts[m_pointList.size() - 1] + position - m_moveStartPt;
            if(m_parentList[1]) {
                m_parentList[1]->RemoveChild(this);
                m_parentList[1] = NULL;
                m_online = false;
            }
        }
    }

    // Recalculate switches positions
    UpdateSwitchesPosition();
    UpdatePowerFlowArrowsPosition();
}

bool Line::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_ELEMENT, _("Edit line"));
    if(m_activePickboxID == ID_PB_NONE) {
        wxMenuItem* addNodeItem = new wxMenuItem(&menu, ID_LINE_ADD_NODE, _("Insert node"));
        addNodeItem->SetBitmap(wxImage("..\\data\\images\\menu\\addNode16.png"));
        menu.Append(addNodeItem);
    } else {
        wxMenuItem* addNodeItem = new wxMenuItem(&menu, ID_LINE_REMOVE_NODE, _("Remove node"));
        addNodeItem->SetBitmap(wxImage("..\\data\\images\\menu\\removeNode16.png"));
        menu.Append(addNodeItem);
    }
    wxMenuItem* deleteItem = new wxMenuItem(&menu, ID_DELETE, _("Delete"));
    deleteItem->SetBitmap(wxImage("..\\data\\images\\menu\\delete16.png"));
    menu.Append(deleteItem);
    return true;
}

void Line::RemoveNode(wxPoint2DDouble point)
{
    if(PickboxContains(point)) {
        for(int i = 2; i < (int)m_pointList.size() - 2; i++) {
            if(m_activePickboxID == i) {
                m_pointList.erase(m_pointList.begin() + i);
                break;
            }
        }
    }
    UpdateSwitchesPosition();
    UpdatePowerFlowArrowsPosition();
}

void Line::AddNode(wxPoint2DDouble point)
{
    int segmentNumber = 0;
    PointToLineDistance(point, &segmentNumber);
    if(segmentNumber > 0 && segmentNumber < (int)m_pointList.size() - 2) {
        m_pointList.insert(m_pointList.begin() + segmentNumber + 1, point);
    }
    UpdateSwitchesPosition();
    UpdatePowerFlowArrowsPosition();
}

void Line::CalculateBoundaries(wxPoint2DDouble& leftUp, wxPoint2DDouble& rightBottom) const
{
    if(m_pointList.size() > 0) {
        // Check points list boundaries.
        leftUp = m_pointList[0];
        rightBottom = m_pointList[0];
        for(int i = 1; i < (int)m_pointList.size(); i++) {
            if(m_pointList[i].m_x < leftUp.m_x) leftUp.m_x = m_pointList[i].m_x;
            if(m_pointList[i].m_y < leftUp.m_y) leftUp.m_y = m_pointList[i].m_y;
            if(m_pointList[i].m_x > rightBottom.m_x) rightBottom.m_x = m_pointList[i].m_x;
            if(m_pointList[i].m_y > rightBottom.m_y) rightBottom.m_y = m_pointList[i].m_y;
        }
    }
}

bool Line::ShowForm(wxWindow* parent, Element* element)
{
    LineForm* lineForm = new LineForm(parent, this);
    if(lineForm->ShowModal() == wxID_OK) {
        lineForm->Destroy();
        return true;
    }
    lineForm->Destroy();
    return false;
}

void Line::SetNominalVoltage(std::vector<double> nominalVoltage, std::vector<ElectricalUnit> nominalVoltageUnit)
{
    if(nominalVoltage.size() > 0) {
        m_electricaData.nominalVoltage = nominalVoltage[0];
        m_electricaData.nominalVoltageUnit = nominalVoltageUnit[0];
    }
}

bool Line::SetNodeParent(Element* parent)
{
    if(m_activeNodeID == 1 && parent == m_parentList[0]) return false;
    if(m_activeNodeID == 2 && parent == m_parentList[1]) return false;

    if(parent && m_activeNodeID != 0) {
        wxRect2DDouble nodeRect(0, 0, 0, 0);
        if(m_activeNodeID == 1) {
            nodeRect = wxRect2DDouble(m_pointList[0].m_x - 5.0 - m_borderSize, m_pointList[0].m_y - 5.0 - m_borderSize,
                10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);
        }
        if(m_activeNodeID == 2) {
            nodeRect = wxRect2DDouble(m_pointList[m_pointList.size() - 1].m_x - 5.0 - m_borderSize,
                m_pointList[m_pointList.size() - 1].m_y - 5.0 - m_borderSize, 10 + 2.0 * m_borderSize,
                10 + 2.0 * m_borderSize);
        }

        if(parent->Intersects(nodeRect)) {
            // If the line has no parents set the new rated voltage, otherwise check if it's not connecting
            // two different voltages buses
            Bus* parentBus = static_cast<Bus*>(parent);
            if(!m_parentList[0] && !m_parentList[1]) {
                m_electricaData.nominalVoltage = parentBus->GetElectricalData().nominalVoltage;
                m_electricaData.nominalVoltageUnit = parentBus->GetElectricalData().nominalVoltageUnit;
            } else if(m_electricaData.nominalVoltage != parentBus->GetElectricalData().nominalVoltage ||
                m_electricaData.nominalVoltageUnit != parentBus->GetElectricalData().nominalVoltageUnit) {
                wxMessageDialog msgDialog(NULL, _("Unable to connect two buses with different nominal voltages.\n"
                                                  "Use a transformer or edit the bus properties."),
                    _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
                msgDialog.ShowModal();
                m_activeNodeID = 0;
                return false;
            }

            if(m_activeNodeID == 1) {
                // Check if the user is trying to connect the same bus.
                if(m_parentList[1] == parent) {
                    m_activeNodeID = 0;
                    return false;
                }

                m_parentList[0] = parent;

                // Centralize the node on bus.
                wxPoint2DDouble parentPt = parent->RotateAtPosition(
                    m_pointList[0], -parent->GetAngle()); // Rotate click to horizontal position.
                parentPt.m_y = parent->GetPosition().m_y; // Centralize on bus.
                parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());
                m_pointList[0] = parentPt;

                UpdateSwitchesPosition();
                UpdatePowerFlowArrowsPosition();
                return true;
            }
            if(m_activeNodeID == 2) {
                if(m_parentList[0] == parent) {
                    m_activeNodeID = 0;
                    return false;
                }

                m_parentList[1] = parent;

                wxPoint2DDouble parentPt =
                    parent->RotateAtPosition(m_pointList[m_pointList.size() - 1], -parent->GetAngle());
                parentPt.m_y = parent->GetPosition().m_y;
                parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());
                m_pointList[m_pointList.size() - 1] = parentPt;

                UpdateSwitchesPosition();
                UpdatePowerFlowArrowsPosition();
                return true;
            }
        } else {
            if(m_activeNodeID == 1) m_parentList[0] = NULL;
            if(m_activeNodeID == 2) m_parentList[1] = NULL;
        }
    }
    return false;
}

void Line::SetPowerFlowDirection(PowerFlowDirection pfDirection)
{
    m_pfDirection = pfDirection;
    UpdatePowerFlowArrowsPosition();
}

void Line::UpdatePowerFlowArrowsPosition()
{
    std::vector<wxPoint2DDouble> edges;
    switch(m_pfDirection) {
        case PF_NONE: {
            m_powerFlowArrow.clear();
        } break;
        case PF_BUS1_TO_BUS2: {
            for(int i = 1; i < (int)m_pointList.size() - 1; i++) {
                edges.push_back(m_pointList[i]);
            }
        } break;
        case PF_BUS2_TO_BUS1: {
            for(int i = (int)m_pointList.size() - 2; i > 0; i--) {
                edges.push_back(m_pointList[i]);
            }
        } break;
        default:
            break;
    }
    CalculatePowerFlowPts(edges);
}

void Line::RotateNode(Element* parent, bool clockwise)
{
    double rotAngle = m_rotationAngle;
    if(!clockwise) rotAngle = -m_rotationAngle;

    if(parent == m_parentList[0]) {
        m_pointList[0] = parent->RotateAtPosition(m_pointList[0], rotAngle);
    } else if(parent == m_parentList[1]) {
        m_pointList[m_pointList.size() - 1] = parent->RotateAtPosition(m_pointList[m_pointList.size() - 1], rotAngle);
    }
    UpdateSwitchesPosition();
    UpdatePowerFlowArrowsPosition();
}

void Line::SetPointList(std::vector<wxPoint2DDouble> pointList)
{
    m_pointList = pointList;
    UpdateSwitchesPosition();
    UpdatePowerFlowArrowsPosition();
}

Element* Line::GetCopy()
{
    Line* copy = new Line();
    *copy = *this;
    return copy;
}

wxString Line::GetTipText() const
{
    wxString tipText = m_electricaData.name;

    if(m_online) {
        tipText += "\n";
        int busNumber[2];
        busNumber[0] = static_cast<Bus*>(m_parentList[0])->GetElectricalData().number + 1;
        busNumber[1] = static_cast<Bus*>(m_parentList[1])->GetElectricalData().number + 1;

        tipText += _("\nP") + wxString::Format("(%d-%d) = ", busNumber[0], busNumber[1]) +
            wxString::FromDouble(m_electricaData.powerFlow[0].real(), 5) + _(" p.u.");
        tipText += _("\nQ") + wxString::Format("(%d-%d) = ", busNumber[0], busNumber[1]) +
            wxString::FromDouble(m_electricaData.powerFlow[0].imag(), 5) + _(" p.u.");
        tipText += _("\nP") + wxString::Format("(%d-%d) = ", busNumber[1], busNumber[0]) +
            wxString::FromDouble(m_electricaData.powerFlow[1].real(), 5) + _(" p.u.");
        tipText += _("\nQ") + wxString::Format("(%d-%d) = ", busNumber[1], busNumber[0]) +
            wxString::FromDouble(m_electricaData.powerFlow[1].imag(), 5) + _(" p.u.");
    }

    return tipText;
}
