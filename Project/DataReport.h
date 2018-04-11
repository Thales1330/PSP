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

#ifndef DATAREPORT_H
#define DATAREPORT_H

#include "DataReportBase.h"

class Workspace;
class ElectricCalculation;

/**
 * @class DataReport
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form that shows the results of power flow and fault calculations.
 * @file DataReport.h
 */
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
    virtual void GridKeyHandler(wxGrid* grid, wxKeyEvent& event);

   protected:
    virtual void OnFaulrGridChanged(wxGridEvent& event);
    virtual void OnFaultBranchesGridChanged(wxGridEvent& event);
    virtual void OnFaultBusesGridChanged(wxGridEvent& event);
    virtual void OnFaultGeneratorsGridChanged(wxGridEvent& event);
    virtual void OnPFBranchesGridChanged(wxGridEvent& event);
    virtual void OnPowerFlowGridChanged(wxGridEvent& event);
    virtual void OnPFBusGridChanged(wxGridEvent& event);
    virtual void OnGridFaultBranchesKeyDown(wxKeyEvent& event) { GridKeyHandler(m_gridFaultBranches, event); }
    virtual void OnGridFaultBusesKeyDown(wxKeyEvent& event) { GridKeyHandler(m_gridFaultBuses, event); }
    virtual void OnGridFaultGeneratorsKeyDown(wxKeyEvent& event) { GridKeyHandler(m_gridFaultGenerators, event); }
    virtual void OnGridFaultKeyDown(wxKeyEvent& event) { GridKeyHandler(m_gridFault, event); }
    virtual void OnGridPFBranchesKeyDown(wxKeyEvent& event) { GridKeyHandler(m_gridPFBranches, event); }
    virtual void OnGridPFBusesKeyDown(wxKeyEvent& event) { GridKeyHandler(m_gridPFBuses, event); }
    virtual void OnGridPFKeyDown(wxKeyEvent& event) { GridKeyHandler(m_gridPowerFlow, event); }
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
