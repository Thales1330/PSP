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
#include "../../forms/ExponentialForm.h"
#include <wx/pen.h>
#include <wx/brush.h>

Exponential::Exponential(int id) : ControlElement(id)
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

Exponential::~Exponential()
{
    for (auto& node : m_nodeList) if (node) delete node;
    m_nodeList.clear();
}
//void Exponential::Draw(wxPoint2DDouble translation, double scale) const
//{
//    glLineWidth(1.0);
//    if(m_selected) {
//        glColor4dv(m_selectionColour.GetRGBA());
//        double borderSize = (m_borderSize * 2.0 + 1.0) / scale;
//        DrawRectangle(m_position, m_width + borderSize, m_height + borderSize);
//    }
//    glColor4d(1.0, 1.0, 1.0, 1.0);
//    DrawRectangle(m_position, m_width, m_height);
//    glColor4d(0.0, 0.0, 0.0, 1.0);
//    DrawRectangle(m_position, m_width, m_height, GL_LINE_LOOP);
//
//    // Plot symbol.
//    std::vector<wxPoint2DDouble> axis;
//    axis.push_back(m_position + wxPoint2DDouble(-13, 13));
//    axis.push_back(m_position + wxPoint2DDouble(13, 13));
//    axis.push_back(m_position + wxPoint2DDouble(-13, -13));
//    axis.push_back(m_position + wxPoint2DDouble(-13, 13));
//    DrawLine(axis, GL_LINES);
//
//    glLineWidth(2.0);
//    std::vector<wxPoint2DDouble> expSymbol;
//    expSymbol.push_back(m_position + wxPoint2DDouble(-13, 13));
//    expSymbol.push_back(m_position + wxPoint2DDouble(-6, 13));
//    expSymbol.push_back(m_position + wxPoint2DDouble(2, 12));
//    expSymbol.push_back(m_position + wxPoint2DDouble(4, 11));
//    expSymbol.push_back(m_position + wxPoint2DDouble(6, 10));
//    expSymbol.push_back(m_position + wxPoint2DDouble(8, 7));
//    expSymbol.push_back(m_position + wxPoint2DDouble(11, -1));
//    expSymbol.push_back(m_position + wxPoint2DDouble(12, -7));
//    expSymbol.push_back(m_position + wxPoint2DDouble(13, -13));
//    glColor4d(0.0, 0.3, 1.0, 1.0);
//    DrawLine(expSymbol);
//
//    glColor4d(0.0, 0.0, 0.0, 1.0);
//    DrawNodes();
//}

void Exponential::DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const
{
    if (m_selected) {
        gc->SetPen(*wxTRANSPARENT_PEN);
        gc->SetBrush(wxBrush(m_selectionColour));
        double borderSize = (m_borderSize * 2.0 + 1.0) / scale;
        gc->DrawRectangle(m_position.m_x - m_width / 2 - borderSize / 2, m_position.m_y - m_height / 2 - borderSize / 2, m_width + borderSize, m_height + borderSize);
    }
    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);
    //DrawRectangle(m_position, m_width, m_height);
    gc->DrawRectangle(m_position.m_x - m_width / 2, m_position.m_y - m_height / 2, m_width, m_height);

    // Plot symbol.
    wxPoint2DDouble axis[4];
    axis[0] = m_position + wxPoint2DDouble(-13, 13);
    axis[1] = m_position + wxPoint2DDouble(13, 13);
    axis[2] = m_position + wxPoint2DDouble(-13, -13);
    axis[3] = m_position + wxPoint2DDouble(-13, 13);
    gc->StrokeLines(2, &axis[0]);
    gc->StrokeLines(2, &axis[2]);

    gc->SetPen(wxPen(wxColour(0, 77, 255, 255), 2));
    gc->SetBrush(*wxTRANSPARENT_BRUSH);
    wxPoint2DDouble expSymbol[9];
    expSymbol[0] = m_position + wxPoint2DDouble(-13, 13);
    expSymbol[1] = m_position + wxPoint2DDouble(-6, 13);
    expSymbol[2] = m_position + wxPoint2DDouble(2, 12);
    expSymbol[3] = m_position + wxPoint2DDouble(4, 11);
    expSymbol[4] = m_position + wxPoint2DDouble(6, 10);
    expSymbol[5] = m_position + wxPoint2DDouble(8, 7);
    expSymbol[6] = m_position + wxPoint2DDouble(11, -1);
    expSymbol[7] = m_position + wxPoint2DDouble(12, -7);
    expSymbol[8] = m_position + wxPoint2DDouble(13, -13);
    gc->StrokeLines(9, expSymbol);

    gc->SetPen(*wxTRANSPARENT_PEN);
    gc->SetBrush(wxBrush(wxColour(0, 0, 0, 255)));
    DrawDCNodes(gc);
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

rapidxml::xml_node<>* Exponential::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
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
    
    return elementNode;
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
