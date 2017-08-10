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
    bool faultAfterPowerFlow = true;
    bool scPowerAfterPowerFlow = true;

    // Power flow
    PowerFlowMethod powerFlowMethod = GAUSS_SEIDEL;
    double accFator = 1.0;
    double powerFlowTolerance = 1e-7;
    int powerFlowMaxIterations = 5000;

    // Stability
    double stabilityFrequency = 60.0;
    double timeStep = 1e-2;
    double stabilitySimulationTime = 10.0;
    double stabilityTolerance = 1e-8;
    int stabilityMaxIterations = 100;
    int controlTimeStepRatio = 10;
    double plotTime = 1e-2;
};

struct GeneralData {
    wxLanguage language = wxLANGUAGE_ENGLISH;
    GUITheme theme = THEME_LIGHT;
};

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
