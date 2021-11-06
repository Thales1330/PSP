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
#include "Sum.h"
#include "SumForm.h"
#include <wx/pen.h>
#include <wx/brush.h>

Sum::Sum(int id) : ControlElement(id)
{
    m_width = m_height = 36.0;
    Node* nodeIn1 = new Node(m_position + wxPoint2DDouble(-m_width / 2, 9 - m_height / 2), Node::NodeType::NODE_IN, m_borderSize);
    nodeIn1->StartMove(m_position);
    Node* nodeIn2 =
        new Node(m_position + wxPoint2DDouble(-m_width / 2, 27 - m_height / 2), Node::NodeType::NODE_IN, m_borderSize);
    nodeIn2->StartMove(m_position);
    Node* nodeOut = new Node(m_position + wxPoint2DDouble(m_width / 2, 0), Node::NodeType::NODE_OUT, m_borderSize);
    nodeOut->SetAngle(180.0);
    nodeOut->StartMove(m_position);
    m_nodeList.push_back(nodeIn1);
    m_nodeList.push_back(nodeIn2);
    m_nodeList.push_back(nodeOut);
    m_signalList.push_back(SIGNAL_POSITIVE);
    m_signalList.push_back(SIGNAL_NEGATIVE);

    UpdatePoints();
}

Sum::~Sum() {}
void Sum::Draw(wxPoint2DDouble translation, double scale) const
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

    // Plot signals.
    glLineWidth(2.0);
    wxPoint2DDouble signalOffset[4];
    wxPoint2DDouble sigmaOffset;
    if(m_angle == 0.0) {
        signalOffset[0] = wxPoint2DDouble(6, 0);
        signalOffset[1] = wxPoint2DDouble(12, 0);
        signalOffset[2] = wxPoint2DDouble(9, -3);
        signalOffset[3] = wxPoint2DDouble(9, 3);
        sigmaOffset = wxPoint2DDouble(6, 0);
    } else if(m_angle == 90.0) {
        signalOffset[0] = wxPoint2DDouble(-3, 9);
        signalOffset[1] = wxPoint2DDouble(3, 9);
        signalOffset[2] = wxPoint2DDouble(0, 6);
        signalOffset[3] = wxPoint2DDouble(0, 12);
        sigmaOffset = wxPoint2DDouble(0, 6);
    } else if(m_angle == 180.0) {
        signalOffset[0] = wxPoint2DDouble(-6, 0);
        signalOffset[1] = wxPoint2DDouble(-12, 0);
        signalOffset[2] = wxPoint2DDouble(-9, -3);
        signalOffset[3] = wxPoint2DDouble(-9, 3);
        sigmaOffset = wxPoint2DDouble(-6, 0);
    } else if(m_angle == 270.0) {
        signalOffset[0] = wxPoint2DDouble(-3, -9);
        signalOffset[1] = wxPoint2DDouble(3, -9);
        signalOffset[2] = wxPoint2DDouble(0, -6);
        signalOffset[3] = wxPoint2DDouble(0, -12);
        sigmaOffset = wxPoint2DDouble(0, -6);
    }
    for(int i = 0; i < (int)m_nodeList.size() - 1; ++i) {
        std::vector<wxPoint2DDouble> hLine;
        hLine.push_back(m_nodeList[i]->GetPosition() + signalOffset[0]);
        hLine.push_back(m_nodeList[i]->GetPosition() + signalOffset[1]);
        DrawLine(hLine);
        if(m_signalList[i] == SIGNAL_POSITIVE) {
            std::vector<wxPoint2DDouble> vLine;
            vLine.push_back(m_nodeList[i]->GetPosition() + signalOffset[2]);
            vLine.push_back(m_nodeList[i]->GetPosition() + signalOffset[3]);
            DrawLine(vLine);
        }
    }

    // Plot sigma.
    std::vector<wxPoint2DDouble> sigma;
    sigma.push_back(m_position + wxPoint2DDouble(4, 9) + sigmaOffset);
    sigma.push_back(m_position + wxPoint2DDouble(-6, 9) + sigmaOffset);
    sigma.push_back(m_position + wxPoint2DDouble(0, 0) + sigmaOffset);
    sigma.push_back(m_position + wxPoint2DDouble(-6, -9) + sigmaOffset);
    sigma.push_back(m_position + wxPoint2DDouble(4, -9) + sigmaOffset);
    glColor4d(0.0, 0.3, 1.0, 1.0);
    DrawLine(sigma);

    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawNodes();
}

void Sum::DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const
{
    if (m_selected) {
        gc->SetPen(*wxTRANSPARENT_PEN);
        gc->SetBrush(wxBrush(m_selectionColour.GetDcRGBA()));
        double borderSize = (m_borderSize * 2.0 + 1.0) / scale;
        gc->DrawRectangle(m_position.m_x - m_width / 2 - borderSize / 2, m_position.m_y - m_height / 2 - borderSize / 2, m_width + borderSize, m_height + borderSize);
    }
    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);
    gc->DrawRectangle(m_position.m_x - m_width / 2, m_position.m_y - m_height / 2, m_width, m_height);

    // Plot signals.
    gc->SetPen(wxPen(wxColour(0, 0, 0, 255), 2));
    gc->SetBrush(*wxTRANSPARENT_BRUSH);
    wxPoint2DDouble signalOffset[4];
    wxPoint2DDouble sigmaOffset;
    if (m_angle == 0.0) {
        signalOffset[0] = wxPoint2DDouble(6, 0);
        signalOffset[1] = wxPoint2DDouble(12, 0);
        signalOffset[2] = wxPoint2DDouble(9, -3);
        signalOffset[3] = wxPoint2DDouble(9, 3);
        sigmaOffset = wxPoint2DDouble(6, 0);
    }
    else if (m_angle == 90.0) {
        signalOffset[0] = wxPoint2DDouble(-3, 9);
        signalOffset[1] = wxPoint2DDouble(3, 9);
        signalOffset[2] = wxPoint2DDouble(0, 6);
        signalOffset[3] = wxPoint2DDouble(0, 12);
        sigmaOffset = wxPoint2DDouble(0, 6);
    }
    else if (m_angle == 180.0) {
        signalOffset[0] = wxPoint2DDouble(-6, 0);
        signalOffset[1] = wxPoint2DDouble(-12, 0);
        signalOffset[2] = wxPoint2DDouble(-9, -3);
        signalOffset[3] = wxPoint2DDouble(-9, 3);
        sigmaOffset = wxPoint2DDouble(-6, 0);
    }
    else if (m_angle == 270.0) {
        signalOffset[0] = wxPoint2DDouble(-3, -9);
        signalOffset[1] = wxPoint2DDouble(3, -9);
        signalOffset[2] = wxPoint2DDouble(0, -6);
        signalOffset[3] = wxPoint2DDouble(0, -12);
        sigmaOffset = wxPoint2DDouble(0, -6);
    }
    for (unsigned int i = 0; i < m_nodeList.size() - 1; ++i) {
        wxPoint2DDouble hLine[2];
        hLine[0] = m_nodeList[i]->GetPosition() + signalOffset[0];
        hLine[1] = m_nodeList[i]->GetPosition() + signalOffset[1];
        gc->DrawLines(2, hLine);
        if (m_signalList[i] == SIGNAL_POSITIVE) {
            wxPoint2DDouble vLine[2];
            vLine[0] = m_nodeList[i]->GetPosition() + signalOffset[2];
            vLine[1] = m_nodeList[i]->GetPosition() + signalOffset[3];
            gc->DrawLines(2, vLine);
        }
    }

    // Plot sigma.
    gc->SetPen(wxPen(wxColour(0, 77, 255, 255), 2));
    wxPoint2DDouble sigma[5];
    sigma[0] = m_position + wxPoint2DDouble(4, 9) + sigmaOffset;
    sigma[1] = m_position + wxPoint2DDouble(-6, 9) + sigmaOffset;
    sigma[2] = m_position + wxPoint2DDouble(0, 0) + sigmaOffset;
    sigma[3] = m_position + wxPoint2DDouble(-6, -9) + sigmaOffset;
    sigma[4] = m_position + wxPoint2DDouble(4, -9) + sigmaOffset;
    gc->DrawLines(5, sigma);

    gc->SetPen(*wxTRANSPARENT_PEN);
    gc->SetBrush(*wxBLACK_BRUSH);
    DrawDCNodes(gc);
}

bool Sum::ShowForm(wxWindow* parent, Element* element)
{
    SumForm* sumForm = new SumForm(parent, this);
    if(sumForm->ShowModal() == wxID_OK) {
        sumForm->Destroy();
        return true;
    }
    sumForm->Destroy();
    return false;
}

void Sum::UpdatePoints()
{
    if(m_angle == 0.0 || m_angle == 180.0) {
        m_height = 18.0 * (m_nodeList.size() - 1);
        m_width = 36.0;
    } else {
        m_width = 18.0 * (m_nodeList.size() - 1);
        m_height = 42.0;
    }

    for(int i = 0; i < (int)m_nodeList.size() - 1; ++i) {
        if(m_angle == 0.0)
            m_nodeList[i]->SetPosition(m_position + wxPoint2DDouble(-m_width / 2, 9 + 18 * i - m_height / 2));
        else if(m_angle == 90.0)
            m_nodeList[i]->SetPosition(m_position + wxPoint2DDouble(m_width / 2 - 9 - 18 * i, -m_height / 2));
        else if(m_angle == 180.0)
            m_nodeList[i]->SetPosition(m_position + wxPoint2DDouble(m_width / 2, m_height / 2 - 9 - 18 * i));
        else if(m_angle == 270.0)
            m_nodeList[i]->SetPosition(m_position + wxPoint2DDouble(9 + 18 * i - m_width / 2, m_height / 2));
    }
    if(m_angle == 0.0)
        m_nodeList[m_nodeList.size() - 1]->SetPosition(m_position + wxPoint2DDouble(m_width / 2, 0));
    else if(m_angle == 90.0)
        m_nodeList[m_nodeList.size() - 1]->SetPosition(m_position + wxPoint2DDouble(0, m_height / 2));
    else if(m_angle == 180.0)
        m_nodeList[m_nodeList.size() - 1]->SetPosition(m_position + wxPoint2DDouble(-m_width / 2, 0));
    else if(m_angle == 270.0)
        m_nodeList[m_nodeList.size() - 1]->SetPosition(m_position + wxPoint2DDouble(0, -m_height / 2));

    SetPosition(m_position);  // Update rect.
}

void Sum::AddInNode()
{
    Node* newNode = new Node(wxPoint2DDouble(0, 0), Node::NodeType::NODE_IN, m_borderSize);
    newNode->SetAngle(m_angle);
    m_nodeList.insert(m_nodeList.end() - 1, newNode);
}

void Sum::RemoveInNode()
{
    Node* nodeToRemove = *(m_nodeList.end() - 2);
    bool foundChild = false;
    for(auto it = m_childList.begin(), itEnd = m_childList.end(); it != itEnd; ++it) {
        ControlElement* child = static_cast<ControlElement*>(*it);
        auto childNodeList = child->GetNodeList();
        for(auto itN = childNodeList.begin(), itEndN = childNodeList.end(); itN != itEndN; ++itN) {
            Node* node = *itN;
            if(node == nodeToRemove) {
                child->RemoveParent(this);
                RemoveChild(child);
                foundChild = true;
                break;
            }
        }
        if(foundChild) break;
    }
    m_nodeList.erase(m_nodeList.end() - 2);
}

void Sum::Rotate(bool clockwise)
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

bool Sum::Solve(double* input, double timeStep)
{
    if(!input) {
        m_output = 0.0;
        return true;
    }
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

    if(m_signalList.size() != inputVector.size()) return false;

    m_output = 0.0;
    for(unsigned int i = 0; i < m_signalList.size(); ++i) {
        if(m_signalList[i] == SIGNAL_POSITIVE)
            m_output += inputVector[i];
        else if(m_signalList[i] == SIGNAL_NEGATIVE)
            m_output -= inputVector[i];
    }
    return true;
}

Element* Sum::GetCopy()
{
    Sum* copy = new Sum(m_elementID);
    *copy = *this;
    return copy;
}

rapidxml::xml_node<>* Sum::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
    auto elementNode = XMLParser::AppendNode(doc, elementListNode, "Sum");
    XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

    SaveCADProperties(doc, elementNode);
    SaveControlNodes(doc, elementNode);

    // Element properties
    auto signsNode = XMLParser::AppendNode(doc, elementNode, "Signs");
    for(unsigned int i = 0; i < m_signalList.size(); ++i) {
        auto value = XMLParser::AppendNode(doc, signsNode, "Value");
        XMLParser::SetNodeValue(doc, value, static_cast<int>(m_signalList[i]));
    }

    return elementNode;
}

bool Sum::OpenElement(rapidxml::xml_node<>* elementNode)
{
    if(!OpenCADProperties(elementNode)) return false;
    if(!OpenControlNodes(elementNode)) return false;

    m_signalList.clear();
    auto signsNode = elementNode->first_node("Signs");
    auto sign = signsNode->first_node("Value");
    while(sign) {
        long value;
        wxString(sign->value()).ToCLong(&value);
        m_signalList.push_back(static_cast<Sum::Signal>(value));
        sign = sign->next_sibling("Value");
    }

    StartMove(m_position);
    UpdatePoints();

    return true;
}
