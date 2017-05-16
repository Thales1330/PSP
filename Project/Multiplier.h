#ifndef MULTIPLIER_H
#define MULTIPLIER_H

#include "ControlElement.h"

class ConnectionLine;

class Multiplier : public ControlElement
{
   public:
    Multiplier(int id);
    ~Multiplier();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element) { return false; }
    virtual void Rotate(bool clockwise = true);

    virtual void UpdatePoints();
    
    virtual bool Solve(double input, double timeStep);
    
    virtual Element* GetCopy();
};

#endif  // MULTIPLIER_H
