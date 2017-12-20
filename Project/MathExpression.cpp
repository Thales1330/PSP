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

#include "MathExpression.h"
#include "MathExpressionForm.h"

MathExpression::MathExpression(int id) : ControlElement(id)
{
    m_variablesVector.push_back("x");
    // m_variablesVector.push_back("y");

    for(unsigned int i = 0; i < m_variablesVector.size(); ++i) {
        m_glTextInputVector.push_back(new OpenGLText(m_variablesVector[i]));
    }

    // Symbol
    m_symbol.SetFontSize(12);
    m_symbol.SetFontWeight(wxFONTWEIGHT_BOLD);
    m_symbol.SetFontStyle(wxFONTSTYLE_ITALIC);
    m_symbol.SetFontFamily(wxFONTFAMILY_ROMAN);
    m_symbol.SetText("f(x)");
    m_symbolSize = wxSize(m_symbol.GetWidth(), m_symbol.GetHeight());

    CalculateBlockSize(static_cast<double>(m_variablesVector.size()));

    for(unsigned int i = 0; i < m_variablesVector.size(); ++i) {
        wxPoint2DDouble nodePosition(0, 0);
        if(m_variablesVector.size() == 1) {
            nodePosition = m_position + wxPoint2DDouble(-m_width / 2, 0);
        } else {
            nodePosition = m_position + wxPoint2DDouble(-m_width / 2, 9 + 18 * i - m_height / 2);
        }
        Node* nodeIn = new Node(nodePosition, Node::NODE_IN, m_borderSize);
        nodeIn->StartMove(m_position);
        m_nodeList.push_back(nodeIn);
    }
    Node* nodeOut = new Node(m_position + wxPoint2DDouble(m_width / 2, 0), Node::NODE_OUT, m_borderSize);
    nodeOut->SetAngle(180.0);
    nodeOut->StartMove(m_position);
    m_nodeList.push_back(nodeOut);

    UpdatePoints();
}

MathExpression::~MathExpression()
{
    for(auto it = m_glTextInputVector.begin(), itEnd = m_glTextInputVector.end(); it != itEnd; ++it) { delete *it; }
    m_glTextInputVector.clear();
}

void MathExpression::Draw(wxPoint2DDouble translation, double scale) const
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

    // Plot input variables and symbol.
    glColor4d(0.0, 0.3, 1.0, 1.0);
    if(m_angle == 0.0) {
        m_symbol.Draw(m_nodeList[m_nodeList.size() - 1]->GetPosition() -
                      wxPoint2DDouble(m_symbolSize.GetWidth() / 2.0 + 6.0, 0));
        glColor4d(0.0, 0.0, 0.0, 1.0);
        for(unsigned int i = 0; i < m_glTextInputVector.size(); ++i) {
            m_glTextInputVector[i]->Draw(m_nodeList[i]->GetPosition() +
                                         wxPoint2DDouble(m_glTextInputVector[i]->GetWidth() / 2.0 + 6, 0));
        }
    } else if(m_angle == 90.0) {
        m_symbol.Draw(m_nodeList[m_nodeList.size() - 1]->GetPosition() -
                      wxPoint2DDouble(0, m_symbolSize.GetHeight() / 2.0 + 6.0));
        glColor4d(0.0, 0.0, 0.0, 1.0);
        for(unsigned int i = 0; i < m_glTextInputVector.size(); ++i) {
            m_glTextInputVector[i]->Draw(
                m_nodeList[i]->GetPosition() +
                    wxPoint2DDouble(m_glTextInputVector[i]->GetWidth() / 2.0 + m_glTextInputVector[i]->GetHeight() / 2,
                                    15),
                90);
        }
    } else if(m_angle == 180.0) {
        m_symbol.Draw(m_nodeList[m_nodeList.size() - 1]->GetPosition() +
                      wxPoint2DDouble(m_symbolSize.GetWidth() / 2.0 + 6.0, 0));
        glColor4d(0.0, 0.0, 0.0, 1.0);
        for(unsigned int i = 0; i < m_glTextInputVector.size(); ++i) {
            m_glTextInputVector[i]->Draw(m_nodeList[i]->GetPosition() -
                                         wxPoint2DDouble(m_glTextInputVector[i]->GetWidth() / 2.0 + 6, 0));
        }
    } else if(m_angle == 270.0) {
        m_symbol.Draw(m_nodeList[m_nodeList.size() - 1]->GetPosition() +
                      wxPoint2DDouble(0, m_symbolSize.GetHeight() / 2.0 + 6.0));
        glColor4d(0.0, 0.0, 0.0, 1.0);
        for(unsigned int i = 0; i < m_glTextInputVector.size(); ++i) {
            m_glTextInputVector[i]->Draw(
                m_nodeList[i]->GetPosition() + wxPoint2DDouble(m_glTextInputVector[i]->GetWidth() / 2.0 +
                                                                   m_glTextInputVector[i]->GetHeight() / 2.0,
                                                               -m_glTextInputVector[i]->GetWidth()),
                90);
        }
    }

    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawNodes();
}

bool MathExpression::ShowForm(wxWindowMSW* parent, Element* element)
{
    MathExpressionForm* mathExprForm = new MathExpressionForm(parent, this);
    if(mathExprForm->ShowModal() == wxID_OK) {
        mathExprForm->Destroy();
        return true;
    }
    mathExprForm->Destroy();
    return false;
}

void MathExpression::Rotate(bool clockwise)
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

bool MathExpression::Solve(double input, double timeStep)
{
    // Solve the math expression using fparser
    return true;
}

Element* MathExpression::GetCopy()
{
    MathExpression* copy = new MathExpression(m_elementID);
    *copy = *this;
    copy->m_glTextInputVector.clear();
    for(auto it = m_glTextInputVector.begin(), itEnd = m_glTextInputVector.end(); it != itEnd; ++it) {
        copy->m_glTextInputVector.push_back((*it)->GetCopy());
    }
    return copy;
}

void MathExpression::UpdatePoints()
{
    CalculateBlockSize(static_cast<double>(m_nodeList.size()) - 1.0);

    if(m_nodeList.size() == 2)  // Only one input (and the output).
    {
        if(m_angle == 0.0)
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(-m_width / 2, 0));
        else if(m_angle == 90.0)
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(0, -m_height / 2));
        else if(m_angle == 180.0)
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(m_width / 2, 0));
        else if(m_angle == 270.0)
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(0, m_height / 2));
    } else {
        for(unsigned int i = 0; i < m_nodeList.size() - 1; ++i) {
            if(m_angle == 0.0)
                m_nodeList[i]->SetPosition(m_position + wxPoint2DDouble(-m_width / 2, 9 + 18 * i - m_height / 2));
            else if(m_angle == 90.0)
                m_nodeList[i]->SetPosition(m_position + wxPoint2DDouble(m_width / 2 - 9 - 18 * i, -m_height / 2));
            else if(m_angle == 180.0)
                m_nodeList[i]->SetPosition(m_position + wxPoint2DDouble(m_width / 2, m_height / 2 - 9 - 18 * i));
            else if(m_angle == 270.0)
                m_nodeList[i]->SetPosition(m_position + wxPoint2DDouble(9 + 18 * i - m_width / 2, m_height / 2));
        }
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

void MathExpression::AddInNode()
{
    Node* newNode = new Node(wxPoint2DDouble(0, 0), Node::NODE_IN, m_borderSize);
    newNode->SetAngle(m_angle);
    m_nodeList.insert(m_nodeList.end() - 1, newNode);
}

void MathExpression::RemoveInNode()
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

void MathExpression::CalculateBlockSize(double numInNodes)
{
    m_maxSringSize = 0;
    for(auto it = m_glTextInputVector.begin(), itEnd = m_glTextInputVector.end(); it != itEnd; ++it) {
        if(m_maxSringSize < (*it)->GetWidth()) m_maxSringSize = (*it)->GetWidth();
    }
    if(m_angle == 0.0 || m_angle == 180.0) {
        m_height = 18.0 * numInNodes;
        if(m_height < m_minimumSize) m_height = m_minimumSize;  // minimum height
        m_width = m_maxSringSize + m_symbolSize.GetWidth() + 18;
    } else {
        m_width = 18.0 * numInNodes;
        if(m_width < m_minimumSize) m_width = m_minimumSize;  // minimum width
        m_height = m_maxSringSize + m_symbolSize.GetHeight() + 18;
    }
}
