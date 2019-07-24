//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: MainFrame.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _PSP_PROJECT_MAINFRAME_BASE_CLASSES_H
#define _PSP_PROJECT_MAINFRAME_BASE_CLASSES_H

#include <wx/artprov.h>
#include <wx/aui/auibook.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/ribbon/art.h>
#include <wx/ribbon/bar.h>
#include <wx/ribbon/buttonbar.h>
#include <wx/ribbon/page.h>
#include <wx/ribbon/panel.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/xrc/xmlres.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/treebook.h>
#endif

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

class MainFrameBase : public wxFrame
{
   public:
    enum {
        ID_RIBBON_FREQRESP = 10001,
        ID_RIBBON_SCPOWER = 10002,
        ID_RIBBON_FAULT = 10003,
        ID_RIBBON_RUNSTAB = 10004,
        ID_RIBBON_POWERFLOW = 10005,
        ID_RIBBON_DISABLESOL = 10006,
        ID_RIBBON_ENABLESOL = 10007,
        ID_RIBBON_SNAPSHOT = 10008,
        ID_RIBBON_PROJSETTINGS = 10009,
        ID_RIBBON_CHARTS = 10010,
        ID_RIBBON_ROTATEC = 10011,
        ID_RIBBON_SIMULSETTINGS = 10012,
        ID_RIBBON_DATAREPORT = 10013,
        ID_RIBBON_FIT = 10014,
        ID_RIBBON_MOVE = 10015,
        ID_RIBBON_DRAG = 10016,
        ID_RIBBON_ADDELEMENT = 10017,
        ID_RIBBON_DELETE = 10018,
        ID_RIBBON_REDO = 10019,
        ID_RIBBON_UNDO = 10020,
        ID_RIBBON_PASTE = 10021,
        ID_RIBBON_ABOUT = 10022,
        ID_RIBBON_GUIDE = 10023,
        ID_RIBBON_HARMDIST = 10024,
        ID_RIBBON_CLOSE = 10025,
        ID_RIBBON_SAVEAS = 10026,
        ID_RIBBON_SAVE = 10027,
        ID_RIBBON_EXIT = 10028,
        ID_RIBBON_GENSETTINGS = 10029,
        ID_RIBBON_ROTATECC = 10030,
        ID_RIBBON_COPY = 10031,
        ID_RIBBON_IMPORT = 10032,
        ID_RIBBON_OPEN = 10033,
        ID_RIBBON_RESETVOLT = 10034,
        ID_RIBBON_NEW = 10035,
    };

   protected:
    wxRibbonBar* m_ribbonBar;
    wxRibbonPage* m_ribbonPageFile;
    wxRibbonPanel* m_ribbonPanelProjects;
    wxRibbonButtonBar* m_ribbonButtonBarProjects;
    wxRibbonPanel* m_ribbonPanelCProject;
    wxRibbonButtonBar* m_ribbonButtonBarCProject;
    wxRibbonPanel* m_ribbonPanelHelp;
    wxRibbonButtonBar* m_ribbonButtonBarHelp;
    wxRibbonPage* m_ribbonPageTools;
    wxRibbonPanel* m_ribbonPanelClipboard;
    wxRibbonButtonBar* m_ribbonButtonBarClipboard;
    wxRibbonPanel* m_ribbonPanelCircuit;
    wxRibbonButtonBar* m_ribbonButtonBarCircuit;
    wxRibbonPanel* m_ribbonPanelReports;
    wxRibbonButtonBar* m_ribbonButtonBarReports;
    wxRibbonPage* m_ribbonPageSimulation;
    wxRibbonPanel* m_ribbonPanelContinuous;
    wxRibbonButtonBar* m_ribbonButtonBarContinuous;
    wxRibbonPanel* m_ribbonPanelSimulations;
    wxRibbonButtonBar* m_ribbonButtonBarSimulations;
    wxAuiNotebook* m_auiNotebook;
    wxStatusBar* m_statusBar;

   protected:
    virtual void OnNewClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnOpenClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnImportClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnGeneralSettingsClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnExitClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnSaveClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnSaveAsClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnCloseClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnPSPGuideClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnAboutClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnCopyClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnPasteClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnUndoClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnRedoClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnAddElementDropdown(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnDragClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnMoveClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnDeleteClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnFitClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnRotClockClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnRotCounterClockClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnDataReportClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnChartsClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnSnapshotClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnEnableSolutionClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnDisableSolutionClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnResetVoltagesClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnPowerFlowClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnRunStabilityClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnStabilityDropdown(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnFaultClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnSCPowerClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnHarmDistortionsClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnFreqResponseClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void OnSimulationSettingsClick(wxRibbonButtonBarEvent& event) { event.Skip(); }
    virtual void NotebookPageClosing(wxAuiNotebookEvent& event) { event.Skip(); }
    virtual void NotebookPageClosed(wxAuiNotebookEvent& event) { event.Skip(); }

   public:
    wxRibbonBar* GetRibbonBar() { return m_ribbonBar; }
    wxAuiNotebook* GetAuiNotebook() { return m_auiNotebook; }
    wxStatusBar* GetStatusBar() { return m_statusBar; }
    MainFrameBase(wxWindow* parent,
                  wxWindowID id = wxID_ANY,
                  const wxString& title = _("PSP-UFU"),
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxSize(800, 600),
                  long style = wxCAPTION | wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX | wxSYSTEM_MENU |
                               wxCLOSE_BOX);
    virtual ~MainFrameBase();
};

#endif
