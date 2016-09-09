#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <wx/dcclient.h>
#include <wx/msgdlg.h>
#include <wx/statusbr.h>

#include "WorkspaceBase.h"
#include "Bus.h"

class Camera;
class Element;
//class Bus;
class Line;
class Transformer;
class SyncGenerator;
class IndMotor;
class SyncMotor;
class Load;
class Inductor;
class Capacitor;

enum WorkspaceMode
{
    MODE_EDIT = 0,
    MODE_MOVE_ELEMENT,
    MODE_MOVE_PICKBOX,
	MODE_MOVE_NODE,
    MODE_DRAG,
	MODE_DRAG_INSERT,
    MODE_INSERT,
    MODE_SELECTION_RECT
};

class Workspace : public WorkspaceBase
{
   public:
    Workspace();
    Workspace(wxWindow* parent, wxString name = wxEmptyString, wxStatusBar* statusBar = NULL);
    ~Workspace();

    wxString GetName() const { return m_name; }
    std::vector<Element*> GetElementList() const { return m_elementList; }
	WorkspaceMode GetWorkspaceMode() const { return m_mode; }
	Camera* GetCamera() const { return m_camera; }
	
	void SetName(wxString name) { m_name = name; }
	void SetElementList(std::vector<Element*> elementList) { m_elementList = elementList; }
	void SetStatusBarText(wxString text) { m_statusBar->SetStatusText(text); }
	void SetWorkspaceMode(WorkspaceMode mode) { m_mode = mode; }
	
	void Redraw() { m_glCanvas->Refresh(); }
	void RotateSelectedElements(bool clockwise = true);
	void DeleteSelectedElements();
	void Fit();

   protected:
    virtual void OnLeftDoubleClick(wxMouseEvent& event);
    virtual void OnRightClickDown(wxMouseEvent& event);
    virtual void OnLeftClickUp(wxMouseEvent& event);
    virtual void OnScroll(wxMouseEvent& event);
    virtual void OnMiddleDown(wxMouseEvent& event);
    virtual void OnMiddleUp(wxMouseEvent& event);
    virtual void OnMouseMotion(wxMouseEvent& event);
    virtual void OnKeyDown(wxKeyEvent& event);
    virtual void OnLeftClickDown(wxMouseEvent& event);
    virtual void OnPaint(wxPaintEvent& event);
    virtual void OnPopupClick(wxCommandEvent& event);

    void SetViewport();

    wxGLContext* m_glContext;
    wxStatusBar* m_statusBar;
    Camera* m_camera;
    wxString m_name;

    WorkspaceMode m_mode = MODE_EDIT;

    std::vector<Element*> m_elementList;

    void UpdateStatusBar();

   private:
    wxRect2DDouble m_selectionRect;
    wxPoint2DDouble m_startSelRect;
};

class Camera
{
   public:
    Camera();
    ~Camera();

    void SetScale(wxPoint2DDouble screenPoint, double delta);
	void SetScale(double scale) { m_scale = scale; }
    void SetTranslation(wxPoint2DDouble screenPoint);
    void StartTranslation(wxPoint2DDouble startPoint) { this->m_translationStartPt = startPoint; }
    void UpdateMousePosition(wxPoint2DDouble mousePosition) { this->m_mousePosition = mousePosition; }
    double GetScale() const { return m_scale; }
    wxPoint2DDouble GetTranslation() const { return m_translation; }
    wxPoint2DDouble GetMousePosition(bool worldCoords = true) const;
    wxPoint2DDouble ScreenToWorld(wxPoint2DDouble screenCoords) const;
	double GetZoomMin() const { return m_zoomMin; }
	double GetZoomMax() const { return m_zoomMax; }

   protected:
    wxPoint2DDouble m_translation;
    wxPoint2DDouble m_translationStartPt;
    double m_scale;

    wxPoint2DDouble m_mousePosition;
	
	double m_zoomMin = 0.05;
	double m_zoomMax = 3.0;
};

#endif  // WORKSPACE_H
