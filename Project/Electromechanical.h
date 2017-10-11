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

#include <wx/progdlg.h>
#include <wx/log.h>

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

   protected:
    void SetEventTimeList();
    bool HasEvent(double currentTime);
    void SetEvent(double currentTime);
    inline bool EventTrigger(double eventTime, double currentTime);

    // double GetPowerValue(double value, ElectricalUnit unit);

    void InsertSyncMachinesOnYBus();
    std::complex<double> GetSyncMachineAdmittance(SyncGenerator* generator);
    bool InitializeDynamicElements();
    bool CalculateMachinesCurrents();
    void CalculateIntegrationConstants(SyncGenerator* syncGenerator, double id, double iq, double k = 1.0);
    bool SolveSynchronousMachines();
    void SetSyncMachinesModel();
    SyncMachineModelData GetSyncMachineModelData(SyncGenerator* syncMachine);
    double CalculateSyncMachineIntVariables(SyncGenerator* syncGenerator,
                                            double id,
                                            double iq,
                                            double sd,
                                            double sq,
                                            double pe,
                                            double k = 1.0);
    bool CalculateSyncMachineNonIntVariables(SyncGenerator* syncGenerator,
                                             double& id,
                                             double& iq,
                                             double& sd,
                                             double& sq,
                                             double& pe,
                                             double k = 1.0);
    void CalculateReferenceSpeed();
    bool CalculateSyncMachineSaturation(SyncGenerator* syncMachine,
                                        double& id,
                                        double& iq,
                                        double& sd,
                                        double& sq,
                                        bool updateCurrents = true,
                                        double k = 1.0);

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
    double m_saturationTolerance = 1e-8;

    std::vector<double> m_eventTimeList;
    std::vector<bool> m_eventOccurrenceList;

    std::vector<double> m_timeVector;
};

#endif  // ELECTROMECHANICAL_H
