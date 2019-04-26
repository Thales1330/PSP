#include "PowerQuality.h"

PowerQuality::PowerQuality() {}

PowerQuality::~PowerQuality() {}

PowerQuality::PowerQuality(std::vector<Element*> elementList) { GetElementsFromList(elementList); }

void PowerQuality::CalculateHarmonicYbusList(double systemPowerBase)
{
    // Clear and fill with zeros all the harmonic Ybuses
    for(auto it = m_harmYbusList.begin(), itEnd = m_harmYbusList.end(); it != itEnd; ++it) {
        HarmonicYbus harmYBus = *it;
        harmYBus.yBus.clear();
        for(unsigned int i = 0; i < m_busList.size(); i++) {
            std::vector<std::complex<double> > line;
            for(unsigned int j = 0; j < m_busList.size(); j++) { line.push_back(std::complex<double>(0.0, 0.0)); }
            harmYBus.yBus.push_back(line);
        }
        *it = harmYBus;
    }

    // Fill all Ybuses
    for(auto itYbus = m_harmYbusList.begin(), itYbusEnd = m_harmYbusList.end(); itYbus != itYbusEnd; ++itYbus) {
        HarmonicYbus harmYBus = *itYbus;
        CalculateHarmonicYbus(harmYBus.yBus, systemPowerBase, harmYBus.order);
        *itYbus = harmYBus;
    }
}

void PowerQuality::CalculateHarmonicYbus(std::vector<std::vector<std::complex<double> > >& yBus,
                                         double systemPowerBase,
                                         double order)
{
    // Load
    for(auto it = m_loadList.begin(), itEnd = m_loadList.end(); it != itEnd; ++it) {
        Load* load = *it;
        if(load->IsOnline()) {
            int n = static_cast<Bus*>(load->GetParentList()[0])->GetElectricalData().number;
            LoadElectricalData data = load->GetPUElectricalData(systemPowerBase);
            std::complex<double> yLoad = std::complex<double>(data.activePower, -data.reactivePower / order);
            std::complex<double> v = static_cast<Bus*>(load->GetParentList()[0])->GetElectricalData().voltage;
            yLoad /= (std::abs(v) * std::abs(v));
            yBus[n][n] += yLoad;
        }
    }

    // Capacitor
    for(auto it = m_capacitorList.begin(), itEnd = m_capacitorList.end(); it != itEnd; ++it) {
        Capacitor* capacitor = *it;
        if(capacitor->IsOnline()) {
            int n = static_cast<Bus*>(capacitor->GetParentList()[0])->GetElectricalData().number;
            CapacitorElectricalData data = capacitor->GetPUElectricalData(systemPowerBase);
            yBus[n][n] += std::complex<double>(0.0, data.reactivePower) * order;
        }
    }

    // Inductor
    for(auto it = m_inductorList.begin(), itEnd = m_inductorList.end(); it != itEnd; ++it) {
        Inductor* inductor = *it;
        if(inductor->IsOnline()) {
            int n = static_cast<Bus*>(inductor->GetParentList()[0])->GetElectricalData().number;
            InductorElectricalData data = inductor->GetPUElectricalData(systemPowerBase);
            yBus[n][n] += std::complex<double>(0.0, -data.reactivePower) / order;
        }
    }

    // Power line
    for(auto it = m_lineList.begin(), itEnd = m_lineList.end(); it != itEnd; ++it) {
        Line* line = *it;
        if(line->IsOnline()) {
            LineElectricalData data = line->GetPUElectricalData(systemPowerBase);

            int n1 = static_cast<Bus*>(line->GetParentList()[0])->GetElectricalData().number;
            int n2 = static_cast<Bus*>(line->GetParentList()[1])->GetElectricalData().number;

            yBus[n1][n2] -= 1.0 / std::complex<double>(data.resistance, data.indReactance * order);
            yBus[n2][n1] -= 1.0 / std::complex<double>(data.resistance, data.indReactance * order);

            yBus[n1][n1] += 1.0 / std::complex<double>(data.resistance, data.indReactance * order);
            yBus[n2][n2] += 1.0 / std::complex<double>(data.resistance, data.indReactance * order);

            yBus[n1][n1] += std::complex<double>(0.0, (data.capSusceptance * order) / 2.0);
            yBus[n2][n2] += std::complex<double>(0.0, (data.capSusceptance * order) / 2.0);
        }
    }

    // Transformer
    for(auto it = m_transformerList.begin(), itEnd = m_transformerList.end(); it != itEnd; ++it) {
        Transformer* transformer = *it;

        if(transformer->IsOnline()) {
            TransformerElectricalData data = transformer->GetPUElectricalData(systemPowerBase);

            int n1 = static_cast<Bus*>(transformer->GetParentList()[0])->GetElectricalData().number;
            int n2 = static_cast<Bus*>(transformer->GetParentList()[1])->GetElectricalData().number;

            // If the transformer have nominal turns ratio (1.0) and no phase shifting, it will be modelled as
            // series impedance.
            if(data.turnsRatio == 1.0 && data.phaseShift == 0.0) {
                yBus[n1][n2] += -1.0 / std::complex<double>(data.resistance, data.indReactance * order);
                yBus[n2][n1] += -1.0 / std::complex<double>(data.resistance, data.indReactance * order);

                yBus[n1][n1] += 1.0 / std::complex<double>(data.resistance, data.indReactance * order);
                yBus[n2][n2] += 1.0 / std::complex<double>(data.resistance, data.indReactance * order);
            }
            // If the transformer have no-nominal turn ratio and/or phase shifting, it will be modelled in a
            // different way (see references).
            //[Ref. 1: Elementos de analise de sistemas de potencia - Stevenson - pg. 232]
            //[Ref. 2:
            // http://www.ee.washington.edu/research/real/Library/Reports/Tap_Adjustments_in_AC_Load_Flows.pdf]
            // [Ref. 3: http://www.columbia.edu/~dano/courses/power/notes/power/andersson1.pdf]
            else {
                // Complex turns ratio
                double radPhaseShift = wxDegToRad(data.phaseShift);
                std::complex<double> a = std::complex<double>(data.turnsRatio * std::cos(radPhaseShift),
                                                              -data.turnsRatio * std::sin(radPhaseShift));

                // Transformer admitance
                std::complex<double> y = 1.0 / std::complex<double>(data.resistance, data.indReactance * order);

                yBus[n1][n1] += y / (std::pow(std::abs(a), 2.0));
                yBus[n1][n2] += -(y / std::conj(a));
                yBus[n2][n1] += -(y / a);
                yBus[n2][n2] += y;
            }
        }
    }

    // Synchronous generator
    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        if(syncGenerator->IsOnline()) {
            int n = static_cast<Bus*>(syncGenerator->GetParentList()[0])->GetElectricalData().number;
            SyncGeneratorElectricalData data = syncGenerator->GetPUElectricalData(systemPowerBase);
            yBus[n][n] += 1.0 / std::complex<double>(data.positiveResistance, data.positiveReactance * order);
        }
    }
    // Synchronous motor
    for(auto it = m_syncMotorList.begin(), itEnd = m_syncMotorList.end(); it != itEnd; ++it) {
        SyncMotor* syncMotor = *it;
        if(syncMotor->IsOnline()) {
            int n = static_cast<Bus*>(syncMotor->GetParentList()[0])->GetElectricalData().number;
            SyncMotorElectricalData data = syncMotor->GetPUElectricalData(systemPowerBase);
            yBus[n][n] += 1.0 / std::complex<double>(data.positiveResistance, data.positiveReactance * order);
        }
    }
}

bool PowerQuality::CalculateDistortions(double systemPowerBase)
{
    // Get harmonic orders
    m_harmYbusList.clear();
    std::vector<double> harmOrders = GetHarmonicOrdersList();

    // Fill the Ybuses
    for(unsigned int i = 0; i < harmOrders.size(); ++i) {
        HarmonicYbus newYbus;
        newYbus.order = harmOrders[i];
        m_harmYbusList.push_back(newYbus);
    }
    CalculateHarmonicYbusList(systemPowerBase);

    // Initialize current arrays with zeros
    std::vector<std::vector<std::complex<double> > > iHarmInjList;
    for(unsigned int i = 0; i < harmOrders.size(); i++) {
        std::vector<std::complex<double> > line;
        for(unsigned int j = 0; j < m_busList.size(); j++) { line.push_back(std::complex<double>(0.0, 0.0)); }
        iHarmInjList.push_back(line);
    }

    // Fill the current array
    for(unsigned int i = 0; i < harmOrders.size(); ++i) {
        for(auto it = m_harmCurrentList.begin(), itEnd = m_harmCurrentList.end(); it != itEnd; ++it) {
            HarmCurrent* harmCurrent = *it;
            if(harmCurrent->IsOnline()) {
                // Get only the current order in analysis
                for(unsigned int k = 0; k < harmCurrent->GetElectricalData().harmonicOrder.size(); ++k) {
                    if(harmCurrent->GetElectricalData().harmonicOrder[k] == static_cast<int>(harmOrders[i])) {
                        Bus* parentBus = static_cast<Bus*>(harmCurrent->GetParentList()[0]);
                        auto busData = parentBus->GetElectricalData();
                        int j = busData.number;

                        // Bus voltage
                        double voltage = busData.nominalVoltage;
                        if(busData.nominalVoltageUnit == UNIT_kV) voltage *= 1e3;

                        auto puData = harmCurrent->GetPUElectricalData(systemPowerBase, voltage);

                        iHarmInjList[i][j] += std::complex<double>(
                            puData.injHarmCurrent[k] * std::cos(wxDegToRad(puData.injHarmAngle[k])),
                            puData.injHarmCurrent[k] * std::sin(wxDegToRad(puData.injHarmAngle[k])));
                    }
                }
            }
        }
    }

    // Calculate harmonic voltages
    std::vector<std::vector<std::complex<double> > > vHarmList;
    for(unsigned int i = 0; i < m_harmYbusList.size(); ++i) {
        vHarmList.push_back(GaussianElimination(m_harmYbusList[i].yBus, iHarmInjList[i]));
    }

    for(auto it = m_busList.begin(), itEnd = m_busList.end(); it != itEnd; ++it) {
        Bus* bus = *it;
        auto data = bus->GetElectricalData();
        data.harmonicOrder.clear();
        data.harmonicVoltage.clear();
        double thd = 0.0;
        for(unsigned int i = 0; i < vHarmList.size(); ++i) {
            thd += std::abs(vHarmList[i][data.number]) * std::abs(vHarmList[i][data.number]);
            data.harmonicVoltage.push_back(vHarmList[i][data.number]);
            data.harmonicOrder.push_back(static_cast<int>(harmOrders[i]));
        }
        // distortion = std::sqrt(distortion) / std::abs(data.voltage);
        thd = std::sqrt(thd) * 100.0;
        data.thd = thd;
        bus->SetElectricalData(data);
    }

    return true;
}

std::vector<double> PowerQuality::GetHarmonicOrdersList()
{
    std::vector<int> harmOrders;
    auto harmCurrentList = GetHarmCurrentList();
    // Check all harmonic sources and get all harmonic orders in the system
    for(auto it = harmCurrentList.begin(), itEnd = harmCurrentList.end(); it != itEnd; ++it) {
        HarmCurrent* harmCurrent = *it;
        if(harmCurrent->IsOnline()) {
            auto data = harmCurrent->GetElectricalData();
            for(unsigned int i = 0; i < data.harmonicOrder.size(); ++i) {
                int order = data.harmonicOrder[i];
                // Check if this harmonic order have been added already
                bool newOrder = true;
                for(unsigned int j = 0; j < harmOrders.size(); ++j) {
                    if(order == harmOrders[j]) {
                        newOrder = false;
                        break;
                    }
                }
                if(newOrder) harmOrders.push_back(order);
            }
        }
    }
    std::vector<double> doubleHarmOrder;
    for(unsigned int i = 0; i < harmOrders.size(); ++i) {
        doubleHarmOrder.push_back(static_cast<double>(harmOrders[i]));
    }
    return doubleHarmOrder;
}

bool PowerQuality::CalculateFrequencyResponse(double systemFreq,
                                              double initFreq,
                                              double endFreq,
                                              double stepFreq,
                                              double systemPowerBase)
{
    // Clear all previous data
    for(unsigned int i = 0; i < m_busList.size(); i++) {
        auto data = m_busList[i]->GetElectricalData();
        data.absImpedanceVector.clear();
        data.absImpedanceVector.shrink_to_fit();
        m_busList[i]->SetElectricalData(data);
    }

    // Create and fill with zeros the YBus
    std::vector<std::vector<std::complex<double> > > yBus;
    for(unsigned int i = 0; i < m_busList.size(); i++) {
        std::vector<std::complex<double> > line;
        for(unsigned int j = 0; j < m_busList.size(); j++) { line.push_back(std::complex<double>(0.0, 0.0)); }
        yBus.push_back(line);
    }
    // Create and fill with zoros the injected current vector
    std::vector<std::complex<double> > iInj;
    for(unsigned int i = 0; i < m_busList.size(); i++) { iInj.push_back(std::complex<double>(10.0, 0.0)); }

    if(initFreq < 1e-6) initFreq = stepFreq;
    double currentFreq = initFreq;
    while(currentFreq <= endFreq) {
        m_frequencyList.push_back(currentFreq);

        double order = currentFreq / systemFreq;

        // Fill YBus with zeros
        for(unsigned int i = 0; i < m_busList.size(); i++) {
            for(unsigned int j = 0; j < m_busList.size(); j++) { yBus[i][j] = std::complex<double>(0.0, 0.0); }
        }

        CalculateHarmonicYbus(yBus, systemPowerBase, order);

        for(unsigned int i = 0; i < m_busList.size(); i++) {
            auto data = m_busList[i]->GetElectricalData();
            iInj[data.number] = std::complex<double>(1.0, 0.0);

            auto zh = GaussianElimination(yBus, iInj);

            data.absImpedanceVector.push_back(std::abs(zh[data.number]));
            m_busList[i]->SetElectricalData(data);
            iInj[data.number] = std::complex<double>(0.0, 0.0);
        }

        currentFreq += stepFreq;
    }
    return false;
}
