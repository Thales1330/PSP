/*
 *  Copyright (C) 2017  Thales Lima Oliveira <thales@ufu.br>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

 //#define SHOW_DEBUG_PANEL;
 //#define SHOW_SIMULATION_PARAMETERS;

#include "Workspace.h"

#include "ChartView.h"

#include "../elements/Element.h"
#include "../elements/ElementDataObject.h"
#include "../elements/Text.h"
#include "../elements/powerElement/Capacitor.h"
#include "../elements/powerElement/HarmCurrent.h"
#include "../elements/powerElement/IndMotor.h"
#include "../elements/powerElement/Inductor.h"
#include "../elements/powerElement/Line.h"
#include "../elements/powerElement/Load.h"
#include "../elements/powerElement/SyncGenerator.h"
#include "../elements/powerElement/SyncMotor.h"
#include "../elements/powerElement/Transformer.h"
#include "../elements/powerElement/EMTElement.h"

#include "../simulation/Electromechanical.h"
#include "../simulation/Fault.h"
#include "../simulation/PowerFlow.h"
#include "../simulation/PowerQuality.h"

#include "../utils/ElementPlotData.h"

#include "../forms/FrequencyResponseForm.h"

#include "../utils/Camera.h"
#include "../utils/PropertiesData.h"
#include "../utils/HMPlane.h"
#include "../utils/FileHanding.h"
#include "../utils/Path.h"

#include <wx/busyinfo.h>
#include <wx/dcsvg.h>

#ifdef __WXMSW__
#include <windows.h>

namespace
{
	DWORD GetWorkspaceGDIObjects()
	{
		return GetGuiResources(GetCurrentProcess(), GR_GDIOBJECTS);
	}

	void LogWorkspaceGDIDelta(const wxString& tag, DWORD before)
	{
		const DWORD after = GetWorkspaceGDIObjects();
		if (after != before) {
			wxLogDebug("%s: GDI %lu -> %lu (%+ld)",
				tag,
				before,
				after,
				static_cast<long>(after) - static_cast<long>(before));
		}
	}
}
#endif

// Workspace
Workspace::Workspace() : WorkspaceBase(nullptr)
{
#ifdef _DEBUG
#ifdef SHOW_DEBUG_PANEL
	m_debugFrame = new DebugMainFrame(this);
	m_debugFrame->SetTitle(_("Debug window: ") + m_name);
	m_debugFrame->Show();
	//m_debugFrame->SetSize(m_debugFrame->GetBestVirtualSize());
#endif
#endif
	SetColourTheme();
	m_workspacePanel->SetBackgroundStyle(wxBG_STYLE_PAINT); // To allow wxBufferedPaintDC works properly.
}

Workspace::Workspace(wxWindow* parent, wxString name, wxStatusBar* statusBar, wxAuiNotebook* auiNotebook)
	: WorkspaceBase(parent)
{
#ifdef _DEBUG
#ifdef SHOW_DEBUG_PANEL
	m_debugFrame = new DebugMainFrame(this);
	m_debugFrame->SetTitle(_("Debug window: ") + m_name);
	m_debugFrame->Show();
	//m_debugFrame->SetSize(m_debugFrame->GetBestVirtualSize());
#endif
#endif

	m_timer->Start();
	m_name = name;
	m_statusBar = statusBar;
	m_auiNotebook = auiNotebook;
	//m_glContext = new wxGLContext(m_glCanvas, sharedGLContext);
	m_camera = new Camera();
	m_selectionRect = wxRect2DDouble(0, 0, 0, 0);

	for (int i = 0; i < NUM_ELEMENTS; ++i) { m_elementNumber[i] = 1; }

	const int widths[4] = { -3, -1, 100, 100 };
	m_statusBar->SetStatusWidths(4, widths);

	m_properties = new PropertiesData();

	//m_glCanvas->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	SetColourTheme();
	m_workspacePanel->SetBackgroundStyle(wxBG_STYLE_PAINT); // To allow wxBufferedPaintDC works properly.

	//m_width = static_cast<float>(m_glCanvas->GetSize().x) - 1.0;
	//m_height = static_cast<float>(m_glCanvas->GetSize().y) - 1.0;
	m_width = static_cast<float>(m_workspacePanel->GetSize().x) - 1.0;
	m_height = static_cast<float>(m_workspacePanel->GetSize().y) - 1.0;
	//m_renderer = new Renderer();

	m_CEMngr = std::make_unique<ControlEditorManager>(this);

	SaveCurrentState();
}

Workspace::~Workspace()
{
	if (m_hmPlane) delete m_hmPlane;
	if (m_camera) delete m_camera;
	if (m_properties) delete m_properties;
}

void Workspace::SetColourTheme()
{
	m_properties->SetGUIColourTheme();
	m_workspacePanel->SetBackgroundColour(m_properties->GetGUIColour()->background);
}

void Workspace::OnPaint(wxPaintEvent& event)
{
	//#ifdef __WXMSW__
	//	const DWORD beforePaint = GetWorkspaceGDIObjects();
	//#endif
		//{
	wxAutoBufferedPaintDC dc(m_workspacePanel);
	dc.Clear();
	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

	// Draw
	DrawScene(gc);
	delete gc;
	//}
//#ifdef __WXMSW__
//	LogWorkspaceGDIDelta("Workspace::OnPaint", beforePaint);
//#endif

	event.Skip();
}

void Workspace::DrawScene(wxGraphicsContext* gc)
{
	// Draw
	if (gc) {

		// HMPlane
		if (m_hmPlane && m_showHM) {
			//#ifdef __WXMSW__
			//			const DWORD beforeHMPlane = GetWorkspaceGDIObjects();
			//#endif
			m_hmPlane->DrawDC(gc);
			//#ifdef __WXMSW__
			//			LogWorkspaceGDIDelta("Workspace::DrawScene HMPlane", beforeHMPlane);
			//#endif
		}

		gc->Scale(m_camera->GetScale(), m_camera->GetScale());
		gc->Translate(m_camera->GetTranslation().m_x, m_camera->GetTranslation().m_y);

		// Elements
		for (auto& element : m_elementList) {
			//#ifdef __WXMSW__
			//			const DWORD beforeElement = GetWorkspaceGDIObjects();
			//#endif
			element->DrawDC(m_properties->GetGUIColour(), m_camera->GetTranslation(), m_camera->GetScale(), gc);
			//#ifdef __WXMSW__
			//			LogWorkspaceGDIDelta(wxString::Format("Workspace::DrawScene element type %d", element->GetElementType()), beforeElement);
			//#endif
		}

		// Dummy Text to set correct context
		// TODO: Find a better way to do this.
//#ifdef __WXMSW__
//		const DWORD beforeDummyText = GetWorkspaceGDIObjects();
//#endif
		Text* text = new Text(wxPoint2DDouble(0.0, 0.0), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		text->SetText("");
		text->DrawDC(m_properties->GetGUIColour(), m_camera->GetTranslation(), m_camera->GetScale(), gc);
		delete text;
		//#ifdef __WXMSW__
		//		LogWorkspaceGDIDelta("Workspace::DrawScene dummy text", beforeDummyText);
		//#endif

				// Texts
		for (auto& text : m_textList) {
			//#ifdef __WXMSW__
			//			const DWORD beforeText = GetWorkspaceGDIObjects();
			//#endif
			text->DrawDC(m_properties->GetGUIColour(), m_camera->GetTranslation(), m_camera->GetScale(), gc);
			//#ifdef __WXMSW__
			//			LogWorkspaceGDIDelta("Workspace::DrawScene text", beforeText);
			//#endif
		}

		// Selection rectangle
//#ifdef __WXMSW__
//		const DWORD beforeSelectionRect = GetWorkspaceGDIObjects();
//#endif
		wxColour selectionColour = m_properties->GetGUIColour()->selection;
		gc->SetPen(wxPen(selectionColour));
		gc->SetBrush(wxBrush(selectionColour));
		gc->DrawRectangle(m_selectionRect.m_x, m_selectionRect.m_y, m_selectionRect.m_width, m_selectionRect.m_height);
		//#ifdef __WXMSW__
		//		LogWorkspaceGDIDelta("Workspace::DrawScene selection rectangle", beforeSelectionRect);
		//#endif

		if (m_hmPlane && m_showHM) {
			//#ifdef __WXMSW__
			//			const DWORD beforeHMLabel = GetWorkspaceGDIObjects();
			//#endif
			m_hmPlane->DrawLabelDC(gc);
			//#ifdef __WXMSW__
			//			LogWorkspaceGDIDelta("Workspace::DrawScene heatmap label", beforeHMLabel);
			//#endif
		}
	}
}

void Workspace::DrawScene(wxDC& dc)
{
	// HMPlane
	if (m_hmPlane && m_showHM) {
		m_hmPlane->DrawDC(dc);
	}

	dc.SetUserScale(m_camera->GetScale(), m_camera->GetScale());
	dc.SetDeviceOrigin(m_camera->GetTranslation().m_x * m_camera->GetScale(), m_camera->GetTranslation().m_y * m_camera->GetScale());
	//dc.SetLogicalOrigin(-m_camera->GetTranslation().m_x, -m_camera->GetTranslation().m_y);

	// Elements
	for (auto& element : m_elementList) {
		element->DrawDC(m_properties->GetGUIColour(), m_camera->GetTranslation(), m_camera->GetScale(), dc);
	}

	// Dummy Text to set correct context
	// TODO: Find a better way to do this.
	Text* text = new Text(wxPoint2DDouble(0.0, 0.0), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
	text->SetText("");
	text->DrawDC(m_properties->GetGUIColour(), m_camera->GetTranslation(), m_camera->GetScale(), dc);
	delete text;

	// Texts
	for (auto& text : m_textList) {
		text->DrawDC(m_properties->GetGUIColour(), m_camera->GetTranslation(), m_camera->GetScale(), dc);
	}

	if (m_hmPlane && m_showHM) {
		m_hmPlane->DrawLabelDC(dc);
	}
}

void Workspace::CopyToClipboard()
{
	wxSize size = GetClientSize();

	int scale = 2;

	wxBitmap bitmap(size.x * scale, size.y * scale);
	wxMemoryDC memDC(bitmap);

	memDC.SetBackground(*wxWHITE_BRUSH);
	memDC.Clear();

	wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
	if (!gc) return;
	gc->Scale(scale, scale); // Increase the scale to improve the quality of the copied image.

	DrawScene(gc);
	delete gc;

	memDC.SelectObject(wxNullBitmap);

	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxBitmapDataObject(bitmap));
		wxTheClipboard->Close();
	}
}

void Workspace::ExportAsSVG(wxString path)
{
	wxSize size = GetClientSize();

	wxSVGFileDC svgDC(path, size.x, size.y);

	svgDC.SetClippingRegion(wxRect(0, 0, size.x, size.y));

	DrawScene(svgDC);
}

void Workspace::OnLeftClickDown(wxMouseEvent& event)
{
	wxWindow* dialogParent = wxGetTopLevelParent(this);
	if (!dialogParent) dialogParent = this;
	//wxWindow* dialogParent = this;

	wxPoint clickPoint = event.GetPosition();
	bool foundElement = false;
	Element* newElement = nullptr;
	bool showNewElementForm = false;
	bool clickOnSwitch = false;
	bool unselectAll = true;
	std::vector<Element*> notUnselectElementList;
	std::vector<Text*> notUnselectTextList;

	if (m_mode == WorkspaceMode::MODE_INSERT_TEXT || m_mode == WorkspaceMode::MODE_PASTE || m_mode == WorkspaceMode::MODE_DRAG_PASTE) {
		m_mode = WorkspaceMode::MODE_EDIT;
		SaveCurrentState();
	}
	else if (m_mode == WorkspaceMode::MODE_INSERT || m_mode == WorkspaceMode::MODE_DRAG_INSERT || m_mode == WorkspaceMode::MODE_DRAG_INSERT_TEXT) {
		wxPoint2DDouble clickPointWorld = m_camera->ScreenToWorld(clickPoint);

		if (!m_elementList.empty()) {
			// Get the last element inserted on the list.
			newElement = m_elementList.back().get();
			for (auto& element : m_elementList) {
				// Clicked in any element.
				if (element->Contains(clickPointWorld)) {
					// Click at a bus.
					if (auto bus = dynamic_cast<Bus*>(element.get())) {
						// Select the bus.
						bus->SetSelected();
						foundElement = true;  // Element found.
						// Add the new element's parent. If the element being inserted returns true, back to
						// edit mode.
						if (newElement->AddParent(bus, clickPointWorld)) {
							ValidateElementsVoltages();
							m_timer->Stop();
							showNewElementForm = true;
							m_mode = WorkspaceMode::MODE_EDIT;
							bus->SetInserted();
						}
					}
				}
			}

			// The line element can have an undefined number of points.
			if (!foundElement) {
				if (auto line = dynamic_cast<Line*>(newElement)) { line->AddPoint(clickPointWorld); }
			}
			foundElement = true;
			unselectAll = false;
		}
	}
	else {

		bool clickPickbox = false;

		for (auto& element : m_elementList) {
			element->ResetPickboxes();  // Reset pickbox state.

			// Set movement initial position (not necessarily will be moved).
			element->StartMove(m_camera->ScreenToWorld(clickPoint));

			// Click in selected element node.
			if (element->NodeContains(m_camera->ScreenToWorld(clickPoint)) != 0 && element->IsSelected()) {
				m_mode = WorkspaceMode::MODE_MOVE_NODE;
				m_disconnectedElement = true;
				foundElement = true;
				unselectAll = false;
				if (m_hmPlane && m_showHM) {
					m_hmPlane->Clear();
				}
			}

			// Click in an element.
			else if (element->Contains(m_camera->ScreenToWorld(clickPoint))) {
				notUnselectElementList.emplace_back(element.get());
				if (!foundElement) {
					if (element->IsSelected()) unselectAll = false;
					// Select and show pickbox.
					element->SetSelected();
					element->ShowPickbox();
					foundElement = true;
					// Select the associated text
					for (auto& text : m_textList) {
						if (text->GetElement() == element.get()) {
							notUnselectTextList.emplace_back(text.get());
							text->SetSelected();
							text->SetAllowRotation(false);
							if (unselectAll) text->SetAltSelectionColour();
						}
					}
				}
				// If pickbox contains the click, move the pickbox
				if (element->PickboxContains(m_camera->ScreenToWorld(clickPoint))) {
					m_mode = WorkspaceMode::MODE_MOVE_PICKBOX;
					clickPickbox = true;
				}
				// If didn't found a pickbox, move the element
				if (!clickPickbox) { m_mode = WorkspaceMode::MODE_MOVE_ELEMENT; }

				if (m_hmPlane && m_showHM) {
					m_hmPlane->Clear();
				}
			}

			// Click in a switch.
			else if (element->SwitchesContains(m_camera->ScreenToWorld(clickPoint))) {
				element->SetOnline(element->IsOnline() ? false : true);
				clickOnSwitch = true;
			}
		}

		// Text element
		for (auto& text : m_textList) {
			text->StartMove(m_camera->ScreenToWorld(clickPoint));

			if (text->Contains(m_camera->ScreenToWorld(clickPoint))) {
				notUnselectTextList.emplace_back(text.get());
				if (!foundElement) {
					if (text->IsSelected()) unselectAll = false;
					text->SetSelected();
					text->SetAltSelectionColour(false);
					text->SetAllowRotation();
					m_mode = WorkspaceMode::MODE_MOVE_ELEMENT;
					foundElement = true;
					if (m_hmPlane && m_showHM) {
						m_hmPlane->Clear();
					}
				}
			}
		}
	}

	// Unselect all elements
	if (!event.ControlDown() && unselectAll) {
		for (auto& element : m_elementList) {
			bool select = false;
			for (Element* notUnselectElement : notUnselectElementList) {
				if (notUnselectElement == element.get()) select = true;
			}
			element->SetSelected(select);
		}
		for (auto& text : m_textList) {
			bool select = false;
			for (auto& notUnselectText : notUnselectTextList) {
				if (notUnselectText == text.get()) select = true;
			}
			text->SetSelected(select);
		}
	}

	if (!foundElement && !clickOnSwitch) {
		m_mode = WorkspaceMode::MODE_SELECTION_RECT;
		m_startSelRect = m_camera->ScreenToWorld(clickPoint);
		if (m_hmPlane && m_showHM) {
			m_hmPlane->Clear();
		}
	}

	Redraw();
	UpdateStatusBar();

	if (showNewElementForm) {
		if (newElement) {
			newElement->ShowForm(dialogParent, newElement, this);
			// Modal dialogs can steal focus; restore it to the workspace panel to keep mouse/keyboard handling consistent.
			if (m_workspacePanel) m_workspacePanel->SetFocus();
			CheckSlackBusDuplication(newElement);
			SaveCurrentState();
			if (m_continuousCalc) RunStaticStudies();
		}
	}
	if (clickOnSwitch && m_continuousCalc) RunStaticStudies();

	event.Skip();
}

void Workspace::OnLeftDoubleClick(wxMouseEvent& event)
{
	wxWindow* dialogParent = wxGetTopLevelParent(this);
	if (!dialogParent) dialogParent = this;
	//wxWindow* dialogParent = this;

	bool elementEdited = false;
	bool clickOnSwitch = false;
	bool redraw = false;

	for (auto& element : m_elementList) {
		// Click in an element.
		if (element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
			bool elementIsBus = false;
			Bus oldBus;
			Bus* currentBus = nullptr;
			if ((currentBus = dynamic_cast<Bus*>(element.get()))) {
				elementIsBus = true;
				oldBus = *currentBus;
			}
			m_timer->Stop();
			if (element->ShowForm(dialogParent, element.get(), this)) {
				CheckSlackBusDuplication(element.get());
				SaveCurrentState();
			}
			// Modal dialogs can steal focus; restore it to the workspace panel to keep mouse/keyboard handling consistent.
			if (m_workspacePanel) m_workspacePanel->SetFocus();
			elementEdited = true;
			redraw = true;

			// If the edited element is a bus and was changed the rated voltage, this voltage must be
			// propagated through the lines
			if (elementIsBus) {
				// The voltage was changed
				if (oldBus.GetElectricalData().nominalVoltage != currentBus->GetElectricalData().nominalVoltage ||
					oldBus.GetElectricalData().nominalVoltageUnit !=
					currentBus->GetElectricalData().nominalVoltageUnit) {
					// Check if the bus has line as child.
					std::vector<Element*> childList = element->GetChildList();
					for (auto itc = childList.begin(), itcEnd = childList.end(); itc != itcEnd; ++itc) {
						Element* child = *itc;
						if (typeid(*child) == typeid(Line)) {
							wxMessageDialog msgDialog(dialogParent, _("Do you want to change the rated voltage of the path?"),
								_("Warning"), wxYES_NO | wxCENTRE | wxICON_WARNING);
							if (msgDialog.ShowModal() == wxID_YES)
								ValidateBusesVoltages(element.get());
							else {
								auto data = currentBus->GetElectricalData();
								data.nominalVoltage = oldBus.GetElectricalData().nominalVoltage;
								data.nominalVoltageUnit = oldBus.GetElectricalData().nominalVoltageUnit;
								currentBus->SetElectricalData(data);
							}
							break;
						}
					}
				}
				ValidateElementsVoltages();
			}

			// Handle only one element per double-click to avoid opening multiple modals in sequence.
			break;
		}

		// Click in a switch.
		else if (element->SwitchesContains(m_camera->ScreenToWorld(event.GetPosition()))) {
			element->SetOnline(element->IsOnline() ? false : true);
			clickOnSwitch = true;
		}
	}

	// Text element
	for (auto& text : m_textList) {
		if (text->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
			if (text->ShowForm(dialogParent, GetElementList())) SaveCurrentState();
			// Modal dialogs can steal focus; restore it to the workspace panel to keep mouse/keyboard handling consistent.
			if (m_workspacePanel) m_workspacePanel->SetFocus();
			redraw = true;
			break;
		}
	}
	if (elementEdited) {
		UpdateTextElements();
		if (m_continuousCalc) RunStaticStudies();
	}
	if (clickOnSwitch && m_continuousCalc) RunStaticStudies();

	if (redraw) Redraw();
	m_timer->Start();
}

void Workspace::OnRightClickDown(wxMouseEvent& event)
{
	bool redraw = false;
	if (m_mode == WorkspaceMode::MODE_EDIT) {
		for (auto& element : m_elementList) {
			if (element->IsSelected()) {
				// Show context menu.
				if (element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
					element->ShowPickbox(false);
					wxMenu menu;
					menu.SetClientData(element.get());
					if (element->GetContextMenu(menu)) {
						m_timer->Stop();
						menu.Bind(wxEVT_COMMAND_MENU_SELECTED, &Workspace::OnPopupClick, this);
						PopupMenu(&menu);
						// Context menus can steal focus; restore it so subsequent mouse events keep working.
						if (m_workspacePanel) m_workspacePanel->SetFocus();
						redraw = true;

						if (!menu.GetClientData()) break;
					}
					element->ResetPickboxes();
				}
			}
		}
	}
	if (redraw) Redraw();
	m_timer->Start();
}

void Workspace::OnLeftClickUp(wxMouseEvent& event)
{
	// This event (under certain conditions) deselects the elements and back to edit mode or select the elements using
	// the selection rectangle.
	bool foundPickbox = false;
	bool findNewParent = false;
	bool updateVoltages = false;
	bool saveCurrentState = false;
	auto itnp = m_elementList.begin();

	for (auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
		auto& element = *it;
		// The user was moving a pickbox.
		if (m_mode == WorkspaceMode::MODE_MOVE_PICKBOX) {
			// Catch only the element that have the pickbox shown.
			if (element->IsPickboxShown()) {
				saveCurrentState = true;
				// If the element is a bus, check if a node is outside.
				if (auto bus = dynamic_cast<Bus*>(element.get())) {
					// Get all the bus children.
					for (auto child : m_elementList) {
						for (auto parent : child->GetParentList()) {
							// The child have a parent that is the element (bus).
							if (parent == bus) {
								child->UpdateNodes();
								m_disconnectedElement = true;
							}
						}
					}
					//for (int i = 0; i < (int)m_elementList.size(); i++) {
					//	Element* child = m_elementList[i];
					//	for (int j = 0; j < (int)child->GetParentList().size(); j++) {
					//		Element* parent = child->GetParentList()[j];
					//		// The child have a parent that is the element.
					//		if (parent == element) {
					//			child->UpdateNodes();
					//			m_disconnectedElement = true;
					//		}
					//	}
					//}
				}
			}
		}

		if (m_mode == WorkspaceMode::MODE_SELECTION_RECT) {
			if (element->Intersects(m_selectionRect)) {
				element->SetSelected();
				// Select the associated text
				for (auto& text : m_textList) {
					if (text->GetElement() == element.get()) {
						text->SetSelected();
						text->SetAltSelectionColour(false);
						text->SetAllowRotation();
					}
				}
			}
			//else if (!event.ControlDown()) {
			//	element->SetSelected(false);
			//}
		}
		else if (m_mode == WorkspaceMode::MODE_MOVE_NODE) {
			if (element->IsSelected()) {
				saveCurrentState = true;
				for (auto parent : m_elementList) {
					if (auto bus = dynamic_cast<Bus*>(parent.get())) {
						if (element->SetNodeParent(bus)) {
							parent->AddChild(element.get());
							findNewParent = true;
							itnp = it;
							element->ResetNodes();
							break;
						}
					}
				}
				// element->ResetNodes();
			}
		}
		else {
			// Deselect
			//if (!event.ControlDown()) {
			//	if (!element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) { element->SetSelected(false); }
			//}

			if (element->PickboxContains(m_camera->ScreenToWorld(event.GetPosition()))) {
				foundPickbox = true;
			}
			else {
				element->ShowPickbox(false);
				element->ResetPickboxes();
			}
		}
	}

	// Text element
	for (auto& text : m_textList) {
		if (m_mode == WorkspaceMode::MODE_SELECTION_RECT) {
			if (text->Intersects(m_selectionRect)) {
				text->SetSelected();
				text->SetAltSelectionColour(false);
				text->SetAllowRotation();
			}
			//else if (!event.ControlDown()) {
			//	text->SetSelected(false);
			//}
		}
		//else if (!event.ControlDown()) {
		//	if (!text->Contains(m_camera->ScreenToWorld(event.GetPosition()))) { text->SetSelected(false); }
		//}
	}
	if (m_mode == WorkspaceMode::MODE_MOVE_ELEMENT) saveCurrentState = true;

	if (findNewParent) {
		std::rotate(itnp, itnp + 1, m_elementList.end());
		updateVoltages = true;
	}
	if (!foundPickbox) { SetCursor(wxCURSOR_ARROW); }

	if (m_mode != WorkspaceMode::MODE_INSERT) { m_mode = WorkspaceMode::MODE_EDIT; }

	if (updateVoltages) { ValidateElementsVoltages(); }

	if (saveCurrentState) SaveCurrentState();

	if (m_continuousCalc && m_disconnectedElement) {
		m_disconnectedElement = false;
		RunStaticStudies();
	}

	m_selectionRect = wxRect2DDouble(0, 0, 0, 0);

	if (m_hmPlane && m_showHM) {
		m_showHMTimer = true;
		m_timerHeatMap->Start();
	}

	Redraw();
	UpdateStatusBar();
}

void Workspace::OnMouseMotion(wxMouseEvent& event)
{
	bool redraw = false;
	switch (m_mode) {
	case WorkspaceMode::MODE_INSERT: {
		auto& newElement = m_elementList.back();  // Get the last element in the list.
		newElement->SetPosition(m_camera->ScreenToWorld(event.GetPosition()));
		redraw = true;
	} break;

	case WorkspaceMode::MODE_INSERT_TEXT: {
		auto& newText = m_textList.back();
		newText->SetPosition(m_camera->ScreenToWorld(event.GetPosition()));
		redraw = true;
	} break;

	case WorkspaceMode::MODE_DRAG:
	case WorkspaceMode::MODE_DRAG_INSERT:
	case WorkspaceMode::MODE_DRAG_INSERT_TEXT:
	case WorkspaceMode::MODE_DRAG_PASTE: {
		m_camera->SetTranslation(event.GetPosition());
		redraw = true;
	} break;

	case WorkspaceMode::MODE_EDIT: {
		bool foundPickbox = false;
		for (auto& element : m_elementList) {
			if (element->IsSelected()) {
				// Show element pickbox (when it has) if the mouse is over the selected object.
				if (element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
					element->ShowPickbox();
					redraw = true;

					// If the mouse is over a pickbox set correct mouse cursor.
					if (element->PickboxContains(m_camera->ScreenToWorld(event.GetPosition()))) {
						foundPickbox = true;
//#ifdef __WXMSW__
//						const DWORD beforeCursor = GetWorkspaceGDIObjects();
//#endif
						SetCursor(element->GetBestPickboxCursor());
//#ifdef __WXMSW__
//						LogWorkspaceGDIDelta(wxString::Format("Workspace::OnMouseMotion pickbox cursor type %d", element->GetElementType()), beforeCursor);
//#endif
					}
					else if (!foundPickbox) {
//#ifdef __WXMSW__
//						const DWORD beforeCursor = GetWorkspaceGDIObjects();
//#endif
						SetCursor(wxCURSOR_ARROW);
//#ifdef __WXMSW__
//						LogWorkspaceGDIDelta("Workspace::OnMouseMotion arrow cursor", beforeCursor);
//#endif
						element->ResetPickboxes();
					}
				}
				else if (!foundPickbox) {
					if (element->IsPickboxShown()) redraw = true;

					element->ShowPickbox(false);
					element->ResetPickboxes();
//#ifdef __WXMSW__
//					const DWORD beforeCursor = GetWorkspaceGDIObjects();
//#endif
					SetCursor(wxCURSOR_ARROW);
//#ifdef __WXMSW__
//					LogWorkspaceGDIDelta("Workspace::OnMouseMotion arrow cursor", beforeCursor);
//#endif
				}
			}
		}
	} break;

	case WorkspaceMode::MODE_MOVE_NODE: {
		for (auto& element : m_elementList) {
			if (element->IsSelected()) {
				element->MoveNode(nullptr, m_camera->ScreenToWorld(event.GetPosition()));
				redraw = true;
			}
		}
	} break;

	case WorkspaceMode::MODE_MOVE_PICKBOX: {
		for (auto& element : m_elementList) {
			if (element->IsSelected()) {
				element->MovePickbox(m_camera->ScreenToWorld(event.GetPosition()));
				redraw = true;
			}
		}
		if (m_hmPlane && m_showHM) {
			m_hmPlane->Clear();
		}
	} break;

	case WorkspaceMode::MODE_MOVE_ELEMENT:
	case WorkspaceMode::MODE_PASTE: {
		for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
			auto element = *it;
			if (element->IsSelected()) {
				element->Move(m_camera->ScreenToWorld(event.GetPosition()));
				// Move child nodes
				std::vector<Element*> childList = element->GetChildList();
				for (auto it = childList.begin(), itEnd = childList.end(); it != itEnd; ++it) {
					(*it)->MoveNode(element.get(), m_camera->ScreenToWorld(event.GetPosition()));
				}
				redraw = true;
			}
		}
		// Text element motion
		for (auto& text : m_textList) {
			if (text->IsSelected()) {
				text->Move(m_camera->ScreenToWorld(event.GetPosition()));
				redraw = true;
			}
		}
		if (m_hmPlane && m_showHM) {
			m_hmPlane->Clear();
		}
	} break;

	case WorkspaceMode::MODE_SELECTION_RECT: {
		wxPoint2DDouble currentPos = m_camera->ScreenToWorld(event.GetPosition());
		double x, y, w, h;
		if (currentPos.m_x < m_startSelRect.m_x) {
			x = currentPos.m_x;
			w = m_startSelRect.m_x - currentPos.m_x;
		}
		else {
			x = m_startSelRect.m_x;
			w = currentPos.m_x - m_startSelRect.m_x;
		}
		if (currentPos.m_y < m_startSelRect.m_y) {
			y = currentPos.m_y;
			h = m_startSelRect.m_y - currentPos.m_y;
		}
		else {
			y = m_startSelRect.m_y;
			h = currentPos.m_y - m_startSelRect.m_y;
		}

		m_selectionRect = wxRect2DDouble(x, y, w, h);
		redraw = true;
	} break;
	}

	if (redraw) {
		Redraw();
	}
	m_camera->UpdateMousePosition(event.GetPosition());
	UpdateStatusBar();
	m_timer->Start();  // Restart the timer.
	event.Skip();
}

void Workspace::OnMiddleDown(wxMouseEvent& event)
{
	// Set to drag mode.
	switch (m_mode) {
	case WorkspaceMode::MODE_INSERT: {
		m_mode = WorkspaceMode::MODE_DRAG_INSERT;
	} break;
	case WorkspaceMode::MODE_INSERT_TEXT: {
		m_mode = WorkspaceMode::MODE_DRAG_INSERT_TEXT;
	} break;
	case WorkspaceMode::MODE_PASTE: {
		m_mode = WorkspaceMode::MODE_DRAG_PASTE;
	} break;
	default: {
		m_mode = WorkspaceMode::MODE_DRAG;
	} break;
	}
	m_camera->StartTranslation(m_camera->ScreenToWorld(event.GetPosition()));
	UpdateStatusBar();

	if (m_hmPlane && m_showHM) {
		m_hmPlane->Clear();
	}

	event.Skip();
}

void Workspace::OnMiddleUp(wxMouseEvent& event)
{
	switch (m_mode) {
	case WorkspaceMode::MODE_DRAG_INSERT: {
		m_mode = WorkspaceMode::MODE_INSERT;
	} break;
	case WorkspaceMode::MODE_DRAG_INSERT_TEXT: {
		m_mode = WorkspaceMode::MODE_INSERT_TEXT;
	} break;
	case WorkspaceMode::MODE_DRAG_PASTE: {
		m_mode = WorkspaceMode::MODE_PASTE;
	} break;
	case WorkspaceMode::MODE_INSERT:
	case WorkspaceMode::MODE_INSERT_TEXT:
	case WorkspaceMode::MODE_PASTE: {
		// Does nothing.
	} break;
	default: {
		m_mode = WorkspaceMode::MODE_EDIT;
	} break;
	}
	UpdateStatusBar();

	if (m_hmPlane && m_showHM) {
		UpdateHeatMap();
		Redraw();
	}

	event.Skip();
}

void Workspace::OnScroll(wxMouseEvent& event)
{
	if (event.GetWheelRotation() > 0)
		m_camera->SetScale(event.GetPosition(), +0.05);
	else
		m_camera->SetScale(event.GetPosition(), -0.05);

	if (m_hmPlane && m_showHM) {
		m_hmPlane->Clear();
		m_showHMTimer = true;
		m_timerHeatMap->Start();
	}

	UpdateStatusBar();
	Redraw();
}

void Workspace::OnKeyDown(wxKeyEvent& event)
{
	bool insertingElement = false;
	if (m_mode == WorkspaceMode::MODE_INSERT || m_mode == WorkspaceMode::MODE_INSERT_TEXT) insertingElement = true;

	char key = event.GetUnicodeKey();
	if (key != WXK_NONE) {
		switch (key) {
		case WXK_ESCAPE:  // Cancel operations.
		{
			if (m_mode == WorkspaceMode::MODE_INSERT) {
				//Element* elementToDelete = m_elementList[m_elementList.size() - 1];
				auto elementToDelete = m_elementList.back();

				// Remove child element that has to be deleted (specially buses)
				for (auto& element : m_elementList) {
					element->RemoveChild(elementToDelete.get());
				}

				m_elementList.pop_back();  // Removes the last element being inserted.
				m_mode = WorkspaceMode::MODE_EDIT;
				Redraw();
			}
			else if (m_mode == WorkspaceMode::MODE_INSERT_TEXT) {
				m_textList.pop_back();
				m_mode = WorkspaceMode::MODE_EDIT;
				Redraw();
			}
		} break;
		case WXK_DELETE:  // Delete selected elements
		{
			DeleteSelectedElements();
		} break;
		case 'A': {
			if (!insertingElement) {
				//Text* newText = new Text(m_camera->ScreenToWorld(event.GetPosition()), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
				auto newText = std::make_shared<Text>(
					m_camera->ScreenToWorld(event.GetPosition()),
					m_properties->GetGeneralPropertiesData().labelFont,
					m_properties->GetGeneralPropertiesData().labelFontSize);
				m_textList.push_back(newText);
				m_mode = WorkspaceMode::MODE_INSERT_TEXT;
				m_statusBar->SetStatusText(_("Insert Text: Click to insert, ESC to cancel."));
				if (m_hmPlane && m_showHM) {
					m_hmPlane->Clear();
				}
				Redraw();
			}
		} break;
		case 'F': {
			if (event.GetModifiers() == wxMOD_SHIFT) { Fit(); }
		} break;
		case 'R':  // Rotate the selected elements.
		{
			RotateSelectedElements(event.GetModifiers() != wxMOD_SHIFT);
		} break;
		case 'B':  // Insert a bus.
		{
			if (!insertingElement) {
				auto newBus = std::make_shared<Bus>(m_camera->ScreenToWorld(event.GetPosition()),
					wxString::Format(_("Bus %d"), GetElementNumber(ID_BUS)));
				IncrementElementNumber(ID_BUS);
				m_elementList.push_back(newBus);
				m_mode = WorkspaceMode::MODE_INSERT;
				m_statusBar->SetStatusText(_("Insert Bus: Click to insert, ESC to cancel."));
				if (m_hmPlane && m_showHM) {
					m_hmPlane->Clear();
				}
				Redraw();
			}
		} break;
		case 'L': {
			if (!insertingElement) {
				if (!event.ControlDown() && event.ShiftDown()) {  // Insert a load.
					auto newLoad = std::make_shared<Load>(wxString::Format(_("Load %d"), GetElementNumber(ID_LOAD)));
					IncrementElementNumber(ID_LOAD);
					m_elementList.push_back(newLoad);
					m_mode = WorkspaceMode::MODE_INSERT;
					m_statusBar->SetStatusText(_("Insert Load: Click on a bus, ESC to cancel."));
				}
				else if (!event.ControlDown() && !event.ShiftDown()) {  // Insert a power line.
					auto newLine = std::make_shared<Line>(wxString::Format(_("Line %d"), GetElementNumber(ID_LINE)));
					IncrementElementNumber(ID_LINE);
					m_elementList.push_back(newLine);
					m_mode = WorkspaceMode::MODE_INSERT;
					m_statusBar->SetStatusText(_("Insert Line: Click on two buses, ESC to cancel."));
				}
				if (m_hmPlane && m_showHM) {
					m_hmPlane->Clear();
				}
				Redraw();
			}
			// Tests - Ctrl + Shift + L
		} break;
		case 'T':  // Insert a transformer.
		{
			if (!insertingElement) {
				auto newTransformer = std::make_shared<Transformer>(wxString::Format(_("Transformer %d"), GetElementNumber(ID_TRANSFORMER)));
				IncrementElementNumber(ID_TRANSFORMER);
				m_elementList.push_back(newTransformer);
				m_mode = WorkspaceMode::MODE_INSERT;
				m_statusBar->SetStatusText(_("Insert Transformer: Click on two buses, ESC to cancel."));
				if (m_hmPlane && m_showHM) {
					m_hmPlane->Clear();
				}
				Redraw();
			}
		} break;
		case 'G':  // Insert a generator.
		{
			if (!insertingElement) {
				auto newGenerator = std::make_shared<SyncGenerator>(wxString::Format(_("Generator %d"), GetElementNumber(ID_SYNCGENERATOR)));
				IncrementElementNumber(ID_SYNCGENERATOR);
				m_elementList.push_back(newGenerator);
				m_mode = WorkspaceMode::MODE_INSERT;
				m_statusBar->SetStatusText(_("Insert Generator: Click on a bus, ESC to cancel."));
				if (m_hmPlane && m_showHM) {
					m_hmPlane->Clear();
				}
				Redraw();
			}
		} break;
		case 'I': {
			if (!insertingElement) {
				if (event.GetModifiers() == wxMOD_SHIFT) {  // Insert an inductor.
					auto newInductor = std::make_shared<Inductor>(wxString::Format(_("Inductor %d"), GetElementNumber(ID_INDUCTOR)));
					IncrementElementNumber(ID_INDUCTOR);
					m_elementList.push_back(newInductor);
					m_mode = WorkspaceMode::MODE_INSERT;
					m_statusBar->SetStatusText(_("Insert Inductor: Click on a bus, ESC to cancel."));
				}
				else  // Insert an induction motor.
				{
					auto newIndMotor = std::make_shared<IndMotor>(wxString::Format(_("Induction motor %d"), GetElementNumber(ID_INDMOTOR)));
					IncrementElementNumber(ID_INDMOTOR);
					m_elementList.push_back(newIndMotor);
					m_mode = WorkspaceMode::MODE_INSERT;
					m_statusBar->SetStatusText(_("Insert Induction Motor: Click on a bus, ESC to cancel."));
				}
				if (m_hmPlane && m_showHM) {
					m_hmPlane->Clear();
				}
				Redraw();
			}
		} break;
		case 'K':  // Insert a synchronous condenser.
		{
			if (!insertingElement) {
				auto newSyncCondenser = std::make_shared<SyncMotor>(wxString::Format(_("Synchronous condenser %d"), GetElementNumber(ID_SYNCMOTOR)));
				IncrementElementNumber(ID_SYNCMOTOR);
				m_elementList.push_back(newSyncCondenser);
				m_mode = WorkspaceMode::MODE_INSERT;
				m_statusBar->SetStatusText(_("Insert Synchronous Condenser: Click on a bus, ESC to cancel."));
				if (m_hmPlane && m_showHM) {
					m_hmPlane->Clear();
				}
				Redraw();
			}
		} break;
		case 'C': {
			if (!insertingElement) {
				if (event.GetModifiers() == wxMOD_SHIFT) {  // Insert a capacitor.
					auto newCapacitor = std::make_shared<Capacitor>(wxString::Format(_("Capacitor %d"), GetElementNumber(ID_CAPACITOR)));
					IncrementElementNumber(ID_CAPACITOR);
					m_elementList.push_back(newCapacitor);
					m_mode = WorkspaceMode::MODE_INSERT;
					m_statusBar->SetStatusText(_("Insert Capacitor: Click on a bus, ESC to cancel."));
					if (m_hmPlane && m_showHM) {
						m_hmPlane->Clear();
					}
					Redraw();
				}
				else if (event.GetModifiers() == wxMOD_CONTROL) {  // Copy.
					CopySelection();
				}
			}
		} break;
		case 'H': {
			if (!insertingElement) {
				if (event.ShiftDown() && event.ControlDown()) {
					if (!m_showHM) {
						m_showHM = true;
						UpdateHeatMap();
					}
					else {
						m_showHM = false;
					}

				}
				else if (event.GetModifiers() == wxMOD_SHIFT) {  // Insert an harmonic current source.
					auto newHarmCurrent = std::make_shared<HarmCurrent>(
						wxString::Format(_("Harmonic Current %d"), GetElementNumber(ID_HARMCURRENT)));
					IncrementElementNumber(ID_HARMCURRENT);
					m_elementList.push_back(newHarmCurrent);
					m_mode = WorkspaceMode::MODE_INSERT;
					m_statusBar->SetStatusText(
						_("Insert Harmonic Current Source: Click on a bus, ESC to cancel."));
				}
				if (m_hmPlane && m_showHM) {
					m_hmPlane->Clear();
				}
				Redraw();
			}
		} break;
		case 'V': {
			if (!insertingElement) {
				if (event.GetModifiers() == wxMOD_CONTROL) { Paste(); }
			}
		} break;
		case 'S': {
			if (!insertingElement) {
				if (event.GetModifiers() == wxMOD_CONTROL) {
					// Save the workspace.
					FileHanding fileHandling(this);

					if (GetSavedPath().IsOk()) {
						fileHandling.SaveProject(GetSavedPath());
					}
					else {
						wxFileDialog saveFileDialog(this, _("Save PSP file"), "", "", "PSP files (*.psp)|*.psp",
							wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
						if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

						fileHandling.SaveProject(saveFileDialog.GetPath());
						wxFileName fileName(saveFileDialog.GetPath());
						SetName(fileName.GetName());
						if (m_auiNotebook) m_auiNotebook->SetPageText(m_auiNotebook->GetPageIndex(this), GetName());
						SetSavedPath(fileName);
					}
				}
			}
		} break;
		case 'Z': {
			if (!insertingElement) {
				if (event.ControlDown() && !event.ShiftDown()) { SetPreviousState(); }
				if (event.ControlDown() && event.ShiftDown()) { SetNextState(); }
			}
		} break;
		case 'Y': {
			if (!insertingElement) {
				if (event.GetModifiers() == wxMOD_CONTROL) { SetNextState(); }
			}
		} break;
		case 'E': {
			if (!insertingElement) {
				if (event.GetModifiers() == wxMOD_SHIFT) {

					if (!insertingElement) {
						auto newEMTElement = std::make_shared<EMTElement>(wxString::Format(_("Electromagnetic Element %d"), GetElementNumber(ID_EMTELEMENT)));
						IncrementElementNumber(ID_EMTELEMENT);
						m_elementList.push_back(newEMTElement);
						m_mode = WorkspaceMode::MODE_INSERT;
						m_statusBar->SetStatusText(_("Insert Electromagnetic Transient Element: Click on a bus, ESC to cancel."));
						if (m_hmPlane && m_showHM) {
							m_hmPlane->Clear();
						}
						Redraw();
					}
				}
			}
		} break;
#ifdef _DEBUG
#ifdef SHOW_DEBUG_PANEL
		case 'D': {
			if (event.GetModifiers() == wxMOD_CONTROL) {
				if (m_debugFrame) {
					m_debugFrame->SetFocus();
				}
			}
		} break;
#endif
#endif
		default:
			break;
		}
	}

	UpdateStatusBar();
	event.Skip();
}

void Workspace::UpdateStatusBar()
{
	switch (m_mode) {
	case WorkspaceMode::MODE_DRAG: {
		m_statusBar->SetStatusText(_("MODE: DRAG"), 1);
	} break;

	case WorkspaceMode::MODE_PASTE:
	case WorkspaceMode::MODE_DRAG_PASTE: {
		m_statusBar->SetStatusText(_("MODE: PASTE"), 1);
	} break;

	case WorkspaceMode::MODE_INSERT:
	case WorkspaceMode::MODE_INSERT_TEXT:
	case WorkspaceMode::MODE_DRAG_INSERT:
	case WorkspaceMode::MODE_DRAG_INSERT_TEXT: {
		m_statusBar->SetStatusText(_("MODE: INSERT"), 1);
	} break;

	case WorkspaceMode::MODE_MOVE_ELEMENT:
	case WorkspaceMode::MODE_MOVE_PICKBOX:
	case WorkspaceMode::MODE_MOVE_NODE:
	case WorkspaceMode::MODE_SELECTION_RECT:
	case WorkspaceMode::MODE_EDIT: {
		if (m_oldStatusMode != m_mode)
			m_statusBar->SetStatusText(wxT(""));
		m_statusBar->SetStatusText(_("MODE: EDIT"), 1);
	} break;
	}

	m_statusBar->SetStatusText(wxString::Format(_("ZOOM: %d%%"), (int)(m_camera->GetScale() * 100.0)), 2);
	m_statusBar->SetStatusText(
		wxString::Format(wxT("X: %.1f  Y: %.1f"), m_camera->GetMousePosition().m_x, m_camera->GetMousePosition().m_y),
		3);
	m_oldStatusMode = m_mode;
}

int Workspace::GetElementNumberFromList(Element* element)
{
	int elementNum = 0;
	for (auto& elementFromList : m_elementList) {
		if (element->GetElementType() == elementFromList->GetElementType()) {
			if (element == elementFromList.get()) return elementNum;
			elementNum++;
		}
	}
	return 0;
}

void Workspace::GetStateListsCopy(const std::vector< std::shared_ptr<PowerElement> >& elementsList,
	const std::vector< std::shared_ptr<Text> >& textList,
	std::vector< std::shared_ptr<PowerElement> >& elementsListCopy,
	std::vector< std::shared_ptr<Text> >& textListCopy)
{
	// Free copy lists first
	//for (auto& element : elementsListCopy) delete element;
	//for (auto& text : textListCopy) delete text;
	elementsListCopy.clear();
	textListCopy.clear();

	std::map<Element*, Element*> elementMap;

	for (auto& element : elementsList) {
//#ifdef __WXMSW__
//		const DWORD beforeElementCopy = GetWorkspaceGDIObjects();
//#endif
		PowerElement* copyElement = static_cast<PowerElement*>(element->GetCopy());
//#ifdef __WXMSW__
//		LogWorkspaceGDIDelta(wxString::Format("Workspace::GetStateListsCopy element copy type %d", element->GetElementType()), beforeElementCopy);
//#endif
		elementsListCopy.emplace_back(copyElement);
		elementMap[element.get()] = copyElement;
	}
	// Correct the parent and child pointers
	for (auto& copyElement : elementsListCopy) {
		// Parent
		int i = 0;
		for (Element* parent : copyElement->GetParentList()) {
			auto it = elementMap.find(parent);

			if (it != elementMap.end())
				copyElement->SetParent(it->second, i);

			i++;
		}
		// Child
		i = 0;
		for (Element* child : copyElement->GetChildList()) {
			auto it = elementMap.find(child);

			if (it != elementMap.end())
				copyElement->SetChild(it->second, i);

			i++;
		}
	}

	for (const auto& text : textList) {
//#ifdef __WXMSW__
//		const DWORD beforeTextCopy = GetWorkspaceGDIObjects();
//#endif
		auto copyText = static_cast<Text*>(text->GetCopy());
//#ifdef __WXMSW__
//		LogWorkspaceGDIDelta("Workspace::GetStateListsCopy text copy", beforeTextCopy);
//#endif
		// Set text the correct element associated with the text
		auto it = elementMap.find(copyText->GetElement());

		if (it != elementMap.end())
			copyText->SetElement(it->second);
		else
			copyText->SetElement(nullptr);
		textListCopy.emplace_back(copyText);
	}
}

void Workspace::UpdateHeatMap()
{
	if (m_hmPlane && m_showHM) {
		m_hmPlane->Clear();

		wxRect2DDouble screenRect(-100, -100, m_width + 200.0, m_height + 200.0);

		// Get new voltage range
		std::vector<Bus*> busList;
		float minVoltage, maxVoltage;
		if (m_hmAutomaticLabel) {
			minVoltage = 10.0f;
			maxVoltage = 0.0f;
		}
		else {
			minVoltage = m_hmPlane->GetMinLimit();
			maxVoltage = m_hmPlane->GetMaxLimit();
		}

		for (auto& element : m_elementList) {
			if (Bus* bus = dynamic_cast<Bus*>(element.get())) {
				if (m_hmAutomaticLabel) {
					const float voltage = std::abs(bus->GetElectricalData().voltage);
					if (minVoltage > voltage) minVoltage = voltage;
					if (maxVoltage < voltage) maxVoltage = voltage;
				}
				busList.push_back(bus);
			}
		}
		if (m_hmAutomaticLabel) {
			m_hmPlane->SetLabelLimits(minVoltage, maxVoltage);
		}

		for (Bus* bus : busList) {
			const float voltage = std::abs(bus->GetElectricalData().voltage);
			float depth = 2.0f * (voltage - (maxVoltage + minVoltage) / 2.0f) / (maxVoltage - minVoltage);
			if (depth < -1.0) depth = -1.0;
			if (depth > 1.0) depth = 1.0;

			wxRect2DDouble rect = bus->GetRect();
			rect = wxRect2DDouble(
				(rect.m_x - 100.0f) * m_camera->GetScale() + m_camera->GetTranslation().m_x * m_camera->GetScale(),
				(rect.m_y - 50.0f) * m_camera->GetScale() + m_camera->GetTranslation().m_y * m_camera->GetScale(),
				(rect.m_width + 200.0f) * m_camera->GetScale(),
				(rect.m_height + 100.0f) * m_camera->GetScale());

			if (screenRect.Contains(rect))
				m_hmPlane->SetRectSlope(rect, M_PI * static_cast<float>(bus->GetAngle()) / 180.0f, depth);
		}

		//m_hmPlane->UpdateCoords();

		//int iterations = std::lround(3.3 * std::pow(m_camera->GetScale(), 0.62));
		int iterations = std::lround(10 * std::pow(m_camera->GetScale(), 2));
		//int iterations = std::lround(1 * std::pow(m_camera->GetScale(), 0.2));
		//int iterations = 1;
		if (iterations < 1) iterations = 1;
		m_hmPlane->SmoothPlane(iterations);
	}
}

void Workspace::OnPopupClick(wxCommandEvent& event)
{
	wxWindow* dialogParent = wxGetTopLevelParent(this);
	if (!dialogParent) dialogParent = this;
	//wxWindow* dialogParent = this;

	bool redrawHM = false;

	wxMenu* menu = static_cast<wxMenu*>(event.GetEventObject());
	Element* element = static_cast<Element*>(menu->GetClientData());
	int eventID = event.GetId();
	switch (eventID) {
	case ID_EDIT_ELEMENT: {
		if (element->ShowForm(dialogParent, element, this)) {
			CheckSlackBusDuplication(element);
			UpdateTextElements();
			SaveCurrentState();
		}
		// Modal dialogs can steal focus; restore it to the workspace panel to keep mouse/keyboard handling consistent.
		if (m_workspacePanel) m_workspacePanel->SetFocus();
	} break;
	case ID_LINE_ADD_NODE: {
		Line* line = static_cast<Line*>(element);
		line->AddNode(m_camera->GetMousePosition());
		SaveCurrentState();
		Redraw();
	} break;
	case ID_LINE_REMOVE_NODE: {
		Line* line = static_cast<Line*>(element);
		line->RemoveNode(m_camera->GetMousePosition());
		SaveCurrentState();
		Redraw();
	} break;
	case ID_ROTATE_CLOCK: {
		element->Rotate();
		for (auto& iElement : m_elementList) {
			// Parent's element rotating...
			for (int i = 0; i < (int)iElement->GetParentList().size(); i++) {
				Element* parent = iElement->GetParentList()[i];
				if (parent == element) { iElement->RotateNode(parent); }
			}
		}
		redrawHM = true;
		SaveCurrentState();
		Redraw();
	} break;
	case ID_ROTATE_COUNTERCLOCK: {
		element->Rotate(false);
		for (auto& iElement : m_elementList) {
			// Parent's element rotating...
			for (int i = 0; i < (int)iElement->GetParentList().size(); i++) {
				Element* parent = iElement->GetParentList()[i];
				if (parent == element) { iElement->RotateNode(parent, false); }
			}
		}
		redrawHM = true;
		SaveCurrentState();
		Redraw();
	} break;
	case ID_DELETE: {
		//for (auto& iElement : m_elementList) {
		//	if (element == iElement.get()) {
		//		// Remove child/parent.
		//		std::vector<Element*> childList = element->GetChildList();
		//		for (auto& child : childList) {
		//			if (child) {
		//				child->RemoveParent(element);
		//				element->RemoveChild(child);
		//			}
		//		}
		//		std::vector<Element*> parentList = element->GetParentList();
		//		for (auto& parent : parentList) {
		//			if (parent) { parent->RemoveChild(element); }
		//		}
		//
		//		//for (auto itt = m_textList.begin(); itt != m_textList.end(); ++itt) {
		//		//	Text* text = *itt;
		//		//	if (text->GetElement() == element) {
		//		//		m_textList.erase(itt--);
		//		//		if (text) delete text;
		//		//	}
		//		//}
		//		std::erase_if(m_textList, [&](const auto& text) {
		//			return text->GetElement() == element;
		//			});
		//
		//		std::erase_if(m_elementList, [&](const auto& delElement) {
		//			return delElement.get() == element;
		//			});
		//
		//		//m_elementList.erase(it);
		//		//if (element) delete element;
		//		menu->SetClientData(nullptr);
		//		break;
		//	}
		//}
		//SaveCurrentState();

		// Remove child/parent.
		std::vector<Element*> childList = element->GetChildList();
		for (auto child : childList) {
			if (child) {
				child->RemoveParent(element);
				element->RemoveChild(child);
			}
		}

		std::vector<Element*> parentList = element->GetParentList();
		for (auto parent : parentList) {
			if (parent)
				parent->RemoveChild(element);
		}

		std::erase_if(m_textList, [&](const auto& text) {
			return text->GetElement() == element;
			});

		std::erase_if(m_elementList, [&](const auto& delElement) {
			return delElement.get() == element;
			});

		menu->SetClientData(nullptr);
		SaveCurrentState();
	} break;
	default:
		if (InsertTextElement(eventID, element)) {
			UpdateTextElements();
			SaveCurrentState();
		}
		break;
	}
	if (redrawHM && m_hmPlane && m_showHM) {
		m_hmPlane->Clear();
		m_showHMTimer = true;
		m_timerHeatMap->Start();
	}
}

void Workspace::RotateSelectedElements(bool clockwise)
{
	bool saveCurrrentState = false;
	for (auto& element : m_elementList) {
		// Parent's element rotating...
		for (int i = 0; i < (int)element->GetParentList().size(); i++) {
			Element* parent = element->GetParentList()[i];
			if (parent) {  // Check if parent is not null
				if (parent->IsSelected()) {
					element->RotateNode(parent, clockwise);
					// Update the positions used on motion action, the element will not be necessarily
					// moved.
					element->StartMove(m_camera->GetMousePosition());
				}
			}
		}
		if (element->IsSelected()) {
			saveCurrrentState = true;
			element->Rotate(clockwise);
			element->StartMove(m_camera->GetMousePosition());
		}
	}

	// Rotate text element
	for (auto& text : m_textList) {
		if (text->IsSelected()) {
			saveCurrrentState = true;
			text->Rotate(clockwise);
			text->StartMove(m_camera->GetMousePosition());
		}
	}
	if (saveCurrrentState) SaveCurrentState();

	if (m_hmPlane && m_showHM) {
		m_hmPlane->Clear();
		m_showHMTimer = true;
		m_timerHeatMap->Start();
	}
	Redraw();
}

void Workspace::DeleteSelectedElements()
{
	// Don't set the end of the list at the loop's begin.
	for (auto it = m_elementList.begin(); it != m_elementList.end();) {
		Element* element = it->get();

		if (element->IsSelected()) {
			// Remove child/parent.
			std::vector<Element*> childList = element->GetChildList();
			for (auto itc = childList.begin(), itEnd = childList.end(); itc != itEnd; ++itc) {
				Element* child = *itc;
				if (child) {
					child->RemoveParent(element);
					element->RemoveChild(child);
				}
			}
			std::vector<Element*> parentList = element->GetParentList();
			for (auto itp = parentList.begin(), itEnd = parentList.end(); itp != itEnd; ++itp) {
				Element* parent = *itp;
				if (parent) { parent->RemoveChild(element); }
			}

			//for (auto& text : m_textList) {
			//	if (text->GetElement() == element) {
			//		if (m_textList.size() == 1) {
			//			m_textList.erase(itt);
			//			break;
			//		}
			//		else  m_textList.erase(itt--);
			//		if (text) delete text;
			//	}
			//}
			std::erase_if(m_textList, [&](const auto& text) {
				return text->GetElement() == element;
				});

			it = m_elementList.erase(it);
			//if (element) delete element;
		}
		else it++;
	}

	//for (auto it = m_textList.begin(); it != m_textList.end(); ++it) {
	//	Text* text = *it;
	//	if (text->IsSelected()) {
	//		if (m_textList.size() == 1) {
	//			m_textList.erase(it);
	//			break;
	//		}
	//		else  m_textList.erase(it--);
	//		if (text) delete text;
	//	}
	//}
	std::erase_if(m_textList, [](const auto& text) {
		return text->IsSelected();
		});

	if (m_hmPlane && m_showHM) {
		m_hmPlane->Clear();
		m_showHMTimer = true;
		m_timerHeatMap->Start();
	}
	SaveCurrentState();
	Redraw();
}

bool Workspace::GetElementsCorners(wxPoint2DDouble& leftUpCorner,
	wxPoint2DDouble& rightDownCorner,
	std::vector<Element*> elementList)
{
	if (elementList.size() == 0) return false;

	elementList[0]->CalculateBoundaries(leftUpCorner, rightDownCorner);

	for (auto it = elementList.begin() + 1, itEnd = elementList.end(); it != itEnd; it++) {
		Element* element = *it;
		wxPoint2DDouble leftUp;
		wxPoint2DDouble rightDown;
		element->CalculateBoundaries(leftUp, rightDown);
		if (leftUp.m_x < leftUpCorner.m_x) leftUpCorner.m_x = leftUp.m_x;
		if (leftUp.m_y < leftUpCorner.m_y) leftUpCorner.m_y = leftUp.m_y;
		if (rightDown.m_x > rightDownCorner.m_x) rightDownCorner.m_x = rightDown.m_x;
		if (rightDown.m_y > rightDownCorner.m_y) rightDownCorner.m_y = rightDown.m_y;
	}
	return true;
}

void Workspace::Fit()
{
	wxPoint2DDouble leftUpCorner(0, 0);
	wxPoint2DDouble rightDownCorner(0, 0);
	std::vector<Element*> elementList = GetElementList();
	for (const auto& text : m_textList) { elementList.push_back(text.get()); }

	if (!GetElementsCorners(leftUpCorner, rightDownCorner, elementList)) return;
	wxPoint2DDouble middleCoords = (leftUpCorner + rightDownCorner) / 2.0;

	int width = 0.0;
	int height = 0.0;
	GetSize(&width, &height);

	const double scaleX = static_cast<double>(width) / (rightDownCorner.m_x - leftUpCorner.m_x);
	const double scaleY = static_cast<double>(height) / (rightDownCorner.m_y - leftUpCorner.m_y);

	double scale = scaleX < scaleY ? scaleX : scaleY;
	scale = std::min(scale, m_camera->GetZoomMax());
	scale = std::max(scale, m_camera->GetZoomMin());

	m_camera->SetScale(scale);

	m_camera->StartTranslation(middleCoords);
	m_camera->SetTranslation(wxPoint2DDouble(width / 2.0, height / 2.0));

	if (m_hmPlane && m_showHM) {
		UpdateHeatMap();
	}

	Redraw();
}

bool Workspace::InsertTextElement(int textID, Element* parentElement, ElectricalUnit unit, int precision)
{
	switch (textID) {
	case ID_TXT_NAME: {
		if (FindTextElement(parentElement, DATA_NAME)) return false; // Avoid inserting more than one text element of the same type for an element.
		//Text* newText = new Text(parentElement->GetPosition() + wxPoint2DDouble(40, -30), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		auto newText = std::make_shared<Text>(parentElement->GetPosition() + wxPoint2DDouble(40, -30), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		newText->SetElement(parentElement);
		newText->SetDataType(DATA_NAME);
		newText->SetElementTypeText(parentElement->GetElementType());
		newText->SetElementNumber(GetElementNumberFromList(parentElement));

		m_textList.push_back(newText);
	} break;
	case ID_TXT_VOLTAGE: {
		if (FindTextElement(parentElement, DATA_VOLTAGE)) return false;
		//Text* newText = new Text(parentElement->GetPosition() + wxPoint2DDouble(40, 15), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		auto newText = std::make_shared<Text>(parentElement->GetPosition() + wxPoint2DDouble(40, 15), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		newText->SetElement(parentElement);
		newText->SetDataType(DATA_VOLTAGE);
		if (unit == ElectricalUnit::UNIT_NONE)
			newText->SetUnit(ElectricalUnit::UNIT_PU);
		else
			newText->SetUnit(unit);
		newText->SetDecimalPlaces(precision);
		newText->SetElementTypeText(parentElement->GetElementType());
		newText->SetElementNumber(GetElementNumberFromList(parentElement));

		m_textList.emplace_back(newText);
	} break;
	case ID_TXT_ANGLE: {
		if (FindTextElement(parentElement, DATA_ANGLE)) return false;
		//Text* newText = new Text(parentElement->GetPosition() + wxPoint2DDouble(40, 30), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		auto newText = std::make_shared<Text>(parentElement->GetPosition() + wxPoint2DDouble(40, 30), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		newText->SetElement(parentElement);
		newText->SetDataType(DATA_ANGLE);
		if (unit == ElectricalUnit::UNIT_NONE)
			newText->SetUnit(ElectricalUnit::UNIT_DEGREE);
		else
			newText->SetUnit(unit);
		newText->SetDecimalPlaces(precision);
		newText->SetElementTypeText(parentElement->GetElementType());
		newText->SetElementNumber(GetElementNumberFromList(parentElement));

		m_textList.emplace_back(newText);
	} break;
	case ID_TXT_FAULTCURRENT: {
		if (FindTextElement(parentElement, DATA_SC_CURRENT)) return false;
		//Text* newText = new Text(parentElement->GetPosition() + wxPoint2DDouble(-70, 30), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		auto newText = std::make_shared<Text>(parentElement->GetPosition() + wxPoint2DDouble(-70, 30), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		newText->SetElement(parentElement);
		newText->SetDataType(DATA_SC_CURRENT);
		if (unit == ElectricalUnit::UNIT_NONE)
			newText->SetUnit(ElectricalUnit::UNIT_A);
		else
			newText->SetUnit(unit);
		newText->SetDecimalPlaces(precision);
		newText->SetElementTypeText(parentElement->GetElementType());
		newText->SetElementNumber(GetElementNumberFromList(parentElement));

		m_textList.emplace_back(newText);
	} break;
	case ID_TXT_FAULTVOLTAGE: {
		if (FindTextElement(parentElement, DATA_SC_VOLTAGE)) return false;
		//Text* newText = new Text(parentElement->GetPosition() + wxPoint2DDouble(-70, 75), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		auto newText = std::make_shared<Text>(parentElement->GetPosition() + wxPoint2DDouble(-70, 75), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		newText->SetElement(parentElement);
		newText->SetDataType(DATA_SC_VOLTAGE);
		if (unit == ElectricalUnit::UNIT_NONE)
			newText->SetUnit(ElectricalUnit::UNIT_PU);
		else
			newText->SetUnit(unit);
		newText->SetDecimalPlaces(precision);
		newText->SetElementTypeText(parentElement->GetElementType());
		newText->SetElementNumber(GetElementNumberFromList(parentElement));

		m_textList.emplace_back(newText);
	} break;
	case ID_TXT_SCC: {
		if (FindTextElement(parentElement, DATA_SC_POWER)) return false;
		auto newText = std::make_shared<Text>(parentElement->GetPosition() + wxPoint2DDouble(-50, -30), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		newText->SetElement(parentElement);
		newText->SetDataType(DATA_SC_POWER);
		newText->SetUnit(ElectricalUnit::UNIT_MVA);
		if (unit == ElectricalUnit::UNIT_NONE)
			newText->SetUnit(ElectricalUnit::UNIT_MVA);
		else
			newText->SetUnit(unit);
		newText->SetDecimalPlaces(precision);
		newText->SetElementTypeText(parentElement->GetElementType());
		newText->SetElementNumber(GetElementNumberFromList(parentElement));

		m_textList.emplace_back(newText);
	} break;
	case ID_TXT_THD: {
		if (FindTextElement(parentElement, DATA_PQ_THD)) return false;
		auto newText = std::make_shared<Text>(parentElement->GetPosition() + wxPoint2DDouble(-50, -15), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		newText->SetElement(parentElement);
		newText->SetDataType(DATA_PQ_THD);
		newText->SetDecimalPlaces(precision);
		newText->SetElementTypeText(parentElement->GetElementType());
		newText->SetElementNumber(GetElementNumberFromList(parentElement));

		m_textList.emplace_back(newText);
	} break;
	case ID_TXT_ACTIVE_POWER: {
		if (FindTextElement(parentElement, DATA_ACTIVE_POWER)) return false;
		auto newText = std::make_shared<Text>(parentElement->GetPosition() + wxPoint2DDouble(0, 35), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		newText->SetElement(parentElement);
		newText->SetDataType(DATA_ACTIVE_POWER);
		if (unit == ElectricalUnit::UNIT_NONE)
			newText->SetUnit(ElectricalUnit::UNIT_MW);
		else
			newText->SetUnit(unit);
		newText->SetDecimalPlaces(precision);
		newText->SetElementTypeText(parentElement->GetElementType());
		newText->SetElementNumber(GetElementNumberFromList(parentElement));

		m_textList.emplace_back(newText);
	} break;
	case ID_TXT_REACTIVE_POWER: {
		if (FindTextElement(parentElement, DATA_REACTIVE_POWER)) return false;
		auto newText = std::make_shared<Text>(parentElement->GetPosition() + wxPoint2DDouble(0, 50), m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		newText->SetElement(parentElement);
		newText->SetDataType(DATA_REACTIVE_POWER);
		if (unit == ElectricalUnit::UNIT_NONE)
			newText->SetUnit(ElectricalUnit::UNIT_Mvar);
		else
			newText->SetUnit(unit);
		newText->SetDecimalPlaces(precision);
		newText->SetElementTypeText(parentElement->GetElementType());
		newText->SetElementNumber(GetElementNumberFromList(parentElement));

		m_textList.emplace_back(newText);
	} break;
	case ID_TXT_BRANCH_ACTIVE_POWER_1_2:
	case ID_TXT_BRANCH_ACTIVE_POWER_2_1: {
		if (FindTextElement(parentElement, DATA_PF_ACTIVE)) return false;
		wxPoint2DDouble position(0.0, -10.0);
		if (textID == ID_TXT_BRANCH_ACTIVE_POWER_1_2)
			position += 2.0 * parentElement->GetPointList()[1] - parentElement->GetPointList()[0];
		else
			position += 2.0 * parentElement->GetPointList()[parentElement->GetPointList().size() - 2] - parentElement->GetPointList()[parentElement->GetPointList().size() - 1];

		auto newText = std::make_shared<Text>(position, m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		newText->SetElement(parentElement);
		newText->SetDataType(DATA_PF_ACTIVE);
		if (unit == ElectricalUnit::UNIT_NONE)
			newText->SetUnit(ElectricalUnit::UNIT_MW);
		else
			newText->SetUnit(unit);
		newText->SetDecimalPlaces(precision);
		newText->SetElementTypeText(parentElement->GetElementType());
		newText->SetElementNumber(GetElementNumberFromList(parentElement));
		if (textID == ID_TXT_BRANCH_ACTIVE_POWER_2_1)
			newText->SetDirection(1); // std is 0

		m_textList.emplace_back(newText);
	} break;
	case ID_TXT_BRANCH_REACTIVE_POWER_1_2:
	case ID_TXT_BRANCH_REACTIVE_POWER_2_1: {
		if (FindTextElement(parentElement, DATA_PF_REACTIVE)) return false;
		wxPoint2DDouble position(0.0, 10.0);
		if (textID == ID_TXT_BRANCH_REACTIVE_POWER_1_2)
			position += 2.0 * parentElement->GetPointList()[1] - parentElement->GetPointList()[0];
		else
			position += 2.0 * parentElement->GetPointList()[parentElement->GetPointList().size() - 2] - parentElement->GetPointList()[parentElement->GetPointList().size() - 1];

		auto newText = std::make_shared<Text>(position, m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		newText->SetElement(parentElement);
		newText->SetDataType(DATA_PF_REACTIVE);
		if (unit == ElectricalUnit::UNIT_NONE)
			newText->SetUnit(ElectricalUnit::UNIT_Mvar);
		else
			newText->SetUnit(unit);
		newText->SetDecimalPlaces(precision);
		newText->SetElementTypeText(parentElement->GetElementType());
		newText->SetElementNumber(GetElementNumberFromList(parentElement));
		if (textID == ID_TXT_BRANCH_REACTIVE_POWER_2_1)
			newText->SetDirection(1); // std is 0

		m_textList.emplace_back(newText);
	} break;
	case ID_TXT_BRANCH_LOSSES: {
		if (FindTextElement(parentElement, DATA_PF_LOSSES)) return false;
		wxPoint2DDouble position = wxPoint2DDouble(0, 35) + (parentElement->GetPointList()[0] + parentElement->GetPointList()[parentElement->GetPointList().size() - 1]) / 2.0;
		auto newText = std::make_shared<Text>(position, m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		newText->SetElement(parentElement);
		newText->SetDataType(DATA_PF_LOSSES);
		if (unit == ElectricalUnit::UNIT_NONE)
			newText->SetUnit(ElectricalUnit::UNIT_MW);
		else
			newText->SetUnit(unit);
		newText->SetDecimalPlaces(precision);
		newText->SetElementTypeText(parentElement->GetElementType());
		newText->SetElementNumber(GetElementNumberFromList(parentElement));

		m_textList.emplace_back(newText);
	} break;
	case ID_TXT_BRANCH_CURRENT_1_2:
	case ID_TXT_BRANCH_CURRENT_2_1: {
		if (FindTextElement(parentElement, DATA_PF_CURRENT)) return false;
		wxPoint2DDouble position(0.0, 10.0);
		if (textID == ID_TXT_BRANCH_CURRENT_1_2)
			position += 2.0 * parentElement->GetPointList()[1] - parentElement->GetPointList()[0];
		else
			position += 2.0 * parentElement->GetPointList()[parentElement->GetPointList().size() - 2] - parentElement->GetPointList()[parentElement->GetPointList().size() - 1];

		auto newText = std::make_shared<Text>(position, m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		newText->SetElement(parentElement);
		newText->SetDataType(DATA_PF_CURRENT);
		if (unit == ElectricalUnit::UNIT_NONE)
			newText->SetUnit(ElectricalUnit::UNIT_A);
		else
			newText->SetUnit(unit);
		newText->SetDecimalPlaces(precision);
		newText->SetElementTypeText(parentElement->GetElementType());
		newText->SetElementNumber(GetElementNumberFromList(parentElement));
		if (textID == ID_TXT_BRANCH_CURRENT_2_1)
			newText->SetDirection(1); // std is 0

		m_textList.emplace_back(newText);
	} break;
	case ID_TXT_BRANCH_FAULT_CURRENT_1_2:
	case ID_TXT_BRANCH_FAULT_CURRENT_2_1: {
		if (FindTextElement(parentElement, DATA_SC_CURRENT)) return false;
		wxPoint2DDouble position(0.0, 25.0);
		if (textID == ID_TXT_BRANCH_FAULT_CURRENT_1_2)
			position += 2.0 * parentElement->GetPointList()[1] - parentElement->GetPointList()[0];
		else
			position += 2.0 * parentElement->GetPointList()[parentElement->GetPointList().size() - 2] - parentElement->GetPointList()[parentElement->GetPointList().size() - 1];

		auto newText = std::make_shared<Text>(position, m_properties->GetGeneralPropertiesData().labelFont, m_properties->GetGeneralPropertiesData().labelFontSize);
		newText->SetElement(parentElement);
		newText->SetDataType(DATA_SC_CURRENT);
		if (unit == ElectricalUnit::UNIT_NONE)
			newText->SetUnit(ElectricalUnit::UNIT_kA);
		else
			newText->SetUnit(unit);
		newText->SetDecimalPlaces(precision);
		newText->SetElementTypeText(parentElement->GetElementType());
		newText->SetElementNumber(GetElementNumberFromList(parentElement));
		if (textID == ID_TXT_BRANCH_FAULT_CURRENT_2_1)
			newText->SetDirection(1); // std is 0

		m_textList.emplace_back(newText);
	} break;
	default:
		return false;
		break;
	}
	return true;
}

Element* Workspace::FindTextElement(Element* parentElement, int dataType)
{
	for (auto& text : m_textList) {
		if (text->GetElement() == parentElement && text->GetDataType() == dataType)
			return text.get();
	}
	return nullptr;
}

void Workspace::RemoveAllTextElements()
{
	//for (auto* text : m_textList) {
	//	if (text) delete text;
	//}
	m_textList.clear();
	SaveCurrentState();
}

void Workspace::CheckSlackBusDuplication(Element* newSlackBus)
{
	Bus* newBus = dynamic_cast<Bus*>(newSlackBus);
	if (newBus) {
		if (!newBus->GetElectricalData().slackBus) return; // If the new bus is not set as slack bus, no need to check for duplication.

		for (auto& element : m_elementList) {
			Bus* bus = dynamic_cast<Bus*>(element.get());
			if (!bus) continue;
			if (bus->GetElectricalData().slackBus && bus != newSlackBus) {
				wxMessageDialog msgDialog(this,
					wxString::Format(_("The system already has %s as the slack bus.\nDo you want to set %s as the new slack bus?"), bus->GetElectricalData().name, newBus->GetElectricalData().name),
					_("Warning"), wxYES_NO | wxCENTRE | wxICON_WARNING);
				if (msgDialog.ShowModal() == wxID_YES) {
					auto data = bus->GetElectricalData();
					data.slackBus = false;
					bus->SetElectricalData(data);
					return;
				}
				else {
					auto data = newBus->GetElectricalData();
					data.slackBus = false;
					newBus->SetElectricalData(data);
					return;
				}
			}
		}
	}
}

void Workspace::ValidateBusesVoltages(Element* initialBus)
{
	double nominalVoltage = static_cast<Bus*>(initialBus)->GetElectricalData().nominalVoltage;
	ElectricalUnit nominalVoltageUnit = static_cast<Bus*>(initialBus)->GetElectricalData().nominalVoltageUnit;

	for (auto it = m_elementList.begin(); it != m_elementList.end(); it++) {
		Element* child = it->get();

		if (auto line = dynamic_cast<Line*>(child)) {
			if (line->GetParentList()[0] && line->GetParentList()[1]) {
				BusElectricalData data1 = static_cast<Bus*>(line->GetParentList()[0])->GetElectricalData();
				BusElectricalData data2 = static_cast<Bus*>(line->GetParentList()[1])->GetElectricalData();

				if (data1.nominalVoltage != data2.nominalVoltage ||
					data1.nominalVoltageUnit != data2.nominalVoltageUnit) {
					data1.nominalVoltage = nominalVoltage;
					data2.nominalVoltage = nominalVoltage;
					data1.nominalVoltageUnit = nominalVoltageUnit;
					data2.nominalVoltageUnit = nominalVoltageUnit;

					static_cast<Bus*>(line->GetParentList()[0])->SetElectricalData(data1);
					static_cast<Bus*>(line->GetParentList()[1])->SetElectricalData(data2);

					it = m_elementList.begin();  // Restart search.
				}
			}
		}
	}

	// ValidateElementsVoltages();
}

void Workspace::ValidateElementsVoltages()
{
	for (auto& child : m_elementList) {
		std::vector<double> nominalVoltage;
		std::vector<ElectricalUnit> nominalVoltageUnit;
		for (int i = 0; i < (int)child->GetParentList().size(); i++) {
			Bus* parent = static_cast<Bus*>(child->GetParentList()[i]);
			if (parent) {
				nominalVoltage.push_back(parent->GetElectricalData().nominalVoltage);
				nominalVoltageUnit.push_back(parent->GetElectricalData().nominalVoltageUnit);
			}
		}
		child->SetNominalVoltage(nominalVoltage, nominalVoltageUnit);
	}
}

void Workspace::ResetAllVoltages()
{
	PowerFlow pf(GetElementList());
	pf.ResetVoltages();
	UpdateTextElements();
	Redraw();
}

bool Workspace::RunPowerFlow(bool resetVoltages, bool showBusyInfo)
{
	auto simProp = m_properties->GetSimulationPropertiesData();
	double basePower = simProp.basePower;
	if (simProp.basePowerUnit == ElectricalUnit::UNIT_MVA)
		basePower *= 1e6;
	else if (simProp.basePowerUnit == ElectricalUnit::UNIT_kVA)
		basePower *= 1e3;

	// Update EMTElements
	for (auto& element : m_elementList) {
		if (auto emtElement = dynamic_cast<EMTElement*>(element.get())) {
			if (emtElement->IsOnline()) {
				emtElement->UpdateData(m_properties, true);
			}
		}
	}
	bool result = false;
	wxString errorMsg = "";
	int numIt = 0;

	wxStopWatch sw;
	{
		wxBusyInfo* info = nullptr;
		if (showBusyInfo)
			info = new wxBusyInfo(
				wxBusyInfoFlags()
				.Parent(this)
				.Icon(wxIcon(Paths::GetDataPath() + "/images/ribbon/powerFLow32.png", wxBITMAP_TYPE_PNG))
				.Title(_("<b>Calculating Power Flow</b>"))
				.Text(_("Please wait..."))
				.Foreground(*wxWHITE)
				.Background(*wxBLACK)
				.Transparency(4 * wxALPHA_OPAQUE / 5)
			);
		//wxBusyInfo info
		//(
		//	wxBusyInfoFlags()
		//	.Parent(this)
		//	.Icon(wxIcon(wxT("..\\data\\images\\ribbon\\powerFLow32.png"), wxBITMAP_TYPE_PNG))
		//	.Title(_("<b>Calculating Power Flow</b>"))
		//	.Text(_("Please wait..."))
		//	.Foreground(*wxWHITE)
		//	.Background(*wxBLACK)
		//	.Transparency(4 * wxALPHA_OPAQUE / 5)
		//);

		PowerFlow pf(GetElementList());
		if (resetVoltages) pf.ResetVoltages();

		switch (simProp.powerFlowMethod) {
		case GAUSS_SEIDEL: {
			result = pf.RunGaussSeidel(basePower, simProp.powerFlowMaxIterations, simProp.powerFlowTolerance,
				simProp.initAngle, simProp.accFator);
		} break;
		case NEWTON_RAPHSON: {
			result = pf.RunNewtonRaphson(basePower, simProp.powerFlowMaxIterations, simProp.powerFlowTolerance,
				simProp.initAngle, simProp.newtonInertia);
		} break;
		case GAUSS_NEWTON: {
			result =
				pf.RunGaussNewton(basePower, simProp.powerFlowMaxIterations, simProp.powerFlowTolerance,
					simProp.initAngle, simProp.accFator, simProp.gaussTolerance, simProp.newtonInertia);
		} break;
		}

		errorMsg = pf.GetErrorMessage();
		numIt = pf.GetIterations();

		if (showBusyInfo) delete info;
	}
	sw.Pause();

	if (!result) {
		wxMessageDialog msgDialog(this, errorMsg, _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
		msgDialog.ShowModal();
	}
	else {
		m_statusBar->SetStatusText(
			wxString::Format(_("Power flow converge with %d iterations (%ld ms)"), numIt, sw.Time()));
	}

	UpdateTextElements();
	UpdateHeatMap();
	Redraw();

	return result;
}

bool Workspace::UpdateTextElements()
{
	bool isTexturesOK = true;
	double basePower = m_properties->GetSimulationPropertiesData().basePower;
	if (m_properties->GetSimulationPropertiesData().basePowerUnit == ElectricalUnit::UNIT_kVA)
		basePower *= 1e3;
	else if (m_properties->GetSimulationPropertiesData().basePowerUnit == ElectricalUnit::UNIT_MVA)
		basePower *= 1e6;
	for (auto& text : m_textList) {
		text->SetFontName(m_properties->GetGeneralPropertiesData().labelFont);
		text->SetFontSize(m_properties->GetGeneralPropertiesData().labelFontSize);
		text->UpdateText(basePower);
		//if (!text->IsGLTextOK()) isTexturesOK = false;
	}
	return isTexturesOK;
}

void Workspace::CopySelection()
{
	UpdateElementsID();
	std::vector<Element*> selectedElements;
	// The buses need to be numerated to associate the child's parents to the copies.
	int busNumber = 0;
	for (auto& element : m_elementList) {
		if (auto bus = dynamic_cast<Bus*>(element.get())) {
			auto data = bus->GetElectricalData();
			data.number = busNumber;
			bus->SetElectricalData(data);
			busNumber++;
		}
		if (element->IsSelected()) { selectedElements.push_back(element.get()); }
	}
	for (auto& text : m_textList) {
		if (text->IsSelected()) { selectedElements.push_back(text.get()); }
	}
	ElementDataObject* dataObject = new ElementDataObject(selectedElements);
	if (wxTheClipboard->Open()) {
		wxTheClipboard->SetData(dataObject);
		wxTheClipboard->Close();
	}
}

bool Workspace::Paste()
{
	if (wxTheClipboard->Open()) {
		ElementDataObject dataObject;

		if (wxTheClipboard->IsSupported(dataObject.GetFormat())) {
			if (!wxTheClipboard->GetData(dataObject)) {
				wxMessageDialog dialog(this, _("It was not possible to paste from clipboard."), _("Error"),
					wxOK | wxCENTER | wxICON_ERROR, wxDefaultPosition);
				dialog.ShowModal();
				wxTheClipboard->Close();
				return false;
			}
		}
		else {
			wxTheClipboard->Close();
			return false;
		}
		wxTheClipboard->Close();

		UnselectAll();

		std::vector<Element*> pastedElements;
		ElementsLists* elementsLists = dataObject.GetElementsLists();

		// Paste buses (parents).
		auto parentList = elementsLists->parentList;
		std::vector<Bus*> pastedBusList;  // To set new parents;
		for (auto it = parentList.begin(), itEnd = parentList.end(); it != itEnd; ++it) {
			Element* copy = (*it)->GetCopy();
			if (copy) {
				pastedElements.push_back(copy);
				pastedBusList.push_back(static_cast<Bus*>(copy));
				m_elementList.emplace_back(static_cast<PowerElement*>(copy));
			}
		}

		// Paste other elements.
		auto elementLists = elementsLists->elementList;
		for (auto it = elementLists.begin(), itEnd = elementLists.end(); it != itEnd; ++it) {
			Element* copy = (*it)->GetCopy();
			if (copy) {
				// Check if is text element
				if (Text* text = dynamic_cast<Text*>(copy)) {
					// Check if element associated with the text exists.
					bool elementExist = false;
					for (auto& element : m_elementList) {
						if (text->GetElement() == element.get()) {
							elementExist = true;
							break;
						}
					}
					if (elementExist) {
						pastedElements.push_back(copy);
						m_textList.emplace_back(text);
					}
				}
				else {
					// Change the parent if copied, otherwise remove it.
					for (size_t j = 0; j < copy->GetParentList().size(); j++) {
						Bus* currentParent = static_cast<Bus*>(copy->GetParentList()[j]);
						if (currentParent) {
							int parentID = currentParent->GetID();
							bool parentCopied = false;
							for (size_t k = 0; k < pastedBusList.size(); k++) {
								Bus* newParent = pastedBusList[k];
								if (parentID == newParent->GetID()) {
									parentCopied = true;
									copy->ReplaceParent(currentParent, newParent);
									break;
								}
							}
							if (!parentCopied) copy->RemoveParent(currentParent);
						}
					}

					pastedElements.push_back(copy);
					m_elementList.emplace_back(static_cast<PowerElement*>(copy));
				}
			}
		}

		// Update buses childs
		for (auto it = pastedBusList.begin(), itEnd = pastedBusList.end(); it != itEnd; ++it) {
			Bus* bus = *it;
			std::vector<Element*> childList = bus->GetChildList();
			for (auto it = childList.begin(), itEnd = childList.end(); it != itEnd; ++it) {
				Element* currentChild = *it;
				int childID = currentChild->GetID();
				bool childCopied = false;
				for (int i = 0; i < (int)pastedElements.size(); i++) {
					Element* newChild = pastedElements[i];
					if (childID == newChild->GetID()) {
						childCopied = true;
						bus->ReplaceChild(currentChild, newChild);
						break;
					}
				}
				if (!childCopied) bus->RemoveChild(currentChild);
			}
		}

		// Move elements (and nodes) to the mouse position.
		// The start position it's the center of the pasted objects.
		wxPoint2DDouble leftUpCorner, rightDownCorner;
		GetElementsCorners(leftUpCorner, rightDownCorner, pastedElements);
		wxPoint2DDouble startPosition = (leftUpCorner + rightDownCorner) / 2.0;
		for (auto it = pastedElements.begin(), itEnd = pastedElements.end(); it != itEnd; ++it) {
			Element* element = *it;
			element->StartMove(startPosition);
			element->Move(m_camera->GetMousePosition());
			for (int i = 0; i < (int)element->GetParentList().size(); i++) {
				Element* parent = element->GetParentList()[i];
				element->MoveNode(parent, m_camera->GetMousePosition());
			}
		}
	}
	else {
		wxMessageDialog dialog(this, _("It was not possible to paste from clipboard."), _("Error"),
			wxOK | wxCENTER | wxICON_ERROR, wxDefaultPosition);
		dialog.ShowModal();
		return false;
	}

	UpdateElementsID();
	m_mode = WorkspaceMode::MODE_PASTE;
	m_statusBar->SetStatusText(_("Click to paste."));
	UpdateStatusBar();
	Redraw();
	return true;
}

void Workspace::SaveCurrentState()
{
	//return;
//#ifdef __WXMSW__
//	const DWORD beforeSaveState = GetWorkspaceGDIObjects();
//#endif
	// Setup current state
	std::vector< std::shared_ptr<PowerElement> > currentStateElementList;
	std::vector< std::shared_ptr<Text> > currentStateTextList;

	GetStateListsCopy(m_elementList, m_textList, currentStateElementList, currentStateTextList);
//#ifdef __WXMSW__
//	LogWorkspaceGDIDelta("Workspace::SaveCurrentState after copy", beforeSaveState);
//#endif

	// Delete all states after the current one
	//auto itE = m_elementListState.begin();
	//std::advance(itE, m_currenteState + 1);
	//for (; itE != m_elementListState.end(); ++itE) {
	//	auto& elementList = *itE;
	//	for (auto& element : elementList) delete element;
	//	elementList.clear();
	//}
	m_elementListState.resize(m_currenteState + 1);

	//auto itT = m_textListState.begin();
	//std::advance(itT, m_currenteState + 1);
	//for (; itT != m_textListState.end(); ++itT) {
	//	auto& textList = *itT;
	//	for (auto& text : textList) delete text;
	//	textList.clear();
	//}
	m_textListState.resize(m_currenteState + 1);

	m_currenteState++;
	if (m_currenteState >= m_maxStates) {
		m_currenteState = m_maxStates - 1;
		// Erase the first sate on the list
		//auto itE = m_elementListState.begin();
		////auto itT = m_textListState.begin();
		//auto& elementList = *itE;
		////auto& textList = *itT;
		//for (auto& element : elementList) delete element;
		//elementList.clear();
		////for (auto& text : textList) delete text;
		////textList.clear();
		//
		//m_elementListState.erase(itE);
		//m_textListState.erase(itT);
		m_elementListState.erase(m_elementListState.begin());
		m_textListState.erase(m_textListState.begin());
	}

	m_elementListState.emplace_back(currentStateElementList);
	m_textListState.emplace_back(currentStateTextList);
//#ifdef __WXMSW__
//	LogWorkspaceGDIDelta(wxString::Format("Workspace::SaveCurrentState stored states=%zu elements=%zu texts=%zu",
//		m_elementListState.size(),
//		currentStateElementList.size(),
//		currentStateTextList.size()),
//		beforeSaveState);
//#endif

#ifdef _DEBUG
	wxString msg = "";
	wxString pointerStr;
	pointerStr.Printf("[%d S saved s%d] ", m_currenteState, m_elementListState.size());
	msg += pointerStr;
	for (auto& element : currentStateElementList) {
		pointerStr.Printf("%p ", element.get());
		msg += pointerStr;
	}
	msg += "\n";
	pointerStr.Printf("[%d S curr s%d] ", m_currenteState, m_elementListState.size());
	msg += pointerStr;
	for (auto& element : m_elementList) {
		pointerStr.Printf("%p ", element.get());
		msg += pointerStr;
	}
	msg += "\n";
	//m_debugFrame->AppendDebugMessage(msg);
#endif
}

void Workspace::SetNextState()
{
	m_currenteState++;
	if (m_currenteState >= 0 &&
		static_cast<size_t>(m_currenteState) < m_elementListState.size() &&
		static_cast<size_t>(m_currenteState) < m_textListState.size()) {
		//m_elementList = m_elementListState[m_currenteState];
		//m_textList = m_textListState[m_currenteState];
		GetStateListsCopy(m_elementListState[m_currenteState], m_textListState[m_currenteState], m_elementList, m_textList);

#ifdef _DEBUG
#ifdef SHOW_DEBUG_PANEL
		wxString msg = "";
		wxString pointerStr;
		pointerStr.Printf("[%d N curr s%d] ", m_currenteState, m_elementListState.size());
		msg += pointerStr;
		for (Element* element : m_elementList) {
			pointerStr.Printf("%p ", element);
			msg += pointerStr;
		}
		msg += "\n";
		//m_debugFrame->AppendDebugMessage(msg);
#endif
#endif

		UpdateTextElements();
		Redraw();
	}
	else {
		m_currenteState--;
	}
}

void Workspace::SetPreviousState()
{
	m_currenteState--;
	if (m_currenteState >= 0) {
		//m_elementList = m_elementListState[m_currenteState];
		//m_textList = m_textListState[m_currenteState];
		GetStateListsCopy(m_elementListState[m_currenteState], m_textListState[m_currenteState], m_elementList, m_textList);

#ifdef _DEBUG
#ifdef SHOW_DEBUG_PANEL
		wxString msg = "";
		wxString pointerStr;
		pointerStr.Printf("[%d P curr s%d] ", m_currenteState, m_elementListState.size());
		msg += pointerStr;
		for (Element* element : m_elementListState[m_currenteState]) {
			pointerStr.Printf("%p ", element);
			msg += pointerStr;
		}
		msg += "\n";
		pointerStr.Printf("[%d P list s%d] ", m_currenteState, m_elementListState.size());
		msg += pointerStr;
		for (Element* element : m_elementList) {
			pointerStr.Printf("%p ", element);
			msg += pointerStr;
		}
		msg += "\n";
		//m_debugFrame->AppendDebugMessage(msg);
#endif
#endif

		UpdateTextElements();
		Redraw();
	}
	else {
		m_currenteState++;
	}
}

void Workspace::UnselectAll()
{
	for (auto& element : m_elementList) {
		element->SetSelected(false);
	}
	for (auto& text : m_textList) {
		text->SetSelected(false);
	}
}

void Workspace::EnableHeatMap(const bool& enable)
{
	m_showHM = enable;
	if (m_showHM) {
		UpdateHeatMap();
	}
	Redraw();
}

void Workspace::UpdateElementsID()
{
	int id = 0;
	for (auto& element : m_elementList) {
		element->SetID(id);
		id++;
	}
	for (auto& text : m_textList) {
		text->SetID(id);
		id++;
	}
}
void Workspace::OnTimer(wxTimerEvent& event)
{
	if (m_tipWindow) {
		m_tipWindow->Close();
		m_tipWindow = nullptr;
	}
	if (m_mode == WorkspaceMode::MODE_EDIT) {
		for (auto& element : m_elementList) {
			if (element->Contains(m_camera->GetMousePosition())) {
				wxString tipText = element->GetTipText();
				if (!tipText.IsEmpty()) {
					//m_tipWindow = new wxTipWindow(this, tipText, 10000, &m_tipWindow);
					m_tipWindow = wxTipWindow::New(this, tipText, 10000);
					// Creates a very tiny bounding rect to remove the tip on any mouse movement.
					m_tipWindow->SetBoundingRect(wxRect(wxGetMousePosition(), wxSize(1, 1)));
					break;
				}
			}
		}
	}

	m_timer->Stop();
}

void Workspace::SetTextList(const std::vector< std::shared_ptr<Text> >& textList)
{
	//m_textList.clear();
	//for (auto it = textList.begin(), itEnd = textList.end(); it != itEnd; ++it) m_textList.push_back(*it);
	m_textList = std::move(textList);

	UpdateTextElements();
}

void Workspace::SetName(wxString name)
{
	m_name = name;
#ifdef _DEBUG
#ifdef SHOW_DEBUG_PANEL
	m_debugFrame->SetTitle(_("Debug window: ") + m_name);
#endif
#endif
}

void Workspace::SetElementList(std::vector< std::shared_ptr<PowerElement> > elementList)
{
	m_elementList = std::move(elementList);
}

void Workspace::SetElementList(std::vector<Element*> elementList)
{
	m_elementList.clear();
	for (auto it = elementList.begin(), itEnd = elementList.end(); it != itEnd; ++it)
		m_elementList.emplace_back(static_cast<PowerElement*>(*it));
}

void Workspace::OnIdle(wxIdleEvent& event)
{
	/*
	// The OpenGL element (m_glCanvas) must be completely initialized (showed) to draw properly the textures and init glew.
	// TODO(?): Find other solution to text displayed wrong on opened file.
	if (m_justOpened) {
		if (UpdateTextElements()) m_justOpened = false;
		if (glewInit() != GLEW_OK)
			wxMessageBox("Erro ao iniciar glew ='(");
		else
		{
			// Load and create shader
			m_basicShader = new Shader("..//data//shaders//Basic.shader");
			m_hmLabelShader = new Shader("..//data//shaders//HMLabel.shader");
			float limits[2] = { 1.05, 0.95 };
			m_hmPlane = new HMPlane(m_basicShader, m_hmLabelShader, m_width, m_height, limits);
		}

		Redraw();
	}
	*/
	if (m_justOpened) {
		m_justOpened = false;
		double limits[2] = { 1.05, 0.95 };
		m_hmPlane = new HMPlane(m_width, m_height, limits);

		Redraw();
	}
}

std::vector<Element*> Workspace::GetAllElements() const
{
	std::vector<Element*> allElements;

	for (auto& element : m_elementList) allElements.push_back(element.get());
	for (auto& text : m_textList) allElements.push_back(text.get());

	return allElements;
}

bool Workspace::RunFault()
{
	auto simProp = m_properties->GetSimulationPropertiesData();
	double basePower = simProp.basePower;
	if (simProp.basePowerUnit == ElectricalUnit::UNIT_MVA)
		basePower *= 1e6;
	else if (simProp.basePowerUnit == ElectricalUnit::UNIT_kVA)
		basePower *= 1e3;

	Fault fault(GetElementList());
	bool result = fault.RunFaultCalculation(basePower);
	if (!result) {
		wxMessageDialog msgDialog(this, fault.GetErrorMessage(), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
		msgDialog.ShowModal();
	}

	UpdateTextElements();
	Redraw();

	return result;
}

std::vector<Element*> Workspace::GetElementList() const
{
	std::vector<Element*> elementList;
	for (auto& element : m_elementList) elementList.push_back(element.get());
	return elementList;
}

bool Workspace::RunSCPower()
{
	Fault fault(GetElementList());
	bool result = fault.RunSCPowerCalcutation(100e6);
	if (!result) {
		wxMessageDialog msgDialog(this, fault.GetErrorMessage(), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
		msgDialog.ShowModal();
	}

	UpdateTextElements();
	Redraw();

	return result;
}

bool Workspace::RunStability()
{
	// Run power flow before stability.
	RunPowerFlow();

	Electromechanical stability(this, GetElementList(), m_properties->GetSimulationPropertiesData());
	wxStopWatch sw;
	bool result = stability.RunStabilityCalculation();
#ifdef _DEBUG
#ifdef SHOW_DEBUG_PANEL
	m_debugFrame->AppendDebugMessage(stability.GetDebugMessage());
#endif 
#endif // _DEBUG

	sw.Pause();
	if (!result) {
		wxMessageDialog msgDialog(this, stability.GetErrorMessage(), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
		msgDialog.ShowModal();
	}
	m_stabilityTimeVector.clear();
	m_stabilityTimeVector = stability.GetTimeVector();

	// Run power flow after stability.
	RunPowerFlow();

	wxMessageDialog msgDialog(
		this,
		wxString::Format(_("The program took %ld ms to run this system.\nDo you wish to open the stability graphics?"),
			sw.Time()),
		_("Question"), wxYES_NO | wxCENTRE | wxICON_QUESTION);
	if (msgDialog.ShowModal() == wxID_YES) {
		std::vector<ElementPlotData> plotDataList;
		for (auto& element : m_elementList) {
			ElementPlotData plotData;
			if (element->GetPlotData(plotData)) plotDataList.push_back(plotData);
		}
#ifdef SHOW_SIMULATION_PARAMETERS
		ElementPlotData plotData;
		plotData.SetName(_("Simulation parameters"));
		plotData.SetCurveType(ElementPlotData::CurveType::CT_TEST);
		plotData.AddData(stability.GetIterationVector(), _("Iterations number"));
		plotDataList.push_back(plotData);
#endif
		ChartView* cView = new ChartView(this, plotDataList, m_stabilityTimeVector, m_properties->GetGeneralPropertiesData().plotLib);
		cView->Show();
	}

	return result;
}
void Workspace::OnMiddleDoubleClick(wxMouseEvent& event)
{
	Fit();
	event.Skip();
}

bool Workspace::RunStaticStudies()
{
	bool pfStatus, faultStatus, scStatus, harmStatus;
	pfStatus = faultStatus = scStatus = harmStatus = false;

	bool runHarmDistortion = m_properties->GetSimulationPropertiesData().harmDistortionAfterPowerFlow;

	pfStatus = RunPowerFlow(runHarmDistortion);

	if (m_properties->GetSimulationPropertiesData().faultAfterPowerFlow) {
		if (pfStatus) faultStatus = RunFault();
	}
	else {
		faultStatus = true;
	}

	if (m_properties->GetSimulationPropertiesData().scPowerAfterPowerFlow) {
		if (pfStatus) scStatus = RunSCPower();
	}
	else {
		scStatus = true;
	}

	if (runHarmDistortion) {
		if (pfStatus) harmStatus = RunHarmonicDistortion(false);
	}
	else {
		harmStatus = true;
	}

	if (pfStatus && faultStatus && scStatus && harmStatus) return true;

	return false;
}

bool Workspace::RunHarmonicDistortion(bool runPowerFlowBefore)
{
	auto simProp = m_properties->GetSimulationPropertiesData();
	double basePower = simProp.basePower;
	if (simProp.basePowerUnit == ElectricalUnit::UNIT_MVA)
		basePower *= 1e6;
	else if (simProp.basePowerUnit == ElectricalUnit::UNIT_kVA)
		basePower *= 1e3;
	if (runPowerFlowBefore) {
		if (!RunPowerFlow(true)) return false;
	}

	bool hasEMTElement = false;
	for (auto& element : m_elementList) {
		if (auto emtElement = dynamic_cast<EMTElement*>(element.get())) {
			if (emtElement->IsOnline()) hasEMTElement = true;
		}
	}

	HarmLoadConnection loadConnection = simProp.harmLoadConnection;

	PowerQuality pq(GetElementList());
	bool result = pq.CalculateDistortions(basePower, loadConnection);

	// If has EMT element, repeat the Power Flow and Harmonics calculation untion DHT converge.
	if (hasEMTElement && result) {
		wxBusyInfo info(
			wxBusyInfoFlags()
			.Parent(this)
			.Icon(wxIcon(Paths::GetDataPath() + "/images/ribbon/harmDist32.png", wxBITMAP_TYPE_PNG))
			.Title(_("<b>Calculating Harmonic Flow</b>"))
			.Text(_("Please wait..."))
			.Foreground(*wxWHITE)
			.Background(*wxBLACK)
			.Transparency(4 * wxALPHA_OPAQUE / 5)
		);
		std::vector<double> thdList;
		for (auto const& bus : pq.GetBusList())
			thdList.emplace_back(bus->GetElectricalData().thd);
		double error = 1e3;
		while (error > 1e-3) {
			// Run Power Flow
			if (!RunPowerFlow(false, false)) return false;

			// Run Harmonic Distortion
			bool result = pq.CalculateDistortions(basePower, loadConnection);
			if (!result) break;

			// Calculate error
			int i = 0;
			for (auto const& bus : pq.GetBusList()) {
				double errorBus = std::abs(bus->GetElectricalData().thd - thdList[i]);
				if (i == 0)
					error = errorBus;
				else if (errorBus > error)
					error = errorBus;
				thdList[i] = bus->GetElectricalData().thd;
				i++;
			}
		}
	}

	if (!result) {
		wxMessageDialog msgDialog(this, pq.GetErrorMessage(), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
		msgDialog.ShowModal();
	}
	else {
		UpdateTextElements();
		Redraw();
	}

	return result;
}

bool Workspace::RunFrequencyResponse()
{
	// Get bus list
	std::vector<Bus*> busList;
	for (auto& element : m_elementList) {
		if (Bus* bus = dynamic_cast<Bus*>(element.get())) { busList.push_back(bus); }
	}

	auto data = m_properties->GetFreqRespData();

	FrequencyResponseForm frForm(this, busList, data.injBusNumber, data.initFreq, data.finalFreq, data.stepFreq);

	if (frForm.ShowModal() == wxID_OK) {
		data.initFreq = frForm.GetInitFreq();
		data.finalFreq = frForm.GetEndFreq();
		data.stepFreq = frForm.GetStepFreq();
		data.injBusNumber = frForm.GetInjBusNumber();
		m_properties->SetFreqRespData(data);
	}
	else
		return false;

	auto simProp = m_properties->GetSimulationPropertiesData();
	double basePower = simProp.basePower;
	if (simProp.basePowerUnit == ElectricalUnit::UNIT_MVA)
		basePower *= 1e6;
	else if (simProp.basePowerUnit == ElectricalUnit::UNIT_kVA)
		basePower *= 1e3;
	PowerQuality pq(GetElementList());
	bool result = pq.CalculateFrequencyResponse(simProp.stabilityFrequency, data.initFreq, data.finalFreq,
		data.stepFreq, data.injBusNumber, basePower, simProp.harmLoadConnection);

	wxMessageDialog msgDialog(
		this, wxString::Format(_("Calculations done.\nDo you wish to open the frequency response graphics?")),
		_("Question"), wxYES_NO | wxCENTRE | wxICON_QUESTION);
	if (msgDialog.ShowModal() == wxID_YES) {
		std::vector<ElementPlotData> plotDataList;
		for (auto& element : m_elementList) {
			ElementPlotData plotData;
			if (element->GetPlotData(plotData, PlotStudy::FREQRESPONSE)) plotDataList.push_back(plotData);
		}

		ChartView* cView = new ChartView(this, plotDataList, pq.GetFrequencies(), m_properties->GetGeneralPropertiesData().plotLib);
		cView->Show();
	}

	UpdateTextElements();
	Redraw();

	return result;
}
void Workspace::OnResize(wxSizeEvent& event)
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
void Workspace::OnHeatMapTime(wxTimerEvent& event)
{
	if (m_showHMTimer) {
		UpdateHeatMap();
		Redraw();
		m_showHMTimer = false;
	}
}
