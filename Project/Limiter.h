#ifndef LIMITER_H
#define LIMITER_H

#include "ControlElement.h"

class LimiterForm;

class Limiter : public ControlElement
{
public:
    Limiter(int id);
    ~Limiter();
    
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);
    virtual bool Solve(double input, double timeStep);

    virtual void UpdatePoints();
    
    double GetUpLimit() const { return m_upLimit; }
    double GetLowLimit() const { return m_lowLimit; }
    
    void SetUpLimit(double upLimit) { m_upLimit = upLimit; }
    void SetLowLimit(double lowLimit) { m_lowLimit = lowLimit; }
    
    virtual Element* GetCopy();
    
protected:
    double m_upLimit = 5.0;
    double m_lowLimit = -5.0;
};

#endif // LIMITER_H
