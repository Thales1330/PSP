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
    Node* nodeOut = new Node(m_position + wxPoint2DDouble(m_width / 2, 0), Node::NODE_OUT, m_borderSize);
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
