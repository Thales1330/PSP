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

#include "HMPlane.h"

WorkspaceDC::WorkspaceDC()
{
}

WorkspaceDC::WorkspaceDC(wxWindow* parent, wxString name, wxStatusBar* statusBar) : Workspace(parent, name, statusBar, nullptr)
{
	// Disconnect events from GLCanvas
	m_glCanvas->Disconnect(wxEVT_PAINT, wxPaintEventHandler(Workspace::OnPaint), nullptr, this);
	m_glCanvas->Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(Workspace::OnLeftClickDown), nullptr, this);
	m_glCanvas->Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(Workspace::OnKeyDown), nullptr, this);
	m_glCanvas->Disconnect(wxEVT_MOTION, wxMouseEventHandler(Workspace::OnMouseMotion), nullptr, this);
	m_glCanvas->Disconnect(wxEVT_MIDDLE_DOWN, wxMouseEventHandler(Workspace::OnMiddleDown), nullptr, this);
	m_glCanvas->Disconnect(wxEVT_MIDDLE_UP, wxMouseEventHandler(Workspace::OnMiddleUp), nullptr, this);
	m_glCanvas->Disconnect(wxEVT_LEFT_UP, wxMouseEventHandler(Workspace::OnLeftClickUp), nullptr, this);
	m_glCanvas->Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(Workspace::OnScroll), nullptr, this);
	m_glCanvas->Disconnect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(Workspace::OnRightClickDown), nullptr, this);
	m_glCanvas->Disconnect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(Workspace::OnLeftDoubleClick), nullptr, this);
	m_glCanvas->Disconnect(wxEVT_IDLE, wxIdleEventHandler(Workspace::OnIdle), nullptr, this);
	m_glCanvas->Disconnect(wxEVT_MIDDLE_DCLICK, wxMouseEventHandler(Workspace::OnMiddleDoubleClick), nullptr, this);
	m_glCanvas->Disconnect(wxEVT_SIZE, wxSizeEventHandler(Workspace::OnResize), nullptr, this);

	// Reconnect events to this
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(WorkspaceDC::OnPaint), nullptr, this); // Connect to overloaded method
	this->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(Workspace::OnLeftClickDown), nullptr, this);
	this->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(Workspace::OnKeyDown), nullptr, this);
	this->Connect(wxEVT_MOTION, wxMouseEventHandler(Workspace::OnMouseMotion), nullptr, this);
	this->Connect(wxEVT_MIDDLE_DOWN, wxMouseEventHandler(Workspace::OnMiddleDown), nullptr, this);
	this->Connect(wxEVT_MIDDLE_UP, wxMouseEventHandler(Workspace::OnMiddleUp), nullptr, this);
	this->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(Workspace::OnLeftClickUp), nullptr, this);
	this->Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(Workspace::OnScroll), nullptr, this);
	this->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(Workspace::OnRightClickDown), nullptr, this);
	this->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(Workspace::OnLeftDoubleClick), nullptr, this);
	this->Connect(wxEVT_IDLE, wxIdleEventHandler(WorkspaceDC::OnIdle), nullptr, this);  // Connect to overloaded method
	this->Connect(wxEVT_MIDDLE_DCLICK, wxMouseEventHandler(Workspace::OnMiddleDoubleClick), nullptr, this);
	this->Connect(wxEVT_SIZE, wxSizeEventHandler(Workspace::OnResize), nullptr, this);

	this->GetSizer()->Remove(this->GetSizer()->GetChildren()[0]->GetId()); // remove m_glCanvas object from sizer

	delete m_glCanvas; // Delete GLcanvas to allow drawing with wxDC
	m_glCanvas = nullptr;
	m_glContext = nullptr;
	SetBackgroundColour(wxColour(255, 255, 255));
	SetBackgroundStyle(wxBG_STYLE_PAINT); // To allow wxBufferedPaintDC works properly.

	//float limits[2] = { 1.05, 0.95 };
	//m_hmPlane = new HMPlane(m_width, m_height, limits);

	Redraw();
}

WorkspaceDC::~WorkspaceDC()
{
	// Disconnect events
	this->Disconnect(wxEVT_PAINT, wxPaintEventHandler(WorkspaceDC::OnPaint), nullptr, this);
	this->Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(Workspace::OnLeftClickDown), nullptr, this);
	this->Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(Workspace::OnKeyDown), nullptr, this);
	this->Disconnect(wxEVT_MOTION, wxMouseEventHandler(Workspace::OnMouseMotion), nullptr, this);
	this->Disconnect(wxEVT_MIDDLE_DOWN, wxMouseEventHandler(Workspace::OnMiddleDown), nullptr, this);
	this->Disconnect(wxEVT_MIDDLE_UP, wxMouseEventHandler(Workspace::OnMiddleUp), nullptr, this);
	this->Disconnect(wxEVT_LEFT_UP, wxMouseEventHandler(Workspace::OnLeftClickUp), nullptr, this);
	this->Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(Workspace::OnScroll), nullptr, this);
	this->Disconnect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(Workspace::OnRightClickDown), nullptr, this);
	this->Disconnect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(Workspace::OnLeftDoubleClick), nullptr, this);
	this->Disconnect(wxEVT_IDLE, wxIdleEventHandler(WorkspaceDC::OnIdle), nullptr, this);
	this->Disconnect(wxEVT_MIDDLE_DCLICK, wxMouseEventHandler(Workspace::OnMiddleDoubleClick), nullptr, this);

	// Recreate the GLCanvas. This is necessary to prevent WorkspaceBase destructor access nullpr.
	// Also avoid the code editing of WorkspaceBase, since is automatically generated by wxCrafter.
	// TODO(?): Find a better way to solve this problem.
	m_glCanvas = new wxGLCanvas(this);
}

wxColor WorkspaceDC::MixColors(const wxColor& color1, const wxColor& color2, double factor)
{
	unsigned char red = static_cast<unsigned char>(color1.Red() + factor * (color2.Red() - color1.Red()));
	unsigned char green = static_cast<unsigned char>(color1.Green() + factor * (color2.Green() - color1.Green()));
	unsigned char blue = static_cast<unsigned char>(color1.Blue() + factor * (color2.Blue() - color1.Blue()));
	return wxColor(red, green, blue);
}

void WorkspaceDC::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
	dc.Clear();
	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);


	// Draw
	if (gc) {

		// HMPlane
		if (m_hmPlane && m_showHM) {
			m_hmPlane->DrawDC(gc);
		}

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
		gc->SetPen(wxPen(wxColour(0, 125, 255, 255)));
		gc->SetBrush(wxBrush(wxColour(0, 125, 255, 125)));
		gc->DrawRectangle(m_selectionRect.m_x, m_selectionRect.m_y, m_selectionRect.m_width, m_selectionRect.m_height);

		if (m_hmPlane && m_showHM) {
			m_hmPlane->DrawLabelDC(gc);
		}
		//const wxColor UPPER_LEFT(255, 0, 0, 125); // Red
		//const wxColor UPPER_RIGHT(255, 255, 0, 125); // Yellow
		//const wxColor LOWER_LEFT(0, 0, 255, 125); // Blue
		//const wxColor LOWER_RIGHT(0, 255, 255, 125); // Cyan

		//for (int y = m_selectionRect.GetTop(); y <= m_selectionRect.GetBottom(); ++y) {
		//	wxColor leftColor = MixColors(UPPER_LEFT, LOWER_LEFT, static_cast<double>(y - m_selectionRect.GetTop()) / static_cast<double>(m_selectionRect.GetSize().GetHeight()));
		//	wxColor rightColor = MixColors(UPPER_RIGHT, LOWER_RIGHT, static_cast<double>(y - m_selectionRect.GetTop()) / static_cast<double>(m_selectionRect.GetSize().GetHeight()));
		//
		//	gc->SetBrush(gc->CreateLinearGradientBrush(m_selectionRect.GetLeft(), y, m_selectionRect.GetSize().GetWidth(), 1, leftColor, rightColor));
		//	gc->DrawRectangle(m_selectionRect.GetLeft(), y, m_selectionRect.GetSize().GetWidth(), 1);
		//}	

		//wxGraphicsBrush brush = gc->CreateLinearGradientBrush(m_selectionRect.GetLeftTop().m_x, m_selectionRect.GetLeftTop().m_y, m_selectionRect.GetRightBottom().m_x, m_selectionRect.GetRightBottom().m_y, gStops);
		//gc->SetBrush(brush);
		//gc->DrawRectangle(m_selectionRect.m_x, m_selectionRect.m_y, m_selectionRect.m_width, m_selectionRect.m_height);

		// Apply this concept to each line and compare with the mesh calculated
		//gc->SetPen(*wxTRANSPARENT_PEN);
		//wxGraphicsGradientStops gStops;
		//gStops.Add(UPPER_LEFT, 0.0);
		//gStops.Add(UPPER_RIGHT, 0.3);
		//gStops.Add(LOWER_LEFT, 0.7);
		//gStops.Add(LOWER_RIGHT, 1.0);
		//for (int y = m_selectionRect.GetTop(); y <= m_selectionRect.GetBottom(); ++y) {
		//	wxGraphicsBrush brush = gc->CreateLinearGradientBrush(m_selectionRect.GetLeft(), y, m_selectionRect.GetRight(), y, gStops);
		//	gc->SetBrush(brush);
		//	gc->DrawRectangle(m_selectionRect.GetLeft(), y, m_selectionRect.GetSize().GetWidth(), 1);
		//}

		delete gc;
	}
	event.Skip();
}

void WorkspaceDC::OnIdle(wxIdleEvent& event)
{
	if (m_justOpened) {
		m_justOpened = false;
		float limits[2] = { 1.05, 0.95 };
		m_hmPlane = new HMPlane(m_width, m_height, limits);

		Redraw();
	}
}

void WorkspaceDC::OnResize(wxSizeEvent& event)
{
	m_width = static_cast<float>(GetSize().x) - 1.0f;
	m_height = static_cast<float>(GetSize().y) - 1.0f;

	if (m_hmPlane && m_showHM) {
		m_hmPlane->ResizeDC(m_width, m_height);
		m_showHMTimer = true;
		m_timerHeatMap->Start();
	}

	event.Skip();
}
