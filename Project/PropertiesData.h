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

#ifndef PROPERTIESDATA_H
#define PROPERTIESDATA_H

#include "wx/language.h"
#include "Element.h"
#include "PowerElement.h"

enum PowerFlowMethod { GAUSS_SEIDEL = 0, NEWTON_RAPHSON };
enum GUITheme { THEME_LIGHT = 0, THEME_DARK };

struct SimulationData {
    // General simulation data
    double basePower = 100.0;
    ElectricalUnit basePowerUnit = UNIT_MVA;
    bool faultAfterPowerFlow = false;
    bool scPowerAfterPowerFlow = false;

    // Power flow
    PowerFlowMethod powerFlowMethod = GAUSS_SEIDEL;
    double accFator = 1.0;
    double powerFlowTolerance = 1e-7;
    int powerFlowMaxIterations = 5000;
    double initAngle = 0.0;

    // Stability
    double stabilityFrequency = 60.0;
    double timeStep = 1e-2;
    double stabilitySimulationTime = 10.0;
    double stabilityTolerance = 1e-8;
    int stabilityMaxIterations = 100;
    int controlTimeStepRatio = 10;
    double plotTime = 1e-2;
    bool useCOI = true;
    
    // ZIP load
    bool useCompLoads = false;
    double constImpedanceActive = 100.0;
    double constCurrentActive = 0.0;
    double constPowerActive = 0.0;
    double constImpedanceReactive = 100.0;
    double constCurrentReactive = 0.0;
    double constPowerReactive = 0.0;
    double underVoltageConstCurrent = 70.0;
    double underVoltageConstPower = 70.0;
};

struct GeneralData {
    wxLanguage language = wxLANGUAGE_ENGLISH;
    GUITheme theme = THEME_LIGHT;
};

/**
 * @class PropertiesData
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief General and simulation data manager.
 * @file PropertiesData.h
 */
class PropertiesData
{
   public:
    PropertiesData();
    ~PropertiesData();

    SimulationData GetSimulationPropertiesData() const { return m_simulData; }
    void SetSimulationPropertiesData(SimulationData simulationData) { m_simulData = simulationData; }
    GeneralData GetGeneralPropertiesData() const { return m_genData; }
    void SetGeneralPropertiesData(GeneralData generalData) { m_genData = generalData; }
   protected:
    SimulationData m_simulData;
    GeneralData m_genData;
};

#endif  // PROPERTIESDATA_H
