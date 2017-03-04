#include "ControlElement.h"

Node::Node(wxPoint2DDouble position, NodeType nodeType, double borderSize)
{
    double totalRadius = m_radius + borderSize;
    m_rect = wxRect2DDouble(position.m_x - totalRadius, position.m_y - totalRadius, totalRadius * 2, totalRadius * 2);
    m_nodeType = nodeType;

    m_triPts.push_back(GetPosition() + wxPoint2DDouble(-m_radius - m_rect.GetSize().GetWidth() / 2, m_radius));
    m_triPts.push_back(GetPosition() + wxPoint2DDouble(-m_radius - m_rect.GetSize().GetWidth() / 2, -m_radius));
    m_triPts.push_back(GetPosition() + wxPoint2DDouble(-m_radius + 1, 0));
}

Node::~Node() {}

void Node::SetPosition(wxPoint2DDouble position)
{
    m_rect = wxRect2DDouble(
        position.m_x - m_rect.m_width / 2, position.m_y - m_rect.m_height / 2, m_rect.m_width, m_rect.m_height);
    m_triPts[0] = GetPosition() + wxPoint2DDouble(-m_radius - m_rect.GetSize().GetWidth() / 2, m_radius);
    m_triPts[1] = GetPosition() + wxPoint2DDouble(-m_radius - m_rect.GetSize().GetWidth() / 2, -m_radius);
    m_triPts[2] = GetPosition() + wxPoint2DDouble(-m_radius + 1, 0);

    // Rotate according to the angle (node rect center as reference)
    if(m_angle != 0.0) RotateTriPt(m_angle);
}

void Node::StartMove(wxPoint2DDouble position)
{
    m_moveStartPt = position;
    m_movePos = m_rect.GetPosition() - wxPoint2DDouble(-m_rect.m_width / 2, -m_rect.m_height / 2);
}

void Node::Move(wxPoint2DDouble position) { SetPosition(m_movePos + position - m_moveStartPt); }

wxPoint2DDouble Node::GetPosition() const
{
    return m_rect.GetPosition() + wxPoint2DDouble(m_rect.GetSize().GetWidth() / 2, m_rect.GetSize().GetHeight() / 2);
}

void Node::RotateTriPt(double angle)
{
    double radAngle = wxDegToRad(angle);
    wxPoint2DDouble rectCenter =
        m_rect.GetPosition() + wxPoint2DDouble(m_rect.GetSize().GetWidth() / 2.0, m_rect.GetSize().GetHeight() / 2.0);
    m_triPts[0] = wxPoint2DDouble(std::cos(radAngle) * (m_triPts[0].m_x - rectCenter.m_x) -
            std::sin(radAngle) * (m_triPts[0].m_y - rectCenter.m_y) + rectCenter.m_x,
        std::sin(radAngle) * (m_triPts[0].m_x - rectCenter.m_x) +
            std::cos(radAngle) * (m_triPts[0].m_y - rectCenter.m_y) + rectCenter.m_y);
    m_triPts[1] = wxPoint2DDouble(std::cos(radAngle) * (m_triPts[1].m_x - rectCenter.m_x) -
            std::sin(radAngle) * (m_triPts[1].m_y - rectCenter.m_y) + rectCenter.m_x,
        std::sin(radAngle) * (m_triPts[1].m_x - rectCenter.m_x) +
            std::cos(radAngle) * (m_triPts[1].m_y - rectCenter.m_y) + rectCenter.m_y);
    m_triPts[2] = wxPoint2DDouble(std::cos(radAngle) * (m_triPts[2].m_x - rectCenter.m_x) -
            std::sin(radAngle) * (m_triPts[2].m_y - rectCenter.m_y) + rectCenter.m_x,
        std::sin(radAngle) * (m_triPts[2].m_x - rectCenter.m_x) +
            std::cos(radAngle) * (m_triPts[2].m_y - rectCenter.m_y) + rectCenter.m_y);
}

void Node::Rotate(bool clockwise)
{
    if(clockwise)
        m_angle += 90.0;
    else
        m_angle -= 90.0;
    if(m_angle >= 360.0)
        m_angle = 0.0;
    else if(m_angle < 0)
        m_angle = 270.0;

    // Update input triangle points.
    m_triPts[0] = GetPosition() + wxPoint2DDouble(-m_radius - m_rect.GetSize().GetWidth() / 2, m_radius);
    m_triPts[1] = GetPosition() + wxPoint2DDouble(-m_radius - m_rect.GetSize().GetWidth() / 2, -m_radius);
    m_triPts[2] = GetPosition() + wxPoint2DDouble(-m_radius + 1, 0);

    // Rotate according to the angle (node rect center as reference)
    if(m_angle != 0.0) RotateTriPt(m_angle);
}

bool Node::Contains(wxPoint2DDouble position) const
{
    if(m_connected) return false;
    return m_rect.Contains(position);
}

ControlElement::ControlElement()
    : Element()
{
}

ControlElement::~ControlElement() {}

void ControlElement::DrawNodes() const
{
    for(auto it = m_nodeList.begin(), itEnd = m_nodeList.end(); it != itEnd; ++it) {
        Node* node = *it;
        DrawCircle(node->GetPosition(), node->GetRadius(), 10, GL_POLYGON);
        if(node->GetNodeType() == Node::NODE_IN) {
            DrawTriangle(node->GetInTrianglePts());
        }
    }
}

void ControlElement::StartMove(wxPoint2DDouble position)
{
    m_moveStartPt = position;
    m_movePos = m_position;
    for(int i = 0; i < (int)m_nodeList.size(); ++i) {
        m_nodeList[i]->StartMove(position);
    }
}

void ControlElement::Move(wxPoint2DDouble position)
{
    SetPosition(m_movePos + position - m_moveStartPt);
    for(int i = 0; i < (int)m_nodeList.size(); ++i) {
        m_nodeList[i]->Move(position);
    }
}
