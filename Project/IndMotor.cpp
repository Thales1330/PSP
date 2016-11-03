#include "IndMotorForm.h"
#include "IndMotor.h"

IndMotor::IndMotor() : Machines() { }
IndMotor::IndMotor(wxString name) : Machines()
{
    m_electricalData.name = name;
}
IndMotor::~IndMotor() {}
void IndMotor::DrawSymbol() const
{
    std::vector<wxPoint2DDouble> mPts;
    mPts.push_back(wxPoint2DDouble(-10, 13) + m_position);
    mPts.push_back(wxPoint2DDouble(-10, -13) + m_position);
    mPts.push_back(wxPoint2DDouble(0, 2) + m_position);
    mPts.push_back(wxPoint2DDouble(10, -13) + m_position);
    mPts.push_back(wxPoint2DDouble(10, 13) + m_position);
    DrawLine(mPts);
}

bool IndMotor::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_INDMOTOR, _("Edit induction motor"));
    GeneralMenuItens(menu);
    return true;
}

bool IndMotor::ShowForm(wxWindow* parent, Element* element)
{
    IndMotorForm* indMotorForm = new IndMotorForm(parent, this);
    if(indMotorForm->ShowModal() == wxID_OK) {
        indMotorForm->Destroy();
        return true;
    }
    indMotorForm->Destroy();
    return false;
}

IndMotorElectricalData IndMotor::GetPUElectricalData(double systemPowerBase)
{
	IndMotorElectricalData data = m_electricalData;
	
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
	
	return data;
}
