#include "SyncMachineForm.h"
#include "SyncMotor.h"

SyncMotor::SyncMotor() : Machines() {}
SyncMotor::SyncMotor(wxString name) : Machines() { m_electricalData.name = name; }
SyncMotor::~SyncMotor() {}
void SyncMotor::DrawSymbol() const { DrawArc(m_position, 12, 30, 330, 10, GL_LINE_STRIP); }
bool SyncMotor::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_ELEMENT, _("Edit Synchronous Condenser"));
    GeneralMenuItens(menu);

    return true;
}

bool SyncMotor::ShowForm(wxWindow* parent, Element* element)
{
    SyncMachineForm* syncMotorForm = new SyncMachineForm(parent, this);
    syncMotorForm->SetTitle(_("Synchronous Condenser"));
    if(syncMotorForm->ShowModal() == wxID_OK) {
        syncMotorForm->Destroy();
        return true;
    }

    syncMotorForm->Destroy();
    return false;
}

SyncMotorElectricalData SyncMotor::GetPUElectricalData(double systemPowerBase)
{
    SyncMotorElectricalData data = m_electricalData;
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

void SyncMotor::SetNominalVoltage(std::vector<double> nominalVoltage, std::vector<ElectricalUnit> nominalVoltageUnit)
{
    if(nominalVoltage.size() > 0) {
        m_electricalData.nominalVoltage = nominalVoltage[0];
        m_electricalData.nominalVoltageUnit = nominalVoltageUnit[0];
    }
}

Element* SyncMotor::GetCopy()
{
    SyncMotor* copy = new SyncMotor();
    *copy = *this;
    return copy;
}

wxString SyncMotor::GetTipText() const
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
