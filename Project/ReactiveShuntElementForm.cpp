#include "ReactiveShuntElementForm.h"
#include "SwitchingForm.h"
#include "Capacitor.h"
#include "Inductor.h"

ReactiveShuntElementForm::ReactiveShuntElementForm(wxWindow* parent, Capacitor* capacitor)
    : ReactiveShuntElementFormBase(parent)
{
    SetSize(GetBestSize());
    CapacitorElectricalData data = capacitor->GetElectricalData();

    m_textCtrlName->SetValue(data.name);

    m_textCtrlReactivePower->SetValue(Capacitor::StringFromDouble(data.reactivePower));
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

    m_parent = parent;
    m_capacitor = capacitor;
}

ReactiveShuntElementForm::ReactiveShuntElementForm(wxWindow* parent, Inductor* inductor)
    : ReactiveShuntElementFormBase(parent)
{
    InductorElectricalData data = inductor->GetElectricalData();

    m_textCtrlName->SetValue(data.name);

    m_textCtrlReactivePower->SetValue(Inductor::StringFromDouble(data.reactivePower));
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

    m_parent = parent;
    m_inductor = inductor;
}

ReactiveShuntElementForm::~ReactiveShuntElementForm() {}
void ReactiveShuntElementForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

void ReactiveShuntElementForm::OnStabilityButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) {
        if(m_capacitor) {
            SwitchingForm swForm(m_parent, m_capacitor);
            swForm.SetTitle(_("Capacitor: Switching"));
            swForm.ShowModal();
        } else if(m_inductor) {
            SwitchingForm swForm(m_parent, m_inductor);
            swForm.SetTitle(_("Inductor: Switching"));
            swForm.ShowModal();
        }

        EndModal(wxID_OK);
    }
}

bool ReactiveShuntElementForm::ValidateData()
{
    if(m_capacitor) {
        CapacitorElectricalData data;

        data.name = m_textCtrlName->GetValue();

        if(!m_capacitor->DoubleFromString(m_parent, m_textCtrlReactivePower->GetValue(), data.reactivePower,
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

        m_capacitor->SetElectricalData(data);
    } else if(m_inductor) {
        InductorElectricalData data;

        data.name = m_textCtrlName->GetValue();

        if(!m_inductor->DoubleFromString(m_parent, m_textCtrlReactivePower->GetValue(), data.reactivePower,
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

        m_inductor->SetElectricalData(data);
    }
    return true;
}
