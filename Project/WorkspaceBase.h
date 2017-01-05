//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: Workspace.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _PSP_PROJECT_WORKSPACE_BASE_CLASSES_H
#define _PSP_PROJECT_WORKSPACE_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/panel.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/glcanvas.h>
#include <wx/timer.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif


class WorkspaceBase : public wxPanel
{
protected:
    wxGLCanvas* m_glCanvas;
    wxTimer* m_timer;

protected:
    virtual void OnPaint(wxPaintEvent& event) { event.Skip(); }
    virtual void OnLeftClickDown(wxMouseEvent& event) { event.Skip(); }
    virtual void OnKeyDown(wxKeyEvent& event) { event.Skip(); }
    virtual void OnMouseMotion(wxMouseEvent& event) { event.Skip(); }
    virtual void OnMiddleDown(wxMouseEvent& event) { event.Skip(); }
    virtual void OnMiddleUp(wxMouseEvent& event) { event.Skip(); }
    virtual void OnLeftClickUp(wxMouseEvent& event) { event.Skip(); }
    virtual void OnScroll(wxMouseEvent& event) { event.Skip(); }
    virtual void OnRightClickDown(wxMouseEvent& event) { event.Skip(); }
    virtual void OnLeftDoubleClick(wxMouseEvent& event) { event.Skip(); }
    virtual void OnIdle(wxIdleEvent& event) { event.Skip(); }
    virtual void OnTimer(wxTimerEvent& event) { event.Skip(); }

public:
    wxGLCanvas* GetGlCanvas() { return m_glCanvas; }
    wxTimer* GetTimer() { return m_timer; }
    WorkspaceBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxTAB_TRAVERSAL);
    virtual ~WorkspaceBase();
};

#endif
