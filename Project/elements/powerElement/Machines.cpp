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
        parentPt.m_x = std::round(parentPt.m_x / 20.0) * 20.0;             // Snap along bus to grid.
        double halfLength = parent->GetWidth() / 2.0;
        double minX = parent->GetPosition().m_x - halfLength;
        double maxX = parent->GetPosition().m_x + halfLength;
        if (parentPt.m_x < minX) parentPt.m_x = minX;
        if (parentPt.m_x > maxX) parentPt.m_x = maxX;
        parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.

        // Determine orientation based on bus angle and click position
        bool busHorizontal = (std::abs(parent->GetAngle() - 0.0) < 1.0 || std::abs(parent->GetAngle() - 180.0) < 1.0);
        if (busHorizontal) {
            if (position.m_y < parent->GetPosition().m_y) {
                // Above bus: lead points down (+Y, angle 90)
                m_position = parentPt + wxPoint2DDouble(0.0, -80.0);
                m_angle = 90.0;
            }
            else {
                // Below bus: lead points up (-Y, angle 270)
                m_position = parentPt + wxPoint2DDouble(0.0, 80.0);
                m_angle = 270.0;
            }
        }
        else {
            if (position.m_x < parent->GetPosition().m_x) {
                // Left of bus: lead points right (+X, angle 0)
                m_position = parentPt + wxPoint2DDouble(-80.0, 0.0);
                m_angle = 0.0;
            }
            else {
                // Right of bus: lead points left (-X, angle 180)
                m_position = parentPt + wxPoint2DDouble(80.0, 0.0);
                m_angle = 180.0;
            }
        }

        m_width = m_height = 40.0;
        m_rect = wxRect2DDouble(m_position.m_x - 20.0, m_position.m_y - 20.0, m_width, m_height);

        m_pointList.push_back(parentPt);
        wxPoint2DDouble term1 = m_position + RotateLocal(wxPoint2DDouble(40.0, 0.0), m_angle);
        wxPoint2DDouble term2 = m_position + RotateLocal(wxPoint2DDouble(20.0, 0.0), m_angle);
        term1.m_x = std::round(term1.m_x);
        term1.m_y = std::round(term1.m_y);
        term2.m_x = std::round(term2.m_x);
        term2.m_y = std::round(term2.m_y);

        m_pointList.push_back(GetSwitchPoint(parent, parentPt, term1));
        m_pointList.push_back(term1);
        m_pointList.push_back(term2);
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

void Machines::DrawDC(GUIColour* guiColour, wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const
{
	wxColour elementColour;
    if (m_online) {
        if (m_dynEvent)
            elementColour = guiColour->eventElement;
        else
            elementColour = guiColour->enabled;
    }
    else
        elementColour = guiColour->disable;

	if (m_inserted) {
		// Draw Selection (layer 1).
		if (m_selected) {
			gc->SetPen(wxPen(guiColour->selection, 2 + m_borderSize * 2.0));
			gc->SetBrush(*wxTRANSPARENT_BRUSH);
			gc->StrokeLines(m_pointList.size(), &m_pointList[0]);

			gc->SetPen(*wxTRANSPARENT_PEN);
			gc->SetBrush(wxBrush(guiColour->selection));
            DrawDCCircle(m_position, 20.0 + (m_borderSize + 1.5) / scale, 20, gc);

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
		DrawDCCircle(m_position, 20.0, 20.0, gc);

		DrawDCSwitches(guiColour, gc);
		DrawDCPowerFlowPts(guiColour, gc);

		gc->SetPen(*wxTRANSPARENT_PEN);
		gc->SetBrush(wxBrush(guiColour->background));
		DrawDCCircle(m_position, 20.0, 20.0, gc);

		gc->SetPen(wxPen(elementColour, 2));
		gc->SetBrush(*wxTRANSPARENT_BRUSH);
        DrawDCCircle(m_position, 20.0, 20.0, gc);

		// Draw machine symbol.
		DrawDCSymbol(gc);
	}
}

void Machines::DrawDC(GUIColour* guiColour, wxPoint2DDouble translation, double scale, wxDC& dc) const
{
    wxColour elementColour;
    if (m_online) {
        if (m_dynEvent)
            elementColour = guiColour->eventElement;
        else
            elementColour = guiColour->enabled;
    }
    else
        elementColour = guiColour->disable;

	std::vector<wxPoint> pointListInt;
    for(auto& pt : m_pointList) {
        pointListInt.emplace_back(static_cast<int>(pt.m_x), static_cast<int>(pt.m_y));
	}

    if (m_inserted) {
        // Draw Selection (layer 1).
        if (m_selected) {
            dc.SetPen(wxPen(guiColour->selection, 2 + m_borderSize * 2.0));
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.DrawLines(pointListInt.size(), &pointListInt[0]);

            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.SetBrush(wxBrush(guiColour->selection));
            DrawDCCircle(m_position, 20.0 + (m_borderSize + 1.5) / scale, dc);

            // Draw nodes selection.
            DrawDCCircle(m_pointList[0], 5.0 + m_borderSize / scale, dc);
        }

        // Draw Machines (layer 2).
        // Draw node.
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxBrush(elementColour));
        DrawDCCircle(m_pointList[0], 5.0, dc);

        dc.SetPen(wxPen(wxColour(elementColour), 2));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawLines(pointListInt.size(), &pointListInt[0]);
        DrawDCCircle(m_position, 20.0, dc);

        DrawDCSwitches(guiColour, dc);
        DrawDCPowerFlowPts(guiColour, dc);

        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxBrush(guiColour->background));
        DrawDCCircle(m_position, 20.0, dc);

        dc.SetPen(wxPen(elementColour, 2));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        DrawDCCircle(m_position, 20.0, dc);

        // Draw machine symbol.
        DrawDCSymbol(dc);
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
    if (m_pointList.size() >= 4) {
        wxPoint2DDouble t1 = m_position + RotateLocal(wxPoint2DDouble(40.0, 0.0), m_angle);
        wxPoint2DDouble t2 = m_position + RotateLocal(wxPoint2DDouble(20.0, 0.0), m_angle);
        m_pointList[2] = wxPoint2DDouble(std::round(t1.m_x), std::round(t1.m_y));
        m_pointList[3] = wxPoint2DDouble(std::round(t2.m_x), std::round(t2.m_y));
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
    while (m_angle >= 360.0) m_angle -= 360.0;
    while (m_angle < 0.0) m_angle += 360.0;

    if (std::abs(m_angle - 90.0) < 1e-4) m_angle = 90.0;
    else if (std::abs(m_angle - 180.0) < 1e-4) m_angle = 180.0;
    else if (std::abs(m_angle - 270.0) < 1e-4) m_angle = 270.0;
    else if (std::abs(m_angle - 0.0) < 1e-4 || std::abs(m_angle - 360.0) < 1e-4) m_angle = 0.0;

    if (m_pointList.size() >= 4) {
        wxPoint2DDouble t1 = m_position + RotateLocal(wxPoint2DDouble(40.0, 0.0), m_angle);
        wxPoint2DDouble t2 = m_position + RotateLocal(wxPoint2DDouble(20.0, 0.0), m_angle);
        m_pointList[2] = wxPoint2DDouble(std::round(t1.m_x), std::round(t1.m_y));
        m_pointList[3] = wxPoint2DDouble(std::round(t2.m_x), std::round(t2.m_y));
    }
    UpdateSwitchesPosition();
    UpdatePowerFlowArrowsPosition();
}

void Machines::AlignToGrid(double gridSize)
{
    if (gridSize <= 0.0) gridSize = 20.0;
    m_position.m_x = std::round(m_position.m_x / gridSize) * gridSize;
    m_position.m_y = std::round(m_position.m_y / gridSize) * gridSize;
    m_width = 40.0;
    m_height = 40.0;
    SetPosition(m_position);

    if (m_pointList.size() >= 4) {
        wxPoint2DDouble t1 = m_position + RotateLocal(wxPoint2DDouble(40.0, 0.0), m_angle);
        wxPoint2DDouble t2 = m_position + RotateLocal(wxPoint2DDouble(20.0, 0.0), m_angle);
        m_pointList[2] = wxPoint2DDouble(std::round(t1.m_x), std::round(t1.m_y));
        m_pointList[3] = wxPoint2DDouble(std::round(t2.m_x), std::round(t2.m_y));
    }

    if (m_parentList.size() > 0 && m_parentList[0] && m_pointList.size() > 0) {
        Element* bus = m_parentList[0];
        if (std::abs(m_angle - 90.0) < 1.0 || std::abs(m_angle - 270.0) < 1.0) {
            wxPoint2DDouble loc = bus->RotateAtPosition(wxPoint2DDouble(m_position.m_x, bus->GetPosition().m_y), -bus->GetAngle());
            double halfW = bus->GetWidth() / 2.0 + 2.0;
            if (std::abs(loc.m_x - bus->GetPosition().m_x) <= halfW) {
                m_pointList[0].m_x = m_position.m_x;
            }
            m_pointList[0].m_y = std::round(m_pointList[0].m_y / gridSize) * gridSize;
        }
        else {
            wxPoint2DDouble loc = bus->RotateAtPosition(wxPoint2DDouble(bus->GetPosition().m_x, m_position.m_y), -bus->GetAngle());
            double halfW = bus->GetWidth() / 2.0 + 2.0;
            if (std::abs(loc.m_x - bus->GetPosition().m_x) <= halfW) {
                m_pointList[0].m_y = m_position.m_y;
            }
            m_pointList[0].m_x = std::round(m_pointList[0].m_x / gridSize) * gridSize;
        }
    }
    else if (m_pointList.size() > 0) {
        m_pointList[0].m_x = std::round(m_pointList[0].m_x / gridSize) * gridSize;
        m_pointList[0].m_y = std::round(m_pointList[0].m_y / gridSize) * gridSize;
    }

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
