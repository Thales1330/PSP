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
#include "../elements/powerElement/Transformer.h"
#include "../elements/powerElement/Bus.h"
#include <wx/msgdlg.h>
#include <wx/statbox.h>

static wxString OltcTr(const wxString& en, const wxString& pt)
{
    wxString s = wxGetTranslation(en);
    if (s != en) return s;
    if (wxGetLocale() && wxGetLocale()->GetLanguage() == wxLANGUAGE_PORTUGUESE_BRAZILIAN) {
        return pt;
    }
    return en;
}

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
        case ElectricalUnit::UNIT_V: {
            primVoltStr += " V";
        } break;
        case ElectricalUnit::UNIT_kV: {
            primVoltStr += " kV";
        } break;
        default:
            break;
    }
    wxString secVoltStr = Transformer::StringFromDouble(data.secondaryNominalVoltage);
    switch(data.secondaryNominalVoltageUnit) {
        case ElectricalUnit::UNIT_V: {
            secVoltStr += " V";
        } break;
        case ElectricalUnit::UNIT_kV: {
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
        case ElectricalUnit::UNIT_VA: {
            m_choiceNominalPower->SetSelection(0);
        } break;
        case ElectricalUnit::UNIT_kVA: {
            m_choiceNominalPower->SetSelection(1);
        } break;
        case ElectricalUnit::UNIT_MVA: {
            m_choiceNominalPower->SetSelection(2);
        } break;
        default:
            break;
    }

    m_textCtrlResistance->SetValue(Transformer::StringFromDouble(data.resistance));
    switch(data.resistanceUnit) {
        case ElectricalUnit::UNIT_PU: {
            m_choiceResistance->SetSelection(0);
        } break;
        case ElectricalUnit::UNIT_OHM: {
            m_choiceResistance->SetSelection(1);
        } break;
        default:
            break;
    }

    m_textCtrlReactance->SetValue(Transformer::StringFromDouble(data.indReactance));
    switch(data.indReactanceUnit) {
        case ElectricalUnit::UNIT_PU: {
            m_choiceReactance->SetSelection(0);
        } break;
        case ElectricalUnit::UNIT_OHM: {
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

    // Tap Changer (OLTC) Tab
    m_panelTapChanger = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    m_notebook->AddPage(m_panelTapChanger, OltcTr(wxT("Tap Changer (OLTC)"), wxString(L"Comutador de Tap (OLTC)")), false);

    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);

    m_checkEnableTapChanger = new wxCheckBox(m_panelTapChanger, wxID_ANY, OltcTr(wxT("Enable On-Load Tap Changer (OLTC)"), wxString(L"Habilitar comutador de tap sob carga (OLTC)")));
    m_checkEnableTapChanger->SetValue(data.hasTapChanger);
    topSizer->Add(m_checkEnableTapChanger, 0, wxALL, 10);

    wxStaticBoxSizer* paramsBoxSizer = new wxStaticBoxSizer(wxVERTICAL, m_panelTapChanger, OltcTr(wxT("OLTC Parameters"), wxString(L"Par\u00e2metros do OLTC")));

    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(6, 3, 8, 10);
    gridSizer->AddGrowableCol(1, 1);

    // Controlled Bus
    m_staticTextControlledBus = new wxStaticText(paramsBoxSizer->GetStaticBox(), wxID_ANY, OltcTr(wxT("Controlled bus:"), wxString(L"Barra controlada:")));
    wxArrayString busChoices;
    wxString primLabel = OltcTr(wxT("Primary (Bus 1)"), wxString(L"Prim\u00e1rio (Barra 1)"));
    wxString secLabel = OltcTr(wxT("Secondary (Bus 2)"), wxString(L"Secund\u00e1rio (Barra 2)"));
    if (m_transformer->GetParentList().size() >= 2) {
        Bus* b1 = static_cast<Bus*>(m_transformer->GetParentList()[0]);
        Bus* b2 = static_cast<Bus*>(m_transformer->GetParentList()[1]);
        if (b1) primLabel = wxString::Format(OltcTr(wxT("Primary: Bus %d (%s)"), wxString(L"Prim\u00e1rio: Barra %d (%s)")), b1->GetElectricalData().number + 1, b1->GetElectricalData().name);
        if (b2) secLabel = wxString::Format(OltcTr(wxT("Secondary: Bus %d (%s)"), wxString(L"Secund\u00e1rio: Barra %d (%s)")), b2->GetElectricalData().number + 1, b2->GetElectricalData().name);
    }
    busChoices.Add(primLabel);
    busChoices.Add(secLabel);
    m_choiceControlledBus = new wxChoice(paramsBoxSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, busChoices);
    m_choiceControlledBus->SetSelection(data.oltcControlledBus == 0 ? 0 : 1);
    gridSizer->Add(m_staticTextControlledBus, 0, wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(m_choiceControlledBus, 1, wxEXPAND);
    gridSizer->Add(new wxStaticText(paramsBoxSizer->GetStaticBox(), wxID_ANY, wxEmptyString), 0);

    // Target Voltage
    m_staticTextTargetVoltage = new wxStaticText(paramsBoxSizer->GetStaticBox(), wxID_ANY, OltcTr(wxT("Target voltage (Vset):"), wxString(L"Tens\u00e3o de refer\u00eancia (Vset):")));
    m_textCtrlTargetVoltage = new wxTextCtrl(paramsBoxSizer->GetStaticBox(), wxID_ANY, Transformer::StringFromDouble(data.oltcTargetVoltage));
    gridSizer->Add(m_staticTextTargetVoltage, 0, wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(m_textCtrlTargetVoltage, 1, wxEXPAND);
    gridSizer->Add(new wxStaticText(paramsBoxSizer->GetStaticBox(), wxID_ANY, _("p.u.")), 0, wxALIGN_CENTER_VERTICAL);

    // Voltage Deadband
    m_staticTextDeadband = new wxStaticText(paramsBoxSizer->GetStaticBox(), wxID_ANY, OltcTr(wxT("Voltage deadband:"), wxString(L"Banda morta de tens\u00e3o:")));
    m_textCtrlDeadband = new wxTextCtrl(paramsBoxSizer->GetStaticBox(), wxID_ANY, Transformer::StringFromDouble(data.oltcVoltageDeadband));
    gridSizer->Add(m_staticTextDeadband, 0, wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(m_textCtrlDeadband, 1, wxEXPAND);
    gridSizer->Add(new wxStaticText(paramsBoxSizer->GetStaticBox(), wxID_ANY, _("p.u.")), 0, wxALIGN_CENTER_VERTICAL);

    // Min Tap
    m_staticTextMinTap = new wxStaticText(paramsBoxSizer->GetStaticBox(), wxID_ANY, OltcTr(wxT("Minimum tap (Tap min):"), wxString(L"Tap m\u00ednimo (Tap min):")));
    m_textCtrlMinTap = new wxTextCtrl(paramsBoxSizer->GetStaticBox(), wxID_ANY, Transformer::StringFromDouble(data.oltcMinTap));
    gridSizer->Add(m_staticTextMinTap, 0, wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(m_textCtrlMinTap, 1, wxEXPAND);
    gridSizer->Add(new wxStaticText(paramsBoxSizer->GetStaticBox(), wxID_ANY, _("p.u.")), 0, wxALIGN_CENTER_VERTICAL);

    // Max Tap
    m_staticTextMaxTap = new wxStaticText(paramsBoxSizer->GetStaticBox(), wxID_ANY, OltcTr(wxT("Maximum tap (Tap max):"), wxString(L"Tap m\u00e1ximo (Tap max):")));
    m_textCtrlMaxTap = new wxTextCtrl(paramsBoxSizer->GetStaticBox(), wxID_ANY, Transformer::StringFromDouble(data.oltcMaxTap));
    gridSizer->Add(m_staticTextMaxTap, 0, wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(m_textCtrlMaxTap, 1, wxEXPAND);
    gridSizer->Add(new wxStaticText(paramsBoxSizer->GetStaticBox(), wxID_ANY, _("p.u.")), 0, wxALIGN_CENTER_VERTICAL);

    // Tap Step Size
    m_staticTextTapStep = new wxStaticText(paramsBoxSizer->GetStaticBox(), wxID_ANY, OltcTr(wxT("Tap step size:"), wxString(L"Passo do tap:")));
    m_textCtrlTapStep = new wxTextCtrl(paramsBoxSizer->GetStaticBox(), wxID_ANY, Transformer::StringFromDouble(data.oltcTapStep));
    gridSizer->Add(m_staticTextTapStep, 0, wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(m_textCtrlTapStep, 1, wxEXPAND);
    gridSizer->Add(new wxStaticText(paramsBoxSizer->GetStaticBox(), wxID_ANY, _("p.u.")), 0, wxALIGN_CENTER_VERTICAL);

    paramsBoxSizer->Add(gridSizer, 1, wxALL | wxEXPAND, 8);

    m_checkDiscreteTap = new wxCheckBox(paramsBoxSizer->GetStaticBox(), wxID_ANY, OltcTr(wxT("Use discrete tap steps"), wxString(L"Usar passos discretos de tap")));
    m_checkDiscreteTap->SetValue(data.oltcIsDiscrete);
    paramsBoxSizer->Add(m_checkDiscreteTap, 0, wxALL, 6);

    topSizer->Add(paramsBoxSizer, 1, wxALL | wxEXPAND, 8);
    m_panelTapChanger->SetSizer(topSizer);

    m_checkEnableTapChanger->Bind(wxEVT_CHECKBOX, &TransformerForm::OnEnableTapChangerCheck, this);
    m_checkDiscreteTap->Bind(wxEVT_CHECKBOX, &TransformerForm::OnDiscreteTapCheck, this);

    UpdateTapChangerFields();

    SetSize(GetBestSize());
    Layout();
}

void TransformerForm::OnEnableTapChangerCheck(wxCommandEvent& event)
{
    UpdateTapChangerFields();
}

void TransformerForm::OnDiscreteTapCheck(wxCommandEvent& event)
{
    UpdateTapChangerFields();
}

void TransformerForm::UpdateTapChangerFields()
{
    bool enable = m_checkEnableTapChanger->GetValue();
    m_staticTextControlledBus->Enable(enable);
    m_choiceControlledBus->Enable(enable);
    m_staticTextTargetVoltage->Enable(enable);
    m_textCtrlTargetVoltage->Enable(enable);
    m_staticTextDeadband->Enable(enable);
    m_textCtrlDeadband->Enable(enable);
    m_staticTextMinTap->Enable(enable);
    m_textCtrlMinTap->Enable(enable);
    m_staticTextMaxTap->Enable(enable);
    m_textCtrlMaxTap->Enable(enable);
    m_checkDiscreteTap->Enable(enable);

    bool discrete = enable && m_checkDiscreteTap->GetValue();
    m_staticTextTapStep->Enable(discrete);
    m_textCtrlTapStep->Enable(discrete);
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
            data.nominalPowerUnit = ElectricalUnit::UNIT_VA;
        } break;
        case 1: {
            data.nominalPowerUnit = ElectricalUnit::UNIT_kVA;
        } break;
        case 2: {
            data.nominalPowerUnit = ElectricalUnit::UNIT_MVA;
        } break;
    }

    if(!m_transformer->DoubleFromString(m_parent, m_textCtrlResistance->GetValue(), data.resistance,
                                        _("Value entered incorrectly in the field \"Resistance\".")))
        return false;
    switch(m_choiceResistance->GetSelection()) {
        case 0: {
            data.resistanceUnit = ElectricalUnit::UNIT_PU;
        } break;
        case 1: {
            data.resistanceUnit = ElectricalUnit::UNIT_OHM;
        } break;
    }

    if(!m_transformer->DoubleFromString(m_parent, m_textCtrlReactance->GetValue(), data.indReactance,
                                        _("Value entered incorrectly in the field \"Indutive reactance\".")))
        return false;
    switch(m_choiceReactance->GetSelection()) {
        case 0: {
            data.indReactanceUnit = ElectricalUnit::UNIT_PU;
        } break;
        case 1: {
            data.indReactanceUnit = ElectricalUnit::UNIT_OHM;
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

    data.hasTapChanger = m_checkEnableTapChanger->GetValue();
    if (data.hasTapChanger) {
        data.oltcControlledBus = (m_choiceControlledBus->GetSelection() == 0) ? 0 : 1;

        if (!m_transformer->DoubleFromString(m_parent, m_textCtrlTargetVoltage->GetValue(), data.oltcTargetVoltage,
                                            OltcTr(wxT("Value entered incorrectly in the field \"Target voltage\"."),
                                                   wxString(L"Valor inserido incorretamente no campo \"Tens\u00e3o de refer\u00eancia\"."))))
            return false;
        if (data.oltcTargetVoltage <= 0.0) {
            wxMessageBox(OltcTr(wxT("Target voltage must be greater than 0."), wxString(L"A tens\u00e3o de refer\u00eancia deve ser maior que 0.")),
                         _("Error"), wxOK | wxICON_ERROR, m_parent);
            return false;
        }

        if (!m_transformer->DoubleFromString(m_parent, m_textCtrlDeadband->GetValue(), data.oltcVoltageDeadband,
                                            OltcTr(wxT("Value entered incorrectly in the field \"Voltage deadband\"."),
                                                   wxString(L"Valor inserido incorretamente no campo \"Banda morta de tens\u00e3o\"."))))
            return false;
        if (data.oltcVoltageDeadband < 0.0) {
            wxMessageBox(OltcTr(wxT("Voltage deadband cannot be negative."), wxString(L"A banda morta de tens\u00e3o n\u00e3o pode ser negativa.")),
                         _("Error"), wxOK | wxICON_ERROR, m_parent);
            return false;
        }

        if (!m_transformer->DoubleFromString(m_parent, m_textCtrlMinTap->GetValue(), data.oltcMinTap,
                                            OltcTr(wxT("Value entered incorrectly in the field \"Minimum tap\"."),
                                                   wxString(L"Valor inserido incorretamente no campo \"Tap m\u00ednimo\"."))))
            return false;
        if (data.oltcMinTap <= 0.0) {
            wxMessageBox(OltcTr(wxT("Minimum tap must be greater than 0."), wxString(L"O tap m\u00ednimo deve ser maior que 0.")),
                         _("Error"), wxOK | wxICON_ERROR, m_parent);
            return false;
        }

        if (!m_transformer->DoubleFromString(m_parent, m_textCtrlMaxTap->GetValue(), data.oltcMaxTap,
                                            OltcTr(wxT("Value entered incorrectly in the field \"Maximum tap\"."),
                                                   wxString(L"Valor inserido incorretamente no campo \"Tap m\u00e1ximo\"."))))
            return false;
        if (data.oltcMaxTap < data.oltcMinTap) {
            wxMessageBox(OltcTr(wxT("Maximum tap must be greater than or equal to minimum tap."), wxString(L"O tap m\u00e1ximo deve ser maior ou igual ao tap m\u00ednimo.")),
                         _("Error"), wxOK | wxICON_ERROR, m_parent);
            return false;
        }

        data.oltcIsDiscrete = m_checkDiscreteTap->GetValue();

        if (!m_transformer->DoubleFromString(m_parent, m_textCtrlTapStep->GetValue(), data.oltcTapStep,
                                            OltcTr(wxT("Value entered incorrectly in the field \"Tap step size\"."),
                                                   wxString(L"Valor inserido incorretamente no campo \"Passo do tap\"."))))
            return false;
        if (data.oltcTapStep < 0.0) {
            wxMessageBox(OltcTr(wxT("Tap step size cannot be negative."), wxString(L"O passo do tap n\u00e3o pode ser negativo.")),
                         _("Error"), wxOK | wxICON_ERROR, m_parent);
            return false;
        }
    }
    data.nominalTurnsRatio = data.turnsRatio;

    m_transformer->SetElectricaData(data);
    return true;
}
