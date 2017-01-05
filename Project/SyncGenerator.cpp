#include "SyncMachineForm.h"
#include "SyncGenerator.h"

SyncGenerator::SyncGenerator() : Machines()
{
    Init();
}

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

void SyncGenerator::SetNominalVoltage(std::vector<double> nominalVoltage, std::vector<ElectricalUnit> nominalVoltageUnit)
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
	return copy;
}

wxString SyncGenerator::GetTipText() const
{
    wxString tipText = m_electricalData.name;
    tipText += "\n";
    tipText += _("\nP = ") + wxString::FromDouble(m_electricalData.activePower, 5);
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
    tipText += _("\nQ = ") + wxString::FromDouble(m_electricalData.reactivePower, 5);
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
