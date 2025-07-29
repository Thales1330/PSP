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

#include "PowerFlow.h"

PowerFlow::PowerFlow() : ElectricCalculation() {}
PowerFlow::PowerFlow(std::vector<Element*> elementList) : ElectricCalculation() { GetElementsFromList(elementList); }
PowerFlow::~PowerFlow() {}
bool PowerFlow::InitPowerFlow(std::vector<BusType>& busType,
	std::vector<std::complex<double> >& voltage,
	std::vector<std::complex<double> >& power,
	std::vector<std::complex<double> >& loadPower,
	std::vector<ReactiveLimits>& reactiveLimit,
	double systemPowerBase,
	double initAngle)
{
	double radInitAngle = wxDegToRad(initAngle);

	// Calculate EMT Elements admittance to add to the Ybus.
	if (!CalculateEMTElementsAdmittance(systemPowerBase, m_errorMsg)) return false;

	// Calculate the Ybus.
	if (!GetYBus(m_yBus, systemPowerBase)) {
		m_errorMsg = _("No buses found on the system.");
		return false;
	}

	// Calculate EMT elements power for 1.0 p.u. voltage
	if (!CalculateEMTElementsPower(systemPowerBase, m_errorMsg, false)) return false;

	// Number of buses in the system.
	//m_numberOfBuses = static_cast<int>(m_busList.size());
	m_numberOfBuses = static_cast<int>(m_yBus.size());

	busType.clear();
	voltage.clear();
	power.clear();
	loadPower.clear();
	reactiveLimit.clear();

	reactiveLimit.resize(m_numberOfBuses);

	int busNumber = 0;
	Bus* slackBus = nullptr;
	for (auto itb = m_busList.begin(); itb != m_busList.end(); itb++) {
		Bus* bus = *itb;
		BusElectricalData data = bus->GetElectricalData();
		if (data.isConnected) {
			// Fill the bus type
			if (data.slackBus) {
				busType.push_back(BUS_SLACK);
				slackBus = bus;
			}
			// If the bus have controlled voltage, check if at least one synchronous machine is connected, then set the
			// bus type.
			else if (data.isVoltageControlled) {
				bool hasSyncMachine = false;
				// Synchronous generator
				for (auto itsg = m_syncGeneratorList.begin(); itsg != m_syncGeneratorList.end(); itsg++) {
					SyncGenerator* syncGenerator = *itsg;
					if (bus == syncGenerator->GetParentList()[0] && syncGenerator->IsOnline()) hasSyncMachine = true;
				}
				// Synchronous motor
				for (auto itsm = m_syncMotorList.begin(); itsm != m_syncMotorList.end(); itsm++) {
					SyncMotor* syncMotor = *itsm;
					if (bus == syncMotor->GetParentList()[0] && syncMotor->IsOnline()) hasSyncMachine = true;
				}
				if (hasSyncMachine)
					busType.push_back(BUS_PV);
				else
					busType.push_back(BUS_PQ);
			}
			else
				busType.push_back(BUS_PQ);

			// Fill the voltages array
			if (data.isVoltageControlled && busType[busNumber] != BUS_PQ) {
				voltage.push_back(std::complex<double>(data.controlledVoltage * std::cos(radInitAngle),
					data.controlledVoltage * std::sin(radInitAngle)));
			}
			else {
				voltage.push_back(std::complex<double>(std::cos(radInitAngle), std::sin(radInitAngle)));
			}

			// Fill the power array
			power.push_back(std::complex<double>(0.0, 0.0));  // Initial value
			loadPower.push_back(std::complex<double>(0.0, 0.0));

			// Synchronous generator
			for (auto itsg = m_syncGeneratorList.begin(); itsg != m_syncGeneratorList.end(); itsg++) {
				SyncGenerator* syncGenerator = *itsg;
				if (syncGenerator->IsOnline()) {
					if (bus == syncGenerator->GetParentList()[0]) {
						SyncGeneratorElectricalData childData = syncGenerator->GetPUElectricalData(systemPowerBase);
						power[busNumber] += std::complex<double>(childData.activePower, childData.reactivePower);

						if (busType[busNumber] == BUS_PV) {
							if (childData.haveMaxReactive && reactiveLimit[busNumber].maxLimitType != RL_UNLIMITED_SOURCE) {
								reactiveLimit[busNumber].maxLimitType = RL_LIMITED;
								reactiveLimit[busNumber].maxLimit += childData.maxReactive;
							}
							else if (!childData.haveMaxReactive)
								reactiveLimit[busNumber].maxLimitType = RL_UNLIMITED_SOURCE;

							if (childData.haveMinReactive && reactiveLimit[busNumber].minLimitType != RL_UNLIMITED_SOURCE) {
								reactiveLimit[busNumber].minLimitType = RL_LIMITED;
								reactiveLimit[busNumber].minLimit += childData.minReactive;
							}
							else if (!childData.haveMinReactive)
								reactiveLimit[busNumber].minLimitType = RL_UNLIMITED_SOURCE;
						}
					}
				}
			}
			// Synchronous motor
			for (auto itsm = m_syncMotorList.begin(); itsm != m_syncMotorList.end(); itsm++) {
				SyncMotor* syncMotor = *itsm;
				if (syncMotor->IsOnline()) {
					if (bus == syncMotor->GetParentList()[0]) {
						SyncMotorElectricalData childData = syncMotor->GetPUElectricalData(systemPowerBase);
						power[busNumber] += std::complex<double>(-childData.activePower, childData.reactivePower);
						loadPower[busNumber] += std::complex<double>(-childData.activePower, 0.0);

						if (busType[busNumber] == BUS_PV) {
							if (childData.haveMaxReactive && reactiveLimit[busNumber].maxLimitType != RL_UNLIMITED_SOURCE) {
								reactiveLimit[busNumber].maxLimitType = RL_LIMITED;
								reactiveLimit[busNumber].maxLimit += childData.maxReactive;
							}
							else if (!childData.haveMaxReactive)
								reactiveLimit[busNumber].maxLimitType = RL_UNLIMITED_SOURCE;

							if (childData.haveMinReactive && reactiveLimit[busNumber].minLimitType != RL_UNLIMITED_SOURCE) {
								reactiveLimit[busNumber].minLimitType = RL_LIMITED;
								reactiveLimit[busNumber].minLimit += childData.minReactive;
							}
							else if (!childData.haveMinReactive)
								reactiveLimit[busNumber].minLimitType = RL_UNLIMITED_SOURCE;
						}
					}
				}
			}
			// Load
			for (auto itl = m_loadList.begin(); itl != m_loadList.end(); itl++) {
				Load* load = *itl;
				if (load->IsOnline()) {
					if (bus == load->GetParentList()[0]) {
						LoadElectricalData childData = load->GetPUElectricalData(systemPowerBase);
						if (childData.loadType == CONST_POWER) {
							power[busNumber] += std::complex<double>(-childData.activePower, -childData.reactivePower);
							loadPower[busNumber] += std::complex<double>(-childData.activePower, -childData.reactivePower);
						}
					}
				}
			}

			// Induction motor
			for (auto itim = m_indMotorList.begin(); itim != m_indMotorList.end(); itim++) {
				IndMotor* indMotor = *itim;
				if (indMotor->IsOnline()) {
					if (bus == indMotor->GetParentList()[0]) {
						IndMotorElectricalData childData = indMotor->GetPUElectricalData(systemPowerBase);
						double reactivePower = childData.reactivePower;

						if (childData.calcQInPowerFlow) {
							indMotor->InitPowerFlowMotor(systemPowerBase, data.number);
							if (!indMotor->CalculateReactivePower(std::abs(voltage[childData.busNum]))) {
								m_errorMsg = _("It was not possible to solve the induction motors.");
								return false;
							}
							reactivePower = indMotor->GetElectricalData().qValue;
						}

						power[busNumber] += std::complex<double>(-childData.activePower, -reactivePower);
						loadPower[busNumber] += std::complex<double>(-childData.activePower, -reactivePower);
					}
				}
			}

			// EMTElements
			for (EMTElement* emtElement : m_emtElementList) {
				if (emtElement->IsOnline() && !emtElement->GetParentList().empty()) {
					if (bus == emtElement->GetParentList()[0]) {
						EMTElementData childData = emtElement->GetEMTElementData();
						power[busNumber] -= childData.power;
						loadPower[busNumber] -= childData.power;
					}
				}
			}

			busNumber++;
		}
	}

	// Check if have slack bus and if have generation on the slack bus
	bool haveSlackBus = false;
	bool slackBusHaveGeneration = false;
	for (unsigned int i = 0; i < busType.size(); i++) {
		if (busType[i] == BUS_SLACK) {
			for (auto itsg = m_syncGeneratorList.begin(); itsg != m_syncGeneratorList.end(); itsg++) {
				SyncGenerator* syncGenerator = *itsg;
				if (syncGenerator->IsOnline() && slackBus == syncGenerator->GetParentList()[0]) slackBusHaveGeneration = true;
			}
			haveSlackBus = true;
		}
	}
	if (!haveSlackBus) {
		m_errorMsg = _("There is no slack bus on the system.");
		return false;
	}
	if (!slackBusHaveGeneration) {
		m_errorMsg = _("The slack bus don't have generation.");
		return false;
	}

	return true;
}

bool PowerFlow::RunGaussSeidel(double systemPowerBase,
	int maxIteration,
	double error,
	double initAngle,
	double accFactor)
{
	std::vector<BusType> busType;                  // Bus type
	std::vector<std::complex<double> > voltage;    // Voltage of buses
	std::vector<std::complex<double> > power;      // Injected power
	std::vector<std::complex<double> > loadPower;  // Only the load power
	std::vector<ReactiveLimits> reactiveLimit;     // Limit of reactive power on PV buses

	if (!InitPowerFlow(busType, voltage, power, loadPower, reactiveLimit, systemPowerBase, initAngle)) return false;

	// Gauss-Seidel method
	std::vector<std::complex<double> > oldVoltage;  // Old voltage array.
	oldVoltage.resize(voltage.size());

	auto oldBusType = busType;

	int iteration = 0;  // Current itaration number.
	double emtPowerError = 1e3; // EMT Elements power error.

	while (true) {
		// Reach the max number of iterations.
		if (iteration >= maxIteration) {
			m_errorMsg = _("The maximum number of iterations was reached.");
			return false;
		}

		// Calculate induction motor reactive power
		if (!CalculateMotorsReactivePower(voltage, power)) {
			m_errorMsg = _("It was not possible to solve the induction motors.");
			return false;
		}

		// Update the old voltage array to current iteration values.
		for (int i = 0; i < m_numberOfBuses; i++) oldVoltage[i] = voltage[i];

		double iterationError = GaussSeidel(busType, voltage, oldVoltage, power, accFactor);

		if (iterationError < error) {
			// Calculate power error in EMT elements.
			double oldError = emtPowerError;
			emtPowerError = CalculateEMTPowerError(voltage, power, systemPowerBase, m_errorMsg);
			//wxMessageBox(wxString::Format(wxT("It %d, EMT Power Error: %e (base = %e)"), iteration, emtPowerError, error));
			if (abs(emtPowerError - oldError) < 1e-12) { // Without change in error the convergence is impossible
				m_errorMsg = _("Impossible to reach a convergence with the current Electromagnetic Transient Elements.");
				return false;
			}

			if (!CheckReactiveLimits(busType, reactiveLimit, power, loadPower) && emtPowerError < error) break;
		}

		iteration++;
	}
	m_iterations = iteration;

	// Adjust the power array.
	for (int i = 0; i < m_numberOfBuses; i++) {
		std::complex<double> sBus = std::complex<double>(0.0, 0.0);
		for (int j = 0; j < m_numberOfBuses; j++) sBus += voltage[i] * std::conj(voltage[j]) * std::conj(m_yBus[i][j]);
		power[i] = sBus;
	}


	UpdateElementsPowerFlow(voltage, power, oldBusType, reactiveLimit, systemPowerBase);

	return true;
}

bool PowerFlow::RunNewtonRaphson(double systemPowerBase,
	int maxIteration,
	double error,
	double initAngle,
	double inertia)
{
	std::vector<BusType> busType;                  // Bus type
	std::vector<std::complex<double> > voltage;    // Voltage of buses
	std::vector<std::complex<double> > power;      // Injected power
	std::vector<std::complex<double> > loadPower;  // Only the load power
	std::vector<ReactiveLimits> reactiveLimit;     // Limit of reactive power on PV buses

	if (!InitPowerFlow(busType, voltage, power, loadPower, reactiveLimit, systemPowerBase, initAngle)) return false;
	auto oldBusType = busType;

	// Newton-Raphson method
	int numPQ = 0;  // Number of PQ buses
	int numPV = 0;  // Number of PV buses
	GetNumPVPQ(busType, numPQ, numPV);

	// DeltaP and DeltaQ array
	std::vector<double> dPdQ;
	dPdQ.resize(numPV + 2 * numPQ, 0.0);

	int iteration = 0;  // Current iteration number.
	while (true) {
		// Reach the max number of iterations.
		if (iteration >= maxIteration) {
			m_errorMsg = _("The maximum number of iterations was reached.");
			return false;
		}

		// Calculate induction motor reactive power
		if (!CalculateMotorsReactivePower(voltage, power)) {
			m_errorMsg = _("It was not possible to solve the induction motors.");
			return false;
		}

		// Calculate dPdQ array

		// Fill it with zeros
		std::fill(dPdQ.begin(), dPdQ.end(), 0.0);

		int indexDP = 0;
		int indexDQ = numPQ + numPV;
		for (int i = 0; i < m_numberOfBuses; i++) {
			if (busType[i] != BUS_SLACK) {
				for (int j = 0; j < m_numberOfBuses; j++) {
					// PV ou PQ bus
					std::complex<double> sInj = std::conj(m_yBus[i][j]) * voltage[i] * std::conj(voltage[j]);
					dPdQ[indexDP] += sInj.real();

					// PQ bus
					if (busType[i] == BUS_PQ) dPdQ[indexDQ] += sInj.imag();
				}

				// PQ or PV bus
				dPdQ[indexDP] = power[i].real() - dPdQ[indexDP];
				indexDP++;

				// PQ bus
				if (busType[i] == BUS_PQ) {
					dPdQ[indexDQ] = power[i].imag() - dPdQ[indexDQ];
					indexDQ++;
				}
			}
		}

		// Calculate the iteration error
		double iterationError = 0.0;
		for (unsigned int i = 0; i < dPdQ.size(); ++i) {
			if (iterationError < std::abs(dPdQ[i])) iterationError = std::abs(dPdQ[i]);
		}

		// Check if the iteration error is less than tolerance, also check if any reactive limit was reached.
		// If any reactive limit was reached, change the bus type.
		if (iterationError < error) {
			double emtPowerError = CalculateEMTPowerError(voltage, power, systemPowerBase, m_errorMsg);

			if (!CheckReactiveLimits(busType, reactiveLimit, power, loadPower) && emtPowerError < error)
				break;
			else {
				GetNumPVPQ(busType, numPQ, numPV);
				dPdQ.clear();
				dPdQ.resize(numPV + 2 * numPQ, 0.0);
			}
		}

		NewtonRaphson(busType, voltage, power, numPV, numPQ, dPdQ, inertia);

		iteration++;
	}
	m_iterations = iteration;

	// Adjust the power array.
	for (int i = 0; i < m_numberOfBuses; i++) {
		std::complex<double> sBus = std::complex<double>(0.0, 0.0);
		for (int j = 0; j < m_numberOfBuses; j++) sBus += voltage[i] * std::conj(voltage[j]) * std::conj(m_yBus[i][j]);
		power[i] = sBus;
	}

	UpdateElementsPowerFlow(voltage, power, oldBusType, reactiveLimit, systemPowerBase);

	return true;
}

bool PowerFlow::RunGaussNewton(double systemPowerBase,
	int maxIteration,
	double error,
	double initAngle,
	double accFactor,
	double gaussTol,
	double inertia)
{
	std::vector<BusType> busType;                  // Bus type
	std::vector<std::complex<double> > voltage;    // Voltage of buses
	std::vector<std::complex<double> > power;      // Injected power
	std::vector<std::complex<double> > loadPower;  // Only the load power
	std::vector<ReactiveLimits> reactiveLimit;     // Limit of reactive power on PV buses

	if (!InitPowerFlow(busType, voltage, power, loadPower, reactiveLimit, systemPowerBase, initAngle)) return false;

	// Gauss-Seidel method
	std::vector<std::complex<double> > oldVoltage;  // Old voltage array.
	oldVoltage.resize(voltage.size());

	auto oldBusType = busType;

	int iteration = 0;  // Current itaration number.

	while (true) {
		// Reach the max number of iterations.
		if (iteration >= maxIteration) {
			m_errorMsg = _("The maximum number of iterations was reached.");
			return false;
		}

		// Calculate induction motor reactive power
		if (!CalculateMotorsReactivePower(voltage, power)) {
			m_errorMsg = _("It was not possible to solve the induction motors.");
			return false;
		}

		// Update the old voltage array to current iteration values.
		for (int i = 0; i < m_numberOfBuses; i++) oldVoltage[i] = voltage[i];

		double iterationError = GaussSeidel(busType, voltage, oldVoltage, power, accFactor);

		if (iterationError < gaussTol) break;

		iteration++;
	}

	// Newton-Raphson method
	int numPQ = 0;  // Number of PQ buses
	int numPV = 0;  // Number of PV buses
	GetNumPVPQ(busType, numPQ, numPV);

	// DeltaP and DeltaQ array
	std::vector<double> dPdQ;
	dPdQ.resize(numPV + 2 * numPQ, 0.0);

	while (true) {
		// Reach the max number of iterations.
		if (iteration >= maxIteration) {
			m_errorMsg = _("The maximum number of iterations was reached.");
			return false;
		}

		// Calculate induction motor reactive power
		if (!CalculateMotorsReactivePower(voltage, power)) {
			m_errorMsg = _("It was not possible to solve the induction motors.");
			return false;
		}

		// Calculate dPdQ array

		// Fill it with zeros
		std::fill(dPdQ.begin(), dPdQ.end(), 0.0);

		int indexDP = 0;
		int indexDQ = numPQ + numPV;
		for (int i = 0; i < m_numberOfBuses; i++) {
			if (busType[i] != BUS_SLACK) {
				for (int j = 0; j < m_numberOfBuses; j++) {
					// PV ou PQ bus
					std::complex<double> sInj = std::conj(m_yBus[i][j]) * voltage[i] * std::conj(voltage[j]);
					dPdQ[indexDP] += sInj.real();

					// PQ bus
					if (busType[i] == BUS_PQ) dPdQ[indexDQ] += sInj.imag();
				}

				// PQ or PV bus
				dPdQ[indexDP] = power[i].real() - dPdQ[indexDP];
				indexDP++;

				// PQ bus
				if (busType[i] == BUS_PQ) {
					dPdQ[indexDQ] = power[i].imag() - dPdQ[indexDQ];
					indexDQ++;
				}
			}
		}

		// Calculate the iteration error
		double iterationError = 0.0;
		for (unsigned int i = 0; i < dPdQ.size(); ++i) {
			if (iterationError < std::abs(dPdQ[i])) iterationError = std::abs(dPdQ[i]);
		}

		// Check if the iteration error is less than tolerance, also check if any reactive limit was reached.
		// If any reactive limit was reached, change the bus type.
		if (iterationError < error) {
			double emtPowerError = CalculateEMTPowerError(voltage, power, systemPowerBase, m_errorMsg);

			if (!CheckReactiveLimits(busType, reactiveLimit, power, loadPower) && emtPowerError < error)
				break;
			else {
				GetNumPVPQ(busType, numPQ, numPV);
				dPdQ.clear();
				dPdQ.resize(numPV + 2 * numPQ, 0.0);
			}
		}

		NewtonRaphson(busType, voltage, power, numPV, numPQ, dPdQ, inertia);

		iteration++;
	}
	m_iterations = iteration;

	// Adjust the power array.
	for (int i = 0; i < m_numberOfBuses; i++) {
		std::complex<double> sBus = std::complex<double>(0.0, 0.0);
		for (int j = 0; j < m_numberOfBuses; j++) sBus += voltage[i] * std::conj(voltage[j]) * std::conj(m_yBus[i][j]);
		power[i] = sBus;
	}

	UpdateElementsPowerFlow(voltage, power, oldBusType, reactiveLimit, systemPowerBase);

	return true;
}

void PowerFlow::ResetVoltages()
{
	for (auto* bus : m_busList) {
		BusElectricalData data = bus->GetElectricalData();
		data.voltage = std::complex<double>(1.0, 0.0);
		data.harmonicOrder.clear();
		data.harmonicVoltage.clear();
		data.thd = 0.0;
		bus->SetElectricalData(data);
	}
}

void PowerFlow::GetNumPVPQ(std::vector<BusType> busType, int& numPQ, int& numPV)
{
	numPQ = 0;
	numPV = 0;
	for (auto it = busType.begin(), itEnd = busType.end(); it != itEnd; ++it) {
		if (*it == BUS_PQ)
			numPQ++;
		else if (*it == BUS_PV)
			numPV++;
	}
}

double PowerFlow::GaussSeidel(std::vector<BusType> busType,
	std::vector<std::complex<double> >& voltage,
	std::vector<std::complex<double> > oldVoltage,
	std::vector<std::complex<double> >& power,
	double accFactor)
{
	double error = 0.0;

	for (int i = 0; i < m_numberOfBuses; i++) {
		if (busType[i] == BUS_PQ) {
			std::complex<double> yeSum(0.0, 0.0);
			for (int k = 0; k < m_numberOfBuses; k++) {
				if (i != k) {
					// Sum { Y[i,k] * E[k] } | k = 1->n; k diff i
					yeSum += m_yBus[i][k] * voltage[k];
				}
			}

			// E[i] = (1/Y[i,i])*((P[i]-jQ[i])/E*[i] - Sum { Y[i,k] * E[k] (k diff i) })
			std::complex<double> newVolt = (1.0 / m_yBus[i][i]) * (std::conj(power[i]) / std::conj(voltage[i]) - yeSum);

			// Apply the acceleration factor.
			newVolt = std::complex<double>(accFactor * (newVolt.real() - voltage[i].real()) + voltage[i].real(),
				accFactor * (newVolt.imag() - voltage[i].imag()) + voltage[i].imag());

			voltage[i] = newVolt;
		}
		if (busType[i] == BUS_PV) {
			std::complex<double> yeSum(0.0, 0.0);
			for (int k = 0; k < m_numberOfBuses; k++) {
				if (i != k) {
					// Sum { Y[i,k] * E[k] } | k = 1->n; k diff i
					yeSum += m_yBus[i][k] * voltage[k];
				}
			}
			std::complex<double> yeSumT = yeSum + (m_yBus[i][i] * voltage[i]);

			// Q[i] = - Im( E*[i] * Sum { Y[i,k] * E[k] } )
			std::complex<double> qCalc = std::conj(voltage[i]) * yeSumT;
			power[i] = std::complex<double>(power[i].real(), -qCalc.imag());

			// E[i] = (1/Y[i,i])*((P[i]-jQ[i])/E*[i] - Sum { Y[i,k] * E[k] (k diff i) })
			std::complex<double> newVolt = (1.0 / m_yBus[i][i]) * (std::conj(power[i]) / std::conj(voltage[i]) - yeSum);

			// Apply the acceleration factor.
			newVolt = std::complex<double>(accFactor * (newVolt.real() - voltage[i].real()) + voltage[i].real(),
				accFactor * (newVolt.imag() - voltage[i].imag()) + voltage[i].imag());

			// Keep the same voltage magnitude.
			voltage[i] = std::complex<double>(std::abs(voltage[i]) * std::cos(std::arg(newVolt)),
				std::abs(voltage[i]) * std::sin(std::arg(newVolt)));
		}

		double busError = std::max(std::abs(voltage[i].real() - oldVoltage[i].real()),
			std::abs(voltage[i].imag() - oldVoltage[i].imag()));

		if (busError > error) error = busError;
	}
	return error;
}

void PowerFlow::NewtonRaphson(std::vector<BusType> busType,
	std::vector<std::complex<double> >& voltage,
	std::vector<std::complex<double> > power,
	int numPV,
	int numPQ,
	std::vector<double> dPdQ,
	double inertia)
{
	// Jacobian matrix
	std::vector<std::vector<double> > jacobMatrix = CalculateJacobianMatrix(voltage, power, busType, numPV, numPQ);

	// Apply inertia
	for (unsigned int i = 0; i < dPdQ.size(); ++i) { dPdQ[i] = inertia * dPdQ[i]; }

	// Calculate DeltaTheta DeltaV array
	std::vector<double> dTdV = GaussianElimination(jacobMatrix, dPdQ);

	// Update voltage array
	int indexDT = 0;
	int indexDV = numPQ + numPV;
	for (int i = 0; i < m_numberOfBuses; i++) {
		if (busType[i] != BUS_SLACK) {
			if (busType[i] == BUS_PV) {
				double newV = std::abs(voltage[i]);
				double newT = std::arg(voltage[i]) + dTdV[indexDT];
				voltage[i] = std::complex<double>(newV * std::cos(newT), newV * std::sin(newT));
				indexDT++;
			}
			else {
				double newV = std::abs(voltage[i]) * (1.0 + dTdV[indexDV]);
				double newT = std::arg(voltage[i]) + dTdV[indexDT];
				voltage[i] = std::complex<double>(newV * std::cos(newT), newV * std::sin(newT));
				indexDV++;
				indexDT++;
			}
		}
	}
}

std::vector<std::vector<double> > PowerFlow::CalculateJacobianMatrix(std::vector<std::complex<double> > voltage,
	std::vector<std::complex<double> > power,
	std::vector<BusType> busType,
	int numPV,
	int numPQ)
{
	// Jacobian matrix
	std::vector<std::vector<double> > jacobMatrix;
	jacobMatrix.resize(2 * numPQ + numPV);  // Jabobian matrix size
	for (int i = 0; i < (2 * numPQ + numPV); i++) jacobMatrix[i].resize(2 * numPQ + numPV);

	// Calculate the submatrices
	//{ J11
	std::vector<std::vector<double> > j11;
	j11.resize(numPQ + numPV);  // submatrix j11 size
	for (int i = 0; i < (numPQ + numPV); i++) j11[i].resize(numPQ + numPV);

	int busNumI = 0;
	int busNumJ = 0;
	for (int i = 0; i < m_numberOfBuses; i++) {
		if (busType[i] != BUS_SLACK) {
			for (int j = 0; j < m_numberOfBuses; j++) {
				if (busType[j] != BUS_SLACK) {
					if (busNumI != busNumJ) {
						double vi = std::abs(voltage[i]);
						double ti = std::arg(voltage[i]);
						double vj = std::abs(voltage[j]);
						double tj = std::arg(voltage[j]);
						double yij = std::abs(m_yBus[i][j]);
						double tij = std::arg(m_yBus[i][j]);
#ifdef STD_NR
						j11[busNumI][busNumJ] = -vi * vj * yij * std::sin(tij + tj - ti);
#else
						double gij = m_yBus[i][j].real();
						double bij = m_yBus[i][j].imag();
						j11[busNumI][busNumJ] = vi * vj * (gij * std::sin(ti - tj) - bij * std::cos(ti - tj));
#endif
					}
					else {  // busNumI == busNumJ
						std::complex<double> sj = std::complex<double>(0.0, 0.0);
						for (int k = 0; k < m_numberOfBuses; k++)
							sj += voltage[j] * std::conj(m_yBus[j][k]) * std::conj(voltage[k]);
#ifdef STD_NR
						j11[busNumI][busNumJ] = -sj.imag() - m_yBus[j][j].imag() * std::pow(std::abs(voltage[j]), 2.0);
#else
						j11[busNumI][busNumJ] = -power[i].imag() - m_yBus[j][j].imag() * std::pow(std::abs(voltage[j]), 2.0);
#endif
					}
					busNumJ++;
				}
			}
			busNumI++;
			busNumJ = 0;
		}
	}
	//}
	//{ J12
	std::vector<std::vector<double> > j12;
	j12.resize(numPQ + numPV);  // submatrix j12 size
	for (int i = 0; i < (numPQ + numPV); i++) j12[i].resize(numPQ);

	busNumI = 0;
	busNumJ = 0;
	for (int i = 0; i < m_numberOfBuses; i++) {
		if (busType[i] != BUS_SLACK) {
			for (int j = 0; j < m_numberOfBuses; j++) {
				if (busType[j] == BUS_PQ)  // Only PQ buses
				{
					if (busNumI != busNumJ) {
						double vi = std::abs(voltage[i]);
						double ti = std::arg(voltage[i]);
						double vj = std::abs(voltage[j]);
						double tj = std::arg(voltage[j]);
						double yij = std::abs(m_yBus[i][j]);
						double tij = std::arg(m_yBus[i][j]);
#ifdef STD_NR
						j12[busNumI][busNumJ] = vj * vi * yij * std::cos(tij + tj - ti);
#else
						double gij = m_yBus[i][j].real();
						double bij = m_yBus[i][j].imag();
						j12[busNumI][busNumJ] = vi * (gij * std::cos(ti - tj) - bij * std::sin(ti - tj));
#endif
					}
					else {  // busNumI == busNumJ
						std::complex<double> sj = std::complex<double>(0.0, 0.0);
						for (int k = 0; k < m_numberOfBuses; k++)
							sj += voltage[j] * std::conj(m_yBus[j][k]) * std::conj(voltage[k]);
#ifdef STD_NR
						j12[busNumI][busNumJ] = sj.real() + m_yBus[j][j].real() * std::pow(std::abs(voltage[j]), 2.0);
#else
						j12[busNumI][busNumJ] = (power[i].real() + m_yBus[j][j].real() * std::pow(std::abs(voltage[j]), 2.0)) / std::abs(voltage[j]);
#endif
					}
					busNumJ++;
				}
			}
			busNumI++;
			busNumJ = 0;
		}
	}
	//}
	//{ J21
	std::vector<std::vector<double> > j21;
	j21.resize(numPQ);  // submatrix j21 size
	for (int i = 0; i < (numPQ); i++) j21[i].resize(numPQ + numPV);

	busNumI = 0;
	busNumJ = 0;
	for (int i = 0; i < m_numberOfBuses; i++) {
		if (busType[i] == BUS_PQ)  // Only PQ
		{
			for (int j = 0; j < m_numberOfBuses; j++) {
				if (busType[j] == BUS_SLACK) {
					if (busNumI != busNumJ) {
						double vi = std::abs(voltage[i]);
						double ti = std::arg(voltage[i]);
						double vj = std::abs(voltage[j]);
						double tj = std::arg(voltage[j]);
						double yij = std::abs(m_yBus[i][j]);
						double tij = std::arg(m_yBus[i][j]);

#ifdef STD_NR
						j21[busNumI][busNumJ] = -vi * vj * yij * std::cos(tij + tj - ti);
#else
						double gij = m_yBus[i][j].real();
						double bij = m_yBus[i][j].imag();
						j21[busNumI][busNumJ] = -vi * vj * (gij * std::cos(ti - tj) + bij * std::sin(ti - tj));
#endif
					}
					else {  // busNumI == busNumJ
						std::complex<double> sj = std::complex<double>(0.0, 0.0);
						for (int k = 0; k < m_numberOfBuses; k++)
							sj += voltage[j] * std::conj(m_yBus[j][k]) * std::conj(voltage[k]);
#ifdef STD_NR
						j21[busNumI][busNumJ] = sj.real() - m_yBus[j][j].real() * std::pow(std::abs(voltage[j]), 2.0);
#else
						j21[busNumI][busNumJ] = power[i].real() - m_yBus[j][j].real() * std::pow(std::abs(voltage[j]), 2.0);
#endif
					}
					busNumJ++;
				}
			}
			busNumI++;
			busNumJ = 0;
		}
	}
	//}
	//{ J22
	std::vector<std::vector<double> > j22;
	j22.resize(numPQ);  // submatrix j22 size
	for (int i = 0; i < (numPQ); i++) j22[i].resize(numPQ);

	busNumI = 0;
	busNumJ = 0;
	for (int i = 0; i < m_numberOfBuses; i++) {
		if (busType[i] == BUS_PQ) {
			for (int j = 0; j < m_numberOfBuses; j++) {
				if (busType[j] == BUS_PQ) {
					if (busNumI != busNumJ) {
						double vi = std::abs(voltage[i]);
						double ti = std::arg(voltage[i]);
						double vj = std::abs(voltage[j]);
						double tj = std::arg(voltage[j]);
						double yij = std::abs(m_yBus[i][j]);
						double tij = std::arg(m_yBus[i][j]);

#ifdef STD_NR
						j22[busNumI][busNumJ] = -vj * vi * yij * std::sin(tij + tj - ti);
#else
						double gij = m_yBus[i][j].real();
						double bij = m_yBus[i][j].imag();
						j22[busNumI][busNumJ] = vi * (gij * std::sin(ti - tj) - bij * std::cos(ti - tj));
#endif
					}
					else {  // busNumI == busNumJ
						std::complex<double> sj = std::complex<double>(0.0, 0.0);
						for (int k = 0; k < m_numberOfBuses; k++)
							sj += voltage[j] * std::conj(m_yBus[j][k]) * std::conj(voltage[k]);
#ifdef STD_NR
						j22[busNumI][busNumJ] = sj.imag() - m_yBus[j][j].imag() * std::pow(std::abs(voltage[j]), 2.0);
#else
						j22[busNumI][busNumJ] = (power[i].imag() - m_yBus[j][j].imag() * std::pow(std::abs(voltage[j]), 2.0)) / std::abs(voltage[j]);
#endif
					}
					busNumJ++;
				}
			}
			busNumI++;
			busNumJ = 0;
		}
	}
	//}

	// Fill Jacobian matrix
	for (int i = 0; i < (2 * numPQ + numPV); i++) {
		for (int j = 0; j < (2 * numPQ + numPV); j++) {
			if (i < (numPQ + numPV)) {
				if (j < (numPQ + numPV))
					jacobMatrix[i][j] = j11[i][j];
				else  // j >= numPQ + numPV
					jacobMatrix[i][j] = j12[i][j - (numPQ + numPV)];
			}
			else {  // i >= numPQ + numPV
				if (j < (numPQ + numPV))
					jacobMatrix[i][j] = j21[i - (numPQ + numPV)][j];
				else  // j >= numPQ + numPV
					jacobMatrix[i][j] = j22[i - (numPQ + numPV)][j - (numPQ + numPV)];
			}
		}
	}
	return jacobMatrix;
}

bool PowerFlow::CheckReactiveLimits(std::vector<BusType>& busType,
	std::vector<ReactiveLimits>& reactiveLimit,
	std::vector<std::complex<double> > power,
	std::vector<std::complex<double> > loadPower)
{
	bool limitReach = false;
	for (int i = 0; i < m_numberOfBuses; i++) {
		if (busType[i] == BUS_PV) {
			if (reactiveLimit[i].maxLimitType == RL_LIMITED) {
				if (power[i].imag() - loadPower[i].imag() > reactiveLimit[i].maxLimit) {
					power[i] = std::complex<double>(power[i].real(), reactiveLimit[i].maxLimit + loadPower[i].imag());
					busType[i] = BUS_PQ;
					reactiveLimit[i].limitReached = RL_MAX_REACHED;
					limitReach = true;
				}
			}
			if (reactiveLimit[i].minLimitType == RL_LIMITED) {
				if (power[i].imag() - loadPower[i].imag() < reactiveLimit[i].minLimit) {
					power[i] = std::complex<double>(power[i].real(), reactiveLimit[i].minLimit + loadPower[i].imag());
					busType[i] = BUS_PQ;
					reactiveLimit[i].limitReached = RL_MIN_REACHED;
					limitReach = true;
				}
			}
		}
	}
	return limitReach;
}

bool PowerFlow::CalculateMotorsReactivePower(std::vector<std::complex<double> > voltage,
	std::vector<std::complex<double> >& power)
{
	for (unsigned int i = 0; i < m_indMotorList.size(); ++i) {
		IndMotor* motor = m_indMotorList[i];
		auto data = motor->GetElectricalData();
		if (motor->IsOnline() && data.calcQInPowerFlow) {
			double oldQ = data.qValue;
			if (!motor->CalculateReactivePower(std::abs(voltage[data.busNum]))) return false;
			double dQ = oldQ - motor->GetElectricalData().qValue;
			power[data.busNum] += std::complex<double>(0.0, dQ);
		}
	}
	return true;
}
