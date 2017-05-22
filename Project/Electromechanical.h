#ifndef ELECTROMECHANICAL_H
#define ELECTROMECHANICAL_H

#include "ElectricCalculation.h"

class Electromechanical : public ElectricCalculation
{
public:
    Electromechanical(std::vector<Element*> elementList);
    ~Electromechanical();
    
    bool RunStabilityCalculation();
    wxString GetErrorMessage() const { return m_errorMsg; }
    
protected:
    void SetEventTimeList();
    bool HasEvent(double currentTime);
    void SetEvent(double currentTime);
    inline bool EventTrigger(double eventTime, double currentTime);
    
    void InsertSyncMachinesOnYBus();
    std::complex<double> GetSyncMachineAdmittance(SyncGenerator* generator);
    
    wxString m_errorMsg = _("Unknown error");
    
    std::vector<std::vector<std::complex<double> > > m_yBus;
    double m_powerSystemBase = 100e6;
    
    double m_timeStep = 1e-3;
    
    std::vector<double> m_eventTimeList;
    std::vector<bool> m_eventOccurrenceList;
};

#endif // ELECTROMECHANICAL_H
