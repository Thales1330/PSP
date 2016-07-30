#ifndef BUS_H
#define BUS_H

#include "Element.h"

class Bus : public Element
{
   public:
    Bus(wxPoint2DDouble position);
    ~Bus();
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual wxCursor GetBestPickboxCursor() const;
    virtual void MovePickbox(wxPoint2DDouble position, int pickboxID);
    virtual int PickboxContains(wxPoint2DDouble position) const;
};

#endif  // BUS_H
