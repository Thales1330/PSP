#include "ChartView.h"

ChartView::ChartView(wxWindow* parent) : ChartViewBase(parent)
{
    m_pgPropColor = m_pgMgr->Insert(m_pgPropLineProp, 1, new wxColourProperty(_("Color")));
    m_pgPropColor->SetEditor(wxT("ChoiceAndButton"));
    m_pgPropColor->SetValue(static_cast<wxVariant>(*wxBLACK));

    m_pgPropMargins->SetValue(wxT("<composed>"));
    m_pgMgr->Collapse(m_pgPropMargins);
    m_pgPropAxisLimit->SetValue(wxT("<composed>"));
    m_pgMgr->Collapse(m_pgPropAxisLimit);

    SetMPWindow();
    wxSizer* sizer = GetSizer();
    sizer->Add(m_mpWindow, 1, wxEXPAND, WXC_FROM_DIP(5));
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
    m_xaxis->SetTicks(false);
    m_yaxis->SetTicks(false);

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

    m_leg->SetVisible(true);
    m_coords->SetVisible(false);

    m_mpWindow->EnableDoubleBuffer(true);
    m_mpWindow->LockAspect(false);
    m_mpWindow->Fit();
}

void ChartView::OnPropertyGridChange(wxPropertyGridEvent& event) {}
