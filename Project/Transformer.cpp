#include "TransformerForm.h"
#include "Transformer.h"

Transformer::Transformer()
    : Branch()
{
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 3; j++) {
            m_electricalData.faultCurrent[i][j] = std::complex<double>(0.0, 0.0);
        }
    }
}
Transformer::Transformer(wxString name)
    : Branch()
{
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 3; j++) {
            m_electricalData.faultCurrent[i][j] = std::complex<double>(0.0, 0.0);
        }
    }
    m_electricalData.name = name;
}
Transformer::~Transformer() {}
bool Transformer::AddParent(Element* parent, wxPoint2DDouble position)
{
    if(parent) {
        // First bus.
        if(m_parentList.size() == 0) {
            m_position = position;
            m_parentList.push_back(parent);
            wxPoint2DDouble parentPt =
                parent->RotateAtPosition(position, -parent->GetAngle());       // Rotate click to horizontal position.
            parentPt.m_y = parent->GetPosition().m_y;                          // Centralize on bus.
            parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle()); // Rotate back.
            m_pointList.push_back(parentPt);                                   // First point
            m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_position));

            wxRect2DDouble genRect(0, 0, 0, 0);
            m_switchRect.push_back(genRect);

            return false;
        }
        // Second bus.
        else if(parent != m_parentList[0]) {
            m_parentList.push_back(parent);
            wxPoint2DDouble parentPt =
                parent->RotateAtPosition(position, -parent->GetAngle());       // Rotate click to horizontal position.
            parentPt.m_y = parent->GetPosition().m_y;                          // Centralize on bus.
            parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle()); // Rotate back.

            // Get the average between the two bus points.
            m_position =
                wxPoint2DDouble((m_pointList[0].m_x + parentPt.m_x) / 2.0, (m_pointList[0].m_y + parentPt.m_y) / 2.0);
            // Set the transformer rectangle.
            m_width = 70.0;
            m_height = 40.0;
            m_rect = wxRect2DDouble(m_position.m_x - m_width / 2.0, m_position.m_y - m_height / 2.0, m_width, m_height);
            // Set the "side" points.
            m_pointList.push_back(wxPoint2DDouble(m_rect.GetPosition() + wxPoint2DDouble(-10, m_height / 2.0)));
            m_pointList.push_back(
                wxPoint2DDouble(m_rect.GetPosition() + wxPoint2DDouble(m_width + 10, m_height / 2.0)));

            // Set first switch point.
            wxPoint2DDouble secondPoint = parentPt;
            if(m_pointList.size() > 2) {
                secondPoint = m_pointList[2];
            }
            m_pointList[1] = GetSwitchPoint(m_parentList[0], m_pointList[0], secondPoint);

            // Set the second switch point.
            m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_pointList[m_pointList.size() - 1]));

            m_pointList.push_back(parentPt); // Last point.
            m_inserted = true;

            wxRect2DDouble genRect(0, 0, 0, 0);
            m_switchRect.push_back(genRect);
            UpdateSwitches();
            UpdatePowerFlowArrowsPosition();

            return true;
        }
    }
    return false;
}

bool Transformer::Contains(wxPoint2DDouble position) const
{
    wxPoint2DDouble ptR = RotateAtPosition(position, -m_angle);
    return m_rect.Contains(ptR);
}

void Transformer::Draw(wxPoint2DDouble translation, double scale) const
{
    OpenGLColour* elementColour;
    if(m_online) elementColour = m_onlineElementColour;
    else elementColour = m_offlineElementColour;
    
    if(m_inserted) {
        // Draw selection (layer 1).
        if(m_selected) {
            // Push the current matrix on stack.
            glLineWidth(1.5 + m_borderSize * 2.0);
            glColor4dv(m_selectionColour->GetRGBA());
            DrawLine(m_pointList);
            glPushMatrix();
            // Rotate the matrix around the object position.
            glTranslated(m_position.m_x, m_position.m_y, 0.0);
            glRotated(m_angle, 0.0, 0.0, 1.0);
            glTranslated(-m_position.m_x, -m_position.m_y, 0.0);

            DrawCircle(
                m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20 + (m_borderSize + 1.5) / scale, 20, GL_POLYGON);
            DrawCircle(
                m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20 + (m_borderSize + 1.5) / scale, 20, GL_POLYGON);

            glPopMatrix();

            // Draw nodes selection.
            if(m_pointList.size() > 0) {
                DrawCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
                if(m_inserted) {
                    DrawCircle(m_pointList[m_pointList.size() - 1], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
                }
            }
        }

        // Draw transformer (layer 2).
        // Transformer line
        glLineWidth(1.5);
        glColor4dv(elementColour->GetRGBA());
        DrawLine(m_pointList);

        // Draw nodes.
        if(m_pointList.size() > 0) {
            glColor4dv(elementColour->GetRGBA());
            DrawCircle(m_pointList[0], 5.0, 10, GL_POLYGON);
            if(m_inserted) {
                DrawCircle(m_pointList[m_pointList.size() - 1], 5.0, 10, GL_POLYGON);
            }
        }

        DrawSwitches();
        DrawPowerFlowPts();

        // Push the current matrix on stack.
        glPushMatrix();
        // Rotate the matrix around the object position.
        glTranslated(m_position.m_x, m_position.m_y, 0.0);
        glRotated(m_angle, 0.0, 0.0, 1.0);
        glTranslated(-m_position.m_x, -m_position.m_y, 0.0);

        glColor4d(1.0, 1.0, 1.0, 1.0);
        DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20, 20, GL_POLYGON);
        DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20, 20, GL_POLYGON);

        glColor4dv(elementColour->GetRGBA());
        DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20, 20);
        DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20, 20);

        DrawPoint(m_rect.GetPosition(), 8.0 * scale);

        glPopMatrix();
    }
}

bool Transformer::Intersects(wxRect2DDouble rect) const
{
    if(m_angle == 0.0 || m_angle == 180.0) return m_rect.Intersects(rect);
    return RotatedRectanglesIntersects(m_rect, rect, m_angle, 0.0);
}

void Transformer::Rotate(bool clockwise)
{
    double rotAngle = m_rotationAngle;
    if(!clockwise) rotAngle = -m_rotationAngle;

    m_angle += rotAngle;
    if(m_angle >= 360 || m_angle <= -360) m_angle = 0.0;

    // Rotate all the points, except the switches and buses points.
    for(int i = 2; i < (int)m_pointList.size() - 2; i++) {
        m_pointList[i] = RotateAtPosition(m_pointList[i], rotAngle);
    }
    UpdateSwitchesPosition();
    UpdatePowerFlowArrowsPosition();
}

void Transformer::Move(wxPoint2DDouble position)
{
    SetPosition(m_movePos + position - m_moveStartPt);

    // Move all the points, except the switches and buses points.
    for(int i = 2; i < (int)m_pointList.size() - 2; i++) {
        m_pointList[i] = m_movePts[i] + position - m_moveStartPt;
    }

    if(!m_parentList[0]) {
        m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
    }
    if(!m_parentList[1]) {
        m_pointList[m_pointList.size() - 1] = m_movePts[m_pointList.size() - 1] + position - m_moveStartPt;
    }

    UpdateSwitchesPosition();
    UpdatePowerFlowArrowsPosition();
}

void Transformer::MoveNode(Element* parent, wxPoint2DDouble position)
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

void Transformer::StartMove(wxPoint2DDouble position)
{
    m_moveStartPt = position;
    m_movePts = m_pointList;
    m_movePos = m_position;
}

bool Transformer::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_TRANSFORMER, _("Edit tranformer"));
    GeneralMenuItens(menu);
    return true;
}

bool Transformer::ShowForm(wxWindow* parent, Element* element)
{
    TransformerForm* transfForm = new TransformerForm(parent, this);
    if(transfForm->ShowModal() == wxID_OK) {
        transfForm->Destroy();
        return true;
    }
    transfForm->Destroy();
    return false;
}

void Transformer::SetNominalVoltage(std::vector<double> nominalVoltage, std::vector<ElectricalUnit> nominalVoltageUnit)
{
    if(nominalVoltage.size() == 1) {
        m_electricalData.primaryNominalVoltage = nominalVoltage[0];
        m_electricalData.primaryNominalVoltageUnit = nominalVoltageUnit[0];
    } else if(nominalVoltage.size() == 2) {
        m_electricalData.primaryNominalVoltage = nominalVoltage[0];
        m_electricalData.primaryNominalVoltageUnit = nominalVoltageUnit[0];
        m_electricalData.secondaryNominalVoltage = nominalVoltage[1];
        m_electricalData.secondaryNominalVoltageUnit = nominalVoltageUnit[1];
    }
}

void Transformer::UpdatePowerFlowArrowsPosition()
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

void Transformer::RotateNode(Element* parent, bool clockwise)
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

bool Transformer::SetNodeParent(Element* parent)
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

void Transformer::SetPowerFlowDirection(PowerFlowDirection pfDirection)
{
    m_pfDirection = pfDirection;
    UpdatePowerFlowArrowsPosition();
}
