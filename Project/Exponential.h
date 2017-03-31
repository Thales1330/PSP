#ifndef EXPONENTIAL_H
#define EXPONENTIAL_H

#include "ControlElement.h"

class Exponential : public ControlElement
{
public:
    Exponential();
    ~Exponential();
    
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);

    virtual void UpdatePoints();

};

#endif // EXPONENTIAL_H
