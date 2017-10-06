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
bool PowerFlow::RunGaussSeidel(double systemPowerBase,
                               int maxIteration,
                               double error,
                               double initAngle,
                               double accFactor)
{
    // Calculate the Ybus.
    if(!GetYBus(m_yBus, systemPowerBase)) {
        m_errorMsg = _("No buses found on the system.");
        return false;
    }

    // Number of buses on the system.
    int numberOfBuses = static_cast<int>(m_busList.size());

    std::vector<BusType> busType;                  // Bus type
    std::vector<std::complex<double> > voltage;    // Voltage of buses
    std::vector<std::complex<double> > power;      // Injected power
    std::vector<std::complex<double> > loadPower;  // Only the load power
    std::vector<ReactiveLimits> reactiveLimit;     // Limit of reactive power on PV buses

    reactiveLimit.resize(numberOfBuses);

    int busNumber = 0;
    for(auto itb = m_busList.begin(); itb != m_busList.end(); itb++) {
        Bus* bus = *itb;
        BusElectricalData data = bus->GetElectricalData();

        // Fill the bus type
        if(data.slackBus) busType.push_back(BUS_SLACK);
        // If the bus have controlled voltage, check if at least one synchronous machine is connected, then set the
        // bus type.
        else if(data.isVoltageControlled) {
            bool hasSyncMachine = false;
            // Synchronous generator
            for(auto itsg = m_syncGeneratorList.begin(); itsg != m_syncGeneratorList.end(); itsg++) {
                SyncGenerator* syncGenerator = *itsg;
                if(bus == syncGenerator->GetParentList()[0] && syncGenerator->IsOnline()) hasSyncMachine = true;
            }
            // Synchronous motor
            for(auto itsm = m_syncMotorList.begin(); itsm != m_syncMotorList.end(); itsm++) {
                SyncMotor* syncMotor = *itsm;
                if(bus == syncMotor->GetParentList()[0] && syncMotor->IsOnline()) hasSyncMachine = true;
            }
            if(hasSyncMachine)
                busType.push_back(BUS_PV);
            else
                busType.push_back(BUS_PQ);
        } else
            busType.push_back(BUS_PQ);

        // Fill the voltages array
        if(data.isVoltageControlled && busType[busNumber] != BUS_PQ) {
            voltage.push_back(std::complex<double>(data.controlledVoltage, 0.0));
        } else {
            voltage.push_back(std::complex<double>(1.0, 0.0));
        }

        // Fill the power array
        power.push_back(std::complex<double>(0.0, 0.0));  // Initial value
        loadPower.push_back(std::complex<double>(0.0, 0.0));

        // Synchronous generator
        for(auto itsg = m_syncGeneratorList.begin(); itsg != m_syncGeneratorList.end(); itsg++) {
            SyncGenerator* syncGenerator = *itsg;
            if(syncGenerator->IsOnline()) {
                if(bus == syncGenerator->GetParentList()[0]) {
                    SyncGeneratorElectricalData childData = syncGenerator->GetPUElectricalData(systemPowerBase);
                    power[busNumber] += std::complex<double>(childData.activePower, childData.reactivePower);

                    if(busType[busNumber] == BUS_PV) {
                        if(childData.haveMaxReactive && reactiveLimit[busNumber].maxLimitType != RL_UNLIMITED_SOURCE) {
                            reactiveLimit[busNumber].maxLimitType = RL_LIMITED;
                            reactiveLimit[busNumber].maxLimit += childData.maxReactive;
                        } else if(!childData.haveMaxReactive)
                            reactiveLimit[busNumber].maxLimitType = RL_UNLIMITED_SOURCE;

                        if(childData.haveMinReactive && reactiveLimit[busNumber].minLimitType != RL_UNLIMITED_SOURCE) {
                            reactiveLimit[busNumber].minLimitType = RL_LIMITED;
                            reactiveLimit[busNumber].minLimit += childData.minReactive;
                        } else if(!childData.haveMinReactive)
                            reactiveLimit[busNumber].minLimitType = RL_UNLIMITED_SOURCE;
                    }
                }
            }
        }
        // Synchronous motor
        for(auto itsm = m_syncMotorList.begin(); itsm != m_syncMotorList.end(); itsm++) {
            SyncMotor* syncMotor = *itsm;
            if(syncMotor->IsOnline()) {
                if(bus == syncMotor->GetParentList()[0]) {
                    SyncMotorElectricalData childData = syncMotor->GetPUElectricalData(systemPowerBase);
                    power[busNumber] += std::complex<double>(-childData.activePower, childData.reactivePower);
                    loadPower[busNumber] += std::complex<double>(-childData.activePower, 0.0);

                    if(busType[busNumber] == BUS_PV) {
                        if(childData.haveMaxReactive && reactiveLimit[busNumber].maxLimitType != RL_UNLIMITED_SOURCE) {
                            reactiveLimit[busNumber].maxLimitType = RL_LIMITED;
                            reactiveLimit[busNumber].maxLimit += childData.maxReactive;
                        } else if(!childData.haveMaxReactive)
                            reactiveLimit[busNumber].maxLimitType = RL_UNLIMITED_SOURCE;

                        if(childData.haveMinReactive && reactiveLimit[busNumber].minLimitType != RL_UNLIMITED_SOURCE) {
                            reactiveLimit[busNumber].minLimitType = RL_LIMITED;
                            reactiveLimit[busNumber].minLimit += childData.minReactive;
                        } else if(!childData.haveMinReactive)
                            reactiveLimit[busNumber].minLimitType = RL_UNLIMITED_SOURCE;
                    }
                }
            }
        }
        // Load
        for(auto itl = m_loadList.begin(); itl != m_loadList.end(); itl++) {
            Load* load = *itl;
            if(load->IsOnline()) {
                if(bus == load->GetParentList()[0]) {
                    LoadElectricalData childData = load->GetPUElectricalData(systemPowerBase);
                    if(childData.loadType == CONST_POWER) {
                        power[busNumber] += std::complex<double>(-childData.activePower, -childData.reactivePower);
                        loadPower[busNumber] += std::complex<double>(-childData.activePower, -childData.reactivePower);
                    }
                }
            }
        }

        // Induction motor
        for(auto itim = m_indMotorList.begin(); itim != m_indMotorList.end(); itim++) {
            IndMotor* indMotor = *itim;
            if(indMotor->IsOnline()) {
                if(bus == indMotor->GetParentList()[0]) {
                    IndMotorElectricalData childData = indMotor->GetPUElectricalData(systemPowerBase);
                    power[busNumber] += std::complex<double>(-childData.activePower, -childData.reactivePower);
                    loadPower[busNumber] += std::complex<double>(-childData.activePower, -childData.reactivePower);
                }
            }
        }

        busNumber++;
    }

    // Check if have slack bus and if have generation on the slack bus
    bool haveSlackBus = false;
    bool slackBusHaveGeneration = false;
    for(int i = 0; i < (int)busType.size(); i++) {
        if(busType[i] == BUS_SLACK) {
            auto itb = m_busList.begin();
            std::advance(itb, i);
            Bus* bus = *itb;

            for(auto itsg = m_syncGeneratorList.begin(); itsg != m_syncGeneratorList.end(); itsg++) {
                SyncGenerator* syncGenerator = *itsg;
                if(syncGenerator->IsOnline() && bus == syncGenerator->GetParentList()[0]) slackBusHaveGeneration = true;
            }
            haveSlackBus = true;
        }
    }
    if(!haveSlackBus) {
        m_errorMsg = _("There is no slack bus on the system.");
        return false;
    }
    if(!slackBusHaveGeneration) {
        m_errorMsg = _("The slack bus don't have generation.");
        return false;
    }

    // Gauss-Seidel method
    std::vector<std::complex<double> > oldVoltage;  // Old voltage array.
    oldVoltage.resize(voltage.size());

    auto oldBusType = busType;

    int iteration = 0;  // Current itaration number.

    while(true) {
        // Reach the max number of iterations.
        if(iteration >= maxIteration) {
            m_errorMsg = _("The maximum number of iterations was reached.");
            return false;
        }

        // Update the old voltage array to current iteration values.
        for(int i = 0; i < numberOfBuses; i++) oldVoltage[i] = voltage[i];

        double iterationError = 0.0;

        for(int i = 0; i < numberOfBuses; i++) {
            if(busType[i] == BUS_PQ) {
                std::complex<double> yeSum(0.0, 0.0);
                for(int k = 0; k < numberOfBuses; k++) {
                    if(i != k) {
                        // Sum { Y[i,k] * E[k] } | k = 1->n; k diff i
                        yeSum += m_yBus[i][k] * voltage[k];
                    }
                }

                // E[i] = (1/Y[i,i])*((P[i]-jQ[i])/E*[i] - Sum { Y[i,k] * E[k] (k diff i) })
                std::complex<double> newVolt =
                    (1.0 / m_yBus[i][i]) * (std::conj(power[i]) / std::conj(voltage[i]) - yeSum);

                // Apply the acceleration factor.
                newVolt = std::complex<double>(accFactor * (newVolt.real() - voltage[i].real()) + voltage[i].real(),
                                               accFactor * (newVolt.imag() - voltage[i].imag()) + voltage[i].imag());

                voltage[i] = newVolt;
            }
            if(busType[i] == BUS_PV) {
                std::complex<double> yeSum(0.0, 0.0);
                for(int k = 0; k < numberOfBuses; k++) {
                    if(i != k) {
                        // Sum { Y[i,k] * E[k] } | k = 1->n; k diff i
                        yeSum += m_yBus[i][k] * voltage[k];
                    }
                }
                std::complex<double> yeSumT = yeSum + (m_yBus[i][i] * voltage[i]);

                // Q[i] = - Im( E*[i] * Sum { Y[i,k] * E[k] } )
                std::complex<double> qCalc = std::conj(voltage[i]) * yeSumT;
                power[i] = std::complex<double>(power[i].real(), -qCalc.imag());

                // E[i] = (1/Y[i,i])*((P[i]-jQ[i])/E*[i] - Sum { Y[i,k] * E[k] (k diff i) })
                std::complex<double> newVolt =
                    (1.0 / m_yBus[i][i]) * (std::conj(power[i]) / std::conj(voltage[i]) - yeSum);

                // Apply the acceleration factor.
                newVolt = std::complex<double>(accFactor * (newVolt.real() - voltage[i].real()) + voltage[i].real(),
                                               accFactor * (newVolt.imag() - voltage[i].imag()) + voltage[i].imag());

                // Keep the same voltage magnitude.
                voltage[i] = std::complex<double>(std::abs(voltage[i]) * std::cos(std::arg(newVolt)),
                                                  std::abs(voltage[i]) * std::sin(std::arg(newVolt)));
            }

            double busError = std::max(std::abs(voltage[i].real() - oldVoltage[i].real()),
                                       std::abs(voltage[i].imag() - oldVoltage[i].imag()));

            if(busError > iterationError) iterationError = busError;
        }

        if(iterationError < error) {
            bool limitReach = false;
            for(int i = 0; i < numberOfBuses; i++) {
                if(busType[i] == BUS_PV) {
                    if(reactiveLimit[i].maxLimitType == RL_LIMITED) {
                        if(power[i].imag() - loadPower[i].imag() > reactiveLimit[i].maxLimit) {
                            power[i] =
                                std::complex<double>(power[i].real(), reactiveLimit[i].maxLimit + loadPower[i].imag());
                            busType[i] = BUS_PQ;
                            reactiveLimit[i].limitReached = RL_MAX_REACHED;
                            limitReach = true;
                        }
                    }
                    if(reactiveLimit[i].minLimitType == RL_LIMITED) {
                        if(power[i].imag() - loadPower[i].imag() < reactiveLimit[i].minLimit) {
                            power[i] =
                                std::complex<double>(power[i].real(), reactiveLimit[i].minLimit + loadPower[i].imag());
                            busType[i] = BUS_PQ;
                            reactiveLimit[i].limitReached = RL_MIN_REACHED;
                            limitReach = true;
                        }
                    }
                }
            }
            if(!limitReach) break;
        }

        iteration++;
    }

    // Adjust the power array.
    // TODO: Only the slack bus??
    for(int i = 0; i < numberOfBuses; i++) {
        std::complex<double> sBus = std::complex<double>(0.0, 0.0);
        for(int j = 0; j < numberOfBuses; j++) sBus += voltage[i] * std::conj(voltage[j]) * std::conj(m_yBus[i][j]);
        power[i] = sBus;
    }

    UpdateElementsPowerFlow(voltage, power, oldBusType, reactiveLimit, systemPowerBase);

    return true;
}
