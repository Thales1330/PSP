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

#include "Gain.h"
#include "GainForm.h"

Gain::Gain(int id) : ControlElement(id)
{
    m_triPts.resize(3);
    SetValue(m_value);
    Node* nodeIn = new Node(m_position + wxPoint2DDouble(-m_width / 2, 0), Node::NodeType::NODE_IN, m_borderSize);
    nodeIn->StartMove(m_position);
    Node* nodeOut = new Node(m_position + wxPoint2DDouble(m_width / 2, 0), Node::NodeType::NODE_OUT, m_borderSize);
    nodeOut->SetAngle(180.0);
    nodeOut->StartMove(m_position);
    m_nodeList.push_back(nodeIn);
    m_nodeList.push_back(nodeOut);
}

Gain::~Gain()
{
    if(m_glText) delete m_glText;
}
void Gain::Draw(wxPoint2DDouble translation, double scale) const
{
    if(m_selected) {
        glColor4dv(m_selectionColour.GetRGBA());
        double borderSize = (m_borderSize * 2.0 + 1.0) / scale;
        std::vector<wxPoint2DDouble> m_triSelectedPts;
        if(m_angle == 0.0) {
            m_triSelectedPts.push_back(m_triPts[0] - wxPoint2DDouble(borderSize / 2, borderSize / 1.5));
            m_triSelectedPts.push_back(m_triPts[1] - wxPoint2DDouble(borderSize / 2, -borderSize / 1.5));
            m_triSelectedPts.push_back(m_triPts[2] - wxPoint2DDouble(-borderSize, 0));
        } else if(m_angle == 90.0) {
            m_triSelectedPts.push_back(m_triPts[0] - wxPoint2DDouble(borderSize / 1.5, borderSize / 2));
            m_triSelectedPts.push_back(m_triPts[1] - wxPoint2DDouble(-borderSize / 1.5, borderSize / 2));
            m_triSelectedPts.push_back(m_triPts[2] - wxPoint2DDouble(0, -borderSize));
        } else if(m_angle == 180.0) {
            m_triSelectedPts.push_back(m_triPts[0] - wxPoint2DDouble(borderSize, 0));
            m_triSelectedPts.push_back(m_triPts[1] - wxPoint2DDouble(-borderSize / 2, borderSize / 1.5));
            m_triSelectedPts.push_back(m_triPts[2] - wxPoint2DDouble(-borderSize / 2, -borderSize / 1.5));
        } else if(m_angle == 270.0) {
            m_triSelectedPts.push_back(m_triPts[0] - wxPoint2DDouble(0, borderSize));
            m_triSelectedPts.push_back(m_triPts[1] - wxPoint2DDouble(-borderSize / 1.5, -borderSize / 2));
            m_triSelectedPts.push_back(m_triPts[2] - wxPoint2DDouble(borderSize / 1.5, -borderSize / 2));
        }
        DrawTriangle(m_triSelectedPts);
    }
    glLineWidth(1.0);
    glColor4d(1.0, 1.0, 1.0, 1.0);
    DrawTriangle(m_triPts);
    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawTriangle(m_triPts, GL_LINE_LOOP);

    // Plot number.
    glColor4d(0.0, 0.0, 0.0, 1.0);
    if(m_angle == 0.0)
        m_glText->Draw(m_position + wxPoint2DDouble(-m_width / 2 + m_glText->GetWidth() / 2 + 2 + m_borderSize, 0.0));
    else if(m_angle == 90.0)
        m_glText->Draw(m_position + wxPoint2DDouble(0.0, -m_height / 2 + m_glText->GetHeight() / 2 + 2 + m_borderSize));
    else if(m_angle == 180.0)
        m_glText->Draw(m_position + wxPoint2DDouble(m_width / 2 - m_glText->GetWidth() / 2 - 2 - m_borderSize, 0.0));
    else if(m_angle == 270.0)
        m_glText->Draw(m_position + wxPoint2DDouble(0.0, m_height / 2 - m_glText->GetHeight() / 2 - 2 - m_borderSize));

    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawNodes();
}

bool Gain::ShowForm(wxWindow* parent, Element* element)
{
    GainForm* form = new GainForm(parent, this);
    if(form->ShowModal() == wxID_OK) {
        form->Destroy();
        return true;
    }
    form->Destroy();
    return false;
}

void Gain::Rotate(bool clockwise)
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

void Gain::SetValue(double value)
{
    m_value = value;
    wxString text = "";
    if(std::abs(m_value) > 1e3 || std::abs(m_value) < 1e-3)
        text = wxString::Format("%g", m_value);
    else
        text = StringFromDouble(m_value);

    if(m_glText)
        m_glText->SetText(text);
    else
        m_glText = new OpenGLText(text);

    m_width = m_glText->GetWidth() + 18 + 2 * m_borderSize;
    m_height = m_glText->GetHeight() + 18 + 2 * m_borderSize;

    if(m_width > m_height)
        m_height = m_width;
    else
        m_width = m_height;

    SetPosition(m_position);  // Update rectangle.

    UpdatePoints();
}

void Gain::UpdatePoints()
{
    if(m_nodeList.size() != 0) {
        if(m_angle == 0.0) {
            m_triPts[0] = m_rect.GetLeftTop() + wxPoint2DDouble(m_borderSize, m_borderSize);
            m_triPts[1] = m_rect.GetLeftBottom() + wxPoint2DDouble(m_borderSize, -m_borderSize);
            m_triPts[2] = m_position + wxPoint2DDouble(m_width / 2 - m_borderSize, 0);
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(-m_width / 2, 0));
            m_nodeList[1]->SetPosition(m_position + wxPoint2DDouble(m_width / 2 - 2, 0));
        } else if(m_angle == 90.0) {
            m_triPts[0] = m_rect.GetLeftTop() + wxPoint2DDouble(m_borderSize, m_borderSize);
            m_triPts[1] = m_rect.GetRightTop() + wxPoint2DDouble(-m_borderSize, m_borderSize);
            m_triPts[2] = m_position + wxPoint2DDouble(0, m_height / 2 - m_borderSize);
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(0, -m_height / 2));
            m_nodeList[1]->SetPosition(m_position + wxPoint2DDouble(0, m_height / 2 - 2));
        } else if(m_angle == 180.0) {
            m_triPts[0] = m_position + wxPoint2DDouble(-m_width / 2 + m_borderSize, 0);
            m_triPts[1] = m_rect.GetRightTop() + wxPoint2DDouble(-m_borderSize, m_borderSize);
            m_triPts[2] = m_rect.GetRightBottom() + wxPoint2DDouble(-m_borderSize, -m_borderSize);
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(m_width / 2, 0));
            m_nodeList[1]->SetPosition(m_position + wxPoint2DDouble(-m_width / 2 + 2, 0));
        } else if(m_angle == 270.0) {
            m_triPts[0] = m_position + wxPoint2DDouble(0, -m_height / 2 + m_borderSize);
            m_triPts[1] = m_rect.GetRightBottom() + wxPoint2DDouble(-m_borderSize, -m_borderSize);
            m_triPts[2] = m_rect.GetLeftBottom() + wxPoint2DDouble(m_borderSize, -m_borderSize);
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(0, m_height / 2));
            m_nodeList[1]->SetPosition(m_position + wxPoint2DDouble(0, -m_height / 2 + 2));
        }
    }
}

void Gain::Move(wxPoint2DDouble position)
{
    SetPosition(m_movePos + position - m_moveStartPt);
    UpdatePoints();
}

bool Gain::Solve(double* input, double timeStep)
{
    if(!input) {
        m_output = 0.0;
        return true;
    }
    m_output = input[0] * m_value;
    return true;
}

Element* Gain::GetCopy()
{
    Gain* copy = new Gain(m_elementID);
    *copy = *this;
    copy->m_glText = m_glText->GetCopy();
    return copy;
}

bool Gain::UpdateText()
{
    SetValue(m_value);
    if(!m_glText->IsTextureOK()) return false;
    return true;
}

rapidxml::xml_node<>* Gain::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
    auto elementNode = XMLParser::AppendNode(doc, elementListNode, "Gain");
    XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

    SaveCADProperties(doc, elementNode);
    SaveControlNodes(doc, elementNode);

    // Element properties
    auto value = XMLParser::AppendNode(doc, elementNode, "Value");
    XMLParser::SetNodeValue(doc, value, m_value);

    return elementNode;
}

bool Gain::OpenElement(rapidxml::xml_node<>* elementNode)
{
    if(!OpenCADProperties(elementNode)) return false;
    if(!OpenControlNodes(elementNode)) return false;

    // Element properties
    double value = XMLParser::GetNodeValueDouble(elementNode, "Value");
    SetValue(value);
    return true;
}