#include "ChartView.h"

ChartView::ChartView(wxWindow* parent) : ChartViewBase(parent)
{
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
    Layout();
    SetInitialSize();
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
