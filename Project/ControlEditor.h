#ifndef CONTROLEDITOR_H
#define CONTROLEDITOR_H

#include "ControlEditorBase.h"
#include <wx/wrapsizer.h>
#include <wx/dcclient.h>
#include <wx/dcscreen.h>
#include <wx/graphics.h>

class ControlElementButton : public wxWindow
{
public:
    ControlElementButton(wxWindow* parent, wxString label, wxImage image);
    ~ControlElementButton();

protected:
    virtual void OnPaint(wxPaintEvent& event);
    virtual void OnMouseEnter(wxMouseEvent& event);
    virtual void OnMouseLeave(wxMouseEvent& event);
    virtual void OnLeftClickDown(wxMouseEvent& event);
    virtual void OnLeftClickUp(wxMouseEvent& event);

    wxString m_label;
    wxFont m_font;
    wxPoint m_labelPosition;
    
    wxImage m_image;
    wxSize m_imageSize;
    wxPoint m_imagePosition;
    
    int m_borderSize = 2;
    bool m_mouseAbove = false;
    bool m_selected = false;
    
    wxSize m_buttonSize;
};

class ControlEditor : public ControlEditorBase
{
public:
    ControlEditor(wxWindow* parent);
    virtual ~ControlEditor();

protected:
    void BuildControlElementPanel();
    virtual void LeftClickDown(wxMouseEvent& event);

    wxButton* m_tfButton;
};
#endif // CONTROLEDITOR_H
