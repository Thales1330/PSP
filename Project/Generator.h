#ifndef GENERATOR_H
#define GENERATOR_H

#include "Element.h"

class Generator : public Element
{
   public:
    Generator();
    ~Generator();
    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
	virtual void Move(wxPoint2DDouble position);
	virtual void MoveNode(Element* element, wxPoint2DDouble position);
	virtual void StartMove(wxPoint2DDouble position);
	virtual void RotateNode(Element* parent);
	virtual void RemoveParent(Element* parent);
	virtual bool NodeContains(wxPoint2DDouble position);
    virtual bool SetNodeParent(Element* parent);
	virtual void UpdateNodes();
	virtual bool GetContextMenu(wxMenu& menu);
	virtual void Rotate();
	
   private:
    bool m_inserted = false;
    std::vector<wxPoint2DDouble> m_pointList;
    void UpdateSwitchesPosition();
	std::vector<wxPoint2DDouble> m_movePts;
	std::vector<wxPoint2DDouble> m_sinePts;
};

#endif  // GENERATOR_H
