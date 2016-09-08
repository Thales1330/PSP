#ifndef LINE_H
#define LINE_H

#include "Branch.h"

#include <wx/log.h>

class Line : public Branch
{
   public:
    Line();
    ~Line();
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void Move(wxPoint2DDouble position);
    virtual void StartMove(wxPoint2DDouble position);
    virtual void MoveNode(Element* parent, wxPoint2DDouble position);
    virtual wxCursor GetBestPickboxCursor() const { return wxCURSOR_SIZING; }
    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void MovePickbox(wxPoint2DDouble position);
    virtual bool PickboxContains(wxPoint2DDouble position);
    virtual void AddPoint(wxPoint2DDouble point);
    virtual bool GetContextMenu(wxMenu& menu);
    virtual void RemoveNode(wxPoint2DDouble point);
    virtual void AddNode(wxPoint2DDouble point);
	virtual void CalculateBoundaries(wxPoint2DDouble& leftUp, wxPoint2DDouble& rightBottom) const;

   protected:
    double PointToLineDistance(wxPoint2DDouble point, int* segmentNumber = NULL) const;
};

#endif  // LINE_H
