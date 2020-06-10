#pragma once
#include "Workspace.h"

#include <wx/graphics.h>
//#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
/**
 * @brief A Workspace class that draws using Device Context.
*/
class WorkspaceDC : public Workspace
{
public:
	/**
	 * @brief Default constructor
	*/
	WorkspaceDC();
	/**
	 * @brief WorkspaceDC constructor
	 * @param parent Parent window
	 * @param name Project name (displayed at the tabs)
	 * @param statusBar Status bar to show useful info
	 * @return 
	*/
	WorkspaceDC(wxWindow* parent, wxString name = wxEmptyString, wxStatusBar* statusBar = nullptr);
	~WorkspaceDC();

	/**
	 * @brief Redraws the screen
	*/
	virtual void Redraw() { this->Refresh(); }
	/**
	 * @brief Get shared OpenGL context. This method prevents to share any OpenGL context when use DC.
	 * @return Aways nullptr
	*/
	virtual wxGLContext* GetSharedGLContext() const { return nullptr; } // Prevent share any OpenGL context when use DC

	protected:
		virtual void OnPaint(wxPaintEvent& event);
		virtual void OnIdle(wxIdleEvent& event) {} // Prevent OpenGL checks
};

