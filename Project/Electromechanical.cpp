#include "Electromechanical.h"
#include "ControlElementSolver.h"

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
}

Electromechanical::~Electromechanical() {}
bool Electromechanical::RunStabilityCalculation()
{
    wxProgressDialog pbd(_("Running simulation"), _("Initializing..."), 100, m_parent,
                         wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_CAN_ABORT | wxPD_SMOOTH);

    SetSyncMachinesModel();

    // Calculate the admittance matrix with the synchronous machines.
    if(!GetYBus(m_yBus, m_powerSystemBase, POSITIVE_SEQ, false, true)) {
        m_errorMsg = _("It was not possible to build the admittance matrix.");
        return false;
    }
    InsertSyncMachinesOnYBus();
    GetLUDecomposition(m_yBus, m_yBusL, m_yBusU);

    // Get buses voltages.
    m_vBus.clear();
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

    double pbdTime = m_plotTime;
    double currentTime = 0.0;
    double currentPlotTime = 0.0;
    double currentPbdTime = 0.0;
    while(currentTime < m_simTime) {
        if(HasEvent(currentTime)) {
            SetEvent(currentTime);
            GetLUDecomposition(m_yBus, m_yBusL, m_yBusU);
        }

        if(currentPlotTime >= m_plotTime || currentTime == 0.0) {
            m_timeVector.push_back(currentTime);
            SaveData();
            currentPlotTime = 0.0;
        }

        if(currentPbdTime > pbdTime) {
            if(!pbd.Update((currentTime / m_simTime) * 100, wxString::Format("Time = %.2fs", currentTime))) {
                m_errorMsg = wxString::Format(_("Simulation cancelled at %.2fs."), currentTime);
                pbd.Update(100);
                return false;
            }
            currentPbdTime = 0.0;
        }

        if(!SolveSynchronousMachines()) return false;

        currentTime += m_timeStep;
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
                    int n = static_cast<Bus*>(generator->GetParentList()[0])->GetElectricalData().number;
                    m_yBus[n][n] -= GetSyncMachineAdmittance(generator);
                }

                // Insert machine (only disconnected machines)
                if(swData.swType[i] == SW_INSERT && !generator->IsOnline() && generator->GetParentList().size() == 1) {
                    if(generator->SetOnline(true)) {
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
                    m_yBus[n][n] -= std::complex<double>(0.0, data.reactivePower);
                }

                // Insert capacitor (only disconnected capacitors)
                if(swData.swType[i] == SW_INSERT && !capacitor->IsOnline() && capacitor->GetParentList().size() == 1) {
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
                if(swData.swType[i] == SW_REMOVE && inductor->IsOnline()) {
                    inductor->SetOnline(false);
                    auto data = inductor->GetPUElectricalData(m_powerSystemBase);
                    int n = static_cast<Bus*>(inductor->GetParentList()[0])->GetElectricalData().number;
                    m_yBus[n][n] -= std::complex<double>(0.0, -data.reactivePower);
                }

                // Insert inductor (only disconnected inductors)
                if(swData.swType[i] == SW_INSERT && !inductor->IsOnline() && inductor->GetParentList().size() == 1) {
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
        bus->SetElectricalData(data);
    }
    // Synchronous generators
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        auto dataPU = syncGenerator->GetPUElectricalData(m_powerSystemBase);
        auto data = syncGenerator->GetElectricalData();
        if(syncGenerator->IsOnline()) {
            double k = 1.0;  // Power base change factor.
            if(data.useMachineBase) {
                double oldBase = syncGenerator->GetValueFromUnit(data.nominalPower, data.nominalPowerUnit);
                k = m_powerSystemBase / oldBase;
            }
            data.terminalVoltage = static_cast<Bus*>(syncGenerator->GetParentList()[0])->GetElectricalData().voltage;

            std::complex<double> conjS(dataPU.activePower, -dataPU.reactivePower);
            std::complex<double> conjV = std::conj(data.terminalVoltage);
            std::complex<double> ia = conjS / conjV;

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
            std::complex<double> eq0 = data.terminalVoltage + std::complex<double>(ra, xq) * ia;
            double delta = std::arg(eq0);

            double id0, iq0, vd0, vq0;
            ABCtoDQ0(ia, delta, id0, iq0);
            ABCtoDQ0(data.terminalVoltage, delta, vd0, vq0);

            // Initialize saturation
            double xqs = xq;
            double xds = xd;
            if(hasSaturation) {
                double oldDelta = 0;
                bool exit = false;
                int numIt = 0;
                while(!exit) {
                    oldDelta = delta;
                    // ABCtoDQ0(ia, delta, id0, iq0);
                    // ABCtoDQ0(data.terminalVoltage, delta, vd0, vq0);

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
            data.speed = 2.0 * M_PI * m_systemFreq;
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
                    // double tranXd = data.transXd * k;

                    // data.tranEq = data.initialFieldVoltage + (xd - tranXd) * id0;
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
                data.avrSolver = new ControlElementSolver(data.avr, m_timeStep * m_ctrlTimeStepMultiplier, m_tolerance,
                                                          false, std::abs(data.terminalVoltage), m_parent);
                if(!data.avrSolver->IsOK()) {
                    m_errorMsg = _("Error on initializate the AVR of \"") + data.name + _("\".");
                    syncGenerator->SetElectricalData(data);
                    return false;
                }
            }
            if(data.useSpeedGovernor) {
                if(data.speedGovSolver) delete data.speedGovSolver;
                data.speedGovSolver = new ControlElementSolver(data.speedGov, m_timeStep * m_ctrlTimeStepMultiplier,
                                                               m_tolerance, false, data.speed, m_parent);
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
    CalculateReferenceSpeed();
    return true;
}

bool Electromechanical::CalculateMachinesCurrents()
{
    // Reset injected currents vector
    for(unsigned int i = 0; i < m_iBus.size(); ++i) m_iBus[i] = std::complex<double>(0.0, 0.0);

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

            // Calculate the saturation effect
            if(data.satFactor != 0.0) {
                if(!CalculateSyncMachineSaturation(syncGenerator, false, k)) return false;
            }

            double sd = data.sd;
            double sq = data.sq;

            double xdq, xds, xqs, xdqs;
            xdq = 0.5 * (xd + xq);
            xds = (xd - xp) / sd + xp;
            xqs = (xq - xp) / sq + xp;
            xdqs = 0.5 * (xds + xqs);

            std::complex<double> y0 = std::complex<double>(ra, -xdq) / std::complex<double>(ra * ra + xd * xq, 0.0);
            // std::complex<double> iUnadjusted = y0 * e;
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

            double id, iq;
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
        // data.icTranEq.c = (1.0f - 2.0 * data.icTranEq.m) * data.tranEq +
        //                  data.icTranEq.m * (data.fieldVoltage + (syncXd - transXd) * id);
        data.icTranEq.c = (1.0 - data.icTranEq.m * (1.0 + data.sd)) * data.tranEq +
                          data.icTranEq.m * (data.fieldVoltage + (syncXd - transXd) * id);
    }

    // Ed'
    if(data.model == Machines::SM_MODEL_3 || data.model == Machines::SM_MODEL_4 || data.model == Machines::SM_MODEL_5) {
        data.icTranEd.m = m_timeStep / (2.0f * transTq0 + m_timeStep);
        // data.icTranEd.c = (1.0f - 2.0f * data.icTranEd.m) * data.tranEd - data.icTranEd.m * (syncXq - transXq) * iq;
        data.icTranEd.c =
            (1.0 - data.icTranEd.m * (1.0 + data.sq)) * data.tranEd - data.icTranEd.m * (syncXq - transXq) * iq;
    }

    // Eq''
    if(data.model == Machines::SM_MODEL_4 || data.model == Machines::SM_MODEL_5) {
        data.icSubEq.m = m_timeStep / (2.0f * subTd0 + m_timeStep);
        // data.icSubEq.c =
        //    (1.0f - 2.0f * data.icSubEq.m) * data.subEq + data.icSubEq.m * (data.tranEq + (transXd - subXd) * id);
        data.icSubEq.c = (1.0 - data.icSubEq.m * (1.0 + data.sd)) * data.subEq +
                         data.icSubEq.m * (data.sd * data.tranEq + (transXd - subXd) * id);
    }
    // Ed''
    if(data.model == Machines::SM_MODEL_4) {
        data.icSubEd.m = m_timeStep / (2.0f * subTq0 + m_timeStep);
        // data.icSubEd.c = (1.0f - 2.0f * data.icSubEd.m) * data.subEd - data.icSubEd.m * (syncXq - subXq) * iq;
        data.icSubEd.c =
            (1.0f - data.icSubEd.m * (1.0 + data.sq)) * data.subEd - data.icSubEd.m * (syncXq - subXq) * iq;
    }
    if(data.model == Machines::SM_MODEL_5) {
        data.icSubEd.m = m_timeStep / (2.0f * subTq0 + m_timeStep);
        // data.icSubEd.c =
        //    (1.0f - 2.0f * data.icSubEd.m) * data.subEd + data.icSubEd.m * (data.tranEd - (transXq - subXq) * iq);
        data.icSubEd.c = (1.0f - data.icSubEd.m * (1.0 + data.sq)) * data.subEd +
                         data.icSubEd.m * (data.sq * data.tranEd - (transXq - subXq) * iq);
    }

    syncGenerator->SetElectricalData(data);
}

bool Electromechanical::SolveSynchronousMachines()
{
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        auto data = syncGenerator->GetElectricalData();

        if(syncGenerator->IsOnline()) {
            double id, iq, pe, sd, sq;

            double k = 1.0;  // Power base change factor.
            if(data.useMachineBase) {
                double oldBase = syncGenerator->GetValueFromUnit(data.nominalPower, data.nominalPowerUnit);
                k = m_powerSystemBase / oldBase;
            }
            id = data.id;
            iq = data.iq;

            // Calculate integration constants.
            CalculateIntegrationConstants(syncGenerator, id, iq, k);

            if(!CalculateSyncMachineNonIntVariables(syncGenerator, id, iq, sd, sq, pe, k)) return false;
            // Extrapolate nonintegrable variables.
            id = 2.0 * id - data.oldId;
            iq = 2.0 * iq - data.oldIq;
            pe = 2.0 * pe - data.oldPe;
            sd = 2.0 * sd - data.oldSd;
            sq = 2.0 * sq - data.oldSq;

            CalculateSyncMachineIntVariables(syncGenerator, id, iq, sd, sq, pe, k);
        } else {
            CalculateIntegrationConstants(syncGenerator, 0.0f, 0.0f);
        }
    }

    m_wError = 0;

    double error = 1.0;
    int iterations = 0;
    while(error > m_tolerance) {
        error = 0.0;

        // Calculate the injected currents.
        if(!CalculateMachinesCurrents()) return false;

        // Calculate the buses voltages.
        m_vBus = LUEvaluate(m_yBusU, m_yBusL, m_iBus);

        // Solve machine equations.
        for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
            SyncGenerator* syncGenerator = *it;

            auto data = syncGenerator->GetElectricalData();

            double id, iq, pe, sd, sq;
            double k = 1.0;  // Power base change factor.
            if(data.useMachineBase) {
                double oldBase = syncGenerator->GetValueFromUnit(data.nominalPower, data.nominalPowerUnit);
                k = m_powerSystemBase / oldBase;
            }

            // Calculate id, iq, dq, sd
            if(!CalculateSyncMachineNonIntVariables(syncGenerator, id, iq, sd, sq, pe, k)) return false;
            if(data.satFactor != 0.0) {
                m_sdC = sd;
                m_sqC = sq;
            }

            double genError = CalculateSyncMachineIntVariables(syncGenerator, id, iq, sd, sq, pe, k);

            if(genError > error) error = genError;
        }

        ++iterations;

        if(iterations > m_maxIterations) {
            m_errorMsg = _("Impossible to solve the synchronous generators.\nCheck the system parameters and/or "
                           "decrease the time step.");
            return false;
        }
    }
    m_numIt = iterations;

    // Solve controllers.
    int ctrlRatio = static_cast<int>(1 / m_ctrlTimeStepMultiplier);
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        auto data = syncGenerator->GetElectricalData();
        if(data.useAVR && data.avrSolver) {
            for(int i = 0; i < ctrlRatio; ++i) data.avrSolver->SolveNextStep(std::abs(data.terminalVoltage));
            data.fieldVoltage = data.initialFieldVoltage + data.avrSolver->GetLastSolution();
        }

        if(data.useSpeedGovernor && data.speedGovSolver) {
            for(int i = 0; i < ctrlRatio; ++i) data.speedGovSolver->SolveNextStep(data.speed);
            data.pm = data.speedGovSolver->GetLastSolution();
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
        if(data.plotSyncMachine) {
            data.terminalVoltageVector.push_back(data.terminalVoltage);
            data.electricalPowerVector.push_back(data.electricalPower);
            data.mechanicalPowerVector.push_back(data.pm);
            data.freqVector.push_back(data.speed / (2.0f * M_PI));
            data.fieldVoltageVector.push_back(data.fieldVoltage);
            data.deltaVector.push_back(wxRadToDeg(data.delta));
            syncGenerator->SetElectricalData(data);
        }
    }
    for(auto it = m_busList.begin(), itEnd = m_busList.end(); it != itEnd; ++it) {
        Bus* bus = *it;
        auto data = bus->GetElectricalData();
        if(data.plotBus) {
            data.stabVoltageVector.push_back(m_vBus[data.number]);
            bus->SetElectricalData(data);
        }
    }

    m_wErrorVector.push_back(m_wError);
    m_numItVector.push_back(m_numIt);
    m_sdCVector.push_back(m_sdC);
    m_sqCVector.push_back(m_sqC);
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

bool Electromechanical::CalculateSyncMachineNonIntVariables(SyncGenerator* syncGenerator,
                                                            double& id,
                                                            double& iq,
                                                            double& sd,
                                                            double& sq,
                                                            double& pe,
                                                            double k)
{
    auto data = syncGenerator->GetElectricalData();
    int n = static_cast<Bus*>(syncGenerator->GetParentList()[0])->GetElectricalData().number;

    if(syncGenerator->IsOnline()) {
        data.terminalVoltage = m_vBus[n];
    }

    double vd, vq;
    ABCtoDQ0(data.terminalVoltage, data.delta, vd, vq);
    if(data.satFactor != 0.0) {
        if(!CalculateSyncMachineSaturation(syncGenerator, k)) return false;
    }
    sd = data.sd;
    sq = data.sq;
    id = data.id;
    iq = data.iq;

    if(syncGenerator->IsOnline()) {
        pe = id * vd + iq * vq + (id * id + iq * iq) * data.armResistance * k;
    } else {
        pe = id = iq = 0.0f;
    }
    data.pe = pe;
    data.sd = sd;
    data.sq = sq;
    data.id = id;
    data.iq = iq;
    data.oldPe = pe;
    data.oldId = id;
    data.oldIq = iq;
    data.oldSd = sd;
    data.oldSq = sq;
    syncGenerator->SetElectricalData(data);

    return true;
}

double Electromechanical::CalculateSyncMachineIntVariables(SyncGenerator* syncGenerator,
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

    m_wError += std::abs(data.speed - w) / m_refSpeed;

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
                (data.icSubEd.c - data.icSubEd.m * ((syncXq - subXq) * iq)) / (1.0 + data.icSubEd.m * (data.sq - 1.0));
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

bool Electromechanical::CalculateSyncMachineSaturation(SyncGenerator* syncMachine, bool updateCurrents, double k)
{
    // [Ref] Arrillaga, J.; Arnold, C. P.. "Computer Modelling of Electrical Power Systems". Pg. 254-260
    auto data = syncMachine->GetElectricalData();
    auto smDataModel = GetSyncMachineModelData(syncMachine);

    int n = static_cast<Bus*>(syncMachine->GetParentList()[0])->GetElectricalData().number;
    if(syncMachine->IsOnline()) {
        data.terminalVoltage = m_vBus[n];
    }

    double vd, vq;
    ABCtoDQ0(data.terminalVoltage, data.delta, vd, vq);
    double deltaVd = smDataModel.ed - vd;
    double deltaVq = smDataModel.eq - vq;
    // wxMessageBox(wxString::Format("Vd = %.13f\nEd = %.13f\nVq = %.13f\nEq = %.13f", vd, smDataModel.ed, vq,
    // smDataModel.eq));

    double id, iq;
    id = data.id;
    iq = data.iq;

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
    double sd = data.sd;
    double sq = data.sq;
    double satFacd = (data.satFactor - 1.2) / std::pow(1.2, 7);
    double satFacq = satFacd * (syncXq / syncXd);

    bool exit = false;
    int iterations = 0;
    // wxMessageBox(wxString::Format("%.13f\n%.13f", deltaVd, deltaVq));
    while(!exit) {
        double oldSd = sd;
        double oldSq = sq;

        // Saturated reactances.
        double xds = (xd - xp) / sd + xp;
        double xqs = (xq - xp) / sq + xp;
        // dq currents.
        double den = 1.0 / (ra * ra + xds * xqs);
        iq = den * (ra * deltaVq + xds * deltaVd);
        id = den * (-xqs * deltaVq + ra * deltaVd);
        // Potier voltages
        double epq = vq + ra * iq - xp * id;
        double epd = vd + ra * id + xp * iq;
        // Saturation factors.

        // Gauss
        // sd = 1.0 + satFacd * std::pow(epq, 6);
        // sq = 1.0 + satFacq * std::pow(epd, 6);

        // Newton-raphson
        double f1 = 1.0 - sd + satFacd * std::pow(epq, 6);
        double f2 = 1.0 - sq + satFacq * std::pow(epd, 6);
        double dF1dSd = (6.0 * satFacd * std::pow(epq, 5) * xp * (xd - xp) * deltaVq) / ((sd - 1.0) * xp + xd) - 1.0;
        double dF2dSq = (6.0 * satFacq * std::pow(epd, 5) * xp * (xq - xp) * deltaVd) / ((sq - 1.0) * xp + xq) - 1.0;

        sd = sd - f1 / dF1dSd;
        sq = sq - f2 / dF2dSq;

        double error = std::abs(sd - oldSd) + std::abs(sq - oldSq);
        if(error < m_saturationTolerance) exit = true;

        iterations++;
        if((iterations >= m_maxIterations) & !exit) {
            m_errorMsg =
                _("It was not possible to solve the saturation of the synchronous machine \"") + data.name + wxT("\".");
            return false;
        }
    }

    data.sd = sd;
    data.sq = sq;
    if(updateCurrents) {
        data.id = id;
        data.iq = iq;
    }
    syncMachine->SetElectricalData(data);
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
                if(smModelData.xq == 0.0) {
                    smModelData.xq = data.syncXd * k;
                }
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
