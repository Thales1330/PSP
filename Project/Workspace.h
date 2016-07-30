#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <wx/dcclient.h>
#include <wx/msgdlg.h>

#include "WorkspaceBase.h"
#include "Bus.h"

class Workspace : public WorkspaceBase
{
   protected:
    virtual void OnLeftClickDown(wxMouseEvent& event);
    virtual void OnPaint(wxPaintEvent& event);

    void SetViewport();

    wxGLContext* m_glContext;
    wxString m_name;
	
	std::vector<Element*> m_elementList;

   public:
    Workspace(wxWindow* parent, wxString name = wxEmptyString);
    ~Workspace();
	
	void Redraw() { this->Refresh(); }

    wxString GetName() const { return m_name; }
    void SetName(wxString name) { m_name = name; }
};

#endif  // WORKSPACE_H
