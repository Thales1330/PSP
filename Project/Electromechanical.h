#ifndef ELECTROMECHANICAL_H
#define ELECTROMECHANICAL_H

#include "ElectricCalculation.h"

class ControlElementSolver;

class Electromechanical : public ElectricCalculation
{
   public:
    Electromechanical(wxWindow* parent, std::vector<Element*> elementList);
    ~Electromechanical();

    bool RunStabilityCalculation();
    wxString GetErrorMessage() const { return m_errorMsg; }
    
    std::vector<double> GetTimeVector() const { return m_timeVector; }
    
   protected:
    void SetEventTimeList();
    bool HasEvent(double currentTime);
    void SetEvent(double currentTime);
    inline bool EventTrigger(double eventTime, double currentTime);
    
    double GetPowerValue(double value, ElectricalUnit unit);

    void InsertSyncMachinesOnYBus();
    std::complex<double> GetSyncMachineAdmittance(SyncGenerator* generator);
    bool InitializeDynamicElements();
    void CalculateMachinesCurrents();
    void CalculateIntegrationConstants(SyncGenerator* syncGenerator, double id, double iq);
    bool SolveSynchronousMachines();
    
    void SaveData();

    wxWindow* m_parent = NULL;
    wxString m_errorMsg = _("Unknown error");
    
    double m_systemFreq = 60.0;
    
    std::vector<std::vector<std::complex<double> > > m_yBus;
    std::vector<std::vector<std::complex<double> > > m_yBusU;
    std::vector<std::vector<std::complex<double> > > m_yBusL;
    
    std::vector<std::complex<double> > m_vBus;
    std::vector<std::complex<double> > m_iBus;
    
    double m_powerSystemBase = 100e6;

    double m_timeStep = 1e-3;
    double m_tolerance = 1e-5;
    int m_maxIterations = 100;

    std::vector<double> m_eventTimeList;
    std::vector<bool> m_eventOccurrenceList;
    
    std::vector<double> m_timeVector;
};

#endif  // ELECTROMECHANICAL_H
