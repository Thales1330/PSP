#ifndef EXPONENTIAL_H
#define EXPONENTIAL_H

#include "ControlElement.h"

class ExponentialForm;

class Exponential : public ControlElement
{
public:
    Exponential(int id);
    ~Exponential();
    
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);

    virtual void UpdatePoints();
    
    virtual void GetValues(double& aValue, double &bValue);
    virtual void SetValues(double aValue, double bValue);
    
    virtual bool Solve(double input, double timeStep);
    
    virtual Element* GetCopy();
    
protected:
    double m_aValue = 0.001;
    double m_bValue = 5.0;

};

#endif // EXPONENTIAL_H
