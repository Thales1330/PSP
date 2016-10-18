#include "TransformerForm.h"
#include "Transformer.h"

Transformer::Transformer() : Branch() {}
Transformer::Transformer(wxString name) : Branch() { m_electricalData.name = name; }
Transformer::~Transformer() {}
bool Transformer::AddParent(Element* parent, wxPoint2DDouble position)
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

            return false;
        }
        // Second bus.
        else if(parent != m_parentList[0]) {
            m_parentList.push_back(parent);
            wxPoint2DDouble parentPt =
                parent->RotateAtPosition(position, -parent->GetAngle());        // Rotate click to horizontal position.
            parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
            parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.

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

            m_pointList.push_back(parentPt);  // Last point.
            m_inserted = true;

            wxRect2DDouble genRect(0, 0, 0, 0);
            m_switchRect.push_back(genRect);
            UpdateSwitches();

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
    if(m_inserted) {
        // Draw selection (layer 1).
        if(m_selected) {
            // Push the current matrix on stack.
            glLineWidth(1.5 + m_borderSize * 2.0);
            glColor4d(0.0, 0.5, 1.0, 0.5);
            DrawLine(m_pointList);
            glPushMatrix();
            // Rotate the matrix around the object position.
            glTranslated(m_position.m_x, m_position.m_y, 0.0);
            glRotated(m_angle, 0.0, 0.0, 1.0);
            glTranslated(-m_position.m_x, -m_position.m_y, 0.0);

            DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20 + (m_borderSize + 1.5) / scale, 20,
                       GL_POLYGON);
            DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20 + (m_borderSize + 1.5) / scale, 20,
                       GL_POLYGON);

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
        glColor4d(0.2, 0.2, 0.2, 1.0);
        DrawLine(m_pointList);

        // Draw nodes.
        if(m_pointList.size() > 0) {
            glColor4d(0.2, 0.2, 0.2, 1.0);
            DrawCircle(m_pointList[0], 5.0, 10, GL_POLYGON);
            if(m_inserted) {
                DrawCircle(m_pointList[m_pointList.size() - 1], 5.0, 10, GL_POLYGON);
            }
        }

        DrawSwitches();

        // Push the current matrix on stack.
        glPushMatrix();
        // Rotate the matrix around the object position.
        glTranslated(m_position.m_x, m_position.m_y, 0.0);
        glRotated(m_angle, 0.0, 0.0, 1.0);
        glTranslated(-m_position.m_x, -m_position.m_y, 0.0);

        glColor4d(1.0, 1.0, 1.0, 1.0);
        DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20, 20, GL_POLYGON);
        DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20, 20, GL_POLYGON);

        glColor4d(0.2, 0.2, 0.2, 1.0);
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
    if(m_angle >= 360.0) m_angle = 0.0;

    // Rotate all the points, except the switches and buses points.
    for(int i = 2; i < (int)m_pointList.size() - 2; i++) {
        m_pointList[i] = RotateAtPosition(m_pointList[i], rotAngle);
    }
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
