#include "Electromechanical.h"
#include "ControlElementSolver.h"

Electromechanical::Electromechanical(wxWindow* parent, std::vector<Element*> elementList)
{
    m_parent = parent;
    GetElementsFromList(elementList);
    SetEventTimeList();
}

Electromechanical::~Electromechanical() {}
bool Electromechanical::RunStabilityCalculation()
{
    // Calculate the admittance matrix with the synchronous machines.
    if(!GetYBus(m_yBus, m_powerSystemBase, POSITIVE_SEQ, false, true)) {
        m_errorMsg = _("It was not possible to build the admittance matrix.");
        return false;
    }
    InsertSyncMachinesOnYBus();

    if(!InitializeDynamicElements()) return false;

    // test
    double simTime = 10.0;
    double currentTime = 0.0;
    while(currentTime <= simTime) {
        if(HasEvent(currentTime)) {
            SetEvent(currentTime);
        }
        currentTime += m_timeStep;
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
            m_eventTimeList.push_back(data.stabFaultTime);
            m_eventOccurrenceList.push_back(false);
            m_eventTimeList.push_back(data.stabFaultTime + data.stabFaultLength);
            m_eventOccurrenceList.push_back(false);
        }
    }
    // Switching
    for(auto it = m_powerElementList.begin(), itEnd = m_powerElementList.end(); it != itEnd; ++it) {
        PowerElement* element = *it;
        SwitchingData swData = element->GetSwitchingData();
        for(unsigned int i = 0; i < swData.swTime.size(); ++i) {
            m_eventTimeList.push_back(swData.swTime[i]);
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
                if(swData.swType[i] == SW_REMOVE && generator->IsOnline()) {
                    generator->SetOnline(false);
                    auto data = generator->GetPUElectricalData(m_powerSystemBase);
                    int n = static_cast<Bus*>(generator->GetParentList()[0])->GetElectricalData().number;
                    m_yBus[n][n] -= GetSyncMachineAdmittance(generator);
                }

                // Insert machine (only disconnected machines)
                if(swData.swType[i] == SW_INSERT && !generator->IsOnline() && generator->GetParentList().size() == 1) {
                    if(generator->SetOnline(true)) {
                        auto data = generator->GetPUElectricalData(m_powerSystemBase);
                        int n = static_cast<Bus*>(generator->GetParentList()[0])->GetElectricalData().number;
                        m_yBus[n][n] += GetSyncMachineAdmittance(generator);
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
                if(swData.swType[i] == SW_REMOVE && load->IsOnline()) {
                    load->SetOnline(false);
                    auto data = load->GetPUElectricalData(m_powerSystemBase);
                    Bus* parentBus = static_cast<Bus*>(load->GetParentList()[0]);
                    int n = parentBus->GetElectricalData().number;
                    std::complex<double> v = parentBus->GetElectricalData().voltage;
                    m_yBus[n][n] -= std::complex<double>(data.activePower, -data.reactivePower) / (v * v);
                }

                // Insert load (only disconnected load)
                if(swData.swType[i] == SW_INSERT && !load->IsOnline() && load->GetParentList().size() == 1) {
                    if(load->SetOnline(true)) {
                        auto data = load->GetPUElectricalData(m_powerSystemBase);
                        Bus* parentBus = static_cast<Bus*>(load->GetParentList()[0]);
                        int n = parentBus->GetElectricalData().number;
                        std::complex<double> v = parentBus->GetElectricalData().voltage;
                        m_yBus[n][n] += std::complex<double>(data.activePower, -data.reactivePower) / (v * v);
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
                if(swData.swType[i] == SW_REMOVE && line->IsOnline()) {
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
                if(swData.swType[i] == SW_INSERT && !line->IsOnline() && line->GetParentList().size() == 2) {
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
                if(swData.swType[i] == SW_REMOVE && transformer->IsOnline()) {
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
                if(swData.swType[i] == SW_INSERT && !transformer->IsOnline() &&
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
                if(swData.swType[i] == SW_REMOVE && capacitor->IsOnline()) {
                    capacitor->SetOnline(false);
                    auto data = capacitor->GetPUElectricalData(m_powerSystemBase);
                    int n = static_cast<Bus*>(capacitor->GetParentList()[0])->GetElectricalData().number;
                    m_yBus[n][n] += std::complex<double>(0.0, data.reactivePower);
                }

                // Insert capacitor (only disconnected capacitors)
                if(swData.swType[i] == SW_INSERT && !capacitor->IsOnline() && capacitor->GetParentList().size() == 1) {
                    if(capacitor->SetOnline(true)) {
                        auto data = capacitor->GetPUElectricalData(m_powerSystemBase);
                        int n = static_cast<Bus*>(capacitor->GetParentList()[0])->GetElectricalData().number;
                        m_yBus[n][n] -= std::complex<double>(0.0, data.reactivePower);
                    }
                }
            }
        }
    }

    // Capacitor switching
    for(auto it = m_inductorList.begin(), itEnd = m_inductorList.end(); it != itEnd; ++it) {
        Inductor* inductor = *it;
        auto swData = inductor->GetSwitchingData();
        for(unsigned int i = 0; i < swData.swType.size(); ++i) {
            if(EventTrigger(swData.swTime[i], currentTime)) {
                // Remove inductor (only connected inductors)
                if(swData.swType[i] == SW_REMOVE && inductor->IsOnline()) {
                    inductor->SetOnline(false);
                    auto data = inductor->GetPUElectricalData(m_powerSystemBase);
                    int n = static_cast<Bus*>(inductor->GetParentList()[0])->GetElectricalData().number;
                    m_yBus[n][n] += std::complex<double>(0.0, -data.reactivePower);
                }

                // Insert inductor (only disconnected inductors)
                if(swData.swType[i] == SW_INSERT && !inductor->IsOnline() && inductor->GetParentList().size() == 1) {
                    if(inductor->SetOnline(true)) {
                        auto data = inductor->GetPUElectricalData(m_powerSystemBase);
                        int n = static_cast<Bus*>(inductor->GetParentList()[0])->GetElectricalData().number;
                        m_yBus[n][n] -= std::complex<double>(0.0, -data.reactivePower);
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
    auto data = generator->GetPUElectricalData(m_powerSystemBase);
    double k = 1.0;  // Power base change factor.
    if(data.useMachineBase) {
        double oldBase = data.nominalPower * std::pow(1000.0f, data.nominalPowerUnit);
        k = m_powerSystemBase / oldBase;
    }

    double xd = 0.0;
    double xq = 0.0;
    double ra = data.armResistance * k;

    switch(GetMachineModel(generator)) {
        case SM_MODEL_1:
        case SM_MODEL_2:
        case SM_MODEL_3: {
            xd = data.transXd * k;
            xq = data.transXq * k;
            break;
        }
        case SM_MODEL_4:
        case SM_MODEL_5: {
            xd = data.subXd * k;
            xq = data.subXq * k;
            break;
        }
    }
    double xdq = 0.5 * (xd + xq);
    return std::complex<double>(ra, -xdq) / std::complex<double>(ra * ra + xd * xq, 0.0);
}

bool Electromechanical::InitializeDynamicElements()
{
    // Synchronous generators
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        if(syncGenerator->IsOnline()) {
            auto data = syncGenerator->GetPUElectricalData(m_powerSystemBase);
            double k = 1.0;  // Power base change factor.
            if(data.useMachineBase) {
                double oldBase = data.nominalPower * std::pow(1000.0f, data.nominalPowerUnit);
                k = m_powerSystemBase / oldBase;
            }
            data.terminalVoltage = static_cast<Bus*>(syncGenerator->GetParentList()[0])->GetElectricalData().voltage;

            std::complex<double> conjS(data.activePower, -data.reactivePower);
            std::complex<double> conjV = std::conj(data.terminalVoltage);
            std::complex<double> ia = conjS / conjV;

            double xd = data.syncXd * k;
            double xq = data.syncXq * k;
            double ra = data.armResistance * k;

            // Initialize state variables
            std::complex<double> eq0 = data.terminalVoltage + std::complex<double>(ra, xq) * ia;
            data.delta = std::arg(eq0);

            double teta0 = std::arg(data.terminalVoltage);
            double vd0, vq0;
            ABCtoDQ0(data.terminalVoltage, data.delta - teta0, vd0, vq0);

            double fi0 = std::arg(ia);
            double id0, iq0;
            ABCtoDQ0(ia, data.delta - fi0, id0, iq0);

            data.initialFieldVoltage = std::abs(eq0) - (xd - xq) * id0;
            data.pm = std::real((data.terminalVoltage * std::conj(ia)) + (std::abs(ia) * std::abs(ia) * ra));
            data.speed = 2.0 * M_PI * 60.0;

            switch(GetMachineModel(syncGenerator)) {
                case SM_MODEL_1: {
                    data.tranEq = 0.0;
                    data.tranEd = 0.0;
                    data.subEq = 0.0;
                    data.subEd = 0.0;
                } break;
                case SM_MODEL_2: {
                    double tranXd = data.transXd * k;

                    data.tranEq = data.initialFieldVoltage + (xd - tranXd) * id0;
                    data.tranEd = 0.0;
                    data.subEd = 0.0;
                    data.subEq = 0.0;
                } break;
                case SM_MODEL_3: {
                    double tranXd = data.transXd * k;
                    double tranXq = data.transXq * k;

                    data.tranEq = data.initialFieldVoltage + (xd - tranXd) * id0;
                    data.tranEd = -(xq - tranXq) * iq0;
                    data.subEd = 0.0;
                    data.subEq = 0.0;
                } break;
                case SM_MODEL_4: {
                    double tranXd = data.transXd * k;
                    double subXd = data.subXd * k;
                    double subXq = data.subXq * k;

                    data.tranEq = data.initialFieldVoltage + (xd - tranXd) * id0;
                    data.tranEd = 0.0;
                    data.subEq = data.tranEq + (tranXd - subXd) * id0;
                    data.subEd = -(xq - subXq) * iq0;
                } break;
                case SM_MODEL_5: {
                    double tranXd = data.transXd * k;
                    double tranXq = data.transXq * k;
                    double subXd = data.subXd * k;
                    double subXq = data.subXq * k;

                    data.tranEq = data.initialFieldVoltage + (xd - tranXd) * id0;
                    data.tranEd = -(xq - tranXq) * iq0;
                    data.subEq = data.tranEq + (tranXd - subXd) * id0;
                    data.subEd = data.tranEd + (tranXq - subXq) * iq0;
                } break;
                default: {
                    break;
                }
            }

            // Initialize controllers
            if(data.useAVR) {
                if(data.avrSolver) delete data.avrSolver;
                data.avrSolver = new ControlElementSolver(data.avr, m_timeStep, 1e-3, false,
                                                          std::abs(data.terminalVoltage), m_parent);
                if(!data.avrSolver->IsOK()) {
                    m_errorMsg = _("Error on initializate the AVR of \"") + data.name + _("\".");
                    syncGenerator->SetElectricalData(data);
                    return false;
                }
            }
            if(data.useSpeedGovernor) {
                if(data.speedGovSolver) delete data.speedGovSolver;
                data.speedGovSolver =
                    new ControlElementSolver(data.speedGov, m_timeStep, 1e-3, false, data.speed, m_parent);
                if(!data.speedGovSolver->IsOK()) {
                    m_errorMsg = _("Error on initializate the speed governor of \"") + data.name + _("\".");
                    syncGenerator->SetElectricalData(data);
                    return false;
                }
            }

            syncGenerator->SetElectricalData(data);
        }
    }
    return true;
}
