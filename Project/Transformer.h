#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "Element.h"

class Transformer : public Element
{
public:
   Transformer();
    virtual ~Transformer();
	
    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void Rotate();
	virtual void Move(wxPoint2DDouble position);
	virtual void MoveNode(Element* parent, wxPoint2DDouble position);
	virtual void StartMove(wxPoint2DDouble position);
	virtual void RemoveParent(Element* parent);
	virtual bool NodeContains(wxPoint2DDouble position);
    virtual bool SetNodeParent(Element* parent);
	virtual void UpdateNodes();
	virtual void RotateNode(Element* parent);
	virtual bool GetContextMenu(wxMenu& menu);
	
	protected:
    void UpdateSwitchesPosition();
    std::vector<wxPoint2DDouble> m_pointList;
    bool m_inserted = false;
    std::vector<wxPoint2DDouble> m_movePts;
};

#endif  // TRANSFORMER_H
