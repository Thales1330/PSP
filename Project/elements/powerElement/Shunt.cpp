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

#include "Shunt.h"

Shunt::Shunt() : PowerElement() {}
Shunt::~Shunt() {}
void Shunt::UpdateSwitchesPosition()
{
    if(m_parentList[0]) {
        m_pointList[1] = GetSwitchPoint(m_parentList[0], m_pointList[0], m_pointList[2]);
    } else {
        m_pointList[1] = m_pointList[0];
    }
    UpdateSwitches();
}

void Shunt::Move(wxPoint2DDouble position)
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

void Shunt::AlignToGrid(double gridSize)
{
    if (gridSize <= 0.0) gridSize = 20.0;
    wxPoint2DDouble oldPos = m_position;
    wxPoint2DDouble newPos(std::round(oldPos.m_x / gridSize) * gridSize,
                           std::round(oldPos.m_y / gridSize) * gridSize);
    wxPoint2DDouble delta = newPos - oldPos;
    SetPosition(newPos);
    for (size_t i = 2; i < m_pointList.size(); ++i) {
        m_pointList[i] += delta;
    }

    if (m_parentList.size() > 0 && m_parentList[0] && m_pointList.size() > 0) {
        Element* bus = m_parentList[0];
        if (std::abs(m_angle - 0.0) < 1.0 || std::abs(m_angle - 180.0) < 1.0) {
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

void Shunt::MoveNode(Element* element, wxPoint2DDouble position)
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

void Shunt::StartMove(wxPoint2DDouble position)
{
    m_moveStartPt = position;
    m_movePts = m_pointList;
    m_movePos = m_position;
}

void Shunt::RemoveParent(Element* parent)
{
    if(parent == m_parentList[0]) {
        m_parentList[0] = nullptr;
        m_online = false;
        UpdateSwitchesPosition();
        UpdatePowerFlowArrowsPosition();
    }
}

bool Shunt::NodeContains(wxPoint2DDouble position)
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

bool Shunt::SetNodeParent(Element* parent)
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

void Shunt::UpdateNodes()
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

void Shunt::RotateNode(Element* parent, bool clockwise)
{
    double rotAngle = m_rotationAngle;
    if(!clockwise) rotAngle = -m_rotationAngle;

    if(parent == m_parentList[0]) {
        m_pointList[0] = parent->RotateAtPosition(m_pointList[0], rotAngle);
        UpdateSwitchesPosition();
        UpdatePowerFlowArrowsPosition();
    }
}

//void Shunt::DrawGround(wxPoint2DDouble position) const
//{
//    std::vector<wxPoint2DDouble> groundPts;
//    groundPts.push_back(position);
//    groundPts.push_back(position + wxPoint2DDouble(0, 10));
//    groundPts.push_back(position + wxPoint2DDouble(-10, 10));
//    groundPts.push_back(position + wxPoint2DDouble(10, 10));
//    groundPts.push_back(position + wxPoint2DDouble(-6, 15));
//    groundPts.push_back(position + wxPoint2DDouble(6, 15));
//    groundPts.push_back(position + wxPoint2DDouble(-3, 20));
//    groundPts.push_back(position + wxPoint2DDouble(3, 20));
//
//    DrawLine(groundPts, GL_LINES);
//}

void Shunt::DrawDCGround(wxPoint2DDouble position, wxGraphicsContext* gc) const
{
	std::vector<wxPoint2DDouble> groundPts;
	groundPts.push_back(position);
	groundPts.push_back(position + wxPoint2DDouble(0, 10));
	groundPts.push_back(position + wxPoint2DDouble(-10, 10));
	groundPts.push_back(position + wxPoint2DDouble(10, 10));
	groundPts.push_back(position + wxPoint2DDouble(-6, 15));
	groundPts.push_back(position + wxPoint2DDouble(6, 15));
	groundPts.push_back(position + wxPoint2DDouble(-3, 20));
	groundPts.push_back(position + wxPoint2DDouble(3, 20));

	gc->StrokeLines(2, &groundPts[0]);
	gc->StrokeLines(2, &groundPts[2]);
	gc->StrokeLines(2, &groundPts[4]);
	gc->StrokeLines(2, &groundPts[6]);
}

void Shunt::DrawDCGround(wxPoint2DDouble position, wxDC& dc) const
{
    wxPoint groundPts[8];
    wxPoint2DDouble p;

    p = position;
    groundPts[0] = RotateAround(p, m_position, m_angle);
    p = position + wxPoint2DDouble(0, 10);
    groundPts[1] = RotateAround(p, m_position, m_angle);
    p = position + wxPoint2DDouble(-10, 10);
    groundPts[2] = RotateAround(p, m_position, m_angle);
    p = position + wxPoint2DDouble(10, 10);
    groundPts[3] = RotateAround(p, m_position, m_angle);
    p = position + wxPoint2DDouble(-6, 15);
    groundPts[4] = RotateAround(p, m_position, m_angle);
    p = position + wxPoint2DDouble(6, 15);
    groundPts[5] = RotateAround(p, m_position, m_angle);
    p = position + wxPoint2DDouble(-3, 20);
    groundPts[6] = RotateAround(p, m_position, m_angle);
    p = position + wxPoint2DDouble(3, 20);
    groundPts[7] = RotateAround(p, m_position, m_angle);

    dc.DrawLines(2, &groundPts[0]);
    dc.DrawLines(2, &groundPts[2]);
    dc.DrawLines(2, &groundPts[4]);
    dc.DrawLines(2, &groundPts[6]);
}

void Shunt::UpdatePowerFlowArrowsPosition()
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