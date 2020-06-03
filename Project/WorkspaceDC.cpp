#include "WorkspaceDC.h"

#include "Camera.h"
#include "Element.h"
#include "Workspace.h"
//#include "Bus.h"
#include "Capacitor.h"
#include "ElementDataObject.h"
#include "HarmCurrent.h"
#include "IndMotor.h"
#include "Inductor.h"
#include "Line.h"
#include "Load.h"
#include "SyncGenerator.h"
#include "SyncMotor.h"
#include "Transformer.h"

#include "Text.h"

#include "Electromechanical.h"
#include "Fault.h"
#include "PowerFlow.h"
#include "PowerQuality.h"

#include "ChartView.h"
#include "ElementPlotData.h"

#include "PropertiesData.h"

#include "FrequencyResponseForm.h"

WorkspaceDC::WorkspaceDC()
{
}

WorkspaceDC::WorkspaceDC(wxWindow* parent, wxString name, wxStatusBar* statusBar) : Workspace(parent, name, statusBar, nullptr)
{
	// Disconnect events from GLCanvas
	m_glCanvas->Disconnect(wxEVT_PAINT, wxPaintEventHandler(Workspace::OnPaint), NULL, this);
	m_glCanvas->Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(Workspace::OnLeftClickDown), NULL, this);
	m_glCanvas->Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(Workspace::OnKeyDown), NULL, this);
	m_glCanvas->Disconnect(wxEVT_MOTION, wxMouseEventHandler(Workspace::OnMouseMotion), NULL, this);
	m_glCanvas->Disconnect(wxEVT_MIDDLE_DOWN, wxMouseEventHandler(Workspace::OnMiddleDown), NULL, this);
	m_glCanvas->Disconnect(wxEVT_MIDDLE_UP, wxMouseEventHandler(Workspace::OnMiddleUp), NULL, this);
	m_glCanvas->Disconnect(wxEVT_LEFT_UP, wxMouseEventHandler(Workspace::OnLeftClickUp), NULL, this);
	m_glCanvas->Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(Workspace::OnScroll), NULL, this);
	m_glCanvas->Disconnect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(Workspace::OnRightClickDown), NULL, this);
	m_glCanvas->Disconnect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(Workspace::OnLeftDoubleClick), NULL, this);
	m_glCanvas->Disconnect(wxEVT_IDLE, wxIdleEventHandler(Workspace::OnIdle), NULL, this);
	m_glCanvas->Disconnect(wxEVT_MIDDLE_DCLICK, wxMouseEventHandler(Workspace::OnMiddleDoubleClick), NULL, this);

	// Reconnect events to this
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(WorkspaceDC::OnPaint), NULL, this); // Connect to overloaded method
	this->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(Workspace::OnLeftClickDown), NULL, this);
	this->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(Workspace::OnKeyDown), NULL, this);
	this->Connect(wxEVT_MOTION, wxMouseEventHandler(Workspace::OnMouseMotion), NULL, this);
	this->Connect(wxEVT_MIDDLE_DOWN, wxMouseEventHandler(Workspace::OnMiddleDown), NULL, this);
	this->Connect(wxEVT_MIDDLE_UP, wxMouseEventHandler(Workspace::OnMiddleUp), NULL, this);
	this->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(Workspace::OnLeftClickUp), NULL, this);
	this->Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(Workspace::OnScroll), NULL, this);
	this->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(Workspace::OnRightClickDown), NULL, this);
	this->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(Workspace::OnLeftDoubleClick), NULL, this);
	this->Connect(wxEVT_IDLE, wxIdleEventHandler(WorkspaceDC::OnIdle), NULL, this);  // Connect to overloaded method
	this->Connect(wxEVT_MIDDLE_DCLICK, wxMouseEventHandler(Workspace::OnMiddleDoubleClick), NULL, this);

	this->GetSizer()->Remove(this->GetSizer()->GetChildren()[0]->GetId()); // remove m_glCanvas object from sizer

	//for (int i = 0; i < this->GetSizer()->GetChildren().GetCount(); ++i) {
		//wxMessageBox(wxString::Format("%d", this->GetSizer()->GetChildren()[i]->GetId()));
	//}

	delete m_glCanvas; // Delete GLcanvas to allow drawing with wxDC
	m_glCanvas = nullptr;
	SetBackgroundColour(wxColour(255, 255, 255));
	SetBackgroundStyle(wxBG_STYLE_PAINT); // To allow wxBufferedPaintDC works properly.
	Redraw();
}

WorkspaceDC::~WorkspaceDC()
{
	// Disconnect events
	this->Disconnect(wxEVT_PAINT, wxPaintEventHandler(WorkspaceDC::OnPaint), NULL, this);
	this->Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(Workspace::OnLeftClickDown), NULL, this);
	this->Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(Workspace::OnKeyDown), NULL, this);
	this->Disconnect(wxEVT_MOTION, wxMouseEventHandler(Workspace::OnMouseMotion), NULL, this);
	this->Disconnect(wxEVT_MIDDLE_DOWN, wxMouseEventHandler(Workspace::OnMiddleDown), NULL, this);
	this->Disconnect(wxEVT_MIDDLE_UP, wxMouseEventHandler(Workspace::OnMiddleUp), NULL, this);
	this->Disconnect(wxEVT_LEFT_UP, wxMouseEventHandler(Workspace::OnLeftClickUp), NULL, this);
	this->Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(Workspace::OnScroll), NULL, this);
	this->Disconnect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(Workspace::OnRightClickDown), NULL, this);
	this->Disconnect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(Workspace::OnLeftDoubleClick), NULL, this);
	this->Disconnect(wxEVT_IDLE, wxIdleEventHandler(WorkspaceDC::OnIdle), NULL, this);
	this->Disconnect(wxEVT_MIDDLE_DCLICK, wxMouseEventHandler(Workspace::OnMiddleDoubleClick), NULL, this);

	// Recreate the GLCanvas. This is necessary to prevent WorkspaceBase destructor access nullpr.
	// Also avoid the code editing of WorkspaceBase, since is automatically generated by wxCrafter.
	// TODO(?): Find a better way to solve this problem.
	m_glCanvas = new wxGLCanvas(this);
}

void WorkspaceDC::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
	dc.Clear();
	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

	
	// Draw
	if (gc) {

		gc->Scale(m_camera->GetScale(), m_camera->GetScale());
		gc->Translate(m_camera->GetTranslation().m_x, m_camera->GetTranslation().m_y);

		// Elements
		for (auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
			Element* element = *it;
			element->DrawDC(m_camera->GetTranslation(), m_camera->GetScale(), gc);
		}

		// Texts
		for (auto it = m_textList.begin(); it != m_textList.end(); ++it) {
			Text* text = *it;
			text->DrawDC(m_camera->GetTranslation(), m_camera->GetScale(), gc);
		}

		// Selection rectangle
		//dc.SetPen(wxPen(wxColour(0, 125, 255)));
		gc->SetPen(wxPen(wxColour(0, 125, 255, 255)));
		gc->SetBrush(wxBrush(wxColour(0, 125, 255, 125)));
		//dc.SetBrush(wxBrush(wxColour(0, 125, 255, 125)));
		gc->DrawRectangle(m_selectionRect.m_x, m_selectionRect.m_y, m_selectionRect.m_width, m_selectionRect.m_height);

		delete gc;
	}
	event.Skip();
}
