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

#include "ControlElementSolver.h"
#include "Electromechanical.h"

Electromechanical::Electromechanical(wxWindow* parent, std::vector<Element*> elementList, SimulationData data)
{
    m_parent = parent;
    GetElementsFromList(elementList);
    SetEventTimeList();

    Bus dummyBus;
    m_powerSystemBase = dummyBus.GetValueFromUnit(data.basePower, data.basePowerUnit);
    m_systemFreq = data.stabilityFrequency;
    m_simTime = data.stabilitySimulationTime;
    m_timeStep = data.timeStep;
    m_tolerance = data.stabilityTolerance;
    m_maxIterations = data.stabilityMaxIterations;

    m_ctrlTimeStepMultiplier = 1.0 / static_cast<double>(data.controlTimeStepRatio);

    m_plotTime = data.plotTime;
    m_useCOI = data.useCOI;
    // If the user use all load as ZIP, updates the portions of each model, otherwise use constant impedance only.
    for(auto it = m_loadList.begin(), itEnd = m_loadList.end(); it != itEnd; ++it) {
        Load* load = *it;
        auto loadData = load->GetElectricalData();
        if(!loadData.useCompLoad) {  // If no individual load composition defined.
            if(data.useCompLoads) {  // Use general composition, if defined.
                loadData.constImpedanceActive = data.constImpedanceActive;
                loadData.constCurrentActive = data.constCurrentActive;
                loadData.constPowerActive = data.constPowerActive;
                loadData.constImpedanceReactive = data.constImpedanceReactive;
                loadData.constCurrentReactive = data.constCurrentReactive;
                loadData.constPowerReactive = data.constPowerReactive;
            } else {  // Otherwise, use constant impedance.
                loadData.constImpedanceActive = 100.0;
                loadData.constCurrentActive = 0.0;
                loadData.constPowerActive = 0.0;
                loadData.constImpedanceReactive = 100.0;
                loadData.constCurrentReactive = 0.0;
                loadData.constPowerReactive = 0.0;
            }
        }

        loadData.constCurrentUV = data.underVoltageConstCurrent / 100.0;
        loadData.constPowerUV = data.underVoltageConstPower / 100.0;
        load->SetElectricalData(loadData);
    }
}

Electromechanical::~Electromechanical() {}
bool Electromechanical::RunStabilityCalculation()
{
    wxProgressDialog pbd(_("Running simulation"), _("Initializing..."), 100, m_parent,
                         wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_CAN_ABORT | wxPD_SMOOTH);

    SetSyncMachinesModel();

    // Calculate the admittance matrix with the synchronous machines.
    if(!GetYBus(m_yBus, m_powerSystemBase, POSITIVE_SEQ, false, true, true)) {
        m_errorMsg = _("It was not possible to build the admittance matrix.");
        return false;
    }
    InsertSyncMachinesOnYBus();
    if(!InsertIndMachinesOnYBus()) return false;
    GetLUDecomposition(m_yBus, m_yBusL, m_yBusU);

    // Get buses voltages.
    m_vBus.clear();
    m_vBus.shrink_to_fit();
    m_vBus.resize(m_busList.size());
    for(auto it = m_busList.begin(), itEnd = m_busList.end(); it != itEnd; ++it) {
        Bus* bus = *it;
        auto data = bus->GetElectricalData();
        m_vBus[data.number] = data.voltage;
    }

    // Calculate injected currents
    m_iBus = ComplexMatrixTimesVector(m_yBus, m_vBus);
    for(unsigned int i = 0; i < m_iBus.size(); ++i) {
        if(std::abs(m_iBus[i]) < 1e-5) m_iBus[i] = std::complex<double>(0.0, 0.0);
    }

    if(!InitializeDynamicElements()) return false;
    PreallocateVectors();  // Reserve the vectors' memory with a estimated size, this can optimize the simulation.

    double pbdTime = m_plotTime;
    m_currentTime = 0.0;
    double currentPlotTime = 0.0;
    double currentPbdTime = 0.0;
    while(m_currentTime < m_simTime) {
        if(HasEvent(m_currentTime)) {
            SetEvent(m_currentTime);
            GetLUDecomposition(m_yBus, m_yBusL, m_yBusU);
        }

        if(currentPlotTime >= m_plotTime || m_currentTime == 0.0) {
            m_timeVector.emplace_back(m_currentTime);
            SaveData();
            currentPlotTime = 0.0;
        }

        if(currentPbdTime > pbdTime) {
            if(!pbd.Update((m_currentTime / m_simTime) * 100, wxString::Format("Time = %.2fs", m_currentTime))) {
                m_errorMsg = wxString::Format(_("Simulation cancelled at %.2fs."), m_currentTime);
                pbd.Update(100);
                return false;
            }
            currentPbdTime = 0.0;
        }

        if(!SolveMachines()) return false;

        m_currentTime += m_timeStep;
        currentPlotTime += m_timeStep;
        currentPbdTime += m_timeStep;
    }
    return true;
}

void Electromechanical::SetEventTimeList()
{
    // Fault
    for(auto it = m_busList.begin(), itEnd = m_busList.end(); it != itEnd; ++it) {
        Bus* bus = *it;
        auto data = bus->GetElectricalData();
        if(data.stabHasFault) {
            m_eventTimeList.emplace_back(data.stabFaultTime);
            m_eventOccurrenceList.push_back(false);
            m_eventTimeList.emplace_back(data.stabFaultTime + data.stabFaultLength);
            m_eventOccurrenceList.push_back(false);
        }
    }
    // Switching
    for(auto it = m_powerElementList.begin(), itEnd = m_powerElementList.end(); it != itEnd; ++it) {
        PowerElement* element = *it;
        SwitchingData swData = element->GetSwitchingData();
        for(unsigned int i = 0; i < swData.swTime.size(); ++i) {
            m_eventTimeList.emplace_back(swData.swTime[i]);
            m_eventOccurrenceList.push_back(false);
        }
    }
}

bool Electromechanical::HasEvent(double currentTime)
{
    for(unsigned int i = 0; i < m_eventTimeList.size(); ++i) {
        if(!m_eventOccurrenceList[i]) {
            if(EventTrigger(m_eventTimeList[i], currentTime)) {
                m_eventOccurrenceList[i] = true;
                return true;
            }
        }
    }
    return false;
}

void Electromechanical::SetEvent(double currentTime)
{
    // Fault
    for(auto it = m_busList.begin(), itEnd = m_busList.end(); it != itEnd; ++it) {
        Bus* bus = *it;
        auto data = bus->GetElectricalData();
        if(data.stabHasFault) {
            int n = data.number;

            // Insert fault
            if(EventTrigger(data.stabFaultTime, currentTime)) {
                double r, x;
                r = data.stabFaultResistance;
                x = data.stabFaultReactance;
                if(x < 1e-5) x = 1e-5;
                m_yBus[n][n] += std::complex<double>(1.0, 0.0) / std::complex<double>(r, x);
            }

            // Remove fault
            else if(EventTrigger(data.stabFaultTime + data.stabFaultLength, currentTime)) {
                double r, x;
                r = data.stabFaultResistance;
                x = data.stabFaultReactance;
                if(x < 1e-5) x = 1e-5;
                m_yBus[n][n] -= std::complex<double>(1.0, 0.0) / std::complex<double>(r, x);
            }
        }
    }

    // SyncGenerator switching
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* generator = *it;
        auto swData = generator->GetSwitchingData();
        for(unsigned int i = 0; i < swData.swType.size(); ++i) {
            if(EventTrigger(swData.swTime[i], currentTime)) {
                // Remove machine (only connected machines)
                if(swData.swType[i] == SwitchingType::SW_REMOVE && generator->IsOnline()) {
                    generator->SetOnline(false);
                    int n = static_cast<Bus*>(generator->GetParentList()[0])->GetElectricalData().number;
                    m_yBus[n][n] -= GetSyncMachineAdmittance(generator);
                }

                // Insert machine (only disconnected machines)
                if(swData.swType[i] == SwitchingType::SW_INSERT && !generator->IsOnline() && generator->GetParentList().size() == 1) {
                    if(generator->SetOnline(true)) {
                        int n = static_cast<Bus*>(generator->GetParentList()[0])->GetElectricalData().number;
                        m_yBus[n][n] += GetSyncMachineAdmittance(generator);
                    }
                }
            }
        }
    }

    // Induction motor switching
    for(auto it = m_indMotorList.begin(), itEnd = m_indMotorList.end(); it != itEnd; ++it) {
        IndMotor* motor = *it;
        auto swData = motor->GetSwitchingData();
        for(unsigned int i = 0; i < swData.swType.size(); ++i) {
            if(EventTrigger(swData.swTime[i], currentTime)) {
                // Remove machine (only connected machines)
                if(swData.swType[i] == SwitchingType::SW_REMOVE && motor->IsOnline() && motor->GetParentList().size() == 1) {
                    auto data = motor->GetElectricalData();
                    motor->SetOnline(false);
                    int n = static_cast<Bus*>(motor->GetParentList()[0])->GetElectricalData().number;
                    m_yBus[n][n] -= (std::complex<double>(1, 0) / std::complex<double>(data.r1t, data.xt));
                }

                // Insert machine (only disconnected machines)
                if(swData.swType[i] == SwitchingType::SW_INSERT && !motor->IsOnline() && motor->GetParentList().size() == 1) {
                    auto data = motor->GetElectricalData();
                    if(motor->SetOnline(true)) {
                        int n = static_cast<Bus*>(motor->GetParentList()[0])->GetElectricalData().number;
                        m_yBus[n][n] += (std::complex<double>(1, 0) / std::complex<double>(data.r1t, data.xt));
                    }
                }
            }
        }
    }

    // Load switching
    for(auto it = m_loadList.begin(), itEnd = m_loadList.end(); it != itEnd; ++it) {
        Load* load = *it;
        auto swData = load->GetSwitchingData();
        for(unsigned int i = 0; i < swData.swType.size(); ++i) {
            if(EventTrigger(swData.swTime[i], currentTime)) {
                // Remove load (only connected loads)
                if(swData.swType[i] == SwitchingType::SW_REMOVE && load->IsOnline() && load->GetParentList().size() == 1) {
                    load->SetOnline(false);
                    auto data = load->GetPUElectricalData(m_powerSystemBase);
                    Bus* parentBus = static_cast<Bus*>(load->GetParentList()[0]);
                    int n = parentBus->GetElectricalData().number;
                    std::complex<double> v = parentBus->GetElectricalData().voltage;
                    m_yBus[n][n] -=
                        std::complex<double>(data.activePower, -data.reactivePower) / (std::abs(v) * std::abs(v));
                }

                // Insert load (only disconnected load)
                if(swData.swType[i] == SwitchingType::SW_INSERT && !load->IsOnline() && load->GetParentList().size() == 1) {
                    if(load->SetOnline(true)) {
                        auto data = load->GetPUElectricalData(m_powerSystemBase);
                        Bus* parentBus = static_cast<Bus*>(load->GetParentList()[0]);
                        int n = parentBus->GetElectricalData().number;
                        std::complex<double> v = parentBus->GetElectricalData().voltage;
                        m_yBus[n][n] +=
                            std::complex<double>(data.activePower, -data.reactivePower) / (std::abs(v) * std::abs(v));
                    }
                }
            }
        }
    }

    // Line switching
    for(auto it = m_lineList.begin(), itEnd = m_lineList.end(); it != itEnd; ++it) {
        Line* line = *it;
        auto swData = line->GetSwitchingData();
        for(unsigned int i = 0; i < swData.swType.size(); ++i) {
            if(EventTrigger(swData.swTime[i], currentTime)) {
                // Remove line (only connected lines)
                if(swData.swType[i] == SwitchingType::SW_REMOVE && line->IsOnline()) {
                    line->SetOnline(false);
                    auto data = line->GetElectricalData();

                    int n1 = static_cast<Bus*>(line->GetParentList()[0])->GetElectricalData().number;
                    int n2 = static_cast<Bus*>(line->GetParentList()[1])->GetElectricalData().number;

                    m_yBus[n1][n2] += 1.0 / std::complex<double>(data.resistance, data.indReactance);
                    m_yBus[n2][n1] += 1.0 / std::complex<double>(data.resistance, data.indReactance);

                    m_yBus[n1][n1] -= 1.0 / std::complex<double>(data.resistance, data.indReactance);
                    m_yBus[n2][n2] -= 1.0 / std::complex<double>(data.resistance, data.indReactance);

                    m_yBus[n1][n1] -= std::complex<double>(0.0, data.capSusceptance / 2.0);
                    m_yBus[n2][n2] -= std::complex<double>(0.0, data.capSusceptance / 2.0);
                }

                // Insert line (only disconnected lines)
                if(swData.swType[i] == SwitchingType::SW_INSERT && !line->IsOnline() && line->GetParentList().size() == 2) {
                    if(line->SetOnline(true)) {
                        auto data = line->GetElectricalData();

                        int n1 = static_cast<Bus*>(line->GetParentList()[0])->GetElectricalData().number;
                        int n2 = static_cast<Bus*>(line->GetParentList()[1])->GetElectricalData().number;

                        m_yBus[n1][n2] -= 1.0 / std::complex<double>(data.resistance, data.indReactance);
                        m_yBus[n2][n1] -= 1.0 / std::complex<double>(data.resistance, data.indReactance);

                        m_yBus[n1][n1] += 1.0 / std::complex<double>(data.resistance, data.indReactance);
                        m_yBus[n2][n2] += 1.0 / std::complex<double>(data.resistance, data.indReactance);

                        m_yBus[n1][n1] += std::complex<double>(0.0, data.capSusceptance / 2.0);
                        m_yBus[n2][n2] += std::complex<double>(0.0, data.capSusceptance / 2.0);
                    }
                }
            }
        }
    }

    // Transformer switching
    for(auto it = m_transformerList.begin(), itEnd = m_transformerList.end(); it != itEnd; ++it) {
        Transformer* transformer = *it;
        auto swData = transformer->GetSwitchingData();
        for(unsigned int i = 0; i < swData.swType.size(); ++i) {
            if(EventTrigger(swData.swTime[i], currentTime)) {
                // Remove transformer (only connected transformers)
                if(swData.swType[i] == SwitchingType::SW_REMOVE && transformer->IsOnline()) {
                    transformer->SetOnline(false);
                    auto data = transformer->GetElectricalData();

                    int n1 = static_cast<Bus*>(transformer->GetParentList()[0])->GetElectricalData().number;
                    int n2 = static_cast<Bus*>(transformer->GetParentList()[1])->GetElectricalData().number;

                    if(data.turnsRatio == 1.0 && data.phaseShift == 0.0) {
                        m_yBus[n1][n2] -= -1.0 / std::complex<double>(data.resistance, data.indReactance);
                        m_yBus[n2][n1] -= -1.0 / std::complex<double>(data.resistance, data.indReactance);

                        m_yBus[n1][n1] -= 1.0 / std::complex<double>(data.resistance, data.indReactance);
                        m_yBus[n2][n2] -= 1.0 / std::complex<double>(data.resistance, data.indReactance);
                    } else {
                        // Complex turns ratio
                        double radPhaseShift = wxDegToRad(data.phaseShift);
                        std::complex<double> a = std::complex<double>(data.turnsRatio * std::cos(radPhaseShift),
                                                                      -data.turnsRatio * std::sin(radPhaseShift));

                        // Transformer admitance
                        std::complex<double> y = 1.0 / std::complex<double>(data.resistance, data.indReactance);
                        m_yBus[n1][n1] -= y / std::pow(std::abs(a), 2.0);
                        m_yBus[n1][n2] -= -(y / std::conj(a));
                        m_yBus[n2][n1] -= -(y / a);
                        m_yBus[n2][n2] -= y;
                    }
                }

                // Insert transformer (only disconnected transformers)
                if(swData.swType[i] == SwitchingType::SW_INSERT && !transformer->IsOnline() &&
                   transformer->GetParentList().size() == 2) {
                    if(transformer->SetOnline(true)) {
                        auto data = transformer->GetElectricalData();

                        int n1 = static_cast<Bus*>(transformer->GetParentList()[0])->GetElectricalData().number;
                        int n2 = static_cast<Bus*>(transformer->GetParentList()[1])->GetElectricalData().number;

                        if(data.turnsRatio == 1.0 && data.phaseShift == 0.0) {
                            m_yBus[n1][n2] += -1.0 / std::complex<double>(data.resistance, data.indReactance);
                            m_yBus[n2][n1] += -1.0 / std::complex<double>(data.resistance, data.indReactance);

                            m_yBus[n1][n1] += 1.0 / std::complex<double>(data.resistance, data.indReactance);
                            m_yBus[n2][n2] += 1.0 / std::complex<double>(data.resistance, data.indReactance);
                        } else {
                            // Complex turns ratio
                            double radPhaseShift = wxDegToRad(data.phaseShift);
                            std::complex<double> a = std::complex<double>(data.turnsRatio * std::cos(radPhaseShift),
                                                                          -data.turnsRatio * std::sin(radPhaseShift));

                            // Transformer admitance
                            std::complex<double> y = 1.0 / std::complex<double>(data.resistance, data.indReactance);
                            m_yBus[n1][n1] += y / std::pow(std::abs(a), 2.0);
                            m_yBus[n1][n2] += -(y / std::conj(a));
                            m_yBus[n2][n1] += -(y / a);
                            m_yBus[n2][n2] += y;
                        }
                    }
                }
            }
        }
    }

    // Capacitor switching
    for(auto it = m_capacitorList.begin(), itEnd = m_capacitorList.end(); it != itEnd; ++it) {
        Capacitor* capacitor = *it;
        auto swData = capacitor->GetSwitchingData();
        for(unsigned int i = 0; i < swData.swType.size(); ++i) {
            if(EventTrigger(swData.swTime[i], currentTime)) {
                // Remove capacitor (only connected capacitors)
                if(swData.swType[i] == SwitchingType::SW_REMOVE && capacitor->IsOnline()) {
                    capacitor->SetOnline(false);
                    auto data = capacitor->GetPUElectricalData(m_powerSystemBase);
                    int n = static_cast<Bus*>(capacitor->GetParentList()[0])->GetElectricalData().number;
                    m_yBus[n][n] -= std::complex<double>(0.0, data.reactivePower);
                }

                // Insert capacitor (only disconnected capacitors)
                if(swData.swType[i] == SwitchingType::SW_INSERT && !capacitor->IsOnline() && capacitor->GetParentList().size() == 1) {
                    if(capacitor->SetOnline(true)) {
                        auto data = capacitor->GetPUElectricalData(m_powerSystemBase);
                        int n = static_cast<Bus*>(capacitor->GetParentList()[0])->GetElectricalData().number;
                        m_yBus[n][n] += std::complex<double>(0.0, data.reactivePower);
                    }
                }
            }
        }
    }

    // Inductor switching
    for(auto it = m_inductorList.begin(), itEnd = m_inductorList.end(); it != itEnd; ++it) {
        Inductor* inductor = *it;
        auto swData = inductor->GetSwitchingData();
        for(unsigned int i = 0; i < swData.swType.size(); ++i) {
            if(EventTrigger(swData.swTime[i], currentTime)) {
                // Remove inductor (only connected inductors)
                if(swData.swType[i] == SwitchingType::SW_REMOVE && inductor->IsOnline()) {
                    inductor->SetOnline(false);
                    auto data = inductor->GetPUElectricalData(m_powerSystemBase);
                    int n = static_cast<Bus*>(inductor->GetParentList()[0])->GetElectricalData().number;
                    m_yBus[n][n] -= std::complex<double>(0.0, -data.reactivePower);
                }

                // Insert inductor (only disconnected inductors)
                if(swData.swType[i] == SwitchingType::SW_INSERT && !inductor->IsOnline() && inductor->GetParentList().size() == 1) {
                    if(inductor->SetOnline(true)) {
                        auto data = inductor->GetPUElectricalData(m_powerSystemBase);
                        int n = static_cast<Bus*>(inductor->GetParentList()[0])->GetElectricalData().number;
                        m_yBus[n][n] += std::complex<double>(0.0, -data.reactivePower);
                    }
                }
            }
        }
    }
}

void Electromechanical::InsertSyncMachinesOnYBus()
{
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* generator = *it;
        if(generator->IsOnline()) {
            auto data = generator->GetElectricalData();
            int n = static_cast<Bus*>(generator->GetParentList()[0])->GetElectricalData().number;
            m_yBus[n][n] += GetSyncMachineAdmittance(generator);
        }
    }
}

bool Electromechanical::EventTrigger(double eventTime, double currentTime)
{
    return (((eventTime - m_timeStep) < currentTime) && (eventTime >= currentTime));
}

std::complex<double> Electromechanical::GetSyncMachineAdmittance(SyncGenerator* generator)
{
    auto data = generator->GetElectricalData();
    double k = 1.0;  // Power base change factor.
    if(data.useMachineBase) {
        double oldBase = generator->GetValueFromUnit(data.nominalPower, data.nominalPowerUnit);
        k = m_powerSystemBase / oldBase;
    }

    double ra = data.armResistance * k;
    auto smModelData = GetSyncMachineModelData(generator);
    double xd = smModelData.xd;
    double xq = smModelData.xq;
    double xdq = 0.5 * (xd + xq);
    return (std::complex<double>(ra, -xdq) / std::complex<double>(ra * ra + xd * xq, 0.0));
}

bool Electromechanical::InitializeDynamicElements()
{
    // Buses
    for(auto it = m_busList.begin(), itEnd = m_busList.end(); it != itEnd; ++it) {
        Bus* bus = *it;
        auto data = bus->GetElectricalData();
        data.stabVoltageVector.clear();
        data.stabVoltageVector.shrink_to_fit();
        bus->SetElectricalData(data);
    }
    // Loads
    for(auto it = m_loadList.begin(), itEnd = m_loadList.end(); it != itEnd; ++it) {
        Load* load = *it;
        auto dataPU = load->GetPUElectricalData(m_powerSystemBase);
        auto data = load->GetElectricalData();

        double activePower = dataPU.activePower;
        double reactivePower = dataPU.reactivePower;

        if(load) data.voltage = static_cast<Bus*>(load->GetParentList()[0])->GetElectricalData().voltage;
        data.v0 = std::abs(data.voltage);
        data.y0 = std::complex<double>(activePower, -reactivePower) / (data.v0 * data.v0);

        if(data.loadType == CONST_IMPEDANCE) {
            std::complex<double> s0 = std::complex<double>(activePower, -reactivePower) * (data.v0 * data.v0);
            activePower = s0.real();
            reactivePower = -s0.imag();
        }

        data.pz0 = (data.constImpedanceActive / 100.0) * activePower;
        data.pi0 = (data.constCurrentActive / 100.0) * activePower;
        data.pp0 = (data.constPowerActive / 100.0) * activePower;

        data.qz0 = (data.constImpedanceReactive / 100.0) * reactivePower;
        data.qi0 = (data.constCurrentReactive / 100.0) * reactivePower;
        data.qp0 = (data.constPowerReactive / 100.0) * reactivePower;

        data.voltageVector.clear();
        data.voltageVector.shrink_to_fit();
        data.electricalPowerVector.clear();
        data.electricalPowerVector.shrink_to_fit();

        if(load->IsOnline())
            data.electricalPower = std::complex<double>(activePower, reactivePower);
        else {
            data.electricalPower = std::complex<double>(0.0, 0.0);
            data.voltage = std::complex<double>(0.0, 0.0);
        }

        load->SetElectricalData(data);
    }
    // Synchronous generators
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        auto dataPU = syncGenerator->GetPUElectricalData(m_powerSystemBase);
        auto data = syncGenerator->GetElectricalData();
        // if(syncGenerator->IsOnline()) {
        double k = 1.0;  // Power base change factor.
        if(data.useMachineBase) {
            double oldBase = syncGenerator->GetValueFromUnit(data.nominalPower, data.nominalPowerUnit);
            k = m_powerSystemBase / oldBase;
        }
        data.terminalVoltage = static_cast<Bus*>(syncGenerator->GetParentList()[0])->GetElectricalData().voltage;

        std::complex<double> conjS(dataPU.activePower, -dataPU.reactivePower);
        std::complex<double> vt = data.terminalVoltage;
        std::complex<double> ia = conjS / std::conj(vt);

        double xd = data.syncXd * k;
        double xq = data.syncXq * k;
        double ra = data.armResistance * k;

        if(data.model == Machines::SM_MODEL_1) {
            xq = data.transXd * k;
            xd = xq;
        } else if(data.syncXq == 0.0)
            xq = data.syncXd * k;

        double sd = 1.0;
        double sq = 1.0;
        double satF = 1.0;
        double xp = data.potierReactance * k;
        bool hasSaturation = false;
        if(data.satFactor != 0.0) {  // Have saturation.
            satF = (data.satFactor - 1.2) / std::pow(1.2, 7);
            if(xp == 0.0) xp = 0.8 * (data.transXd * k);
            hasSaturation = true;
        }

        // Initialize state variables
        std::complex<double> eq0 = vt + std::complex<double>(ra, xq) * ia;
        double delta = std::arg(eq0);

        double id0, iq0, vd0, vq0;
        ABCtoDQ0(ia, delta, id0, iq0);
        ABCtoDQ0(vt, delta, vd0, vq0);

        // Initialize saturation
        double xqs = xq;
        double xds = xd;
        if(hasSaturation) {
            double oldDelta = 0;
            bool exit = false;
            int numIt = 0;
            while(!exit) {
                oldDelta = delta;
                ABCtoDQ0(ia, delta, id0, iq0);
                ABCtoDQ0(vt, delta, vd0, vq0);

                // Direct-axis Potier voltage.
                double epd = vd0 + ra * id0 + xp * iq0;

                sq = 1.0 + satF * (xq / xd) * std::pow(epd, 6);
                xqs = (xq - xp) / sq + xp;
                eq0 = data.terminalVoltage + std::complex<double>(ra, xqs) * ia;
                delta = std::arg(eq0);
                if(std::abs(delta - oldDelta) < m_saturationTolerance) {
                    exit = true;
                } else if(numIt >= m_maxIterations) {
                    m_errorMsg = _("Error on initializate the saturation values of \"") + data.name + _("\".");
                    return false;
                }
                numIt++;
            }
            // Quadrature-axis Potier voltage.
            double epq = vq0 + ra * iq0 - xp * id0;
            sd = 1.0 + satF * std::pow(epq, 6);
            xds = (xd - xp) / sd + xp;
        }

        double ef0 = vq0 + ra * iq0 - xds * id0;

        data.initialFieldVoltage = ef0 * sd;
        data.fieldVoltage = data.initialFieldVoltage;
        data.pm = std::real((data.terminalVoltage * std::conj(ia)) + (std::abs(ia) * std::abs(ia) * ra));
        syncGenerator->IsOnline() ? data.speed = 2.0 * M_PI* m_systemFreq : data.speed = 2.0 * M_PI * data.ocFrequency;
        data.delta = delta;
        data.pe = data.pm;
        data.electricalPower = std::complex<double>(dataPU.activePower, dataPU.reactivePower);
        data.sd = sd;
        data.sq = sq;
        data.id = id0;
        data.iq = iq0;

        // Variables to extrapolate.
        data.oldIq = iq0;
        data.oldId = id0;
        data.oldPe = data.pe;
        data.oldSd = sd;
        data.oldSq = sq;

        switch(data.model) {
            case Machines::SM_MODEL_1: {
                data.tranEq = std::abs(eq0);

                data.tranEd = 0.0;
                data.subEq = 0.0;
                data.subEd = 0.0;
            } break;
            case Machines::SM_MODEL_2: {
                double tranXd = data.transXd * k;

                data.tranEq = ef0 + (xd - tranXd) * (id0 / sd);
                data.tranEd = 0.0;
                data.subEd = 0.0;
                data.subEq = 0.0;
            } break;
            case Machines::SM_MODEL_3: {
                double tranXd = data.transXd * k;
                double tranXq = data.transXq * k;
                if(tranXq == 0.0) tranXq = tranXd;

                data.tranEq = ef0 + (xd - tranXd) * (id0 / sd);
                data.tranEd = -(xq - tranXq) * (iq0 / sq);

                data.subEd = 0.0;
                data.subEq = 0.0;
            } break;
            case Machines::SM_MODEL_4: {
                double tranXd = data.transXd * k;
                double subXd = data.subXd * k;
                double subXq = data.subXq * k;
                if(subXd == 0.0) subXd = subXq;
                if(subXq == 0.0) subXq = subXd;

                data.tranEq = ef0 + (xd - tranXd) * (id0 / sd);
                data.tranEd = 0.0;
                data.subEq = data.tranEq + (tranXd - subXd) * (id0 / sd);
                data.subEd = -(xq - subXq) * (iq0 / sq);
            } break;
            case Machines::SM_MODEL_5: {
                double tranXd = data.transXd * k;
                double tranXq = data.transXq * k;
                double subXd = data.subXd * k;
                double subXq = data.subXq * k;
                if(subXd == 0.0) subXd = subXq;
                if(subXq == 0.0) subXq = subXd;

                data.tranEq = ef0 + (xd - tranXd) * (id0 / sd);
                data.tranEd = -(xq - tranXq) * (iq0 / sq);
                data.subEq = data.tranEq + (tranXd - subXd) * (id0 / sd);
                data.subEd = data.tranEd - (tranXq - subXq) * (iq0 / sq);
            } break;
            default:
                break;
        }

        // Initialize controllers
        if(data.useAVR) {
            if(data.avrSolver) delete data.avrSolver;
            data.avrSolver =
                new ControlElementSolver(data.avr, m_timeStep * m_ctrlTimeStepMultiplier, m_tolerance, m_parent);
            data.avrSolver->SetSwitchStatus(syncGenerator->IsOnline());
            data.avrSolver->SetCurrentTime(m_currentTime);
            data.avrSolver->SetTerminalVoltage(std::abs(data.terminalVoltage));
            data.avrSolver->SetInitialTerminalVoltage(std::abs(data.terminalVoltage));
            data.avrSolver->SetActivePower(dataPU.activePower);
            data.avrSolver->SetReactivePower(dataPU.reactivePower);
            data.avrSolver->SetVelocity(data.speed);
            data.avrSolver->SetInitialVelocity(data.speed);
            data.avrSolver->InitializeValues(false);
            if(!data.avrSolver->IsOK()) {
                m_errorMsg = _("Error on initializate the AVR of \"") + data.name + wxT("\".\n") +
                             data.avrSolver->GetErrorMessage();
                syncGenerator->SetElectricalData(data);
                return false;
            }
        }
        if(data.useSpeedGovernor) {
            if(data.speedGovSolver) delete data.speedGovSolver;
            data.speedGovSolver =
                new ControlElementSolver(data.speedGov, m_timeStep * m_ctrlTimeStepMultiplier, m_tolerance, m_parent);
            data.speedGovSolver->SetSwitchStatus(syncGenerator->IsOnline());
            data.speedGovSolver->SetCurrentTime(m_currentTime);
            data.speedGovSolver->SetActivePower(dataPU.activePower);
            data.speedGovSolver->SetReactivePower(dataPU.reactivePower);
            data.speedGovSolver->SetVelocity(data.speed);
            data.speedGovSolver->SetInitialVelocity(data.speed);
            data.speedGovSolver->SetInitialMecPower(data.pm);
            data.speedGovSolver->InitializeValues(false);
            if(!data.speedGovSolver->IsOK()) {
                m_errorMsg = _("Error on initializate the speed governor of \"") + data.name + wxT("\".\n") +
                             data.speedGovSolver->GetErrorMessage();
                syncGenerator->SetElectricalData(data);
                return false;
            }
        }
        //} else {
        // Initialize open circuit machine.
        //}
        // Reset plot data
        data.terminalVoltageVector.clear();
        data.terminalVoltageVector.shrink_to_fit();
        data.electricalPowerVector.clear();
        data.electricalPowerVector.shrink_to_fit();
        data.mechanicalPowerVector.clear();
        data.mechanicalPowerVector.shrink_to_fit();
        data.freqVector.clear();
        data.freqVector.shrink_to_fit();
        data.fieldVoltageVector.clear();
        data.fieldVoltageVector.shrink_to_fit();
        data.deltaVector.clear();
        data.deltaVector.shrink_to_fit();

        syncGenerator->SetElectricalData(data);
    }
    // Induction motors
    for(auto it = m_indMotorList.begin(), itEnd = m_indMotorList.end(); it != itEnd; ++it) {
        IndMotor* indMotor = *it;
        auto dataPU = indMotor->GetPUElectricalData(m_powerSystemBase);
        auto data = indMotor->GetElectricalData();

        double w0 = 2.0 * M_PI * m_systemFreq;
        std::complex<double> i1 = std::complex<double>(dataPU.activePower, -data.q0) / std::conj(data.terminalVoltage);
        double ir = std::real(i1);
        double im = std::imag(i1);
        std::complex<double> e = data.terminalVoltage - std::complex<double>(data.r1t, data.x1t) * i1;
        double te = std::real(e * std::conj(i1)) / w0;

        double wi = w0 * (1 - data.s0);  // Initial velocity
        data.as = te * (data.aw + (data.bw * w0) / wi + (data.cw * w0 * w0) / (wi * wi));
        data.bs = te * ((data.bw * w0) / wi + (2.0 * data.cw * w0 * w0) / (wi * wi));
        data.cs = (te * data.cw * w0 * w0) / (wi * wi);
        
        data.aCalc = data.as;
        data.bCalc = data.bs;
        data.cCalc = data.cs;

        if(indMotor->IsOnline()) {
            std::complex<double> tranE =
                (std::complex<double>(0, data.x0 - data.xt) * i1) / std::complex<double>(1.0, w0 * data.s0 * data.t0);

            data.tranEr = std::real(tranE);
            data.tranEm = std::imag(tranE);

            data.slip = data.s0;
            data.ir = ir;
            data.im = im;
            data.te = te;
        } else {  // Offline
            data.tranEr = 0.0;
            data.tranEm = 0.0;
            data.slip = 1.0 - 1e-7;
            data.ir = 0.0;
            data.im = 0.0;
            data.te = 0.0;
        }

        // Variables to extrapolate.
        data.oldTe = data.te;
        data.oldIr = data.ir;
        data.oldIm = data.im;

        // Reset plot data
        data.slipVector.clear();
        data.slipVector.shrink_to_fit();
        data.electricalTorqueVector.clear();
        data.electricalTorqueVector.shrink_to_fit();
        data.mechanicalTorqueVector.clear();
        data.mechanicalTorqueVector.shrink_to_fit();
        data.velocityVector.clear();
        data.velocityVector.shrink_to_fit();
        data.currentVector.clear();
        data.currentVector.shrink_to_fit();
        data.terminalVoltageVector.clear();
        data.terminalVoltageVector.shrink_to_fit();
        data.activePowerVector.clear();
        data.activePowerVector.shrink_to_fit();
        data.reactivePowerVector.clear();
        data.reactivePowerVector.shrink_to_fit();

        indMotor->SetElectricalData(data);
    }
    CalculateReferenceSpeed();
    return true;
}

bool Electromechanical::CalculateInjectedCurrents()
{
    // Reset injected currents vector
    for(unsigned int i = 0; i < m_iBus.size(); ++i) m_iBus[i] = std::complex<double>(0.0, 0.0);

    // Synchronous machines
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        auto data = syncGenerator->GetElectricalData();
        if(syncGenerator->IsOnline()) {
            double k = 1.0;  // Power base change factor.
            if(data.useMachineBase) {
                double oldBase = syncGenerator->GetValueFromUnit(data.nominalPower, data.nominalPowerUnit);
                k = m_powerSystemBase / oldBase;
            }

            double ra = data.armResistance * k;
            double xp = data.potierReactance * k;
            if(xp == 0.0) xp = 0.8 * data.transXd * k;

            int n = static_cast<Bus*>(syncGenerator->GetParentList()[0])->GetElectricalData().number;
            std::complex<double> e = std::complex<double>(0.0, 0.0);
            std::complex<double> v = m_vBus[n];
            std::complex<double> iInj = m_iBus[n];

            auto smModelData = GetSyncMachineModelData(syncGenerator);
            DQ0toABC(smModelData.ed, smModelData.eq, data.delta, e);
            double xd = smModelData.xd;
            double xq = smModelData.xq;

            double sd = data.sd;
            double sq = data.sq;
            double id, iq;

            // Calculate the saturation effect
            if(data.satFactor != 0.0) {
                if(!CalculateSyncMachineSaturation(syncGenerator, id, iq, sd, sq, false, k)) return false;
            }

            double xdq, xds, xqs, xdqs;
            xdq = 0.5 * (xd + xq);
            xds = (xd - xp) / sd + xp;
            xqs = (xq - xp) / sq + xp;
            xdqs = 0.5 * (xds + xqs);

            std::complex<double> y0 = std::complex<double>(ra, -xdq) / std::complex<double>(ra * ra + xd * xq, 0.0);
            std::complex<double> iUnadjusted = y0 * v;

            // [Ref] Arrillaga, J.; Arnold, C. P.. "Computer Modelling of Electrical Power Systems". Pg. 225-226
            // [Ref] Dommell, H. W.; Sato, N.. "Fast transient stability solutions". IEEE Transactions on Power
            // Apparatus and Systems, PAS-91 (4), 1643-1650
            std::complex<double> iSaliency = std::complex<double>(0.0, -((0.5 * (xqs - xds)) / (ra * ra + xds * xqs))) *
                                             (std::conj(e) - std::conj(v));
            iSaliency = iSaliency * std::cos(2.0 * data.delta) +
                        iSaliency * std::complex<double>(0.0, std::sin(2.0 * data.delta));

            // [Ref] Arrillaga, J.; Arnold, C. P.; Computer Modelling of Electrical Power Systems. Pg. 258-259
            std::complex<double> y0s = std::complex<double>(ra, -xdqs) / std::complex<double>(ra * ra + xds * xqs, 0.0);
            std::complex<double> iSaturation = y0s * (e - v);

            iInj = iUnadjusted + iSaliency + iSaturation;

            m_iBus[n] += iInj;

            // Remove the current flowing through y0 (i.e. iUnadjusted in this case, y0 is inserted in admittance
            // matrix) to calculate the electrical power.
            std::complex<double> iMachine = iInj - iUnadjusted;
            data.electricalPower = v * std::conj(iMachine);

            ABCtoDQ0(iMachine, data.delta, id, iq);

            data.id = id;
            data.iq = iq;
            data.sd = sd;
            data.sq = sq;
        } else {
            data.electricalPower = std::complex<double>(0.0, 0.0);
        }

        syncGenerator->SetElectricalData(data);
    }
    // Induction motors
    for(auto it = m_indMotorList.begin(), itEnd = m_indMotorList.end(); it != itEnd; ++it) {
        IndMotor* motor = *it;
        auto data = motor->GetElectricalData();
        if(motor->IsOnline()) {
            int n = static_cast<Bus*>(motor->GetParentList()[0])->GetElectricalData().number;
            std::complex<double> y0 = std::complex<double>(1.0, 0.0) / std::complex<double>(data.r1t, data.xt);
            std::complex<double> v = m_vBus[n];
            std::complex<double> e = std::complex<double>(data.tranEr, data.tranEm);
            std::complex<double> iInj = y0 * e;
            m_iBus[n] += iInj;

            std::complex<double> iMachine = y0 * (v - e);

            data.ir = std::real(iMachine);
            data.im = std::imag(iMachine);
        } else {
            data.ir = 0.0;
            data.im = 0.0;
        }
        motor->SetElectricalData(data);
    }

    // Loads
    for(auto it = m_loadList.begin(), itEnd = m_loadList.end(); it != itEnd; ++it) {
        Load* load = *it;
        auto data = load->GetElectricalData();

        if(load->IsOnline()) {
            int n = static_cast<Bus*>(load->GetParentList()[0])->GetElectricalData().number;
            data.voltage = m_vBus[n];
            double vAbs = std::abs(data.voltage);

            double pz, pi, pp, qz, qi, qp;
            pz = data.pz0 * std::pow(vAbs / data.v0, 2);
            pi = data.pi0 * (vAbs / data.v0);
            pp = data.pp0;
            qz = data.qz0 * std::pow(vAbs / data.v0, 2);
            qi = data.qi0 * (vAbs / data.v0);
            qp = data.qp0;

            // If voltage value is low, set the ZIP load to constant impedance.
            if(vAbs < data.constCurrentUV) {
                pi = data.pi0 * (data.constCurrentUV / data.v0) * std::pow(vAbs / data.constCurrentUV, 2);
                qi = data.qi0 * (data.constCurrentUV / data.v0) * std::pow(vAbs / data.constCurrentUV, 2);
            }
            if(vAbs < data.constPowerUV) {
                pp *= std::pow(vAbs / data.constPowerUV, 2);
                qp *= std::pow(vAbs / data.constPowerUV, 2);
            }

            double activePower = pz + pi + pp;
            double reactivePower = qz + qi + qp;

            std::complex<double> newY = std::complex<double>(activePower, -reactivePower) / (vAbs * vAbs);
            m_iBus[n] += (data.y0 - newY) * data.voltage;

            data.electricalPower = std::complex<double>(activePower, reactivePower);
        } else {
            data.voltage = std::complex<double>(0.0, 0.0);
            data.electricalPower = std::complex<double>(0.0, 0.0);
        }

        load->SetElectricalData(data);
    }
    return true;
}

void Electromechanical::CalculateIntegrationConstants(SyncGenerator* syncGenerator, double id, double iq, double k)
{
    CalculateReferenceSpeed();
    auto data = syncGenerator->GetElectricalData();

    double syncXd, syncXq, transXd, transXq, subXd, subXq;
    syncXd = data.syncXd * k;
    syncXq = data.syncXq * k;
    transXd = data.transXd * k;
    transXq = data.transXq * k;
    subXd = data.subXd * k;
    subXq = data.subXq * k;

    if(syncXq == 0.0) syncXq = syncXd;
    if(transXq == 0.0) transXq = transXd;
    if(subXd == 0.0) subXd = subXq;
    if(subXq == 0.0) subXq = subXd;

    double transTd0, transTq0, subTd0, subTq0;
    transTd0 = data.transTd0;
    transTq0 = data.transTq0;
    subTd0 = data.subTd0;
    subTq0 = data.subTq0;

    if(subTd0 == 0.0) subTd0 = subTq0;
    if(subTq0 == 0.0) subTq0 = subTd0;

    // Speed
    data.icSpeed.m = m_timeStep / ((4.0f * data.inertia / m_refSpeed) / k + m_timeStep * data.damping * k);
    data.icSpeed.c = (1.0f - 2.0f * data.icSpeed.m * data.damping * k) * data.speed +
                     data.icSpeed.m * (data.pm - data.pe + 2.0f * m_refSpeed * data.damping * k);

    // Delta
    data.icDelta.m = 0.5f * m_timeStep;
    data.icDelta.c = data.delta + data.icDelta.m * (data.speed - 2.0f * m_refSpeed);

    // Eq'
    if(data.model == Machines::SM_MODEL_2 || data.model == Machines::SM_MODEL_3 || data.model == Machines::SM_MODEL_4 ||
       data.model == Machines::SM_MODEL_5) {
        data.icTranEq.m = m_timeStep / (2.0f * transTd0 + m_timeStep);
        data.icTranEq.c = (1.0 - data.icTranEq.m * (1.0 + data.sd)) * data.tranEq +
                          data.icTranEq.m * (data.fieldVoltage + (syncXd - transXd) * id);
    }

    // Ed'
    if(data.model == Machines::SM_MODEL_3 || data.model == Machines::SM_MODEL_4 || data.model == Machines::SM_MODEL_5) {
        data.icTranEd.m = m_timeStep / (2.0f * transTq0 + m_timeStep);
        data.icTranEd.c =
            (1.0 - data.icTranEd.m * (1.0 + data.sq)) * data.tranEd - data.icTranEd.m * (syncXq - transXq) * iq;
    }

    // Eq''
    if(data.model == Machines::SM_MODEL_4 || data.model == Machines::SM_MODEL_5) {
        data.icSubEq.m = m_timeStep / (2.0f * subTd0 + m_timeStep);
        data.icSubEq.c = (1.0 - data.icSubEq.m * (1.0 + data.sd)) * data.subEq +
                         data.icSubEq.m * (data.sd * data.tranEq + (transXd - subXd) * id);
    }
    // Ed''
    if(data.model == Machines::SM_MODEL_4) {
        data.icSubEd.m = m_timeStep / (2.0f * subTq0 + m_timeStep);
        data.icSubEd.c =
            (1.0f - data.icSubEd.m * (1.0 + data.sq)) * data.subEd - data.icSubEd.m * (syncXq - subXq) * iq;
    }
    if(data.model == Machines::SM_MODEL_5) {
        data.icSubEd.m = m_timeStep / (2.0f * subTq0 + m_timeStep);
        data.icSubEd.c = (1.0f - data.icSubEd.m * (1.0 + data.sq)) * data.subEd +
                         data.icSubEd.m * (data.sq * data.tranEd - (transXq - subXq) * iq);
    }

    syncGenerator->SetElectricalData(data);
}

void Electromechanical::CalculateIntegrationConstants(IndMotor* indMotor, double ir, double im, double k)
{
    double w0 = 2.0 * M_PI * m_systemFreq;

    auto data = indMotor->GetElectricalData();
    
    // If the velocity is too low set mechanical torque to zero (a, b and c coeficients)
    if(data.slip > 0.99999) {
        data.as = 0.0;
        data.bs = 0.0;
        data.cs = 0.0;
    } else {
        data.as = data.aCalc;
        data.bs = data.bCalc;
        data.cs = data.cCalc;
    }

    // Mechanical equations
    // s
    data.icSlip.m = m_timeStep / ((4.0f * data.inertia / w0) / k + data.bs * m_timeStep);
    data.icSlip.c = data.slip * (1.0 - 2.0 * data.bs * data.icSlip.m) +
                    data.icSlip.m * (2.0 * data.as + data.cs * data.slip * data.slip - data.te);

    // Electrical equations
    // Er
    data.icTranEr.m = m_timeStep / (2.0 * data.t0 + m_timeStep);
    data.icTranEr.c = data.tranEr * (1.0 - 2.0 * data.icTranEr.m) +
                      data.icTranEr.m * (w0 * data.t0 * data.slip * data.tranEm - (data.x0 - data.xt) * im);
    // Em
    data.icTranEm.m = m_timeStep / (2.0 * data.t0 + m_timeStep);
    data.icTranEm.c = data.tranEm * (1.0 - 2.0 * data.icTranEm.m) -
                      data.icTranEm.m * (w0 * data.t0 * data.slip * data.tranEr - (data.x0 - data.xt) * ir);

    indMotor->SetElectricalData(data);
}

bool Electromechanical::SolveMachines()
{
    // First interation values and extrapolations
    // Synchronous machines
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        auto data = syncGenerator->GetElectricalData();

        if(syncGenerator->IsOnline()) {
            double id, iq, pe, sd, sq;
            pe = data.pe;
            id = data.id;
            iq = data.iq;
            sd = data.sd;
            sq = data.sq;

            double k = 1.0;  // Power base change factor.
            if(data.useMachineBase) {
                double oldBase = syncGenerator->GetValueFromUnit(data.nominalPower, data.nominalPowerUnit);
                k = m_powerSystemBase / oldBase;
            }

            // Calculate integration constants.
            CalculateIntegrationConstants(syncGenerator, id, iq, k);

            if(!CalculateNonIntVariables(syncGenerator, id, iq, sd, sq, pe, k)) return false;
            // Extrapolate nonintegrable variables.
            id = 2.0 * id - data.oldId;
            iq = 2.0 * iq - data.oldIq;
            pe = 2.0 * pe - data.oldPe;
            sd = 2.0 * sd - data.oldSd;
            sq = 2.0 * sq - data.oldSq;

            CalculateIntVariables(syncGenerator, id, iq, sd, sq, pe, k);
        } else {
            CalculateIntegrationConstants(syncGenerator, 0.0f, 0.0f);
        }
    }
    // Induction machines
    for(auto it = m_indMotorList.begin(), itEnd = m_indMotorList.end(); it != itEnd; ++it) {
        IndMotor* indMotor = *it;
        auto data = indMotor->GetElectricalData();

        //if(indMotor->IsOnline()) {
            double ir, im, te;
            te = data.te;
            ir = data.ir;
            im = data.im;

            double k = 1.0;  // Power base change factor.
            if(data.useMachinePowerAsBase) {
                double oldBase = indMotor->GetValueFromUnit(data.ratedPower, data.ratedPowerUnit);
                k = m_powerSystemBase / oldBase;
            }

            // Calculate integration constants.
            CalculateIntegrationConstants(indMotor, ir, im, k);

            if(!CalculateNonIntVariables(indMotor, ir, im, te, k)) return false;
            // Extrapolate nonintegrable variables.
            ir = 2.0 * ir - data.oldIr;
            im = 2.0 * im - data.oldIm;
            te = 2.0 * te - data.oldTe;

            CalculateIntVariables(indMotor, ir, im, te, k);
        //} else {
            //CalculateIntegrationConstants(indMotor, 0.0f, 0.0f);
        //}
    }

    double error = 1.0;
    int iterations = 0;
    while(error > m_tolerance) {
        error = 0.0;

        // Calculate the injected currents.
        if(!CalculateInjectedCurrents()) return false;

        // Calculate the buses voltages.
        m_vBus = LUEvaluate(m_yBusU, m_yBusL, m_iBus);

        // Solve synchronous machine equations.
        for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
            SyncGenerator* syncGenerator = *it;

            auto data = syncGenerator->GetElectricalData();

            double id = data.id;
            double iq = data.iq;
            double pe = data.pe;
            double sd = data.sd;
            double sq = data.sq;

            // Power base change factor.
            double k = 1.0;
            if(data.useMachineBase) {
                double oldBase = syncGenerator->GetValueFromUnit(data.nominalPower, data.nominalPowerUnit);
                k = m_powerSystemBase / oldBase;
            }

            // Calculate id, iq, dq, sd
            if(!CalculateNonIntVariables(syncGenerator, id, iq, sd, sq, pe, k)) return false;

            double genError = CalculateIntVariables(syncGenerator, id, iq, sd, sq, pe, k);

            if(genError > error) error = genError;
        }

        // Solve induction machine equation
        for(auto it = m_indMotorList.begin(), itEnd = m_indMotorList.end(); it != itEnd; ++it) {
            IndMotor* indMotor = *it;

            auto data = indMotor->GetElectricalData();

            double ir = data.ir;
            double im = data.im;
            double te = data.te;

            // Power base change factor.
            double k = 1.0;
            if(data.useMachinePowerAsBase) {
                double oldBase = indMotor->GetValueFromUnit(data.ratedPower, data.ratedPowerUnit);
                k = m_powerSystemBase / oldBase;
            }

            // Calculate te
            if(!CalculateNonIntVariables(indMotor, ir, im, te, k)) return false;

            double motorError = CalculateIntVariables(indMotor, ir, im, te, k);

            if(motorError > error) error = motorError;
        }

        ++iterations;

        if(iterations > m_maxIterations) {
            m_errorMsg = _("Impossible to solve the system machines.\nCheck the system parameters and/or "
                           "decrease the time step.");
            return false;
        }
    }
    m_iterationsNum = iterations;

    // Solve controllers.
    int ctrlRatio = static_cast<int>(1 / m_ctrlTimeStepMultiplier);
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        auto data = syncGenerator->GetElectricalData();
        if(data.useAVR && data.avrSolver) {
            data.avrSolver->SetSwitchStatus(syncGenerator->IsOnline());
            data.avrSolver->SetCurrentTime(m_currentTime);
            data.avrSolver->SetTerminalVoltage(std::abs(data.terminalVoltage));
            data.avrSolver->SetDeltaActivePower((data.electricalPower.real() - data.avrSolver->GetActivePower()) /
                                                m_timeStep);
            data.avrSolver->SetActivePower(data.electricalPower.real());
            data.avrSolver->SetReactivePower(data.electricalPower.imag());
            data.avrSolver->SetDeltaVelocity((data.speed - data.avrSolver->GetVelocity()) / m_timeStep);
            data.avrSolver->SetVelocity(data.speed);

            for(int i = 0; i < ctrlRatio; ++i) data.avrSolver->SolveNextStep();

            data.fieldVoltage = data.initialFieldVoltage + data.avrSolver->GetFieldVoltage();
        }

        if(data.useSpeedGovernor && data.speedGovSolver) {
            data.speedGovSolver->SetSwitchStatus(syncGenerator->IsOnline());
            data.speedGovSolver->SetCurrentTime(m_currentTime);
            data.speedGovSolver->SetVelocity(data.speed);
            data.speedGovSolver->SetActivePower(data.electricalPower.real());
            data.speedGovSolver->SetReactivePower(data.electricalPower.imag());

            for(int i = 0; i < ctrlRatio; ++i) data.speedGovSolver->SolveNextStep();

            data.pm = data.speedGovSolver->GetMechanicalPower();
        }
        syncGenerator->SetElectricalData(data);
    }

    return true;
}

void Electromechanical::SaveData()
{
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        auto data = syncGenerator->GetElectricalData();
        if(data.plotSyncMachine) { syncGenerator->SavePlotData(); }
    }
    for(auto it = m_busList.begin(), itEnd = m_busList.end(); it != itEnd; ++it) {
        Bus* bus = *it;
        auto data = bus->GetElectricalData();
        if(data.plotBus) {
            data.stabVoltageVector.emplace_back(m_vBus[data.number]);
            bus->SetElectricalData(data);
        }
    }
    for(auto it = m_loadList.begin(), itEnd = m_loadList.end(); it != itEnd; ++it) {
        Load* load = *it;
        auto data = load->GetElectricalData();
        if(data.plotLoad) {
            data.voltageVector.emplace_back(data.voltage);
            data.electricalPowerVector.emplace_back(data.electricalPower);
            load->SetElectricalData(data);
        }
    }
    for(auto it = m_indMotorList.begin(), itEnd = m_indMotorList.end(); it != itEnd; ++it) {
        IndMotor* motor = *it;
        auto data = motor->GetElectricalData();
        if(data.plotIndMachine) {
            data.slipVector.emplace_back(data.slip * 100.0);
            data.electricalTorqueVector.emplace_back(data.te * 2.0 * M_PI * m_systemFreq);
            double tm = (data.as - data.bs * data.slip + data.cs * data.slip * data.slip) * 2.0 * M_PI * m_systemFreq;
            data.mechanicalTorqueVector.emplace_back(tm);
            double w = (1.0 - data.slip) * 2.0 * M_PI * m_systemFreq;
            data.velocityVector.emplace_back(w);
            std::complex<double> i1 = std::complex<double>(data.ir, data.im);
            data.currentVector.emplace_back(std::abs(i1));
            int n = static_cast<Bus*>(motor->GetParentList()[0])->GetElectricalData().number;
            std::complex<double> v = m_vBus[n];
            data.terminalVoltageVector.emplace_back(std::abs(v));
            std::complex<double> s = v * std::conj(i1);
            data.activePowerVector.emplace_back(std::real(s));
            data.reactivePowerVector.emplace_back(std::imag(s));
            motor->SetElectricalData(data);
        }
    }
    m_iterationsNumVector.emplace_back(m_iterationsNum);
}

void Electromechanical::SetSyncMachinesModel()
{
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        auto data = syncGenerator->GetElectricalData();
        data.model = GetMachineModel(syncGenerator);
        syncGenerator->SetElectricalData(data);
    }
}

bool Electromechanical::CalculateNonIntVariables(SyncGenerator* syncGenerator,
                                                 double& id,
                                                 double& iq,
                                                 double& sd,
                                                 double& sq,
                                                 double& pe,
                                                 double k)
{
    auto data = syncGenerator->GetElectricalData();
    int n = static_cast<Bus*>(syncGenerator->GetParentList()[0])->GetElectricalData().number;

    if(syncGenerator->IsOnline()) { data.terminalVoltage = m_vBus[n]; }

    double vd, vq;
    ABCtoDQ0(data.terminalVoltage, data.delta, vd, vq);

    if(data.satFactor != 0.0) {
        if(!CalculateSyncMachineSaturation(syncGenerator, id, iq, sd, sq, true, k)) return false;
        data.sd = sd;
        data.sq = sq;
        data.oldSd = sd;
        data.oldSq = sq;
    }

    if(syncGenerator->IsOnline()) {
        pe = id * vd + iq * vq + (id * id + iq * iq) * data.armResistance * k;
    } else {
        pe = id = iq = 0.0f;
    }
    data.pe = pe;
    data.id = id;
    data.iq = iq;
    data.oldPe = pe;
    data.oldId = id;
    data.oldIq = iq;
    syncGenerator->SetElectricalData(data);

    return true;
}

bool Electromechanical::CalculateNonIntVariables(IndMotor* indMotor, double& ir, double& im, double& te, double k)
{
    auto data = indMotor->GetElectricalData();
    if(indMotor->IsOnline()) {
        double w0 = 2.0 * M_PI * m_systemFreq;
        te = (data.tranEr * ir + data.tranEm * im) / w0;
    } else {
        te = ir = im = 0.0;
    }
    data.te = te;
    data.ir = ir;
    data.im = im;
    data.oldTe = te;
    data.oldIr = ir;
    data.oldIm = im;
    indMotor->SetElectricalData(data);

    return true;
}

double Electromechanical::CalculateIntVariables(SyncGenerator* syncGenerator,
                                                double id,
                                                double iq,
                                                double sd,
                                                double sq,
                                                double pe,
                                                double k)
{
    double error = 0.0;
    auto data = syncGenerator->GetElectricalData();

    // Mechanical differential equations.
    double w = data.icSpeed.c + data.icSpeed.m * (data.pm - pe);
    error = std::max(error, std::abs(data.speed - w) / m_refSpeed);

    double delta = data.icDelta.c + data.icDelta.m * w;
    error = std::max(error, std::abs(data.delta - delta));

    data.speed = w;
    data.delta = delta;

    // Electrical differential equations
    switch(data.model) {
        case Machines::SM_MODEL_1: {
            // There is no differential equations.
        } break;
        case Machines::SM_MODEL_2: {
            double syncXd, transXd;
            syncXd = data.syncXd * k;
            transXd = data.transXd * k;

            double tranEq = (data.icTranEq.c + data.icTranEq.m * (data.fieldVoltage + (syncXd - transXd) * id)) /
                            (1.0 + data.icTranEq.m * (sd - 1.0));
            error = std::max(error, std::abs(data.tranEq - tranEq));

            data.tranEq = tranEq;
        } break;
        case Machines::SM_MODEL_3: {
            double syncXd, syncXq, transXd, transXq;
            syncXd = data.syncXd * k;
            syncXq = data.syncXq * k;
            transXd = data.transXd * k;
            transXq = data.transXq * k;
            if(syncXq == 0.0) syncXq = syncXd;
            if(transXq == 0.0) transXq = transXd;

            double tranEq = (data.icTranEq.c + data.icTranEq.m * (data.fieldVoltage + (syncXd - transXd) * id)) /
                            (1.0 + data.icTranEq.m * (sd - 1.0));
            error = std::max(error, std::abs(data.tranEq - tranEq));

            double tranEd =
                (data.icTranEd.c - data.icTranEd.m * (syncXq - transXq) * iq) / (1.0 + data.icTranEd.m * (sq - 1.0));
            error = std::max(error, std::abs(data.tranEd - tranEd));

            data.tranEq = tranEq;
            data.tranEd = tranEd;

            if(!syncGenerator->IsOnline()) {
                std::complex<double> e;
                DQ0toABC(data.tranEd, data.tranEq, data.delta, e);
                data.terminalVoltage = e;
            }
        } break;
        case Machines::SM_MODEL_4: {
            double syncXd, syncXq, transXd, subXd, subXq;
            syncXd = data.syncXd * k;
            syncXq = data.syncXq * k;
            transXd = data.transXd * k;
            subXd = data.subXd * k;
            subXq = data.subXq * k;
            if(syncXq == 0.0) syncXq = syncXd;
            if(subXd == 0.0) subXd = subXq;
            if(subXq == 0.0) subXq = subXd;

            double tranEq = (data.icTranEq.c + data.icTranEq.m * (data.fieldVoltage + (syncXd - transXd) * id)) /
                            (1.0 + data.icTranEq.m * (sd - 1.0));
            error = std::max(error, std::abs(data.tranEq - tranEq));

            double subEq = (data.icSubEq.c + data.icSubEq.m * (sd * tranEq + (transXd - subXd) * id)) /
                           (1.0 + data.icSubEq.m * (sd - 1.0));
            error = std::max(error, std::abs(data.subEq - subEq));

            double subEd =
                (data.icSubEd.c - data.icSubEd.m * ((syncXq - subXq) * iq)) / (1.0 + data.icSubEd.m * (sq - 1.0));
            error = std::max(error, std::abs(data.subEd - subEd));

            data.tranEq = tranEq;
            data.subEq = subEq;
            data.subEd = subEd;
        } break;
        case Machines::SM_MODEL_5: {
            double syncXd, syncXq, transXd, transXq, subXd, subXq;
            syncXd = data.syncXd * k;
            syncXq = data.syncXq * k;
            transXd = data.transXd * k;
            transXq = data.transXq * k;
            subXd = data.subXd * k;
            subXq = data.subXq * k;
            if(syncXq == 0.0) syncXq = syncXd;
            if(transXq == 0.0) transXq = transXd;
            if(subXd == 0.0) subXd = subXq;
            if(subXq == 0.0) subXq = subXd;

            double tranEq = (data.icTranEq.c + data.icTranEq.m * (data.fieldVoltage + (syncXd - transXd) * id)) /
                            (1.0 + data.icTranEq.m * (sd - 1.0));
            error = std::max(error, std::abs(data.tranEq - tranEq));

            double tranEd =
                (data.icTranEd.c - data.icTranEd.m * (syncXq - transXq) * iq) / (1.0 + data.icTranEd.m * (sq - 1.0));
            error = std::max(error, std::abs(data.tranEd - tranEd));

            double subEq = (data.icSubEq.c + data.icSubEq.m * (sd * tranEq + (transXd - subXd) * id)) /
                           (1.0 + data.icSubEq.m * (sd - 1.0));
            error = std::max(error, std::abs(data.subEq - subEq));

            double subEd = (data.icSubEd.c + data.icSubEd.m * (sq * tranEd - (transXq - subXq) * iq)) /
                           (1.0 + data.icSubEd.m * (sq - 1.0));
            error = std::max(error, std::abs(data.subEd - subEd));

            data.tranEq = tranEq;
            data.tranEd = tranEd;
            data.subEq = subEq;
            data.subEd = subEd;
        } break;
    }

    syncGenerator->SetElectricalData(data);
    return error;
}

double Electromechanical::CalculateIntVariables(IndMotor* indMotor, double ir, double im, double te, double k)
{
    double error = 0.0;
    auto data = indMotor->GetElectricalData();
    double w0 = 2.0 * M_PI * m_systemFreq;

    // Mechanical differential equations
    // Using Newton method to solve the non-linear slip equation: s = Cs + Ms * (C * s^2 - Te):
    double slip = data.slip;  // Initial value. CAN BE THE PROBLEM ON MOTOR START!
    double ds = (data.icSlip.c + data.icSlip.m * (data.cs * slip * slip - te) - slip) /
                (1.0 - 2.0 * data.icSlip.m * data.cs * slip * slip);
    int iteration = 0;
    while(std::abs(ds) > 1e-8) {
        slip += ds;
        ds = (data.icSlip.c + data.icSlip.m * (data.cs * slip * slip - te) - slip) /
             (1.0 - 2.0 * data.icSlip.m * data.cs * slip * slip);
        iteration++;
        if(iteration > m_maxIterations) break;
    }

    //if(!indMotor->IsOnline()) slip = 1.0 - 1e-7;
    error = std::max(error, std::abs(data.slip - slip));
    data.slip = slip;

    // Change T'0 with the cage factor
    if(data.useKf)
        data.t0 = (data.x2t + data.xmt) / (2.0 * M_PI * m_systemFreq * data.r2t * (1.0 + data.kf * data.r2t));

    // Electrical differential equations
    double tranEr = data.icTranEr.c + data.icTranEr.m * (w0 * data.t0 * slip * data.tranEm - (data.x0 - data.xt) * im);
    error = std::max(error, std::abs(data.tranEr - tranEr));

    double tranEm = data.icTranEm.c - data.icTranEm.m * (w0 * data.t0 * slip * data.tranEr - (data.x0 - data.xt) * ir);
    error = std::max(error, std::abs(data.tranEm - tranEm));

    data.tranEr = tranEr;
    data.tranEm = tranEm;

    indMotor->SetElectricalData(data);
    return error;
}

void Electromechanical::CalculateReferenceSpeed()
{
    if(m_useCOI) {
        double sumHW = 0.0;
        double sumH = 0.0;
        for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
            SyncGenerator* syncGenerator = *it;
            if(syncGenerator->IsOnline()) {
                auto data = syncGenerator->GetElectricalData();
                double k = 1.0;  // Power base change factor.
                if(data.useMachineBase) {
                    double oldBase = syncGenerator->GetValueFromUnit(data.nominalPower, data.nominalPowerUnit);
                    k = m_powerSystemBase / oldBase;
                }
                sumH += data.inertia / k;
                sumHW += data.inertia * data.speed / k;
            }
        }
        m_refSpeed = sumHW / sumH;
    } else {
        m_refSpeed = 2.0 * M_PI * m_systemFreq;
    }
}

bool Electromechanical::CalculateSyncMachineSaturation(SyncGenerator* syncMachine,
                                                       double& id,
                                                       double& iq,
                                                       double& sd,
                                                       double& sq,
                                                       bool updateCurrents,
                                                       double k)
{
    // [Ref] Arrillaga, J.; Arnold, C. P.. "Computer Modelling of Electrical Power Systems". Pg. 254-260
    auto data = syncMachine->GetElectricalData();
    auto smDataModel = GetSyncMachineModelData(syncMachine);

    int n = static_cast<Bus*>(syncMachine->GetParentList()[0])->GetElectricalData().number;
    if(syncMachine->IsOnline()) { data.terminalVoltage = m_vBus[n]; }
    double idCalc = id;
    double iqCalc = iq;
    double sdCalc = sd;
    double sqCalc = sq;

    double vd, vq;
    ABCtoDQ0(data.terminalVoltage, data.delta, vd, vq);
    double deltaVd = smDataModel.ed - vd;
    double deltaVq = smDataModel.eq - vq;

    double ra = data.armResistance * k;
    double xd = smDataModel.xd;
    double xq = smDataModel.xq;

    double syncXd = data.syncXd * k;
    double syncXq = data.syncXq * k;
    if(data.model == Machines::SM_MODEL_1) {
        syncXq = data.transXd * k;
        syncXd = syncXq;
    } else if(data.syncXq == 0.0)
        syncXq = data.syncXd * k;

    double xp = data.potierReactance * k;
    if(xp == 0.0) xp = 0.8 * data.transXd * k;
    double satFacd = (data.satFactor - 1.2) / std::pow(1.2, 7);
    double satFacq = satFacd * (syncXq / syncXd);

    bool exit = false;
    int iterations = 0;
    while(!exit) {
        double oldSd = sdCalc;
        double oldSq = sqCalc;

        // Saturated reactances.
        double xds = (xd - xp) / sdCalc + xp;
        double xqs = (xq - xp) / sqCalc + xp;
        // dq currents.
        double den = 1.0 / (ra * ra + xds * xqs);
        iqCalc = den * (ra * deltaVq + xds * deltaVd);
        idCalc = den * (-xqs * deltaVq + ra * deltaVd);
        // Potier voltages
        double epq = vq + ra * iqCalc - xp * idCalc;
        double epd = vd + ra * idCalc + xp * iqCalc;
        // Saturation factors.
        // Gauss
        /*sdCalc = 1.0 + satFacd * std::pow(epq, 6);
        sqCalc = 1.0 + satFacq * std::pow(epd, 6);*/

        // Newton-raphson
        double f1 = 1.0 - sdCalc + satFacd * std::pow(epq, 6);
        double f2 = 1.0 - sqCalc + satFacq * std::pow(epd, 6);
        double dF1dSd =
            (6.0 * satFacd * std::pow(epq, 5) * xp * (xd - xp) * deltaVq) / ((sdCalc - 1.0) * xp + xd) - 1.0;
        double dF2dSq =
            (6.0 * satFacq * std::pow(epd, 5) * xp * (xq - xp) * deltaVd) / ((sqCalc - 1.0) * xp + xq) - 1.0;

        sdCalc = sdCalc - f1 / dF1dSd;
        sqCalc = sqCalc - f2 / dF2dSq;

        double error = std::abs(sdCalc - oldSd) + std::abs(sqCalc - oldSq);
        if(error < m_saturationTolerance) exit = true;

        iterations++;
        if((iterations >= m_maxIterations) & !exit) {
            m_errorMsg =
                _("It was not possible to solve the saturation of the synchronous machine \"") + data.name + wxT("\".");
            return false;
        }
    }

    sd = sdCalc;
    sq = sqCalc;
    if(updateCurrents) {
        id = idCalc;
        iq = iqCalc;
    }
    return true;
}

SyncMachineModelData Electromechanical::GetSyncMachineModelData(SyncGenerator* syncMachine)
{
    SyncMachineModelData smModelData;

    auto data = syncMachine->GetElectricalData();
    double k = 1.0;  // Power base change factor.
    if(data.useMachineBase) {
        double oldBase = syncMachine->GetValueFromUnit(data.nominalPower, data.nominalPowerUnit);
        k = m_powerSystemBase / oldBase;
    }

    switch(data.model) {
        case Machines::SM_MODEL_1: {
            smModelData.ed = data.tranEd;
            smModelData.eq = data.tranEq;
            smModelData.xq = data.transXd * k;
            smModelData.xd = smModelData.xq;
        } break;
        case Machines::SM_MODEL_2: {
            smModelData.ed = data.tranEd;
            smModelData.eq = data.tranEq;
            smModelData.xd = data.transXd * k;
            smModelData.xq = data.transXq * k;
            if(smModelData.xq == 0.0) {
                smModelData.xq = data.syncXq * k;
                if(smModelData.xq == 0.0) { smModelData.xq = data.syncXd * k; }
            }
        } break;
        case Machines::SM_MODEL_3: {
            smModelData.ed = data.tranEd;
            smModelData.eq = data.tranEq;
            smModelData.xd = data.transXd * k;
            smModelData.xq = data.transXq * k;
            if(smModelData.xq == 0.0) smModelData.xq = smModelData.xd;
        } break;
        case Machines::SM_MODEL_4:
        case Machines::SM_MODEL_5: {
            smModelData.ed = data.subEd;
            smModelData.eq = data.subEq;
            smModelData.xd = data.subXd * k;
            smModelData.xq = data.subXq * k;
            if(smModelData.xd == 0.0) smModelData.xd = smModelData.xq;
            if(smModelData.xq == 0.0) smModelData.xq = smModelData.xd;
        } break;
    }
    return smModelData;
}

void Electromechanical::PreallocateVectors()
{
    int numPoints = static_cast<unsigned int>(m_simTime / m_plotTime);

    m_timeVector.reserve(numPoints);
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        auto data = syncGenerator->GetElectricalData();
        if(data.plotSyncMachine) {
            data.terminalVoltageVector.reserve(numPoints);
            data.electricalPowerVector.reserve(numPoints);
            data.mechanicalPowerVector.reserve(numPoints);
            data.freqVector.reserve(numPoints);
            data.fieldVoltageVector.reserve(numPoints);
            data.deltaVector.reserve(numPoints);
            syncGenerator->SetElectricalData(data);
        }
    }
    for(auto it = m_busList.begin(), itEnd = m_busList.end(); it != itEnd; ++it) {
        Bus* bus = *it;
        auto data = bus->GetElectricalData();
        if(data.plotBus) {
            data.stabVoltageVector.reserve(numPoints);
            bus->SetElectricalData(data);
        }
    }
    for(auto it = m_loadList.begin(), itEnd = m_loadList.end(); it != itEnd; ++it) {
        Load* load = *it;
        auto data = load->GetElectricalData();
        if(data.plotLoad) {
            data.voltageVector.reserve(numPoints);
            data.electricalPowerVector.reserve(numPoints);
            load->SetElectricalData(data);
        }
    }
    for(auto it = m_indMotorList.begin(), itEnd = m_indMotorList.end(); it != itEnd; ++it) {
        IndMotor* motor = *it;
        auto data = motor->GetElectricalData();
        if(data.plotIndMachine) {
            data.slipVector.reserve(numPoints);
            data.electricalTorqueVector.reserve(numPoints);
            motor->SetElectricalData(data);
        }
    }
}

std::complex<double> Electromechanical::GetIndMachineAdmittance(IndMotor* motor)
{
    auto data = motor->GetElectricalData();
    auto dataPU = motor->GetPUElectricalData(m_powerSystemBase);
    std::complex<double> v = static_cast<Bus*>(motor->GetParentList()[0])->GetElectricalData().voltage;
    std::complex<double> y0 = (std::complex<double>(1, 0) / std::complex<double>(data.r1t, data.xt));
    // The difference between calculated and defined reactive power
    std::complex<double> yq = std::complex<double>(0.0, data.q0 - dataPU.reactivePower) / (std::abs(v) * std::abs(v));
    return y0 + yq;
}

bool Electromechanical::InsertIndMachinesOnYBus()
{
    for(auto it = m_indMotorList.begin(), itEnd = m_indMotorList.end(); it != itEnd; ++it) {
        IndMotor* motor = *it;
        if(!CalculateIndMachinesTransientValues(motor)) return false;
        if(motor->IsOnline()) {
            int n = static_cast<Bus*>(motor->GetParentList()[0])->GetElectricalData().number;
            m_yBus[n][n] += GetIndMachineAdmittance(motor);
        }
    }
    return true;
}

bool Electromechanical::CalculateIndMachinesTransientValues(IndMotor* motor)
{
    auto data = motor->GetElectricalData();
    auto dataPU = motor->GetPUElectricalData(m_powerSystemBase);
    double k = 1.0;  // Power base change factor.
    if(data.useMachinePowerAsBase) {
        double oldBase = motor->GetValueFromUnit(data.ratedPower, data.ratedPowerUnit);
        k = m_powerSystemBase / oldBase;
    }

    data.terminalVoltage = static_cast<Bus*>(motor->GetParentList()[0])->GetElectricalData().voltage;

    // Calculate the induction machine transient constants at the machine base
    double r1t = data.r1 * k;
    double r2t = data.r2 * k;
    double x1t = data.x1 * k;
    double x2t = data.x2 * k;
    double xmt = data.xm * k;
    data.r1t = r1t;
    data.r2t = r2t;
    data.x1t = x1t;
    data.x2t = x2t;
    data.xmt = xmt;

    double xt = x1t + (x2t * xmt) / (x2t + xmt);
    double x0 = x1t + xmt;
    data.xt = xt;
    data.x0 = x0;

    double p = dataPU.activePower;
    double v = std::abs(data.terminalVoltage);

    //[Ref.] Induction Motor Static Models for Power Flow and Voltage stability Studies
    // If the motor is offline, calculate the nominal slip to user-defined power input and 1.0 p.u. voltage
    if(!motor->IsOnline()) v = 1.0;
    double r1 = data.r1t;
    double r2 = data.r2t;
    if(data.useKf) r2 *= (1.0 + data.kf * r2t);
    double x1 = data.x1t;
    double x2 = data.x2t;
    double xm = data.xmt;
    double k1 = x2 + xm;
    double k2 = -x1 * k1 - x2 * xm;
    double k3 = xm + x1;
    double k4 = r1 * k1;
    double a = p * (r1 * r1 + k3 * k3) - v * v * r1;
    double b = 2.0 * p * (r1 * k2 + k3 * k4) - v * v * (k2 + k1 * k3);
    double c = p * (k2 * k2 + k4 * k4) - v * v * k1 * k4;
    double d = (b * b - 4.0 * a * c);
    if(d < 0.0) {
        m_errorMsg = _("Error on initializate the slip of \"") + data.name + _("\".");
        return false;
    }
    double r2_s = (-b + std::sqrt(d)) / (2.0 * a);
    data.s0 = r2 / r2_s;

    double qa = k1 * (r2_s * r1 - x1 * k1 - x2 * xm);
    double qb = r2_s * (r2_s * (xm + x1) + r1 * k1);
    double qc = r2_s * r1 - x1 * k1 - x2 * xm;
    double qd = r2_s * (xm + x1) + r1 * k1;
    data.q0 = (-v * v * (qa - qb)) / (qc * qc + qd * qd);

    data.t0 = (x2t + xmt) / (2.0 * M_PI * m_systemFreq * r2);

    motor->SetElectricalData(data);
    return true;
}
