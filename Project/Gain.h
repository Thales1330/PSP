#ifndef GAIN_H
#define GAIN_H

#include "ControlElement.h"

#include <wx/dcscreen.h>
#include "wxGLString.h"

class GainForm;

class Gain : public ControlElement
{
   public:
    Gain(int id);
    ~Gain();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);
    virtual void Move(wxPoint2DDouble position);
    
    virtual void SetValue(double value);
    virtual double GetValue() const { return m_value; }
    virtual void UpdatePoints();

   protected:
    double m_value = 1.0;
    
    wxGLString* m_glStringValue = NULL;
    int m_fontSize = 10;
    
    std::vector<wxPoint2DDouble> m_triPts;
};

#endif  // GAIN_H
