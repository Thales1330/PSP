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

ChartView::ChartView(wxWindow* parent, std::vector<ElementPlotData> epdList, std::vector<double> time)
    : ChartViewBase(parent)
{
    m_epdList = epdList;
    m_time = time;
    m_xAxisValues = time;

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
    m_pgProplineType->AddChoice(_("Cross"), wxPENSTYLE_CROSS_HATCH);
    m_pgProplineType->AddChoice(_("Driagonal cross"), wxPENSTYLE_CROSSDIAG_HATCH);

    SetMPWindow();
    GetSizer()->Add(m_mpWindow, 1, wxEXPAND, WXC_FROM_DIP(5));
    SetTreectrl();
    Layout();
    SetInitialSize();

    BuildColourList();
}

ChartView::~ChartView() {}
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
    for(int i = 0; i < static_cast<unsigned int>(ElementPlotData::CurveType::NUM_ELEMENTS); ++i) firstElement[i] = true;

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

    for(auto it = m_epdList.begin(), itEnd = m_epdList.end(); it != itEnd; ++it) {
        ElementPlotData data = *it;
        unsigned int curveType = static_cast<unsigned int>(data.GetCurveType());

        if(firstElement[curveType]) {
            rootItemID[curveType] = m_treeCtrl->AppendItem(rootID, rootElementName[curveType]);
            firstElement[curveType] = false;
        }
        wxTreeItemId itemID = m_treeCtrl->AppendItem(rootItemID[curveType], data.GetName());
        for(int i = 0; i < data.GetElementDataNumber(); ++i) {
            m_treeCtrl->AppendItem(itemID, data.GetDataName(i), -1, -1, data.GetPlotData(i));
        }
    }
}

void ChartView::OnPropertyGridChange(wxPropertyGridEvent& event)
{
    bool fit = false;

    if(m_treeCtrl->GetSelection()) {
        if(PlotData* data = dynamic_cast<PlotData*>(m_treeCtrl->GetItemData(m_treeCtrl->GetSelection()))) {
            if(event.GetPropertyName() == _("Draw")) {
                bool isPlotting = m_pgPropDraw->GetValue();
                data->SetPlot(isPlotting);
                if(isPlotting) {
                    wxColour colour = GetNextColour();
                    data->SetColour(colour);
                    m_pgPropColor->SetValue(static_cast<wxVariant>(static_cast<wxAny>(colour)));
                    m_treeCtrl->SetItemBold(m_treeCtrl->GetSelection(), true);
                } else {
                    m_treeCtrl->SetItemBold(m_treeCtrl->GetSelection(), false);
                }
                fit = true;
            } else if(event.GetPropertyName() == _("Color")) {
                wxColour colour;
                colour << m_pgPropColor->GetValue();
                data->SetColour(colour);
            } else if(event.GetPropertyName() == _("Thickness")) {
                data->SetThick(m_pgProplineThick->GetValue().GetInteger());
            } else if(event.GetPropertyName() == _("Type")) {
                data->SetPenType(static_cast<wxPenStyle>(m_pgProplineType->GetValue().GetInteger()));
            } else if(event.GetPropertyName() == _("Axis")) {
                int axis = m_pgProplineAxis->GetValue().GetInteger();
                if(axis == 1) {  // Y
                    // All lines to Y axis
                    AllToYAxis(m_treeCtrl->GetRootItem());
                    // curva selecionada para o eixo X
                    m_treeCtrl->SetItemTextColour(m_treeCtrl->GetSelection(), *wxRED);
                    m_xAxisValues = data->GetValues();
                }
                data->SetAxis(axis);
                fit = true;
            }
        }
    }

    if(event.GetPropertyName() == _("Margins")) {
        m_mpWindow->SetMargins(m_pgPropMarginsUp->GetValue().GetLong(), m_pgPropMarginsRight->GetValue().GetLong(),
                               m_pgPropMarginsBot->GetValue().GetLong(), m_pgPropMarginsLeft->GetValue().GetLong());
        m_mpWindow->UpdateAll();
    }
    if(event.GetPropertyName() == _("Axis limit")) {
        m_mpWindow->Fit(m_pgPropXMin->GetValue().GetDouble(), m_pgPropXMax->GetValue().GetDouble(),
                        m_pgPropYMin->GetValue().GetDouble(), m_pgPropYMax->GetValue().GetDouble());
        m_mpWindow->UpdateAll();
    }
    UpdatePlot(fit);
}

void ChartView::OnMenuDarkThemeClick(wxCommandEvent& event)
{
    m_darkTheme = event.IsChecked();
    wxColour grey(96, 96, 96);

    if(m_darkTheme) {
        m_mpWindow->SetColourTheme(*wxBLACK, *wxWHITE, grey);
        m_leg->SetBrush(*wxBLACK_BRUSH);
        m_coords->SetBrush(*wxBLACK_BRUSH);
    } else {
        m_mpWindow->SetColourTheme(*wxWHITE, *wxBLACK, grey);
        m_leg->SetBrush(*wxWHITE_BRUSH);
        m_coords->SetBrush(*wxWHITE_BRUSH);
    }

    m_mpWindow->UpdateAll();
}

void ChartView::OnMenuSaveImageClick(wxCommandEvent& event)
{
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
    if(saveFileDialog.ShowModal() == wxID_CANCEL) return;

    wxFileName imagePath(saveFileDialog.GetPath());
    wxBitmapType imageType = wxBITMAP_TYPE_BMP;

    if(imagePath.GetExt() == "png")
        imageType = wxBITMAP_TYPE_PNG;
    else if(imagePath.GetExt() == "jpg")
        imageType = wxBITMAP_TYPE_JPEG;

    screenshot.SaveFile(imagePath.GetFullPath(), imageType);
}

void ChartView::OnMenuSendClipClick(wxCommandEvent& event)
{
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

    if(wxTheClipboard->Open()) {
        wxTheClipboard->SetData(new wxBitmapDataObject(screenshot));
        wxTheClipboard->Close();

        wxMessageDialog msgDialog(this, _("Chart send to clipboard"), _("Info"), wxOK | wxICON_INFORMATION,
                                  wxDefaultPosition);
        msgDialog.ShowModal();
    } else {
        wxMessageDialog msgDialog(this, _("It was not possible to send to clipboard"), _("Error"), wxOK | wxICON_ERROR,
                                  wxDefaultPosition);
        msgDialog.ShowModal();
    }
}

void ChartView::OnMenuShowCoordinatesClick(wxCommandEvent& event)
{
    m_showCoords = event.IsChecked();
    m_coords->SetVisible(m_showCoords);
    m_mpWindow->UpdateAll();
}

void ChartView::OnMenuShowGridClick(wxCommandEvent& event)
{
    m_hideGrid = event.IsChecked() ? false : true;
    m_xaxis->SetTicks(m_hideGrid);
    m_yaxis->SetTicks(m_hideGrid);
    m_mpWindow->UpdateAll();
}

void ChartView::OnMenuShowLabelClick(wxCommandEvent& event)
{
    m_showLeg = event.IsChecked();
    m_leg->SetVisible(m_showLeg);
    m_mpWindow->UpdateAll();
}

void ChartView::Fit()
{
    m_mpWindow->Fit();
    double bBox[4];
    m_mpWindow->GetBoundingBox(bBox);

    m_pgPropXMin->SetValue(bBox[0]);
    m_pgPropXMax->SetValue(bBox[1]);
    m_pgPropYMin->SetValue(bBox[2]);
    m_pgPropYMax->SetValue(bBox[3]);
}

void ChartView::UpdatePlot(bool fit)
{
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

    if(fit) Fit();

    wxColour grey(96, 96, 96);
    if(m_darkTheme) {
        m_mpWindow->SetColourTheme(*wxBLACK, *wxWHITE, grey);
        m_leg->SetBrush(*wxBLACK_BRUSH);
        m_coords->SetBrush(*wxBLACK_BRUSH);
    } else {
        m_mpWindow->SetColourTheme(*wxWHITE, *wxBLACK, grey);
        m_leg->SetBrush(*wxWHITE_BRUSH);
        m_coords->SetBrush(*wxWHITE_BRUSH);
    }
}

void ChartView::OnTreeItemActivated(wxTreeEvent& event)
{
    if(PlotData* data = dynamic_cast<PlotData*>(m_treeCtrl->GetItemData(event.GetItem()))) {
        bool isPlotting = data->IsPlot() ? false : true;
        data->SetPlot(isPlotting);
        m_pgPropDraw->SetValue(data->IsPlot());
        if(isPlotting) {
            wxColour colour = GetNextColour();
            data->SetColour(colour);
            m_pgPropColor->SetValue(static_cast<wxVariant>(static_cast<wxAny>(colour)));
            m_treeCtrl->SetItemBold(m_treeCtrl->GetSelection(), true);
        } else {
            m_treeCtrl->SetItemBold(m_treeCtrl->GetSelection(), false);
        }
        UpdatePlot(true);
    }

    if(event.GetItem() == m_treeTimeID) {
        AllToYAxis(m_treeCtrl->GetRootItem());
        m_treeCtrl->SetItemTextColour(m_treeTimeID, *wxRED);
        m_xAxisValues = m_time;
        UpdatePlot(true);
    }

    event.Skip();
}

void ChartView::OnTreeItemSelectionChanged(wxTreeEvent& event)
{
    if(PlotData* data = dynamic_cast<PlotData*>(m_treeCtrl->GetItemData(event.GetItem()))) {
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
    m_colourList.push_back(wxColour(60, 60, 60));
    m_colourList.push_back(wxColour(255, 0, 0));
    m_colourList.push_back(wxColour(0, 0, 255));
    m_colourList.push_back(wxColour(0, 255, 0));
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
    if(*m_itColourList == *wxBLACK)
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

    while(item.IsOk()) {
        m_treeCtrl->SetItemTextColour(item, *wxBLACK);
        if(PlotData* data = dynamic_cast<PlotData*>(m_treeCtrl->GetItemData(item))) data->SetAxis(0);  // X axis.

        if(m_treeCtrl->ItemHasChildren(item)) {
            wxTreeItemId nextChild = AllToYAxis(item);
            if(nextChild.IsOk()) return nextChild;
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

    while(item.IsOk()) {
        if(PlotData* data = dynamic_cast<PlotData*>(m_treeCtrl->GetItemData(item))) {
            if(data->IsPlot()) {
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

        if(m_treeCtrl->ItemHasChildren(item)) {
            wxTreeItemId nextChild = UpdateAllPlots(item);
            if(nextChild.IsOk()) return nextChild;
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
    if(saveFileDialog.ShowModal() == wxID_CANCEL) return;

    wxTextFile csvFile(saveFileDialog.GetPath());
    if(!csvFile.Create()) {
        if(!csvFile.Open()) {
            wxMessageDialog msgDialog(this, _("It was not possible to open or create the selected file."), _("Error"),
                                      wxOK | wxCENTRE | wxICON_ERROR);
            msgDialog.ShowModal();
        }
    } else
        csvFile.Open();
    if(csvFile.IsOpened()) {
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
    for(auto it = activePlotDataList.begin(), itEnd = activePlotDataList.end(); it != itEnd; ++it) {
        PlotData* data = *it;
        if(data->GetAxis() == 1) {
            xValues = data->GetValues();
            xName = data->GetName();
            foundXAxis = true;
            activePlotDataList.erase(it);
            delete data;
            break;
        }
    }
    if(!foundXAxis) {
        xValues = m_time;
        xName = _("Time");
    }

    // Build CSV text.
    wxString csvText = xName + ";";
    // Header
    for(auto it = activePlotDataList.begin(), itEnd = activePlotDataList.end(); it != itEnd; ++it) {
        PlotData* data = *it;
        csvText += data->GetName() + ";";
    }
    csvText[csvText.length() - 1] = '\n';
    // Values
    for(unsigned int i = 0; i < xValues.size(); ++i) {
        csvText += wxString::FromCDouble(xValues[i], 13) + ";";
        for(unsigned int j = 0; j < activePlotDataList.size(); ++j) {
            double value = 0.0;
            if(i < activePlotDataList[j]->GetValues().size()) {
                value = activePlotDataList[j]->GetValues()[i];
            }
            csvText += wxString::FromCDouble(value, 13) + ";";
        }
        csvText[csvText.length() - 1] = '\n';
    }

    // Clear active plot data vector.
    for(auto it = activePlotDataList.begin(); it != activePlotDataList.end(); ++it) {
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

    while(item.IsOk()) {
        if(PlotData* data = dynamic_cast<PlotData*>(m_treeCtrl->GetItemData(item))) {
            if(data->IsPlot() || data->GetAxis() == 1) {
                wxString parentName = m_treeCtrl->GetItemText(m_treeCtrl->GetItemParent(item));

                PlotData* dataCopy = new PlotData();
                *dataCopy = *data;
                dataCopy->SetName(data->GetName() + " (" + parentName + ")");
                plotDataList.push_back(dataCopy);
            }
        }

        if(m_treeCtrl->ItemHasChildren(item)) {
            wxTreeItemId nextChild = GetActivePlotData(item, plotDataList);
            if(nextChild.IsOk()) return nextChild;
        }
        item = m_treeCtrl->GetNextChild(root, cookie);
    }

    wxTreeItemId dummyID;
    return dummyID;
}
