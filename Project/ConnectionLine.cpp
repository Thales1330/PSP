#include "ConnectionLine.h"

ConnectionLine::ConnectionLine(Node* firstNode)
    : ControlElement()
{
    wxPoint2DDouble pt = firstNode->GetPosition();
    m_tmpSndPt = pt;
    for(int i = 0; i < 6; i++) {
        m_pointList.push_back(pt);
    }
    m_nodeList.push_back(firstNode);
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
}

bool ConnectionLine::Contains(wxPoint2DDouble position) const
{
    if(PointToLineDistance(position) < 5.0) {
        return true;
    }
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
