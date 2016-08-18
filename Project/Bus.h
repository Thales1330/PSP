#ifndef BUS_H
#define BUS_H

#include "Element.h"

class Bus : public Element
{
   public:
    Bus();
    Bus(wxPoint2DDouble position);
    ~Bus();
    virtual bool AddParent(Element* parent, wxPoint2DDouble position) { return true; }
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void Rotate();
    virtual wxCursor GetBestPickboxCursor() const;
    virtual void MovePickbox(wxPoint2DDouble position);
    virtual bool PickboxContains(wxPoint2DDouble position);
    virtual bool GetContextMenu(wxMenu& menu);
};

#endif  // BUS_H
