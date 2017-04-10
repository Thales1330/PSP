#ifndef CONNECTIONLINE_H
#define CONNECTIONLINE_H

#include "ControlElement.h"

class ConnectionLine : public ControlElement
{
   public:
    enum ConnectionLineType { ELEMENT_ELEMENT = 0, ELEMENT_LINE };
    ConnectionLine(Node* firstNode, int id);
    ~ConnectionLine();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void RemoveParent(Element* parent);
    virtual void StartMove(wxPoint2DDouble position);
    virtual void Move(wxPoint2DDouble position);
    virtual bool AppendNode(Node* node, ControlElement* parent);
    virtual void UpdatePoints();
    virtual void SetTemporarySecondPoint(wxPoint2DDouble point) { m_tmpSndPt = point; }
    virtual wxPoint2DDouble GetMidPoint() const;
    virtual double GetOffset() const { return m_lineOffset; }
    virtual ConnectionLineType GetType() const { return m_type; }
    virtual void SetType(ConnectionLineType newType) { m_type = newType; }
    virtual ConnectionLine* GetParentLine() const { return m_parentLine; }
    virtual bool SetParentLine(ConnectionLine* parent);

    virtual std::vector<ConnectionLine*> GetLineChildList() const;

   protected:
    double m_lineOffset = 0.0;
    double m_moveStartPtY = 0.0;
    double m_moveStartOffset = 0.0;
    wxPoint2DDouble m_tmpSndPt;

    ConnectionLineType m_type = ELEMENT_ELEMENT;
    ConnectionLine* m_parentLine = NULL;
};

#endif  // CONNECTIONLINE_H
