#include "DataReport.h"
#include "Workspace.h"
#include "ElectricCalculation.h"

DataReport::DataReport(wxWindow* parent, Workspace* workspace) : DataReportBase(parent)
{
    m_workspace = workspace;
    CreateGrids();
    SetHeaders();
    FillValues();

    Layout();
}

DataReport::~DataReport() {}
void DataReport::SetHeaders()
{
    // Headers choices fill
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

    // Power flow
    m_gridPowerFlow->SetCellValue(0, 0, _("Type"));
    m_gridPowerFlow->SetCellValue(0, 1, _("Name"));
    m_gridPowerFlow->SetCellValue(0, 2, _("From"));
    m_gridPowerFlow->SetCellValue(0, 3, _("To"));
    m_gridPowerFlow->SetCellEditor(0, 4, new wxGridCellChoiceEditor(m_activePowerChoices));
    m_gridPowerFlow->SetCellValue(0, 4, m_activePowerChoices[3]);
    m_gridPowerFlow->SetCellEditor(0, 5, new wxGridCellChoiceEditor(m_reactivePowerChoices));
    m_gridPowerFlow->SetCellValue(0, 5, m_reactivePowerChoices[3]);

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

    // Power flow brances
    m_gridPFBranches->SetCellValue(0, 0, _("Type"));
    m_gridPFBranches->SetCellValue(0, 1, _("Name"));
    m_gridPFBranches->SetCellValue(0, 2, _("From"));
    m_gridPFBranches->SetCellValue(0, 3, _("To"));
    m_gridPFBranches->SetCellValue(0, 4, _("R (p.u.)"));
    m_gridPFBranches->SetCellValue(0, 5, _("XL (p.u.)"));
    m_gridPFBranches->SetCellValue(0, 6, _("B (p.u.)"));
    m_gridPFBranches->SetCellValue(0, 7, _("TAP"));
    m_gridPFBranches->SetCellValue(0, 8, _("Phase Shift"));
    m_gridPFBranches->SetCellValue(0, 9, _("Online"));
}

void DataReport::FillValues(GridSelection gridToFill)
{
    m_changingValues = true;
    ElectricCalculation eCalc;
    eCalc.GetElementsFromList(m_workspace->GetElementList());

    double basePower = m_workspace->GetProperties()->GetSimulationPropertiesData().basePower;
    switch(m_workspace->GetProperties()->GetSimulationPropertiesData().basePowerUnit) {
        case UNIT_kVA: {
            basePower *= 1e3;
        } break;
        case UNIT_MVA: {
            basePower *= 1e6;
        } break;
        default:
            break;
    }

    int rowNumber = 1;
    auto lineList = eCalc.GetLineList();
    auto transformerList = eCalc.GetTransformerList();
    auto busList = eCalc.GetBusList();

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
            if(line->IsOnline()) {
                auto data = line->GetElectricalData();

                Bus* bus1 = static_cast<Bus*>(line->GetParentList()[0]);
                Bus* bus2 = static_cast<Bus*>(line->GetParentList()[1]);

                m_gridPowerFlow->SetCellValue(rowNumber, 0, _("Line"));
                m_gridPowerFlow->SetCellValue(rowNumber, 1, data.name);
                m_gridPowerFlow->SetCellValue(rowNumber, 2, bus1->GetElectricalData().name);
                m_gridPowerFlow->SetCellValue(rowNumber, 3, bus2->GetElectricalData().name);
                m_gridPowerFlow->SetCellValue(rowNumber, 4,
                                              line->StringFromDouble(std::real(data.powerFlow[0]) * kActivePower));
                m_gridPowerFlow->SetCellValue(rowNumber, 5,
                                              line->StringFromDouble(std::imag(data.powerFlow[0]) * kReactivePower));
                rowNumber++;

                m_gridPowerFlow->SetCellValue(rowNumber, 0, _("Line"));
                m_gridPowerFlow->SetCellValue(rowNumber, 1, data.name);
                m_gridPowerFlow->SetCellValue(rowNumber, 2, bus2->GetElectricalData().name);
                m_gridPowerFlow->SetCellValue(rowNumber, 3, bus1->GetElectricalData().name);
                m_gridPowerFlow->SetCellValue(rowNumber, 4,
                                              line->StringFromDouble(std::real(data.powerFlow[1]) * kActivePower));
                m_gridPowerFlow->SetCellValue(rowNumber, 5,
                                              line->StringFromDouble(std::imag(data.powerFlow[1]) * kReactivePower));
                rowNumber++;
            }
        }

        for(auto it = transformerList.begin(), itEnd = transformerList.end(); it != itEnd; ++it) {
            Transformer* transformer = *it;
            if(transformer->IsOnline()) {
                auto data = transformer->GetElectricalData();

                Bus* bus1 = static_cast<Bus*>(transformer->GetParentList()[0]);
                Bus* bus2 = static_cast<Bus*>(transformer->GetParentList()[1]);

                m_gridPowerFlow->SetCellValue(rowNumber, 0, _("Transformer"));
                m_gridPowerFlow->SetCellValue(rowNumber, 1, data.name);
                m_gridPowerFlow->SetCellValue(rowNumber, 2, bus1->GetElectricalData().name);
                m_gridPowerFlow->SetCellValue(rowNumber, 3, bus2->GetElectricalData().name);
                m_gridPowerFlow->SetCellValue(
                    rowNumber, 4, transformer->StringFromDouble(std::real(data.powerFlow[0]) * kActivePower));
                m_gridPowerFlow->SetCellValue(
                    rowNumber, 5, transformer->StringFromDouble(std::imag(data.powerFlow[0]) * kReactivePower));
                rowNumber++;

                m_gridPowerFlow->SetCellValue(rowNumber, 0, _("Transformer"));
                m_gridPowerFlow->SetCellValue(rowNumber, 1, data.name);
                m_gridPowerFlow->SetCellValue(rowNumber, 2, bus2->GetElectricalData().name);
                m_gridPowerFlow->SetCellValue(rowNumber, 3, bus1->GetElectricalData().name);
                m_gridPowerFlow->SetCellValue(
                    rowNumber, 4, transformer->StringFromDouble(std::real(data.powerFlow[1]) * kActivePower));
                m_gridPowerFlow->SetCellValue(
                    rowNumber, 5, transformer->StringFromDouble(std::imag(data.powerFlow[1]) * kReactivePower));
                rowNumber++;
            }
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
            if(data.nominalVoltageUnit == UNIT_kV) vb *= 1e3;
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

            m_gridPFBranches->SetCellValue(rowNumber, 0, _("Line"));
            m_gridPFBranches->SetCellValue(rowNumber, 1, data.name);

            wxString busName1 = "-";
            if(line->GetParentList()[0])
                busName1 = static_cast<Bus*>(line->GetParentList()[0])->GetElectricalData().name;
            wxString busName2 = "-";
            if(line->GetParentList()[1])
                busName2 = static_cast<Bus*>(line->GetParentList()[1])->GetElectricalData().name;
            m_gridPFBranches->SetCellValue(rowNumber, 2, busName1);
            m_gridPFBranches->SetCellValue(rowNumber, 3, busName2);

            m_gridPFBranches->SetCellValue(rowNumber, 4, line->StringFromDouble(data.resistance));
            m_gridPFBranches->SetCellValue(rowNumber, 5, line->StringFromDouble(data.indReactance));
            m_gridPFBranches->SetCellValue(rowNumber, 6, line->StringFromDouble(data.capSusceptance));
            m_gridPFBranches->SetCellValue(rowNumber, 7, "-");
            m_gridPFBranches->SetCellValue(rowNumber, 8, "-");

            wxString isOnline = _("Yes");
            if(!line->IsOnline()) isOnline = _("No");
            m_gridPFBranches->SetCellValue(rowNumber, 9, isOnline);
            rowNumber++;
        }
        for(auto it = transformerList.begin(), itEnd = transformerList.end(); it != itEnd; ++it) {
            Transformer* transformer = *it;
            auto data = transformer->GetPUElectricalData(basePower);

            m_gridPFBranches->SetCellValue(rowNumber, 0, _("Transformer"));
            m_gridPFBranches->SetCellValue(rowNumber, 1, data.name);

            wxString busName1 = "-";
            if(transformer->GetParentList()[0])
                busName1 = static_cast<Bus*>(transformer->GetParentList()[0])->GetElectricalData().name;
            wxString busName2 = "-";
            if(transformer->GetParentList()[1])
                busName2 = static_cast<Bus*>(transformer->GetParentList()[1])->GetElectricalData().name;
            m_gridPFBranches->SetCellValue(rowNumber, 2, busName1);
            m_gridPFBranches->SetCellValue(rowNumber, 3, busName2);

            m_gridPFBranches->SetCellValue(rowNumber, 4, transformer->StringFromDouble(data.resistance));
            m_gridPFBranches->SetCellValue(rowNumber, 5, transformer->StringFromDouble(data.indReactance));
            m_gridPFBranches->SetCellValue(rowNumber, 6, "-");
            m_gridPFBranches->SetCellValue(rowNumber, 7, transformer->StringFromDouble(data.turnsRatio));
            m_gridPFBranches->SetCellValue(rowNumber, 8, transformer->StringFromDouble(data.phaseShift));

            wxString isOnline = _("Yes");
            if(!transformer->IsOnline()) isOnline = _("No");
            m_gridPFBranches->SetCellValue(rowNumber, 9, isOnline);
            rowNumber++;
        }
        m_gridPFBranches->AutoSize();
    }

    m_changingValues = false;
}

void DataReport::CreateGrids()
{
    wxColour headerColour(200, 200, 200);
    wxFont headerFont = m_gridPowerFlow->GetLabelFont();
    headerFont.SetWeight(wxFONTWEIGHT_BOLD);

    ElectricCalculation eCalc;
    eCalc.GetElementsFromList(m_workspace->GetElementList());

    // Power Flow
    // Header
    m_gridPowerFlow->AppendCols(6);
    m_gridPowerFlow->AppendRows();
    m_gridPowerFlow->HideColLabels();
    m_gridPowerFlow->HideRowLabels();
    for(int i = 0; i < 6; ++i) {
        m_gridPowerFlow->SetCellBackgroundColour(0, i, headerColour);
        m_gridPowerFlow->SetCellFont(0, i, headerFont);
    }
    m_gridPowerFlow->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    // Values
    auto lineList = eCalc.GetLineList();
    for(auto it = lineList.begin(), itEnd = lineList.end(); it != itEnd; ++it) {
        Line* line = *it;
        if(line->IsOnline()) {
            m_gridPowerFlow->AppendRows(2);
        }
    }
    auto transformerList = eCalc.GetTransformerList();
    for(auto it = transformerList.begin(), itEnd = transformerList.end(); it != itEnd; ++it) {
        Transformer* transformer = *it;
        if(transformer->IsOnline()) {
            m_gridPowerFlow->AppendRows(2);
        }
    }
    m_gridPowerFlow->AutoSize();

    // Power Flow buses
    // Header
    m_gridPFBuses->AppendCols(6);
    m_gridPFBuses->AppendRows();
    m_gridPFBuses->HideColLabels();
    m_gridPFBuses->HideRowLabels();
    for(int i = 0; i < 6; ++i) {
        m_gridPFBuses->SetCellBackgroundColour(0, i, headerColour);
        m_gridPFBuses->SetCellFont(0, i, headerFont);
    }
    m_gridPFBuses->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    // Values
    auto busList = eCalc.GetBusList();
    m_gridPFBuses->AppendRows(busList.size());
    m_gridPFBuses->AutoSize();

    // Power flow branches
    // Header
    m_gridPFBranches->AppendCols(10);
    m_gridPFBranches->AppendRows(1);
    m_gridPFBranches->HideColLabels();
    m_gridPFBranches->HideRowLabels();
    for(int i = 0; i < 10; ++i) {
        m_gridPFBranches->SetCellBackgroundColour(0, i, headerColour);
        m_gridPFBranches->SetCellFont(0, i, headerFont);
    }
    m_gridPFBranches->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    // Values
    m_gridPFBranches->AppendRows(lineList.size() + transformerList.size());
    m_gridPFBranches->AutoSize();
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
