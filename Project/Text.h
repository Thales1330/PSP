#ifndef TEXT_H
#define TEXT_H

#include "Element.h"
#include "wxGLString.h"

class wxGLString;
class Bus;
class Line;
class Transformer;
class SyncGenerator;
class IndMotor;
class SyncMotor;
class Load;
class Inductor;
class Capacitor;

class Text : public Element
{
   public:
    Text();
    Text(wxPoint2DDouble position);
    ~Text();

    virtual bool AddParent(Element* parent, wxPoint2DDouble position) { return true; };
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual void Draw(wxPoint2DDouble translation, double scale, wxDC& dc);
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual wxString GetText() const { return m_text; }
    virtual void SetText(wxString text);
    
protected:

    wxString m_text = _("Text");
    wxGLString glString;
    bool consolidate = true;
};

#endif  // TEXT_H
