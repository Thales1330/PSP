#include "Multiplier.h"

Multiplier::Multiplier() : ControlElement()
{
    m_width = m_height = 36.0;
    Node* nodeIn1 = new Node(m_position + wxPoint2DDouble(-m_width / 2, 9 - m_height / 2), Node::NODE_IN, m_borderSize);
    nodeIn1->StartMove(m_position);
    Node* nodeIn2 =
        new Node(m_position + wxPoint2DDouble(-m_width / 2, 27 - m_height / 2), Node::NODE_IN, m_borderSize);
    nodeIn2->StartMove(m_position);
    Node* nodeOut = new Node(m_position + wxPoint2DDouble(m_width / 2, 0), Node::NODE_OUT, m_borderSize);
    nodeOut->SetAngle(180.0);
    nodeOut->StartMove(m_position);
    m_nodeList.push_back(nodeIn1);
    m_nodeList.push_back(nodeIn2);
    m_nodeList.push_back(nodeOut);
}

Multiplier::~Multiplier() {}

void Multiplier::Draw(wxPoint2DDouble translation, double scale) const
{
}

bool Multiplier::ShowForm(wxWindow* parent, Element* element) {}

void Multiplier::Rotate(bool clockwise)
{
    if(clockwise)
        m_angle += 90.0;
    else
        m_angle -= 90.0;
    if(m_angle >= 360.0)
        m_angle = 0.0;
    else if(m_angle < 0)
        m_angle = 270.0;

    UpdatePoints();

    for(auto it = m_nodeList.begin(), itEnd = m_nodeList.end(); it != itEnd; ++it) {
        Node* node = *it;
        node->Rotate(clockwise);
    }
}

void Multiplier::UpdatePoints()
{
}
