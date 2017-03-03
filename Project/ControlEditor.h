#ifndef CONTROLEDITOR_H
#define CONTROLEDITOR_H

#include "ControlEditorBase.h"
#include <wx/wrapsizer.h>
#include <wx/dcclient.h>
#include <wx/dcscreen.h>
#include <wx/graphics.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Camera;
class ControlElement;
class TransferFunction;
class ConnectionLine;

enum ControlElementButtonID { ID_IO = 0, ID_TF, ID_SUM, ID_CONST, ID_LIMITER, ID_GAIN, ID_MULT, ID_SAT, ID_RATELIM };

class ControlElementButton : public wxWindow
{
public:
    ControlElementButton(wxWindow* parent, wxString label, wxImage image, wxWindowID id = wxID_ANY);
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
    enum ControlEditorMode {
        MODE_EDIT = 0,
        MODE_MOVE_ELEMENT,
        MODE_MOVE_LINE,
        MODE_DRAG,
        MODE_DRAG_INSERT,
        MODE_INSERT,
        MODE_INSERT_LINE,
        MODE_SELECTION_RECT,
        MODE_PASTE,
        MODE_DRAG_PASTE
    };
    ControlEditor(wxWindow* parent);
    virtual ~ControlEditor();

    virtual void AddElement(ControlElementButtonID id);
    virtual void Redraw() { m_glCanvas->Refresh(); }
    virtual void RotateSelectedElements(bool clockwise);

protected:
    virtual void OnKeyDown(wxKeyEvent& event);
    virtual void OnIdle(wxIdleEvent& event);
    virtual void OnScroll(wxMouseEvent& event);
    virtual void OnDoubleClick(wxMouseEvent& event);
    virtual void OnLeftClickDown(wxMouseEvent& event);
    virtual void OnLeftClickUp(wxMouseEvent& event);
    virtual void OnMiddleDown(wxMouseEvent& event);
    virtual void OnMiddleUp(wxMouseEvent& event);
    virtual void OnMouseMotion(wxMouseEvent& event);
    virtual void OnPaint(wxPaintEvent& event);
    virtual void LeftClickDown(wxMouseEvent& event);

    void BuildControlElementPanel();
    void SetViewport();

    wxGLContext* m_glContext = NULL;
    Camera* m_camera = NULL;

    ControlEditorMode m_mode = MODE_EDIT;

    wxRect2DDouble m_selectionRect;
    wxPoint2DDouble m_startSelRect;
    
    std::vector<ControlElement*> m_elementList;
    std::vector<ConnectionLine*> m_connectionList;
    
    bool m_firstDraw = true;
};
#endif // CONTROLEDITOR_H
