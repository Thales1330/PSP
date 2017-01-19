#ifndef CONTROLEDITOR_H
#define CONTROLEDITOR_H

#include "ControlEditorBase.h"
#include <wx/wrapsizer.h>
#include <wx/bmpbuttn.h>
#include <wx/dcclient.h>
#include <wx/graphics.h>

class ControlElementButton : public wxWindow
{
public:
    ControlElementButton(wxWindow* parent, wxString label, wxImage image);
    ~ControlElementButton();

protected:
    void OnPaint(wxPaintEvent& event);
    
    wxString m_label;
    wxImage m_image;
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
