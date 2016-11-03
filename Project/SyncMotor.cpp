#include "SyncMachineForm.h"
#include "SyncMotor.h"

SyncMotor::SyncMotor() : Machines() {}
SyncMotor::SyncMotor(wxString name) : Machines() { m_electricalData.name = name; }
SyncMotor::~SyncMotor() {}
void SyncMotor::DrawSymbol() const { DrawArc(m_position, 12, 30, 330, 10, GL_LINE_STRIP); }
bool SyncMotor::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_SYNCMOTOR, _("Edit Synchronous Condenser"));
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
	
	switch(data.activePowerUnit) {
        case UNIT_W: {
            data.activePower = data.activePower / systemPowerBase;
            data.activePowerUnit = UNIT_PU;
        } break;
        case UNIT_kW: {
            data.activePower = (data.activePower * 1e3) / systemPowerBase;
            data.activePowerUnit = UNIT_PU;
        } break;
        case UNIT_MW: {
            data.activePower = (data.activePower * 1e6) / systemPowerBase;
            data.activePowerUnit = UNIT_PU;
        } break;
        default:
            break;
    }
    switch(data.reactivePowerUnit) {
        case UNIT_VAr: {
            data.reactivePower = data.reactivePower / systemPowerBase;
            data.reactivePowerUnit = UNIT_PU;
        } break;
        case UNIT_kVAr: {
            data.reactivePower = (data.reactivePower * 1e3) / systemPowerBase;
            data.reactivePowerUnit = UNIT_PU;
        } break;
        case UNIT_MVAr: {
            data.reactivePower = (data.reactivePower * 1e6) / systemPowerBase;
            data.reactivePowerUnit = UNIT_PU;
        } break;
        default:
            break;
    }
	switch(data.maxReactiveUnit) {
        case UNIT_VAr: {
            data.maxReactive = data.maxReactive / systemPowerBase;
            data.maxReactiveUnit = UNIT_PU;
        } break;
        case UNIT_kVAr: {
            data.maxReactive = (data.maxReactive * 1e3) / systemPowerBase;
            data.maxReactiveUnit = UNIT_PU;
        } break;
        case UNIT_MVAr: {
            data.maxReactive = (data.maxReactive * 1e6) / systemPowerBase;
            data.maxReactiveUnit = UNIT_PU;
        } break;
        default:
            break;
    }
	switch(data.minReactiveUnit) {
        case UNIT_VAr: {
            data.minReactive = data.minReactive / systemPowerBase;
            data.minReactiveUnit = UNIT_PU;
        } break;
        case UNIT_kVAr: {
            data.minReactive = (data.minReactive * 1e3) / systemPowerBase;
            data.minReactiveUnit = UNIT_PU;
        } break;
        case UNIT_MVAr: {
            data.minReactive = (data.minReactive * 1e6) / systemPowerBase;
            data.minReactiveUnit = UNIT_PU;
        } break;
        default:
            break;
    }
	
	return data;
}
