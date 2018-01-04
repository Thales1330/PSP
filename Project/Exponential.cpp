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

#include "Exponential.h"
#include "ExponentialForm.h"

Exponential::Exponential(int id) : ControlElement(id)
{
    m_width = m_height = 36.0;
    Node* nodeIn = new Node(m_position + wxPoint2DDouble(-18, 0), Node::NODE_IN, m_borderSize);
    nodeIn->StartMove(m_position);
    Node* nodeOut = new Node(m_position + wxPoint2DDouble(18, 0), Node::NODE_OUT, m_borderSize);
    nodeOut->SetAngle(180.0);
    nodeOut->StartMove(m_position);
    m_nodeList.push_back(nodeIn);
    m_nodeList.push_back(nodeOut);
}

Exponential::~Exponential() {}
void Exponential::Draw(wxPoint2DDouble translation, double scale) const
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
    std::vector<wxPoint2DDouble> axis;
    axis.push_back(m_position + wxPoint2DDouble(-13, 13));
    axis.push_back(m_position + wxPoint2DDouble(13, 13));
    axis.push_back(m_position + wxPoint2DDouble(-13, -13));
    axis.push_back(m_position + wxPoint2DDouble(-13, 13));
    DrawLine(axis, GL_LINES);

    glLineWidth(2.0);
    std::vector<wxPoint2DDouble> expSymbol;
    expSymbol.push_back(m_position + wxPoint2DDouble(-13, 13));
    expSymbol.push_back(m_position + wxPoint2DDouble(-6, 13));
    expSymbol.push_back(m_position + wxPoint2DDouble(2, 12));
    expSymbol.push_back(m_position + wxPoint2DDouble(4, 11));
    expSymbol.push_back(m_position + wxPoint2DDouble(6, 10));
    expSymbol.push_back(m_position + wxPoint2DDouble(8, 7));
    expSymbol.push_back(m_position + wxPoint2DDouble(11, -1));
    expSymbol.push_back(m_position + wxPoint2DDouble(12, -7));
    expSymbol.push_back(m_position + wxPoint2DDouble(13, -13));
    glColor4d(0.0, 0.3, 1.0, 1.0);
    DrawLine(expSymbol);

    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawNodes();
}

bool Exponential::ShowForm(wxWindow* parent, Element* element)
{
    ExponentialForm* form = new ExponentialForm(parent, this);
    if(form->ShowModal() == wxID_OK) {
        form->Destroy();
        return true;
    }
    form->Destroy();
    return false;
}

void Exponential::Rotate(bool clockwise)
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

void Exponential::UpdatePoints()
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

void Exponential::GetValues(double& aValue, double& bValue)
{
    aValue = m_aValue;
    bValue = m_bValue;
}

void Exponential::SetValues(double aValue, double bValue)
{
    m_aValue = aValue;
    m_bValue = bValue;
}

bool Exponential::Solve(double* input, double timeStep)
{
    if(!input) {
        m_output = 0.0;
        return true;
    }
    m_output = m_aValue * std::exp(m_bValue * input[0]);
    return true;
}

Element* Exponential::GetCopy()
{
    Exponential* copy = new Exponential(m_elementID);
    *copy = *this;
    return copy;
}

void Exponential::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
    auto elementNode = XMLParser::AppendNode(doc, elementListNode, "Exponential");
    XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

    SaveCADProperties(doc, elementNode);
    SaveControlNodes(doc, elementNode);

    auto value = XMLParser::AppendNode(doc, elementNode, "Value");
    auto aValue = XMLParser::AppendNode(doc, value, "A");
    XMLParser::SetNodeValue(doc, aValue, m_aValue);
    auto bValue = XMLParser::AppendNode(doc, value, "B");
    XMLParser::SetNodeValue(doc, bValue, m_bValue);
}

bool Exponential::OpenElement(rapidxml::xml_node<>* elementNode)
{
    if(!OpenCADProperties(elementNode)) return false;
    if(!OpenControlNodes(elementNode)) return false;

    // Element properties
    auto value = elementNode->first_node("Value");
    m_aValue = XMLParser::GetNodeValueDouble(value, "A");
    m_bValue = XMLParser::GetNodeValueDouble(value, "B");

    StartMove(m_position);
    UpdatePoints();
    return true;
}
