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
    // Bad design?
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

    // Build connection map
    std::vector<std::vector<int> > lineMap;
    std::vector<std::vector<int> > transfomerMap;
    lineMap.resize(m_lineList.size());
    transfomerMap.resize(m_transformerList.size());

    // Get the connection map for
    int busNumber = 0;
    for(auto itb = m_busList.begin(); itb != m_busList.end(); itb++) {
        Bus* bus = *itb;

        // Get power line  connection map
        for(int i = 0; i < (int)m_lineList.size(); i++) {
            for(int j = 0; j < (int)m_lineList[i]->GetParentList().size(); j++) {
                if(bus == m_lineList[i]->GetParentList()[j]) lineMap[i].push_back(busNumber);
            }
        }

        // Get transformer connection map
        for(int i = 0; i < (int)m_transformerList.size(); i++) {
            for(int j = 0; j < (int)m_transformerList[i]->GetParentList().size(); j++) {
                if(bus == m_transformerList[i]->GetParentList()[j]) transfomerMap[i].push_back(busNumber);
            }
        }

        // Load
        for(auto itlo = m_loadList.begin(); itlo != m_loadList.end(); itlo++) {
            Load* load = *itlo;
            if(bus == load->GetParentList()[0] &&  load->IsOnline()) {
                LoadElectricalData data = load->GetPUElectricalData(systemPowerBase);
                if(data.loadType == CONST_IMPEDANCE)
                    yBus[busNumber][busNumber] += std::complex<double>(data.activePower, -data.reactivePower);
            }
        }

        // Capacitor
        for(auto itca = m_capacitorList.begin(); itca != m_capacitorList.end(); itca++) {
            Capacitor* capacitor = *itca;
            if(bus == capacitor->GetParentList()[0] && capacitor->IsOnline()) {
                CapacitorElectricalData data = capacitor->GetPUElectricalData(systemPowerBase);
                yBus[busNumber][busNumber] += std::complex<double>(0.0, data.reactivePower);
            }
        }

        // Inductor
        for(auto itin = m_inductorList.begin(); itin != m_inductorList.end(); itin++) {
            Inductor* inductor = *itin;
            if(bus == inductor->GetParentList()[0] && inductor->IsOnline()) {
                InductorElectricalData data = inductor->GetPUElectricalData(systemPowerBase);
                yBus[busNumber][busNumber] += std::complex<double>(0.0, -data.reactivePower);
            }
        }

        busNumber++;
    }

    // Power line
    int lineNumber = 0;
    for(auto itl = m_lineList.begin(); itl != m_lineList.end(); itl++) {
        Line* line = *itl;
        LineElectricalData data = line->GetElectricalData();
        if(line->IsOnline()) {
            yBus[lineMap[lineNumber][0]][lineMap[lineNumber][1]] -=
                1.0 / std::complex<double>(data.resistance, data.indReactance);
            yBus[lineMap[lineNumber][1]][lineMap[lineNumber][0]] -=
                1.0 / std::complex<double>(data.resistance, data.indReactance);

            yBus[lineMap[lineNumber][0]][lineMap[lineNumber][0]] +=
                1.0 / std::complex<double>(data.resistance, data.indReactance);
            yBus[lineMap[lineNumber][1]][lineMap[lineNumber][1]] +=
                1.0 / std::complex<double>(data.resistance, data.indReactance);

            yBus[lineMap[lineNumber][0]][lineMap[lineNumber][0]] +=
                std::complex<double>(0.0, data.capSusceptance / 2.0);
            yBus[lineMap[lineNumber][1]][lineMap[lineNumber][1]] +=
                std::complex<double>(0.0, data.capSusceptance / 2.0);
        }
        lineNumber++;
    }

    int transformerNumber = 0;
    for(auto itt = m_transformerList.begin(); itt != m_transformerList.end(); ++itt) {
        Transformer* transformer = *itt;
        TransformerElectricalData data = transformer->GetElectricalData();

        if(transformer->IsOnline()) {
            // If the transformer have nominal turns ratio (1.0) and no phase shifting, it will be modelled as series
            // impedance.
            if(data.turnsRatio == 1.0 && data.phaseShift == 0.0) {
                yBus[transfomerMap[transformerNumber][0]][transfomerMap[transformerNumber][1]] +=
                    -1.0 / std::complex<double>(data.resistance, data.indReactance);
                yBus[transfomerMap[transformerNumber][1]][transfomerMap[transformerNumber][0]] +=
                    -1.0 / std::complex<double>(data.resistance, data.indReactance);

                yBus[transfomerMap[transformerNumber][0]][transfomerMap[transformerNumber][0]] +=
                    1.0 / std::complex<double>(data.resistance, data.indReactance);
                yBus[transfomerMap[transformerNumber][1]][transfomerMap[transformerNumber][1]] +=
                    1.0 / std::complex<double>(data.resistance, data.indReactance);
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

                yBus[transfomerMap[transformerNumber][0]][transfomerMap[transformerNumber][0]] +=
                    y / std::pow(std::abs(a), 2.0);
                yBus[transfomerMap[transformerNumber][0]][transfomerMap[transformerNumber][1]] += -(y / std::conj(a));
                yBus[transfomerMap[transformerNumber][1]][transfomerMap[transformerNumber][0]] += -(y / a);
                yBus[transfomerMap[transformerNumber][1]][transfomerMap[transformerNumber][1]] += y;
            }
        }

        transformerNumber++;
    }

    return true;
}
