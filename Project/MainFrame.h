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

#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/glcanvas.h>

#include "base/MainFrameBase.h"

class MainFrameBase;
class wxRibbonMetroArtProvider;
class Workspace;
class FileHanding;
class GeneralPropertiesForm;
class SimulationsSettingsForm;
class PropertiesData;
class ChartView;
class DataReport;
class AboutForm;
class ImportForm;

enum {
    ID_ADDMENU_BUS = 20000,
    ID_ADDMENU_LINE,
    ID_ADDMENU_TRANSFORMER,
    ID_ADDMENU_GENERATOR,
    ID_ADDMENU_LOAD,
    ID_ADDMENU_CAPACITOR,
    ID_ADDMENU_INDUCTOR,
    ID_ADDMENU_INDMOTOR,
    ID_ADDMENU_SYNCCOMP
};

/**
 * @class MainFrame
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 19/09/2017
 * @brief Main frame of the program. This class manage the ribbon menu and the notebook behavior.
 * @file MainFrame.h
 */
class MainFrame : public MainFrameBase
{
   public:
    /**
     * @brief Default constructor.
     */

    MainFrame();
    /**
     * @brief Main frame contructor.
     * @param parent Parent window.
     * @param locale Locale settings.
     * @param initProperties Initial properties.
     */
    MainFrame(wxWindow* parent, wxLocale* locale, PropertiesData* initProperties, wxString openPath = "");

    /**
     * @brief Default destructor.
     */
    ~MainFrame();

   protected:
    virtual void OnGeneralSettingsClick(wxRibbonButtonBarEvent& event);
    virtual void OnSimulationSettingsClick(wxRibbonButtonBarEvent& event);
    virtual void OnRotClockClick(wxRibbonButtonBarEvent& event);
    virtual void OnRotCounterClockClick(wxRibbonButtonBarEvent& event);
    virtual void NotebookPageClosed(wxAuiNotebookEvent& event);
    virtual void NotebookPageClosing(wxAuiNotebookEvent& event);
    virtual void OnAboutClick(wxRibbonButtonBarEvent& event);
    virtual void OnAddElementDropdown(wxRibbonButtonBarEvent& event);
    virtual void OnChartsClick(wxRibbonButtonBarEvent& event);
    virtual void OnCloseClick(wxRibbonButtonBarEvent& event);
    virtual void OnCopyClick(wxRibbonButtonBarEvent& event);
    virtual void OnDataReportClick(wxRibbonButtonBarEvent& event);
    virtual void OnDeleteClick(wxRibbonButtonBarEvent& event);
    virtual void OnDisableSolutionClick(wxRibbonButtonBarEvent& event);
    virtual void OnDragClick(wxRibbonButtonBarEvent& event);
    virtual void OnEnableSolutionClick(wxRibbonButtonBarEvent& event);
    virtual void OnExitClick(wxRibbonButtonBarEvent& event) { this->Close(); };
    //virtual void OnExpImpClick(wxRibbonButtonBarEvent& event);
    virtual void OnFaultClick(wxRibbonButtonBarEvent& event);
    virtual void OnFitClick(wxRibbonButtonBarEvent& event);
    virtual void OnMoveClick(wxRibbonButtonBarEvent& event);
    virtual void OnOpenClick(wxRibbonButtonBarEvent& event);
    virtual void OnPSPGuideClick(wxRibbonButtonBarEvent& event);
    virtual void OnPasteClick(wxRibbonButtonBarEvent& event);
    virtual void OnPowerFlowClick(wxRibbonButtonBarEvent& event);
    virtual void OnRedoClick(wxRibbonButtonBarEvent& event);
    virtual void OnResetVoltagesClick(wxRibbonButtonBarEvent& event);
    virtual void OnRunStabilityClick(wxRibbonButtonBarEvent& event);
    virtual void OnSCPowerClick(wxRibbonButtonBarEvent& event);
    virtual void OnSaveAsClick(wxRibbonButtonBarEvent& event);
    virtual void OnSaveClick(wxRibbonButtonBarEvent& event);
    virtual void OnSnapshotClick(wxRibbonButtonBarEvent& event);
    virtual void OnUndoClick(wxRibbonButtonBarEvent& event);
    virtual void OnNewClick(wxRibbonButtonBarEvent& event);
    virtual void OnImportClick(wxRibbonButtonBarEvent& event);

    std::vector<Workspace*> m_workspaceList;
    int m_projectNumber = 1;

    wxRibbonMetroArtProvider* m_artMetro = NULL;
    wxMenu* m_addElementsMenu = NULL;
    wxLocale* m_locale = NULL;
    PropertiesData* m_generalProperties = NULL;
    wxGLContext* m_sharedGLContext = NULL;

    void Init();
    void EnableCurrentProjectRibbon(bool enable = true);
    void CreateAddElementsMenu();

    void OnAddElementsClick(wxCommandEvent& event);
};

#endif  // MAINFRAME_H
