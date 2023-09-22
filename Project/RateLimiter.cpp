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

#include "RateLimiter.h"
#include "RateLimiterForm.h"
#include <wx/pen.h>
#include <wx/brush.h>

RateLimiter::RateLimiter(int id) : ControlElement(id)
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

RateLimiter::~RateLimiter() {}
void RateLimiter::Draw(wxPoint2DDouble translation, double scale) const
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
    axis.push_back(m_position + wxPoint2DDouble(-13, 0));
    axis.push_back(m_position + wxPoint2DDouble(13, 0));
    axis.push_back(m_position + wxPoint2DDouble(0, -13));
    axis.push_back(m_position + wxPoint2DDouble(0, 13));
    DrawLine(axis, GL_LINES);

    glLineWidth(2.0);
    std::vector<wxPoint2DDouble> limSymbol;
    limSymbol.push_back(m_position + wxPoint2DDouble(10, -10));
    limSymbol.push_back(m_position + wxPoint2DDouble(-10, 10));
    glColor4d(0.0, 0.3, 1.0, 1.0);
    DrawLine(limSymbol);

    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawNodes();
}

void RateLimiter::DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const
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

    // Plot symbol.
    gc->SetBrush(*wxTRANSPARENT_BRUSH);
    wxPoint2DDouble axis[4];
    axis[0] = m_position + wxPoint2DDouble(-13, 0);
    axis[1] = m_position + wxPoint2DDouble(13, 0);
    axis[2] = m_position + wxPoint2DDouble(0, -13);
    axis[3] = m_position + wxPoint2DDouble(0, 13);
    gc->DrawLines(2, &axis[0]);
    gc->DrawLines(2, &axis[2]);

    gc->SetPen(wxPen(wxColour(0, 77, 255, 255), 2));
    wxPoint2DDouble limSymbol[2];
    limSymbol[0] = m_position + wxPoint2DDouble(10, -10);
    limSymbol[1] = m_position + wxPoint2DDouble(-10, 10);
    gc->DrawLines(2, limSymbol);

    gc->SetPen(*wxTRANSPARENT_PEN);
    gc->SetBrush(*wxBLACK_BRUSH);
    DrawDCNodes(gc);
}

bool RateLimiter::ShowForm(wxWindow* parent, Element* element)
{
    RateLimiterForm* form = new RateLimiterForm(parent, this);
    if(form->ShowModal() == wxID_OK) {
        form->Destroy();
        return true;
    }
    form->Destroy();
    return false;
}

void RateLimiter::Rotate(bool clockwise)
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

void RateLimiter::UpdatePoints()
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

bool RateLimiter::Solve(double* input, double timeStep)
{
    if(!input) {
        m_output = 0.0;
        return true;
    }
    double rate = (input[0] - m_output) / timeStep;

    bool reachLimit = false;
    if(rate > m_upLimit) {
        rate = m_upLimit;
        reachLimit = true;
    } else if(rate < m_lowLimit) {
        rate = m_lowLimit;
        reachLimit = true;
    }

    if(reachLimit)
        m_output += rate * timeStep;
    else
        m_output = input[0];
    return true;
}

Element* RateLimiter::GetCopy()
{
    RateLimiter* copy = new RateLimiter(m_elementID);
    *copy = *this;
    return copy;
}

rapidxml::xml_node<>* RateLimiter::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
    auto elementNode = XMLParser::AppendNode(doc, elementListNode, "RateLimiter");
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

bool RateLimiter::OpenElement(rapidxml::xml_node<>* elementNode)
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
