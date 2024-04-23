#ifndef POWERQUALITY_H
#define POWERQUALITY_H

#include "ElectricCalculation.h"

#include <wx/intl.h>  //_()
#include <wx/string.h>

/**
 * @class PowerQuality
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 24/04/2019
 * @brief Responsible for the power quality calculations.
 * @file PowerQuality.h
 */
class PowerQuality : public ElectricCalculation
{
public:
	struct HarmonicYbus {
		double order;
		std::vector<std::vector<std::complex<double> > > yBus;
	};

	PowerQuality();
	PowerQuality(std::vector<Element*> elementList);
	~PowerQuality();

	virtual wxString GetErrorMessage() { return m_errorMsg; }

	virtual void CalculateHarmonicYbusList(double systemPowerBase = 100e6, HarmLoadConnection loadConnection = HarmLoadConnection::PARALLEL);
	virtual void CalculateHarmonicYbus(std::vector<std::vector<std::complex<double> > >& yBus,
		double systemPowerBase,
		double order,
		bool ignoreTransformerConnection = false,
		HarmLoadConnection loadConnection = HarmLoadConnection::PARALLEL);

	virtual bool CalculateDistortions(double systemPowerBase = 100e6, HarmLoadConnection loadConnection = HarmLoadConnection::PARALLEL);
	virtual bool CalculateFrequencyResponse(double systemFreq = 60.0,
		double initFreq = 0.0,
		double endFreq = 1500.0,
		double stepFreq = 1.0,
		int injBusNumber = 0,
		double systemPowerBase = 100e6);
	virtual std::vector<double> GetHarmonicOrdersList();
	virtual std::vector<double> GetFrequencies() { return m_frequencyList; }

protected:
	std::vector<std::vector< std::complex<double> > > GetTransformerHarmAdmmitance(Transformer* transformer, double systemPowerBase, double hOrder, bool ignoreConnection = false);

	std::vector<HarmonicYbus> m_harmYbusList;
	std::vector<double> m_frequencyList;
	wxString m_errorMsg = "";
};

#endif  // POWERQUALITY_H
