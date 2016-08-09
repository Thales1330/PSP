#ifndef LINE_H
#define LINE_H

#include <wx/msgdlg.h>

#include "Element.h"

class Line : public Element
{
   public:
    Line();
    ~Line();
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual wxCursor GetBestPickboxCursor() const;
    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void MovePickbox(wxPoint2DDouble position);
    virtual bool PickboxContains(wxPoint2DDouble position);
    virtual void Rotate();
    virtual void AddPoint(wxPoint2DDouble point);
   protected:
    std::vector<Element*> m_parentList;
    std::vector<wxPoint2DDouble> m_pointList;
};

#endif  // LINE_H
