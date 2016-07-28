#include "MainFrame.h"

MainFrame::MainFrame(wxWindow* parent) : MainFrameBase(parent) { Init(); }
MainFrame::~MainFrame()
{
    if(!m_artMetro) delete m_artMetro;
}
void MainFrame::Init()
{
    this->SetSize(800, 600);
    // carregar configs locais

    EnableCurrentProjectRibbon(false);

    m_artMetro = new wxRibbonMetroArtProvider();
    m_ribbonBar->SetArtProvider(m_artMetro);
    m_ribbonBar->Layout();
}

void MainFrame::EnableCurrentProjectRibbon(bool enable)
{
    m_ribbonButtonBarCircuit->EnableButton(ID_RIBBON_ADDELEMENT, enable);
    m_ribbonButtonBarStabData->EnableButton(ID_RIBBON_CHARTS, enable);
	m_ribbonButtonBarCProject->EnableButton(ID_RIBBON_CLOSE, enable);
    m_ribbonButtonBarClipboard->EnableButton(ID_RIBBON_COPY, enable);
    m_ribbonButtonBarReports->EnableButton(ID_RIBBON_DATAREPORT, enable);
    m_ribbonButtonBarHandling->EnableButton(ID_RIBBON_DELETE, enable);
	m_ribbonButtonBarCircuit->EnableButton(ID_RIBBON_DISABLESOL, enable);
    m_ribbonButtonBarHandling->EnableButton(ID_RIBBON_DRAG, enable);
	m_ribbonButtonBarCircuit->EnableButton(ID_RIBBON_ENABLESOL, enable);
    m_ribbonButtonBarCalculations->EnableButton(ID_RIBBON_FAULT, enable);
    m_ribbonButtonBarHandling->EnableButton(ID_RIBBON_FIT, enable);
    m_ribbonButtonBarHandling->EnableButton(ID_RIBBON_MOVE, enable);
    m_ribbonButtonBarClipboard->EnableButton(ID_RIBBON_PASTE, enable);
    m_ribbonButtonBarCalculations->EnableButton(ID_RIBBON_POWERFLOW, enable);
    m_ribbonButtonBarClipboard->EnableButton(ID_RIBBON_REDO, enable);
    m_ribbonButtonBarCircuit->EnableButton(ID_RIBBON_RESETVOLT, enable);
    m_ribbonButtonBarStability->EnableButton(ID_RIBBON_RUNSTAB, enable);
    m_ribbonButtonBarCProject->EnableButton(ID_RIBBON_SAVE, enable);
    m_ribbonButtonBarCProject->EnableButton(ID_RIBBON_SAVEAS, enable);
    m_ribbonButtonBarCalculations->EnableButton(ID_RIBBON_SCPOWER, enable);
    m_ribbonButtonBarCProject->EnableButton(ID_RIBBON_SETTINGS, enable);
    m_ribbonButtonBarReports->EnableButton(ID_RIBBON_SNAPSHOT, enable);
    m_ribbonButtonBarStability->EnableButton(ID_RIBBON_STABSETTINGS, enable);
    m_ribbonButtonBarClipboard->EnableButton(ID_RIBBON_UNDO, enable);
}

void MainFrame::OnNewClick(wxRibbonButtonBarEvent& event) { EnableCurrentProjectRibbon(); }
void MainFrame::OnAboutClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnAddElementDropdown(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnChartsClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnCloseClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnCopyClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnDataReportClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnDeleteClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnDisableSolutionClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnDragClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnEnableSolutionClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnExitClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnExpImpClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnFaultClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnFitClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnMoveClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnOpenClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnPSPGuideClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnPasteClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnPowerFlowClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnProjectSettingsClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnRedoClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnResetVoltagesClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnRunStabilityClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnSCPowerClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnSaveAsClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnSaveClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnSnapshotClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnStabilitySettingsClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnUndoClick(wxRibbonButtonBarEvent& event) {}
