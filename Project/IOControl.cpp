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

#include "IOControl.h"
#include "IOControlForm.h"

IOControl::IOControl(int ioFlags, int id) : ControlElement(id)
{
    m_ioFlags = ioFlags;

    Node* node = new Node(m_position, Node::NODE_IN, m_borderSize);
    m_nodeList.push_back(node);

    if(ioFlags & IN_TERMINAL_VOLTAGE)
        SetValue(IN_TERMINAL_VOLTAGE);
    else if(ioFlags & IN_VELOCITY)
        SetValue(IN_VELOCITY);
    node->StartMove(m_position);
}

IOControl::~IOControl()
{
    if(m_glText) delete m_glText;
}

void IOControl::Draw(wxPoint2DDouble translation, double scale) const
{
    std::vector<wxPoint2DDouble> pts;
    if(m_angle == 0.0) {
        pts.push_back(m_rect.GetLeftTop() + wxPoint2DDouble(m_borderSize, m_borderSize));
        pts.push_back(m_rect.GetRightTop() + wxPoint2DDouble(-m_borderSize - 10, m_borderSize));
        pts.push_back(m_position + wxPoint2DDouble(m_width / 2 - m_borderSize, 0));
        pts.push_back(m_rect.GetRightBottom() + wxPoint2DDouble(-m_borderSize - 10, -m_borderSize));
        pts.push_back(m_rect.GetLeftBottom() + wxPoint2DDouble(m_borderSize, -m_borderSize));
    } else if(m_angle == 90.0) {
        pts.push_back(m_rect.GetLeftTop() + wxPoint2DDouble(m_borderSize, m_borderSize));
        pts.push_back(m_rect.GetRightTop() + wxPoint2DDouble(-m_borderSize, m_borderSize));
        pts.push_back(m_rect.GetRightBottom() + wxPoint2DDouble(-m_borderSize, -m_borderSize - 10));
        pts.push_back(m_position + wxPoint2DDouble(0, m_height / 2 - m_borderSize));
        pts.push_back(m_rect.GetLeftBottom() + wxPoint2DDouble(m_borderSize, -m_borderSize - 10));
    } else if(m_angle == 180.0) {
        pts.push_back(m_rect.GetLeftTop() + wxPoint2DDouble(m_borderSize + 10, m_borderSize));
        pts.push_back(m_rect.GetRightTop() + wxPoint2DDouble(-m_borderSize, m_borderSize));
        pts.push_back(m_rect.GetRightBottom() + wxPoint2DDouble(-m_borderSize, -m_borderSize));
        pts.push_back(m_rect.GetLeftBottom() + wxPoint2DDouble(m_borderSize + 10, -m_borderSize));
        pts.push_back(m_position + wxPoint2DDouble(-m_width / 2 + m_borderSize, 0));
    } else if(m_angle == 270.0) {
        pts.push_back(m_position + wxPoint2DDouble(0, -m_height / 2 + m_borderSize));
        pts.push_back(m_rect.GetRightTop() + wxPoint2DDouble(-m_borderSize, m_borderSize + 10));
        pts.push_back(m_rect.GetRightBottom() + wxPoint2DDouble(-m_borderSize, -m_borderSize));
        pts.push_back(m_rect.GetLeftBottom() + wxPoint2DDouble(m_borderSize, -m_borderSize));
        pts.push_back(m_rect.GetLeftTop() + wxPoint2DDouble(m_borderSize, m_borderSize + 10));
    }

    if(m_selected) {
        glColor4dv(m_selectionColour.GetRGBA());
        double borderSize = (m_borderSize * 2.0 + 1.0) / scale;
        std::vector<wxPoint2DDouble> selPts = pts;
        if(m_angle == 0.0) {
            selPts[0] += wxPoint2DDouble(-borderSize / 2, -borderSize / 2);
            selPts[1] += wxPoint2DDouble(borderSize / 2, -borderSize / 2);
            selPts[2] += wxPoint2DDouble(1.5 * borderSize / 2, 0);
            selPts[3] += wxPoint2DDouble(borderSize / 2, borderSize / 2);
            selPts[4] += wxPoint2DDouble(-borderSize / 2, borderSize / 2);
        } else if(m_angle == 90.0) {
            selPts[0] += wxPoint2DDouble(-borderSize / 2, -borderSize / 2);
            selPts[1] += wxPoint2DDouble(borderSize / 2, -borderSize / 2);
            selPts[2] += wxPoint2DDouble(borderSize / 2, borderSize / 2);
            selPts[3] += wxPoint2DDouble(0, 1.5 * borderSize / 2);
            selPts[4] += wxPoint2DDouble(-borderSize / 2, borderSize / 2);
        } else if(m_angle == 180.0) {
            selPts[0] += wxPoint2DDouble(-borderSize / 2, -borderSize / 2);
            selPts[1] += wxPoint2DDouble(borderSize / 2, -borderSize / 2);
            selPts[2] += wxPoint2DDouble(borderSize / 2, borderSize / 2);
            selPts[3] += wxPoint2DDouble(-borderSize / 2, borderSize / 2);
            selPts[4] += wxPoint2DDouble(-1.5 * borderSize / 2, 0);
        } else if(m_angle == 270.0) {
            selPts[0] += wxPoint2DDouble(0, -1.5 * borderSize / 2);
            selPts[1] += wxPoint2DDouble(borderSize / 2, -borderSize / 2);
            selPts[2] += wxPoint2DDouble(borderSize / 2, borderSize / 2);
            selPts[3] += wxPoint2DDouble(-borderSize / 2, borderSize / 2);
            selPts[4] += wxPoint2DDouble(-borderSize / 2, -borderSize / 2);
        }
        DrawLine(selPts, GL_POLYGON);
    }
    glLineWidth(1.0);
    glColor4d(1.0, 1.0, 1.0, 1.0);
    DrawLine(pts, GL_POLYGON);
    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawLine(pts, GL_LINE_LOOP);

    // Plot number.
    glColor4d(0.0, 0.0, 0.0, 1.0);
    if(m_angle == 0.0) {
        m_glText->Draw(m_position + wxPoint2DDouble(-5.0, 0.0));
    } else if(m_angle == 90.0) {
        m_glText->Draw(m_position + wxPoint2DDouble(0.0, -5.0));
    } else if(m_angle == 180.0) {
        m_glText->Draw(m_position + wxPoint2DDouble(5.0, 0.0));
    } else if(m_angle == 270.0) {
        m_glText->Draw(m_position + wxPoint2DDouble(0.0, 5.0));
    }

    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawNodes();
}

bool IOControl::ShowForm(wxWindow* parent, Element* element)
{
    IOControlForm* form = new IOControlForm(parent, this);
    if(form->ShowModal() == wxID_OK) {
        form->Destroy();
        return true;
    }
    form->Destroy();
    return false;
}

void IOControl::Rotate(bool clockwise)
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

wxString IOControl::GenerateText()
{
    wxString omega = wxString::FromUTF8("\xCF\x89");
    wxString subZero = wxString::FromUTF8("\xE2\x82\x92");
    wxString capDelta = wxString::FromUTF8("\xCE\x94");

    switch(m_value) {
        case IN_TERMINAL_VOLTAGE: {
            m_ioNodeType = Node::NODE_OUT;
            return _("Vt");
        } break;
        case IN_VELOCITY: {
            m_ioNodeType = Node::NODE_OUT;
            return omega;
        } break;
        case IN_ACTIVE_POWER: {
            m_ioNodeType = Node::NODE_OUT;
            return _("P");
        } break;
        case IN_REACTIVE_POWER: {
            m_ioNodeType = Node::NODE_OUT;
            return _("Q");
        } break;
        case OUT_FIELD_VOLTAGE: {
            m_ioNodeType = Node::NODE_IN;
            return _("Vf");
        } break;
        case OUT_MEC_POWER: {
            m_ioNodeType = Node::NODE_IN;
            return _("Pm");
        } break;
        case IN_INITIAL_TERMINAL_VOLTAGE: {
            m_ioNodeType = Node::NODE_OUT;
            return _("Vt") + subZero;
        } break;
        case IN_INITIAL_MEC_POWER: {
            m_ioNodeType = Node::NODE_OUT;
            return _("Pm") + subZero;
        } break;
        case IN_INITIAL_VELOCITY: {
            m_ioNodeType = Node::NODE_OUT;
            return omega + subZero;
        } break;
        case IN_DELTA_VELOCITY: {
            m_ioNodeType = Node::NODE_OUT;
            return capDelta + omega;
        } break;
        case IN_DELTA_ACTIVE_POWER: {
            m_ioNodeType = Node::NODE_OUT;
            return capDelta + _("P");
        } break;
    }
    return "";
}

void IOControl::SetValue(IOFlags value)
{
    m_value = value;
    wxString text = GenerateText();

    if(m_glText)
        m_glText->SetText(text);
    else
        m_glText = new OpenGLText(text);

    m_width = m_glText->GetWidth() + 10 + 2 * m_borderSize;
    m_height = m_glText->GetHeight() + 10 + 2 * m_borderSize;

    SetPosition(m_position);  // Update rectangle.

    UpdatePoints();
}

void IOControl::UpdatePoints()
{
    if(m_nodeList.size() != 0) {
        Node* node = m_nodeList[0];
        if(node->GetNodeType() != m_ioNodeType) {
            // Rotate 180 degrees
            node->Rotate();
            node->Rotate();
        }
        node->SetNodeType(m_ioNodeType);
        if(m_angle == 0.0) {
            if(m_ioNodeType == Node::NODE_IN)
                node->SetPosition(m_position + wxPoint2DDouble(-m_width / 2, 0));
            else
                node->SetPosition(m_position + wxPoint2DDouble(m_width / 2 - 2, 0));
        } else if(m_angle == 90.0) {
            if(m_ioNodeType == Node::NODE_IN)
                node->SetPosition(m_position + wxPoint2DDouble(0, -m_height / 2));
            else
                node->SetPosition(m_position + wxPoint2DDouble(0, m_height / 2 - 2));
        } else if(m_angle == 180.0) {
            if(m_ioNodeType == Node::NODE_IN)
                node->SetPosition(m_position + wxPoint2DDouble(m_width / 2, 0));
            else
                node->SetPosition(m_position + wxPoint2DDouble(2 - m_width / 2, 0));
        } else if(m_angle == 270.0) {
            if(m_ioNodeType == Node::NODE_IN)
                node->SetPosition(m_position + wxPoint2DDouble(0, m_height / 2));
            else
                node->SetPosition(m_position + wxPoint2DDouble(0, 2 - m_height / 2));
        }
    }
}

Element* IOControl::GetCopy()
{
    IOControl* copy = new IOControl(m_ioFlags, m_elementID);
    *copy = *this;
    copy->m_glText = m_glText->GetCopy();
    return copy;
}

bool IOControl::UpdateText()
{
    SetValue(m_value);
    if(!m_glText->IsTextureOK()) return false;
    return true;
}

void IOControl::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
    auto elementNode = XMLParser::AppendNode(doc, elementListNode, "IO");
    XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

    SaveCADProperties(doc, elementNode);
    SaveControlNodes(doc, elementNode);

    // Element properties
    auto value = XMLParser::AppendNode(doc, elementNode, "Value");
    XMLParser::SetNodeValue(doc, value, m_value);
    auto ioFlags = XMLParser::AppendNode(doc, elementNode, "IOFlags");
    XMLParser::SetNodeValue(doc, ioFlags, m_ioFlags);
}

bool IOControl::OpenElement(rapidxml::xml_node<>* elementNode)
{
    if(!OpenCADProperties(elementNode)) return false;
    if(!OpenControlNodes(elementNode)) return false;

    // Element properties
    IOControl::IOFlags value = static_cast<IOControl::IOFlags>(XMLParser::GetNodeValueInt(elementNode, "Value"));
    SetValue(value);

    return true;
}
