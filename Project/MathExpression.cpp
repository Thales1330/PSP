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
#include "MathExpression.h"
#include "MathExpressionForm.h"

MathExpression::MathExpression(int id) : ControlElement(id)
{
    m_variablesVector.push_back("x");
    m_variablesVector.push_back("y");

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

bool MathExpression::Solve(double* input, double timeStep)
{
    if(!input) {
        m_output = 0.0;
        return true;
    }
    // Get the input vector from connection lines (can't use default (one) input argument)
    m_inputValues[0] = input[1];  // Current time
    m_inputValues[1] = timeStep;
    m_inputValues[2] = input[2];  // Switch status
    int i = 3;
    for(auto itN = m_nodeList.begin(), itNEnd = m_nodeList.end(); itN != itNEnd; ++itN) {
        Node* node = *itN;
        if(node->GetNodeType() != Node::NODE_OUT) {
            if(!node->IsConnected()) {
                m_inputValues[i] = 0.0;  // Node not connected means zero value as input.
            } else {
                for(auto itC = m_childList.begin(), itCEnd = m_childList.end(); itC != itCEnd; ++itC) {
                    ConnectionLine* cLine = static_cast<ConnectionLine*>(*itC);
                    auto nodeList = cLine->GetNodeList();
                    for(auto itCN = nodeList.begin(), itCNEnd = nodeList.end(); itCN != itCNEnd; ++itCN) {
                        Node* childNode = *itCN;
                        if(childNode == node) {
                            if(!cLine->IsSolved())
                                m_inputValues[i] = 0.0;
                            else
                                m_inputValues[i] = cLine->GetValue();
                            break;
                        }
                    }
                }
            }
            ++i;
        }
    }

    // Solve the math expression using fparser
    double result = m_fparser.Eval(m_inputValues);
    if(m_fparser.EvalError() != 0) return false;
    m_output = result;
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

bool MathExpression::UpdateText()
{
    bool isTextureOK = true;
    m_symbol.SetText(m_symbol.GetText());
    if(!m_symbol.IsTextureOK()) isTextureOK = false;
    for(auto it = m_glTextInputVector.begin(), itEnd = m_glTextInputVector.end(); it != itEnd; ++it) {
        (*it)->SetText((*it)->GetText());
        if(!(*it)->IsTextureOK()) isTextureOK = false;
    }
    return isTextureOK;
}

void MathExpression::SetVariables(std::vector<wxString> variablesVector)
{
    m_variablesVector = variablesVector;
    // Clear old glTextVector
    for(auto it = m_glTextInputVector.begin(), itEnd = m_glTextInputVector.end(); it != itEnd; ++it) { delete *it; }
    m_glTextInputVector.clear();

    for(auto it = m_variablesVector.begin(), itEnd = m_variablesVector.end(); it != itEnd; ++it)
        m_glTextInputVector.push_back(new OpenGLText(*(it)));
}

bool MathExpression::Initialize()
{
    m_variables = "time,step,switch,";
    for(auto it = m_variablesVector.begin(), itEnd = m_variablesVector.end(); it != itEnd; ++it)
        m_variables += *(it) + ",";
    m_variables.RemoveLast();

    // Set locale to ENGLISH_US to avoid parser error in numbers (eg. comma).
    int currentLang = wxLocale::GetSystemLanguage();
    wxLocale newLocale(wxLANGUAGE_ENGLISH_US);
    int parserRes = m_fparser.Parse(static_cast<std::string>(m_mathExpression), static_cast<std::string>(m_variables));
    if(parserRes != -1) return false;  // Parse error.
    wxLocale oldLocale(currentLang);   // Return to current language.

    if(m_inputValues) delete m_inputValues;
    m_inputValues = new double[m_variablesVector.size() + 3];  // Custom variables + time + step + switch

    // Optimize only once to gain performance.
    m_fparser.Optimize();

    m_solved = false;
    m_output = 0.0;
    return true;
}

rapidxml::xml_node<>* MathExpression::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
    auto elementNode = XMLParser::AppendNode(doc, elementListNode, "MathExpr");
    XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

    SaveCADProperties(doc, elementNode);
    SaveControlNodes(doc, elementNode);

    // Element properties
    auto variablesNode = XMLParser::AppendNode(doc, elementNode, "VariableList");
    for(unsigned int i = 0; i < m_variablesVector.size(); ++i) {
        auto variable = XMLParser::AppendNode(doc, variablesNode, "Variable");
        XMLParser::SetNodeValue(doc, variable, m_variablesVector[i]);
    }
    auto mathExprValue = XMLParser::AppendNode(doc, elementNode, "MathExprValue");
    XMLParser::SetNodeValue(doc, mathExprValue, m_mathExpression);

    return elementNode;
}

bool MathExpression::OpenElement(rapidxml::xml_node<>* elementNode)
{
    if(!OpenCADProperties(elementNode)) return false;
    if(!OpenControlNodes(elementNode)) return false;

    // Element properties
    std::vector<wxString> variables;
    auto variablesNode = elementNode->first_node("VariableList");
    auto variable = variablesNode->first_node("Variable");
    while(variable) {
        variables.push_back(variable->value());
        variable = variable->next_sibling("Variable");
    }
    SetVariables(variables);

    auto mathExprValueNode = elementNode->first_node("MathExprValue");
    m_mathExpression = mathExprValueNode->value();

    // Init opened properties
    StartMove(m_position);
    UpdatePoints();

    return true;
}
