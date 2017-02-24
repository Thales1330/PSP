#ifndef CONNECTIONLINE_H
#define CONNECTIONLINE_H

#include "ControlElement.h"

class ConnectionLine : public ControlElement
{
public:
    ConnectionLine();
    ~ConnectionLine();
    
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual bool Intersects(wxRect2DDouble rect) const;
};

#endif // CONNECTIONLINE_H
