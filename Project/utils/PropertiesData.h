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

#include "../elements/Element.h"
#include "../elements/powerElement/PowerElement.h"
#include "wx/language.h"

enum PowerFlowMethod { GAUSS_SEIDEL = 0, NEWTON_RAPHSON, GAUSS_NEWTON };
enum GUITheme { THEME_LIGHT = 0, THEME_DARK };
enum class HarmLoadConnection { PARALLEL = 0, SERIES, DISCONNECTED };
enum class PlotLib { wxMATH_PLOT = 0, wxCHART_DIR = 1 };
enum class BusFreqEstimation { ANGLE_DERIVATION = 0, WASHOUT_FILTER = 1};

struct SimulationData {
	// General simulation data
	double basePower = 100.0;
	ElectricalUnit basePowerUnit = ElectricalUnit::UNIT_MVA;
	bool faultAfterPowerFlow = false;
	bool scPowerAfterPowerFlow = false;
	bool harmDistortionAfterPowerFlow = false;

	// Power flow
	PowerFlowMethod powerFlowMethod = GAUSS_SEIDEL;
	double accFator = 1.0;
	double powerFlowTolerance = 1e-7;
	int powerFlowMaxIterations = 5000;
	double initAngle = 0.0;
	double newtonInertia = 1.0;
	double gaussTolerance = 1e-2;

	// Stability
	double stabilityFrequency = 60.0;
	double timeStep = 1e-2;
	double stabilitySimulationTime = 10.0;
	double stabilityTolerance = 1e-8;
	int stabilityMaxIterations = 100;
	int controlTimeStepRatio = 10;
	double plotTime = 1e-2;
	bool useCOI = true;
	BusFreqEstimation busFreqEstimation = BusFreqEstimation::ANGLE_DERIVATION;
	bool ignoreBusFreqEventStep = true;
	double tf = 0.05;
	double tw = 3.0 / (2.0 * M_PI * 60.0);

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

	// Hamonics
	HarmLoadConnection harmLoadConnection = HarmLoadConnection::PARALLEL;
};

struct GeneralData {
	wxLanguage language = wxLANGUAGE_ENGLISH;
	GUITheme theme = THEME_LIGHT;
	//bool useOpenGL = true;
	PlotLib plotLib = PlotLib::wxCHART_DIR;
	wxFileName atpPath;
	wxString atpWorkFolder = "";
};

struct FreqResponseData {
	double initFreq = 0.0;
	double finalFreq = 1500.0;
	double stepFreq = 1.0;
	int injBusNumber = -1;
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

	void SetFreqRespData(const FreqResponseData& freqRespData) { this->m_freqRespData = freqRespData; }
	const FreqResponseData& GetFreqRespData() const { return m_freqRespData; }

protected:
	SimulationData m_simulData;
	GeneralData m_genData;
	FreqResponseData m_freqRespData;
};

#endif  // PROPERTIESDATA_H
