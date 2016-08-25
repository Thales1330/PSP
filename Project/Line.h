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
    virtual void Move(wxPoint2DDouble position);
    virtual void StartMove(wxPoint2DDouble position);
    virtual void MoveNode(Element* parent, wxPoint2DDouble position);
    virtual bool NodeContains(wxPoint2DDouble position);
    virtual bool SetNodeParent(Element* parent);
    virtual void UpdateNodes();
    virtual wxCursor GetBestPickboxCursor() const { return wxCURSOR_SIZING; }
    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void MovePickbox(wxPoint2DDouble position);
    virtual bool PickboxContains(wxPoint2DDouble position);
    virtual void Rotate() {}
    virtual void RotateNode(Element* parent);
    virtual void AddPoint(wxPoint2DDouble point);
    virtual bool GetContextMenu(wxMenu& menu);
    virtual void RemoveNode(wxPoint2DDouble point);
    virtual void AddNode(wxPoint2DDouble point);

   protected:
    void UpdateSwitchesPosition();
    double PointToLineDistance(wxPoint2DDouble point, int* segmentNumber = NULL) const;
    std::vector<wxPoint2DDouble> m_pointList;
    bool m_inserted = false;
    std::vector<wxPoint2DDouble> m_movePts;
};

#endif  // LINE_H
