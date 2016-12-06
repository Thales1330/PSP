#include "LoadForm.h"
#include "SwitchingForm.h"
#include "Load.h"

LoadForm::LoadForm(wxWindow* parent, Load* load) : LoadFormBase(parent)
{
    LoadElectricalData data = load->GetElectricalData();

    m_textCtrlName->SetValue(data.name);

    m_textCtrlActivePower->SetValue(Load::StringFromDouble(data.activePower));
    switch(data.activePowerUnit) {
        case UNIT_PU: {
            m_choiceActivePower->SetSelection(0);
        } break;
        case UNIT_W: {
            m_choiceActivePower->SetSelection(1);
        } break;
        case UNIT_kW: {
            m_choiceActivePower->SetSelection(2);
        } break;
        case UNIT_MW: {
            m_choiceActivePower->SetSelection(3);
        } break;
        default:
            break;
    }

    m_textCtrlReactivePower->SetValue(Load::StringFromDouble(data.reactivePower));
    switch(data.reactivePowerUnit) {
        case UNIT_PU: {
            m_choiceReactivePower->SetSelection(0);
        } break;
        case UNIT_VAr: {
            m_choiceReactivePower->SetSelection(1);
        } break;
        case UNIT_kVAr: {
            m_choiceReactivePower->SetSelection(2);
        } break;
        case UNIT_MVAr: {
            m_choiceReactivePower->SetSelection(3);
        } break;
        default:
            break;
    }

    switch(data.loadType) {
        case CONST_POWER: {
            m_choiceType->SetSelection(0);
        } break;
        case CONST_IMPEDANCE: {
            m_choiceType->SetSelection(1);
        } break;
    }

    m_parent = parent;
    m_load = load;
}

LoadForm::~LoadForm() {}
void LoadForm::OnOnButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

void LoadForm::OnStabilityButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) {
        SwitchingForm swForm(m_parent, m_load);
        swForm.SetTitle(_("Load: Switching"));
        swForm.ShowModal();
        EndModal(wxID_OK);
    }
}

bool LoadForm::ValidateData()
{
    LoadElectricalData data;

    data.name = m_textCtrlName->GetValue();

    if(!m_load->DoubleFromString(m_parent, m_textCtrlActivePower->GetValue(), data.activePower,
                                 _("Value entered incorrectly in the field \"Active power\".")))
        return false;
    switch(m_choiceActivePower->GetSelection()) {
        case 0: {
            data.activePowerUnit = UNIT_PU;
        } break;
        case 1: {
            data.activePowerUnit = UNIT_W;
        } break;
        case 2: {
            data.activePowerUnit = UNIT_kW;
        } break;
        case 3: {
            data.activePowerUnit = UNIT_MW;
        } break;
    }

    if(!m_load->DoubleFromString(m_parent, m_textCtrlReactivePower->GetValue(), data.reactivePower,
                                 _("Value entered incorrectly in the field \"Reactive power\".")))
        return false;
    switch(m_choiceReactivePower->GetSelection()) {
        case 0: {
            data.reactivePowerUnit = UNIT_PU;
        } break;
        case 1: {
            data.reactivePowerUnit = UNIT_VAr;
        } break;
        case 2: {
            data.reactivePowerUnit = UNIT_kVAr;
        } break;
        case 3: {
            data.reactivePowerUnit = UNIT_MVAr;
        } break;
    }

    switch(m_choiceType->GetSelection()) {
        case 0: {
            data.loadType = CONST_POWER;
        } break;
        case 1: {
            data.loadType = CONST_IMPEDANCE;
        } break;
    }

    m_load->SetElectricalData(data);
    return true;
}
