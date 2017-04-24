#include "Multiplier.h"

Multiplier::Multiplier(int id) : ControlElement(id)
{
    m_width = m_height = 36.0;
    Node* nodeIn1 = new Node(m_position + wxPoint2DDouble(-18, -9), Node::NODE_IN, m_borderSize);
    nodeIn1->StartMove(m_position);
    Node* nodeIn2 =
        new Node(m_position + wxPoint2DDouble(-18, 9), Node::NODE_IN, m_borderSize);
    nodeIn2->StartMove(m_position);
    Node* nodeOut = new Node(m_position + wxPoint2DDouble(18, 0), Node::NODE_OUT, m_borderSize);
    nodeOut->SetAngle(180.0);
    nodeOut->StartMove(m_position);
    m_nodeList.push_back(nodeIn1);
    m_nodeList.push_back(nodeIn2);
    m_nodeList.push_back(nodeOut);
}

Multiplier::~Multiplier() {}

void Multiplier::Draw(wxPoint2DDouble translation, double scale) const
{
    glLineWidth(1.0);
    if(m_selected) {
        glColor4dv(m_selectionColour.GetRGBA());
        double borderSize = (m_borderSize * 2.0 + 1.0) / scale;
        DrawRectangle(m_position, m_width + borderSize, m_height + borderSize);
    }
    glColor4d(1.0, 1.0, 1.0, 1.0);
    DrawRectangle(m_position, m_width, m_height);
    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawRectangle(m_position, m_width, m_height, GL_LINE_LOOP);
    
    // Plot x.
    glLineWidth(2.0);
    std::vector<wxPoint2DDouble> xSymbol;
    xSymbol.push_back(m_position + wxPoint2DDouble(-5, -5));
    xSymbol.push_back(m_position + wxPoint2DDouble(5, 5));
    xSymbol.push_back(m_position + wxPoint2DDouble(-5, 5));
    xSymbol.push_back(m_position + wxPoint2DDouble(5, -5));
    glColor4d(0.0, 0.3, 1.0, 1.0);
    DrawLine(xSymbol, GL_LINES);

    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawNodes();
}

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
    if(m_angle == 0.0) {
        m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(-18, -9));
        m_nodeList[1]->SetPosition(m_position + wxPoint2DDouble(-18, 9));
        m_nodeList[2]->SetPosition(m_position + wxPoint2DDouble(18, 0));
    } else if(m_angle == 90.0) {
        m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(9, -18));
        m_nodeList[1]->SetPosition(m_position + wxPoint2DDouble(-9, -18));
        m_nodeList[2]->SetPosition(m_position + wxPoint2DDouble(0, 18));
    } else if(m_angle == 180.0) {
        m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(18, 9));
        m_nodeList[1]->SetPosition(m_position + wxPoint2DDouble(18, -9));
        m_nodeList[2]->SetPosition(m_position + wxPoint2DDouble(-18, 0));
    } else if(m_angle == 270.0) {
        m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(-9, 18));
        m_nodeList[1]->SetPosition(m_position + wxPoint2DDouble(9, 18));
        m_nodeList[2]->SetPosition(m_position + wxPoint2DDouble(0, -18));
    }
}
