#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <wx/dcclient.h>
#include <wx/msgdlg.h>

class MouseEventsHandler;

#include "WorkspaceBase.h"
//#include "MouseEventsHandler.h"
#include "Bus.h"

class Camera;

class Workspace : public WorkspaceBase
{
   protected:
    virtual void OnMouseMotion(wxMouseEvent& event);
    virtual void OnKeyDown(wxKeyEvent& event) = 0;
    virtual void OnLeftClickDown(wxMouseEvent& event);
    virtual void OnPaint(wxPaintEvent& event);

    void SetViewport();

    wxGLContext* m_glContext;
    wxString m_name;

    bool m_insertMode = false;
    bool m_dragMode = false;

    std::vector<Element*> m_elementList;

   public:
    Workspace(wxWindow* parent, wxString name = wxEmptyString);
    ~Workspace();

    MouseEventsHandler* m_mouseEventsHandler;
    Camera* m_camera;

    wxString GetName() const { return m_name; }
    void SetName(wxString name) { m_name = name; }
    void SetDragMode(bool dragMode) { this->m_dragMode = dragMode; }
    void SetInsertMode(bool insertMode) { this->m_insertMode = insertMode; }
    bool IsDragMode() const { return m_dragMode; }
    const std::vector<Element*>& GetElementList() const { return m_elementList; }
    bool IsInsertMode() const { return m_insertMode; }
    void Redraw() { this->Refresh(); }
};

class Camera
{
   private:
    wxPoint2DDouble m_translation;
    double m_scale;

   public:
    Camera();
    ~Camera();

    void SetScale(double scale) { this->m_scale = scale; }
    void SetTranslation(const wxPoint2DDouble& translation) { this->m_translation = translation; }
    double GetScale() const { return m_scale; }
    const wxPoint2DDouble GetTranslation() const { return m_translation; }
    wxPoint2DDouble ScreenToWorld(wxPoint2DDouble screenCoords);
};

#endif  // WORKSPACE_H
