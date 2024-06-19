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

#include "ChartView.h"
#include "../utils/ElementPlotData.h"

#include <sstream>
#include <wx/mstream.h>

ChartView::ChartView(wxWindow* parent, std::vector<ElementPlotData> epdList, std::vector<double> time, PlotLib plotLib)
	: ChartViewBase(parent)
{
	m_epdList = epdList;
	m_time = time;
	m_xAxisValues = time;
	m_plotLib = plotLib;

	m_menuItemShowGrid->Check(m_hideGrid ? false : true);
	m_menuItemShowLabel->Check(m_showLeg);
	m_menuItemShowCoordinates->Check(m_showCoords);
	m_menuItemDarkTheme->Check(m_darkTheme);

	// Create color property.
	m_pgPropColor = m_pgMgr->Insert(m_pgPropLineProp, 1, new wxColourProperty(_("Color")));
	m_pgPropColor->SetEditor(wxT("ChoiceAndButton"));
	m_pgPropColor->SetValue(static_cast<wxVariant>(static_cast<wxAny>(*wxBLACK)));

	// Set margins and axis limit to composed mode.
	m_pgPropMargins->SetValue(wxT("<composed>"));
	m_pgMgr->Collapse(m_pgPropMargins);
	m_pgPropAxisLimit->SetValue(wxT("<composed>"));
	m_pgMgr->Collapse(m_pgPropAxisLimit);

	// Add line type choices
	m_pgProplineType->AddChoice(_("Solid"), wxPENSTYLE_SOLID);
	m_pgProplineType->AddChoice(_("Dot"), wxPENSTYLE_DOT);
	m_pgProplineType->AddChoice(_("Dash"), wxPENSTYLE_SHORT_DASH);
	m_pgProplineType->AddChoice(_("Dot and dash"), wxPENSTYLE_DOT_DASH);
	if (m_plotLib == PlotLib::wxMATH_PLOT) {
		m_pgProplineType->AddChoice(_("Cross"), wxPENSTYLE_CROSS_HATCH);
		m_pgProplineType->AddChoice(_("Driagonal cross"), wxPENSTYLE_CROSSDIAG_HATCH);
	}

	if (m_plotLib == PlotLib::wxMATH_PLOT) {
		SetMPWindow();
		GetSizer()->Add(m_mpWindow, 1, wxEXPAND, WXC_FROM_DIP(5));
	}
	else if (m_plotLib == PlotLib::wxCHART_DIR) {
		m_chartViewerDir = new wxChartViewer(this, ID_CHARTVIEWER);
		m_chartViewerDir->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
		m_chartViewerDir->SetMinSize(wxSize(400, 300));
		m_chartViewerDir->setMouseUsage(Chart::MouseUsageZoomIn);
		m_chartViewerDir->setSelectionBorderWidth(1);
		m_chartViewerDir->setSelectionBorderColor(wxColour(0, 125, 255, 255));
		m_chartViewerDir->setMouseWheelZoomRatio(1.1);
		m_chartViewerDir->setZoomDirection(Chart::DirectionHorizontalVertical);
		m_chartViewerDir->setScrollDirection(Chart::DirectionHorizontalVertical);
		GetSizer()->Add(m_chartViewerDir, 1, wxEXPAND, WXC_FROM_DIP(5));

		this->Bind(wxEVT_CHARTVIEWER_VIEWPORT_CHANGED, &ChartView::OnViewPortChanged, this, ID_CHARTVIEWER);
		this->Bind(wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA, &ChartView::OnMouseMovePlotArea, this, ID_CHARTVIEWER);
		m_chartViewerDir->Bind(wxEVT_RIGHT_DOWN, &ChartView::OnRightClickDown, this);

		m_pgPropMarginsUp->SetValue(10);
		m_pgPropMarginsLeft->SetValue(50);
		m_pgPropMarginsRight->SetValue(40);
		m_pgPropMarginsBot->SetValue(40);

		m_pgPropXLabel->SetValue(_("auto"));
		m_pgPropYLabel->SetValue(_("auto"));
	}
	SetTreectrl();
	Layout();
	SetInitialSize();

	BuildColourList();

	//if (m_plotLib == PlotLib::wxCHART_DIR) {
	//	DrawChartDir();
	//}
}

ChartView::~ChartView()
{
	if (m_plotLib == PlotLib::wxCHART_DIR) {
		this->Unbind(wxEVT_CHARTVIEWER_VIEWPORT_CHANGED, &ChartView::OnViewPortChanged, this, ID_CHARTVIEWER);
		this->Unbind(wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA, &ChartView::OnMouseMovePlotArea, this, ID_CHARTVIEWER);
		m_chartViewerDir->Unbind(wxEVT_RIGHT_DOWN, &ChartView::OnRightClickDown, this);
	}
}
void ChartView::SetMPWindow()
{
	m_mpWindow = new mpWindow(this, wxID_ANY);

	m_mpWindow->SetDoubleBuffered(true);

	m_mpWindow->SetMargins(20, 10, 40, 60);
	m_xaxis = new mpScaleX("", mpALIGN_BOTTOM, true);
	m_yaxis = new mpScaleY("", mpALIGN_LEFT, true);
	m_xaxis->SetDrawOutsideMargins(false);
	m_yaxis->SetDrawOutsideMargins(false);
	m_xaxis->SetTicks(m_hideGrid);
	m_yaxis->SetTicks(m_hideGrid);

	m_leg = new mpInfoLegend(wxRect(200, 20, 40, 40), wxWHITE_BRUSH);
	m_coords = new mpInfoCoords(wxRect(0, 0, 0, 0), wxWHITE_BRUSH);

	m_chartTitle = new mpText("", 50, 0);
	wxFont chartTitleFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	m_chartTitle->SetFont(chartTitleFont);

	m_mpWindow->AddLayer(m_xaxis);
	m_mpWindow->AddLayer(m_yaxis);
	m_mpWindow->AddLayer(m_leg);
	m_mpWindow->AddLayer(m_coords);
	m_mpWindow->AddLayer(m_chartTitle);

	m_leg->SetVisible(m_showLeg);
	m_coords->SetVisible(m_showCoords);

	m_mpWindow->EnableDoubleBuffer(true);
	m_mpWindow->LockAspect(false);
	Fit();
}

void ChartView::SetTreectrl()
{
	wxTreeItemId rootID = m_treeCtrl->AddRoot(wxT("root"));
	m_treeTimeID = m_treeCtrl->AppendItem(rootID, _("Time"));
	m_treeCtrl->SetItemTextColour(m_treeTimeID, *wxRED);

	bool firstElement[static_cast<unsigned int>(ElementPlotData::CurveType::NUM_ELEMENTS)];
	for (int i = 0; i < static_cast<unsigned int>(ElementPlotData::CurveType::NUM_ELEMENTS); ++i) firstElement[i] = true;

	wxString rootElementName[static_cast<unsigned int>(ElementPlotData::CurveType::NUM_ELEMENTS)];
	rootElementName[static_cast<unsigned int>(ElementPlotData::CurveType::CT_BUS)] = _("Bus");
	rootElementName[static_cast<unsigned int>(ElementPlotData::CurveType::CT_IND_MOTOR)] = _("Induction motor");
	rootElementName[static_cast<unsigned int>(ElementPlotData::CurveType::CT_LINE)] = _("Line");
	rootElementName[static_cast<unsigned int>(ElementPlotData::CurveType::CT_LOAD)] = _("Load");
	rootElementName[static_cast<unsigned int>(ElementPlotData::CurveType::CT_SHUNT_CAPACITOR)] = _("Capacitor");
	rootElementName[static_cast<unsigned int>(ElementPlotData::CurveType::CT_SHUNT_INDUCTOR)] = _("Inductor");
	rootElementName[static_cast<unsigned int>(ElementPlotData::CurveType::CT_SYNC_COMPENSATOR)] = _("Synchronous compensator");
	rootElementName[static_cast<unsigned int>(ElementPlotData::CurveType::CT_SYNC_GENERATOR)] = _("Synchronous generator");
	rootElementName[static_cast<unsigned int>(ElementPlotData::CurveType::CT_TRANSFORMER)] = _("Transformer");
	rootElementName[static_cast<unsigned int>(ElementPlotData::CurveType::CT_TEST)] = _("Test");

	wxTreeItemId rootItemID[static_cast<unsigned int>(ElementPlotData::CurveType::NUM_ELEMENTS)];

	for (auto it = m_epdList.begin(), itEnd = m_epdList.end(); it != itEnd; ++it) {
		ElementPlotData data = *it;
		unsigned int curveType = static_cast<unsigned int>(data.GetCurveType());

		if (firstElement[curveType]) {
			rootItemID[curveType] = m_treeCtrl->AppendItem(rootID, rootElementName[curveType]);
			firstElement[curveType] = false;
		}
		wxTreeItemId itemID = m_treeCtrl->AppendItem(rootItemID[curveType], data.GetName());
		for (int i = 0; i < data.GetElementDataNumber(); ++i) {
			m_treeCtrl->AppendItem(itemID, data.GetDataName(i), -1, -1, data.GetPlotData(i));
		}
	}
}

void ChartView::OnPropertyGridChange(wxPropertyGridEvent& event)
{
	bool fit = false;
	wxString pName = event.GetPropertyName();
	if (pName.empty()) return;

	if (m_treeCtrl->GetSelection()) {
		if (PlotData* data = dynamic_cast<PlotData*>(m_treeCtrl->GetItemData(m_treeCtrl->GetSelection()))) {
			if (pName == _("Draw")) {
				bool isPlotting = m_pgPropDraw->GetValue();
				data->SetPlot(isPlotting);
				if (isPlotting) {
					wxColour colour = GetNextColour();
					data->SetColour(colour);
					m_pgPropColor->SetValue(static_cast<wxVariant>(static_cast<wxAny>(colour)));
					m_treeCtrl->SetItemBold(m_treeCtrl->GetSelection(), true);
				}
				else {
					m_treeCtrl->SetItemBold(m_treeCtrl->GetSelection(), false);
				}
				fit = true;
			}
			else if (pName == _("Color")) {
				wxColour colour;
				colour << m_pgPropColor->GetValue();
				data->SetColour(colour);
			}
			else if (pName == _("Thickness")) {
				data->SetThick(m_pgProplineThick->GetValue().GetInteger());
			}
			else if (pName == _("Type")) {
				data->SetPenType(static_cast<wxPenStyle>(m_pgProplineType->GetValue().GetInteger()));
			}
			else if (pName == _("Axis")) {
				int axis = m_pgProplineAxis->GetValue().GetInteger();
				if (axis == 1) {  // Y
					// All lines to Y axis
					AllToYAxis(m_treeCtrl->GetRootItem());
					// Select curve to X axis
					m_treeCtrl->SetItemTextColour(m_treeCtrl->GetSelection(), *wxRED);
					m_xAxisValues = data->GetValues();
					m_sugestXLabel = data->GetName() + GetUnitFromMagText(data->GetName());
				}
				data->SetAxis(axis);
				fit = true;
			}
		}
	}

	if (pName == _("Margins")) {
		if (m_plotLib == PlotLib::wxMATH_PLOT) {
			m_mpWindow->SetMargins(m_pgPropMarginsUp->GetValue().GetLong(), m_pgPropMarginsRight->GetValue().GetLong(),
				m_pgPropMarginsBot->GetValue().GetLong(), m_pgPropMarginsLeft->GetValue().GetLong());
			m_mpWindow->UpdateAll();
		}
		else if (m_plotLib == PlotLib::wxCHART_DIR) {
			m_chartViewerDir->updateViewPort(true, false);
			event.Skip();
		}
	}
	if (pName == _("Axis limit")) {
		if (m_plotLib == PlotLib::wxMATH_PLOT) {
			m_mpWindow->Fit(m_pgPropXMin->GetValue().GetDouble(), m_pgPropXMax->GetValue().GetDouble(),
				m_pgPropYMin->GetValue().GetDouble(), m_pgPropYMax->GetValue().GetDouble());
			m_mpWindow->UpdateAll();
		}
		else if (m_plotLib == PlotLib::wxCHART_DIR) {
			m_forceAxisLimits = true;
		}
	}
	UpdatePlot(fit);
	event.Skip();
}

void ChartView::OnRightClickDown(wxMouseEvent& event)
{
	Fit();
}

void ChartView::OnMenuDarkThemeClick(wxCommandEvent& event)
{
	m_darkTheme = event.IsChecked();

	if (m_plotLib == PlotLib::wxMATH_PLOT) {
		wxColour grey(96, 96, 96);

		if (m_darkTheme) {
			m_mpWindow->SetColourTheme(*wxBLACK, *wxWHITE, grey);
			m_leg->SetBrush(*wxBLACK_BRUSH);
			m_coords->SetBrush(*wxBLACK_BRUSH);
		}
		else {
			m_mpWindow->SetColourTheme(*wxWHITE, *wxBLACK, grey);
			m_leg->SetBrush(*wxWHITE_BRUSH);
			m_coords->SetBrush(*wxWHITE_BRUSH);
		}

		m_mpWindow->UpdateAll();
	}
	else if (m_plotLib == PlotLib::wxCHART_DIR) {
		DrawChartDir();
	}
}

void ChartView::OnMenuSaveImageClick(wxCommandEvent& event)
{
	if (m_plotLib == PlotLib::wxMATH_PLOT) {
		int x = m_mpWindow->GetScreenPosition().x;
		int y = m_mpWindow->GetScreenPosition().y;
		int width = m_mpWindow->GetSize().GetWidth();
		int height = m_mpWindow->GetSize().GetHeight();

		wxScreenDC dcScreen;
		wxBitmap screenshot(width, height);

		wxMemoryDC memDC;
		memDC.SelectObject(screenshot);

		memDC.Blit(0, 0, width, height, &dcScreen, x, y);
		memDC.SelectObject(wxNullBitmap);

		wxFileDialog saveFileDialog(
			this, _("Save image"), "", "",
			"PNG image file (*.png)|*.png|Bitmap image file (*.bmp)|*.bmp|JPEG image file (*.jpg)|*.jpg",
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

		wxFileName imagePath(saveFileDialog.GetPath());
		wxBitmapType imageType = wxBITMAP_TYPE_BMP;

		if (imagePath.GetExt() == "png")
			imageType = wxBITMAP_TYPE_PNG;
		else if (imagePath.GetExt() == "jpg")
			imageType = wxBITMAP_TYPE_JPEG;

		screenshot.SaveFile(imagePath.GetFullPath(), imageType);
	}
	else if (m_plotLib == PlotLib::wxCHART_DIR) {
		wxFileDialog saveFileDialog(
			this, _("Save image"), "", "",
			"PNG (*.png)|*.png|JPG (*.jpg)|*.jpg|GIF (*.gif)|*.gif|BMP (*.bmp)|*.bmp|SVG (*.svg)|*.svg|PDF (*.pdf)|*.pdf",
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

		wxFileName imagePath(saveFileDialog.GetPath());
		m_chartViewerDir->getChart()->makeChart(imagePath.GetFullPath().ToUTF8());
	}
}

void ChartView::OnMenuSendClipClick(wxCommandEvent& event)
{
	wxBitmap screenshot;
	if (m_plotLib == PlotLib::wxMATH_PLOT) {
		int x = m_mpWindow->GetScreenPosition().x;
		int y = m_mpWindow->GetScreenPosition().y;
		int width = m_mpWindow->GetSize().GetWidth();
		int height = m_mpWindow->GetSize().GetHeight();

		wxScreenDC dcScreen;
		screenshot = wxBitmap(width, height);

		wxMemoryDC memDC;
		memDC.SelectObject(screenshot);

		memDC.Blit(0, 0, width, height, &dcScreen, x, y);
		memDC.SelectObject(wxNullBitmap);
	}
	else if (m_plotLib == PlotLib::wxCHART_DIR) {
		MemBlock memBl = m_chartViewerDir->getChart()->makeChart(Chart::BMP);
		wxMemoryInputStream in(memBl.data, memBl.len);
		screenshot = wxBitmap(wxImage(in, wxBITMAP_TYPE_BMP));
	}

	if (screenshot.IsOk()) {
		if (wxTheClipboard->Open()) {
			wxTheClipboard->SetData(new wxBitmapDataObject(screenshot));
			wxTheClipboard->Close();

			wxMessageDialog msgDialog(this, _("Chart send to clipboard"), _("Info"), wxOK | wxICON_INFORMATION,
				wxDefaultPosition);
			msgDialog.ShowModal();
		}
		else {
			wxMessageDialog msgDialog(this, _("It was not possible to send to clipboard"), _("Error"), wxOK | wxICON_ERROR,
				wxDefaultPosition);
			msgDialog.ShowModal();
		}
	}
	else {
		wxMessageDialog msgDialog(this, _("It was not possible to create the screenshot"), _("Error"), wxOK | wxICON_ERROR,
			wxDefaultPosition);
		msgDialog.ShowModal();
	}
}

void ChartView::OnMenuShowCoordinatesClick(wxCommandEvent& event)
{
	m_showCoords = event.IsChecked();
	if (m_plotLib == PlotLib::wxMATH_PLOT) {
		m_coords->SetVisible(m_showCoords);
		m_mpWindow->UpdateAll();
	}
	else if (m_plotLib == PlotLib::wxCHART_DIR) {
		if (m_showCoords) {
			XYChart* chart = static_cast<XYChart*>(m_chartViewerDir->getChart());
			m_trackLinePos = chart->getPlotArea()->getLeftX() + (int)(chart->getPlotArea()->getWidth() * 0.5);
		}
		DrawChartDir();
	}
}

void ChartView::OnMenuShowGridClick(wxCommandEvent& event)
{
	m_hideGrid = event.IsChecked() ? false : true;
	if (m_plotLib == PlotLib::wxMATH_PLOT) {
		m_xaxis->SetTicks(m_hideGrid);
		m_yaxis->SetTicks(m_hideGrid);
		m_mpWindow->UpdateAll();
	}
	else if (m_plotLib == PlotLib::wxCHART_DIR) {
		DrawChartDir();
	}
}

void ChartView::OnMenuShowLabelClick(wxCommandEvent& event)
{
	m_showLeg = event.IsChecked();

	if (m_plotLib == PlotLib::wxMATH_PLOT) {
		m_leg->SetVisible(m_showLeg);
		m_mpWindow->UpdateAll();
	}
	else if (m_plotLib == PlotLib::wxCHART_DIR) {
		if (!m_showLeg) m_legendHeight = 0;
		DrawChartDir();
	}
}

void ChartView::Fit()
{
	if (m_plotLib == PlotLib::wxMATH_PLOT) {
		m_mpWindow->Fit();
		double bBox[4];
		m_mpWindow->GetBoundingBox(bBox);

		m_pgPropXMin->SetValue(bBox[0]);
		m_pgPropXMax->SetValue(bBox[1]);
		m_pgPropYMin->SetValue(bBox[2]);
		m_pgPropYMax->SetValue(bBox[3]);
	}
	else if (m_plotLib == PlotLib::wxCHART_DIR) {
		m_forceAxisLimits = false;
		// Reset viewport
		m_chartViewerDir->setFullRange("x", 0, 0);
		m_chartViewerDir->setFullRange("y", 0, 0);
		m_chartViewerDir->setViewPortLeft(0);
		m_chartViewerDir->setViewPortTop(0);
		m_chartViewerDir->setViewPortWidth(1);
		m_chartViewerDir->setViewPortHeight(1);

		// redraw the chart
		m_chartViewerDir->updateViewPort(true, false);
	}
}

void ChartView::UpdatePlot(bool fit)
{
	if (m_plotLib == PlotLib::wxMATH_PLOT) {
		wxRect legRect = m_leg->GetRectangle();
		wxRect coordsRect = m_coords->GetRectangle();
		m_mpWindow->DelAllLayers(true, false);

		// GoAllTrees(treeCtrl_ChartSelection->GetRootItem());
		UpdateAllPlots(m_treeCtrl->GetRootItem());

		m_xaxis = new mpScaleX(m_pgPropXLabel->GetValueAsString(), mpALIGN_BOTTOM, true);
		m_yaxis = new mpScaleY(m_pgPropYLabel->GetValueAsString(), mpALIGN_LEFT, true);
		m_leg = new mpInfoLegend(legRect, wxWHITE_BRUSH);
		m_coords = new mpInfoCoords(coordsRect, wxWHITE_BRUSH);

		m_xaxis->SetDrawOutsideMargins(false);
		m_yaxis->SetDrawOutsideMargins(false);
		m_xaxis->SetTicks(m_hideGrid);
		m_yaxis->SetTicks(m_hideGrid);

		mpText* chartTitle = new mpText(m_pgPropChartTitle->GetValueAsString(), 50, 0);
		wxFont chartTitleFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
		chartTitle->SetFont(chartTitleFont);

		m_mpWindow->AddLayer(m_xaxis);
		m_mpWindow->AddLayer(m_yaxis);
		m_mpWindow->AddLayer(m_leg);
		m_mpWindow->AddLayer(m_coords);
		m_mpWindow->AddLayer(chartTitle);

		m_leg->SetVisible(m_showLeg);
		m_coords->SetVisible(m_showCoords);

		if (fit) Fit();

		wxColour grey(96, 96, 96);
		if (m_darkTheme) {
			m_mpWindow->SetColourTheme(*wxBLACK, *wxWHITE, grey);
			m_leg->SetBrush(*wxBLACK_BRUSH);
			m_coords->SetBrush(*wxBLACK_BRUSH);
		}
		else {
			m_mpWindow->SetColourTheme(*wxWHITE, *wxBLACK, grey);
			m_leg->SetBrush(*wxWHITE_BRUSH);
			m_coords->SetBrush(*wxWHITE_BRUSH);
		}
	}
	else if (m_plotLib == PlotLib::wxCHART_DIR) {
		DrawChartDir();
		if (fit) Fit();
	}
}

void ChartView::OnTreeItemActivated(wxTreeEvent& event)
{
	if (PlotData* data = dynamic_cast<PlotData*>(m_treeCtrl->GetItemData(event.GetItem()))) {
		bool isPlotting = data->IsPlot() ? false : true;
		data->SetPlot(isPlotting);
		m_pgPropDraw->SetValue(data->IsPlot());
		if (isPlotting) {
			wxColour colour = GetNextColour();
			data->SetColour(colour);
			m_pgPropColor->SetValue(static_cast<wxVariant>(static_cast<wxAny>(colour)));
			m_treeCtrl->SetItemBold(m_treeCtrl->GetSelection(), true);
		}
		else {
			m_treeCtrl->SetItemBold(m_treeCtrl->GetSelection(), false);
		}
		UpdatePlot(true);
	}

	if (event.GetItem() == m_treeTimeID) {
		AllToYAxis(m_treeCtrl->GetRootItem());
		m_treeCtrl->SetItemTextColour(m_treeTimeID, *wxRED);
		m_xAxisValues = m_time;
		m_sugestXLabel = _("Time (s)");
		UpdatePlot(true);
	}

	event.Skip();
}

void ChartView::OnTreeItemSelectionChanged(wxTreeEvent& event)
{
	if (PlotData* data = dynamic_cast<PlotData*>(m_treeCtrl->GetItemData(event.GetItem()))) {
		m_pgPropDraw->SetValue(data->IsPlot());
		wxVariant colour;
		colour << data->GetColour();
		m_pgPropColor->SetValue(colour);
		m_pgProplineThick->SetValue(data->GetThick());
		m_pgProplineType->SetValue(data->GetPenType());
		m_pgProplineAxis->SetValue(data->GetAxis());
	}
	event.Skip();
}

void ChartView::BuildColourList()
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

wxColour ChartView::GetNextColour()
{
	if (*m_itColourList == *wxBLACK)
		m_itColourList = m_colourList.begin();
	else
		++m_itColourList;

	return *m_itColourList;
}

wxTreeItemId ChartView::AllToYAxis(wxTreeItemId root)
{
	wxTreeItemIdValue cookie;
	wxTreeItemId item = m_treeCtrl->GetFirstChild(root, cookie);
	wxTreeItemId child;

	while (item.IsOk()) {
		m_treeCtrl->SetItemTextColour(item, *wxBLACK);
		if (PlotData* data = dynamic_cast<PlotData*>(m_treeCtrl->GetItemData(item))) data->SetAxis(0);  // X axis.

		if (m_treeCtrl->ItemHasChildren(item)) {
			wxTreeItemId nextChild = AllToYAxis(item);
			if (nextChild.IsOk()) return nextChild;
		}
		item = m_treeCtrl->GetNextChild(root, cookie);
	}

	wxTreeItemId dummyID;
	return dummyID;
}

wxTreeItemId ChartView::UpdateAllPlots(wxTreeItemId root)
{
	wxTreeItemIdValue cookie;
	wxTreeItemId item = m_treeCtrl->GetFirstChild(root, cookie);
	wxTreeItemId child;

	while (item.IsOk()) {
		if (PlotData* data = dynamic_cast<PlotData*>(m_treeCtrl->GetItemData(item))) {
			if (data->IsPlot()) {
				wxString parentName = m_treeCtrl->GetItemText(m_treeCtrl->GetItemParent(item));
				mpFXYVector* newLayer = new mpFXYVector(data->GetName() + " (" + parentName + ")");
				newLayer->SetData(m_xAxisValues, data->GetValues());
				newLayer->SetContinuity(true);
				wxPen layerPen(data->GetColour(), data->GetThick(), data->GetPenType());
				newLayer->SetPen(layerPen);
				newLayer->SetDrawOutsideMargins(false);
				newLayer->ShowName(false);

				m_mpWindow->AddLayer(newLayer);
			}
		}

		if (m_treeCtrl->ItemHasChildren(item)) {
			wxTreeItemId nextChild = UpdateAllPlots(item);
			if (nextChild.IsOk()) return nextChild;
		}
		item = m_treeCtrl->GetNextChild(root, cookie);
	}

	wxTreeItemId dummyID;
	return dummyID;
}

wxTreeItemId ChartView::UpdateAllPlotsCharDir(wxTreeItemId root, XYChart* chartDir)
{
	wxTreeItemIdValue cookie;
	wxTreeItemId item = m_treeCtrl->GetFirstChild(root, cookie);
	wxTreeItemId child;

	while (item.IsOk()) {
		if (PlotData* data = dynamic_cast<PlotData*>(m_treeCtrl->GetItemData(item))) {
			if (data->IsPlot()) {
				wxString parentName = m_treeCtrl->GetItemText(m_treeCtrl->GetItemParent(item));

				std::vector<double> yValues = data->GetValues();

				LineLayer* layer = chartDir->addLineLayer();
				layer->setFastLineMode();
				layer->setLineWidth(data->GetThick());
				layer->setXData(DoubleArray(&m_xAxisValues[0], m_xAxisValues.size()));

				if (m_firstPlot) m_rawDataColour.clear();
				int dataColour = (data->GetColour().Red() << 16) + (data->GetColour().Green() << 8) + data->GetColour().Blue();
				int rawColour = dataColour;
				//layer->addDataSet(DoubleArray(&data->GetValues()[0] + startIndex, endIndex - startIndex + 1), dataColour, data->GetName() + " (" + parentName + ")");
				if (data->GetPenType() == wxPENSTYLE_DOT)
					dataColour = chartDir->dashLineColor(dataColour, Chart::DotLine);
				else if (data->GetPenType() == wxPENSTYLE_SHORT_DASH)
					dataColour = chartDir->dashLineColor(dataColour, Chart::DashLine);
				else if (data->GetPenType() == wxPENSTYLE_DOT_DASH)
					dataColour = chartDir->dashLineColor(dataColour, Chart::DotDashLine);
				m_rawDataColour[dataColour] = rawColour;

				layer->addDataSet(DoubleArray(&yValues[0], yValues.size()), dataColour, data->GetName() + " (" + parentName + ")");

				double minY = *std::min_element(yValues.begin(), yValues.end());
				double maxY = *std::max_element(yValues.begin(), yValues.end());
				if (m_firstPlot) {
					m_firstPlot = false;
					m_yMin = minY;
					m_yMax = maxY;
					m_sugestYLabel = data->GetName();

					m_xMin = *std::min_element(m_xAxisValues.begin(), m_xAxisValues.end());
					m_xMax = *std::max_element(m_xAxisValues.begin(), m_xAxisValues.end());
				}
				else {
					if (minY < m_yMin) m_yMin = minY;
					if (maxY > m_yMax) m_yMax = maxY;
					if (m_sugestYLabel.IsEmpty() || (m_sugestYLabel != data->GetName())) m_sugestYLabel = "";
				}
			}
		}

		if (m_treeCtrl->ItemHasChildren(item)) {
			wxTreeItemId nextChild = UpdateAllPlotsCharDir(item, chartDir);
			if (nextChild.IsOk()) return nextChild;
		}
		item = m_treeCtrl->GetNextChild(root, cookie);
	}

	wxTreeItemId dummyID;
	return dummyID;
}

void ChartView::OnMenuExpCSVClick(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this, _("Save CSV file"), "", "", "CSV file (*.csv)|*.csv",
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

	wxTextFile csvFile(saveFileDialog.GetPath());
	if (!csvFile.Create()) {
		if (!csvFile.Open()) {
			wxMessageDialog msgDialog(this, _("It was not possible to open or create the selected file."), _("Error"),
				wxOK | wxCENTRE | wxICON_ERROR);
			msgDialog.ShowModal();
		}
	}
	else
		csvFile.Open();
	if (csvFile.IsOpened()) {
		csvFile.Clear();
		csvFile.AddLine(GetActiveCurvesCSV());
		csvFile.Write();
		csvFile.Close();
	}
}

wxString ChartView::GetActiveCurvesCSV()
{
	std::vector<PlotData*> activePlotDataList;
	GetActivePlotData(m_treeCtrl->GetRootItem(), activePlotDataList);

	std::vector<double> xValues;
	wxString xName = "";

	// Find X axis curve, if none is found, X is the m_time.
	bool foundXAxis = false;
	for (auto it = activePlotDataList.begin(), itEnd = activePlotDataList.end(); it != itEnd; ++it) {
		PlotData* data = *it;
		if (data->GetAxis() == 1) {
			xValues = data->GetValues();
			xName = data->GetName();
			foundXAxis = true;
			activePlotDataList.erase(it);
			delete data;
			break;
		}
	}
	if (!foundXAxis) {
		xValues = m_time;
		xName = _("Time");
	}

	// Build CSV text.
	wxString csvText = xName + ";";
	// Header
	for (auto it = activePlotDataList.begin(), itEnd = activePlotDataList.end(); it != itEnd; ++it) {
		PlotData* data = *it;
		csvText += data->GetName() + ";";
	}
	csvText[csvText.length() - 1] = '\n';
	// Values
	for (unsigned int i = 0; i < xValues.size(); ++i) {
		csvText += wxString::FromCDouble(xValues[i], 13) + ";";
		for (unsigned int j = 0; j < activePlotDataList.size(); ++j) {
			double value = 0.0;
			if (i < activePlotDataList[j]->GetValues().size()) {
				value = activePlotDataList[j]->GetValues()[i];
			}
			csvText += wxString::FromCDouble(value, 13) + ";";
		}
		csvText[csvText.length() - 1] = '\n';
	}

	// Clear active plot data vector.
	for (auto it = activePlotDataList.begin(); it != activePlotDataList.end(); ++it) {
		delete(*it);
	}
	activePlotDataList.clear();

	return csvText;
}

wxTreeItemId ChartView::GetActivePlotData(wxTreeItemId root, std::vector<PlotData*>& plotDataList)
{
	wxTreeItemIdValue cookie;
	wxTreeItemId item = m_treeCtrl->GetFirstChild(root, cookie);
	wxTreeItemId child;

	while (item.IsOk()) {
		if (PlotData* data = dynamic_cast<PlotData*>(m_treeCtrl->GetItemData(item))) {
			if (data->IsPlot() || data->GetAxis() == 1) {
				wxString parentName = m_treeCtrl->GetItemText(m_treeCtrl->GetItemParent(item));

				PlotData* dataCopy = new PlotData();
				*dataCopy = *data;
				dataCopy->SetName(data->GetName() + " (" + parentName + ")");
				plotDataList.push_back(dataCopy);
			}
		}

		if (m_treeCtrl->ItemHasChildren(item)) {
			wxTreeItemId nextChild = GetActivePlotData(item, plotDataList);
			if (nextChild.IsOk()) return nextChild;
		}
		item = m_treeCtrl->GetNextChild(root, cookie);
	}

	wxTreeItemId dummyID;
	return dummyID;
}
void ChartView::DrawChartDir()
{
	int gridColour = 0xEAEAEA;
	int bgColour = 0xFFFFFF;
	int labelColour = 0x000000;
	if (m_darkTheme) {
		bgColour = 0x303030;
		gridColour = 0x3D3D3D;
		labelColour = 0xA0A0A0;
	}
	if (m_hideGrid) gridColour = Chart::Transparent;

	XYChart* chartDir = new XYChart(std::max(400, m_chartViewerSize.GetWidth()), std::max(300, m_chartViewerSize.GetHeight()), bgColour);

	wxString title = m_pgPropChartTitle->GetValueAsString();
	int titleSize = 0;
	if (!title.IsEmpty()) {
		chartDir->addTitle(title, "arial.ttf", 12, labelColour);
		titleSize = 12;
	}

	LegendBox* box = nullptr;
	if (m_showLeg) {
		box = chartDir->addLegend(0, 0, false, "arial.ttf", 10);
		box->setBackground(Chart::Transparent, Chart::Transparent);
		box->setPos(m_pgPropMarginsLeft->GetValue().GetLong(), m_pgPropMarginsUp->GetValue().GetLong() + titleSize);
		box->setFontColor(labelColour);
		box->setLineStyleKey();
	}
	m_firstPlot = true;
	UpdateAllPlotsCharDir(m_treeCtrl->GetRootItem(), chartDir);

	wxString xLabel = m_pgPropXLabel->GetValueAsString();
	if (xLabel == _("auto")) xLabel = m_sugestXLabel;
	wxString yLabel = m_pgPropYLabel->GetValueAsString();
	if (yLabel == _("auto")) yLabel = m_sugestYLabel + GetUnitFromMagText(m_sugestYLabel);
	int xLabelSize = 10, yLabelSize = 10;
	// If the label is empty, the size is 0.
	if (xLabel.IsEmpty()) xLabelSize = 0;
	if (yLabel.IsEmpty()) yLabelSize = 0;

	chartDir->setPlotArea(
		yLabelSize + m_pgPropMarginsLeft->GetValue().GetLong(),
		titleSize + m_legendHeight + m_pgPropMarginsUp->GetValue().GetLong(),
		chartDir->getWidth() - m_pgPropMarginsRight->GetValue().GetLong() - m_pgPropMarginsLeft->GetValue().GetLong() - yLabelSize,
		chartDir->getHeight() - m_pgPropMarginsBot->GetValue().GetLong() - m_pgPropMarginsUp->GetValue().GetLong() - m_legendHeight - titleSize - xLabelSize,
		bgColour, -1, -1, gridColour, gridColour);
	chartDir->setClipping();

	//DoubleArray timeArray(&m_time[0], m_time.size());
	double xMin = 0, xMax = 0, yMin = 0, yMax = 0;
	if (m_forceAxisLimits) {
		xMin = m_pgPropXMin->GetValue().GetDouble();
		xMax = m_pgPropXMax->GetValue().GetDouble();
		yMin = m_pgPropYMin->GetValue().GetDouble();
		yMax = m_pgPropYMax->GetValue().GetDouble();
	}
	else {
		//xMin = m_xAxisValues[0];
		xMin = m_xMin;
		//xMax = m_time[m_time.size() - 1];
		//xMax = m_xAxisValues[m_xAxisValues.size() - 1];
		xMax = m_xMax;
		yMin = m_yMin - 0.05 * (m_yMax - m_yMin);
		yMax = m_yMax + 0.05 * (m_yMax - m_yMin);
		m_pgPropXMin->SetValue(xMin);
		m_pgPropXMax->SetValue(xMax);
		m_pgPropYMin->SetValue(yMin);
		m_pgPropYMax->SetValue(yMax);
	}
	m_chartViewerDir->setFullRange("x", xMin, xMax);
	m_chartViewerDir->setFullRange("y", yMin, yMax);

	// Get the start date and end date that are visible on the chart.
	double viewPortStartTime = m_chartViewerDir->getValueAtViewPort("x", m_chartViewerDir->getViewPortLeft());
	double viewPortEndTime = m_chartViewerDir->getValueAtViewPort("x", m_chartViewerDir->getViewPortLeft() + m_chartViewerDir->getViewPortWidth());

	chartDir->xAxis()->setTickDensity(20);
	chartDir->xAxis()->setLinearScale(viewPortStartTime, viewPortEndTime);
	chartDir->xAxis()->setLabelFormat(wxString::Format("{value|2~%c}", DotOrComma()));
	chartDir->xAxis()->setColors(labelColour, labelColour);
	chartDir->xAxis()->setTitle(xLabel, "arial.ttf", 10, labelColour);

	chartDir->yAxis()->setLabelFormat(wxString::Format("{value|2~%c}", DotOrComma()));
	chartDir->yAxis()->setColors(labelColour, labelColour);
	chartDir->yAxis()->setTitle(yLabel, "arial.ttf", 10, labelColour);
	m_chartViewerDir->syncLinearAxisWithViewPort("y", chartDir->yAxis());

	if (m_showCoords) {
		DrawTrackLine(chartDir);
	}

	if (m_chartViewerDir->getChart() != nullptr)
	{
		delete m_chartViewerDir->getChart();
	}
	// Set the chart image to the WinChartViewer
	m_chartViewerDir->setChart(chartDir);

	if (m_showLeg && box != nullptr) {
		if (std::abs(m_legendHeight - box->getHeight()) > 1) {
			m_legendHeight = box->getHeight();
			DrawChartDir();
		}
	}
}

void ChartView::OnViewPortChanged(wxCommandEvent& event)
{
	if (m_chartViewerDir->needUpdateChart()) DrawChartDir();
	event.Skip();
}

void ChartView::OnMouseMovePlotArea(wxCommandEvent& event)
{
	if (m_showCoords) {
		const int grabDist = 8;

		int mouseX = m_chartViewerDir->getChartMouseX();
		PlotArea* plotArea = static_cast<XYChart*>(m_chartViewerDir->getChart())->getPlotArea();
		//m_trackLinePos = (std::min)(plotArea->getRightX(), (std::max)(plotArea->getLeftX(), mouseX - grabDist));

		bool moveTrackLine = false;

		if (m_chartViewerDir->getMouseUsage() == Chart::MouseUsageDefault && wxGetMouseState().LeftIsDown()) {
			// Lock in move track line
			SetCursor(wxCURSOR_SIZEWE);
			moveTrackLine = true;
		}

		if (std::abs(mouseX - m_trackLinePos) <= grabDist && !m_chartViewerDir->IsSelectionRectVisible()) {
			SetCursor(wxCURSOR_SIZEWE);
			m_chartViewerDir->setMouseUsage(Chart::MouseUsageDefault);
		}
		else if (!wxGetMouseState().LeftIsDown()) {
			SetCursor(wxCURSOR_ARROW);
			m_chartViewerDir->setMouseUsage(Chart::MouseUsageZoomIn);
		}

		if (moveTrackLine) {
			m_trackLinePos = mouseX;
			DrawTrackLine(static_cast<XYChart*>(m_chartViewerDir->getChart()));
			m_chartViewerDir->updateDisplay();
		}

	}
	event.Skip();
}

void ChartView::DrawTrackLine(XYChart* chartDir)
{
	chartDir->initDynamicLayer();

	double xValue = chartDir->getNearestXValue(m_trackLinePos);
	int xCoor = chartDir->getXCoor(xValue);
	chartDir->getDrawArea()->line(xCoor, chartDir->getPlotArea()->getTopY(), xCoor, chartDir->getPlotArea()->getBottomY(), 0x888888, 3);

	std::ostringstream xlabel;

	//xlabel << "<*font,bgColor=000000*> " << chartDir->xAxis()->getFormattedLabel(xValue) << " s <*/font*>";
	xlabel << "<*font,bgColor=000000*> " << chartDir->formatValue(xValue, wxString::Format("{value|P4~%c}", DotOrComma())) << " <*/font*>";
	TTFText* text = chartDir->getDrawArea()->text(xlabel.str().c_str(), "arialbd.ttf", 10);
	int xLabelPos = std::max(0, std::min(xCoor - text->getWidth() / 2, chartDir->getWidth() - text->getWidth()));
	text->draw(xLabelPos, chartDir->getPlotArea()->getBottomY() + 3, 0xffffff);
	text->destroy();

	for (int i = 0; i < chartDir->getLayerCount(); ++i)
	{
		Layer* layer = chartDir->getLayerByZ(i);

		// The data array index of the x-value
		int xIndex = layer->getXIndexOf(xValue);

		// Iterate through all the data sets in the layer
		for (int j = 0; j < layer->getDataSetCount(); ++j)
		{
			DataSet* dataSet = layer->getDataSetByZ(j);
			const char* dataSetName = dataSet->getDataName();

			// Get the color, name and position of the data label
			int color = dataSet->getDataColor();
			int yCoor = chartDir->getYCoor(dataSet->getPosition(xIndex), dataSet->getUseYAxis());

			// Draw a track dot with a label next to it for visible data points in the plot area
			if ((yCoor >= chartDir->getPlotArea()->getTopY()) && (yCoor <= chartDir->getPlotArea()->getBottomY()) &&
				(color != Chart::Transparent) && dataSetName && *dataSetName)
			{
				chartDir->getDrawArea()->circle(xCoor, yCoor, 4, 4, color, color);

				std::ostringstream label;

				//wxMessageBox(wxString::Format("%d", i));
				label << "<*font,bgColor=" << std::hex << m_rawDataColour[color] << "*> " << chartDir->formatValue(dataSet->getValue(xIndex), wxString::Format("{value|P4~%c}", DotOrComma())) << " <*font*>";
				text = chartDir->getDrawArea()->text(label.str().c_str(), "arialbd.ttf", 10);
				//log[dataSetName] = dataSet->getValue(xIndex);

				// Draw the label on the right side of the dot if the mouse is on the left side the
				// chart, and vice versa. This ensures the label will not go outside the chart image.
				if (xCoor <= (chartDir->getPlotArea()->getLeftX() + chartDir->getPlotArea()->getRightX()) / 2)
				{
					text->draw(xCoor + 6, yCoor, 0xffffff, Chart::Left);
				}
				else
				{
					text->draw(xCoor - 6, yCoor, 0xffffff, Chart::Right);
				}
				text->destroy();
			}
		}
	}
}

wxString ChartView::GetUnitFromMagText(wxString magText)
{
	wxString unitText = " (p.u.)";
	if (magText.IsEmpty()) unitText = "";
	else if (magText == _("Angle")) unitText = " (°)";
	else if (magText == _("Delta")) unitText = " (°)";
	else if (magText == _("Frequency")) unitText = " (Hz)";
	else if (magText == _("Velocity")) unitText = " (rad/s)";
	else if (magText == _("Slip")) unitText = " (%)";
	return unitText;
}

void ChartView::OnResize(wxSizeEvent& event)
{
	if (m_plotLib == PlotLib::wxCHART_DIR) {
		m_chartViewerSize = event.GetSize() - wxSize(m_treeCtrl->GetSize().GetWidth(), 60);
		//m_chartViewerDir->Layout();
		m_chartViewerDir->updateViewPort(true, false);
		//DrawChartDir();
	}
	event.Skip();
}
