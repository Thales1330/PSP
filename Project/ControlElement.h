#ifndef CONTROLELEMENT_H
#define CONTROLELEMENT_H

#include "Element.h"

class Node
{
   public:
    enum NodeType { NODE_IN = 0, NODE_OUT };

    Node(wxPoint2DDouble position = wxPoint2DDouble(0, 0), NodeType nodeType = NODE_IN, double borderSize = 0.0);
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

    virtual bool IsSolved() const { return m_solved; }
    virtual void SetSolved(bool solved = true) { m_solved = solved; }
    virtual bool Solve(double input, double timeStep);
    virtual double GetOutput() const { return m_output; }
    virtual void SetOutput(double output) { m_output = output; }
   protected:
    std::vector<Node*> m_nodeList;
    bool m_solved = false;
    double m_output = 0.0;
};

#endif  // CONTROLELEMENT_H
