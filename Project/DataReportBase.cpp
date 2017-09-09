//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: DataReport.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "DataReportBase.h"


// Declare the bitmap loading function
extern void wxC6A63InitBitmapResources();

static bool bBitmapLoaded = false;


DataReportBase::DataReportBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC6A63InitBitmapResources();
        bBitmapLoaded = true;
    }
    // Set icon(s) to the application/dialog
    wxIconBundle app_icons;
    {
        wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("chart16"));
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        app_icons.AddIcon( icn );
    }
    {
        wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("chart32"));
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        app_icons.AddIcon( icn );
    }
    {
        wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("chart64"));
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        app_icons.AddIcon( icn );
    }
    {
        wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("chart128"));
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        app_icons.AddIcon( icn );
    }
    {
        wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("chart256"));
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        app_icons.AddIcon( icn );
    }
    SetIcons( app_icons );

    
    wxBoxSizer* boxSizerLvl1_1 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizerLvl1_1);
    
    wxBoxSizer* boxSizerLvl2_1 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizerLvl1_1->Add(boxSizerLvl2_1, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    m_notebookDataReport = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxBK_DEFAULT);
    m_notebookDataReport->SetName(wxT("m_notebookDataReport"));
    
    boxSizerLvl2_1->Add(m_notebookDataReport, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelPowerFlowData = new wxPanel(m_notebookDataReport, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebookDataReport, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebookDataReport->AddPage(m_panelPowerFlowData, _("Power flow data"), false);
    
    wxBoxSizer* boxSizerLvl3_1 = new wxBoxSizer(wxVERTICAL);
    m_panelPowerFlowData->SetSizer(boxSizerLvl3_1);
    
    m_notebookPowerFlow = new wxNotebook(m_panelPowerFlowData, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelPowerFlowData, wxSize(-1,-1)), wxBK_LEFT|wxBK_DEFAULT);
    m_notebookPowerFlow->SetName(wxT("m_notebookPowerFlow"));
    
    boxSizerLvl3_1->Add(m_notebookPowerFlow, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelPowerFlow = new wxPanel(m_notebookPowerFlow, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebookPowerFlow, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebookPowerFlow->AddPage(m_panelPowerFlow, _("Power flow"), false);
    
    wxBoxSizer* boxSizerLvl4_1 = new wxBoxSizer(wxVERTICAL);
    m_panelPowerFlow->SetSizer(boxSizerLvl4_1);
    
    m_gridPowerFlow = new wxGrid(m_panelPowerFlow, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelPowerFlow, wxSize(-1,-1)), wxWANTS_CHARS);
    m_gridPowerFlow->CreateGrid(0, 0);
    m_gridPowerFlow->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    m_gridPowerFlow->SetColLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    #if wxVERSION_NUMBER >= 2904
    m_gridPowerFlow->UseNativeColHeader(true);
    #endif
    m_gridPowerFlow->EnableEditing(true);
    
    boxSizerLvl4_1->Add(m_gridPowerFlow, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelPFBuses = new wxPanel(m_notebookPowerFlow, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebookPowerFlow, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebookPowerFlow->AddPage(m_panelPFBuses, _("Buses"), false);
    
    wxBoxSizer* boxSizerLvl4_2 = new wxBoxSizer(wxVERTICAL);
    m_panelPFBuses->SetSizer(boxSizerLvl4_2);
    
    m_gridPFBuses = new wxGrid(m_panelPFBuses, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelPFBuses, wxSize(-1,-1)), wxWANTS_CHARS);
    m_gridPFBuses->CreateGrid(0, 0);
    m_gridPFBuses->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    m_gridPFBuses->SetColLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    #if wxVERSION_NUMBER >= 2904
    m_gridPFBuses->UseNativeColHeader(true);
    #endif
    m_gridPFBuses->EnableEditing(true);
    
    boxSizerLvl4_2->Add(m_gridPFBuses, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelPFBranches = new wxPanel(m_notebookPowerFlow, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebookPowerFlow, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebookPowerFlow->AddPage(m_panelPFBranches, _("Branches"), false);
    
    wxBoxSizer* boxSizerLvl4_3 = new wxBoxSizer(wxVERTICAL);
    m_panelPFBranches->SetSizer(boxSizerLvl4_3);
    
    m_gridPFBranches = new wxGrid(m_panelPFBranches, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelPFBranches, wxSize(-1,-1)), wxWANTS_CHARS);
    m_gridPFBranches->CreateGrid(0, 0);
    m_gridPFBranches->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    m_gridPFBranches->SetColLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    #if wxVERSION_NUMBER >= 2904
    m_gridPFBranches->UseNativeColHeader(true);
    #endif
    m_gridPFBranches->EnableEditing(true);
    
    boxSizerLvl4_3->Add(m_gridPFBranches, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelFaultData = new wxPanel(m_notebookDataReport, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebookDataReport, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebookDataReport->AddPage(m_panelFaultData, _("Fault data"), false);
    
    wxBoxSizer* boxSizerLvl3_2 = new wxBoxSizer(wxVERTICAL);
    m_panelFaultData->SetSizer(boxSizerLvl3_2);
    
    m_notebookFault = new wxNotebook(m_panelFaultData, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelFaultData, wxSize(-1,-1)), wxBK_LEFT|wxBK_DEFAULT);
    m_notebookFault->SetName(wxT("m_notebookFault"));
    
    boxSizerLvl3_2->Add(m_notebookFault, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelFault = new wxPanel(m_notebookFault, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebookFault, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebookFault->AddPage(m_panelFault, _("Fault"), false);
    
    wxBoxSizer* boxSizerLvl4_4 = new wxBoxSizer(wxVERTICAL);
    m_panelFault->SetSizer(boxSizerLvl4_4);
    
    m_gridFault = new wxGrid(m_panelFault, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelFault, wxSize(-1,-1)), wxWANTS_CHARS);
    m_gridFault->CreateGrid(0, 0);
    m_gridFault->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    m_gridFault->SetColLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    #if wxVERSION_NUMBER >= 2904
    m_gridFault->UseNativeColHeader(true);
    #endif
    m_gridFault->EnableEditing(true);
    
    boxSizerLvl4_4->Add(m_gridFault, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelFaultBuses = new wxPanel(m_notebookFault, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebookFault, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebookFault->AddPage(m_panelFaultBuses, _("Buses"), false);
    
    wxBoxSizer* boxSizerLvl4_5 = new wxBoxSizer(wxVERTICAL);
    m_panelFaultBuses->SetSizer(boxSizerLvl4_5);
    
    m_gridFaultBuses = new wxGrid(m_panelFaultBuses, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelFaultBuses, wxSize(-1,-1)), wxWANTS_CHARS);
    m_gridFaultBuses->CreateGrid(0, 0);
    m_gridFaultBuses->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    m_gridFaultBuses->SetColLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    #if wxVERSION_NUMBER >= 2904
    m_gridFaultBuses->UseNativeColHeader(true);
    #endif
    m_gridFaultBuses->EnableEditing(true);
    
    boxSizerLvl4_5->Add(m_gridFaultBuses, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelFaultBranches = new wxPanel(m_notebookFault, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebookFault, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebookFault->AddPage(m_panelFaultBranches, _("Branches"), false);
    
    wxBoxSizer* boxSizerLvl4_6 = new wxBoxSizer(wxVERTICAL);
    m_panelFaultBranches->SetSizer(boxSizerLvl4_6);
    
    m_gridFaultBranches = new wxGrid(m_panelFaultBranches, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelFaultBranches, wxSize(-1,-1)), wxWANTS_CHARS);
    m_gridFaultBranches->CreateGrid(0, 0);
    m_gridFaultBranches->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    m_gridFaultBranches->SetColLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    #if wxVERSION_NUMBER >= 2904
    m_gridFaultBranches->UseNativeColHeader(true);
    #endif
    m_gridFaultBranches->EnableEditing(true);
    
    boxSizerLvl4_6->Add(m_gridFaultBranches, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelFaultGenerators = new wxPanel(m_notebookFault, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebookFault, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebookFault->AddPage(m_panelFaultGenerators, _("Generators"), false);
    
    wxBoxSizer* boxSizerLvl4_118 = new wxBoxSizer(wxVERTICAL);
    m_panelFaultGenerators->SetSizer(boxSizerLvl4_118);
    
    m_gridFaultGenerators = new wxGrid(m_panelFaultGenerators, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelFaultGenerators, wxSize(-1,-1)), wxWANTS_CHARS);
    m_gridFaultGenerators->CreateGrid(0, 0);
    m_gridFaultGenerators->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    m_gridFaultGenerators->SetColLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    #if wxVERSION_NUMBER >= 2904
    m_gridFaultGenerators->UseNativeColHeader(true);
    #endif
    m_gridFaultGenerators->EnableEditing(true);
    
    boxSizerLvl4_118->Add(m_gridFaultGenerators, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    
    #if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(m_notebookDataReport)){
        wxPersistenceManager::Get().RegisterAndRestore(m_notebookDataReport);
    } else {
        wxPersistenceManager::Get().Restore(m_notebookDataReport);
    }
    #endif
    
    #if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(m_notebookPowerFlow)){
        wxPersistenceManager::Get().RegisterAndRestore(m_notebookPowerFlow);
    } else {
        wxPersistenceManager::Get().Restore(m_notebookPowerFlow);
    }
    #endif
    
    #if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(m_notebookFault)){
        wxPersistenceManager::Get().RegisterAndRestore(m_notebookFault);
    } else {
        wxPersistenceManager::Get().Restore(m_notebookFault);
    }
    #endif
    
    SetName(wxT("DataReportBase"));
    SetMinClientSize(wxSize(500,300));
    SetSize(-1,-1);
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
#if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
#endif
    // Connect events
    m_gridPowerFlow->Connect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(DataReportBase::OnPowerFlowGridChanged), NULL, this);
    m_gridPFBuses->Connect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(DataReportBase::OnPFBusGridChanged), NULL, this);
    m_gridPFBranches->Connect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(DataReportBase::OnPFBranchesGridChanged), NULL, this);
    m_gridFault->Connect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(DataReportBase::OnFaulrGridChanged), NULL, this);
    m_gridFaultBuses->Connect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(DataReportBase::OnFaultBusesGridChanged), NULL, this);
    m_gridFaultBranches->Connect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(DataReportBase::OnFaultBranchesGridChanged), NULL, this);
    m_gridFaultGenerators->Connect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(DataReportBase::OnFaultGeneratorsGridChanged), NULL, this);
    
}

DataReportBase::~DataReportBase()
{
    m_gridPowerFlow->Disconnect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(DataReportBase::OnPowerFlowGridChanged), NULL, this);
    m_gridPFBuses->Disconnect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(DataReportBase::OnPFBusGridChanged), NULL, this);
    m_gridPFBranches->Disconnect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(DataReportBase::OnPFBranchesGridChanged), NULL, this);
    m_gridFault->Disconnect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(DataReportBase::OnFaulrGridChanged), NULL, this);
    m_gridFaultBuses->Disconnect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(DataReportBase::OnFaultBusesGridChanged), NULL, this);
    m_gridFaultBranches->Disconnect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(DataReportBase::OnFaultBranchesGridChanged), NULL, this);
    m_gridFaultGenerators->Disconnect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(DataReportBase::OnFaultGeneratorsGridChanged), NULL, this);
    
}
