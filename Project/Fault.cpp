#include "Fault.h"
#ifdef USING_WX_3_0_X
#include "DegreesAndRadians.h"
#endif

Fault::Fault() : ElectricCalculation() {}
Fault::Fault(std::vector<Element*> elementList) { GetElementsFromList(elementList); }
Fault::~Fault() {}
bool Fault::RunFaultCalculation(double systemPowerBase)
{
    int numberOfBuses = static_cast<int>(m_busList.size());
    if(numberOfBuses == 0) {
        m_errorMsg = _("There is no buses in the system.");
        return false;
    }

    // Get adimittance matrices.
    std::vector<std::vector<std::complex<double> > > yBusPos;
    GetYBus(yBusPos, systemPowerBase, POSITIVE_SEQ, true);
    std::vector<std::vector<std::complex<double> > > yBusNeg;
    GetYBus(yBusNeg, systemPowerBase, NEGATIVE_SEQ, true);
    std::vector<std::vector<std::complex<double> > > yBusZero;
    GetYBus(yBusZero, systemPowerBase, ZERO_SEQ, true);

    // Calculate the impedance matrices.
    if(!InvertMatrix(yBusPos, m_zBusPos)) {
        m_errorMsg = _("Fail to invert the positive sequence admittance matrix.");
        return false;
    }
    if(!InvertMatrix(yBusNeg, m_zBusNeg)) {
        m_errorMsg = _("Fail to invert the negative sequence admittance matrix.");
        return false;
    }
    if(!InvertMatrix(yBusZero, m_zBusZero)) {
        m_errorMsg = _("Fail to invert the zero sequence admittance matrix.");
        return false;
    }

    // Pre-fault voltages (power flow solution).
    std::vector<std::complex<double> > preFaultVoltages;
    preFaultVoltages.resize(m_busList.size());

    // Get fault parameters.
    int fNumber = -1;
    FaultData fType = FAULT_THREEPHASE;
    FaultData fLocation = FAULT_LINE_A;
    std::complex<double> fImpedance = std::complex<double>(0.0, 0.0);
    for(auto it = m_busList.begin(), itEnd = m_busList.end(); it != itEnd; ++it) {
        Bus* bus = *it;
        BusElectricalData data = bus->GetElectricalData();
        preFaultVoltages[data.number] = data.voltage;
        if(data.hasFault) {
            fNumber = data.number;
            fType = data.faultType;
            fLocation = data.faultLocation;
            fImpedance = std::complex<double>(data.faultResistance, data.faultReactance);
        }
    }

    if(fNumber == -1) {
        m_errorMsg = _("There is no fault in the system.");
        return false;
    }

    // Fault calculation.
    std::complex<double> fCurrentPos = std::complex<double>(0.0, 0.0);
    std::complex<double> fCurrentNeg = std::complex<double>(0.0, 0.0);
    std::complex<double> fCurrentZero = std::complex<double>(0.0, 0.0);

    std::complex<double> preFaultVoltage = preFaultVoltages[fNumber];
    std::complex<double> a = std::complex<double>(-0.5, 0.866025403784);
    std::complex<double> a2 = std::complex<double>(-0.5, -0.866025403784);

    switch(fType) {
        case FAULT_THREEPHASE: {
            fCurrentPos = preFaultVoltage / (m_zBusPos[fNumber][fNumber] + fImpedance);
        } break;
        case FAULT_2LINE: {
            fCurrentPos = preFaultVoltage / (m_zBusPos[fNumber][fNumber] + m_zBusNeg[fNumber][fNumber] + fImpedance);

            switch(fLocation) {
                case FAULT_LINE_A: {
                    fCurrentNeg = -a2 * fCurrentPos;
                } break;
                case FAULT_LINE_B: {
                    fCurrentNeg = -fCurrentPos;
                } break;
                case FAULT_LINE_C: {
                    fCurrentNeg = -a * fCurrentPos;
                } break;
                default:
                    break;
            }
        } break;
        case FAULT_2LINE_GROUND: {
            std::complex<double> z1 = m_zBusPos[fNumber][fNumber];
            std::complex<double> z2 = m_zBusNeg[fNumber][fNumber];
            std::complex<double> z0 = m_zBusZero[fNumber][fNumber];
            std::complex<double> zf_3 = std::complex<double>(3.0, 0.0) * fImpedance;

            fCurrentPos = (preFaultVoltage * (z2 + z0 + zf_3)) / (z1 * z2 + z2 * z0 + z2 * zf_3 + z1 * z0 + z1 * zf_3);

            switch(fLocation) {
                case FAULT_LINE_A: {
                    fCurrentNeg = -a2 * ((preFaultVoltage - z1 * fCurrentPos) / z2);
                    fCurrentZero = -a * ((preFaultVoltage - z1 * fCurrentPos) / (z0 + zf_3));
                } break;
                case FAULT_LINE_B: {
                    fCurrentNeg = -((preFaultVoltage - z1 * fCurrentPos) / z2);
                    fCurrentZero = -((preFaultVoltage - z1 * fCurrentPos) / (z0 + zf_3));
                } break;
                case FAULT_LINE_C: {
                    fCurrentNeg = -a * ((preFaultVoltage - z1 * fCurrentPos) / z2);
                    fCurrentZero = -a2 * ((preFaultVoltage - z1 * fCurrentPos) / (z0 + zf_3));
                } break;
                default:
                    break;
            }
        } break;
        case FAULT_LINE_GROUND: {
            fCurrentPos =
                preFaultVoltage / (m_zBusPos[fNumber][fNumber] + m_zBusNeg[fNumber][fNumber] +
                                   m_zBusZero[fNumber][fNumber] + std::complex<double>(3.0, 0.0) * fImpedance);
            switch(fLocation) {
                case FAULT_LINE_A: {
                    fCurrentNeg = fCurrentPos;
                    fCurrentZero = fCurrentPos;
                } break;
                case FAULT_LINE_B: {
                    fCurrentNeg = a * fCurrentPos;
                    fCurrentZero = a2 * fCurrentPos;
                } break;
                case FAULT_LINE_C: {
                    fCurrentNeg = a2 * fCurrentPos;
                    fCurrentZero = a * fCurrentPos;
                } break;
                default:
                    break;
            }
        } break;
        default:
            break;
    }

    // Convert sequence currents to ABC. [Iabc] = [A]*[I012]
    m_fCurrentA = fCurrentZero + fCurrentPos + fCurrentNeg;
    m_fCurrentB = fCurrentZero + a2 * fCurrentPos + a * fCurrentNeg;
    m_fCurrentC = fCurrentZero + a * fCurrentPos + a2 * fCurrentNeg;

    // Pos-fault voltages calculation
    m_posFaultVoltagePos.clear();
    m_posFaultVoltageNeg.clear();
    m_posFaultVoltageZero.clear();
    m_posFaultVoltageA.clear();
    m_posFaultVoltageB.clear();
    m_posFaultVoltageC.clear();

    for(int i = 0; i < numberOfBuses; ++i) {
        m_posFaultVoltagePos.push_back(preFaultVoltages[i] - m_zBusPos[i][fNumber] * fCurrentPos);
        m_posFaultVoltageNeg.push_back(-m_zBusNeg[i][fNumber] * fCurrentNeg);
        m_posFaultVoltageZero.push_back(-m_zBusZero[i][fNumber] * fCurrentZero);

        // V012 -> Vabc
        m_posFaultVoltageA.push_back(m_posFaultVoltageZero[i] + m_posFaultVoltagePos[i] + m_posFaultVoltageNeg[i]);
        m_posFaultVoltageB.push_back(m_posFaultVoltageZero[i] + a2 * m_posFaultVoltagePos[i] +
                                     a * m_posFaultVoltageNeg[i]);
        m_posFaultVoltageC.push_back(m_posFaultVoltageZero[i] + a * m_posFaultVoltagePos[i] +
                                     a2 * m_posFaultVoltageNeg[i]);
    }

    UpdateElementsFault(systemPowerBase);
    return true;
}

void Fault::UpdateElementsFault(double systemPowerBase)
{
    std::complex<double> a = std::complex<double>(-0.5, 0.866025403784);
    std::complex<double> a2 = std::complex<double>(-0.5, -0.866025403784);

    for(auto it = m_busList.begin(), itEnd = m_busList.end(); it != itEnd; ++it) {
        Bus* bus = *it;
        auto data = bus->GetElectricalData();
        if(data.hasFault) {
            data.faultCurrent[0] = m_fCurrentA;
            data.faultCurrent[1] = m_fCurrentB;
            data.faultCurrent[2] = m_fCurrentC;
        } else {
            data.faultCurrent[0] = data.faultCurrent[1] = data.faultCurrent[2] = std::complex<double>(0.0, 0.0);
        }
        data.faultVoltage[0] = m_posFaultVoltageA[data.number];
        data.faultVoltage[1] = m_posFaultVoltageB[data.number];
        data.faultVoltage[2] = m_posFaultVoltageC[data.number];
        bus->SetElectricalData(data);
    }

    for(auto it = m_lineList.begin(), itEnd = m_lineList.end(); it != itEnd; ++it) {
        Line* line = *it;
        if(line->IsOnline()) {
            int n1 = static_cast<Bus*>(line->GetParentList()[0])->GetElectricalData().number;
            int n2 = static_cast<Bus*>(line->GetParentList()[1])->GetElectricalData().number;
            auto data = line->GetElectricalData();
            std::complex<double> vPos[2] = {m_posFaultVoltagePos[n1], m_posFaultVoltagePos[n2]};
            std::complex<double> vNeg[2] = {m_posFaultVoltageNeg[n1], m_posFaultVoltageNeg[n2]};
            std::complex<double> vZero[2] = {m_posFaultVoltageZero[n1], m_posFaultVoltageZero[n2]};
            std::complex<double> zPos(data.resistance, data.indReactance);
            std::complex<double> bPos(0.0, data.capSusceptance / 2.0);
            std::complex<double> zZero(data.zeroResistance, data.zeroIndReactance);
            std::complex<double> bZero(0.0, data.zeroCapSusceptance / 2.0);

            std::complex<double> lineCurrentPos[2];
            std::complex<double> lineCurrentNeg[2];
            std::complex<double> lineCurrentZero[2];

            lineCurrentPos[0] = ((vPos[0] - vPos[1]) / zPos) + (vPos[0] * bPos);
            lineCurrentNeg[0] = ((vNeg[0] - vNeg[1]) / zPos) + (vNeg[0] * bPos);
            lineCurrentZero[0] = ((vZero[0] - vZero[1]) / zZero) + (vZero[0] * bZero);
            lineCurrentPos[1] = ((vPos[1] - vPos[0]) / zPos) + (vPos[1] * bPos);
            lineCurrentNeg[1] = ((vNeg[1] - vNeg[0]) / zPos) + (vNeg[1] * bPos);
            lineCurrentZero[1] = ((vZero[1] - vZero[0]) / zZero) + (vZero[1] * bZero);

            data.faultCurrent[0][0] = lineCurrentZero[0] + lineCurrentPos[0] + lineCurrentNeg[0];
            data.faultCurrent[0][1] = lineCurrentZero[0] + a2 * lineCurrentPos[0] + a * lineCurrentNeg[0];
            data.faultCurrent[0][2] = lineCurrentZero[0] + a * lineCurrentPos[0] + a2 * lineCurrentNeg[0];
            data.faultCurrent[1][0] = lineCurrentZero[1] + lineCurrentPos[1] + lineCurrentNeg[1];
            data.faultCurrent[1][1] = lineCurrentZero[1] + a2 * lineCurrentPos[1] + a * lineCurrentNeg[1];
            data.faultCurrent[1][2] = lineCurrentZero[1] + a * lineCurrentPos[1] + a2 * lineCurrentNeg[1];

            line->SetElectricalData(data);
        }
    }

    for(auto it = m_transformerList.begin(), itEnd = m_transformerList.end(); it != itEnd; ++it) {
        Transformer* transformer = *it;
        if(transformer->IsOnline()) {
            int n1 = static_cast<Bus*>(transformer->GetParentList()[0])->GetElectricalData().number;
            int n2 = static_cast<Bus*>(transformer->GetParentList()[1])->GetElectricalData().number;
            auto data = transformer->GetElectricalData();

            std::complex<double> vPos[2] = {m_posFaultVoltagePos[n1], m_posFaultVoltagePos[n2]};
            std::complex<double> vNeg[2] = {m_posFaultVoltageNeg[n1], m_posFaultVoltageNeg[n2]};
            std::complex<double> vZero[2] = {m_posFaultVoltageZero[n1], m_posFaultVoltageZero[n2]};
            std::complex<double> zPos(data.resistance, data.indReactance);
            std::complex<double> zZero(data.zeroResistance, data.zeroIndReactance);

            std::complex<double> transformerCurrentPos[2];
            std::complex<double> transformerCurrentNeg[2];
            std::complex<double> transformerCurrentZero[2];

            if(data.turnsRatio == 1.0 && data.phaseShift == 0.0) {
                transformerCurrentPos[0] = (vPos[0] - vPos[1]) / zPos;
                transformerCurrentNeg[0] = (vNeg[0] - vNeg[1]) / zPos;
                transformerCurrentZero[0] = (vZero[0] - vZero[1]) / zZero;
                transformerCurrentPos[1] = (vPos[1] - vPos[0]) / zPos;
                transformerCurrentNeg[1] = (vNeg[1] - vNeg[0]) / zPos;
                transformerCurrentZero[1] = (vZero[1] - vZero[0]) / zZero;
            } else {
                double radPhaseShift = wxDegToRad(data.phaseShift);
                std::complex<double> t = std::complex<double>(data.turnsRatio * std::cos(radPhaseShift),
                                                              -data.turnsRatio * std::sin(radPhaseShift));

                transformerCurrentPos[0] =
                    vPos[0] * (1.0 / (std::pow(std::abs(t), 2.0) * zPos)) - vPos[1] * (1.0 / (std::conj(t) * zPos));
                transformerCurrentNeg[0] =
                    vNeg[0] * (1.0 / (std::pow(std::abs(t), 2.0) * zPos)) - vNeg[1] * (1.0 / (t * zPos));

                transformerCurrentPos[1] = -vPos[0] * (1.0 / (t * zPos)) + vPos[1] / zPos;
                transformerCurrentNeg[1] = -vNeg[0] * (1.0 / (std::conj(t) * zPos)) + vNeg[1] / zPos;
            }

            switch(data.connection) {
                case GWYE_GWYE: {
                    transformerCurrentZero[0] = (vZero[0] - vZero[1]) / zZero;
                    transformerCurrentZero[1] = (vZero[1] - vZero[0]) / zZero;
                    break;
                }
                case GWYE_DELTA: {
                    transformerCurrentZero[0] = vZero[0] / zZero;
                    transformerCurrentZero[1] = std::complex<double>(0.0, 0.0);
                    break;
                }
                case DELTA_GWYE: {
                    transformerCurrentZero[0] = std::complex<double>(0.0, 0.0);
                    transformerCurrentZero[1] = vZero[1] / zZero;
                    break;
                }
                default: {
                    transformerCurrentZero[0] = std::complex<double>(0.0, 0.0);
                    transformerCurrentZero[1] = std::complex<double>(0.0, 0.0);
                    break;
                }
            }

            data.faultCurrent[0][0] = transformerCurrentZero[0] + transformerCurrentPos[0] + transformerCurrentNeg[0];
            data.faultCurrent[0][1] =
                transformerCurrentZero[0] + a2 * transformerCurrentPos[0] + a * transformerCurrentNeg[0];
            data.faultCurrent[0][2] =
                transformerCurrentZero[0] + a * transformerCurrentPos[0] + a2 * transformerCurrentNeg[0];
            data.faultCurrent[1][0] = transformerCurrentZero[1] + transformerCurrentPos[1] + transformerCurrentNeg[1];
            data.faultCurrent[1][1] =
                transformerCurrentZero[1] + a2 * transformerCurrentPos[1] + a * transformerCurrentNeg[1];
            data.faultCurrent[1][2] =
                transformerCurrentZero[1] + a * transformerCurrentPos[1] + a2 * transformerCurrentNeg[1];

            transformer->SetElectricaData(data);
        }
    }

    for(auto it = m_syncGeneratorList.begin(), itEnd = m_syncGeneratorList.end(); it != itEnd; ++it) {
        SyncGenerator* syncGenerator = *it;
        if(syncGenerator->IsOnline()) {
            Bus* bus = static_cast<Bus*>(syncGenerator->GetParentList()[0]);
            int n = bus->GetElectricalData().number;
            std::complex<double> v = bus->GetElectricalData().voltage;  // Pre-fault voltage.
            auto data = syncGenerator->GetElectricalData();

            std::complex<double> vPos = m_posFaultVoltagePos[n];
            std::complex<double> vNeg = m_posFaultVoltageNeg[n];
            std::complex<double> vZero = m_posFaultVoltageZero[n];

            std::complex<double> zPos(data.positiveResistance, data.positiveReactance);
            std::complex<double> zNeg(data.negativeResistance, data.negativeReactance);
            std::complex<double> zZero(data.zeroResistance + 3.0 * data.groundResistance,
                                       data.negativeReactance + 3.0 * data.groundReactance);

            std::complex<double> syncGeneratorCurrentPos = (v - vPos) / zPos;
            std::complex<double> syncGeneratorCurrentNeg = (-vNeg) / zNeg;
            std::complex<double> syncGeneratorCurrentZero(0.0, 0.0);
            if(data.groundNeutral) syncGeneratorCurrentZero = (-vZero) / zZero;

            data.faultCurrent[0] = syncGeneratorCurrentPos + syncGeneratorCurrentNeg + syncGeneratorCurrentZero;
            data.faultCurrent[1] =
                syncGeneratorCurrentPos + a2 * syncGeneratorCurrentNeg + a * syncGeneratorCurrentZero;
            data.faultCurrent[2] =
                syncGeneratorCurrentPos + a * syncGeneratorCurrentNeg + a2 * syncGeneratorCurrentZero;

            syncGenerator->SetElectricalData(data);
        }
    }

    for(auto it = m_syncMotorList.begin(), itEnd = m_syncMotorList.end(); it != itEnd; ++it) {
        SyncMotor* syncMotor = *it;
        if(syncMotor->IsOnline()) {
            Bus* bus = static_cast<Bus*>(syncMotor->GetParentList()[0]);
            int n = bus->GetElectricalData().number;
            std::complex<double> v = bus->GetElectricalData().voltage;  // Pre-fault voltage.
            auto data = syncMotor->GetElectricalData();

            std::complex<double> vPos = m_posFaultVoltagePos[n];
            std::complex<double> vNeg = m_posFaultVoltageNeg[n];
            std::complex<double> vZero = m_posFaultVoltageZero[n];

            std::complex<double> zPos(data.positiveResistance, data.positiveReactance);
            std::complex<double> zNeg(data.negativeResistance, data.negativeReactance);
            std::complex<double> zZero(data.zeroResistance + 3.0 * data.groundResistance,
                                       data.negativeReactance + 3.0 * data.groundReactance);

            std::complex<double> syncGeneratorCurrentPos = (v - vPos) / zPos;
            std::complex<double> syncGeneratorCurrentNeg = (-vNeg) / zNeg;
            std::complex<double> syncGeneratorCurrentZero(0.0, 0.0);
            if(data.groundNeutral) syncGeneratorCurrentZero = (-vZero) / zZero;

            data.faultCurrent[0] = syncGeneratorCurrentZero + syncGeneratorCurrentPos + syncGeneratorCurrentNeg;
            data.faultCurrent[1] =
                syncGeneratorCurrentZero + a2 * syncGeneratorCurrentPos + a * syncGeneratorCurrentNeg;
            data.faultCurrent[2] =
                syncGeneratorCurrentZero + a * syncGeneratorCurrentPos + a2 * syncGeneratorCurrentNeg;

            syncMotor->SetElectricalData(data);
        }
    }
}

bool Fault::RunSCPowerCalcutation(double systemPowerBase)
{
    // Get adimittance matrix.
    std::vector<std::vector<std::complex<double> > > yBusPos;
    GetYBus(yBusPos, systemPowerBase, POSITIVE_SEQ, true);

    // Calculate the impedance matrix.
    if(!InvertMatrix(yBusPos, m_zBusPos)) {
        m_errorMsg = _("Fail to invert the positive sequence admittance matrix.");
        return false;
    }

    // Set the SC power.
    for(auto it = m_busList.begin(), itEnd = m_busList.end(); it != itEnd; ++it) {
        Bus* bus = *it;
        auto data = bus->GetElectricalData();
        int n = data.number;
        data.scPower = 1.0 / std::abs(m_zBusPos[n][n]);
        bus->SetElectricalData(data);
    }

    return true;
}
