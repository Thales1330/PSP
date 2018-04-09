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

#include "ConnectionLine.h"

ConnectionLine::ConnectionLine() : ControlElement(-1) {}
ConnectionLine::ConnectionLine(Node* firstNode, int id) : ControlElement(id)
{
    wxPoint2DDouble pt = firstNode->GetPosition();
    m_tmpSndPt = pt;
    for(int i = 0; i < 6; i++) { m_pointList.push_back(pt); }
    m_nodeList.push_back(firstNode);
    firstNode->SetConnected();
}

ConnectionLine::~ConnectionLine() {}
void ConnectionLine::Draw(wxPoint2DDouble translation, double scale) const
{
    // Line selected (Layer 1).
    if(m_selected) {
        glLineWidth(1.5 + m_borderSize * 2.0);
        glColor4dv(m_selectionColour.GetRGBA());
        DrawLine(m_pointList);
    }

    // Draw line (Layer 2)
    glLineWidth(1.5);
    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawLine(m_pointList);

    if(m_type == ELEMENT_LINE) {
        glColor4d(0.0, 0.0, 0.0, 1.0);
        DrawCircle(m_pointList[5], 3, 10, GL_POLYGON);
    }
}

bool ConnectionLine::Contains(wxPoint2DDouble position) const
{
    if(PointToLineDistance(position) < 5.0) { return true; }
    return false;
}

bool ConnectionLine::Intersects(wxRect2DDouble rect) const
{
    for(auto it = m_pointList.begin(); it != m_pointList.end(); ++it) {
        if(rect.Contains(*it)) return true;
    }
    return false;
}

void ConnectionLine::UpdatePoints()
{
    if(m_type == ELEMENT_ELEMENT) {
        bool hasOneNode = true;
        wxPoint2DDouble pt1 = m_nodeList[0]->GetPosition();
        wxPoint2DDouble pt2;
        if(m_nodeList.size() == 1)
            pt2 = m_tmpSndPt;
        else {
            pt2 = m_nodeList[1]->GetPosition();
            hasOneNode = false;
        }
        wxPoint2DDouble midPt = (pt1 + pt2) / 2.0 + wxPoint2DDouble(0.0, m_lineOffset);

        m_pointList[0] = pt1;
        if(m_nodeList[0]->GetAngle() == 0.0)
            m_pointList[1] = m_pointList[0] + wxPoint2DDouble(-10, 0);
        else if(m_nodeList[0]->GetAngle() == 90.0)
            m_pointList[1] = m_pointList[0] + wxPoint2DDouble(0, -10);
        else if(m_nodeList[0]->GetAngle() == 180.0)
            m_pointList[1] = m_pointList[0] + wxPoint2DDouble(10, 0);
        else if(m_nodeList[0]->GetAngle() == 270.0)
            m_pointList[1] = m_pointList[0] + wxPoint2DDouble(0, 10);

        m_pointList[2] = m_pointList[1] + wxPoint2DDouble(0.0, midPt.m_y - m_pointList[1].m_y);

        m_pointList[5] = pt2;
        if(hasOneNode)
            m_pointList[4] = pt2;
        else {
            if(m_nodeList[1]->GetAngle() == 0.0)
                m_pointList[4] = m_pointList[5] + wxPoint2DDouble(-10, 0);
            else if(m_nodeList[1]->GetAngle() == 90.0)
                m_pointList[4] = m_pointList[5] + wxPoint2DDouble(0, -10);
            else if(m_nodeList[1]->GetAngle() == 180.0)
                m_pointList[4] = m_pointList[5] + wxPoint2DDouble(10, 0);
            else if(m_nodeList[1]->GetAngle() == 270.0)
                m_pointList[4] = m_pointList[5] + wxPoint2DDouble(0, 10);
        }

        m_pointList[3] = m_pointList[4] + wxPoint2DDouble(0.0, midPt.m_y - m_pointList[4].m_y);
    } else if(m_type == ELEMENT_LINE) {
        wxPoint2DDouble pt1 = m_nodeList[0]->GetPosition();
        wxPoint2DDouble pt2 = m_parentLine->GetMidPoint();
        wxPoint2DDouble midPt = (pt1 + pt2) / 2.0 + wxPoint2DDouble(0.0, m_lineOffset);

        m_pointList[0] = pt1;
        if(m_nodeList[0]->GetAngle() == 0.0)
            m_pointList[1] = m_pointList[0] + wxPoint2DDouble(-10, 0);
        else if(m_nodeList[0]->GetAngle() == 90.0)
            m_pointList[1] = m_pointList[0] + wxPoint2DDouble(0, -10);
        else if(m_nodeList[0]->GetAngle() == 180.0)
            m_pointList[1] = m_pointList[0] + wxPoint2DDouble(10, 0);
        else if(m_nodeList[0]->GetAngle() == 270.0)
            m_pointList[1] = m_pointList[0] + wxPoint2DDouble(0, 10);

        m_pointList[2] = m_pointList[1] + wxPoint2DDouble(0.0, midPt.m_y - m_pointList[1].m_y);

        m_pointList[5] = pt2;
        if(m_pointList[2].m_y > pt2.m_y) {
            m_pointList[4] = m_pointList[5] + wxPoint2DDouble(0, 10);
        } else {
            m_pointList[4] = m_pointList[5] + wxPoint2DDouble(0, -10);
        }

        m_pointList[3] = m_pointList[4] + wxPoint2DDouble(0.0, midPt.m_y - m_pointList[4].m_y);
    }
    for(auto it = m_childList.begin(), itEnd = m_childList.end(); it != itEnd; ++it) {
        ConnectionLine* child = static_cast<ConnectionLine*>(*it);
        child->UpdatePoints();
    }
}

bool ConnectionLine::AppendNode(Node* node, ControlElement* parent)
{
    if(m_nodeList.size() != 1) return false;
    if(m_nodeList[0] == node) return false;
    if(m_nodeList[0]->GetNodeType() == node->GetNodeType()) return false;
    auto nodeList = parent->GetNodeList();
    for(auto it = nodeList.begin(), itEnd = nodeList.end(); it != itEnd; ++it) {
        Node* parentNode = *it;
        if(parentNode == m_nodeList[0]) return false;
    }

    m_nodeList.push_back(node);
    node->SetConnected();
    return true;
}

void ConnectionLine::Move(wxPoint2DDouble position)
{
    m_lineOffset = m_moveStartOffset + position.m_y - m_moveStartPtY;
    UpdatePoints();
}

void ConnectionLine::StartMove(wxPoint2DDouble position)
{
    m_moveStartPtY = position.m_y;
    m_moveStartOffset = m_lineOffset;
}

wxPoint2DDouble ConnectionLine::GetMidPoint() const { return ((m_pointList[2] + m_pointList[3]) / 2.0); }
bool ConnectionLine::SetParentLine(ConnectionLine* parent)
{
    if(m_nodeList[0]->GetNodeType() != Node::NODE_IN) return false;
    if(!parent) return false;

    m_type = ELEMENT_LINE;
    m_parentLine = parent;
    return true;
}

std::vector<ConnectionLine*> ConnectionLine::GetLineChildList() const
{
    std::vector<ConnectionLine*> childList;
    for(auto it = m_childList.begin(), itEnd = m_childList.end(); it != itEnd; ++it) {
        ConnectionLine* child = static_cast<ConnectionLine*>(*it);
        childList.push_back(child);
    }
    return childList;
}

void ConnectionLine::RemoveParent(Element* parent)
{
    for(auto it = m_parentList.begin(); it != m_parentList.end(); ++it) {
        Element* element = *it;
        if(element == parent) m_parentList.erase(it--);
    }
}

Element* ConnectionLine::GetCopy()
{
    ConnectionLine* copy = new ConnectionLine();
    *copy = *this;
    return copy;
}

bool ConnectionLine::Initialize()
{
    m_solved = false;
    m_output = 0.0;
    m_value = 0.0;
    return true;
}
