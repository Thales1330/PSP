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
#include "Divider.h"
#include <wx/pen.h>
#include <wx/brush.h>

Divider::Divider(int id) : MathOperation(id) {}
Divider::~Divider()
{
    for (auto& node : m_nodeList) if (node) delete node;
    m_nodeList.clear();
}

//void Divider::DrawSymbol() const
//{
//    // Plot divider.
//    glLineWidth(2.0);
//    std::vector<wxPoint2DDouble> mSymbol;
//    mSymbol.push_back(m_position + wxPoint2DDouble(-5, 0));
//    mSymbol.push_back(m_position + wxPoint2DDouble(5, 0));
//    glColor4d(0.0, 0.3, 1.0, 1.0);
//    DrawLine(mSymbol, GL_LINES);
//    DrawCircle(m_position + wxPoint2DDouble(0, -3), 2, 10, GL_POLYGON);
//    DrawCircle(m_position + wxPoint2DDouble(0, 3), 2, 10, GL_POLYGON);
//}

void Divider::DrawDCSymbol(wxGraphicsContext* gc) const
{
    // Plot divider.
    gc->SetPen(wxPen(wxColour(0, 77, 255, 255), 2));
    gc->SetBrush(*wxTRANSPARENT_BRUSH);
    wxPoint2DDouble mSymbol[2];
    mSymbol[0] = m_position + wxPoint2DDouble(-5, 0);
    mSymbol[1] = m_position + wxPoint2DDouble(5, 0);
    gc->StrokeLines(2, mSymbol);

    gc->SetPen(*wxTRANSPARENT_PEN);
    gc->SetBrush(wxBrush(wxColour(0, 77, 255, 255)));
    DrawDCCircle(m_position + wxPoint2DDouble(0, -3), 2, 10, gc);
    DrawDCCircle(m_position + wxPoint2DDouble(0, 3), 2, 10, gc);
}

bool Divider::Solve(double* input, double timeStep)
{
    // Get the input vector from connection lines (can't use default (one) input argument)
    std::vector<double> inputVector;
    for(auto itN = m_nodeList.begin(), itNEnd = m_nodeList.end(); itN != itNEnd; ++itN) {
        Node* node = *itN;
        if(node->GetNodeType() != Node::NodeType::NODE_OUT) {
            if(!node->IsConnected()) {
                inputVector.push_back(0.0);
            } else {
                for(auto itC = m_childList.begin(), itCEnd = m_childList.end(); itC != itCEnd; ++itC) {
                    ConnectionLine* cLine = static_cast<ConnectionLine*>(*itC);
                    auto nodeList = cLine->GetNodeList();
                    for(auto itCN = nodeList.begin(), itCNEnd = nodeList.end(); itCN != itCNEnd; ++itCN) {
                        Node* childNode = *itCN;
                        if(childNode == node) {
                            inputVector.push_back(cLine->GetValue());
                            break;
                        }
                    }
                }
            }
        }
    }
    if(inputVector.size() != 2) return false;

    // If the denominator is zero, set the output a big number (1e15).
    if(std::abs(inputVector[1]) < 1e-15) {
        m_output = 1e15;
    } else {
        m_output = inputVector[0] / inputVector[1];
    }

    return true;
}

Element* Divider::GetCopy()
{
    Divider* copy = new Divider(m_elementID);
    *copy = *this;
    return copy;
}

rapidxml::xml_node<>* Divider::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
    auto elementNode = XMLParser::AppendNode(doc, elementListNode, "Divider");
    XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

    SaveCADProperties(doc, elementNode);
    SaveControlNodes(doc, elementNode);

    return elementNode;
}

bool Divider::OpenElement(rapidxml::xml_node<>* elementNode)
{
    if(!OpenCADProperties(elementNode)) return false;
    if(!OpenControlNodes(elementNode)) return false;

    StartMove(m_position);
    UpdatePoints();

    return true;
}
