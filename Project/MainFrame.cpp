#include "MainFrame.h"
#include "ArtMetro.h"
#include "Workspace.h"
#include "Bus.h"
#include "Line.h"
#include "Transformer.h"
#include "SyncGenerator.h"
#include "IndMotor.h"
#include "SyncMotor.h"
#include "Load.h"
#include "Inductor.h"
#include "Capacitor.h"
#include "FileHanding.h"
#include "GeneralPropertiesForm.h"
#include "SimulationsSettingsForm.h"
#include "PropertiesData.h"

MainFrame::MainFrame() : MainFrameBase(NULL) {}
MainFrame::MainFrame(wxWindow* parent, wxLocale* locale, PropertiesData* initProperties) : MainFrameBase(parent)
{
    m_locale = locale;
    m_generalProperties = initProperties;

    Init();
}

MainFrame::~MainFrame()
{
    // if(m_artMetro) delete m_artMetro;
    if(m_addElementsMenu) {
        m_addElementsMenu->Disconnect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnAddElementsClick),
                                      NULL, this);
        delete m_addElementsMenu;
    }
    if(m_locale) delete m_locale;
    if(m_generalProperties) delete m_generalProperties;
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
    m_ribbonButtonBarReports->EnableButton(ID_RIBBON_CHARTS, enable);
    m_ribbonButtonBarCProject->EnableButton(ID_RIBBON_CLOSE, enable);
    m_ribbonButtonBarClipboard->EnableButton(ID_RIBBON_COPY, enable);
    m_ribbonButtonBarReports->EnableButton(ID_RIBBON_DATAREPORT, enable);
    m_ribbonButtonBarCircuit->EnableButton(ID_RIBBON_DELETE, enable);
    m_ribbonButtonBarContinuous->EnableButton(ID_RIBBON_DISABLESOL, enable);
    m_ribbonButtonBarCircuit->EnableButton(ID_RIBBON_DRAG, enable);
    m_ribbonButtonBarContinuous->EnableButton(ID_RIBBON_ENABLESOL, enable);
    m_ribbonButtonBarSimulations->EnableButton(ID_RIBBON_FAULT, enable);
    m_ribbonButtonBarCircuit->EnableButton(ID_RIBBON_FIT, enable);
    m_ribbonButtonBarCircuit->EnableButton(ID_RIBBON_MOVE, enable);
    m_ribbonButtonBarClipboard->EnableButton(ID_RIBBON_PASTE, enable);
    m_ribbonButtonBarSimulations->EnableButton(ID_RIBBON_POWERFLOW, enable);
    m_ribbonButtonBarClipboard->EnableButton(ID_RIBBON_REDO, enable);
    m_ribbonButtonBarContinuous->EnableButton(ID_RIBBON_RESETVOLT, enable);
    m_ribbonButtonBarSimulations->EnableButton(ID_RIBBON_RUNSTAB, enable);
    m_ribbonButtonBarCProject->EnableButton(ID_RIBBON_SAVE, enable);
    m_ribbonButtonBarCProject->EnableButton(ID_RIBBON_SAVEAS, enable);
    m_ribbonButtonBarSimulations->EnableButton(ID_RIBBON_SCPOWER, enable);
    m_ribbonButtonBarCircuit->EnableButton(ID_RIBBON_PROJSETTINGS, enable);
    m_ribbonButtonBarReports->EnableButton(ID_RIBBON_SNAPSHOT, enable);
    m_ribbonButtonBarSimulations->EnableButton(ID_RIBBON_SIMULSETTINGS, enable);
    m_ribbonButtonBarClipboard->EnableButton(ID_RIBBON_UNDO, enable);
    m_ribbonButtonBarCircuit->EnableButton(ID_RIBBON_ROTATEC, enable);
    m_ribbonButtonBarCircuit->EnableButton(ID_RIBBON_ROTATECC, enable);
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

    m_addElementsMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddElementsClick, this);
}

void MainFrame::OnNewClick(wxRibbonButtonBarEvent& event)
{
    EnableCurrentProjectRibbon();

    Workspace* newWorkspace =
        new Workspace(this, wxString::Format(_("New project %d"), m_projectNumber), this->GetStatusBar());
    m_workspaceList.push_back(newWorkspace);

    m_ribbonButtonBarContinuous->ToggleButton(ID_RIBBON_DISABLESOL, true);
    m_ribbonButtonBarContinuous->ToggleButton(ID_RIBBON_ENABLESOL, false);

    m_auiNotebook->AddPage(newWorkspace, newWorkspace->GetName(), true);
    newWorkspace->Redraw();
    m_projectNumber++;
}

void MainFrame::OnAboutClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnAddElementDropdown(wxRibbonButtonBarEvent& event) { event.PopupMenu(m_addElementsMenu); }
void MainFrame::OnChartsClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnCloseClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnCopyClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnDataReportClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnDeleteClick(wxRibbonButtonBarEvent& event)
{
    Workspace* workspace = static_cast<Workspace*>(m_auiNotebook->GetCurrentPage());
    if(workspace) {
        workspace->DeleteSelectedElements();
    }
}
void MainFrame::OnDisableSolutionClick(wxRibbonButtonBarEvent& event)
{
    m_ribbonButtonBarContinuous->ToggleButton(ID_RIBBON_DISABLESOL, true);
    m_ribbonButtonBarContinuous->ToggleButton(ID_RIBBON_ENABLESOL, false);
}

void MainFrame::OnDragClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnEnableSolutionClick(wxRibbonButtonBarEvent& event)
{
    m_ribbonButtonBarContinuous->ToggleButton(ID_RIBBON_ENABLESOL, true);
    m_ribbonButtonBarContinuous->ToggleButton(ID_RIBBON_DISABLESOL, false);
}

void MainFrame::OnExpImpClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnFaultClick(wxRibbonButtonBarEvent& event)
{
    if(Workspace* workspace = dynamic_cast<Workspace*>(m_auiNotebook->GetCurrentPage())) {
        workspace->RunFault();
    }
}

void MainFrame::OnFitClick(wxRibbonButtonBarEvent& event)
{
    Workspace* workspace = static_cast<Workspace*>(m_auiNotebook->GetCurrentPage());
    if(workspace) {
        workspace->Fit();
    }
}

void MainFrame::OnMoveClick(wxRibbonButtonBarEvent& event)
{
    Workspace* workspace = static_cast<Workspace*>(m_auiNotebook->GetCurrentPage());
    if(workspace) {
        auto elementList = workspace->GetAllElements();
        // Calculate the average position of selected elements.
        wxPoint2DDouble averagePos(0, 0);
        int numSelElements = 0;
        for(auto it = elementList.begin(), itEnd = elementList.end(); it != itEnd; ++it) {
            Element* element = *it;
            if(element->IsSelected()) {
                averagePos += element->GetPosition();
                numSelElements++;
            }
        }
        averagePos = wxPoint2DDouble(averagePos.m_x / double(numSelElements), averagePos.m_y / double(numSelElements));
        // Set the move position to the average of selected elements.
        for(auto it = elementList.begin(), itEnd = elementList.end(); it != itEnd; ++it) {
            Element* element = *it;
            if(element->IsSelected()) {
                element->StartMove(averagePos);
            }
        }
        workspace->SetWorkspaceMode(Workspace::MODE_MOVE_ELEMENT);
    }
}

void MainFrame::OnOpenClick(wxRibbonButtonBarEvent& event)
{
    wxFileDialog openFileDialog(this, _("Open PSP file"), "", "", "PSP files (*.psp)|*.psp",
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if(openFileDialog.ShowModal() == wxID_CANCEL) return;

    wxFileName fileName(openFileDialog.GetPath());

    EnableCurrentProjectRibbon();
    Workspace* newWorkspace = new Workspace(this, _("Open project"), this->GetStatusBar());

    FileHanding fileHandling(newWorkspace);
    if(fileHandling.OpenProject(fileName)) {
        newWorkspace->SetSavedPath(fileName);

        m_workspaceList.push_back(newWorkspace);

        m_ribbonButtonBarContinuous->ToggleButton(ID_RIBBON_DISABLESOL, true);
        m_ribbonButtonBarContinuous->ToggleButton(ID_RIBBON_ENABLESOL, false);

        m_auiNotebook->AddPage(newWorkspace, newWorkspace->GetName(), true);
        m_auiNotebook->Layout();
        newWorkspace->Redraw();
        newWorkspace->SetJustOpened(true);
        m_projectNumber++;
    } else {
        wxMessageDialog msgDialog(this, _("It was not possible to open the selected file."), _("Error"),
                                  wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
        delete newWorkspace;
    }
}

void MainFrame::OnPSPGuideClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnPasteClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnPowerFlowClick(wxRibbonButtonBarEvent& event)
{
    Workspace* workspace = static_cast<Workspace*>(m_auiNotebook->GetCurrentPage());
    if(workspace) {
        workspace->RunPowerFlow();
    }
}

void MainFrame::OnRedoClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnResetVoltagesClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnRunStabilityClick(wxRibbonButtonBarEvent& event)
{
    Workspace* workspace = static_cast<Workspace*>(m_auiNotebook->GetCurrentPage());
    if(workspace) {
        workspace->RunStability();
    }
}

void MainFrame::OnSCPowerClick(wxRibbonButtonBarEvent& event)
{
    Workspace* workspace = static_cast<Workspace*>(m_auiNotebook->GetCurrentPage());
    if(workspace) {
        workspace->RunSCPower();
    }
}
void MainFrame::OnSaveAsClick(wxRibbonButtonBarEvent& event)
{
    Workspace* workspace = static_cast<Workspace*>(m_auiNotebook->GetCurrentPage());
    if(workspace) {
        FileHanding fileHandling(workspace);

        wxFileDialog saveFileDialog(this, _("Save PSP file"), "", "", "PSP files (*.psp)|*.psp",
                                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(saveFileDialog.ShowModal() == wxID_CANCEL) return;

        fileHandling.SaveProject(saveFileDialog.GetPath());
        wxFileName fileName(saveFileDialog.GetPath());
        workspace->SetName(fileName.GetName());
        m_auiNotebook->SetPageText(m_auiNotebook->GetPageIndex(workspace), workspace->GetName());
        workspace->SetSavedPath(fileName);
    }
}

void MainFrame::OnSaveClick(wxRibbonButtonBarEvent& event)
{
    Workspace* workspace = static_cast<Workspace*>(m_auiNotebook->GetCurrentPage());
    if(workspace) {
        FileHanding fileHandling(workspace);

        if(workspace->GetSavedPath().IsOk()) {
            fileHandling.SaveProject(workspace->GetSavedPath());
        } else {
            wxFileDialog saveFileDialog(this, _("Save PSP file"), "", "", "PSP files (*.psp)|*.psp",
                                        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
            if(saveFileDialog.ShowModal() == wxID_CANCEL) return;

            fileHandling.SaveProject(saveFileDialog.GetPath());
            wxFileName fileName(saveFileDialog.GetPath());
            workspace->SetName(fileName.GetName());
            m_auiNotebook->SetPageText(m_auiNotebook->GetPageIndex(workspace), workspace->GetName());
            workspace->SetSavedPath(fileName);
        }
    }
}

void MainFrame::OnSnapshotClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnUndoClick(wxRibbonButtonBarEvent& event) {}
void MainFrame::OnAddElementsClick(wxCommandEvent& event)
{
    Workspace* workspace = static_cast<Workspace*>(m_auiNotebook->GetCurrentPage());

    if(workspace) {
        if(workspace->GetWorkspaceMode() != Workspace::MODE_INSERT) {
            auto elementList = workspace->GetElementList();
            wxString statusBarText = "";
            bool newElement = false;

            switch(event.GetId()) {
                case ID_ADDMENU_BUS: {
                    Bus* newBus = new Bus(wxPoint2DDouble(0, 0),
                                          wxString::Format(_("Bus %d"), workspace->GetElementNumber(ID_BUS)));
                    workspace->IncrementElementNumber(ID_BUS);
                    elementList.push_back(newBus);
                    statusBarText = _("Insert Bus: Click to insert, ESC to cancel.");
                    newElement = true;
                } break;
                case ID_ADDMENU_LINE: {
                    Line* newLine = new Line(wxString::Format(_("Line %d"), workspace->GetElementNumber(ID_LINE)));
                    elementList.push_back(newLine);
                    workspace->IncrementElementNumber(ID_LINE);
                    statusBarText = _("Insert Line: Click on two buses, ESC to cancel.");
                    newElement = true;
                } break;
                case ID_ADDMENU_TRANSFORMER: {
                    Transformer* newTransformer = new Transformer(
                        wxString::Format(_("Transformer %d"), workspace->GetElementNumber(ID_TRANSFORMER)));
                    workspace->IncrementElementNumber(ID_TRANSFORMER);
                    elementList.push_back(newTransformer);
                    statusBarText = _("Insert Transformer: Click on two buses, ESC to cancel.");
                    newElement = true;
                } break;
                case ID_ADDMENU_GENERATOR: {
                    SyncGenerator* newGenerator = new SyncGenerator(
                        wxString::Format(_("Generator %d"), workspace->GetElementNumber(ID_SYNCGENERATOR)));
                    workspace->IncrementElementNumber(ID_SYNCGENERATOR);
                    elementList.push_back(newGenerator);
                    statusBarText = _("Insert Generator: Click on a buses, ESC to cancel.");
                    newElement = true;
                } break;
                case ID_ADDMENU_LOAD: {
                    Load* newLoad = new Load(wxString::Format(_("Load %d"), workspace->GetElementNumber(ID_LOAD)));
                    workspace->IncrementElementNumber(ID_LOAD);
                    elementList.push_back(newLoad);
                    statusBarText = _("Insert Load: Click on a buses, ESC to cancel.");
                    newElement = true;
                } break;
                case ID_ADDMENU_CAPACITOR: {
                    Capacitor* newCapacitor =
                        new Capacitor(wxString::Format(_("Capacitor %d"), workspace->GetElementNumber(ID_CAPACITOR)));
                    workspace->IncrementElementNumber(ID_CAPACITOR);
                    elementList.push_back(newCapacitor);
                    statusBarText = _("Insert Capacitor: Click on a buses, ESC to cancel.");
                    newElement = true;
                } break;
                case ID_ADDMENU_INDUCTOR: {
                    Inductor* newInductor =
                        new Inductor(wxString::Format(_("Inductor %d"), workspace->GetElementNumber(ID_INDUCTOR)));
                    workspace->IncrementElementNumber(ID_INDUCTOR);
                    elementList.push_back(newInductor);
                    statusBarText = _("Insert Inductor: Click on a buses, ESC to cancel.");
                    newElement = true;
                } break;
                case ID_ADDMENU_INDMOTOR: {
                    IndMotor* newIndMotor = new IndMotor(
                        wxString::Format(_("Induction motor %d"), workspace->GetElementNumber(ID_INDMOTOR)));
                    workspace->IncrementElementNumber(ID_INDMOTOR);
                    elementList.push_back(newIndMotor);
                    statusBarText = _("Insert Induction Motor: Click on a buses, ESC to cancel.");
                    newElement = true;
                } break;
                case ID_ADDMENU_SYNCCOMP: {
                    SyncMotor* newSyncCondenser = new SyncMotor(
                        wxString::Format(_("Synchronous condenser %d"), workspace->GetElementNumber(ID_SYNCMOTOR)));
                    workspace->IncrementElementNumber(ID_SYNCMOTOR);
                    elementList.push_back(newSyncCondenser);
                    statusBarText = _("Insert Synchronous Condenser: Click on a buses, ESC to cancel.");
                    newElement = true;
                } break;
            }
            if(newElement) {
                workspace->SetElementList(elementList);
                workspace->SetWorkspaceMode(Workspace::MODE_INSERT);
                workspace->SetStatusBarText(statusBarText);
                workspace->Redraw();
            }
        }
    }
}
void MainFrame::NotebookPageClosed(wxAuiNotebookEvent& event)
{
    if(m_auiNotebook->GetPageCount() == 0) EnableCurrentProjectRibbon(false);
}
void MainFrame::NotebookPageClosing(wxAuiNotebookEvent& event)
{
    auto it = m_workspaceList.begin();
    while(it != m_workspaceList.end()) {
        if(*it == m_auiNotebook->GetCurrentPage()) {
            m_workspaceList.erase(it);
            break;
        }
        it++;
    }
    event.Skip();
}
void MainFrame::OnRotClockClick(wxRibbonButtonBarEvent& event)
{
    Workspace* workspace = static_cast<Workspace*>(m_auiNotebook->GetCurrentPage());
    if(workspace) {
        workspace->RotateSelectedElements();
    }
}
void MainFrame::OnRotCounterClockClick(wxRibbonButtonBarEvent& event)
{
    Workspace* workspace = static_cast<Workspace*>(m_auiNotebook->GetCurrentPage());
    if(workspace) {
        workspace->RotateSelectedElements(false);
    }
}
void MainFrame::OnGeneralSettingsClick(wxRibbonButtonBarEvent& event)
{
    GeneralPropertiesForm genPropForm(this, m_generalProperties);
    genPropForm.SetInitialSize();
    genPropForm.ShowModal();
}

void MainFrame::OnSimulationSettingsClick(wxRibbonButtonBarEvent& event)
{
    Workspace* workspace = static_cast<Workspace*>(m_auiNotebook->GetCurrentPage());
    if(workspace) {
        SimulationsSettingsForm simulSettingsForm(this, workspace->GetProperties());
        simulSettingsForm.SetInitialSize();
        simulSettingsForm.ShowModal();
    }
}
