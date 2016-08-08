#ifndef LINE_H
#define LINE_H

#include "Element.h"

class Line : public Element
{
   public:
    Line();
    ~Line();
	virtual bool Contains(wxPoint2DDouble position) const;
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual wxCursor GetBestPickboxCursor() const;
    virtual void Insert(Element* parent, wxPoint2DDouble position);
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void MovePickbox(wxPoint2DDouble position);
    virtual bool PickboxContains(wxPoint2DDouble position);
    virtual void Rotate();
};

#endif  // LINE_H
