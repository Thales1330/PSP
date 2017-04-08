#ifndef CONSTANT_H
#define CONSTANT_H

#include "ControlElement.h"

#include <wx/dcscreen.h>
#include "wxGLString.h"

class ConstantForm;

class Constant : public ControlElement
{
   public:
    Constant(int id);
    ~Constant();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);
    
    virtual void SetValue(double value);
    virtual double GetValue() const { return m_value; }
    virtual void UpdatePoints();

   protected:
    double m_value = 1.0;

    wxGLString* m_glStringValue = NULL;
    int m_fontSize = 10;
};

#endif  // CONSTANT_H
