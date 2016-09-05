#ifndef SHUNT_H
#define SHUNT_H

#include "Element.h"

class Shunt : public Element
{
public:
	Shunt();
	~Shunt();
	
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual void Move(wxPoint2DDouble position);
    virtual void MoveNode(Element* element, wxPoint2DDouble position);
    virtual void StartMove(wxPoint2DDouble position);
    virtual void RotateNode(Element* parent);
    virtual void RemoveParent(Element* parent);
    virtual bool NodeContains(wxPoint2DDouble position);
    virtual bool SetNodeParent(Element* parent);
    virtual void UpdateNodes();

   protected:
    void UpdateSwitchesPosition();
	void DrawGround(wxPoint2DDouble position) const;
    bool m_inserted = false;

};

#endif // SHUNT_H
