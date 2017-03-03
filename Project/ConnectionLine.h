#ifndef CONNECTIONLINE_H
#define CONNECTIONLINE_H

#include "ControlElement.h"

class ConnectionLine : public ControlElement
{
public:
    ConnectionLine(Node* firstNode);
    ~ConnectionLine();
    
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void StartMove(wxPoint2DDouble position);
    virtual void Move(wxPoint2DDouble position);
    virtual bool AppendNode(Node* node, ControlElement* parent);
    virtual void UpdatePoints();
    virtual void SetTemporarySecondPoint(wxPoint2DDouble point) { m_tmpSndPt = point; };
    
protected:
    double m_lineOffset = 0.0;
    double m_moveStartPtY = 0.0;
    double m_moveStartOffset = 0.0;
    wxPoint2DDouble m_tmpSndPt;
};

#endif // CONNECTIONLINE_H
