#ifndef ELECTROMECHANICAL_H
#define ELECTROMECHANICAL_H

#include "ElectricCalculation.h"

#include <wx/progdlg.h>

class ControlElementSolver;

class Electromechanical : public ElectricCalculation
{
   public:
    Electromechanical(wxWindow* parent, std::vector<Element*> elementList, SimulationData data);
    ~Electromechanical();

    bool RunStabilityCalculation();
    wxString GetErrorMessage() const { return m_errorMsg; }
    std::vector<double> GetTimeVector() const { return m_timeVector; }
    std::vector<double> m_wErrorVector;
    std::vector<double> m_deltaErrorVector;
    std::vector<double> m_transEdErrorVector;
    std::vector<double> m_transEqErrorVector;
    std::vector<double> m_numItVector;

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
    void CalculateIntegrationConstants(SyncGenerator* syncGenerator, double id, double iq, double k = 1.0);
    bool SolveSynchronousMachines();
    void SetSyncMachinesModel();
    double CalculateSyncMachineIntVariables(SyncGenerator* syncGenerator, double id, double iq, double pe, double k = 1.0);
    void CalculateSyncMachineNonIntVariables(SyncGenerator* syncGenerator, double& id, double& iq, double& pe, double k = 1.0);
    void CalculateReferenceSpeed();

    void SaveData();

    wxWindow* m_parent = NULL;
    wxString m_errorMsg = _("Unknown error");

    double m_systemFreq = 60.0;
    double m_refSpeed = 2.0 * M_PI * 60.0;
    bool m_useCOI = false;

    std::vector<std::vector<std::complex<double> > > m_yBus;
    std::vector<std::vector<std::complex<double> > > m_yBusU;
    std::vector<std::vector<std::complex<double> > > m_yBusL;

    std::vector<std::complex<double> > m_vBus;
    std::vector<std::complex<double> > m_iBus;

    double m_powerSystemBase = 100e6;
    double m_simTime = 10.0;
    double m_plotTime = 1e-2;
    double m_timeStep = 1e-2;
    double m_ctrlTimeStepMultiplier = 0.1;
    double m_tolerance = 1e-8;
    int m_maxIterations = 100;

    std::vector<double> m_eventTimeList;
    std::vector<bool> m_eventOccurrenceList;

    std::vector<double> m_timeVector;

    // tests
    double m_wError = 0.0;
    double m_numIt = 0;
};

#endif  // ELECTROMECHANICAL_H
