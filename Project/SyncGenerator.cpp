#include "SyncMachineForm.h"
#include "SyncGenerator.h"
#include "ControlElementContainer.h"

SyncGenerator::SyncGenerator() : Machines() { Init(); }
SyncGenerator::SyncGenerator(wxString name) : Machines()
{
    Init();
    m_electricalData.name = name;
}

SyncGenerator::~SyncGenerator() {}
void SyncGenerator::Init()
{
    int numPtsSine = 10;
    double mx = 15.0;
    double my = 10.0;
    double pi = 3.14159265359;

    for(int i = 0; i <= numPtsSine; i++) {
        double x = (2.0 * pi / double(numPtsSine)) * double(i) - pi;
        double y = std::sin(x);
        m_sinePts.push_back(wxPoint2DDouble((x / pi) * mx, y * my));
    }

    m_electricalData.avr = new ControlElementContainer();
    m_electricalData.speedGov = new ControlElementContainer();
}

void SyncGenerator::DrawSymbol() const
{
    // Draw sine.
    std::vector<wxPoint2DDouble> sinePts;
    for(int i = 0; i < (int)m_sinePts.size(); i++) {
        sinePts.push_back(m_sinePts[i] + m_position);
    }
    DrawLine(sinePts);
}
bool SyncGenerator::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_ELEMENT, _("Edit Generator"));
    GeneralMenuItens(menu);
    return true;
}

bool SyncGenerator::ShowForm(wxWindow* parent, Element* element)
{
    SyncMachineForm* generatorForm = new SyncMachineForm(parent, this);
    generatorForm->SetTitle(_("Generator"));
    if(generatorForm->ShowModal() == wxID_OK) {
        generatorForm->Destroy();
        return true;
    }

    generatorForm->Destroy();
    return false;
}

SyncGeneratorElectricalData SyncGenerator::GetPUElectricalData(double systemPowerBase)
{
    SyncGeneratorElectricalData data = m_electricalData;
    double machineBasePower = 1.0;
    if(data.useMachineBase) {
        machineBasePower = GetValueFromUnit(data.nominalPower, data.nominalPowerUnit);
    }

    // Active power
    double activePower = GetValueFromUnit(data.activePower, data.activePowerUnit);
    if(!m_online) activePower = 0.0;
    if(data.activePowerUnit == UNIT_PU) {
        if(data.useMachineBase) data.activePower = (activePower * machineBasePower) / systemPowerBase;
    } else {
        data.activePower = activePower / systemPowerBase;
    }
    data.activePowerUnit = UNIT_PU;

    // Reactive power
    double reactivePower = GetValueFromUnit(data.reactivePower, data.reactivePowerUnit);
    if(!m_online) reactivePower = 0.0;
    if(data.reactivePowerUnit == UNIT_PU) {
        if(data.useMachineBase) data.reactivePower = (reactivePower * machineBasePower) / systemPowerBase;
    } else {
        data.reactivePower = reactivePower / systemPowerBase;
    }
    data.reactivePowerUnit = UNIT_PU;

    // Max reactive power
    double maxReactive = GetValueFromUnit(data.maxReactive, data.maxReactiveUnit);
    if(data.maxReactiveUnit == UNIT_PU) {
        if(data.useMachineBase) data.maxReactive = (maxReactive * machineBasePower) / systemPowerBase;
    } else {
        data.maxReactive = maxReactive / systemPowerBase;
    }
    data.maxReactiveUnit = UNIT_PU;

    // Min reactive power
    double minReactive = GetValueFromUnit(data.minReactive, data.minReactiveUnit);
    if(data.minReactiveUnit == UNIT_PU) {
        if(data.useMachineBase) data.minReactive = (minReactive * machineBasePower) / systemPowerBase;
    } else {
        data.minReactive = minReactive / systemPowerBase;
    }
    data.minReactiveUnit = UNIT_PU;

    double baseVoltage = GetValueFromUnit(data.nominalVoltage, data.nominalVoltageUnit);
    double systemBaseImpedance = (baseVoltage * baseVoltage) / systemPowerBase;
    double machineBaseImpedance = (baseVoltage * baseVoltage) / machineBasePower;

    // Fault data

    // Pos. seq. resistance
    double r1 = data.positiveResistance;
    if(data.useMachineBase) data.positiveResistance = (r1 * machineBaseImpedance) / systemBaseImpedance;

    // Pos. seq. reactance
    double x1 = data.positiveReactance;
    if(data.useMachineBase) data.positiveReactance = (x1 * machineBaseImpedance) / systemBaseImpedance;

    // Neg. seq. resistance
    double r2 = data.negativeResistance;
    if(data.useMachineBase) data.negativeResistance = (r2 * machineBaseImpedance) / systemBaseImpedance;

    // Neg. seq. reactance
    double x2 = data.negativeReactance;
    if(data.useMachineBase) data.negativeReactance = (x2 * machineBaseImpedance) / systemBaseImpedance;

    // Zero seq. resistance
    double r0 = data.zeroResistance;
    if(data.useMachineBase) data.zeroResistance = (r0 * machineBaseImpedance) / systemBaseImpedance;

    // Zero seq. reactance
    double x0 = data.zeroReactance;
    if(data.useMachineBase) data.zeroReactance = (x0 * machineBaseImpedance) / systemBaseImpedance;
    
    if(!m_online) {
        data.faultCurrent[0] = std::complex<double>(0,0);
        data.faultCurrent[1] = std::complex<double>(0,0);
        data.faultCurrent[2] = std::complex<double>(0,0);
    }

    return data;
}

void SyncGenerator::SetNominalVoltage(std::vector<double> nominalVoltage,
                                      std::vector<ElectricalUnit> nominalVoltageUnit)
{
    if(nominalVoltage.size() > 0) {
        m_electricalData.nominalVoltage = nominalVoltage[0];
        m_electricalData.nominalVoltageUnit = nominalVoltageUnit[0];
    }
}

Element* SyncGenerator::GetCopy()
{
    SyncGenerator* copy = new SyncGenerator();
    *copy = *this;
    auto data = copy->GetElectricalData();

    // Copy AVR
    std::vector<ConnectionLine*> cLineList;
    std::vector<ControlElement*> elementList;
    m_electricalData.avr->GetContainerCopy(elementList, cLineList);

    ControlElementContainer* avrCopy = new ControlElementContainer();
    avrCopy->FillContainer(elementList, cLineList);
    data.avr = avrCopy;

    // Copy Speed Governor
    cLineList.clear();
    elementList.clear();
    m_electricalData.speedGov->GetContainerCopy(elementList, cLineList);

    ControlElementContainer* speedGovCopy = new ControlElementContainer();
    speedGovCopy->FillContainer(elementList, cLineList);
    data.speedGov = speedGovCopy;

    copy->SetElectricalData(data);
    return copy;
}

wxString SyncGenerator::GetTipText() const
{
    wxString tipText = m_electricalData.name;
    tipText += "\n";
    double activePower = m_electricalData.activePower;
    if(!m_online) activePower = 0.0;
    tipText += _("\nP = ") + wxString::FromDouble(activePower, 5);
    switch(m_electricalData.activePowerUnit) {
        case UNIT_PU: {
            tipText += _(" p.u.");
        } break;
        case UNIT_W: {
            tipText += _(" W");
        } break;
        case UNIT_kW: {
            tipText += _(" kW");
        } break;
        case UNIT_MW: {
            tipText += _(" MW");
        } break;
        default:
            break;
    }
    double reactivePower = m_electricalData.reactivePower;
    if(!m_online) reactivePower = 0.0;
    tipText += _("\nQ = ") + wxString::FromDouble(reactivePower, 5);
    switch(m_electricalData.reactivePowerUnit) {
        case UNIT_PU: {
            tipText += _(" p.u.");
        } break;
        case UNIT_VAr: {
            tipText += _(" VAr");
        } break;
        case UNIT_kVAr: {
            tipText += _(" kVAr");
        } break;
        case UNIT_MVAr: {
            tipText += _(" MVAr");
        } break;
        default:
            break;
    }

    return tipText;
}

bool SyncGenerator::GetPlotData(ElementPlotData& plotData)
{
    if(!m_electricalData.plotSyncMachine) return false;
    plotData.SetName(m_electricalData.name);
    plotData.SetCurveType(ElementPlotData::CT_SYNC_GENERATOR);

    std::vector<double> absTerminalVoltage, activePower, reactivePower;
    for(unsigned int i = 0; i < m_electricalData.terminalVoltageVector.size(); ++i) {
        absTerminalVoltage.push_back(std::abs(m_electricalData.terminalVoltageVector[i]));
        activePower.push_back(std::real(m_electricalData.electricalPowerVector[i]));
        reactivePower.push_back(std::imag(m_electricalData.electricalPowerVector[i]));
    }
    plotData.AddData(absTerminalVoltage, _("Terminal voltage"));
    plotData.AddData(activePower, _("Active power"));
    plotData.AddData(reactivePower, _("Reactive power"));
    plotData.AddData(m_electricalData.mechanicalPowerVector, _("Mechanical power"));
    plotData.AddData(m_electricalData.freqVector, _("Frequency"));
    plotData.AddData(m_electricalData.fieldVoltageVector, _("Field voltage"));
    plotData.AddData(m_electricalData.deltaVector, _("Delta"));
    return true;
}
