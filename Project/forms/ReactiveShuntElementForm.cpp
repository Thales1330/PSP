/*
 *  Copyright (C) 2017  Thales Lima Oliveira <thales@ufu.br>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ReactiveShuntElementForm.h"
#include "SwitchingForm.h"
#include "../elements/powerElement/Capacitor.h"
#include "../elements/powerElement/Inductor.h"

ReactiveShuntElementForm::ReactiveShuntElementForm(wxWindow* parent, Capacitor* capacitor)
    : ReactiveShuntElementFormBase(parent)
{
    SetSize(GetBestSize());
    CapacitorElectricalData data = capacitor->GetElectricalData();

    m_textCtrlName->SetValue(data.name);

    m_textCtrlReactivePower->SetValue(Capacitor::StringFromDouble(data.reactivePower));
    switch(data.reactivePowerUnit) {
        case ElectricalUnit::UNIT_PU: {
            m_choiceReactivePower->SetSelection(0);
        } break;
        case ElectricalUnit::UNIT_var: {
            m_choiceReactivePower->SetSelection(1);
        } break;
        case ElectricalUnit::UNIT_kvar: {
            m_choiceReactivePower->SetSelection(2);
        } break;
        case ElectricalUnit::UNIT_Mvar: {
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
        case ElectricalUnit::UNIT_PU: {
            m_choiceReactivePower->SetSelection(0);
        } break;
        case ElectricalUnit::UNIT_var: {
            m_choiceReactivePower->SetSelection(1);
        } break;
        case ElectricalUnit::UNIT_kvar: {
            m_choiceReactivePower->SetSelection(2);
        } break;
        case ElectricalUnit::UNIT_Mvar: {
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
                data.reactivePowerUnit = ElectricalUnit::UNIT_PU;
            } break;
            case 1: {
                data.reactivePowerUnit = ElectricalUnit::UNIT_var;
            } break;
            case 2: {
                data.reactivePowerUnit = ElectricalUnit::UNIT_kvar;
            } break;
            case 3: {
                data.reactivePowerUnit = ElectricalUnit::UNIT_Mvar;
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
                data.reactivePowerUnit = ElectricalUnit::UNIT_PU;
            } break;
            case 1: {
                data.reactivePowerUnit = ElectricalUnit::UNIT_var;
            } break;
            case 2: {
                data.reactivePowerUnit = ElectricalUnit::UNIT_kvar;
            } break;
            case 3: {
                data.reactivePowerUnit = ElectricalUnit::UNIT_Mvar;
            } break;
        }

        m_inductor->SetElectricalData(data);
    }
    return true;
}
