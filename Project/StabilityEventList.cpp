#include "StabilityEventList.h"

#include "Bus.h"
#include "ElectricCalculation.h"
#include "Element.h"
#include "PowerElement.h"

StabilityEventList::StabilityEventList(wxWindow* parent, std::vector<Element*> elementList)
    : StabilityEventListBase(parent)
{
    m_elementList = elementList;

    m_time.clear();
    m_eventType.clear();
    m_eventDescription.clear();
    m_eventColour.clear();

    GetTimeEventsList();
    SortEvents();
    FillGrid();
    SetRowsColours(m_gridStabEventList);

    SetSize(GetBestSize());
}

StabilityEventList::~StabilityEventList() {}

void StabilityEventList::GetTimeEventsList()
{
    ElectricCalculation eCalc;
    eCalc.GetElementsFromList(m_elementList);
    auto busList = eCalc.GetBusList();
    for(unsigned int i = 0; i < busList.size(); ++i) {
        Bus* bus = busList[i];
        auto data = bus->GetElectricalData();
        if(data.stabHasFault) {
            AddEvent(data.stabFaultTime, _("Fault"),
                     _("Fault insertion at \"") + data.name + _("\" (Zf = ") +
                         bus->StringFromDouble(data.stabFaultResistance) + wxT(" +j") +
                         bus->StringFromDouble(data.stabFaultReactance) + wxT(" p.u.)"),
                     m_redColour);
            AddEvent(data.stabFaultTime + data.stabFaultLength, _("Fault"),
                     _("Fault removal at \"") + data.name + _("\" (Zf = ") +
                         bus->StringFromDouble(data.stabFaultResistance) + wxT(" +j") +
                         bus->StringFromDouble(data.stabFaultReactance) + wxT(" p.u.)"),
                     m_blueColour);
        }
    }
    auto capacitorList = eCalc.GetCapacitorList();
    for(unsigned int i = 0; i < capacitorList.size(); ++i) {
        Capacitor* capacitor = capacitorList[i];
        SetPowerElementSwitchingEvent(capacitor, capacitor->GetElectricalData().name);
    }
    auto indMotorList = eCalc.GetIndMotorList();
    for(unsigned int i = 0; i < indMotorList.size(); ++i) {
        IndMotor* indMotor = indMotorList[i];
        SetPowerElementSwitchingEvent(indMotor, indMotor->GetElectricalData().name);
    }
    auto inductorList = eCalc.GetInductorList();
    for(unsigned int i = 0; i < inductorList.size(); ++i) {
        Inductor* inductor = inductorList[i];
        SetPowerElementSwitchingEvent(inductor, inductor->GetElectricalData().name);
    }
    auto lineList = eCalc.GetLineList();
    for(unsigned int i = 0; i < lineList.size(); ++i) {
        Line* line = lineList[i];
        SetPowerElementSwitchingEvent(line, line->GetElectricalData().name);
    }
    auto loadList = eCalc.GetLoadList();
    for(unsigned int i = 0; i < loadList.size(); ++i) {
        Load* load = loadList[i];
        SetPowerElementSwitchingEvent(load, load->GetElectricalData().name);
    }
    auto syncGeneratorList = eCalc.GetSyncGeneratorList();
    for(unsigned int i = 0; i < syncGeneratorList.size(); ++i) {
        SyncGenerator* syncGenerator = syncGeneratorList[i];
        SetPowerElementSwitchingEvent(syncGenerator, syncGenerator->GetElectricalData().name);
    }
    auto syncMotorList = eCalc.GetSyncMotorList();
    for(unsigned int i = 0; i < syncMotorList.size(); ++i) {
        SyncMotor* syncMotor = syncMotorList[i];
        SetPowerElementSwitchingEvent(syncMotor, syncMotor->GetElectricalData().name);
    }
    auto transformerList = eCalc.GetTransformerList();
    for(unsigned int i = 0; i < transformerList.size(); ++i) {
        Transformer* transformer = transformerList[i];
        SetPowerElementSwitchingEvent(transformer, transformer->GetElectricalData().name);
    }
}

void StabilityEventList::AddEvent(double eventTime, wxString eventType, wxString eventDescription, wxColour eventColour)
{
    m_time.emplace_back(eventTime);
    m_eventType.emplace_back(eventType);
    m_eventDescription.emplace_back(eventDescription);
    m_eventColour.emplace_back(eventColour);
}

void StabilityEventList::FillGrid()
{
    wxFont headerFont = m_gridStabEventList->GetLabelFont();
    headerFont.SetWeight(wxFONTWEIGHT_BOLD);

    // Create Grid
    // Header
    m_gridStabEventList->AppendCols(3);
    m_gridStabEventList->AppendRows();
    m_gridStabEventList->HideColLabels();
    m_gridStabEventList->HideRowLabels();
    for(int i = 0; i < 7; ++i) {
        m_gridStabEventList->SetCellBackgroundColour(0, i, m_headerColour);
        m_gridStabEventList->SetCellFont(0, i, headerFont);
    }
    m_gridStabEventList->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    // Values
    m_gridStabEventList->AppendRows(m_time.size());

    // Set headers
    m_gridStabEventList->SetCellValue(0, 0, _("Event time"));
    m_gridStabEventList->SetCellValue(0, 1, _("Type"));
    m_gridStabEventList->SetCellValue(0, 2, _("Description"));

    // Fill Values
    for(unsigned int i = 0; i < m_time.size(); ++i) {
        m_gridStabEventList->SetCellValue(i + 1, 0, Element::StringFromDouble(m_time[i]) + wxT(" s"));
        m_gridStabEventList->SetCellValue(i + 1, 1, m_eventType[i]);
        m_gridStabEventList->SetCellValue(i + 1, 2, m_eventDescription[i]);
        m_gridStabEventList->SetCellAlignment(i + 1, 2, wxALIGN_LEFT, wxALIGN_CENTRE);
        for(int j = 0; j < 3; ++j) { m_gridStabEventList->SetCellTextColour(i + 1, j, m_eventColour[i]); }
    }

    m_gridStabEventList->AutoSize();
}

void StabilityEventList::SetRowsColours(wxGrid* grid, int rowStart)
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

void StabilityEventList::SetPowerElementSwitchingEvent(PowerElement* element, wxString elementName)
{
    SwitchingData swData = element->GetSwitchingData();
    for(unsigned int i = 0; i < swData.swTime.size(); ++i) {
        if(swData.swType[i] == SW_INSERT) {
            AddEvent(swData.swTime[i], _("Switching"), _("Insertion of \"") + elementName + _("\""), m_blueColour);
        } else {
            AddEvent(swData.swTime[i], _("Switching"), _("Removal of \"") + elementName + _("\""), m_redColour);
        }
    }
}

void StabilityEventList::SortEvents()
{
    for(unsigned int i = 0; i < m_time.size(); ++i) {
        for(unsigned int j = 0; j < m_time.size(); ++j) {
            if(m_time[i] < m_time[j]) {
                double time = m_time[i];
                wxString type = m_eventType[i];
                wxString description = m_eventDescription[i];
                wxColour colour = m_eventColour[i];
                m_time[i] = m_time[j];
                m_eventType[i] = m_eventType[j];
                m_eventDescription[i] = m_eventDescription[j];
                m_eventColour[i] = m_eventColour[j];
                m_time[j] = time;
                m_eventType[j] = type;
                m_eventDescription[j] = description;
                m_eventColour[j] = colour;
            }
        }
    }
}
