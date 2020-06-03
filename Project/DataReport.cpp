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

#include "DataReport.h"
#include "ElectricCalculation.h"
#include "Workspace.h"

DataReport::DataReport(wxWindow* parent, Workspace* workspace) : DataReportBase(parent)
{
    m_workspace = workspace;

    m_headerColour = wxColour(150, 150, 150);
    m_offlineColour = wxColour(100, 100, 100);
    m_oddRowColour = wxColour(220, 220, 220);
    m_evenRowColour = wxColour(255, 255, 255);

    CreateGrids();
    SetHeaders();
    FillValues();

    SetRowsColours(m_gridPowerFlow);
    SetRowsColours(m_gridPFBuses);
    SetRowsColours(m_gridPFBranches);
    SetRowsColours(m_gridFault, 2);
    SetRowsColours(m_gridFaultBuses, 2);
    SetRowsColours(m_gridFaultBranches, 2);
    SetRowsColours(m_gridFaultGenerators, 2);

    Layout();
}

DataReport::~DataReport() {}
void DataReport::CreateGrids()
{
    wxFont headerFont = m_gridPowerFlow->GetLabelFont();
    headerFont.SetWeight(wxFONTWEIGHT_BOLD);

    ElectricCalculation eCalc;
    eCalc.GetElementsFromList(m_workspace->GetElementList());
    auto lineList = eCalc.GetLineList();
    auto transformerList = eCalc.GetTransformerList();
    auto busList = eCalc.GetBusList();
    auto generatorList = eCalc.GetSyncGeneratorList();

    // Power Flow
    // Header
    m_gridPowerFlow->AppendCols(7);
    m_gridPowerFlow->AppendRows();
    m_gridPowerFlow->HideColLabels();
    m_gridPowerFlow->HideRowLabels();
    for(int i = 0; i < 7; ++i) {
        m_gridPowerFlow->SetCellBackgroundColour(0, i, m_headerColour);
        m_gridPowerFlow->SetCellFont(0, i, headerFont);
    }
    m_gridPowerFlow->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    // Values
    m_gridPowerFlow->AppendRows((lineList.size() + transformerList.size()) * 2);
    m_gridPowerFlow->AutoSize();

    // Power Flow buses
    // Header
    m_gridPFBuses->AppendCols(6);
    m_gridPFBuses->AppendRows();
    m_gridPFBuses->HideColLabels();
    m_gridPFBuses->HideRowLabels();
    for(int i = 0; i < 6; ++i) {
        m_gridPFBuses->SetCellBackgroundColour(0, i, m_headerColour);
        m_gridPFBuses->SetCellFont(0, i, headerFont);
    }
    m_gridPFBuses->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    // Values
    m_gridPFBuses->AppendRows(busList.size());
    m_gridPFBuses->AutoSize();

    // Power flow branches
    // Header
    m_gridPFBranches->AppendCols(10);
    m_gridPFBranches->AppendRows(1);
    m_gridPFBranches->HideColLabels();
    m_gridPFBranches->HideRowLabels();
    for(int i = 0; i < 10; ++i) {
        m_gridPFBranches->SetCellBackgroundColour(0, i, m_headerColour);
        m_gridPFBranches->SetCellFont(0, i, headerFont);
    }
    m_gridPFBranches->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    // Values
    m_gridPFBranches->AppendRows(lineList.size() + transformerList.size());
    m_gridPFBranches->AutoSize();

    // Fault
    // Header
    m_gridFault->AppendCols(7);
    m_gridFault->AppendRows(2);
    m_gridFault->HideColLabels();
    m_gridFault->HideRowLabels();
    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 7; ++j) {
            m_gridFault->SetCellBackgroundColour(i, j, m_headerColour);
            m_gridFault->SetCellFont(i, j, headerFont);
        }
    }
    m_gridFault->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    m_gridFault->SetCellSize(0, 0, 2, 1);
    m_gridFault->SetCellSize(0, 1, 1, 2);
    m_gridFault->SetCellSize(0, 3, 1, 2);
    m_gridFault->SetCellSize(0, 5, 1, 2);
    // Values
    for(auto it = busList.begin(), itEnd = busList.end(); it != itEnd; ++it) {
        Bus* bus = *it;
        if(bus->GetElectricalData().hasFault) m_gridFault->AppendRows();
    }
    m_gridFault->AutoSize();

    // Fault buses
    // Header
    m_gridFaultBuses->AppendCols(7);
    m_gridFaultBuses->AppendRows(2);
    m_gridFaultBuses->HideColLabels();
    m_gridFaultBuses->HideRowLabels();
    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 7; ++j) {
            m_gridFaultBuses->SetCellBackgroundColour(i, j, m_headerColour);
            m_gridFaultBuses->SetCellFont(i, j, headerFont);
        }
    }
    m_gridFaultBuses->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    m_gridFaultBuses->SetCellSize(0, 0, 2, 1);
    m_gridFaultBuses->SetCellSize(0, 1, 1, 2);
    m_gridFaultBuses->SetCellSize(0, 3, 1, 2);
    m_gridFaultBuses->SetCellSize(0, 5, 1, 2);
    // Values
    m_gridFaultBuses->AppendRows(busList.size());
    m_gridFaultBuses->AutoSize();

    // Fault branches
    // Header
    m_gridFaultBranches->AppendCols(11);
    m_gridFaultBranches->AppendRows(2);
    m_gridFaultBranches->HideColLabels();
    m_gridFaultBranches->HideRowLabels();
    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 11; ++j) {
            m_gridFaultBranches->SetCellBackgroundColour(i, j, m_headerColour);
            m_gridFaultBranches->SetCellFont(i, j, headerFont);
        }
    }
    m_gridFaultBranches->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    m_gridFaultBranches->SetCellSize(0, 0, 2, 1);
    m_gridFaultBranches->SetCellSize(0, 1, 2, 1);
    m_gridFaultBranches->SetCellSize(0, 2, 2, 1);
    m_gridFaultBranches->SetCellSize(0, 3, 2, 1);
    m_gridFaultBranches->SetCellSize(0, 10, 2, 1);
    m_gridFaultBranches->SetCellSize(0, 4, 1, 2);
    m_gridFaultBranches->SetCellSize(0, 6, 1, 2);
    m_gridFaultBranches->SetCellSize(0, 8, 1, 2);
    // Values
    m_gridFaultBranches->AppendRows((lineList.size() + transformerList.size()) * 2);
    m_gridFaultBranches->AutoSize();

    // Fault generators
    // Header
    m_gridFaultGenerators->AppendCols(7);
    m_gridFaultGenerators->AppendRows(2);
    m_gridFaultGenerators->HideColLabels();
    m_gridFaultGenerators->HideRowLabels();
    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 7; ++j) {
            m_gridFaultGenerators->SetCellBackgroundColour(i, j, m_headerColour);
            m_gridFaultGenerators->SetCellFont(i, j, headerFont);
        }
    }
    m_gridFaultGenerators->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    m_gridFaultGenerators->SetCellSize(0, 0, 2, 1);
    m_gridFaultGenerators->SetCellSize(0, 1, 1, 2);
    m_gridFaultGenerators->SetCellSize(0, 3, 1, 2);
    m_gridFaultGenerators->SetCellSize(0, 5, 1, 2);
    // Values
    m_gridFaultGenerators->AppendRows(generatorList.size());
    m_gridFaultGenerators->AutoSize();
}

void DataReport::SetHeaders()
{
    // Headers choices fill
    wxString omega = static_cast<wxString>(L'\u03A9');

    m_voltageChoices.Add(_("Voltage (p.u.)"));
    m_voltageChoices.Add(_("Voltage (V)"));
    m_voltageChoices.Add(_("Voltage (kV)"));

    m_activePowerChoices.Add(_("Active Power (p.u.)"));
    m_activePowerChoices.Add(_("Active Power (W)"));
    m_activePowerChoices.Add(_("Active Power (kW)"));
    m_activePowerChoices.Add(_("Active Power (MW)"));

    m_reactivePowerChoices.Add(_("Reactive Power (p.u.)"));
    m_reactivePowerChoices.Add(_("Reactive Power (VAr)"));
    m_reactivePowerChoices.Add(_("Reactive Power (kVAr)"));
    m_reactivePowerChoices.Add(_("Reactive Power (MVAr)"));

    m_resistanceChoices.Add(_("R (p.u.)"));
    m_resistanceChoices.Add(_("R (") + omega + wxT(")"));

    m_indReactanceChoices.Add(_("XL (p.u.)"));
    m_indReactanceChoices.Add(_("XL (") + omega + wxT(")"));

    m_capSusceptanceChoices.Add(_("B (p.u.)"));
    m_capSusceptanceChoices.Add(_("B (S)"));

    m_currentChoices.Add(_("Current (p.u.)"));
    m_currentChoices.Add(_("Current (A)"));
    m_currentChoices.Add(_("Current (kA)"));

    // Power flow
    m_gridPowerFlow->SetCellValue(0, 0, _("Type"));
    m_gridPowerFlow->SetCellValue(0, 1, _("Name"));
    m_gridPowerFlow->SetCellValue(0, 2, _("From"));
    m_gridPowerFlow->SetCellValue(0, 3, _("To"));
    m_gridPowerFlow->SetCellEditor(0, 4, new wxGridCellChoiceEditor(m_activePowerChoices));
    m_gridPowerFlow->SetCellValue(0, 4, m_activePowerChoices[3]);
    m_gridPowerFlow->SetCellEditor(0, 5, new wxGridCellChoiceEditor(m_reactivePowerChoices));
    m_gridPowerFlow->SetCellValue(0, 5, m_reactivePowerChoices[3]);
    m_gridPowerFlow->SetCellValue(0, 6, _("Online"));

    // Power flow buses
    m_gridPFBuses->SetCellValue(0, 0, _("Name"));
    m_gridPFBuses->SetCellValue(0, 1, _("Type"));
    m_gridPFBuses->SetCellEditor(0, 2, new wxGridCellChoiceEditor(m_voltageChoices));
    m_gridPFBuses->SetCellValue(0, 2, m_voltageChoices[0]);
    m_gridPFBuses->SetCellValue(0, 3, _("Angle"));
    m_gridPFBuses->SetCellEditor(0, 4, new wxGridCellChoiceEditor(m_activePowerChoices));
    m_gridPFBuses->SetCellValue(0, 4, m_activePowerChoices[3]);
    m_gridPFBuses->SetCellEditor(0, 5, new wxGridCellChoiceEditor(m_reactivePowerChoices));
    m_gridPFBuses->SetCellValue(0, 5, m_reactivePowerChoices[3]);

    // Power flow branches
    m_gridPFBranches->SetCellValue(0, 0, _("Type"));
    m_gridPFBranches->SetCellValue(0, 1, _("Name"));
    m_gridPFBranches->SetCellValue(0, 2, _("From"));
    m_gridPFBranches->SetCellValue(0, 3, _("To"));
    m_gridPFBranches->SetCellEditor(0, 4, new wxGridCellChoiceEditor(m_resistanceChoices));
    m_gridPFBranches->SetCellValue(0, 4, m_resistanceChoices[0]);
    m_gridPFBranches->SetCellEditor(0, 5, new wxGridCellChoiceEditor(m_indReactanceChoices));
    m_gridPFBranches->SetCellValue(0, 5, m_indReactanceChoices[0]);
    m_gridPFBranches->SetCellEditor(0, 6, new wxGridCellChoiceEditor(m_capSusceptanceChoices));
    m_gridPFBranches->SetCellValue(0, 6, m_capSusceptanceChoices[0]);
    m_gridPFBranches->SetCellValue(0, 7, _("TAP"));
    m_gridPFBranches->SetCellValue(0, 8, _("Phase Shift"));
    m_gridPFBranches->SetCellValue(0, 9, _("Online"));

    // Fault
    m_gridFault->SetCellValue(0, 0, _("Fault bus name"));
    m_gridFault->SetCellValue(0, 1, _("Phase A"));
    m_gridFault->SetCellValue(0, 3, _("Phase B"));
    m_gridFault->SetCellValue(0, 5, _("Phase C"));
    m_gridFault->SetCellEditor(1, 1, new wxGridCellChoiceEditor(m_currentChoices));
    m_gridFault->SetCellValue(1, 1, m_currentChoices[1]);
    m_gridFault->SetCellValue(1, 2, _("Angle"));
    m_gridFault->SetCellEditor(1, 3, new wxGridCellChoiceEditor(m_currentChoices));
    m_gridFault->SetCellValue(1, 3, m_currentChoices[1]);
    m_gridFault->SetCellValue(1, 4, _("Angle"));
    m_gridFault->SetCellEditor(1, 5, new wxGridCellChoiceEditor(m_currentChoices));
    m_gridFault->SetCellValue(1, 5, m_currentChoices[1]);
    m_gridFault->SetCellValue(1, 6, _("Angle"));

    // Fault buses
    m_gridFaultBuses->SetCellValue(0, 0, _("Name"));
    m_gridFaultBuses->SetCellValue(0, 1, _("Phase A"));
    m_gridFaultBuses->SetCellValue(0, 3, _("Phase B"));
    m_gridFaultBuses->SetCellValue(0, 5, _("Phase C"));
    m_gridFaultBuses->SetCellEditor(1, 1, new wxGridCellChoiceEditor(m_voltageChoices));
    m_gridFaultBuses->SetCellValue(1, 1, m_voltageChoices[0]);
    m_gridFaultBuses->SetCellValue(1, 2, _("Angle"));
    m_gridFaultBuses->SetCellEditor(1, 3, new wxGridCellChoiceEditor(m_voltageChoices));
    m_gridFaultBuses->SetCellValue(1, 3, m_voltageChoices[0]);
    m_gridFaultBuses->SetCellValue(1, 4, _("Angle"));
    m_gridFaultBuses->SetCellEditor(1, 5, new wxGridCellChoiceEditor(m_voltageChoices));
    m_gridFaultBuses->SetCellValue(1, 5, m_voltageChoices[0]);
    m_gridFaultBuses->SetCellValue(1, 6, _("Angle"));

    // Fault branches
    m_gridFaultBranches->SetCellValue(0, 0, _("Type"));
    m_gridFaultBranches->SetCellValue(0, 1, _("Name"));
    m_gridFaultBranches->SetCellValue(0, 2, _("From"));
    m_gridFaultBranches->SetCellValue(0, 3, _("To"));
    m_gridFaultBranches->SetCellValue(0, 4, _("Phase A"));
    m_gridFaultBranches->SetCellValue(0, 6, _("Phase B"));
    m_gridFaultBranches->SetCellValue(0, 8, _("Phase C"));
    m_gridFaultBranches->SetCellValue(0, 10, _("Online"));
    m_gridFaultBranches->SetCellEditor(1, 4, new wxGridCellChoiceEditor(m_currentChoices));
    m_gridFaultBranches->SetCellValue(1, 4, m_currentChoices[1]);
    m_gridFaultBranches->SetCellValue(1, 5, _("Angle"));
    m_gridFaultBranches->SetCellEditor(1, 6, new wxGridCellChoiceEditor(m_currentChoices));
    m_gridFaultBranches->SetCellValue(1, 6, m_currentChoices[1]);
    m_gridFaultBranches->SetCellValue(1, 7, _("Angle"));
    m_gridFaultBranches->SetCellEditor(1, 8, new wxGridCellChoiceEditor(m_currentChoices));
    m_gridFaultBranches->SetCellValue(1, 8, m_currentChoices[1]);
    m_gridFaultBranches->SetCellValue(1, 9, _("Angle"));

    // Fault generators
    m_gridFaultGenerators->SetCellValue(0, 0, _("Name"));
    m_gridFaultGenerators->SetCellValue(0, 1, _("Phase A"));
    m_gridFaultGenerators->SetCellValue(0, 3, _("Phase B"));
    m_gridFaultGenerators->SetCellValue(0, 5, _("Phase C"));
    m_gridFaultGenerators->SetCellEditor(1, 1, new wxGridCellChoiceEditor(m_currentChoices));
    m_gridFaultGenerators->SetCellValue(1, 1, m_currentChoices[1]);
    m_gridFaultGenerators->SetCellValue(1, 2, _("Angle"));
    m_gridFaultGenerators->SetCellEditor(1, 3, new wxGridCellChoiceEditor(m_currentChoices));
    m_gridFaultGenerators->SetCellValue(1, 3, m_currentChoices[1]);
    m_gridFaultGenerators->SetCellValue(1, 4, _("Angle"));
    m_gridFaultGenerators->SetCellEditor(1, 5, new wxGridCellChoiceEditor(m_currentChoices));
    m_gridFaultGenerators->SetCellValue(1, 5, m_currentChoices[1]);
    m_gridFaultGenerators->SetCellValue(1, 6, _("Angle"));
}

void DataReport::FillValues(GridSelection gridToFill)
{
    m_changingValues = true;
    ElectricCalculation eCalc;
    eCalc.GetElementsFromList(m_workspace->GetElementList());

    double basePower = m_workspace->GetProperties()->GetSimulationPropertiesData().basePower;
    switch(m_workspace->GetProperties()->GetSimulationPropertiesData().basePowerUnit) {
        case ElectricalUnit::UNIT_kVA: {
            basePower *= 1e3;
        } break;
        case ElectricalUnit::UNIT_MVA: {
            basePower *= 1e6;
        } break;
        default:
            break;
    }

    int rowNumber = 1;
    auto lineList = eCalc.GetLineList();
    auto transformerList = eCalc.GetTransformerList();
    auto busList = eCalc.GetBusList();
    auto generatorList = eCalc.GetSyncGeneratorList();

    // Power Flow
    if(gridToFill == GRID_ALL || gridToFill == GRID_PF) {
        double kActivePower = 1.0;
        if(m_gridPowerFlow->GetCellValue(0, 4) == m_activePowerChoices[1])
            kActivePower = basePower;
        else if(m_gridPowerFlow->GetCellValue(0, 4) == m_activePowerChoices[2])
            kActivePower = basePower / 1e3;
        else if(m_gridPowerFlow->GetCellValue(0, 4) == m_activePowerChoices[3])
            kActivePower = basePower / 1e6;

        double kReactivePower = 1.0;
        if(m_gridPowerFlow->GetCellValue(0, 5) == m_reactivePowerChoices[1])
            kReactivePower = basePower;
        else if(m_gridPowerFlow->GetCellValue(0, 5) == m_reactivePowerChoices[2])
            kReactivePower = basePower / 1e3;
        else if(m_gridPowerFlow->GetCellValue(0, 5) == m_reactivePowerChoices[3])
            kReactivePower = basePower / 1e6;

        for(auto it = lineList.begin(), itEnd = lineList.end(); it != itEnd; ++it) {
            Line* line = *it;

            wxString busName1 = "-";
            if(line->GetParentList()[0])
                busName1 = static_cast<Bus*>(line->GetParentList()[0])->GetElectricalData().name;
            wxString busName2 = "-";
            if(line->GetParentList()[1])
                busName2 = static_cast<Bus*>(line->GetParentList()[1])->GetElectricalData().name;

            wxString isOnline = _("Yes");
            wxColour textColour = m_gridPowerFlow->GetDefaultCellTextColour();
            if(!line->IsOnline()) {
                isOnline = _("No");
                textColour = m_offlineColour;
            }
            for(int i = 0; i < 2; ++i) {
                for(int j = 0; j < 7; ++j) { m_gridPowerFlow->SetCellTextColour(rowNumber + i, j, textColour); }
            }

            m_gridPowerFlow->SetCellValue(rowNumber, 9, isOnline);
            auto data = line->GetPUElectricalData(basePower);

            m_gridPowerFlow->SetCellValue(rowNumber, 0, _("Line"));
            m_gridPowerFlow->SetCellValue(rowNumber, 1, data.name);
            m_gridPowerFlow->SetCellValue(rowNumber, 2, busName1);
            m_gridPowerFlow->SetCellValue(rowNumber, 3, busName2);
            m_gridPowerFlow->SetCellValue(rowNumber, 4,
                                          line->StringFromDouble(std::real(data.powerFlow[0]) * kActivePower));
            m_gridPowerFlow->SetCellValue(rowNumber, 5,
                                          line->StringFromDouble(std::imag(data.powerFlow[0]) * kReactivePower));
            m_gridPowerFlow->SetCellValue(rowNumber, 6, isOnline);
            rowNumber++;

            m_gridPowerFlow->SetCellValue(rowNumber, 0, _("Line"));
            m_gridPowerFlow->SetCellValue(rowNumber, 1, data.name);
            m_gridPowerFlow->SetCellValue(rowNumber, 2, busName2);
            m_gridPowerFlow->SetCellValue(rowNumber, 3, busName1);
            m_gridPowerFlow->SetCellValue(rowNumber, 4,
                                          line->StringFromDouble(std::real(data.powerFlow[1]) * kActivePower));
            m_gridPowerFlow->SetCellValue(rowNumber, 5,
                                          line->StringFromDouble(std::imag(data.powerFlow[1]) * kReactivePower));
            m_gridPowerFlow->SetCellValue(rowNumber, 6, isOnline);
            rowNumber++;
        }

        for(auto it = transformerList.begin(), itEnd = transformerList.end(); it != itEnd; ++it) {
            Transformer* transformer = *it;
            auto data = transformer->GetPUElectricalData(basePower);

            wxString busName1 = "-";
            if(transformer->GetParentList()[0])
                busName1 = static_cast<Bus*>(transformer->GetParentList()[0])->GetElectricalData().name;
            wxString busName2 = "-";
            if(transformer->GetParentList()[1])
                busName2 = static_cast<Bus*>(transformer->GetParentList()[1])->GetElectricalData().name;

            wxString isOnline = _("Yes");
            wxColour textColour = m_gridPowerFlow->GetDefaultCellTextColour();
            if(!transformer->IsOnline()) {
                isOnline = _("No");
                textColour = m_offlineColour;
            }
            for(int i = 0; i < 2; ++i) {
                for(int j = 0; j < 7; ++j) { m_gridPowerFlow->SetCellTextColour(rowNumber + i, j, textColour); }
            }

            m_gridPowerFlow->SetCellValue(rowNumber, 0, _("Transformer"));
            m_gridPowerFlow->SetCellValue(rowNumber, 1, data.name);
            m_gridPowerFlow->SetCellValue(rowNumber, 2, busName1);
            m_gridPowerFlow->SetCellValue(rowNumber, 3, busName2);
            m_gridPowerFlow->SetCellValue(rowNumber, 4,
                                          transformer->StringFromDouble(std::real(data.powerFlow[0]) * kActivePower));
            m_gridPowerFlow->SetCellValue(rowNumber, 5,
                                          transformer->StringFromDouble(std::imag(data.powerFlow[0]) * kReactivePower));
            m_gridPowerFlow->SetCellValue(rowNumber, 6, isOnline);
            rowNumber++;

            m_gridPowerFlow->SetCellValue(rowNumber, 0, _("Transformer"));
            m_gridPowerFlow->SetCellValue(rowNumber, 1, data.name);
            m_gridPowerFlow->SetCellValue(rowNumber, 2, busName2);
            m_gridPowerFlow->SetCellValue(rowNumber, 3, busName1);
            m_gridPowerFlow->SetCellValue(rowNumber, 4,
                                          transformer->StringFromDouble(std::real(data.powerFlow[1]) * kActivePower));
            m_gridPowerFlow->SetCellValue(rowNumber, 5,
                                          transformer->StringFromDouble(std::imag(data.powerFlow[1]) * kReactivePower));
            m_gridPowerFlow->SetCellValue(rowNumber, 6, isOnline);
            rowNumber++;
        }
        m_gridPowerFlow->AutoSize();
    }

    // Power Flow buses
    if(gridToFill == GRID_ALL || gridToFill == GRID_PFBUSES) {
        double kActivePower = 1.0;
        if(m_gridPFBuses->GetCellValue(0, 4) == m_activePowerChoices[1])
            kActivePower = basePower;
        else if(m_gridPFBuses->GetCellValue(0, 4) == m_activePowerChoices[2])
            kActivePower = basePower / 1e3;
        else if(m_gridPFBuses->GetCellValue(0, 4) == m_activePowerChoices[3])
            kActivePower = basePower / 1e6;

        double kReactivePower = 1.0;
        if(m_gridPFBuses->GetCellValue(0, 5) == m_reactivePowerChoices[1])
            kReactivePower = basePower;
        else if(m_gridPFBuses->GetCellValue(0, 5) == m_reactivePowerChoices[2])
            kReactivePower = basePower / 1e3;
        else if(m_gridPFBuses->GetCellValue(0, 5) == m_reactivePowerChoices[3])
            kReactivePower = basePower / 1e6;

        rowNumber = 1;
        for(auto it = busList.begin(), itEnd = busList.end(); it != itEnd; ++it) {
            Bus* bus = *it;
            auto data = bus->GetElectricalData();

            double vb = std::abs(data.nominalVoltage);
            if(data.nominalVoltageUnit == ElectricalUnit::UNIT_kV) vb *= 1e3;
            double kVoltage = 1.0;
            if(m_gridPFBuses->GetCellValue(0, 2) == m_voltageChoices[1])
                kVoltage = vb;
            else if(m_gridPFBuses->GetCellValue(0, 2) == m_voltageChoices[2])
                kVoltage = vb / 1e3;

            m_gridPFBuses->SetCellValue(rowNumber, 0, data.name);
            wxString busTypeString = "";
            switch(data.busType) {
                case BUS_SLACK: {
                    busTypeString = _("Slack");
                } break;
                case BUS_PV: {
                    busTypeString = _("PV");
                } break;
                case BUS_PQ: {
                    busTypeString = _("PQ");
                } break;
            }
            m_gridPFBuses->SetCellValue(rowNumber, 1, busTypeString);
            m_gridPFBuses->SetCellValue(rowNumber, 2, bus->StringFromDouble(std::abs(data.voltage) * kVoltage));
            m_gridPFBuses->SetCellValue(rowNumber, 3, bus->StringFromDouble(wxRadToDeg(std::arg(data.voltage))));
            m_gridPFBuses->SetCellValue(rowNumber, 4, bus->StringFromDouble(std::real(data.power) * kActivePower));
            m_gridPFBuses->SetCellValue(rowNumber, 5, bus->StringFromDouble(std::imag(data.power) * kReactivePower));
            rowNumber++;
        }
        m_gridPFBuses->AutoSize();
    }

    // Power flow branches
    if(gridToFill == GRID_ALL || gridToFill == GRID_PFBRANCHES) {
        rowNumber = 1;
        for(auto it = lineList.begin(), itEnd = lineList.end(); it != itEnd; ++it) {
            Line* line = *it;
            auto data = line->GetPUElectricalData(basePower);

            double vb = data.nominalVoltage;
            if(data.nominalVoltageUnit == ElectricalUnit::UNIT_kV) vb *= 1e3;
            double zb = (vb * vb) / basePower;

            wxString busName1 = "-";
            if(line->GetParentList()[0])
                busName1 = static_cast<Bus*>(line->GetParentList()[0])->GetElectricalData().name;
            wxString busName2 = "-";
            if(line->GetParentList()[1])
                busName2 = static_cast<Bus*>(line->GetParentList()[1])->GetElectricalData().name;
            wxString isOnline = _("Yes");
            wxColour textColour = m_gridPFBranches->GetDefaultCellTextColour();
            if(!line->IsOnline()) {
                isOnline = _("No");
                textColour = m_offlineColour;
            }
            for(int j = 0; j < 10; ++j) { m_gridPFBranches->SetCellTextColour(rowNumber, j, textColour); }

            m_gridPFBranches->SetCellValue(rowNumber, 0, _("Line"));
            m_gridPFBranches->SetCellValue(rowNumber, 1, data.name);

            m_gridPFBranches->SetCellValue(rowNumber, 2, busName1);
            m_gridPFBranches->SetCellValue(rowNumber, 3, busName2);

            double k = 1.0;
            if(m_gridPFBranches->GetCellValue(0, 4) == m_resistanceChoices[1]) k = zb;
            m_gridPFBranches->SetCellValue(rowNumber, 4, line->StringFromDouble(data.resistance * k));
            k = 1.0;
            if(m_gridPFBranches->GetCellValue(0, 5) == m_indReactanceChoices[1]) k = zb;
            m_gridPFBranches->SetCellValue(rowNumber, 5, line->StringFromDouble(data.indReactance * k));
            k = 1.0;
            if(m_gridPFBranches->GetCellValue(0, 6) == m_capSusceptanceChoices[1]) k = zb;
            m_gridPFBranches->SetCellValue(rowNumber, 6, line->StringFromDouble(data.capSusceptance / k));
            m_gridPFBranches->SetCellValue(rowNumber, 7, "-");
            m_gridPFBranches->SetCellValue(rowNumber, 8, "-");
            m_gridPFBranches->SetCellValue(rowNumber, 9, isOnline);
            rowNumber++;
        }
        for(auto it = transformerList.begin(), itEnd = transformerList.end(); it != itEnd; ++it) {
            Transformer* transformer = *it;
            auto data = transformer->GetPUElectricalData(basePower);

            double vb = 0.0;
            if(data.baseVoltage == 0) {
                vb = data.primaryNominalVoltage;
                if(data.primaryNominalVoltageUnit == ElectricalUnit::UNIT_kV) vb *= 1e3;
            } else {
                vb = data.secondaryNominalVoltage;
                if(data.secondaryNominalVoltageUnit == ElectricalUnit::UNIT_kV) vb *= 1e3;
            }
            double zb = (vb * vb) / basePower;

            wxString busName1 = "-";
            if(transformer->GetParentList()[0])
                busName1 = static_cast<Bus*>(transformer->GetParentList()[0])->GetElectricalData().name;
            wxString busName2 = "-";
            if(transformer->GetParentList()[1])
                busName2 = static_cast<Bus*>(transformer->GetParentList()[1])->GetElectricalData().name;

            wxString isOnline = _("Yes");
            wxColour textColour = m_gridPFBranches->GetDefaultCellTextColour();
            if(!transformer->IsOnline()) {
                isOnline = _("No");
                textColour = m_offlineColour;
            }
            for(int j = 0; j < 10; ++j) { m_gridPFBranches->SetCellTextColour(rowNumber, j, textColour); }

            m_gridPFBranches->SetCellValue(rowNumber, 0, _("Transformer"));
            m_gridPFBranches->SetCellValue(rowNumber, 1, data.name);
            m_gridPFBranches->SetCellValue(rowNumber, 2, busName1);
            m_gridPFBranches->SetCellValue(rowNumber, 3, busName2);

            double k = 1.0;
            if(m_gridPFBranches->GetCellValue(0, 4) == m_resistanceChoices[1]) k = zb;
            m_gridPFBranches->SetCellValue(rowNumber, 4, transformer->StringFromDouble(data.resistance * k));
            k = 1.0;
            if(m_gridPFBranches->GetCellValue(0, 5) == m_indReactanceChoices[1]) k = zb;
            m_gridPFBranches->SetCellValue(rowNumber, 5, transformer->StringFromDouble(data.indReactance * k));
            m_gridPFBranches->SetCellValue(rowNumber, 6, "-");
            m_gridPFBranches->SetCellValue(rowNumber, 7, transformer->StringFromDouble(data.turnsRatio));
            m_gridPFBranches->SetCellValue(rowNumber, 8, transformer->StringFromDouble(data.phaseShift));
            m_gridPFBranches->SetCellValue(rowNumber, 9, isOnline);
            rowNumber++;
        }
        m_gridPFBranches->AutoSize();
    }

    // Fault
    if(gridToFill == GRID_ALL || gridToFill == GRID_FAULT) {
        rowNumber = 2;
        for(auto it = busList.begin(), itEnd = busList.end(); it != itEnd; ++it) {
            Bus* bus = *it;
            auto data = bus->GetElectricalData();
            if(data.hasFault) {
                double vb = bus->GetValueFromUnit(data.nominalVoltage, data.nominalVoltageUnit);
                double ib = basePower / (std::sqrt(3.0) * vb);

                m_gridFault->SetCellValue(rowNumber, 0, data.name);

                double kCurrent = 1.0;
                if(m_gridFault->GetCellValue(1, 1) == m_currentChoices[1]) {
                    kCurrent = ib;
                } else if(m_gridFault->GetCellValue(1, 1) == m_currentChoices[2]) {
                    kCurrent = ib / 1e3;
                }
                m_gridFault->SetCellValue(rowNumber, 1,
                                          bus->StringFromDouble(std::abs(data.faultCurrent[0]) * kCurrent));

                m_gridFault->SetCellValue(rowNumber, 2,
                                          bus->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[0]))));

                kCurrent = 1.0;
                if(m_gridFault->GetCellValue(1, 3) == m_currentChoices[1]) {
                    kCurrent = ib;
                } else if(m_gridFault->GetCellValue(1, 3) == m_currentChoices[2]) {
                    kCurrent = ib / 1e3;
                }
                m_gridFault->SetCellValue(rowNumber, 3,
                                          bus->StringFromDouble(std::abs(data.faultCurrent[1]) * kCurrent));

                m_gridFault->SetCellValue(rowNumber, 4,
                                          bus->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[1]))));

                kCurrent = 1.0;
                if(m_gridFault->GetCellValue(1, 5) == m_currentChoices[1]) {
                    kCurrent = ib;
                } else if(m_gridFault->GetCellValue(1, 5) == m_currentChoices[2]) {
                    kCurrent = ib / 1e3;
                }
                m_gridFault->SetCellValue(rowNumber, 5,
                                          bus->StringFromDouble(std::abs(data.faultCurrent[2]) * kCurrent));

                m_gridFault->SetCellValue(rowNumber, 6,
                                          bus->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[2]))));

                rowNumber++;
            }
        }
        m_gridFault->AutoSize();
    }

    // Fault buses
    if(gridToFill == GRID_ALL || gridToFill == GRID_FAULTBUSES) {
        rowNumber = 2;
        for(auto it = busList.begin(), itEnd = busList.end(); it != itEnd; ++it) {
            Bus* bus = *it;
            auto data = bus->GetElectricalData();
            double vb = bus->GetValueFromUnit(data.nominalVoltage, data.nominalVoltageUnit);

            m_gridFaultBuses->SetCellValue(rowNumber, 0, data.name);
            double kVoltage = 1.0;
            if(m_gridFaultBuses->GetCellValue(1, 1) == m_voltageChoices[1]) {
                kVoltage = vb;
            } else if(m_gridFaultBuses->GetCellValue(1, 1) == m_voltageChoices[2]) {
                kVoltage = vb / 1e3;
            }
            m_gridFaultBuses->SetCellValue(rowNumber, 1,
                                           bus->StringFromDouble(std::abs(data.faultVoltage[0]) * kVoltage));
            m_gridFaultBuses->SetCellValue(rowNumber, 2,
                                           bus->StringFromDouble(wxRadToDeg(std::arg(data.faultVoltage[0]))));

            kVoltage = 1.0;
            if(m_gridFaultBuses->GetCellValue(1, 3) == m_voltageChoices[1]) {
                kVoltage = vb;
            } else if(m_gridFaultBuses->GetCellValue(1, 3) == m_voltageChoices[2]) {
                kVoltage = vb / 1e3;
            }
            m_gridFaultBuses->SetCellValue(rowNumber, 3,
                                           bus->StringFromDouble(std::abs(data.faultVoltage[1]) * kVoltage));
            m_gridFaultBuses->SetCellValue(rowNumber, 4,
                                           bus->StringFromDouble(wxRadToDeg(std::arg(data.faultVoltage[1]))));

            kVoltage = 1.0;
            if(m_gridFaultBuses->GetCellValue(1, 5) == m_voltageChoices[1]) {
                kVoltage = vb;
            } else if(m_gridFaultBuses->GetCellValue(1, 5) == m_voltageChoices[2]) {
                kVoltage = vb / 1e3;
            }
            m_gridFaultBuses->SetCellValue(rowNumber, 5,
                                           bus->StringFromDouble(std::abs(data.faultVoltage[2]) * kVoltage));
            m_gridFaultBuses->SetCellValue(rowNumber, 6,
                                           bus->StringFromDouble(wxRadToDeg(std::arg(data.faultVoltage[2]))));

            rowNumber++;
        }
        m_gridFaultBuses->AutoSize();
    }

    // Fault branches
    if(gridToFill == GRID_ALL || gridToFill == GRID_FAULTBRANCHES) {
        rowNumber = 2;
        for(auto it = lineList.begin(), itEnd = lineList.end(); it != itEnd; ++it) {
            Line* line = *it;
            auto data = line->GetPUElectricalData(basePower);

            double vb = line->GetValueFromUnit(data.nominalVoltage, data.nominalVoltageUnit);
            double ib = basePower / (std::sqrt(3.0) * vb);

            wxString busName1 = "-";
            if(line->GetParentList()[0])
                busName1 = static_cast<Bus*>(line->GetParentList()[0])->GetElectricalData().name;
            wxString busName2 = "-";
            if(line->GetParentList()[1])
                busName2 = static_cast<Bus*>(line->GetParentList()[1])->GetElectricalData().name;

            wxString isOnline = _("Yes");
            wxColour textColour = m_gridFaultBranches->GetDefaultCellTextColour();
            if(!line->IsOnline()) {
                isOnline = _("No");
                textColour = m_offlineColour;
            }
            for(int i = 0; i < 2; ++i) {
                for(int j = 0; j < 11; ++j) { m_gridFaultBranches->SetCellTextColour(rowNumber + i, j, textColour); }
            }

            m_gridFaultBranches->SetCellValue(rowNumber, 0, _("Line"));
            m_gridFaultBranches->SetCellValue(rowNumber, 1, data.name);
            m_gridFaultBranches->SetCellValue(rowNumber, 2, busName1);
            m_gridFaultBranches->SetCellValue(rowNumber, 3, busName2);

            double kCurrent = 1.0;
            if(m_gridFaultBranches->GetCellValue(1, 4) == m_currentChoices[1]) {
                kCurrent = ib;
            } else if(m_gridFaultBranches->GetCellValue(1, 4) == m_currentChoices[2]) {
                kCurrent = ib / 1e3;
            }
            m_gridFaultBranches->SetCellValue(rowNumber, 4,
                                              line->StringFromDouble(std::abs(data.faultCurrent[0][0]) * kCurrent));
            m_gridFaultBranches->SetCellValue(rowNumber, 5,
                                              line->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[0][0]))));
            kCurrent = 1.0;
            if(m_gridFaultBranches->GetCellValue(1, 6) == m_currentChoices[1]) {
                kCurrent = ib;
            } else if(m_gridFaultBranches->GetCellValue(1, 6) == m_currentChoices[2]) {
                kCurrent = ib / 1e3;
            }
            m_gridFaultBranches->SetCellValue(rowNumber, 6,
                                              line->StringFromDouble(std::abs(data.faultCurrent[0][1]) * kCurrent));
            m_gridFaultBranches->SetCellValue(rowNumber, 7,
                                              line->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[0][1]))));
            kCurrent = 1.0;
            if(m_gridFaultBranches->GetCellValue(1, 8) == m_currentChoices[1]) {
                kCurrent = ib;
            } else if(m_gridFaultBranches->GetCellValue(1, 8) == m_currentChoices[2]) {
                kCurrent = ib / 1e3;
            }
            m_gridFaultBranches->SetCellValue(rowNumber, 8,
                                              line->StringFromDouble(std::abs(data.faultCurrent[0][2]) * kCurrent));
            m_gridFaultBranches->SetCellValue(rowNumber, 9,
                                              line->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[0][2]))));
            m_gridFaultBranches->SetCellValue(rowNumber, 10, isOnline);
            rowNumber++;

            m_gridFaultBranches->SetCellValue(rowNumber, 0, _("Line"));
            m_gridFaultBranches->SetCellValue(rowNumber, 1, data.name);
            m_gridFaultBranches->SetCellValue(rowNumber, 2, busName2);
            m_gridFaultBranches->SetCellValue(rowNumber, 3, busName1);
            kCurrent = 1.0;
            if(m_gridFaultBranches->GetCellValue(1, 4) == m_currentChoices[1]) {
                kCurrent = ib;
            } else if(m_gridFaultBranches->GetCellValue(1, 4) == m_currentChoices[2]) {
                kCurrent = ib / 1e3;
            }
            m_gridFaultBranches->SetCellValue(rowNumber, 4,
                                              line->StringFromDouble(std::abs(data.faultCurrent[1][0]) * kCurrent));
            m_gridFaultBranches->SetCellValue(rowNumber, 5,
                                              line->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[1][0]))));
            kCurrent = 1.0;
            if(m_gridFaultBranches->GetCellValue(1, 6) == m_currentChoices[1]) {
                kCurrent = ib;
            } else if(m_gridFaultBranches->GetCellValue(1, 6) == m_currentChoices[2]) {
                kCurrent = ib / 1e3;
            }
            m_gridFaultBranches->SetCellValue(rowNumber, 6,
                                              line->StringFromDouble(std::abs(data.faultCurrent[1][1]) * kCurrent));
            m_gridFaultBranches->SetCellValue(rowNumber, 7,
                                              line->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[1][1]))));
            kCurrent = 1.0;
            if(m_gridFaultBranches->GetCellValue(1, 8) == m_currentChoices[1]) {
                kCurrent = ib;
            } else if(m_gridFaultBranches->GetCellValue(1, 8) == m_currentChoices[2]) {
                kCurrent = ib / 1e3;
            }
            m_gridFaultBranches->SetCellValue(rowNumber, 8,
                                              line->StringFromDouble(std::abs(data.faultCurrent[1][2]) * kCurrent));
            m_gridFaultBranches->SetCellValue(rowNumber, 9,
                                              line->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[1][2]))));
            m_gridFaultBranches->SetCellValue(rowNumber, 10, isOnline);
            rowNumber++;
        }

        for(auto it = transformerList.begin(), itEnd = transformerList.end(); it != itEnd; ++it) {
            Transformer* transformer = *it;
            auto data = transformer->GetPUElectricalData(basePower);

            double vb = transformer->GetValueFromUnit(data.primaryNominalVoltage, data.primaryNominalVoltageUnit);
            double ibp = basePower / (std::sqrt(3.0) * vb);
            vb = transformer->GetValueFromUnit(data.secondaryNominalVoltage, data.secondaryNominalVoltageUnit);
            double ibs = basePower / (std::sqrt(3.0) * vb);

            wxString busName1 = "-";
            if(transformer->GetParentList()[0])
                busName1 = static_cast<Bus*>(transformer->GetParentList()[0])->GetElectricalData().name;
            wxString busName2 = "-";
            if(transformer->GetParentList()[1])
                busName2 = static_cast<Bus*>(transformer->GetParentList()[1])->GetElectricalData().name;

            wxString isOnline = _("Yes");
            wxColour textColour = m_gridFaultBranches->GetDefaultCellTextColour();
            if(!transformer->IsOnline()) {
                isOnline = _("No");
                textColour = m_offlineColour;
            }
            for(int i = 0; i < 2; ++i) {
                for(int j = 0; j < 11; ++j) { m_gridFaultBranches->SetCellTextColour(rowNumber + i, j, textColour); }
            }

            m_gridFaultBranches->SetCellValue(rowNumber, 0, _("Transformer"));
            m_gridFaultBranches->SetCellValue(rowNumber, 1, data.name);
            m_gridFaultBranches->SetCellValue(rowNumber, 2, busName1);
            m_gridFaultBranches->SetCellValue(rowNumber, 3, busName2);

            double kCurrent = 1.0;
            if(m_gridFaultBranches->GetCellValue(1, 4) == m_currentChoices[1]) {
                kCurrent = ibp;
            } else if(m_gridFaultBranches->GetCellValue(1, 4) == m_currentChoices[2]) {
                kCurrent = ibp / 1e3;
            }
            m_gridFaultBranches->SetCellValue(
                rowNumber, 4, transformer->StringFromDouble(std::abs(data.faultCurrent[0][0]) * kCurrent));
            m_gridFaultBranches->SetCellValue(
                rowNumber, 5, transformer->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[0][0]))));
            kCurrent = 1.0;
            if(m_gridFaultBranches->GetCellValue(1, 6) == m_currentChoices[1]) {
                kCurrent = ibp;
            } else if(m_gridFaultBranches->GetCellValue(1, 6) == m_currentChoices[2]) {
                kCurrent = ibp / 1e3;
            }
            m_gridFaultBranches->SetCellValue(
                rowNumber, 6, transformer->StringFromDouble(std::abs(data.faultCurrent[0][1]) * kCurrent));
            m_gridFaultBranches->SetCellValue(
                rowNumber, 7, transformer->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[0][1]))));
            kCurrent = 1.0;
            if(m_gridFaultBranches->GetCellValue(1, 8) == m_currentChoices[1]) {
                kCurrent = ibp;
            } else if(m_gridFaultBranches->GetCellValue(1, 8) == m_currentChoices[2]) {
                kCurrent = ibp / 1e3;
            }
            m_gridFaultBranches->SetCellValue(
                rowNumber, 8, transformer->StringFromDouble(std::abs(data.faultCurrent[0][2]) * kCurrent));
            m_gridFaultBranches->SetCellValue(
                rowNumber, 9, transformer->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[0][2]))));
            m_gridFaultBranches->SetCellValue(rowNumber, 10, isOnline);
            rowNumber++;

            m_gridFaultBranches->SetCellValue(rowNumber, 0, _("Transformer"));
            m_gridFaultBranches->SetCellValue(rowNumber, 1, data.name);
            m_gridFaultBranches->SetCellValue(rowNumber, 2, busName2);
            m_gridFaultBranches->SetCellValue(rowNumber, 3, busName1);
            kCurrent = 1.0;
            if(m_gridFaultBranches->GetCellValue(1, 4) == m_currentChoices[1]) {
                kCurrent = ibs;
            } else if(m_gridFaultBranches->GetCellValue(1, 4) == m_currentChoices[2]) {
                kCurrent = ibs / 1e3;
            }
            m_gridFaultBranches->SetCellValue(
                rowNumber, 4, transformer->StringFromDouble(std::abs(data.faultCurrent[1][0]) * kCurrent));
            m_gridFaultBranches->SetCellValue(
                rowNumber, 5, transformer->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[1][0]))));
            kCurrent = 1.0;
            if(m_gridFaultBranches->GetCellValue(1, 6) == m_currentChoices[1]) {
                kCurrent = ibs;
            } else if(m_gridFaultBranches->GetCellValue(1, 6) == m_currentChoices[2]) {
                kCurrent = ibs / 1e3;
            }
            m_gridFaultBranches->SetCellValue(
                rowNumber, 6, transformer->StringFromDouble(std::abs(data.faultCurrent[1][1]) * kCurrent));
            m_gridFaultBranches->SetCellValue(
                rowNumber, 7, transformer->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[1][1]))));
            kCurrent = 1.0;
            if(m_gridFaultBranches->GetCellValue(1, 8) == m_currentChoices[1]) {
                kCurrent = ibs;
            } else if(m_gridFaultBranches->GetCellValue(1, 8) == m_currentChoices[2]) {
                kCurrent = ibs / 1e3;
            }
            m_gridFaultBranches->SetCellValue(
                rowNumber, 8, transformer->StringFromDouble(std::abs(data.faultCurrent[1][2]) * kCurrent));
            m_gridFaultBranches->SetCellValue(
                rowNumber, 9, transformer->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[1][2]))));
            m_gridFaultBranches->SetCellValue(rowNumber, 10, isOnline);
            rowNumber++;
        }

        m_gridFaultBranches->AutoSize();
    }

    // Fault generators
    if(gridToFill == GRID_ALL || gridToFill == GRID_FAULTGENERATORS) {
        rowNumber = 2;
        for(auto it = generatorList.begin(), itEnd = generatorList.end(); it != itEnd; ++it) {
            SyncGenerator* generator = *it;
            auto data = generator->GetPUElectricalData(basePower);
            double vb = generator->GetValueFromUnit(data.nominalVoltage, data.nominalVoltageUnit);
            double ib = basePower / (std::sqrt(3.0) * vb);

            m_gridFaultGenerators->SetCellValue(rowNumber, 0, data.name);

            double kCurrent = 1.0;
            if(m_gridFaultGenerators->GetCellValue(1, 1) == m_currentChoices[1])
                kCurrent = ib;
            else if(m_gridFaultGenerators->GetCellValue(1, 1) == m_currentChoices[2])
                kCurrent = ib / 1e3;
            m_gridFaultGenerators->SetCellValue(rowNumber, 1,
                                                generator->StringFromDouble(std::abs(data.faultCurrent[0]) * kCurrent));
            m_gridFaultGenerators->SetCellValue(
                rowNumber, 2, generator->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[0]))));

            kCurrent = 1.0;
            if(m_gridFaultGenerators->GetCellValue(1, 3) == m_currentChoices[1])
                kCurrent = ib;
            else if(m_gridFaultGenerators->GetCellValue(1, 3) == m_currentChoices[2])
                kCurrent = ib / 1e3;
            m_gridFaultGenerators->SetCellValue(rowNumber, 3,
                                                generator->StringFromDouble(std::abs(data.faultCurrent[1]) * kCurrent));
            m_gridFaultGenerators->SetCellValue(
                rowNumber, 4, generator->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[1]))));

            kCurrent = 1.0;
            if(m_gridFaultGenerators->GetCellValue(1, 5) == m_currentChoices[1])
                kCurrent = ib;
            else if(m_gridFaultGenerators->GetCellValue(1, 5) == m_currentChoices[2])
                kCurrent = ib / 1e3;
            m_gridFaultGenerators->SetCellValue(rowNumber, 5,
                                                generator->StringFromDouble(std::abs(data.faultCurrent[2]) * kCurrent));
            m_gridFaultGenerators->SetCellValue(
                rowNumber, 6, generator->StringFromDouble(wxRadToDeg(std::arg(data.faultCurrent[2]))));

            rowNumber++;
        }
        m_gridFaultGenerators->AutoSize();
    }
    m_changingValues = false;
}

void DataReport::SetRowsColours(wxGrid* grid, int rowStart)
{
    for(int i = rowStart; i < grid->GetNumberRows(); ++i) {
        wxGridCellAttr* attr = grid->GetOrCreateCellAttr(i, 0);
        if((i - rowStart) % 2 == 0)
            attr->SetBackgroundColour(m_evenRowColour);
        else
            attr->SetBackgroundColour(m_oddRowColour);
        grid->SetRowAttr(i, attr);
    }
}

void DataReport::OnPFBusGridChanged(wxGridEvent& event)
{
    if(!m_changingValues) FillValues(GRID_PFBUSES);
}
void DataReport::OnFaulrGridChanged(wxGridEvent& event)
{
    if(!m_changingValues) FillValues(GRID_FAULT);
}
void DataReport::OnFaultBranchesGridChanged(wxGridEvent& event)
{
    if(!m_changingValues) FillValues(GRID_FAULTBRANCHES);
}
void DataReport::OnFaultBusesGridChanged(wxGridEvent& event)
{
    if(!m_changingValues) FillValues(GRID_FAULTBUSES);
}
void DataReport::OnFaultGeneratorsGridChanged(wxGridEvent& event)
{
    if(!m_changingValues) FillValues(GRID_FAULTGENERATORS);
}
void DataReport::OnPFBranchesGridChanged(wxGridEvent& event)
{
    if(!m_changingValues) FillValues(GRID_PFBRANCHES);
}
void DataReport::OnPowerFlowGridChanged(wxGridEvent& event)
{
    if(!m_changingValues) FillValues(GRID_PF);
}

void DataReport::GridKeyHandler(wxGrid* grid, wxKeyEvent& event)
{
    if(event.GetKeyCode() == 'C' && event.GetModifiers() == wxMOD_CONTROL) {  // Copy selection
        // [Ref.] https://forums.wxwidgets.org/viewtopic.php?t=2200
        wxString copyData = "";
        bool lineNotEmpty;

        for(int i = 0; i < grid->GetNumberRows(); i++) {
            lineNotEmpty = false;
            for(int j = 0; j < grid->GetNumberCols(); j++) {
                if(grid->IsInSelection(i, j)) {
                    if(lineNotEmpty == false) {
                        if(copyData.IsEmpty() == false) {
                            copyData.Append(wxT("\r\n"));  // In Windows if copy to notepad need \r\n to new line.
                        }
                        lineNotEmpty = true;
                    } else {
                        copyData.Append(wxT("\t"));
                    }
                    copyData = copyData + grid->GetCellValue(i, j);
                }
            }
        }
        
        if (wxTheClipboard->Open())
        {
            wxTheClipboard->SetData(new wxTextDataObject(copyData));
            wxTheClipboard->Close();
        }
    } else if(event.GetKeyCode() == 'A' && event.GetModifiers() == wxMOD_CONTROL) {  // Select all
        grid->SelectAll();
    }
    event.Skip();
}
