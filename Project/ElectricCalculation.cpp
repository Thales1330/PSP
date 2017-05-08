#include "ElectricCalculation.h"
#ifdef USING_WX_3_0_X
#include "DegreesAndRadians.h"
#endif

ElectricCalculation::ElectricCalculation() {}
ElectricCalculation::~ElectricCalculation() {}
void ElectricCalculation::GetElementsFromList(std::vector<Element*> elementList)
{
    m_busList.clear();
    m_capacitorList.clear();
    m_indMotorList.clear();
    m_inductorList.clear();
    m_lineList.clear();
    m_loadList.clear();
    m_syncGeneratorList.clear();
    m_syncMotorList.clear();
    m_transformerList.clear();
    // TODO: Bad design?
    for(auto it = elementList.begin(); it != elementList.end(); it++) {
        Element* element = *it;
        if(Bus* bus = dynamic_cast<Bus*>(element))
            m_busList.push_back(bus);
        else if(Capacitor* capacitor = dynamic_cast<Capacitor*>(element))
            m_capacitorList.push_back(capacitor);
        else if(IndMotor* indMotor = dynamic_cast<IndMotor*>(element))
            m_indMotorList.push_back(indMotor);
        else if(Inductor* inductor = dynamic_cast<Inductor*>(element))
            m_inductorList.push_back(inductor);
        else if(Line* line = dynamic_cast<Line*>(element))
            m_lineList.push_back(line);
        else if(Load* load = dynamic_cast<Load*>(element))
            m_loadList.push_back(load);
        else if(SyncGenerator* syncGenerator = dynamic_cast<SyncGenerator*>(element))
            m_syncGeneratorList.push_back(syncGenerator);
        else if(SyncMotor* syncMotor = dynamic_cast<SyncMotor*>(element))
            m_syncMotorList.push_back(syncMotor);
        else if(Transformer* transformer = dynamic_cast<Transformer*>(element))
            m_transformerList.push_back(transformer);
    }
}

bool ElectricCalculation::GetYBus(std::vector<std::vector<std::complex<double> > >& yBus,
    double systemPowerBase,
    YBusSequence sequence,
    bool includeSyncMachines)
{
    if(m_busList.size() == 0) return false;

    // Clear and fill with zeros the Ybus
    yBus.clear();
    for(int i = 0; i < (int)m_busList.size(); i++) {
        std::vector<std::complex<double> > line;
        for(int j = 0; j < (int)m_busList.size(); j++) {
            line.push_back(std::complex<double>(0.0, 0.0));
        }
        yBus.push_back(line);
    }

    // Set buses numbers
    int busNumber = 0;
    for(auto itb = m_busList.begin(); itb != m_busList.end(); itb++) {
        Bus* bus = *itb;
        BusElectricalData data = bus->GetEletricalData();
        data.number = busNumber;
        bus->SetElectricalData(data);
        busNumber++;
    }

    // Load
    for(auto it = m_loadList.begin(), itEnd = m_loadList.end(); it != itEnd; ++it) {
        Load* load = *it;
        if(load->IsOnline()) {
            int n = static_cast<Bus*>(load->GetParentList()[0])->GetEletricalData().number;
            LoadElectricalData data = load->GetPUElectricalData(systemPowerBase);
            if(data.loadType == CONST_IMPEDANCE)
                yBus[n][n] += std::complex<double>(data.activePower, -data.reactivePower);
        }
    }

    // Capacitor
    for(auto it = m_capacitorList.begin(), itEnd = m_capacitorList.end(); it != itEnd; ++it) {
        Capacitor* capacitor = *it;
        if(capacitor->IsOnline()) {
            int n = static_cast<Bus*>(capacitor->GetParentList()[0])->GetEletricalData().number;
            CapacitorElectricalData data = capacitor->GetPUElectricalData(systemPowerBase);
            yBus[n][n] += std::complex<double>(0.0, data.reactivePower);
        }
    }

    // Inductor
    for(auto it = m_inductorList.begin(), itEnd = m_inductorList.end(); it != itEnd; ++it) {
        Inductor* inductor = *it;
        if(inductor->IsOnline()) {
            int n = static_cast<Bus*>(inductor->GetParentList()[0])->GetEletricalData().number;
            InductorElectricalData data = inductor->GetPUElectricalData(systemPowerBase);
            yBus[n][n] += std::complex<double>(0.0, -data.reactivePower);
        }
    }

    // Power line
    for(auto it = m_lineList.begin(), itEnd = m_lineList.end(); it != itEnd; ++it) {
        Line* line = *it;
        if(line->IsOnline()) {
            LineElectricalData data = line->GetElectricalData();

            int n1 = static_cast<Bus*>(line->GetParentList()[0])->GetEletricalData().number;
            int n2 = static_cast<Bus*>(line->GetParentList()[1])->GetEletricalData().number;

            switch(sequence) {
                case POSITIVE_SEQ:
                case NEGATIVE_SEQ: {
                    yBus[n1][n2] -= 1.0 / std::complex<double>(data.resistance, data.indReactance);
                    yBus[n2][n1] -= 1.0 / std::complex<double>(data.resistance, data.indReactance);

                    yBus[n1][n1] += 1.0 / std::complex<double>(data.resistance, data.indReactance);
                    yBus[n2][n2] += 1.0 / std::complex<double>(data.resistance, data.indReactance);

                    yBus[n1][n1] += std::complex<double>(0.0, data.capSusceptance / 2.0);
                    yBus[n2][n2] += std::complex<double>(0.0, data.capSusceptance / 2.0);
                } break;
                case ZERO_SEQ: {
                    yBus[n1][n2] -= 1.0 / std::complex<double>(data.zeroResistance, data.zeroIndReactance);
                    yBus[n2][n1] -= 1.0 / std::complex<double>(data.zeroResistance, data.zeroIndReactance);

                    yBus[n1][n1] += 1.0 / std::complex<double>(data.zeroResistance, data.zeroIndReactance);
                    yBus[n2][n2] += 1.0 / std::complex<double>(data.zeroResistance, data.zeroIndReactance);

                    yBus[n1][n1] += std::complex<double>(0.0, data.zeroCapSusceptance / 2.0);
                    yBus[n2][n2] += std::complex<double>(0.0, data.zeroCapSusceptance / 2.0);
                }
            }
        }
    }

    // Transformer
    for(auto it = m_transformerList.begin(), itEnd = m_transformerList.end(); it != itEnd; ++it) {
        Transformer* transformer = *it;

        if(transformer->IsOnline()) {
            TransformerElectricalData data = transformer->GetElectricalData();

            int n1 = static_cast<Bus*>(transformer->GetParentList()[0])->GetEletricalData().number;
            int n2 = static_cast<Bus*>(transformer->GetParentList()[1])->GetEletricalData().number;

            // If the transformer have nominal turns ratio (1.0) and no phase shifting, it will be modelled as series
            // impedance.
            if(data.turnsRatio == 1.0 && data.phaseShift == 0.0 && sequence != ZERO_SEQ) {
                yBus[n1][n2] += -1.0 / std::complex<double>(data.resistance, data.indReactance);
                yBus[n2][n1] += -1.0 / std::complex<double>(data.resistance, data.indReactance);

                yBus[n1][n1] += 1.0 / std::complex<double>(data.resistance, data.indReactance);
                yBus[n2][n2] += 1.0 / std::complex<double>(data.resistance, data.indReactance);
            }
            // If the transformer have no-nominal turn ratio and/or phase shifting, it will be modelled in a different
            // way (see references).
            //[Ref. 1: Elementos de analise de sistemas de potencia - Stevenson - pg. 232]
            //[Ref. 2: http://www.ee.washington.edu/research/real/Library/Reports/Tap_Adjustments_in_AC_Load_Flows.pdf]
            //[Ref. 3: http://www.columbia.edu/~dano/courses/power/notes/power/andersson1.pdf]
            else if(sequence != ZERO_SEQ) {
                // Complex turns ratio
                double radPhaseShift = wxDegToRad(data.phaseShift);
                std::complex<double> a = std::complex<double>(
                    data.turnsRatio * std::cos(radPhaseShift), -data.turnsRatio * std::sin(radPhaseShift));

                // Transformer admitance
                std::complex<double> y = 1.0 / std::complex<double>(data.resistance, data.indReactance);

                if(sequence == POSITIVE_SEQ) {
                    yBus[n1][n1] += y / std::pow(std::abs(a), 2.0);
                    yBus[n1][n2] += -(y / std::conj(a));
                    yBus[n2][n1] += -(y / a);
                    yBus[n2][n2] += y;
                } else if(sequence == NEGATIVE_SEQ) {
                    yBus[n1][n1] += y / std::pow(std::abs(a), 2.0);
                    yBus[n1][n2] += -(y / a);
                    yBus[n2][n1] += -(y / std::conj(a));
                    yBus[n2][n2] += y;
                }
            } else if(sequence == ZERO_SEQ) {
                switch(data.connection) {
                    case GWYE_GWYE: {
                        std::complex<double> y =
                            1.0 / std::complex<double>(data.zeroResistance +
                                          3.0 * (data.primaryGrndResistance + data.secondaryGrndResistance),
                                      data.zeroIndReactance +
                                          3.0 * (data.primaryGrndReactance + data.secondaryGrndReactance));
                        std::complex<double> a = std::complex<double>(data.turnsRatio, 0.0);

                        yBus[n1][n1] += y / (a * a);
                        yBus[n1][n2] += -(y / a);
                        yBus[n2][n1] += -(y / a);
                        yBus[n2][n2] += y;
                    } break;
                    case DELTA_GWYE: {
                        std::complex<double> y =
                            1.0 / std::complex<double>(data.zeroResistance + 3.0 * (data.secondaryGrndResistance),
                                      data.zeroIndReactance + 3.0 * (data.secondaryGrndReactance));
                        yBus[n2][n2] += y;
                        break;
                    }
                    case GWYE_DELTA: {
                        std::complex<double> y =
                            1.0 / std::complex<double>(data.zeroResistance + 3.0 * (data.primaryGrndResistance),
                                      data.zeroIndReactance + 3.0 * (data.primaryGrndReactance));
                        yBus[n1][n1] += y;
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }

    if(includeSyncMachines) {
        // Synchronous generator
        for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
            SyncGenerator* syncGenerator = *it;
            if(syncGenerator->IsOnline()) {
                int n = static_cast<Bus*>(syncGenerator->GetParentList()[0])->GetEletricalData().number;
                SyncGeneratorElectricalData data = syncGenerator->GetPUElectricalData(systemPowerBase);
                switch(sequence) {
                    case POSITIVE_SEQ: {
                        yBus[n][n] += 1.0 / std::complex<double>(data.positiveResistance, data.positiveReactance);
                    } break;
                    case NEGATIVE_SEQ: {
                        yBus[n][n] += 1.0 / std::complex<double>(data.negativeResistance, data.negativeReactance);
                    } break;
                    case ZERO_SEQ: {
                        yBus[n][n] += 1.0 / std::complex<double>(data.zeroResistance, data.zeroReactance);
                    } break;
                }
            }
        }
        // Synchronous motor
        for(auto it = m_syncMotorList.begin(), itEnd = m_syncMotorList.end(); it != itEnd; ++it) {
            SyncMotor* syncMotor = *it;
            if(syncMotor->IsOnline()) {
                int n = static_cast<Bus*>(syncMotor->GetParentList()[0])->GetEletricalData().number;
                SyncMotorElectricalData data = syncMotor->GetPUElectricalData(systemPowerBase);
                switch(sequence) {
                    case POSITIVE_SEQ: {
                        yBus[n][n] += 1.0 / std::complex<double>(data.positiveResistance, data.positiveReactance);
                    } break;
                    case NEGATIVE_SEQ: {
                        yBus[n][n] += 1.0 / std::complex<double>(data.negativeResistance, data.negativeReactance);
                    } break;
                    case ZERO_SEQ: {
                        yBus[n][n] += 1.0 / std::complex<double>(data.zeroResistance, data.zeroReactance);
                    } break;
                }
            }
        }
    }

    return true;
}

void ElectricCalculation::UpdateElementsPowerFlow(std::vector<std::complex<double> > voltage,
    std::vector<std::complex<double> > power,
    std::vector<BusType> busType,
    std::vector<ReactiveLimits> reactiveLimit,
    double systemPowerBase)
{
    for(int i = 0; i < (int)reactiveLimit.size(); ++i) {
        if(reactiveLimit[i].maxLimit > -1e-5 && reactiveLimit[i].maxLimit < 1e-5) reactiveLimit[i].maxLimit = 1e-5;
        if(reactiveLimit[i].minLimit > -1e-5 && reactiveLimit[i].minLimit < 1e-5) reactiveLimit[i].minLimit = 1e-5;
    }
    // Buses voltages
    for(int i = 0; i < (int)m_busList.size(); i++) {
        Bus* bus = m_busList[i];
        BusElectricalData data = bus->GetEletricalData();
        data.voltage = voltage[i];
        bus->SetElectricalData(data);
    }

    // Power line
    for(int i = 0; i < (int)m_lineList.size(); i++) {
        Line* line = m_lineList[i];
        if(line->IsOnline()) {
            int n1 = static_cast<Bus*>(line->GetParentList()[0])->GetEletricalData().number;
            int n2 = static_cast<Bus*>(line->GetParentList()[1])->GetEletricalData().number;

            LineElectricalData data = line->GetElectricalData();
            std::complex<double> v1 = voltage[n1];
            std::complex<double> v2 = voltage[n2];

            data.current[0] = (v1 - v2) / std::complex<double>(data.resistance, data.indReactance) +
                v1 * std::complex<double>(0.0, data.capSusceptance / 2.0);
            data.current[1] = (v2 - v1) / std::complex<double>(data.resistance, data.indReactance) +
                v2 * std::complex<double>(0.0, data.capSusceptance / 2.0);

            data.powerFlow[0] = v1 * std::conj(data.current[0]);
            data.powerFlow[1] = v2 * std::conj(data.current[1]);

            if(data.powerFlow[0].real() > data.powerFlow[1].real())
                line->SetPowerFlowDirection(PF_BUS1_TO_BUS2);
            else
                line->SetPowerFlowDirection(PF_BUS2_TO_BUS1);

            line->SetElectricalData(data);
        }
    }

    // Transformer
    for(int i = 0; i < (int)m_transformerList.size(); i++) {
        Transformer* transformer = m_transformerList[i];
        if(transformer->IsOnline()) {
            TransformerElectricalData data = transformer->GetElectricalData();
            int n1 = static_cast<Bus*>(transformer->GetParentList()[0])->GetEletricalData().number;
            int n2 = static_cast<Bus*>(transformer->GetParentList()[1])->GetEletricalData().number;
            std::complex<double> v1 = voltage[n1]; // Primary voltage
            std::complex<double> v2 = voltage[n2]; // Secondary voltage

            // Transformer admitance
            std::complex<double> y = 1.0 / std::complex<double>(data.resistance, data.indReactance);

            if(data.turnsRatio == 1.0 && data.phaseShift == 0.0) {
                data.current[0] = (v1 - v2) * y;
                data.current[1] = (v2 - v1) * y;
            } else {
                double radPS = wxDegToRad(data.phaseShift);
                std::complex<double> a =
                    std::complex<double>(data.turnsRatio * std::cos(radPS), -data.turnsRatio * std::sin(radPS));

                data.current[0] = v1 * (y / std::pow(std::abs(a), 2)) - v2 * (y / std::conj(a));
                data.current[1] = -v1 * (y / a) + v2 * y;
            }

            data.powerFlow[0] = v1 * std::conj(data.current[0]);
            data.powerFlow[1] = v2 * std::conj(data.current[1]);

            if(data.powerFlow[0].real() > data.powerFlow[1].real())
                transformer->SetPowerFlowDirection(PF_BUS1_TO_BUS2);
            else
                transformer->SetPowerFlowDirection(PF_BUS2_TO_BUS1);

            transformer->SetElectricaData(data);
        }
    }

    // Synchronous machines
    for(int i = 0; i < (int)m_busList.size(); i++) {
        Bus* bus = m_busList[i];
        BusElectricalData data = bus->GetEletricalData();

        // Get the synchronous machines connected and calculate the load power on the bus.
        std::vector<SyncGenerator*> syncGeneratorsOnBus;
        std::vector<SyncMotor*> syncMotorsOnBus;
        std::complex<double> loadPower(0.0, 0.0);

        for(auto itsg = m_syncGeneratorList.begin(); itsg != m_syncGeneratorList.end(); itsg++) {
            SyncGenerator* syncGenerator = *itsg;
            if(bus == syncGenerator->GetParentList()[0] && syncGenerator->IsOnline())
                syncGeneratorsOnBus.push_back(syncGenerator);
        }
        for(auto itsm = m_syncMotorList.begin(); itsm != m_syncMotorList.end(); itsm++) {
            SyncMotor* syncMotor = *itsm;
            if(bus == syncMotor->GetParentList()[0] && syncMotor->IsOnline()) {
                syncMotorsOnBus.push_back(syncMotor);
                SyncMotorElectricalData childData = syncMotor->GetPUElectricalData(systemPowerBase);
                loadPower += std::complex<double>(childData.activePower, 0.0);
            }
        }
        for(auto itlo = m_loadList.begin(); itlo != m_loadList.end(); itlo++) {
            Load* load = *itlo;
            if(bus == load->GetParentList()[0] && load->IsOnline()) {
                LoadElectricalData childData = load->GetPUElectricalData(systemPowerBase);
                if(childData.loadType == CONST_POWER)
                    loadPower += std::complex<double>(childData.activePower, childData.reactivePower);

                if(childData.activePower >= 0.0)
                    load->SetPowerFlowDirection(PF_TO_ELEMENT);
                else
                    load->SetPowerFlowDirection(PF_TO_BUS);
            }
        }
        for(auto itim = m_indMotorList.begin(); itim != m_indMotorList.end(); itim++) {
            IndMotor* indMotor = *itim;
            if(bus == indMotor->GetParentList()[0] && indMotor->IsOnline()) {
                IndMotorElectricalData childData = indMotor->GetPUElectricalData(systemPowerBase);
                loadPower += std::complex<double>(childData.activePower, childData.reactivePower);

                if(childData.activePower >= 0.0)
                    indMotor->SetPowerFlowDirection(PF_TO_ELEMENT);
                else
                    indMotor->SetPowerFlowDirection(PF_TO_BUS);
            }
        }

        // Set the sync generator power
        for(auto itsg = syncGeneratorsOnBus.begin(); itsg != syncGeneratorsOnBus.end(); itsg++) {
            SyncGenerator* generator = *itsg;
            if(generator->IsOnline()) {
                SyncGeneratorElectricalData childData = generator->GetElectricalData();

                if(busType[i] == BUS_SLACK) {
                    double activePower =
                        (power[i].real() + loadPower.real()) * systemPowerBase / (double)(syncGeneratorsOnBus.size());

                    switch(childData.activePowerUnit) {
                        case UNIT_PU: {
                            activePower /= systemPowerBase;
                        } break;
                        case UNIT_kW: {
                            activePower /= 1e3;
                        } break;
                        case UNIT_MW: {
                            activePower /= 1e6;
                        } break;
                        default:
                            break;
                    }

                    childData.activePower = activePower;
                }
                if(busType[i] == BUS_PV || busType[i] == BUS_SLACK) {
                    // double reactivePower = (power[i].imag() + loadPower.imag()) * systemPowerBase /
                    //                       (double)(syncGeneratorsOnBus.size() + syncMotorsOnBus.size());
                    SyncGeneratorElectricalData childData_PU = generator->GetPUElectricalData(systemPowerBase);

                    double reactivePower = (power[i].imag() + loadPower.imag()) * systemPowerBase;

                    if(reactiveLimit[i].limitReached == RL_MAX_REACHED)
                        reactivePower *= (childData_PU.maxReactive / reactiveLimit[i].maxLimit);

                    else if(reactiveLimit[i].limitReached == RL_MIN_REACHED)
                        reactivePower *= (childData_PU.minReactive / reactiveLimit[i].minLimit);

                    else
                        reactivePower /= (double)(syncGeneratorsOnBus.size() + syncMotorsOnBus.size());

                    switch(childData.reactivePowerUnit) {
                        case UNIT_PU: {
                            reactivePower /= systemPowerBase;
                        } break;
                        case UNIT_kVAr: {
                            reactivePower /= 1e3;
                        } break;
                        case UNIT_MVAr: {
                            reactivePower /= 1e6;
                        } break;
                        default:
                            break;
                    }
                    childData.reactivePower = reactivePower;
                }

                if(childData.activePower >= 0.0)
                    generator->SetPowerFlowDirection(PF_TO_BUS);
                else
                    generator->SetPowerFlowDirection(PF_TO_ELEMENT);

                generator->SetElectricalData(childData);
            }
        }

        // Set the sync motor reactive power
        double exceededReactive = 0.0;
        int numMachines = syncGeneratorsOnBus.size() + syncMotorsOnBus.size();
        for(auto itsm = syncMotorsOnBus.begin(); itsm != syncMotorsOnBus.end(); itsm++) {
            SyncMotor* syncMotor = *itsm;
            SyncMotorElectricalData childData = syncMotor->GetElectricalData();

            bool reachedMachineLimit = false;

            if(busType[i] == BUS_PV || busType[i] == BUS_SLACK) {
                // double reactivePower = (power[i].imag() + loadPower.imag()) * systemPowerBase /
                //                       (double)(syncGeneratorsOnBus.size() + syncMotorsOnBus.size());

                SyncMotorElectricalData childData_PU = syncMotor->GetPUElectricalData(systemPowerBase);

                double reactivePower = power[i].imag() + loadPower.imag();

                // Bus reachd maximum reactive limit.
                if(reactiveLimit[i].limitReached == RL_MAX_REACHED)
                    reactivePower *= (childData_PU.maxReactive / reactiveLimit[i].maxLimit);
                // Bus reached minimum reactive limit.
                else if(reactiveLimit[i].limitReached == RL_MIN_REACHED)
                    reactivePower *= (childData_PU.minReactive / reactiveLimit[i].minLimit);
                // Bus didn't reach any limits
                else {
                    reactivePower /= (double)(numMachines);
                    if(childData_PU.haveMaxReactive && (reactivePower > childData_PU.maxReactive)) {
                        exceededReactive += reactivePower - childData_PU.maxReactive;
                        reactivePower = childData_PU.maxReactive;
                        reachedMachineLimit = true;
                    } else if(childData_PU.haveMinReactive && (reactivePower < childData_PU.minReactive)) {
                        exceededReactive += reactivePower - childData_PU.minReactive;
                        reactivePower = childData_PU.minReactive;
                        reachedMachineLimit = true;
                    } else if((!childData_PU.haveMaxReactive && reactiveLimit[i].limitReached == RL_MAX_REACHED) ||
                        (!childData_PU.haveMinReactive && reactiveLimit[i].limitReached == RL_MIN_REACHED) ||
                        (!childData_PU.haveMaxReactive && !childData_PU.haveMaxReactive)) {
                        reactivePower += exceededReactive;
                        exceededReactive = 0.0;
                    }
                }

                reactivePower *= systemPowerBase;

                switch(childData.reactivePowerUnit) {
                    case UNIT_PU: {
                        reactivePower /= systemPowerBase;
                    } break;
                    case UNIT_kVAr: {
                        reactivePower /= 1e3;
                    } break;
                    case UNIT_MVAr: {
                        reactivePower /= 1e6;
                    } break;
                    default:
                        break;
                }
                childData.reactivePower = reactivePower;
            }

            if(childData.activePower > 0.0)
                syncMotor->SetPowerFlowDirection(PF_TO_ELEMENT);
            else
                syncMotor->SetPowerFlowDirection(PF_TO_BUS);

            syncMotor->SetElectricalData(childData);

            if(reachedMachineLimit) {
                syncMotorsOnBus.erase(itsm);
                itsm = syncMotorsOnBus.begin();
            }
        }
    }
}

bool ElectricCalculation::InvertMatrix(std::vector<std::vector<std::complex<double> > > matrix,
    std::vector<std::vector<std::complex<double> > >& inverse)
{
    int order = static_cast<int>(matrix.size());

    inverse.clear();
    // Fill the inverse matrix with identity.
    for(int i = 0; i < order; ++i) {
        std::vector<std::complex<double> > line;
        for(int j = 0; j < order; ++j) {
            line.push_back(i == j ? std::complex<double>(1.0, 0.0) : std::complex<double>(0.0, 0.0));
        }
        inverse.push_back(line);
    }

    // Check if a main diagonal value of the matrix is zero, if one is zero, try a linear combination to remove it.
    for(int i = 0; i < order; ++i) {
        for(int j = 0; j < order; ++j) {
            if(i == j && matrix[i][j] == std::complex<double>(0.0, 0.0)) {
                int row = 0;
                while(row < order) {
                    if(matrix[row][j] != std::complex<double>(0.0, 0.0)) {
                        for(int k = 0; k < order; ++k) {
                            matrix[i][k] += matrix[row][k];
                            inverse[i][k] += inverse[row][k];
                        }
                        break;
                    }
                    row++;
                }
                // If all line values are zero, the matrix is singular and the solution is impossible.
                if(row == order) return false;
            }
        }
    }

    // Linear combinations are made in both matrices, the goal is the input matrix become the identity. The final result
    // have two matrices: the identity and the inverse of the input.
    for(int i = 0; i < order; ++i) {
        for(int j = 0; j < order; ++j) {
            if(i != j) {
                if(matrix[i][i] == std::complex<double>(0.0, 0.0)) return false;

                std::complex<double> factor = matrix[j][i] / matrix[i][i];
                for(int k = 0; k < order; ++k) {
                    matrix[j][k] -= factor * matrix[i][k];
                    inverse[j][k] -= factor * inverse[i][k];
                }
            }
        }
    }
    // Main diagonal calculation.
    for(int i = 0; i < order; ++i) {
        for(int j = 0; j < order; ++j) {
            if(i == j) {
                if(matrix[i][j] == std::complex<double>(0.0, 0.0)) return false;

                std::complex<double> factor = (matrix[i][j] - std::complex<double>(1.0, 0.0)) / matrix[i][j];
                for(int k = 0; k < order; ++k) {
                    matrix[j][k] -= factor * matrix[i][k];
                    inverse[j][k] -= factor * inverse[i][k];
                }
            }
        }
    }

    return true;
}
