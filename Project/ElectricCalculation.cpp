#include "ElectricCalculation.h"

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
        if(typeid(*element) == typeid(Bus))
            m_busList.push_back((Bus*)element);
        else if(typeid(*element) == typeid(Capacitor))
            m_capacitorList.push_back((Capacitor*)element);
        else if(typeid(*element) == typeid(IndMotor))
            m_indMotorList.push_back((IndMotor*)element);
        else if(typeid(*element) == typeid(Inductor))
            m_inductorList.push_back((Inductor*)element);
        else if(typeid(*element) == typeid(Line))
            m_lineList.push_back((Line*)element);
        else if(typeid(*element) == typeid(Load))
            m_loadList.push_back((Load*)element);
        else if(typeid(*element) == typeid(SyncGenerator))
            m_syncGeneratorList.push_back((SyncGenerator*)element);
        else if(typeid(*element) == typeid(SyncMotor))
            m_syncMotorList.push_back((SyncMotor*)element);
        else if(typeid(*element) == typeid(Transformer))
            m_transformerList.push_back((Transformer*)element);
    }
}

bool ElectricCalculation::GetYBus(std::vector<std::vector<std::complex<double> > >& yBus, double systemPowerBase)
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
    for(auto itlo = m_loadList.begin(); itlo != m_loadList.end(); itlo++) {
        Load* load = *itlo;
        if(load->IsOnline()) {
            int n = ((Bus*)load->GetParentList()[0])->GetEletricalData().number;
            LoadElectricalData data = load->GetPUElectricalData(systemPowerBase);
            if(data.loadType == CONST_IMPEDANCE)
                yBus[n][n] += std::complex<double>(data.activePower, -data.reactivePower);
        }
    }

    // Capacitor
    for(auto itca = m_capacitorList.begin(); itca != m_capacitorList.end(); itca++) {
        Capacitor* capacitor = *itca;
        if(capacitor->IsOnline()) {
            int n = ((Bus*)capacitor->GetParentList()[0])->GetEletricalData().number;
            CapacitorElectricalData data = capacitor->GetPUElectricalData(systemPowerBase);
            yBus[n][n] += std::complex<double>(0.0, data.reactivePower);
        }
    }

    // Inductor
    for(auto itin = m_inductorList.begin(); itin != m_inductorList.end(); itin++) {
        Inductor* inductor = *itin;
        if(inductor->IsOnline()) {
            int n = ((Bus*)inductor->GetParentList()[0])->GetEletricalData().number;
            InductorElectricalData data = inductor->GetPUElectricalData(systemPowerBase);
            yBus[n][n] += std::complex<double>(0.0, -data.reactivePower);
        }
    }

    // Power line
    for(auto itl = m_lineList.begin(); itl != m_lineList.end(); itl++) {
        Line* line = *itl;
        if(line->IsOnline()) {
            LineElectricalData data = line->GetElectricalData();

            int n1 = ((Bus*)line->GetParentList()[0])->GetEletricalData().number;
            int n2 = ((Bus*)line->GetParentList()[1])->GetEletricalData().number;

            yBus[n1][n2] -= 1.0 / std::complex<double>(data.resistance, data.indReactance);
            yBus[n2][n1] -= 1.0 / std::complex<double>(data.resistance, data.indReactance);

            yBus[n1][n1] += 1.0 / std::complex<double>(data.resistance, data.indReactance);
            yBus[n2][n2] += 1.0 / std::complex<double>(data.resistance, data.indReactance);

            yBus[n1][n1] += std::complex<double>(0.0, data.capSusceptance / 2.0);
            yBus[n2][n2] += std::complex<double>(0.0, data.capSusceptance / 2.0);
        }
    }

    // Transformer
    for(auto itt = m_transformerList.begin(); itt != m_transformerList.end(); ++itt) {
        Transformer* transformer = *itt;

        if(transformer->IsOnline()) {
            TransformerElectricalData data = transformer->GetElectricalData();

            int n1 = ((Bus*)transformer->GetParentList()[0])->GetEletricalData().number;
            int n2 = ((Bus*)transformer->GetParentList()[1])->GetEletricalData().number;

            // If the transformer have nominal turns ratio (1.0) and no phase shifting, it will be modelled as series
            // impedance.
            if(data.turnsRatio == 1.0 && data.phaseShift == 0.0) {
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
            else {
                // Complex turns ratio
                double radPhaseShift = wxDegToRad(data.phaseShift);
                std::complex<double> a = std::complex<double>(data.turnsRatio * std::cos(radPhaseShift),
                                                              -data.turnsRatio * std::sin(radPhaseShift));

                // Transformer admitance
                std::complex<double> y = 1.0 / std::complex<double>(data.resistance, data.indReactance);

                yBus[n1][n1] += y / std::pow(std::abs(a), 2.0);
                yBus[n1][n2] += -(y / std::conj(a));
                yBus[n2][n1] += -(y / a);
                yBus[n2][n2] += y;
            }
        }
    }

    return true;
}

void ElectricCalculation::UpdateElementsPowerFlow(std::vector<std::complex<double> > voltage,
                                                  std::vector<std::complex<double> > power,
                                                  std::vector<BusType> busType,
                                                  double systemPowerBase)
{
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
            int n1 = ((Bus*)line->GetParentList()[0])->GetEletricalData().number;
            int n2 = ((Bus*)line->GetParentList()[1])->GetEletricalData().number;

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
            int n1 = ((Bus*)transformer->GetParentList()[0])->GetEletricalData().number;
            int n2 = ((Bus*)transformer->GetParentList()[1])->GetEletricalData().number;
            std::complex<double> v1 = voltage[n1];  // Primary voltage
            std::complex<double> v2 = voltage[n2];  // Secondary voltage

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
                    double reactivePower = (power[i].imag() + loadPower.imag()) * systemPowerBase /
                                           (double)(syncGeneratorsOnBus.size() + syncMotorsOnBus.size());
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
        for(auto itmg = syncMotorsOnBus.begin(); itmg != syncMotorsOnBus.end(); itmg++) {
            SyncMotor* syncMotor = *itmg;
            if(syncMotor->IsOnline()) {
                SyncMotorElectricalData childData = syncMotor->GetElectricalData();

                if(busType[i] == BUS_PV || busType[i] == BUS_SLACK) {
                    double reactivePower = (power[i].imag() + loadPower.imag()) * systemPowerBase /
                                           (double)(syncGeneratorsOnBus.size() + syncMotorsOnBus.size());
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
            }
        }
    }
}
