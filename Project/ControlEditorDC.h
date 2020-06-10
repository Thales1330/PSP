#ifndef CONTROLEDITORDC_H
#define CONTROLEDITORDC_H

#include "ControlEditor.h"

#include <wx/graphics.h>
#include <wx/dcbuffer.h>

class ControlEditorDC : public ControlEditor
{
public:
    ControlEditorDC(wxWindow* parent,
        int ioflags = IOControl::IN_TERMINAL_VOLTAGE | IOControl::IN_VELOCITY | IOControl::OUT_FIELD_VOLTAGE |
            IOControl::OUT_MEC_POWER);
    ~ControlEditorDC();
    
    virtual void Redraw() { m_panelWorkspace->Refresh(); }

protected:
    virtual void OnPaint(wxPaintEvent& event);
    virtual void OnIdle(wxIdleEvent& event) {} // Prevent OpenGL checks
};

#endif // CONTROLEDITORDC_H
