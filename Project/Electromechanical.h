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

#ifndef ELECTROMECHANICAL_H
#define ELECTROMECHANICAL_H

#include "ElectricCalculation.h"

#include <wx/log.h>
#include <wx/progdlg.h>

class ControlElementSolver;

/**
 * @brief Synchronous machine data for different models
 *
 * This struct have the parameters and calculated data that depends on each machine model.
 */
struct SyncMachineModelData {
    /** Direct-axis reactance (transient for models 1, 2 and 3; subtransient for models 4 and 5) */
    double xd;
    /** Quadrature-axis reactance (transient for models 1, 2 and 3; subtransient for models 4 and 5) */
    double xq;
    /** Direct-axis internal voltage (transient for models 1, 2 and 3; subtransient for models 4 and 5) */
    double ed;
    /** Quadrature-axis internal voltage (transient for models 1, 2 and 3; subtransient for models 4 and 5) */
    double eq;
};

/**
 * @class Electromechanical
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 23/09/2017
 * @brief Calculates the electromechanical transient based on disturbances (e.g. system fault).
 * @file Electromechanical.h
 */
class Electromechanical : public ElectricCalculation
{
   public:
    Electromechanical(wxWindow* parent, std::vector<Element*> elementList, SimulationData data);
    ~Electromechanical();

    bool RunStabilityCalculation();
    wxString GetErrorMessage() const { return m_errorMsg; }
    std::vector<double> GetTimeVector() const { return m_timeVector; }

    // For tests
    std::vector<double> GetIterationVector() const { return m_iterationsNumVector; }

   protected:
    void SetEventTimeList();
    bool HasEvent(double currentTime);
    void SetEvent(double currentTime);
    inline bool EventTrigger(double eventTime, double currentTime);

    // double GetPowerValue(double value, ElectricalUnit unit);

    void InsertSyncMachinesOnYBus();
    bool InsertIndMachinesOnYBus();
    bool CalculateIndMachinesTransientValues(IndMotor* motor);
    std::complex<double> GetSyncMachineAdmittance(SyncGenerator* generator);
    std::complex<double> GetIndMachineAdmittance(IndMotor* motor);
    bool InitializeDynamicElements();
    bool CalculateInjectedCurrents();
    void CalculateIntegrationConstants(SyncGenerator* syncGenerator, double id, double iq, double k = 1.0);
    void CalculateIntegrationConstants(IndMotor* indMotor, double ir, double im, double k = 1.0);
    bool SolveMachines();
    void SetSyncMachinesModel();
    SyncMachineModelData GetSyncMachineModelData(SyncGenerator* syncMachine);
    double CalculateIntVariables(SyncGenerator* syncGenerator,
                                 double id,
                                 double iq,
                                 double sd,
                                 double sq,
                                 double pe,
                                 double k = 1.0);
    double CalculateIntVariables(IndMotor* indMotor, double ir, double im, double te, double k = 1.0);
    bool CalculateNonIntVariables(SyncGenerator* syncGenerator,
                                  double& id,
                                  double& iq,
                                  double& sd,
                                  double& sq,
                                  double& pe,
                                  double k = 1.0);
    bool CalculateNonIntVariables(IndMotor* indMotor, double& ir, double& im, double& te, double k = 1.0);
    void CalculateReferenceSpeed();
    bool CalculateSyncMachineSaturation(SyncGenerator* syncMachine,
                                        double& id,
                                        double& iq,
                                        double& sd,
                                        double& sq,
                                        bool updateCurrents = true,
                                        double k = 1.0);

    void SaveData();
    void PreallocateVectors();

    wxWindow* m_parent = nullptr;
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
    double m_currentTime = 0.0;
    double m_plotTime = 1e-2;
    double m_timeStep = 1e-2;
    double m_ctrlTimeStepMultiplier = 0.1;
    double m_tolerance = 1e-8;
    int m_maxIterations = 100;
    double m_saturationTolerance = 1e-8;

    int m_currentPoint = 0;

    std::vector<double> m_eventTimeList;
    std::vector<bool> m_eventOccurrenceList;

    std::vector<double> m_timeVector;

    // For tests
    int m_iterationsNum = 0.0;
    std::vector<double> m_iterationsNumVector;
};

#endif  // ELECTROMECHANICAL_H
