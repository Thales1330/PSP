#include "ControlElement.h"

Node::Node(wxPoint2DDouble position, NodeType nodeType, double borderSize)
{
    m_rect = wxRect2DDouble(position.m_x, position.m_y, 2 * m_radius + 2 * borderSize, 2 * m_radius + 2 * borderSize);
    m_nodeType = nodeType;

    m_triPts.push_back(GetPosition() + wxPoint2DDouble(-m_radius - m_rect.GetSize().GetWidth() / 2, m_radius));
    m_triPts.push_back(GetPosition() + wxPoint2DDouble(-m_radius - m_rect.GetSize().GetWidth() / 2, -m_radius));
    m_triPts.push_back(GetPosition() + wxPoint2DDouble(-m_radius + 1, 0));
}

Node::~Node() {}

void Node::SetPosition(wxPoint2DDouble position)
{
    m_rect = wxRect2DDouble(position.m_x, position.m_y, m_rect.m_width, m_rect.m_height);
}

void Node::StartMove(wxPoint2DDouble position)
{
    m_moveStartPt = position;
    m_movePos = m_rect.GetPosition();
    m_triPtsMovePos = m_triPts;
}

void Node::Move(wxPoint2DDouble position)
{
    SetPosition(m_movePos + position - m_moveStartPt);
    m_triPts[0] = m_triPtsMovePos[0] + position - m_moveStartPt;
    m_triPts[1] = m_triPtsMovePos[1] + position - m_moveStartPt;
    m_triPts[2] = m_triPtsMovePos[2] + position - m_moveStartPt;
}

ControlElement::ControlElement()
    : Element()
{
}

ControlElement::~ControlElement() {}

void ControlElement::DrawNodes() const
{
    for(auto it = m_nodeList.begin(), itEnd = m_nodeList.end(); it != itEnd; ++it) {
        Node node = *it;
        DrawCircle(node.GetPosition(), node.GetRadius(), 10, GL_POLYGON);
        if(node.GetNodeType() == Node::NODE_IN) {
            DrawTriangle(node.GetInTrianglePts());
        }
    }
}

void ControlElement::StartMove(wxPoint2DDouble position)
{
    m_moveStartPt = position;
    m_movePos = m_position;
    for(int i = 0; i < (int)m_nodeList.size(); ++i) {
        m_nodeList[i].StartMove(position);
    }
}

void ControlElement::Move(wxPoint2DDouble position)
{
    SetPosition(m_movePos + position - m_moveStartPt);
    for(int i = 0; i < (int)m_nodeList.size(); ++i) {
        m_nodeList[i].Move(position);
    }
}
