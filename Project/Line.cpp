#include "Line.h"

Line::Line() : Branch() {}
Line::Line(wxString name) : Branch() { m_electricaData.name = name; }
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
        glColor4d(0.0, 0.5, 1.0, 0.5);
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
    glColor4d(0.2, 0.2, 0.2, 1.0);
    DrawLine(pointList);

    if(m_inserted){
        DrawSwitches();
        DrawPowerFlowPts();
    }

    // Draw nodes.
    if(pointList.size() > 0) {
        glColor4d(0.2, 0.2, 0.2, 1.0);
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
            wxPoint2DDouble parentPt =
                parent->RotateAtPosition(position, -parent->GetAngle());        // Rotate click to horizontal position.
            parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
            parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.
            m_pointList.push_back(parentPt);                                    // First point
            m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_position));

            wxRect2DDouble genRect(0, 0, 0, 0);
            m_switchRect.push_back(genRect);
            UpdateSwitches();

            Bus* parentBus = (Bus*)parent;
            m_electricaData.nominalVoltage = parentBus->GetEletricalData().nominalVoltage;
            m_electricaData.nominalVoltageUnit = parentBus->GetEletricalData().nominalVoltageUnit;

            return false;
        }
        // Second bus.
        else if(parent != m_parentList[0]) {
            Bus* parentBus = (Bus*)parent;
            if(m_electricaData.nominalVoltage != parentBus->GetEletricalData().nominalVoltage ||
               m_electricaData.nominalVoltageUnit != parentBus->GetEletricalData().nominalVoltageUnit) {
                wxMessageDialog msgDialog(NULL, _("Unable to connect two buses with different nominal voltages.\n"
                                                  "Use a transformer or edit the bus properties."),
                                          _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
                msgDialog.ShowModal();
                return false;
            }

            m_parentList.push_back(parent);
            wxPoint2DDouble parentPt =
                parent->RotateAtPosition(position, -parent->GetAngle());        // Rotate click to horizontal position.
            parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
            parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.

            // Set first switch point.
            wxPoint2DDouble secondPoint = parentPt;
            if(m_pointList.size() > 2) {
                secondPoint = m_pointList[2];
            }
            m_pointList[1] = GetSwitchPoint(m_parentList[0], m_pointList[0], secondPoint);

            // Set the second switch point.
            m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_pointList[m_pointList.size() - 1]));

            m_pointList.push_back(parentPt);  // Last point.

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
            for(int i = 2; i < (int)m_pointList.size() - 2; i++) {
                m_pointList[i] = m_movePts[i] + position - m_moveStartPt;
            }
        }
    } else {
        if(m_activeNodeID == 1) {
            m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
            m_parentList[0] = NULL;
            m_online = false;
        } else if(m_activeNodeID == 2) {
            m_pointList[m_pointList.size() - 1] = m_movePts[m_pointList.size() - 1] + position - m_moveStartPt;
            m_parentList[1] = NULL;
            m_online = false;
        }
    }

    // Recalculate switches positions
    UpdateSwitchesPosition();
    UpdatePowerFlowArrowsPosition();
}

double Line::PointToLineDistance(wxPoint2DDouble point, int* segmentNumber) const
{
    //[Ref] http://geomalgorithms.com/a02-_lines.html
    double distance = 100.0;  // Big initial distance.
    wxPoint2DDouble p0 = point;

    for(int i = 1; i < (int)m_pointList.size() - 2; i++) {
        double d = 0.0;

        wxPoint2DDouble p1 = m_pointList[i];
        wxPoint2DDouble p2 = m_pointList[i + 1];

        wxPoint2DDouble v = p2 - p1;
        wxPoint2DDouble w = p0 - p1;

        double c1 = w.m_x * v.m_x + w.m_y * v.m_y;
        double c2 = v.m_x * v.m_x + v.m_y * v.m_y;

        if(c1 <= 0.0) {
            d = std::sqrt(std::pow(p0.m_y - p1.m_y, 2) + std::pow(p0.m_x - p1.m_x, 2));
        } else if(c2 <= c1) {
            d = std::sqrt(std::pow(p0.m_y - p2.m_y, 2) + std::pow(p0.m_x - p2.m_x, 2));
        } else {
            d = std::abs((p2.m_y - p1.m_y) * p0.m_x - (p2.m_x - p1.m_x) * p0.m_y + p2.m_x * p1.m_y - p2.m_y * p1.m_x) /
                std::sqrt(std::pow(p2.m_y - p1.m_y, 2) + std::pow(p2.m_x - p1.m_x, 2));
        }
        if(d < distance) {
            distance = d;
            if(segmentNumber) *segmentNumber = i;
        }
    }

    return distance;
}

bool Line::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_LINE, _("Edit line"));
    if(m_activePickboxID == ID_PB_NONE) {
        menu.Append(ID_LINE_ADD_NODE, _("Insert node"));
    } else {
        menu.Append(ID_LINE_REMOVE_NODE, _("Remove node"));
    }
    menu.Append(ID_DELETE, _("Delete"));
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
                                      m_pointList[m_pointList.size() - 1].m_y - 5.0 - m_borderSize,
                                      10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);
        }

        if(parent->Intersects(nodeRect)) {
            // If the line has no parents set the new nominal voltage, otherwise check if it's not connecting
            // two different voltages buses
            Bus* parentBus = (Bus*)parent;
            if(!m_parentList[0] && !m_parentList[1]) {
                m_electricaData.nominalVoltage = parentBus->GetEletricalData().nominalVoltage;
                m_electricaData.nominalVoltageUnit = parentBus->GetEletricalData().nominalVoltageUnit;
            } else if(m_electricaData.nominalVoltage != parentBus->GetEletricalData().nominalVoltage ||
                      m_electricaData.nominalVoltageUnit != parentBus->GetEletricalData().nominalVoltageUnit) {
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
                    m_pointList[0], -parent->GetAngle());  // Rotate click to horizontal position.
                parentPt.m_y = parent->GetPosition().m_y;  // Centralize on bus.
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
	}
    else if(parent == m_parentList[1])
	{
	    m_pointList[m_pointList.size() - 1] =
	        parent->RotateAtPosition(m_pointList[m_pointList.size() - 1], rotAngle);
	}
    UpdateSwitchesPosition();
    UpdatePowerFlowArrowsPosition();
}
