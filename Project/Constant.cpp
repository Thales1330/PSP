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

#include "Constant.h"
#include "ConstantForm.h"

Constant::Constant(int id) : ControlElement(id)
{
    SetValue(m_value);
    m_angle = 180.0;
    Node* nodeOut = new Node(m_position + wxPoint2DDouble(m_width / 2, 0), Node::NodeType::NODE_OUT, m_borderSize);
    nodeOut->SetAngle(180.0);
    nodeOut->StartMove(m_position);
    m_nodeList.push_back(nodeOut);
}

Constant::~Constant()
{
    if(m_glText) delete m_glText;
}

void Constant::Draw(wxPoint2DDouble translation, double scale) const
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

    // Plot number.
    m_glText->Draw(m_position);

    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawNodes();
}

void Constant::DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const
{
    if (m_selected) {
        gc->SetPen(wxPen(wxColour(m_selectionColour.GetDcRGBA())));
        gc->SetBrush(*wxTRANSPARENT_BRUSH);
        double borderSize = (m_borderSize * 2.0 + 1.0) / scale;
        gc->DrawRectangle(m_position.m_x - m_width / 2 - borderSize / 2, m_position.m_y - m_height / 2 - borderSize / 2, m_width + borderSize, m_height + borderSize);
    }
    gc->SetPen(wxPen(wxColour(0, 0, 0, 255), 1));
    gc->SetBrush(wxBrush(wxColour(255, 255, 255, 255)));
    DrawRectangle(m_position, m_width, m_height);
    gc->DrawRectangle(m_position.m_x - m_width / 2, m_position.m_y - m_height / 2, m_width, m_height);

    // Plot number.
    m_glText->DrawDC(m_position + wxPoint2DDouble(-m_glText->GetWidth() / 2, - m_glText->GetHeight() / 2), gc);

    gc->SetPen(*wxTRANSPARENT_PEN);
    gc->SetBrush(wxBrush(wxColour(0, 0, 0, 255)));
    DrawDCNodes(gc);
}

bool Constant::ShowForm(wxWindow* parent, Element* element)
{
    ConstantForm* form = new ConstantForm(parent, this);
    if(form->ShowModal() == wxID_OK) {
        form->Destroy();
        return true;
    }
    form->Destroy();
    return false;
}

void Constant::Rotate(bool clockwise)
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

void Constant::UpdatePoints()
{
    if(m_nodeList.size() != 0) {
        if(m_angle == 0.0) {
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(-m_width / 2, 0));
        } else if(m_angle == 90.0) {
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(0, -m_height / 2));
        } else if(m_angle == 180.0) {
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(m_width / 2, 0));
        } else if(m_angle == 270.0) {
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(0, m_height / 2));
        }
    }
}

void Constant::SetValue(double value)
{
    m_value = value;
    wxString text = StringFromDouble(m_value);

    if(m_glText)
        m_glText->SetText(text);
    else
        m_glText = new OpenGLText(text);

    m_width = m_glText->GetWidth() + 6 + 2 * m_borderSize;
    m_height = m_glText->GetHeight() + 6 + 2 * m_borderSize;

    UpdatePoints();
}

Element* Constant::GetCopy()
{
    Constant* copy = new Constant(m_elementID);
    *copy = *this;
    copy->m_glText = m_glText->GetCopy();
    return copy;
}

bool Constant::UpdateText()
{
    SetValue(m_value);
    if(!m_glText->IsTextureOK()) return false;
    return true;
}

rapidxml::xml_node<>* Constant::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
    auto elementNode = XMLParser::AppendNode(doc, elementListNode, "Constant");
    XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

    SaveCADProperties(doc, elementNode);
    SaveControlNodes(doc, elementNode);

    // Element properties
    auto value = XMLParser::AppendNode(doc, elementNode, "Value");
    XMLParser::SetNodeValue(doc, value, m_value);
    
    return elementNode;
}

bool Constant::OpenElement(rapidxml::xml_node<>* elementNode)
{
    if(!OpenCADProperties(elementNode)) return false;
    if(!OpenControlNodes(elementNode)) return false;

    // Element properties
    double value = XMLParser::GetNodeValueDouble(elementNode, "Value");

    SetPosition(m_position);
    SetValue(value);
    return true;
}
