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

#ifndef CONTROLELEMENT_H
#define CONTROLELEMENT_H

#include "Element.h"

/**
 * @class Node
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Node of a control element. This class manages the user interaction with the connection and control elements.
 * @file ControlElement.h
 */
class Node
{
   public:
    enum class NodeType { NODE_IN = 0, NODE_OUT };

    Node(wxPoint2DDouble position = wxPoint2DDouble(0, 0), NodeType nodeType = NodeType::NODE_IN, double borderSize = 0.0);
    ~Node();

    wxRect2DDouble GetRect() const { return m_rect; }
    void SetRect(wxRect2DDouble rect) { m_rect = rect; }
    wxPoint2DDouble GetPosition() const;
    void SetPosition(wxPoint2DDouble position);

    NodeType GetNodeType() const { return m_nodeType; }
    void SetNodeType(NodeType nodeType) { m_nodeType = nodeType; }
    double GetRadius() const { return m_radius; }
    std::vector<wxPoint2DDouble> GetInTrianglePts() const { return m_triPts; }
    double GetAngle() const { return m_angle; }
    void SetAngle(double angle) { m_angle = angle; }
    void Rotate(bool clockwise = true);

    void RotateTriPt(double angle);

    void StartMove(wxPoint2DDouble position);
    void Move(wxPoint2DDouble position);
    bool Contains(wxPoint2DDouble position) const;

    bool IsConnected() const { return m_connected; }
    void SetConnected(bool connected = true) { m_connected = connected; }
    int GetID() const { return m_id; }
    void SetID(int id) { m_id = id; }

   protected:
    int m_id = -1;

    wxRect2DDouble m_rect;
    NodeType m_nodeType;

    bool m_connected = false;

    wxPoint2DDouble m_moveStartPt;
    wxPoint2DDouble m_movePos;

    double m_radius = 3.0;
    std::vector<wxPoint2DDouble> m_triPts;
    double m_angle = 0.0;
};

/**
 * @class ControlElement
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @file ControlElement.h
 * @brief Base class of a control element. Provide general methods to other control classes.
 */
class ControlElement : public Element
{
   public:
    ControlElement(int id);
    ~ControlElement();

    virtual void StartMove(wxPoint2DDouble position);
    virtual void Move(wxPoint2DDouble position);

    void SetNodeList(std::vector<Node*> nodeList) { m_nodeList = nodeList; }
    std::vector<Node*> GetNodeList() const { return m_nodeList; }
    virtual void DrawNodes() const;
    virtual void ReplaceNode(Node* oldNode, Node* newNode);

    /**
     * @brief Update the OpenGL text in the element (if present).
     * @return true if OpenGLText is ok, false otherwise.
     */
    virtual bool UpdateText() { return true; }
    virtual bool IsSolved() const { return m_solved; }
    virtual void SetSolved(bool solved = true) { m_solved = solved; }
    virtual bool Solve(double* input, double timeStep);
    virtual bool Initialize();
    virtual double GetOutput() const { return m_output; }
    virtual void SetOutput(double output) { m_output = output; }
    
    static ControlElement* GetControlElementFromID(std::vector<ControlElement*> elementList, int id);

   protected:
    std::vector<Node*> m_nodeList;
    bool m_solved = false;
    double m_output = 0.0;
    
    void SaveControlNodes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementNode);
    bool OpenControlNodes(rapidxml::xml_node<>* elementNode);
};

#endif  // CONTROLELEMENT_H
