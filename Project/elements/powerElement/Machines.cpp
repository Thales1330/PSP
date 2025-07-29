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

#include "Machines.h"

#include "../controlElement/ControlElementContainer.h"
#include "../controlElement/ControlElementSolver.h"

Machines::Machines() : PowerElement() {}
bool Machines::AddParent(Element* parent, wxPoint2DDouble position)
{
    if(parent) {
        m_parentList.push_back(parent);
        parent->AddChild(this);
        wxPoint2DDouble parentPt =
            parent->RotateAtPosition(position, -parent->GetAngle());        // Rotate click to horizontal position.
        parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
        parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.

        m_position = parentPt + wxPoint2DDouble(-100.0, 0.0);  // Shifts the position to the left of the bus.
        m_width = m_height = 50.0;
        m_rect = wxRect2DDouble(m_position.m_x - 25.0, m_position.m_y - 25.0, m_width, m_height);

        m_pointList.push_back(parentPt);
        m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_position));
        m_pointList.push_back(m_position + wxPoint2DDouble(35.0, 0.0));
        m_pointList.push_back(m_position + wxPoint2DDouble(25.0, 0.0));
        m_inserted = true;

        wxRect2DDouble genRect(0, 0, 0, 0);
        m_switchRect.push_back(genRect);  // Push a general rectangle.
        UpdateSwitches();
        UpdatePowerFlowArrowsPosition();
        return true;
    }
    return false;
}

//void Machines::Draw(wxPoint2DDouble translation, double scale) const
//{
//    OpenGLColour elementColour;
//    if(m_online) {
//        if(m_dynEvent)
//            elementColour = m_dynamicEventColour;
//        else
//            elementColour = m_onlineElementColour;
//    } else
//        elementColour = m_offlineElementColour;
//
//    if(m_inserted) {
//        // Draw Selection (layer 1).
//        if(m_selected) {
//            glLineWidth(1.5 + m_borderSize * 2.0);
//            glColor4dv(m_selectionColour.GetRGBA());
//            DrawCircle(m_position, 25.0 + (m_borderSize + 1.5) / scale, 20, GL_POLYGON);
//            DrawLine(m_pointList);
//
//            // Draw node selection.
//            DrawCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
//        }
//
//        // Draw Machines (layer 2).
//        glLineWidth(1.5);
//
//        // Draw node.
//        glColor4dv(elementColour.GetRGBA());
//        DrawCircle(m_pointList[0], 5.0, 10, GL_POLYGON);
//
//        DrawLine(m_pointList);
//
//        DrawSwitches();
//        DrawPowerFlowPts();
//
//        glColor4d(1.0, 1.0, 1.0, 1.0);
//        DrawCircle(m_position, 25.0, 20, GL_POLYGON);
//
//        glColor4dv(elementColour.GetRGBA());
//        DrawCircle(m_position, 25.0, 20);
//
//        // Draw machine symbol.
//        glLineWidth(2.0);
//        DrawSymbol();
//    }
//}

void Machines::DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const
{
	wxColour elementColour;
	if (m_online) {
		if (m_dynEvent)
			elementColour = m_dynamicEventColour;
		else
			elementColour = m_onlineElementColour;
	}
	else
		elementColour = m_offlineElementColour;

	if (m_inserted) {
		// Draw Selection (layer 1).
		if (m_selected) {
			gc->SetPen(wxPen(m_selectionColour, 2 + m_borderSize * 2.0));
			gc->SetBrush(*wxTRANSPARENT_BRUSH);
			gc->StrokeLines(m_pointList.size(), &m_pointList[0]);

			gc->SetPen(*wxTRANSPARENT_PEN);
			gc->SetBrush(wxBrush(m_selectionColour));
            DrawDCCircle(m_position, 25.0 + (m_borderSize + 1.5) / scale, 20, gc);

            // Draw nodes selection.
			DrawDCCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, gc);
		}

		// Draw Machines (layer 2).
		// Draw node.
		gc->SetPen(*wxTRANSPARENT_PEN);
		gc->SetBrush(wxBrush(elementColour));
		DrawDCCircle(m_pointList[0], 5.0, 10, gc);

		gc->SetPen(wxPen(wxColour(elementColour), 2));
		gc->SetBrush(*wxTRANSPARENT_BRUSH);
		gc->StrokeLines(m_pointList.size(), &m_pointList[0]);
		DrawDCCircle(m_position, 25.0, 20.0, gc);

		DrawDCSwitches(gc);
		DrawDCPowerFlowPts(gc);

		gc->SetPen(*wxTRANSPARENT_PEN);
		gc->SetBrush(*wxWHITE_BRUSH);
		DrawDCCircle(m_position, 25.0, 20.0, gc);

		gc->SetPen(wxPen(elementColour, 2));
		gc->SetBrush(*wxTRANSPARENT_BRUSH);
        DrawDCCircle(m_position, 25.0, 20.0, gc);

		// Draw machine symbol.
		DrawDCSymbol(gc);
	}
}

void Machines::UpdateSwitchesPosition()
{
    if(m_parentList[0]) {
        m_pointList[1] = GetSwitchPoint(m_parentList[0], m_pointList[0], m_pointList[2]);
    } else {
        m_pointList[1] = m_pointList[0];
    }
    UpdateSwitches();
}

void Machines::Move(wxPoint2DDouble position)
{
    SetPosition(m_movePos + position - m_moveStartPt);
    for(int i = 2; i < (int)m_pointList.size(); i++) {
        m_pointList[i] = m_movePts[i] + position - m_moveStartPt;
    }
    if(!m_parentList[0]) {
        m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
    }
    UpdateSwitchesPosition();
    UpdatePowerFlowArrowsPosition();
}

void Machines::MoveNode(Element* element, wxPoint2DDouble position)
{
    if(element) {
        if(element == m_parentList[0]) {
            m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
        }
    } else {
        if(m_activeNodeID == 1) {
            m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
            if(m_parentList[0]) {
                m_parentList[0]->RemoveChild(this);
                m_parentList[0] = nullptr;
                m_online = false;
            }
        }
    }

    // Recalculate switches positions
    UpdateSwitchesPosition();
    UpdatePowerFlowArrowsPosition();
}

void Machines::StartMove(wxPoint2DDouble position)
{
    m_moveStartPt = position;
    m_movePts = m_pointList;
    m_movePos = m_position;
}

void Machines::RotateNode(Element* parent, bool clockwise)
{
    double rotAngle = m_rotationAngle;
    if(!clockwise) rotAngle = -m_rotationAngle;

    if(parent == m_parentList[0]) {
        m_pointList[0] = parent->RotateAtPosition(m_pointList[0], rotAngle);
        UpdateSwitchesPosition();
        UpdatePowerFlowArrowsPosition();
    }
}

void Machines::RemoveParent(Element* parent)
{
    if(parent == m_parentList[0]) {
        m_parentList[0] = nullptr;
        m_online = false;
        UpdateSwitchesPosition();
        UpdatePowerFlowArrowsPosition();
    }
}

bool Machines::NodeContains(wxPoint2DDouble position)
{
    wxRect2DDouble nodeRect(m_pointList[0].m_x - 5.0 - m_borderSize, m_pointList[0].m_y - 5.0 - m_borderSize,
                            10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);

    if(nodeRect.Contains(position)) {
        m_activeNodeID = 1;
        return true;
    }

    m_activeNodeID = 0;
    return false;
}

bool Machines::SetNodeParent(Element* parent)
{
    if(parent && m_activeNodeID != 0) {
        wxRect2DDouble nodeRect(m_pointList[0].m_x - 5.0 - m_borderSize, m_pointList[0].m_y - 5.0 - m_borderSize,
                                10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);

        if(parent->Intersects(nodeRect)) {
            m_parentList[0] = parent;

            // Centralize the node on bus.
            wxPoint2DDouble parentPt =
                parent->RotateAtPosition(m_pointList[0], -parent->GetAngle());  // Rotate click to horizontal position.
            parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
            parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());
            m_pointList[0] = parentPt;

            UpdateSwitchesPosition();
            UpdatePowerFlowArrowsPosition();
            return true;
        } else {
            m_parentList[0] = nullptr;
            m_online = false;
        }
    }
    return false;
}

void Machines::UpdateNodes()
{
    if(m_parentList[0]) {
        wxRect2DDouble nodeRect(m_pointList[0].m_x - 5.0 - m_borderSize, m_pointList[0].m_y - 5.0 - m_borderSize,
                                10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);

        if(!m_parentList[0]->Intersects(nodeRect)) {
            m_parentList[0]->RemoveChild(this);
            m_parentList[0] = nullptr;
            m_online = false;
            UpdateSwitchesPosition();
            UpdatePowerFlowArrowsPosition();
        }
    }
}

void Machines::Rotate(bool clockwise)
{
    double rotAngle = m_rotationAngle;
    if(!clockwise) rotAngle = -m_rotationAngle;

    m_angle += rotAngle;
    if(m_angle >= 360 || m_angle <= -360) m_angle = 0.0;
    m_pointList[2] = RotateAtPosition(m_pointList[2], rotAngle);
    m_pointList[3] = RotateAtPosition(m_pointList[3], rotAngle);
    UpdateSwitchesPosition();
    UpdatePowerFlowArrowsPosition();
}

void Machines::UpdatePowerFlowArrowsPosition()
{
    std::vector<wxPoint2DDouble> edges;
    switch(m_pfDirection) {
        case PowerFlowDirection::PF_NONE: {
            m_powerFlowArrow.clear();
        } break;
        case PowerFlowDirection::PF_TO_BUS: {
            edges.push_back(m_pointList[2]);
            edges.push_back(m_pointList[1]);
        } break;
        case PowerFlowDirection::PF_TO_ELEMENT: {
            edges.push_back(m_pointList[1]);
            edges.push_back(m_pointList[2]);
        } break;
        default:
            break;
    }

    CalculatePowerFlowPts(edges);
}

void Machines::SetPowerFlowDirection(PowerFlowDirection pfDirection)
{
    m_pfDirection = pfDirection;
    UpdatePowerFlowArrowsPosition();
}
