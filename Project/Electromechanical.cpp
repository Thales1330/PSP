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
    GetLUDecomposition(m_yBus, m_yBusL, m_yBusU);

    // Get buses voltages.
    wxString str = "";
    m_vBus.clear();
    m_vBus.resize(m_busList.size());
    for(auto it = m_busList.begin(), itEnd = m_busList.end(); it != itEnd; ++it) {
        Bus* bus = *it;
        auto data = bus->GetElectricalData();
        m_vBus[data.number] = data.voltage;
        str += wxString::Format("%f /_ %f\n", std::abs(m_vBus[data.number]),
                                std::arg(m_vBus[data.number]) * 180.0f / M_PI);
    }
    wxMessageBox(str);

    // Calculate injected currents
    str = "";
    m_iBus = ComplexMatrixTimesVector(m_yBus, m_vBus);
    for(unsigned int i = 0; i < m_iBus.size(); ++i) {
        if(std::abs(m_iBus[i]) < 1e-5) m_iBus[i] = std::complex<double>(0.0, 0.0);
        str += wxString::Format("%f /_ %f\n", std::abs(m_iBus[i]), std::arg(m_iBus[i]) * 180.0f / M_PI);
    }
    wxMessageBox(str);

    if(!InitializeDynamicElements()) return false;

    // test
    double simTime = 10.0;
    double printTime = 0.01;
    double currentTime = 0.0;
    double currentPrintTime = 0.0;
    while(currentTime <= simTime) {
        if(HasEvent(currentTime)) {
            SetEvent(currentTime);
            GetLUDecomposition(m_yBus, m_yBusL, m_yBusU);
        }

        if(currentPrintTime >= printTime) {
            m_timeVector.push_back(currentTime);
            SaveData();
            currentPrintTime = 0.0;
        }

        if(!SolveSynchronousMachines()) {
            wxMessageBox(wxString::Format("%f", currentTime));
            return false;
        }

        currentTime += m_timeStep;
        currentPrintTime += m_timeStep;
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

    // Inductor switching
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
        double oldBase = GetPowerValue(data.nominalPower, data.nominalPowerUnit);
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
    return (std::complex<double>(ra, -xdq) / std::complex<double>(ra * ra + xd * xq, 0.0));
}

bool Electromechanical::InitializeDynamicElements()
{
    // Synchronous generators
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        auto data = syncGenerator->GetPUElectricalData(m_powerSystemBase);
        if(syncGenerator->IsOnline()) {
            double k = 1.0;  // Power base change factor.
            if(data.useMachineBase) {
                double oldBase = GetPowerValue(data.nominalPower, data.nominalPowerUnit);
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

            // double teta0 = std::arg(data.terminalVoltage);
            // double vd0, vq0;
            // ABCtoDQ0(data.terminalVoltage, data.delta - teta0, vd0, vq0);
            // vq0 = std::abs(data.terminalVoltage) * cos(data.delta - teta0);
            // vd0 = -std::abs(data.terminalVoltage) * sin(data.delta - teta0);

            double fi0 = std::arg(ia);
            double id0, iq0;
            // ABCtoDQ0(ia, data.delta - fi0, id0, iq0);
            iq0 = std::abs(ia) * cos(data.delta - fi0);
            id0 = -std::abs(ia) * sin(data.delta - fi0);

            data.initialFieldVoltage = std::abs(eq0) - (xd - xq) * id0;
            data.fieldVoltage = data.initialFieldVoltage;
            data.pm = std::real((data.terminalVoltage * std::conj(ia)) + (std::abs(ia) * std::abs(ia) * ra));
            data.speed = 2.0 * M_PI * m_systemFreq;

            data.pe = data.pm;
            data.electricalPower = std::complex<double>(data.activePower, data.reactivePower);

            // data.pe = id0 * vd0 + iq0 * vq0 + (id0 * id0 + iq0 * iq0) * data.armResistance * k;
            // data.electricalPower = std::complex<double>(data.activePower, data.reactivePower);

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
        } else {
            // Initialize open circuit machine.
        }
        // Reset plot data
        data.terminalVoltageVector.clear();
        data.electricalPowerVector.clear();
        data.mechanicalPowerVector.clear();
        data.freqVector.clear();
        data.fieldVoltageVector.clear();
        data.deltaVector.clear();

        syncGenerator->SetElectricalData(data);
    }
    return true;
}

void Electromechanical::CalculateMachinesCurrents()
{
    // Reset injected currents vector
    for(unsigned int i = 0; i < m_iBus.size(); ++i) m_iBus[i] = std::complex<double>(0.0, 0.0);

    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        auto data = syncGenerator->GetPUElectricalData(m_powerSystemBase);
        if(syncGenerator->IsOnline()) {
            double k = 1.0;  // Power base change factor.
            if(data.useMachineBase) {
                double oldBase = GetPowerValue(data.nominalPower, data.nominalPowerUnit);
                k = m_powerSystemBase / oldBase;
            }

            double xd = 0.0;
            double xq = 0.0;
            double ra = data.armResistance * k;
            int n = static_cast<Bus*>(syncGenerator->GetParentList()[0])->GetElectricalData().number;
            std::complex<double> e = std::complex<double>(0.0, 0.0);
            std::complex<double> v = m_vBus[n];
            std::complex<double> iInj = m_iBus[n];
            double xdq = 0.0;

            switch(GetMachineModel(syncGenerator)) {
                case SM_MODEL_1:
                case SM_MODEL_2:
                case SM_MODEL_3: {
                    DQ0toABC(data.tranEd, data.tranEq, data.delta, e);
                    xd = data.transXd * k;
                    xq = data.transXq * k;
                } break;
                case SM_MODEL_4:
                case SM_MODEL_5: {
                    DQ0toABC(data.subEd, data.subEq, data.delta, e);
                    xd = data.subXd * k;
                    xq = data.subXq * k;
                } break;
            }
            xdq = 0.5 * (xd + xq);

            std::complex<double> y0 = std::complex<double>(ra, -xdq) / std::complex<double>(ra * ra + xd * xq, 0.0);
            std::complex<double> iUnadj = y0 * e;
            wxMessageBox(wxString::Format("%f / %f\n", std::real(iUnadj), std::imag(iUnadj)));

            double dVR = std::real(e) - std::real(v);
            double dVI = std::imag(e) - std::imag(v);

            double iAdjR = ((xd - xq) / (ra * ra + xd * xq)) *
                           (-std::sin(2.0 * data.delta) * dVR + std::cos(2.0 * data.delta) * dVI);
            double iAdjI = ((xd - xq) / (ra * ra + xd * xq)) *
                           (std::cos(2.0 * data.delta) * dVR + std::sin(2.0 * data.delta) * dVI);

            iInj = iUnadj + std::complex<double>(iAdjR, iAdjI);
            m_iBus[n] += iInj;

            std::complex<double> iMachine = iInj - y0 * v;

            //wxMessageBox(wxString::Format("%f /_ %f\n", std::real(data.electricalPower), std::imag(data.electricalPower)));
            data.electricalPower = v * std::conj(iMachine);
            //wxMessageBox(wxString::Format("%f /_ %f\n", std::real(data.electricalPower), std::imag(data.electricalPower)));
        } else {
            data.electricalPower = std::complex<double>(0.0, 0.0);
        }

        syncGenerator->SetElectricalData(data);
    }
}

void Electromechanical::CalculateIntegrationConstants(SyncGenerator* syncGenerator, double id, double iq)
{
    auto data = syncGenerator->GetPUElectricalData(m_powerSystemBase);

    double k = 1.0;  // Power base change factor.
    if(data.useMachineBase) {
        double oldBase = GetPowerValue(data.nominalPower, data.nominalPowerUnit);
        k = m_powerSystemBase / oldBase;
    }
    double w0 = 2.0f * M_PI * m_systemFreq;

    // Speed
    data.icSpeed.m = m_timeStep / ((4.0f * data.inertia / w0) / k + m_timeStep * data.damping * k);
    data.icSpeed.c = (1.0f - 2.0f * data.icSpeed.m * data.damping * k) * data.speed +
                     data.icSpeed.m * (data.pm - data.pe + 2.0f * w0 * data.damping * k);

    // Delta
    data.icDelta.m = 0.5f * m_timeStep;
    data.icDelta.c = data.delta + data.icDelta.m * (data.speed - 2.0f * w0);

    // Eq'
    data.icTranEq.m = m_timeStep / (2.0f * data.transTd0 + m_timeStep);
    data.icTranEq.c = (1.0f - 2.0 * data.icTranEq.m) * data.tranEq +
                      data.icTranEq.m * (data.fieldVoltage + (data.syncXd * k - data.transXd * k) * id);

    // Ed'
    data.icTranEd.m = m_timeStep / (2.0f * data.transTq0 + m_timeStep);
    data.icTranEd.c =
        (1.0f - 2.0f * data.icTranEd.m) * data.tranEd - data.icTranEd.m * (data.syncXq * k - data.transXq * k) * iq;

    // Eq''
    data.icSubEq.m = m_timeStep / (2.0f * data.subTd0 + m_timeStep);
    data.icSubEq.c = (1.0f - 2.0f * data.icSubEq.m) * data.subEq +
                     data.icSubEq.m * (data.tranEq + (data.transXd * k - data.subXd * k) * id);

    // Ed''
    data.icSubEd.m = m_timeStep / (2.0f * data.subTq0 + m_timeStep);
    data.icSubEd.c = (1.0f - 2.0f * data.icSubEd.m) * data.subEd +
                     data.icSubEd.m * (data.tranEd - (data.transXq * k - data.subXq * k) * iq);

    syncGenerator->SetElectricalData(data);
}

bool Electromechanical::SolveSynchronousMachines()
{
    double w0 = 2.0 * M_PI * m_systemFreq;

    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        auto data = syncGenerator->GetElectricalData();

        if(syncGenerator->IsOnline()) {
            int n = static_cast<Bus*>(syncGenerator->GetParentList()[0])->GetElectricalData().number;
            double id, iq;

            std::complex<double> iMachine = std::conj(data.electricalPower) / std::conj(m_vBus[n]);

            ABCtoDQ0(iMachine, data.delta, id, iq);

            // Calculate integration constants.
            CalculateIntegrationConstants(syncGenerator, id, iq);
        }
    }

    double error = 1.0;
    int iterations = 0;
    while(error > m_tolerance) {
        error = 0.0;

        // Calculate the injected currents.
        // wxMessageBox(wxString::Format("%f %f", m_iBus[0].real(), m_iBus[0].imag()));
        CalculateMachinesCurrents();
        // wxMessageBox(wxString::Format("%f %f", m_iBus[0].real(), m_iBus[0].imag()));

        // Calculate the buses voltages.
        m_vBus = LUEvaluate(m_yBusU, m_yBusL, m_iBus);

        // Solve machine equations.
        for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
            SyncGenerator* syncGenerator = *it;
            auto data = syncGenerator->GetElectricalData();

            if(syncGenerator->IsOnline()) {
                double k = 1.0;  // Power base change factor.
                if(data.useMachineBase) {
                    double oldBase = GetPowerValue(data.nominalPower, data.nominalPowerUnit);
                    k = m_powerSystemBase / oldBase;
                }
                int n = static_cast<Bus*>(syncGenerator->GetParentList()[0])->GetElectricalData().number;

                data.terminalVoltage = m_vBus[n];

                // Mechanical differential equations.
                double w = data.icSpeed.c + data.icSpeed.m * (data.pm - data.pe);
                error = std::max(error, std::abs(data.speed - w) / w0);

                double delta = data.icDelta.c + data.icDelta.m * w;
                error = std::max(error, std::abs(data.delta - delta));

                data.speed = w;
                data.delta = delta;

                // Electric power.
                double id, iq, vd, vq;
                std::complex<double> iMachine = std::conj(data.electricalPower) / std::conj(m_vBus[n]);

                ABCtoDQ0(iMachine, data.delta, id, iq);
                ABCtoDQ0(data.terminalVoltage, data.delta, vd, vq);

                double pe = id * vd + iq * vq + (id * id + iq * iq) * data.armResistance * k;
                //data.pe = (2 * pe - data.pe);  // Extrapolating Pe.
                data.pe = pe;  // Don't extrapolating Pe

                // Electrical differential equations
                switch(GetMachineModel(syncGenerator)) {
                    case SM_MODEL_1: {
                        // There is no differential equations.
                    } break;
                    case SM_MODEL_2: {
                    } break;
                    case SM_MODEL_3: {
                        double tranEq =
                            data.icTranEq.c +
                            data.icTranEq.m * (data.fieldVoltage + (data.syncXd * k - data.transXd * k) * id);
                        error = std::max(error, std::abs(data.tranEq - tranEq));

                        double tranEd = data.icTranEd.c - data.icTranEd.m * (data.syncXq * k - data.transXq * k) * iq;
                        // double tranEd = data.icTranEq.c - data.icTranEd.m * (data.syncXq * k - data.transXq * k) *
                        // id;
                        error = std::max(error, std::abs(data.tranEd - tranEd));

                        data.tranEq = tranEq;
                        data.tranEd = tranEd;

                    } break;
                    case SM_MODEL_4: {
                    } break;
                    case SM_MODEL_5: {
                    } break;
                }
            } else {
                // Set values to open circuit machine.
            }

            syncGenerator->SetElectricalData(data);
        }

        ++iterations;
        if(iterations > m_maxIterations) {
            m_errorMsg = _("Impossible to solve the synchronous generators.\nCheck the system parameters and/or "
                           "decrease the time step.");
            return false;
        }
    }

    // Solve controllers.
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        auto data = syncGenerator->GetElectricalData();
        if(data.useAVR && data.avrSolver) {
            data.avrSolver->SolveNextStep(std::abs(data.terminalVoltage));
            data.fieldVoltage = data.initialFieldVoltage + data.avrSolver->GetLastSolution();
        }

        if(data.useSpeedGovernor && data.speedGovSolver) {
            data.speedGovSolver->SolveNextStep(data.speed);
            data.pm = data.speedGovSolver->GetLastSolution();
        }
        syncGenerator->SetElectricalData(data);
    }

    return true;
}

double Electromechanical::GetPowerValue(double value, ElectricalUnit unit)
{
    switch(unit) {
        case UNIT_PU: {
            return value;
        } break;
        case UNIT_VA: {
            return value;
        } break;
        case UNIT_kVA: {
            return value * 1e3;
        } break;
        case UNIT_MVA: {
            return value * 1e6;
        } break;
        case UNIT_W: {
            return value;
        } break;
        case UNIT_kW: {
            return value * 1e3;
        } break;
        case UNIT_MW: {
            return value * 1e6;
        } break;
        case UNIT_VAr: {
            return value;
        } break;
        case UNIT_kVAr: {
            return value * 1e3;
        } break;
        case UNIT_MVAr: {
            return value * 1e6;
        } break;
        default: {
            return 0.0;
        } break;
    }
    return 0.0;
}

void Electromechanical::SaveData()
{
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        auto data = syncGenerator->GetElectricalData();
        if(data.plotSyncMachine) {
            data.terminalVoltageVector.push_back(data.terminalVoltage);
            data.electricalPowerVector.push_back(data.pe);
            data.mechanicalPowerVector.push_back(data.pm);
            data.freqVector.push_back(data.speed / (2.0f * M_PI));
            data.fieldVoltageVector.push_back(data.fieldVoltage);
            data.deltaVector.push_back(data.delta);
            syncGenerator->SetElectricalData(data);
        }
    }
}
