#ifndef TRANSFERFUNCTION_H
#define TRANSFERFUNCTION_H

#include "ControlElement.h"

#include <wx/dcscreen.h>
#include "wxGLString.h"

class TransferFunction : public ControlElement
{
public:
    TransferFunction();
    ~TransferFunction();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool AddParent(Element* parent, wxPoint2DDouble position) { return false; }
    virtual void SetText(wxString numerator, wxString denominator);
    virtual wxString GetSuperscriptNumber(int number);
    virtual void GetTFString(wxString& numerator, wxString& denominator);
    virtual void UpdateTFText();
    
protected:
    wchar_t m_supNumber[10];
    
    wxGLString* m_glStringNum = NULL;
    wxGLString* m_glStringDen = NULL;
    int m_fontSize = 10;
    std::vector<double> m_numerator;
    std::vector<double> m_denominator;
};

#endif // TRANSFERFUNCTION_H
