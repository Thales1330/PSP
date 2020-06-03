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

#include "Limiter.h"
#include "LimiterForm.h"

Limiter::Limiter(int id) : ControlElement(id)
{
    m_width = m_height = 36.0;
    Node* nodeIn = new Node(m_position + wxPoint2DDouble(-18, 0), Node::NodeType::NODE_IN, m_borderSize);
    nodeIn->StartMove(m_position);
    Node* nodeOut = new Node(m_position + wxPoint2DDouble(18, 0), Node::NodeType::NODE_OUT, m_borderSize);
    nodeOut->SetAngle(180.0);
    nodeOut->StartMove(m_position);
    m_nodeList.push_back(nodeIn);
    m_nodeList.push_back(nodeOut);
}

Limiter::~Limiter() {}
void Limiter::Draw(wxPoint2DDouble translation, double scale) const
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

    // Plot symbol.
    glLineWidth(2.0);
    std::vector<wxPoint2DDouble> limSymbol;
    limSymbol.push_back(m_position + wxPoint2DDouble(10, -10));
    limSymbol.push_back(m_position + wxPoint2DDouble(2, -10));
    limSymbol.push_back(m_position + wxPoint2DDouble(-2, 10));
    limSymbol.push_back(m_position + wxPoint2DDouble(-10, 10));
    glColor4d(0.0, 0.3, 1.0, 1.0);
    DrawLine(limSymbol);

    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawNodes();
}

bool Limiter::ShowForm(wxWindow* parent, Element* element)
{
    LimiterForm* limiter = new LimiterForm(parent, this);
    if(limiter->ShowModal() == wxID_OK) {
        limiter->Destroy();
        return true;
    }
    limiter->Destroy();
    return false;
}

void Limiter::Rotate(bool clockwise)
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

void Limiter::UpdatePoints()
{
    if(m_angle == 0.0) {
        m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(-18, 0));
        m_nodeList[1]->SetPosition(m_position + wxPoint2DDouble(18, 0));
    } else if(m_angle == 90.0) {
        m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(0, -18));
        m_nodeList[1]->SetPosition(m_position + wxPoint2DDouble(0, 18));
    } else if(m_angle == 180.0) {
        m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(18, 0));
        m_nodeList[1]->SetPosition(m_position + wxPoint2DDouble(-18, 0));
    } else if(m_angle == 270.0) {
        m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(0, 18));
        m_nodeList[1]->SetPosition(m_position + wxPoint2DDouble(0, -18));
    }
}

bool Limiter::Solve(double* input, double timeStep)
{
    if(!input) {
        m_output = 0.0;
        return true;
    }
    m_output = input[0];
    if(m_output > m_upLimit)
        m_output = m_upLimit;
    else if(m_output < m_lowLimit)
        m_output = m_lowLimit;

    return true;
}

Element* Limiter::GetCopy()
{
    Limiter* copy = new Limiter(m_elementID);
    *copy = *this;
    return copy;
}

rapidxml::xml_node<>* Limiter::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
    auto elementNode = XMLParser::AppendNode(doc, elementListNode, "Limiter");
    XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

    SaveCADProperties(doc, elementNode);
    SaveControlNodes(doc, elementNode);

    // Element properties
    auto upLimit = XMLParser::AppendNode(doc, elementNode, "UpperLimit");
    XMLParser::SetNodeValue(doc, upLimit, m_upLimit);
    auto lowLimit = XMLParser::AppendNode(doc, elementNode, "LowerLimit");
    XMLParser::SetNodeValue(doc, lowLimit, m_lowLimit);

    return elementNode;
}

bool Limiter::OpenElement(rapidxml::xml_node<>* elementNode)
{
    if(!OpenCADProperties(elementNode)) return false;
    if(!OpenControlNodes(elementNode)) return false;

    // Element properties
    m_upLimit = XMLParser::GetNodeValueDouble(elementNode, "UpperLimit");
    m_lowLimit = XMLParser::GetNodeValueDouble(elementNode, "LowerLimit");

    StartMove(m_position);
    UpdatePoints();
    return true;
}
