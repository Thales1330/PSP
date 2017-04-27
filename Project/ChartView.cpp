#include "ChartView.h"
#include "ElementPlotData.h"

ChartView::ChartView(wxWindow* parent, std::vector<ElementPlotData> epdList, std::vector<double> time)
    : ChartViewBase(parent)
{
    m_epdList = epdList;
    m_time = time;

    m_menuItemShowGrid->Check(m_hideGrid ? false : true);
    m_menuItemShowLabel->Check(m_showLeg);
    m_menuItemShowCoordinates->Check(m_showCoords);
    m_menuItemDarkTheme->Check(m_darkTheme);

    // Create color property.
    m_pgPropColor = m_pgMgr->Insert(m_pgPropLineProp, 1, new wxColourProperty(_("Color")));
    m_pgPropColor->SetEditor(wxT("ChoiceAndButton"));
    m_pgPropColor->SetValue(static_cast<wxVariant>(*wxBLACK));

    // Set margins and axis limit to composed mode.
    m_pgPropMargins->SetValue(wxT("<composed>"));
    m_pgMgr->Collapse(m_pgPropMargins);
    m_pgPropAxisLimit->SetValue(wxT("<composed>"));
    m_pgMgr->Collapse(m_pgPropAxisLimit);

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

    bool firstElement[ElementPlotData::NUM_ELEMENTS];
    for(int i = 0; i < ElementPlotData::NUM_ELEMENTS; ++i) firstElement[i] = true;

    wxString rootElementName[ElementPlotData::NUM_ELEMENTS];
    rootElementName[ElementPlotData::CT_BUS] = _("Bus");
    rootElementName[ElementPlotData::CT_IND_MOTOR] = _("Induction motor");
    rootElementName[ElementPlotData::CT_LINE] = _("Line");
    rootElementName[ElementPlotData::CT_LOAD] = _("Load");
    rootElementName[ElementPlotData::CT_SHUNT_CAPACITOR] = _("Capacitor");
    rootElementName[ElementPlotData::CT_SHUNT_INDUCTOR] = _("Inductor");
    rootElementName[ElementPlotData::CT_SYNC_COMPENSATOR] = _("Synchronous compensator");
    rootElementName[ElementPlotData::CT_SYNC_GENERATOR] = _("Synchronous generator");
    rootElementName[ElementPlotData::CT_TRANSFORMER] = _("Transformer");

    wxTreeItemId rootItemID[ElementPlotData::NUM_ELEMENTS];

    for(auto it = m_epdList.begin(), itEnd = m_epdList.end(); it != itEnd; ++it) {
        ElementPlotData data = *it;
        ElementPlotData::CurveType curveType = data.GetCurveType();

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

void ChartView::OnMenuSaveImageClick(wxCommandEvent& event) {}
void ChartView::OnMenuSendClipClick(wxCommandEvent& event) {}
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
    PlotData* data = dynamic_cast<PlotData*>(m_treeCtrl->GetItemData(event.GetItem()));

    if(data) {
        bool isPlotting = data->IsPlot() ? false : true;
        data->SetPlot(isPlotting);
        m_pgPropDraw->SetValue(data->IsPlot());
        if(isPlotting) {
            wxColour colour = GetNextColour();
            wxVariant vColour;
            vColour << colour;
            //data->SetColour(colour);
            m_pgPropColor->SetValue(vColour);
        }
        UpdatePlot(true);
    }

    if(event.GetItem() == m_treeTimeID) {
        // AllToYAxis(treeCtrl_ChartSelection->GetRootItem());
        m_treeCtrl->SetItemTextColour(m_treeTimeID, *wxRED);
        // xAxisValues = time;
        UpdatePlot(true);
    }

    event.Skip();
}

void ChartView::OnTreeItemSelectionChanged(wxTreeEvent& event)
{
    PlotData* data = dynamic_cast<PlotData*>(m_treeCtrl->GetItemData(event.GetItem()));
    if(data) {
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
    m_colourList.push_back(wxColour(0, 0, 0));
    m_itColourList = m_colourList.end();
}

wxColour ChartView::GetNextColour()
{
    if(*m_itColourList == *wxBLACK)
        m_itColourList = m_colourList.begin();
    else
        ++m_itColourList;

    return *m_itColourList;
}
