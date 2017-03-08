#ifndef SUM_H
#define SUM_H

#include "ControlElement.h"

class SumForm;

class Sum : public ControlElement
{
public:
    enum Signal { SIGNAL_POSITIVE = 0, SIGNAL_NEGATIVE };
    Sum();
    ~Sum();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    
    virtual std::vector<Signal> GetSignalList() const { return m_signalList; }
    virtual void SetSignalList(std::vector<Signal> signalList) { m_signalList = signalList; }
    
    virtual void UpdatePoints();

protected:
    std::vector<Signal> m_signalList;
};

#endif // SUM_H
