#include "Electromechanical.h"

Electromechanical::Electromechanical(std::vector<Element*> elementList)
{
    GetElementsFromList(elementList);
    SetEventTimeList();
}

Electromechanical::~Electromechanical() {}
void Electromechanical::SetEventTimeList()
{
    // Fault
    for(auto it = m_busList.begin(), itEnd = m_busList.end(); it != itEnd; ++it) {
        Bus* bus = *it;
        auto data = bus->GetEletricalData();
        if(data.stabHasFault) {
            m_eventTimeList.push_back(data.stabFaultTime);
            m_eventOccurrenceList.push_back(false);
            m_eventTimeList.push_back(data.stabFaultTime + data.stabFaultLength);
            m_eventOccurrenceList.push_back(false);
        }
    }
    // Switching
    for(auto it = m_powerElementList.begin(), itEnd = m_powerElementList.end(); it != itEnd; ++it) {
        PowerElement* element = *it;
        SwitchingData swData = element->GetSwitchingData();
        for(unsigned int i = 0; i < swData.swTime.size(); ++i) {
            m_eventTimeList.push_back(swData.swTime[i]);
            m_eventOccurrenceList.push_back(false);
        }
    }
}

bool Electromechanical::HasEvent(double currentTime)
{
    for(unsigned int i = 0; i < m_eventTimeList.size(); ++i) {
        if(!m_eventOccurrenceList[i]) {
            if((m_eventTimeList[i] - m_timeStep) < currentTime && m_eventTimeList[i] >= currentTime) {
                m_eventOccurrenceList[i] = true;
                return true;
            }
        }
    }
    return false;
}

void Electromechanical::SetEvent(double currentTime) {}
bool Electromechanical::RunStabilityCalculation()
{
    // test
    double simTime = 10.0;
    double currentTime = 0.0;
    while(currentTime <= simTime) {
        if(HasEvent(currentTime)) {
        }
        currentTime += m_timeStep;
    }
    return true;
}
