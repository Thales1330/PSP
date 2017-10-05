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

#include "TransformerForm.h"
#include "SwitchingForm.h"
#include "Transformer.h"

TransformerForm::TransformerForm(wxWindow* parent, Transformer* transformer) : TransformerFormBase(parent)
{
    m_choiceResistance->SetString(1, L'\u03A9');
    m_choiceReactance->SetString(1, L'\u03A9');

    // Reset connections choice labels (to be translated)
    m_choiceConnection->SetString(0, _("Grounded Wye - Grounded Wye"));
    m_choiceConnection->SetString(1, _("Wye - Grounded Wye"));
    m_choiceConnection->SetString(2, _("Grounded Wye - Wye"));
    m_choiceConnection->SetString(3, _("Wye - Wye"));
    m_choiceConnection->SetString(4, _("Delta - Grounded Wye"));
    m_choiceConnection->SetString(5, _("Delta - Wye"));
    m_choiceConnection->SetString(6, _("Grounded Wye - Delta"));
    m_choiceConnection->SetString(7, _("Wye - Delta"));
    m_choiceConnection->SetString(8, _("Delta - Delta"));

    SetSize(GetBestSize());
    Layout();

    m_parent = parent;
    m_transformer = transformer;

    TransformerElectricalData data = transformer->GetElectricalData();

    m_textCtrlName->SetValue(data.name);

    wxString primVoltStr = Transformer::StringFromDouble(data.primaryNominalVoltage);
    switch(data.primaryNominalVoltageUnit) {
        case UNIT_V: {
            primVoltStr += " V";
        } break;
        case UNIT_kV: {
            primVoltStr += " kV";
        } break;
        default:
            break;
    }
    wxString secVoltStr = Transformer::StringFromDouble(data.secondaryNominalVoltage);
    switch(data.secondaryNominalVoltageUnit) {
        case UNIT_V: {
            secVoltStr += " V";
        } break;
        case UNIT_kV: {
            secVoltStr += " kV";
        } break;
        default:
            break;
    }
    m_staticTextNominalVoltageValue->SetLabel(wxString::Format("%s / %s", primVoltStr, secVoltStr));

    m_choiceBaseVoltage->SetString(0, primVoltStr);
    m_choiceBaseVoltage->SetString(1, secVoltStr);
    m_choiceBaseVoltage->SetSelection(data.baseVoltage);

    m_textCtrlNominalPower->SetValue(Transformer::StringFromDouble(data.nominalPower));
    switch(data.nominalPowerUnit) {
        case UNIT_VA: {
            m_choiceNominalPower->SetSelection(0);
        } break;
        case UNIT_kVA: {
            m_choiceNominalPower->SetSelection(1);
        } break;
        case UNIT_MVA: {
            m_choiceNominalPower->SetSelection(2);
        } break;
        default:
            break;
    }

    m_textCtrlResistance->SetValue(Transformer::StringFromDouble(data.resistance));
    switch(data.resistanceUnit) {
        case UNIT_PU: {
            m_choiceResistance->SetSelection(0);
        } break;
        case UNIT_OHM: {
            m_choiceResistance->SetSelection(1);
        } break;
        default:
            break;
    }

    m_textCtrlReactance->SetValue(Transformer::StringFromDouble(data.indReactance));
    switch(data.indReactanceUnit) {
        case UNIT_PU: {
            m_choiceReactance->SetSelection(0);
        } break;
        case UNIT_OHM: {
            m_choiceReactance->SetSelection(1);
        } break;
        default:
            break;
    }

    m_choiceConnection->SetSelection(data.connection);

    m_textCtrlTurnRatio->SetValue(Transformer::StringFromDouble(data.turnsRatio));
    m_textCtrlPhaseShift->SetValue(Transformer::StringFromDouble(data.phaseShift));

    m_checkUseTransformerPower->SetValue(data.useTransformerPower);

    m_textCtrlZeroResistance->SetValue(Transformer::StringFromDouble(data.zeroResistance));
    m_textCtrlZeroReactance->SetValue(Transformer::StringFromDouble(data.zeroIndReactance));
    m_textCtrlPrimResistance->SetValue(Transformer::StringFromDouble(data.primaryGrndResistance));
    m_textCtrlPrimReactance->SetValue(Transformer::StringFromDouble(data.primaryGrndReactance));
    m_textCtrlSecResistance->SetValue(Transformer::StringFromDouble(data.secondaryGrndResistance));
    m_textCtrlSecReactance->SetValue(Transformer::StringFromDouble(data.secondaryGrndReactance));
}

TransformerForm::~TransformerForm() {}
void TransformerForm::OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
void TransformerForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}
void TransformerForm::OnStabilityButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) {
        SwitchingForm swForm(m_parent, m_transformer);
        swForm.SetTitle(_("Transfomer: Switching"));
        swForm.ShowModal();
        EndModal(wxID_OK);
    }
}

bool TransformerForm::ValidateData()
{
    TransformerElectricalData data = m_transformer->GetElectricalData();

    data.name = m_textCtrlName->GetValue();
    data.baseVoltage = m_choiceBaseVoltage->GetSelection();

    if(!m_transformer->DoubleFromString(m_parent, m_textCtrlNominalPower->GetValue(), data.nominalPower,
                                        _("Value entered incorrectly in the field \"Nominal power\".")))
        return false;
    switch(m_choiceNominalPower->GetSelection()) {
        case 0: {
            data.nominalPowerUnit = UNIT_VA;
        } break;
        case 1: {
            data.nominalPowerUnit = UNIT_kVA;
        } break;
        case 2: {
            data.nominalPowerUnit = UNIT_MVA;
        } break;
    }

    if(!m_transformer->DoubleFromString(m_parent, m_textCtrlResistance->GetValue(), data.resistance,
                                        _("Value entered incorrectly in the field \"Resistance\".")))
        return false;
    switch(m_choiceResistance->GetSelection()) {
        case 0: {
            data.resistanceUnit = UNIT_PU;
        } break;
        case 1: {
            data.resistanceUnit = UNIT_OHM;
        } break;
    }

    if(!m_transformer->DoubleFromString(m_parent, m_textCtrlReactance->GetValue(), data.indReactance,
                                        _("Value entered incorrectly in the field \"Indutive reactance\".")))
        return false;
    switch(m_choiceReactance->GetSelection()) {
        case 0: {
            data.indReactanceUnit = UNIT_PU;
        } break;
        case 1: {
            data.indReactanceUnit = UNIT_OHM;
        } break;
    }

    switch(m_choiceConnection->GetSelection()) {
        case 0: {
            data.connection = GWYE_GWYE;
        } break;
        case 1: {
            data.connection = WYE_GWYE;
        } break;
        case 2: {
            data.connection = GWYE_WYE;
        } break;
        case 3: {
            data.connection = WYE_WYE;
        } break;
        case 4: {
            data.connection = DELTA_GWYE;
        } break;
        case 5: {
            data.connection = DELTA_WYE;
        } break;
        case 6: {
            data.connection = GWYE_DELTA;
        } break;
        case 7: {
            data.connection = WYE_DELTA;
        } break;
        case 8: {
            data.connection = DELTA_DELTA;
        } break;
    }

    if(!m_transformer->DoubleFromString(m_parent, m_textCtrlTurnRatio->GetValue(), data.turnsRatio,
                                        _("Value entered incorrectly in the field \"Turns ratio\".")))
        return false;

    if(!m_transformer->DoubleFromString(m_parent, m_textCtrlPhaseShift->GetValue(), data.phaseShift,
                                        _("Value entered incorrectly in the field \"Phase shift\".")))
        return false;

    data.useTransformerPower = m_checkUseTransformerPower->GetValue();

    if(!m_transformer->DoubleFromString(m_parent, m_textCtrlZeroResistance->GetValue(), data.zeroResistance,
                                        _("Value entered incorrectly in the field \"Zero sequence resistance\".")))
        return false;

    if(!m_transformer->DoubleFromString(
           m_parent, m_textCtrlZeroReactance->GetValue(), data.zeroIndReactance,
           _("Value entered incorrectly in the field \"Zero sequence indutive reactance\".")))
        return false;

    if(!m_transformer->DoubleFromString(m_parent, m_textCtrlPrimResistance->GetValue(), data.primaryGrndResistance,
                                        _("Value entered incorrectly in the field \"Primary ground resistance\".")))
        return false;

    if(!m_transformer->DoubleFromString(m_parent, m_textCtrlPrimReactance->GetValue(), data.primaryGrndReactance,
                                        _("Value entered incorrectly in the field \"Primary ground reactance\".")))
        return false;

    if(!m_transformer->DoubleFromString(m_parent, m_textCtrlSecResistance->GetValue(), data.secondaryGrndResistance,
                                        _("Value entered incorrectly in the field \"Secondary ground resistance\".")))
        return false;

    if(!m_transformer->DoubleFromString(m_parent, m_textCtrlSecReactance->GetValue(), data.secondaryGrndReactance,
                                        _("Value entered incorrectly in the field \"Secondary ground reactance\".")))
        return false;

    m_transformer->SetElectricaData(data);
    return true;
}
