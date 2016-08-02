#include "MainFrame.h"
#include "ArtMetro.h"
#include "Workspace.h"

MainFrame::MainFrame(wxWindow* parent, wxLocale* locale) : MainFrameBase(parent)
{
    m_locale = locale;

    Init();
}
MainFrame::~MainFrame()
{
    if(!m_artMetro) delete m_artMetro;
    if(!m_addElementsMenu) {
	    m_addElementsMenu->Disconnect(wxEVT_COMMAND_MENU_SELECTED,
	                                  wxCommandEventHandler(MainFrame::OnAddElementsClick), NULL, this);
	    delete m_addElementsMenu;
	}
}
void MainFrame::Init()
{
    this->SetSize(800, 600);

    CreateAddElementsMenu();

    EnableCurrentProjectRibbon(false);

    m_artMetro = new wxRibbonMetroArtProvider();
    m_ribbonBar->SetArtProvider(m_artMetro);
    m_ribbonBar->Realize();

    this->Layout();
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

void MainFrame::CreateAddElementsMenu()
{
    m_addElementsMenu = new wxMenu();

    wxMenuItem* busElement =
        new wxMenuItem(m_addElementsMenu, ID_ADDMENU_BUS, _("&Bus\tB"), _("Adds a bus at the circuit"));
    // busElement->SetBitmap(wxArtProvider::GetBitmap(wxART_WARNING));
    wxMenuItem* lineElement =
        new wxMenuItem(m_addElementsMenu, ID_ADDMENU_LINE, _("&Line\tL"), _("Adds a power line at the circuit"));
    wxMenuItem* transformerElement = new wxMenuItem(m_addElementsMenu, ID_ADDMENU_TRANSFORMER, _("&Transformer\tT"),
                                                    _("Adds a transformer at the circuit"));
    wxMenuItem* generatorElement = new wxMenuItem(m_addElementsMenu, ID_ADDMENU_GENERATOR, _("&Generator\tG"),
                                                  _("Adds a generator at the circuit"));
    wxMenuItem* indMotorElement = new wxMenuItem(m_addElementsMenu, ID_ADDMENU_INDMOTOR, _("&Induction motor\tI"),
                                                 _("Adds an induction motor at the circuit"));
    wxMenuItem* syncCompElement =
        new wxMenuItem(m_addElementsMenu, ID_ADDMENU_SYNCCOMP, _("&Synchronous compensator \tK"),
                       _("Adds an induction motor at the circuit"));
    wxMenuItem* loadElement =
        new wxMenuItem(m_addElementsMenu, ID_ADDMENU_LOAD, _("&Load\tShift-L"), _("Adds a load at the circuit"));
    wxMenuItem* capacitorElement = new wxMenuItem(m_addElementsMenu, ID_ADDMENU_CAPACITOR, _("&Capacitor\tShift-C"),
                                                  _("Adds a shunt capacitor at the circuit"));
    wxMenuItem* inductorElement = new wxMenuItem(m_addElementsMenu, ID_ADDMENU_INDUCTOR, _("&Inductor\tShift-I"),
                                                 _("Adds a shunt inductor at the circuit"));

    m_addElementsMenu->Append(busElement);
    m_addElementsMenu->Append(lineElement);
    m_addElementsMenu->Append(transformerElement);
    m_addElementsMenu->Append(generatorElement);
    m_addElementsMenu->Append(indMotorElement);
    m_addElementsMenu->Append(syncCompElement);
    m_addElementsMenu->Append(loadElement);
    m_addElementsMenu->Append(capacitorElement);
    m_addElementsMenu->Append(inductorElement);

    m_addElementsMenu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnAddElementsClick), NULL,
                               this);
}

void MainFrame::OnNewClick(wxRibbonButtonBarEvent& event)
{
    EnableCurrentProjectRibbon();

    Workspace* newWorkspace =
        new Workspace(this, wxString::Format(_("New project %d"), m_projectNumber), this->GetStatusBar());
    m_workspaceList.push_back(newWorkspace);

    m_ribbonButtonBarCircuit->ToggleButton(ID_RIBBON_DISABLESOL, true);
    m_ribbonButtonBarCircuit->ToggleButton(ID_RIBBON_ENABLESOL, false);

    m_auiNotebook->AddPage(newWorkspace, newWorkspace->GetName(), true);
    newWorkspace->Layout();
    newWorkspace->Redraw();
    m_projectNumber++;
}

void MainFrame::OnAboutClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnAddElementDropdown(wxRibbonButtonBarEvent& event) { event.PopupMenu(m_addElementsMenu); }
void MainFrame::OnChartsClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnCloseClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnCopyClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnDataReportClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnDeleteClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnDisableSolutionClick(wxRibbonButtonBarEvent& event)
{
    m_ribbonButtonBarCircuit->ToggleButton(ID_RIBBON_DISABLESOL, true);
    m_ribbonButtonBarCircuit->ToggleButton(ID_RIBBON_ENABLESOL, false);
}

void MainFrame::OnDragClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnEnableSolutionClick(wxRibbonButtonBarEvent& event)
{
    m_ribbonButtonBarCircuit->ToggleButton(ID_RIBBON_ENABLESOL, true);
    m_ribbonButtonBarCircuit->ToggleButton(ID_RIBBON_DISABLESOL, false);
}

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
void MainFrame::OnAddElementsClick(wxCommandEvent& event)
{
    switch(event.GetId())
	{
	    case ID_ADDMENU_BUS:
		// inserir barra
		break;
	    case ID_ADDMENU_LINE:
		// inserir linha
		break;
	    case ID_ADDMENU_TRANSFORMER:
		// inserir transformador
		break;
	    case ID_ADDMENU_GENERATOR:
		// inserir gerador
		break;
	    case ID_ADDMENU_LOAD:
		// inserir larga
		break;
	    case ID_ADDMENU_CAPACITOR:
		// inserir capacitor
		break;
	    case ID_ADDMENU_INDUCTOR:
		// inserir indutor
		break;
	    case ID_ADDMENU_INDMOTOR:
		// inserir motor
		break;
	    case ID_ADDMENU_SYNCCOMP:
		// inserir compensador sincrono
		break;
	}
}
void MainFrame::NotebookPageClosed(wxAuiNotebookEvent& event)
{	
    if(m_auiNotebook->GetPageCount() == 0) EnableCurrentProjectRibbon(false);
	//Memory leak?
}
void MainFrame::NotebookPageClosing(wxAuiNotebookEvent& event)
{
    std::vector<Workspace*>::iterator it = m_workspaceList.begin();
    while(it != m_workspaceList.end()) {
	    Workspace* workspace = *it;

	    if(event.GetSelection() == m_auiNotebook->GetPageIndex(workspace)) {
		    //delete workspace; //Memory leak?
			m_workspaceList.erase(it);
		    break;
		}
	    it++;
	}
	event.Skip();
}
