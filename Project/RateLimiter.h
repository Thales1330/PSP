#ifndef RATELIMITER_H
#define RATELIMITER_H

#include "ControlElement.h"

class RateLimiterForm;

class RateLimiter : public ControlElement
{
   public:
    RateLimiter();
    ~RateLimiter();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);

    virtual void UpdatePoints();

    double GetUpLimit() const { return m_upLimit; }
    double GetLowLimit() const { return m_lowLimit; }
    void SetUpLimit(double upLimit) { m_upLimit = upLimit; }
    void SetLowLimit(double lowLimit) { m_lowLimit = lowLimit; }
    
   protected:
    double m_upLimit = 5.0;
    double m_lowLimit = -5.0;
};

#endif  // RATELIMITER_H
