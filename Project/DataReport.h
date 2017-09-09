#ifndef DATAREPORT_H
#define DATAREPORT_H

#include "DataReportBase.h"

class Workspace;
class ElectricCalculation;

class DataReport : public DataReportBase
{
   public:
    enum GridSelection {
        GRID_ALL = 0,
        GRID_PF,
        GRID_PFBUSES,
        GRID_PFBRANCHES,
        GRID_FAULT,
        GRID_FAULTBUSES,
        GRID_FAULTBRANCHES,
        GRID_FAULTGENERATORS,
    };

    DataReport(wxWindow* parent, Workspace* workspace);
    virtual ~DataReport();

    virtual void SetHeaders();
    virtual void CreateGrids();
    virtual void FillValues(GridSelection gridToFill = GRID_ALL);
    virtual void SetRowsColours(wxGrid* grid, int rowStart = 1);

   protected:
    virtual void OnFaulrGridChanged(wxGridEvent& event);
    virtual void OnFaultBranchesGridChanged(wxGridEvent& event);
    virtual void OnFaultBusesGridChanged(wxGridEvent& event);
    virtual void OnFaultGeneratorsGridChanged(wxGridEvent& event);
    virtual void OnPFBranchesGridChanged(wxGridEvent& event);
    virtual void OnPowerFlowGridChanged(wxGridEvent& event);
    virtual void OnPFBusGridChanged(wxGridEvent& event);
    Workspace* m_workspace = NULL;
    bool m_changingValues = false;

    // Headers choices
    wxArrayString m_voltageChoices;
    wxArrayString m_activePowerChoices;
    wxArrayString m_reactivePowerChoices;
    wxArrayString m_resistanceChoices;
    wxArrayString m_indReactanceChoices;
    wxArrayString m_capSusceptanceChoices;
    wxArrayString m_currentChoices;

    // Colors
    wxColour m_headerColour;
    wxColour m_offlineColour;
    wxColour m_oddRowColour;
    wxColour m_evenRowColour;
};
#endif  // DATAREPORT_H
