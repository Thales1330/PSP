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

    wxPoint2DDouble GetPosition() const { return m_rect.GetPosition(); }
    void SetPosition(wxPoint2DDouble position);

    NodeType GetNodeType() const { return m_nodeType; }
    void SetNodeType(NodeType nodeType) { m_nodeType = nodeType; }

    double GetRadius() const { return m_radius; }
    std::vector<wxPoint2DDouble> GetInTrianglePts() const { return m_triPts; }

    void StartMove(wxPoint2DDouble position);
    void Move(wxPoint2DDouble position);
    bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }

protected:
    wxRect2DDouble m_rect;
    NodeType m_nodeType;

    wxPoint2DDouble m_moveStartPt;
    wxPoint2DDouble m_movePos;

    double m_radius = 3.0;
    std::vector<wxPoint2DDouble> m_triPts;
    std::vector<wxPoint2DDouble> m_triPtsMovePos;
};

class ControlElement : public Element
{
public:
    ControlElement();
    ~ControlElement();

    virtual void StartMove(wxPoint2DDouble position);
    virtual void Move(wxPoint2DDouble position);

    void SetNodeList(std::vector<Node> nodeList) { m_nodeList = nodeList; }
    std::vector<Node> GetNodeList() const { return m_nodeList; }

    virtual void DrawNodes() const;

protected:
    std::vector<Node> m_nodeList;
};

#endif // CONTROLELEMENT_H
