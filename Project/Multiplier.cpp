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
#include "Multiplier.h"
#include <wx/pen.h>

Multiplier::Multiplier(int id) : MathOperation(id) {}
Multiplier::~Multiplier() {}
void Multiplier::DrawSymbol() const
{
    // Plot x.
    glLineWidth(2.0);
    std::vector<wxPoint2DDouble> xSymbol;
    xSymbol.push_back(m_position + wxPoint2DDouble(-5, -5));
    xSymbol.push_back(m_position + wxPoint2DDouble(5, 5));
    xSymbol.push_back(m_position + wxPoint2DDouble(-5, 5));
    xSymbol.push_back(m_position + wxPoint2DDouble(5, -5));
    glColor4d(0.0, 0.3, 1.0, 1.0);
    DrawLine(xSymbol, GL_LINES);
}

void Multiplier::DrawDCSymbol(wxGraphicsContext* gc) const
{
    // Plot x.
    gc->SetPen(wxPen(wxColour(0, 77, 255, 255), 2));
    gc->SetBrush(*wxTRANSPARENT_BRUSH);
    wxPoint2DDouble xSymbol[4];
    xSymbol[0] = m_position + wxPoint2DDouble(-5, -5);
    xSymbol[1] = m_position + wxPoint2DDouble(5, 5);
    xSymbol[2] = m_position + wxPoint2DDouble(-5, 5);
    xSymbol[3] = m_position + wxPoint2DDouble(5, -5);
    gc->DrawLines(2, &xSymbol[0]);
    gc->DrawLines(2, &xSymbol[2]);
}

bool Multiplier::Solve(double* input, double timeStep)
{
    std::vector<double> inputVector;
    for(auto itN = m_nodeList.begin(), itNEnd = m_nodeList.end(); itN != itNEnd; ++itN) {
        Node* node = *itN;
        if(node->GetNodeType() != Node::NodeType::NODE_OUT) {
            if(!node->IsConnected()) {
                inputVector.push_back(1.0);
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

    m_output = 1.0;
    for(unsigned int i = 0; i < inputVector.size(); ++i) { m_output *= inputVector[i]; }

    return true;
}

Element* Multiplier::GetCopy()
{
    Multiplier* copy = new Multiplier(m_elementID);
    *copy = *this;
    return copy;
}

rapidxml::xml_node<>* Multiplier::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
    auto elementNode = XMLParser::AppendNode(doc, elementListNode, "Multiplier");
    XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

    SaveCADProperties(doc, elementNode);
    SaveControlNodes(doc, elementNode);
    
    return elementNode;
}

bool Multiplier::OpenElement(rapidxml::xml_node<>* elementNode)
{
    if(!OpenCADProperties(elementNode)) return false;
    if(!OpenControlNodes(elementNode)) return false;

    StartMove(m_position);
    UpdatePoints();
    return true;
}
