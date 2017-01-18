#ifndef BRANCH_H
#define BRANCH_H

#include "PowerElement.h"
#include "Bus.h"

class Branch : public PowerElement
{
public:
	Branch();
	~Branch();
	
	virtual bool Contains(wxPoint2DDouble position) const { return false; }
    virtual void Draw(wxPoint2DDouble translation, double scale) const {}
    virtual void Move(wxPoint2DDouble position) {}
    virtual void StartMove(wxPoint2DDouble position) {}
    virtual void MoveNode(Element* parent, wxPoint2DDouble position) {}
    virtual bool NodeContains(wxPoint2DDouble position);
    virtual bool SetNodeParent(Element* parent);
	virtual void RemoveParent(Element* parent);
    virtual void UpdateNodes();
    virtual wxCursor GetBestPickboxCursor() const { return wxCURSOR_ARROW; }
    virtual bool AddParent(Element* parent, wxPoint2DDouble position) { return false; }
    virtual bool Intersects(wxRect2DDouble rect) const { return false; }
    virtual void MovePickbox(wxPoint2DDouble position) {}
    virtual bool PickboxContains(wxPoint2DDouble position) { return false; }
    virtual void RotateNode(Element* parent, bool clockwise = true);
    virtual void AddPoint(wxPoint2DDouble point) {};
    virtual bool GetContextMenu(wxMenu& menu) { return false; }
	virtual void UpdateSwitchesPosition();
	virtual void UpdateSwitches();

   protected:
	bool m_inserted = false;
};

#endif // BRANCH_H
