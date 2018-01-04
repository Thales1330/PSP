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

#include "ControlElement.h"
#ifdef USING_WX_3_0_X
#include "DegreesAndRadians.h"
#endif

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
    m_rect = wxRect2DDouble(position.m_x - m_rect.m_width / 2, position.m_y - m_rect.m_height / 2, m_rect.m_width,
                            m_rect.m_height);
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

ControlElement::ControlElement(int id) : Element() { m_elementID = id; }
ControlElement::~ControlElement() {}
void ControlElement::DrawNodes() const
{
    for(auto it = m_nodeList.begin(), itEnd = m_nodeList.end(); it != itEnd; ++it) {
        Node* node = *it;
        DrawCircle(node->GetPosition(), node->GetRadius(), 10, GL_POLYGON);
        if(node->GetNodeType() == Node::NODE_IN) { DrawTriangle(node->GetInTrianglePts()); }
    }
}

void ControlElement::StartMove(wxPoint2DDouble position)
{
    m_moveStartPt = position;
    m_movePos = m_position;
    for(int i = 0; i < (int)m_nodeList.size(); ++i) { m_nodeList[i]->StartMove(position); }
}

void ControlElement::Move(wxPoint2DDouble position)
{
    SetPosition(m_movePos + position - m_moveStartPt);
    for(int i = 0; i < (int)m_nodeList.size(); ++i) { m_nodeList[i]->Move(position); }
}

bool ControlElement::Solve(double* input, double timeStep)
{
    if(input)
        m_output = input[0];
    else
        m_output = 0.0;
    return true;
}

void ControlElement::ReplaceNode(Node* oldNode, Node* newNode)
{
    for(unsigned int i = 0; i < m_nodeList.size(); i++) {
        if(m_nodeList[i] == oldNode) m_nodeList[i] = newNode;
    }
}

ControlElement* ControlElement::GetControlElementFromID(std::vector<ControlElement*> elementList, int id)
{
    for(auto it = elementList.begin(), itEnd = elementList.end(); it != itEnd; ++it) {
        ControlElement* element = *it;
        if(element->GetID() == id) return element;
    }
    return NULL;
}

void ControlElement::SaveControlNodes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementNode)
{
    auto nodeList = XMLParser::AppendNode(doc, elementNode, "NodeList");
    int id = 0;
    for(auto it = m_nodeList.begin(), itEnd = m_nodeList.end(); it != itEnd; ++it) {
        Node* node = *it;
        node->SetID(id);
        auto nodeN = XMLParser::AppendNode(doc, nodeList, "Node");
        XMLParser::SetNodeAttribute(doc, nodeN, "ID", id);
        auto nodePosition = XMLParser::AppendNode(doc, nodeN, "Position");
        auto posNodeX = XMLParser::AppendNode(doc, nodePosition, "X");
        XMLParser::SetNodeValue(doc, posNodeX, node->GetPosition().m_x);
        auto posNodeY = XMLParser::AppendNode(doc, nodePosition, "Y");
        XMLParser::SetNodeValue(doc, posNodeY, node->GetPosition().m_y);
        auto angle = XMLParser::AppendNode(doc, nodeN, "Angle");
        XMLParser::SetNodeValue(doc, angle, node->GetAngle());
        auto nodeType = XMLParser::AppendNode(doc, nodeN, "Type");
        XMLParser::SetNodeValue(doc, nodeType, node->GetNodeType());
        id++;
    }
}

bool ControlElement::OpenControlNodes(rapidxml::xml_node<>* elementNode)
{
    // Clear old nodes
    for(auto it = m_nodeList.begin(), itEnd = m_nodeList.end(); it != itEnd; ++it) delete *it;
    m_nodeList.clear();

    auto nodeList = elementNode->first_node("NodeList");
    if(!nodeList) return false;
    auto nodeN = nodeList->first_node("Node");
    while(nodeN) {
        auto nodePosition = nodeN->first_node("Position");
        double nodePosX = XMLParser::GetNodeValueDouble(nodePosition, "X");
        double nodePosY = XMLParser::GetNodeValueDouble(nodePosition, "Y");
        double nodeAngle = XMLParser::GetNodeValueDouble(nodeN, "Angle");
        Node::NodeType nodeType = static_cast<Node::NodeType>(XMLParser::GetNodeValueInt(nodeN, "Type"));
        Node* node = new Node(wxPoint2DDouble(nodePosX, nodePosY), nodeType, 2.0);
        node->SetAngle(nodeAngle);
        m_nodeList.push_back(node);
        nodeN = nodeN->next_sibling("Node");
    }
    return true;
}
