#ifndef IOCONTROL_H
#define IOCONTROL_H

#include "ControlElement.h"

#include <wx/dcscreen.h>
#include "wxGLString.h"

class IOControlForm;

class IOControl : public ControlElement
{
   public:
    enum IOFlags {
        IN_TERMINAL_VOLTAGE = 1 << 0,
        IN_VELOCITY = 1 << 1,
        IN_ACTIVE_POWER = 1 << 2,
        IN_REACTIVE_POWER = 1 << 3,
        OUT_FIELD_VOLTAGE = 1 << 4,
        OUT_MEC_POWER = 1 << 5
    };

    IOControl(int ioFlags, int id);
    ~IOControl();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);

    virtual wxString GenerateText();
    virtual void UpdatePoints();

    virtual IOFlags GetValue() const { return m_value; }
    virtual void SetValue(IOFlags value);
    virtual int GetIOFlags() const { return m_ioFlags; }
    virtual Node::NodeType GetType() { return m_ioNodeType; }

   protected:
    IOFlags m_value;
    int m_ioFlags;
    
    Node::NodeType m_ioNodeType = Node::NODE_IN;

    wxGLString* m_glStringValue = NULL;
    int m_fontSize = 10;
};

#endif  // IOCONTROL_H
