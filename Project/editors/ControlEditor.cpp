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

#include "ControlEditor.h"

#include<wx/settings.h>

#include "../elements/controlElement/ConnectionLine.h"
#include "../elements/controlElement/Constant.h"
#include "../elements/controlElement/ControlElement.h"
#include "../elements/controlElement/Divider.h"
#include "../elements/controlElement/Exponential.h"
#include "../elements/controlElement/Gain.h"
#include "../elements/controlElement/Limiter.h"
#include "../elements/controlElement/MathExpression.h"
#include "../elements/controlElement/MathOperation.h"
#include "../elements/controlElement/Multiplier.h"
#include "../elements/controlElement/RateLimiter.h"
#include "../elements/controlElement/Sum.h"
#include "../elements/controlElement/TransferFunction.h"
#include "../elements/controlElement/ControlElementContainer.h"
#include "../elements/controlElement/ControlElementSolver.h"

#ifdef USING_WX_3_0_X
#include "utils/DegreesAndRadians.h"
#endif
#include "../utils/Camera.h"
#include "../utils/FileHanding.h"
#include "../utils/Path.h"

#include "ChartView.h"
#include "../utils/ElementPlotData.h"

#include "../elements/ControlElementDataObject.h"

namespace {
void SanitizeControlGraph(const std::vector<std::shared_ptr<ControlElement>>& elementList,
	const std::vector<std::shared_ptr<ConnectionLine>>& connectionList)
{
	std::unordered_set<Element*> elementSet;
	std::unordered_set<ConnectionLine*> lineSet;
	elementSet.reserve(elementList.size());
	lineSet.reserve(connectionList.size());

	for (const auto& element : elementList) elementSet.insert(element.get());
	for (const auto& line : connectionList) lineSet.insert(line.get());

	for (const auto& element : elementList) {
		std::vector<Element*> children = element->GetChildList();
		for (Element* child : children) {
			auto* childLine = dynamic_cast<ConnectionLine*>(child);
			if (!childLine || !lineSet.contains(childLine)) {
				element->RemoveChild(child);
			}
		}
	}

	for (const auto& line : connectionList) {
		std::vector<Element*> parents = line->GetParentList();
		for (Element* parent : parents) {
			auto* parentElement = dynamic_cast<ControlElement*>(parent);
			if (!parentElement || !elementSet.contains(parentElement)) {
				line->RemoveParent(parent);
			}
		}

		ConnectionLine* parentLine = line->GetParentLine();
		if (parentLine && !lineSet.contains(parentLine)) {
			line->SetParentLine(nullptr);
		}

		std::vector<Element*> children = line->GetChildList();
		for (Element* child : children) {
			auto* childLine = dynamic_cast<ConnectionLine*>(child);
			if (!childLine || !lineSet.contains(childLine)) {
				line->RemoveChild(child);
			}
		}
	}
}
}  // namespace

ControlElementButton::ControlElementButton(wxWindow* parent, wxString label, wxImage image, wxWindowID id, GUIColour* guiColour)
	: wxWindow(parent, id), m_guiColour(guiColour)
{
	SetBackgroundColour(m_guiColour->background);
	m_buttonFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	m_label = label;
	m_image = image;
	m_imageSize = wxSize(image.GetWidth(), image.GetHeight());

	// Calculate label size.
	wxScreenDC dc;
	dc.SetFont(m_buttonFont);
	wxSize textSize = dc.GetTextExtent(label);

	int buttonWidth = 0;
	if (textSize.GetWidth() > m_imageSize.GetWidth()) {
		buttonWidth = textSize.GetWidth();
		m_imagePosition = wxPoint((buttonWidth - m_imageSize.GetWidth()) / 2 + m_borderSize, m_borderSize);
		m_labelPosition = wxPoint(m_borderSize, m_imageSize.GetHeight() + m_borderSize);
	}
	else {
		buttonWidth = m_imageSize.GetWidth();
		m_imagePosition = wxPoint(m_borderSize, m_borderSize);
		m_labelPosition =
			wxPoint((buttonWidth - textSize.GetWidth()) / 2 + m_borderSize, m_imageSize.GetHeight() + m_borderSize);
	}
	m_buttonSize =
		wxSize(buttonWidth + 2 * m_borderSize, textSize.GetHeight() + m_imageSize.GetHeight() + 2 * m_borderSize);
	SetMinSize(m_buttonSize + wxSize(m_borderSize, m_borderSize));

	// Events.
	Bind(wxEVT_PAINT, &ControlElementButton::OnPaint, this);
	Bind(wxEVT_ENTER_WINDOW, &ControlElementButton::OnMouseEnter, this);
	Bind(wxEVT_LEAVE_WINDOW, &ControlElementButton::OnMouseLeave, this);
	Bind(wxEVT_LEFT_DOWN, &ControlElementButton::OnLeftClickDown, this);
	Bind(wxEVT_LEFT_UP, &ControlElementButton::OnLeftClickUp, this);
}

ControlElementButton::ControlElementButton(wxWindow* parent, wxString label, std::unique_ptr<ControlElement> iconElement,
	wxWindowID id, GUIColour* guiColour, double buttonScale) :
	wxWindow(parent, id), m_guiColour(guiColour), m_iconElement(std::move(iconElement)), m_buttonScale(buttonScale)
{
	SetBackgroundColour(m_guiColour->background);
	m_buttonFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	//m_buttonFont.SetPointSize(static_cast<int>(m_buttonFont.GetPointSize() * (1.0 / m_buttonScale)));
	m_label = label;

	// Calculate label size.
	wxScreenDC dc;
	dc.SetFont(m_buttonFont);
	//wxSize textSize = dc.GetTextExtent(label) * m_buttonScale;
	wxSize textSize = dc.GetTextExtent(label);

	m_imageSize = wxSize(static_cast<int>(m_iconElement->GetWidth()), static_cast<int>(m_iconElement->GetHeight())) * m_buttonScale;

	int buttonWidth = 0;
	if (textSize.GetWidth() > m_imageSize.GetWidth()) {
		buttonWidth = textSize.GetWidth();
		m_imagePosition = wxPoint((buttonWidth - m_imageSize.GetWidth()) / 2 + m_borderSize, m_borderSize);
		m_labelPosition = wxPoint(m_borderSize, m_imageSize.GetHeight() + m_borderSize);
	}
	else {
		buttonWidth = m_imageSize.GetWidth();
		m_imagePosition = wxPoint(m_borderSize, m_borderSize);
		m_labelPosition =
			wxPoint((buttonWidth - textSize.GetWidth()) / 2 + m_borderSize, m_imageSize.GetHeight() + m_borderSize);
	}
	m_buttonSize =
		wxSize(buttonWidth + 2 * m_borderSize, textSize.GetHeight() + m_imageSize.GetHeight() + 2 * m_borderSize);
	SetMinSize(m_buttonSize + wxSize(m_borderSize, m_borderSize));

	m_iconElement->StartMove(m_iconElement->GetPosition());
	m_iconElement->Move(wxPoint2DDouble(m_buttonSize.GetWidth() / 2.0, (m_buttonSize.GetHeight() - textSize.GetHeight()) / 2.0));

	// Events.
	Bind(wxEVT_PAINT, &ControlElementButton::OnPaint, this);
	Bind(wxEVT_ENTER_WINDOW, &ControlElementButton::OnMouseEnter, this);
	Bind(wxEVT_LEAVE_WINDOW, &ControlElementButton::OnMouseLeave, this);
	Bind(wxEVT_LEFT_DOWN, &ControlElementButton::OnLeftClickDown, this);
	Bind(wxEVT_LEFT_UP, &ControlElementButton::OnLeftClickUp, this);
}

ControlElementButton::~ControlElementButton() {}
void ControlElementButton::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
	if (gc) {

		if (m_mouseAbove) {
			if (m_selected) {
				gc->SetPen(wxPen(m_guiColour->altSelection, m_borderSize - 1));
				gc->SetBrush(m_guiColour->altSelection);
			}
			else {
				gc->SetPen(*wxTRANSPARENT_PEN);
				gc->SetBrush(m_guiColour->selection);
			}
			gc->DrawRectangle(m_borderSize / 2, m_borderSize / 2, m_buttonSize.GetWidth(), m_buttonSize.GetHeight());
		}

		gc->SetFont(m_buttonFont, m_guiColour->text);
		gc->DrawText(m_label, m_labelPosition.x, m_labelPosition.y);

		if (m_iconElement) {
			gc->Translate(m_buttonSize.GetWidth() / 2.0, m_buttonSize.GetHeight() / 2.0);
			gc->Scale(m_buttonScale, m_buttonScale);
			gc->Translate(-m_buttonSize.GetWidth() / 2.0, -m_buttonSize.GetHeight() / 2.0);
			m_iconElement->DrawDC(m_guiColour, wxPoint2DDouble(0, 0), 1.0, gc);
		}
		else {
			gc->DrawBitmap(gc->CreateBitmapFromImage(m_image), m_imagePosition.x, m_imagePosition.y, m_imageSize.GetWidth(),
				m_imageSize.GetHeight());
		}

		delete gc;
	}
}

void ControlElementButton::OnMouseEnter(wxMouseEvent& event)
{
	m_mouseAbove = true;
	Refresh();
	event.Skip();
}

void ControlElementButton::OnMouseLeave(wxMouseEvent& event)
{
	m_mouseAbove = false;
	Refresh();
	event.Skip();
}

void ControlElementButton::OnLeftClickDown(wxMouseEvent& event)
{
	m_selected = true;
	Refresh();
	event.Skip();
}

void ControlElementButton::OnLeftClickUp(wxMouseEvent& event)
{
	m_selected = false;
	Refresh();
	event.Skip();
}

ControlEditor::ControlEditor(wxWindow* parent, PropertiesData* properties, int ioflags) : ControlEditorBase(parent), m_properties(properties)
{
	BuildControlElementPanel();
	m_camera = new Camera();
	m_selectionRect = wxRect2DDouble(0, 0, 0, 0);;
	m_cePanel->SetBackgroundColour(properties->GetGUIColour()->background);
	m_cePanel->SetBackgroundStyle(wxBG_STYLE_PAINT);  // To allow wxBufferedPaintDC works properly.
	m_ioFlags = ioflags;
	m_font = wxFont(m_properties->GetGeneralPropertiesData().labelFontSize,
		wxFONTFAMILY_DEFAULT,
		wxFONTSTYLE_NORMAL,
		wxFONTWEIGHT_NORMAL,
		false,
		m_properties->GetGeneralPropertiesData().labelFont);

	BuildColourList();
}
ControlEditor::~ControlEditor()
{
	ClearStates();
	delete m_camera;
	m_camera = nullptr;
	// m_tfButton->Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ControlEditor::LeftClickDown), m_tfButton, this);
}

wxPoint ControlEditor::GetEditorMouseClientPoint() const
{
	if (!m_cePanel) return wxPoint(0, 0);
	return m_cePanel->ScreenToClient(wxGetMousePosition());
}

wxPoint2DDouble ControlEditor::GetEditorMouseWorldPoint() const
{
	if (!m_camera) return wxPoint2DDouble(0, 0);
	wxPoint clientMouse = GetEditorMouseClientPoint();
	if (m_cePanel && m_cePanel->GetClientRect().Contains(clientMouse))
		return m_camera->ScreenToWorld(clientMouse);
	return m_camera->GetMousePosition();
}

void ControlEditor::BuildControlElementPanel()
{
	m_panelControlElements->SetDoubleBuffered(true);

	wxWrapSizer* wrapSizer = new wxWrapSizer();
	m_panelControlElements->SetSizer(wrapSizer);

	auto* guiColour = m_properties->GetGUIColour();

	m_panelControlElements->SetBackgroundColour(guiColour->background);

	wxFileName exeFileName(wxStandardPaths::Get().GetExecutablePath());

	auto iconIOControl = std::make_unique<IOControl>(IOControl::IOFlags::IN_IO, -1);
	iconIOControl->SetValue(IOControl::IOFlags::IN_IO);

	ControlElementButton* ioButton = new ControlElementButton(
		m_panelControlElements, _("In/Out"),
		std::move(iconIOControl),
		static_cast<int>(ControlElementButtonID::ID_IO),
		guiColour, 0.8);
	wrapSizer->Add(ioButton, 0, wxALL, 5);
	ioButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

	auto iconTF = std::make_unique<TransferFunction>(-1);

	ControlElementButton* tfButton = new ControlElementButton(
		m_panelControlElements, _("Transfer fcn"),
		std::move(iconTF),
		static_cast<int>(ControlElementButtonID::ID_TF),
		guiColour, 0.8);
	wrapSizer->Add(tfButton, 0, wxALL, 5);
	tfButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

	auto iconSum = std::make_unique<Sum>(-1);

	ControlElementButton* sumButton = new ControlElementButton(
		m_panelControlElements, _("Sum"),
		std::move(iconSum),
		static_cast<int>(ControlElementButtonID::ID_SUM),
		guiColour, 0.8);
	wrapSizer->Add(sumButton, 0, wxALL, 5);
	sumButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

	auto iconConstant = std::make_unique<Constant>(-1);
	iconConstant->SetValue(3.14);

	ControlElementButton* constButton = new ControlElementButton(
		m_panelControlElements, _("Constant"),
		std::move(iconConstant),
		static_cast<int>(ControlElementButtonID::ID_CONST),
		guiColour);
	wrapSizer->Add(constButton, 0, wxALL, 5);
	constButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

	auto iconGain = std::make_unique<Gain>(-1);
	iconGain->SetValue(0.5);

	ControlElementButton* gainButton = new ControlElementButton(
		m_panelControlElements, _("Gain"),
		std::move(iconGain),
		static_cast<int>(ControlElementButtonID::ID_GAIN),
		guiColour, 0.8);
	wrapSizer->Add(gainButton, 0, wxALL, 5);
	gainButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

	auto iconLimiter = std::make_unique<Limiter>(-1);

	ControlElementButton* limButton = new ControlElementButton(
		m_panelControlElements, _("Limiter"),
		std::move(iconLimiter),
		static_cast<int>(ControlElementButtonID::ID_LIMITER),
		guiColour, 0.8);
	wrapSizer->Add(limButton, 0, wxALL, 5);
	limButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

	auto iconRateLim = std::make_unique<RateLimiter>(-1);

	ControlElementButton* rateLimButton = new ControlElementButton(
		m_panelControlElements, _("Rate limiter"),
		std::move(iconRateLim),
		static_cast<int>(ControlElementButtonID::ID_RATELIM),
		guiColour, 0.8);
	wrapSizer->Add(rateLimButton, 0, wxALL, 5);
	rateLimButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

	auto iconMult = std::make_unique<Multiplier>(-1);

	ControlElementButton* multButton = new ControlElementButton(
		m_panelControlElements, _("Multiplier"),
		std::move(iconMult),
		static_cast<int>(ControlElementButtonID::ID_MULT),
		guiColour, 0.8);
	wrapSizer->Add(multButton, 0, wxALL, 5);
	multButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

	auto iconDiv = std::make_unique<Divider>(-1);

	ControlElementButton* divButton = new ControlElementButton(
		m_panelControlElements, _("Divider"),
		std::move(iconDiv),
		static_cast<int>(ControlElementButtonID::ID_MATH_DIV),
		guiColour, 0.8);
	wrapSizer->Add(divButton, 0, wxALL, 5);
	divButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

	auto iconMathExpr = std::make_unique<MathExpression>(-1);

	ControlElementButton* mathExprButton = new ControlElementButton(
		m_panelControlElements, _("Math Expression"),
		std::move(iconMathExpr),
		static_cast<int>(ControlElementButtonID::ID_MATH_EXPR),
		guiColour, 0.8);
	wrapSizer->Add(mathExprButton, 0, wxALL, 5);
	mathExprButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

	auto iconExp = std::make_unique<Exponential>(-1);

	ControlElementButton* satButton = new ControlElementButton(
		m_panelControlElements, _("Exponential"),
		std::move(iconExp),
		static_cast<int>(ControlElementButtonID::ID_EXP),
		guiColour, 0.8);
	wrapSizer->Add(satButton, 0, wxALL, 5);
	satButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);
}

void ControlEditor::LeftClickDown(wxMouseEvent& event)
{
	AddElement(static_cast<ControlElementButtonID>(event.GetId()));
	event.Skip();
}

void ControlEditor::AddElement(ControlElementButtonID id)
{
	switch (id) {
	case ControlElementButtonID::ID_IO: {
		m_mode = ControlEditorMode::MODE_INSERT;
		auto io = std::make_shared<IOControl>(m_ioFlags, GetNextID());
		m_elementList.push_back(io);
	} break;
	case ControlElementButtonID::ID_TF: {
		m_mode = ControlEditorMode::MODE_INSERT;
		auto tf = std::make_shared<TransferFunction>(GetNextID());
		m_elementList.push_back(tf);
	} break;
	case ControlElementButtonID::ID_SUM: {
		m_mode = ControlEditorMode::MODE_INSERT;
		auto sum = std::make_shared<Sum>(GetNextID());
		m_elementList.push_back(sum);
	} break;
	case ControlElementButtonID::ID_CONST: {
		m_mode = ControlEditorMode::MODE_INSERT;
		auto constant = std::make_shared<Constant>(GetNextID());
		m_elementList.push_back(constant);
	} break;
	case ControlElementButtonID::ID_LIMITER: {
		m_mode = ControlEditorMode::MODE_INSERT;
		auto limiter = std::make_shared<Limiter>(GetNextID());
		m_elementList.push_back(limiter);
	} break;
	case ControlElementButtonID::ID_GAIN: {
		m_mode = ControlEditorMode::MODE_INSERT;
		auto gain = std::make_shared<Gain>(GetNextID());
		m_elementList.push_back(gain);
	} break;
	case ControlElementButtonID::ID_MULT: {
		m_mode = ControlEditorMode::MODE_INSERT;
		auto mult = std::make_shared<Multiplier>(GetNextID());
		m_elementList.push_back(mult);
	} break;
	case ControlElementButtonID::ID_EXP: {
		m_mode = ControlEditorMode::MODE_INSERT;
		auto exp = std::make_shared<Exponential>(GetNextID());
		m_elementList.push_back(exp);
	} break;
	case ControlElementButtonID::ID_RATELIM: {
		m_mode = ControlEditorMode::MODE_INSERT;
		auto rateLim = std::make_shared<RateLimiter>(GetNextID());
		m_elementList.push_back(rateLim);
	} break;
	case ControlElementButtonID::ID_MATH_DIV: {
		m_mode = ControlEditorMode::MODE_INSERT;
		auto divider = std::make_shared<Divider>(GetNextID());
		m_elementList.push_back(divider);
	} break;
	case ControlElementButtonID::ID_MATH_EXPR: {
		m_mode = ControlEditorMode::MODE_INSERT;
		auto mathExpr = std::make_shared<MathExpression>(GetNextID());
		m_elementList.push_back(mathExpr);
	} break;
	}
	for (auto& cElement : m_elementList)
	{
		cElement->SetFont(m_font);
		cElement->StartMove(wxPoint2DDouble(0, 0));
		cElement->Move(wxPoint2DDouble(0, 0));

	}
}

void ControlEditor::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(m_cePanel);
	dc.Clear();
	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

	// Draw
	if (gc) {

		gc->Scale(m_camera->GetScale(), m_camera->GetScale());
		gc->Translate(m_camera->GetTranslation().m_x, m_camera->GetTranslation().m_y);

		for (auto line : m_connectionList) {
			//ConnectionLine* line = *it;
			line->DrawDC(m_properties->GetGUIColour(), m_camera->GetTranslation(), m_camera->GetScale(), gc);
		}

		for (auto element : m_elementList) {
			
			element->DrawDC(m_properties->GetGUIColour(), m_camera->GetTranslation(), m_camera->GetScale(), gc);
		}

		// Selection rectangle
		gc->SetPen(wxPen(m_properties->GetGUIColour()->selection));
		gc->SetBrush(wxBrush(m_properties->GetGUIColour()->selection));
		gc->DrawRectangle(m_selectionRect.m_x, m_selectionRect.m_y, m_selectionRect.m_width, m_selectionRect.m_height);

		delete gc;
	}
	event.Skip();
}

void ControlEditor::OnDoubleClick(wxMouseEvent& event)
{
	wxPoint2DDouble clickPoint = event.GetPosition();
	bool redraw = false;

	if (m_mode == ControlEditor::ControlEditorMode::MODE_EDIT) {
		for (auto& element : m_elementList) {
			if (element->Contains(m_camera->ScreenToWorld(clickPoint))) {
				element->ShowForm(this, element.get());
				CheckConnections();
				auto childList = element->GetChildList();
				for (auto itC = childList.begin(), itEndC = childList.end(); itC != itEndC; ++itC) {
					ConnectionLine* line = static_cast<ConnectionLine*>(*itC);
					line->UpdatePoints();
				}
				redraw = true;
				SaveCurrentState();
				break;
			}
		}
	}

	if (redraw) Redraw();
}

void ControlEditor::OnLeftClickDown(wxMouseEvent& event)
{
	wxPoint2DDouble clickPoint = event.GetPosition();
	bool foundElement = false;

	if (m_mode == ControlEditorMode::MODE_PASTE || m_mode == ControlEditorMode::MODE_DRAG_PASTE) {
		m_mode = ControlEditorMode::MODE_EDIT;
		SaveCurrentState();
	}
	else if (m_mode == ControlEditorMode::MODE_INSERT) {
		// Update the font of the last element in the list.
		//auto cElement = m_elementList.back();
		//cElement->SetFont(m_font);
		m_mode = ControlEditor::ControlEditorMode::MODE_EDIT;
	}
	else {
		for (auto& element : m_elementList) {
			bool foundNode = false;
			auto nodeList = element->GetNodeList();
			for (auto itN = nodeList.begin(), itNEnd = nodeList.end(); itN != itNEnd; ++itN) {
				Node* node = *itN;
				if (node->Contains(m_camera->ScreenToWorld(clickPoint))) {
					m_mode = ControlEditorMode::MODE_INSERT_LINE;
					auto line = std::make_shared<ConnectionLine>(node, GetNextID());
					m_connectionList.push_back(line);
					element->AddChild(line.get());
					line->AddParent(element.get());
					foundElement = true;
					foundNode = true;
				}
			}

			if (!foundNode) {
				// Set movement initial position (not necessarily will be moved).
				element->StartMove(m_camera->ScreenToWorld(clickPoint));

				// Click in an element.
				if (element->Contains(m_camera->ScreenToWorld(clickPoint))) {
					if (!foundElement) {
						element->SetSelected();
						foundElement = true;
					}
					m_mode = ControlEditor::ControlEditorMode::MODE_MOVE_ELEMENT;
				}
			}
		}
		if (m_mode != ControlEditorMode::MODE_INSERT_LINE) {
			for (auto& line : m_connectionList) {
				line->StartMove(m_camera->ScreenToWorld(clickPoint));
				if (line->Contains(m_camera->ScreenToWorld(clickPoint))) {
					line->SetSelected();
					foundElement = true;
					m_mode = ControlEditorMode::MODE_MOVE_LINE;
				}
			}
		}
	}

	if (!foundElement) {
		m_mode = ControlEditorMode::MODE_SELECTION_RECT;
		m_startSelRect = m_camera->ScreenToWorld(clickPoint);
	}

	Redraw();
	event.Skip();
}

void ControlEditor::OnLeftClickUp(wxMouseEvent& event)
{
	bool foundNode = false;
	bool saveCurrentState = false;

	for (auto& element : m_elementList) {
		if (m_mode == ControlEditorMode::MODE_INSERT_LINE) {
			auto nodeList = element->GetNodeList();
			for (auto node : nodeList) {
				if (node->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
					//ConnectionLine* line = *(m_connectionList.end() - 1);
					auto line = m_connectionList.back();
					if (line->AppendNode(node, element.get())) {
						line->AddParent(element.get());
						element->AddChild(line.get());
						line->UpdatePoints();
						m_mode = ControlEditor::ControlEditorMode::MODE_EDIT;
						foundNode = true;
						saveCurrentState = true;
					}
				}
			}
		}
		else if (m_mode == ControlEditorMode::MODE_SELECTION_RECT) {
			if (element->Intersects(m_selectionRect)) {
				element->SetSelected();
			}
			else if (!event.ControlDown()) {
				element->SetSelected(false);
			}
		}
		else if (!event.ControlDown()) {
			if (!element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) { element->SetSelected(false); }
		}
	}

	if (!m_connectionList.empty()) {
		auto& lastLine = m_connectionList.back();
		for (auto& cLine : m_connectionList) {
			if (m_mode == ControlEditorMode::MODE_INSERT_LINE && !foundNode && cLine.get() != lastLine.get()) {
				if (cLine->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
					//ConnectionLine* iLine = *(m_connectionList.end() - 1);
					auto iLine = m_connectionList.back();
					if (iLine->SetParentLine(cLine.get())) {
						cLine->AddChild(iLine.get());
						iLine->UpdatePoints();
						m_mode = ControlEditor::ControlEditorMode::MODE_EDIT;
						foundNode = true;
					}
				}
			}
			else if (m_mode == ControlEditorMode::MODE_SELECTION_RECT) {
				if (cLine->Intersects(m_selectionRect)) {
					cLine->SetSelected();
				}
				else if (!event.ControlDown()) {
					cLine->SetSelected(false);
				}
			}
			else if (!event.ControlDown()) {
				if (!cLine->Contains(m_camera->ScreenToWorld(event.GetPosition()))) { cLine->SetSelected(false); }
			}
		}
	}

	m_selectionRect = wxRect2DDouble(0, 0, 0, 0);

	if (m_mode == ControlEditorMode::MODE_INSERT_LINE && !foundNode) {
		//ConnectionLine* cLine = *(m_connectionList.end() - 1);
		auto cLine = m_connectionList.back();
		// Free nodes
		auto nodeList = cLine->GetNodeList();
		for (auto itN = nodeList.begin(), itEndN = nodeList.end(); itN != itEndN; ++itN) {
			Node* node = *itN;
			node->SetConnected(false);
		}
		// Remove the associated child from parents.
		auto parentList = cLine->GetParentList();
		for (auto it = parentList.begin(), itEnd = parentList.end(); it != itEnd; ++it) {
			Element* element = *it;
			element->RemoveChild(cLine.get());
		}
		m_connectionList.pop_back();
		//if (cLine) delete cLine;
		m_mode = ControlEditor::ControlEditorMode::MODE_EDIT;
	}
	else if (m_mode != ControlEditorMode::MODE_INSERT) {
		if (m_mode == ControlEditorMode::MODE_MOVE_ELEMENT || m_mode == ControlEditorMode::MODE_MOVE_LINE) {
			saveCurrentState = true;
		}
		m_mode = ControlEditor::ControlEditorMode::MODE_EDIT;
	}

	if (saveCurrentState) SaveCurrentState();

	Redraw();
	event.Skip();
}

void ControlEditor::OnMiddleDown(wxMouseEvent& event)
{
	// Set to drag mode.
	switch (m_mode) {
	case ControlEditorMode::MODE_INSERT: {
		m_mode = ControlEditorMode::MODE_DRAG_INSERT;
	} break;
	case ControlEditorMode::MODE_PASTE: {
		m_mode = ControlEditorMode::MODE_DRAG_PASTE;
	} break;
	default: {
		m_mode = ControlEditorMode::MODE_DRAG;
	} break;
	}
	m_camera->StartTranslation(GetEditorMouseWorldPoint());
}

void ControlEditor::OnMiddleUp(wxMouseEvent& event)
{
	switch (m_mode) {
	case ControlEditorMode::MODE_DRAG_INSERT: {
		m_mode = ControlEditorMode::MODE_INSERT;
	} break;
	case ControlEditorMode::MODE_DRAG_PASTE: {
		m_mode = ControlEditorMode::MODE_PASTE;
	} break;
	case ControlEditorMode::MODE_INSERT:
	case ControlEditorMode::MODE_PASTE: {
		// Does nothing.
	} break;
	default: {
		m_mode = ControlEditor::ControlEditorMode::MODE_EDIT;
	} break;
	}
}

void ControlEditor::OnMouseMotion(wxMouseEvent& event)
{
	wxPoint2DDouble clickPoint = event.GetPosition();
	bool redraw = false;

	switch (m_mode) {
	case ControlEditorMode::MODE_INSERT: {
		//Element* newElement = *(m_elementList.end() - 1);  // Get the last element in the list.
		auto newElement = m_elementList.back();
		newElement->Move(m_camera->ScreenToWorld(clickPoint));
		redraw = true;
	} break;
	case ControlEditorMode::MODE_INSERT_LINE: {
		//ConnectionLine* line = *(m_connectionList.end() - 1);
		auto line = m_connectionList.back();
		line->SetTemporarySecondPoint(m_camera->ScreenToWorld(clickPoint));
		line->UpdatePoints();
		redraw = true;
	} break;
	case ControlEditorMode::MODE_DRAG:
	case ControlEditorMode::MODE_DRAG_INSERT:
	case ControlEditorMode::MODE_DRAG_PASTE: {
		m_camera->SetTranslation(clickPoint);
		redraw = true;
	} break;
	case ControlEditorMode::MODE_PASTE: {
		bool movedSelectedElement = false;
		for (auto& element : m_elementList) {
			if (element->IsSelected()) {
				movedSelectedElement = true;
				element->Move(m_camera->ScreenToWorld(clickPoint));
				auto childList = element->GetChildList();
				for (Element* child : childList) {
					if (auto line = dynamic_cast<ConnectionLine*>(child)) line->UpdatePoints();
				}
				redraw = true;
			}
		}
		// Only move lines directly when no element is selected; otherwise line movement would
		// be applied twice and corrupt offsets.
		if (!movedSelectedElement) {
			for (auto& line : m_connectionList) {
				if (line->IsSelected()) {
					line->Move(m_camera->ScreenToWorld(clickPoint));
					redraw = true;
				}
			}
		}
	} break;
	case ControlEditor::ControlEditorMode::MODE_MOVE_ELEMENT: {
		for (auto& element : m_elementList) {
			if (element->IsSelected()) {
				element->Move(m_camera->ScreenToWorld(clickPoint));
				auto childList = element->GetChildList();
				for (auto itC = childList.begin(), itEndC = childList.end(); itC != itEndC; itC++) {
					ConnectionLine* line = static_cast<ConnectionLine*>(*itC);
					line->UpdatePoints();
				}
				redraw = true;
			}
		}
	} break;
	case ControlEditorMode::MODE_MOVE_LINE: {
		for (auto& line : m_connectionList) {
			if (line->IsSelected()) {
				line->Move(m_camera->ScreenToWorld(clickPoint));
				redraw = true;
			}
		}
	} break;
	case ControlEditorMode::MODE_SELECTION_RECT: {
		wxPoint2DDouble currentPos = m_camera->ScreenToWorld(clickPoint);
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
	default:
		break;
	}

	m_camera->UpdateMousePosition(clickPoint);
	if (redraw) Redraw();
	event.Skip();
}

void ControlEditor::OnScroll(wxMouseEvent& event)
{
	if (event.GetWheelRotation() > 0)
		m_camera->SetScale(event.GetPosition(), +0.05);
	else
		m_camera->SetScale(event.GetPosition(), -0.05);

	Redraw();
}

void ControlEditor::OnIdle(wxIdleEvent& event)
{
	//if(m_justOpened) {
	//    this->Raise();
	//
	//    // Update all text elements
	//    m_justOpened = false;
	//    for(auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
	//        ControlElement* element = *it;
	//        if(!element->UpdateText()) m_justOpened = true;
	//    }
	//    Redraw();
	//}
}
void ControlEditor::OnKeyDown(wxKeyEvent& event)
{
	bool hasInsertInProgress = (m_mode == ControlEditorMode::MODE_INSERT ||
		m_mode == ControlEditorMode::MODE_INSERT_LINE);
	char key = event.GetUnicodeKey();
	if (key != WXK_NONE) {
		switch (key) {
		case WXK_DELETE:  // Delete selected elements.
		{
			if (!hasInsertInProgress) {
				wxCommandEvent dummyEvent;
				OnDeleteClick(dummyEvent);
			}
		} break;
		case 'R':  // Rotate the selected elements.
		{
			RotateSelectedElements(event.GetModifiers() != wxMOD_SHIFT);
			SaveCurrentState();
		} break;
		case 'C': {
			if (!hasInsertInProgress && event.GetModifiers() == wxMOD_CONTROL) {
				wxCommandEvent dummyEvent;
				OnCopyClick(dummyEvent);
			}
		} break;
		case 'V': {
			if (!hasInsertInProgress && event.GetModifiers() == wxMOD_CONTROL) {
				wxCommandEvent dummyEvent;
				OnPasteClick(dummyEvent);
			}
		} break;
		case 'Z': {
			if (!hasInsertInProgress && event.ControlDown() && !event.ShiftDown()) {
				wxCommandEvent dummyEvent;
				OnUndoClick(dummyEvent);
			}
			if (!hasInsertInProgress && event.ControlDown() && event.ShiftDown()) {
				wxCommandEvent dummyEvent;
				OnRedoClick(dummyEvent);
			}
		} break;
		case 'Y': {
			if (!hasInsertInProgress && event.GetModifiers() == wxMOD_CONTROL) {
				wxCommandEvent dummyEvent;
				OnRedoClick(dummyEvent);
			}
		} break;
		case 'N': {
			if (!hasInsertInProgress && event.GetModifiers() == wxMOD_CONTROL) {
				wxCommandEvent dummyEvent;
				OnNewClick(dummyEvent);
			}
		} break;
		case 'L': {
			// tests
		} break;
		}
	}
}

void ControlEditor::RotateSelectedElements(bool clockwise)
{
	for (auto& element : m_elementList) {
		if (element->IsSelected()) {
			element->Rotate(clockwise);
			auto childList = element->GetChildList();
			for (auto itC = childList.begin(), itEndC = childList.end(); itC != itEndC; itC++) {
				ConnectionLine* line = static_cast<ConnectionLine*>(*itC);
				line->UpdatePoints();
			}
		}
	}
	Redraw();
}

void ControlEditor::DeleteSelectedElements()
{
	for (auto it = m_elementList.begin(); it != m_elementList.end();) {
		Element* element = it->get();
		if (element->IsSelected()) {
			// Remove child/parent.
			auto childList = element->GetChildList();
			for (auto child : childList) {
				// The child is always a connection line, but check it.
				//ConnectionLine* child = static_cast<ConnectionLine*>(*itC);
				if (auto childLine = dynamic_cast<ConnectionLine*>(child)) {
					// Delete the connection line.
					for (auto itCo = m_connectionList.begin(); itCo != m_connectionList.end(); ) {
						if (itCo->get() == childLine)
							itCo = DeleteLineFromList(itCo);
						else
							++itCo;
					}
				}

			}
			m_elementList.erase(it);
			//if (element) delete element;
		}
		else {
			++it;
		}
	}

	for (auto it = m_connectionList.begin(); it != m_connectionList.end(); ) {
		ConnectionLine* line = it->get();
		if (line->IsSelected())
			it = DeleteLineFromList(it);
		else
			++it;
	}
	Redraw();
}

std::vector< std::shared_ptr<ConnectionLine> >::iterator ControlEditor::DeleteLineFromList(std::vector< std::shared_ptr<ConnectionLine> >::iterator& it)
{
	ConnectionLine* cLine = it->get();

	// Delete children recursively
	auto childList = cLine->GetLineChildList();
	for (auto child : childList) {
		for (auto itL = m_connectionList.begin(); itL != m_connectionList.end(); ) {
			if (itL->get() == child)
				itL = DeleteLineFromList(itL);
			else
				++itL;
		}
	}
	// Remove parents
	auto parentList = cLine->GetParentList();
	for (auto parent : parentList) {
		if (parent) parent->RemoveChild(cLine);
	}
	if (cLine->GetParentLine()) cLine->GetParentLine()->RemoveChild(cLine);

	// Free nodes
	auto nodeList = cLine->GetNodeList();
	for (auto node : nodeList)
		node->SetConnected(false);

	return m_connectionList.erase(it);
}

void ControlEditor::CheckConnections()
{
	for (auto it = m_connectionList.begin(); it != m_connectionList.end(); ++it) {
		ConnectionLine* cLine = it->get();
		if (cLine->GetType() == ConnectionLine::ConnectionLineType::ELEMENT_ELEMENT) {
			if (cLine->GetParentList().size() < 2) { it = DeleteLineFromList(it); }
		}
		else if (cLine->GetParentList().size() < 1) {
			it = DeleteLineFromList(it);
		}
	}
}

void ControlEditor::SetElementsList(const std::vector<std::shared_ptr<ControlElement>>& elementList)
{
	m_elementList = elementList;
	for (auto& cElement : m_elementList)
	{
		cElement->SetFont(m_font);
	}
}

void ControlEditor::OnExportClick(wxCommandEvent& event)
{
	FileHanding fileHandling(this);

	wxFileDialog saveFileDialog(this, _("Save CTL file"), "", "", "CTL files (*.ctl)|*.ctl",
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

	fileHandling.SaveControl(saveFileDialog.GetPath());
	wxFileName fileName(saveFileDialog.GetPath());
	event.Skip();
}

void ControlEditor::OnImportClick(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, _("Open CTL file"), "", "", "CTL files (*.ctl)|*.ctl",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL) return;

	wxFileName fileName(openFileDialog.GetPath());

	FileHanding fileHandling(this);
	if (!fileHandling.OpenControl(fileName, m_elementList, m_connectionList)) {
		wxMessageDialog msgDialog(this, _("It was not possible to open the selected file."), _("Error"),
			wxOK | wxCENTRE | wxICON_ERROR);
		msgDialog.ShowModal();
	}
	for (auto cElement : m_elementList)
	{
		cElement->SetFont(m_font);
	}
	Redraw();
	event.Skip();
}

void ControlEditor::OnTestClick(wxCommandEvent& event)
{
	// Reset colour list position for the test plot.
	m_itColourList = --m_colourList.end();

	std::vector<IOControl*> ioList;

	for (auto& element : m_elementList) {
		if (auto io = dynamic_cast<IOControl*>(element.get())) {
			ioList.push_back(io);
		}
	}

	ControlSystemTest csTest(this, ioList, &m_inputType, &m_startTime, &m_slope, &m_timeStep, &m_simTime);
	if (csTest.ShowModal() == wxID_OK) {
		double printStep = 1e-3;
		double pdbStep = 1e-1;

		struct InputData {
			wxString name;
			std::vector<double> values;
		};

		std::vector<InputData> inputList;

		// Store real flags for restoring after the test and set initial value.
		std::vector<IOControl::IOFlags> realFlagValue;
		for (auto* io : ioList) {
			realFlagValue.push_back(io->GetValue());
			if (io->GetType() == Node::NodeType::NODE_OUT) {
				SimTestData testData = io->GetSimTestData();
				io->SetValue(IOControl::IN_TEST);
				io->SetTestValue(testData.initialValue);
				inputList.push_back({ io->GetName(), {} });
			}
		}

		wxProgressDialog pbd(_("Test"), _("Initializing..."), 100, this,
			wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_CAN_ABORT | wxPD_SMOOTH);
		ControlElementSolver solver(this, m_timeStep, 1e-5);
		solver.InitializeValues(false);
		if (solver.IsOK()) {
			bool simStopped = false;
			double currentTime = 0.0;
			double printTime = 0.0;
			double pdbTime = 0.0;
			std::vector<double> time;
			std::vector<double> solution;

			while (currentTime <= m_simTime) {
				for (auto* io : ioList) {
					SimTestData testData = io->GetSimTestData();
					if (currentTime >= testData.startTime) {
						switch (testData.type) {
						case 0: {
							io->SetTestValue(testData.slope);
						} break;
						case 1: {
							io->SetTestValue(testData.slope * (currentTime - testData.startTime));
						} break;
						case 2: {
							io->SetTestValue(testData.slope * std::pow(currentTime - testData.startTime, 2));
						} break;
						default: {
							io->SetTestValue(0.0);
							break;
						}
						}
					}
				}

				solver.SetCurrentTime(currentTime);
				solver.SolveNextStep();
				if (!solver.IsOK()) {
					wxString msg = wxString::Format(_("Failed to solve the control system.\n%s"), solver.GetErrorMessage());
					wxMessageDialog msgDialog(this, msg, _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
					msgDialog.ShowModal();
					simStopped = true;
					currentTime = m_simTime;
				}

				if (printTime >= printStep) {
					time.push_back(currentTime);
					solution.push_back(solver.GetLastSolution());
					//inputV.push_back(input);
					int i = 0;
					for (auto* io : ioList) {
						if (io->GetType() == Node::NodeType::NODE_OUT) {
							inputList[i].values.push_back(io->GetTestValue());
							i++;
						}
					}
					printTime = 0.0;
				}

				if (pdbTime > pdbStep) {
					if (!pbd.Update((currentTime / m_simTime) * 100, wxString::Format("Time = %.2fs", currentTime))) {
						pbd.Update(100);
						simStopped = true;
						currentTime = m_simTime;
					}
					pdbTime = 0.0;
				}

				printTime += m_timeStep;
				currentTime += m_timeStep;
				pdbTime += m_timeStep;
			}

			if (!simStopped) {
				std::vector<ElementPlotData> epdList;
				ElementPlotData curveData(_("I/O"), ElementPlotData::CurveType::CT_TEST);
				int i = 0;
				for (const auto& input : inputList) {
					curveData.AddData(input.values, input.name);
					curveData.SetPlot(i);
					curveData.SetColour(i, GetNextColour());
					i++;
				}
				//curveData.AddData(inputV, _("Input"));
				curveData.AddData(solution, _("Output"));
				curveData.SetPlot(i);
				curveData.SetColour(i, GetNextColour());

				//curveData.SetPlot(0);
				//curveData.SetColour(0, *wxRED);
				//curveData.SetPlot(1);
				//curveData.SetColour(1, *wxBLUE);

				epdList.push_back(curveData);

				ChartView* cView = new ChartView(this, epdList, time, static_cast<PlotLib>(m_plotLib));
				cView->Show();
				cView->UpdatePlot();
			}
		}
		else {
			wxMessageDialog msgDialog(
				this,
				wxString::Format(_("Failed to solve the control system.\n%s"), solver.GetErrorMessage()),
				_("Error"),
				wxOK | wxCENTRE | wxICON_ERROR
			);

			msgDialog.ShowModal();
		}

		// Restore real flags.
		for (size_t i = 0; i < ioList.size(); i++) {
			if (ioList[i]->GetType() == Node::NodeType::NODE_OUT)
				ioList[i]->SetValue(realFlagValue[i]);
		}
	}
}

std::vector<Element*> ControlEditor::GetElementList() const
{
	std::vector<Element*> elementList;
	for (auto& element : m_elementList) {
		elementList.push_back(element.get());
	}
	return elementList;
}

void ControlEditor::Fit()
{
	wxPoint2DDouble leftUpCorner(0, 0);
	wxPoint2DDouble rightDownCorner(0, 0);
	std::vector<Element*> elementList = GetElementList();

	if (!GetElementsCorners(leftUpCorner, rightDownCorner, elementList)) return;
	wxPoint2DDouble middleCoords = (leftUpCorner + rightDownCorner) / 2.0;

	int width = 0.0;
	int height = 0.0;
	m_cePanel->GetSize(&width, &height);

	const double scaleX = static_cast<double>(width) / (rightDownCorner.m_x - leftUpCorner.m_x);
	const double scaleY = static_cast<double>(height) / (rightDownCorner.m_y - leftUpCorner.m_y);

	double scale = scaleX < scaleY ? scaleX : scaleY;
	scale = std::min(scale, m_camera->GetZoomMax());
	scale = std::max(scale, m_camera->GetZoomMin());

	m_camera->SetScale(scale);

	m_camera->StartTranslation(middleCoords);
	m_camera->SetTranslation(wxPoint2DDouble(width / 2.0, height / 2.0));

	Redraw();
}

void ControlEditor::OnClose(wxCloseEvent& event)
{
	if (m_ctrlContainer) { m_ctrlContainer->FillContainer(this); }
	ClearStates();
	if (m_ctrlManager)
		m_ctrlManager->Remove(this);
	Destroy();
	//event.Skip();
}

void ControlEditor::UnselectAll()
{
	for (auto& element : m_elementList) element->SetSelected(false);
	for (auto& line : m_connectionList) line->SetSelected(false);
}

void ControlEditor::ApplyFontToElements()
{
	for (auto& element : m_elementList) {
		element->SetFont(m_font);
	}
}

bool ControlEditor::GetElementsCorners(wxPoint2DDouble& leftUpCorner,
	wxPoint2DDouble& rightDownCorner,
	const std::vector<Element*>& elementList) const
{
	if (elementList.empty()) return false;
	elementList[0]->CalculateBoundaries(leftUpCorner, rightDownCorner);
	for (size_t i = 1; i < elementList.size(); i++) {
		wxPoint2DDouble lu, rd;
		elementList[i]->CalculateBoundaries(lu, rd);
		if (lu.m_x < leftUpCorner.m_x) leftUpCorner.m_x = lu.m_x;
		if (lu.m_y < leftUpCorner.m_y) leftUpCorner.m_y = lu.m_y;
		if (rd.m_x > rightDownCorner.m_x) rightDownCorner.m_x = rd.m_x;
		if (rd.m_y > rightDownCorner.m_y) rightDownCorner.m_y = rd.m_y;
	}
	return true;
}

void ControlEditor::ClearStates()
{
	m_elementListState.clear();
	m_connectionListState.clear();
	m_currentState = -1;
}

void ControlEditor::SaveCurrentState()
{
	std::vector<std::shared_ptr<ControlElement>> stateElements;
	std::vector<std::shared_ptr<ConnectionLine>> stateLines;

	// Deep copy via ControlElementContainer (same remapping logic used by generators).
	ControlElementContainer container;
	container.FillContainer(m_elementList, m_connectionList);
	container.GetContainerCopy(stateElements, stateLines);

	// Drop redo states
	m_elementListState.resize(m_currentState + 1);
	m_connectionListState.resize(m_currentState + 1);

	m_currentState++;
	if (m_currentState >= m_maxStates) {
		m_currentState = m_maxStates - 1;
		m_elementListState.erase(m_elementListState.begin());
		m_connectionListState.erase(m_connectionListState.begin());
	}

	m_elementListState.emplace_back(std::move(stateElements));
	m_connectionListState.emplace_back(std::move(stateLines));
}

void ControlEditor::SetPreviousState()
{
	m_currentState--;
	if (m_currentState < 0) {
		m_currentState = 0;
		return;
	}
	if (static_cast<size_t>(m_currentState) >= m_elementListState.size() ||
		static_cast<size_t>(m_currentState) >= m_connectionListState.size()) {
		return;
	}

	std::vector<std::shared_ptr<ControlElement>> newElements;
	std::vector<std::shared_ptr<ConnectionLine>> newLines;
	ControlElementContainer container;
	container.FillContainer(m_elementListState[m_currentState], m_connectionListState[m_currentState]);
	container.GetContainerCopy(newElements, newLines);

	m_elementList = std::move(newElements);
	m_connectionList = std::move(newLines);
	ApplyFontToElements();
	Redraw();
}

void ControlEditor::SetNextState()
{
	m_currentState++;
	if (m_currentState < 0) m_currentState = 0;
	if (static_cast<size_t>(m_currentState) >= m_elementListState.size() ||
		static_cast<size_t>(m_currentState) >= m_connectionListState.size()) {
		m_currentState--;
		return;
	}

	std::vector<std::shared_ptr<ControlElement>> newElements;
	std::vector<std::shared_ptr<ConnectionLine>> newLines;
	ControlElementContainer container;
	container.FillContainer(m_elementListState[m_currentState], m_connectionListState[m_currentState]);
	container.GetContainerCopy(newElements, newLines);

	m_elementList = std::move(newElements);
	m_connectionList = std::move(newLines);
	ApplyFontToElements();
	Redraw();
}

void ControlEditor::CopySelectionToClipboard()
{
	std::vector<std::shared_ptr<ControlElement>> selectedElements;
	std::vector<std::shared_ptr<ConnectionLine>> selectedLines;

	for (auto& el : m_elementList) {
		if (el->IsSelected()) selectedElements.push_back(el);
	}
	for (auto& ln : m_connectionList) {
		if (ln->IsSelected()) selectedLines.push_back(ln);
	}

	// If user selected only elements, include the connection lines between them (and children) to keep the
	// copied system consistent.
	if (!selectedElements.empty() && selectedLines.empty()) {
		std::unordered_set<Element*> selectedSet;
		selectedSet.reserve(selectedElements.size());
		for (auto& el : selectedElements) selectedSet.insert(el.get());

		for (auto& ln : m_connectionList) {
			auto parents = ln->GetParentList();
			if (parents.size() >= 2 &&
				selectedSet.contains(parents[0]) &&
				selectedSet.contains(parents[1])) {
				selectedLines.push_back(ln);
			}
		}
	}

	// Deep copy into clipboard payload
	ControlElementContainer container;
	container.FillContainer(selectedElements, selectedLines);
	std::vector<std::shared_ptr<ControlElement>> copyElements;
	std::vector<std::shared_ptr<ConnectionLine>> copyLines;
	container.GetContainerCopy(copyElements, copyLines);
	SanitizeControlGraph(copyElements, copyLines);

	auto* dataObject = new ControlElementDataObject(copyElements, copyLines);
	if (wxTheClipboard->Open()) {
		wxTheClipboard->SetData(dataObject);
		wxTheClipboard->Close();
	}
}

bool ControlEditor::PasteFromClipboard()
{
	if (!wxTheClipboard->Open()) {
		wxMessageDialog dialog(this, _("It was not possible to paste from clipboard."), _("Error"),
			wxOK | wxCENTER | wxICON_ERROR, wxDefaultPosition);
		dialog.ShowModal();
		return false;
	}

	ControlElementDataObject dataObject;
	if (!wxTheClipboard->IsSupported(dataObject.GetFormat())) {
		wxTheClipboard->Close();
		return false;
	}
	if (!wxTheClipboard->GetData(dataObject)) {
		wxMessageDialog dialog(this, _("It was not possible to paste from clipboard."), _("Error"),
			wxOK | wxCENTER | wxICON_ERROR, wxDefaultPosition);
		dialog.ShowModal();
		wxTheClipboard->Close();
		return false;
	}
	wxTheClipboard->Close();

	auto* lists = dataObject.GetElementsLists();
	if (!lists) return false;

	// Deep copy again (clipboard payload must remain immutable)
	std::vector<std::shared_ptr<ControlElement>> pastedElements;
	std::vector<std::shared_ptr<ConnectionLine>> pastedLines;
	ControlElementContainer container;
	container.FillContainer(lists->elementList, lists->connectionList);
	container.GetContainerCopy(pastedElements, pastedLines);
	SanitizeControlGraph(pastedElements, pastedLines);

	if (pastedElements.empty() && pastedLines.empty()) return false;

	UnselectAll();

	// Assign new unique IDs
	for (auto& el : pastedElements) {
		el->SetID(GetNextID());
		el->SetSelected(true);
	}
	for (auto& ln : pastedLines) {
		ln->SetID(GetNextID());
		ln->SetSelected(true);
	}

	// Insert
	for (auto& el : pastedElements) {
		el->SetFont(m_font);
		m_elementList.push_back(el);
	}
	for (auto& ln : pastedLines) {
		m_connectionList.push_back(ln);
		ln->UpdatePoints();
	}

	// Move the pasted objects to the mouse position (center them at mouse, like Workspace).
	wxPoint2DDouble mouseWorld = GetEditorMouseWorldPoint();

	std::vector<Element*> pastedAsElements;
	pastedAsElements.reserve(pastedElements.size() + pastedLines.size());
	for (auto& el : pastedElements) pastedAsElements.push_back(el.get());
	for (auto& ln : pastedLines) pastedAsElements.push_back(ln.get());

	wxPoint2DDouble leftUpCorner, rightDownCorner;
	if (GetElementsCorners(leftUpCorner, rightDownCorner, pastedAsElements)) {
		wxPoint2DDouble startPosition = (leftUpCorner + rightDownCorner) / 2.0;
		for (auto& e : pastedElements) {
			e->StartMove(startPosition);
			e->Move(mouseWorld);
		}
		// Ensure lines follow moved elements; do not move lines directly to avoid offset corruption.
		for (auto& ln : pastedLines) ln->UpdatePoints();
	}

	m_mode = ControlEditorMode::MODE_PASTE;
	Redraw();
	return true;
}

int ControlEditor::GetNextID()
{
	int id = 0;
	for (auto& element : m_elementList) {
		if (element->GetID() > id) id = element->GetID();
	}
	for (auto& line : m_connectionList) {
		if (line->GetID() > id) id = line->GetID();
	}
	id++;
	return id;
}

void ControlEditor::BuildColourList()
{
	m_colourList.push_back(wxColour(255, 30, 0));
	m_colourList.push_back(wxColour(0, 30, 255));
	m_colourList.push_back(wxColour(0, 128, 0));
	m_colourList.push_back(wxColour(100, 100, 100));
	m_colourList.push_back(wxColour(255, 128, 0));
	m_colourList.push_back(wxColour(128, 0, 255));
	m_colourList.push_back(wxColour(0, 255, 128));
	m_colourList.push_back(wxColour(255, 255, 0));
	m_colourList.push_back(wxColour(255, 0, 255));
	m_colourList.push_back(wxColour(0, 255, 255));
	m_colourList.push_back(wxColour(128, 255, 0));
	m_colourList.push_back(wxColour(255, 0, 128));
	m_colourList.push_back(wxColour(0, 128, 255));
	m_colourList.push_back(wxColour(128, 128, 128));
	m_colourList.push_back(*wxBLACK);
	m_itColourList = --m_colourList.end();
}

wxColour ControlEditor::GetNextColour()
{
	if (*m_itColourList == *wxBLACK)
		m_itColourList = m_colourList.begin();
	else
		++m_itColourList;

	return *m_itColourList;
}
void ControlEditor::OnCopyClick(wxCommandEvent& event)
{
	CopySelectionToClipboard();
}
void ControlEditor::OnDeleteClick(wxCommandEvent& event)
{
	DeleteSelectedElements();
	SaveCurrentState();
}
void ControlEditor::OnDragClick(wxCommandEvent& event)
{
	// Same behavior as middle mouse drag: anchor at current mouse, pan only on motion (no jump).
	m_mode = ControlEditorMode::MODE_DRAG;
	m_camera->StartTranslation(GetEditorMouseWorldPoint());
	if (m_cePanel) m_cePanel->SetFocus();
}
void ControlEditor::OnMoveClick(wxCommandEvent& event)
{
	bool hasSelectedElement = false;
	bool hasSelectedLine = false;
	std::vector<Element*> selectedObjects;

	for (auto& element : m_elementList) {
		if (element->IsSelected()) {
			hasSelectedElement = true;
			selectedObjects.push_back(element.get());
		}
	}
	for (auto& line : m_connectionList) {
		if (line->IsSelected()) {
			hasSelectedLine = true;
			selectedObjects.push_back(line.get());
		}
	}
	if (!hasSelectedElement && !hasSelectedLine) return;

	m_mode = hasSelectedElement ? ControlEditorMode::MODE_MOVE_ELEMENT : ControlEditorMode::MODE_MOVE_LINE;

	wxPoint2DDouble mouseWorld = GetEditorMouseWorldPoint();

	wxPoint2DDouble leftUp, rightDown;
	if (!GetElementsCorners(leftUp, rightDown, selectedObjects)) return;
	wxPoint2DDouble startWorld = (leftUp + rightDown) / 2.0;

	if (hasSelectedElement) {
		for (auto& element : m_elementList) {
			if (element->IsSelected()) {
				element->StartMove(startWorld);
				element->Move(mouseWorld);
				auto childList = element->GetChildList();
				for (Element* child : childList) {
					if (auto line = dynamic_cast<ConnectionLine*>(child)) line->UpdatePoints();
				}
			}
		}
	}
	else {
		for (auto& line : m_connectionList) {
			if (line->IsSelected()) {
				line->StartMove(startWorld);
				line->Move(mouseWorld);
			}
		}
	}
	Redraw();
}
void ControlEditor::OnNewClick(wxCommandEvent& event)
{
	wxMessageDialog msgDialog(this,
		_("Do you want to create a new control system?\n\nAll elements will be removed."),
		_("Warning"),
		wxYES_NO | wxCENTRE | wxICON_WARNING);
	if (msgDialog.ShowModal() != wxID_YES) return;

	m_elementList.clear();
	m_connectionList.clear();
	m_selectionRect = wxRect2DDouble(0, 0, 0, 0);
	m_mode = ControlEditorMode::MODE_EDIT;
	ClearStates();
	SaveCurrentState();
	Redraw();
}
void ControlEditor::OnPasteClick(wxCommandEvent& event)
{
	PasteFromClipboard();
}
void ControlEditor::OnRedoClick(wxCommandEvent& event)
{
	SetNextState();
}
void ControlEditor::OnUndoClick(wxCommandEvent& event)
{
	SetPreviousState();
}
void ControlEditor::OnFitClick(wxCommandEvent& event)
{
	Fit();
}
void ControlEditor::OnMiddleDoubleClick(wxMouseEvent& event)
{
	Fit();
	event.Skip();
}
