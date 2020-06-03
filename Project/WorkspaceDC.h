#pragma once
#include "Workspace.h"

#include <wx/graphics.h>
//#include <wx/dcclient.h>
#include <wx/dcbuffer.h>

class WorkspaceDC : public Workspace
{
public:
	WorkspaceDC();
	WorkspaceDC(wxWindow* parent, wxString name = wxEmptyString, wxStatusBar* statusBar = nullptr);
	~WorkspaceDC();

	virtual void Redraw() { this->Refresh(); }

	protected:
		virtual void OnPaint(wxPaintEvent& event);
		virtual void OnIdle(wxIdleEvent& event) {} // Prevent OpenGL checks
};

