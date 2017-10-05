/*
 *  Copyright (C) 2017  Thales Lima Oliveira <thales@ufu.br>
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

#include "TransformerForm.h"
#include "Transformer.h"

Transformer::Transformer() : Branch()
{
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 3; j++) {
            m_electricalData.faultCurrent[i][j] = std::complex<double>(0.0, 0.0);
        }
    }
}
Transformer::Transformer(wxString name) : Branch()
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
            parent->AddChild(this);
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
            parent->AddChild(this);
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
            SetPosition(m_position);  // This method calculates the rectangle propely.
            // Set the "side" points.
            m_pointList.push_back(
                wxPoint2DDouble(m_rect.GetPosition() + wxPoint2DDouble(-10 - m_borderSize, m_height / 2.0)));
            m_pointList.push_back(
                wxPoint2DDouble(m_rect.GetPosition() + wxPoint2DDouble(m_width + 10 + m_borderSize, m_height / 2.0)));

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
    OpenGLColour elementColour;
    if(m_online) {
        if(m_dynEvent)
            elementColour = m_dynamicEventColour;
        else
            elementColour = m_onlineElementColour;
    } else
        elementColour = m_offlineElementColour;

    if(m_inserted) {
        // Draw selection (layer 1).
        if(m_selected) {
            // Push the current matrix on stack.
            glLineWidth(1.5 + m_borderSize * 2.0);
            glColor4dv(m_selectionColour.GetRGBA());
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
        glColor4dv(elementColour.GetRGBA());
        DrawLine(m_pointList);

        // Draw nodes.
        if(m_pointList.size() > 0) {
            glColor4dv(elementColour.GetRGBA());
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

        glColor4dv(elementColour.GetRGBA());
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

void Transformer::StartMove(wxPoint2DDouble position)
{
    m_moveStartPt = position;
    m_movePts = m_pointList;
    m_movePos = m_position;
}

bool Transformer::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_ELEMENT, _("Edit tranformer"));
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
                                      m_pointList[m_pointList.size() - 1].m_y - 5.0 - m_borderSize,
                                      10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);
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

void Transformer::SetPowerFlowDirection(PowerFlowDirection pfDirection)
{
    m_pfDirection = pfDirection;
    UpdatePowerFlowArrowsPosition();
}

Element* Transformer::GetCopy()
{
    Transformer* copy = new Transformer();
    *copy = *this;
    return copy;
}

wxString Transformer::GetTipText() const
{
    wxString tipText = m_electricalData.name;
    wxString primVoltage = StringFromDouble(m_electricalData.primaryNominalVoltage);
    switch(m_electricalData.primaryNominalVoltageUnit) {
        case UNIT_V: {
            primVoltage += _(" V");
        } break;
        case UNIT_kV: {
            primVoltage += _(" kV");
        } break;
        default:
            break;
    }
    wxString secVoltage = StringFromDouble(m_electricalData.secondaryNominalVoltage);
    switch(m_electricalData.secondaryNominalVoltageUnit) {
        case UNIT_V: {
            secVoltage += _(" V");
        } break;
        case UNIT_kV: {
            secVoltage += _(" kV");
        } break;
        default:
            break;
    }

    tipText += "\n" + primVoltage + " / " + secVoltage;

    if(m_online) {
        tipText += "\n";
        int busNumber[2];
        busNumber[0] = static_cast<Bus*>(m_parentList[0])->GetElectricalData().number + 1;
        busNumber[1] = static_cast<Bus*>(m_parentList[1])->GetElectricalData().number + 1;

        tipText += _("\nP") + wxString::Format("(%d-%d) = ", busNumber[0], busNumber[1]) +
                   wxString::FromDouble(m_electricalData.powerFlow[0].real(), 5) + _(" p.u.");
        tipText += _("\nQ") + wxString::Format("(%d-%d) = ", busNumber[0], busNumber[1]) +
                   wxString::FromDouble(m_electricalData.powerFlow[0].imag(), 5) + _(" p.u.");
        tipText += _("\nP") + wxString::Format("(%d-%d) = ", busNumber[1], busNumber[0]) +
                   wxString::FromDouble(m_electricalData.powerFlow[1].real(), 5) + _(" p.u.");
        tipText += _("\nQ") + wxString::Format("(%d-%d) = ", busNumber[1], busNumber[0]) +
                   wxString::FromDouble(m_electricalData.powerFlow[1].imag(), 5) + _(" p.u.");
    }

    return tipText;
}

TransformerElectricalData Transformer::GetPUElectricalData(double systemBasePower)
{
    TransformerElectricalData data = m_electricalData;
    double transformerBasePower = GetValueFromUnit(data.nominalPower, data.nominalPowerUnit);
    double baseVoltage = 0.0;
    if(data.baseVoltage == 0) {
        baseVoltage = GetValueFromUnit(data.primaryNominalVoltage, data.primaryNominalVoltageUnit);
    } else {
        baseVoltage = GetValueFromUnit(data.secondaryNominalVoltage, data.secondaryNominalVoltageUnit);
    }
    double systemBaseImpedance = (baseVoltage * baseVoltage) / systemBasePower;
    double transformerBaseImpedance = (baseVoltage * baseVoltage) / transformerBasePower;

    // Resistance
    double r = data.resistance;
    if(data.resistanceUnit == UNIT_PU) {
        if(data.useTransformerPower) data.resistance = (r * transformerBaseImpedance) / systemBaseImpedance;
    } else {
        data.resistance = r / systemBaseImpedance;
    }
    data.resistanceUnit = UNIT_PU;

    // Indutive reactance
    double x = data.indReactance;
    if(data.indReactanceUnit == UNIT_PU) {
        if(data.useTransformerPower) data.indReactance = (x * transformerBaseImpedance) / systemBaseImpedance;
    } else {
        data.indReactance = x / systemBaseImpedance;
    }
    data.indReactanceUnit = UNIT_PU;

    // Fault

    // Zero seq. resistance
    double r0 = data.zeroResistance;
    if(data.useTransformerPower) data.zeroResistance = (r0 * transformerBaseImpedance) / systemBaseImpedance;

    // Zero seq. ind. reactance
    double x0 = data.zeroIndReactance;
    if(data.useTransformerPower) data.zeroIndReactance = (x0 * transformerBaseImpedance) / systemBaseImpedance;

    // Primary ground resistance
    double rgp = data.primaryGrndResistance;
    if(data.useTransformerPower) data.primaryGrndResistance = (rgp * transformerBaseImpedance) / systemBaseImpedance;

    // Primary ground ind reactance
    double xgp = data.primaryGrndReactance;
    if(data.useTransformerPower) data.primaryGrndReactance = (xgp * transformerBaseImpedance) / systemBaseImpedance;

    // Secondary ground resistance
    double rgs = data.secondaryGrndResistance;
    if(data.useTransformerPower) data.secondaryGrndResistance = (rgs * transformerBaseImpedance) / systemBaseImpedance;

    // Secondary ground ind reactance
    double xgs = data.secondaryGrndReactance;
    if(data.useTransformerPower) data.secondaryGrndReactance = (xgs * transformerBaseImpedance) / systemBaseImpedance;

    if(!m_online) {
        data.powerFlow[0] = std::complex<double>(0, 0);
        data.powerFlow[1] = std::complex<double>(0, 0);
        data.faultCurrent[0][0] = std::complex<double>(0, 0);
        data.faultCurrent[0][1] = std::complex<double>(0, 0);
        data.faultCurrent[0][2] = std::complex<double>(0, 0);
        data.faultCurrent[1][0] = std::complex<double>(0, 0);
        data.faultCurrent[1][1] = std::complex<double>(0, 0);
        data.faultCurrent[1][2] = std::complex<double>(0, 0);
    }

    return data;
}
