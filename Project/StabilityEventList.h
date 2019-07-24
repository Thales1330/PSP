#ifndef STABILITYEVENTLIST_H
#define STABILITYEVENTLIST_H
#include "DataReportBase.h"

class Element;
class PowerElement;
class Bus;
class ElectricCalculation;

class StabilityEventList : public StabilityEventListBase
{
   public:
    StabilityEventList(wxWindow* parent, std::vector<Element*> elementList);
    virtual ~StabilityEventList();

   protected:
    virtual void OnOKButtonClick(wxCommandEvent& event) { EndModal(wxID_OK); };
    virtual void GetTimeEventsList();
    virtual void AddEvent(double eventTime, wxString eventType, wxString eventDescription, wxColour eventColour);
    virtual void FillGrid();
    virtual void SortEvents();
    virtual void SetRowsColours(wxGrid* grid, int rowStart = 1);
    void SetPowerElementSwitchingEvent(PowerElement* element, wxString elementName);

    std::vector<Element*> m_elementList;
    std::vector<double> m_time;
    std::vector<wxString> m_eventType;
    std::vector<wxString> m_eventDescription;
    std::vector<wxColour> m_eventColour;

    // Cell colours
    wxColour m_headerColour = wxColour(150, 150, 150);
    wxColour m_oddRowColour = wxColour(220, 220, 220);
    wxColour m_evenRowColour = wxColour(255, 255, 255);
    wxColour m_redColour = wxColour(255, 0, 0);
    wxColour m_blueColour = wxColour(0, 0, 255);
};
#endif  // STABILITYEVENTLIST_H
