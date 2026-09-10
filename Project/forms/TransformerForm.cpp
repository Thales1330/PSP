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
	switch (data.primaryNominalVoltageUnit) {
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
	switch (data.secondaryNominalVoltageUnit) {
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
	switch (data.nominalPowerUnit) {
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
	switch (data.resistanceUnit) {
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
	switch (data.indReactanceUnit) {
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
	m_checkEnableTapChanger->SetValue(data.hasTapChanger);

	wxArrayString busChoices;
	wxString primLabel = _("Primary (Bus 1)");
	wxString secLabel = _("Secondary (Bus 2)");
	if (m_transformer->GetParentList().size() >= 2) {
		Bus* b1 = dynamic_cast<Bus*>(m_transformer->GetParentList()[0]);
		Bus* b2 = dynamic_cast<Bus*>(m_transformer->GetParentList()[1]);
		if (b1) primLabel = wxString::Format(_("Primary: Bus %d (%s)"), b1->GetElectricalData().number + 1, b1->GetElectricalData().name);
		if (b2) secLabel = wxString::Format(_("Secondary: Bus %d (%s)"), b2->GetElectricalData().number + 1, b2->GetElectricalData().name);
	}
	busChoices.Add(primLabel);
	busChoices.Add(secLabel);
	m_choiceControlledBus->Append(busChoices);
	m_choiceControlledBus->SetSelection(data.oltcControlledBus == 0 ? 0 : 1);

	m_textCtrlTargetVoltage->SetValue(Transformer::StringFromDouble(data.oltcTargetVoltage));
	m_textCtrlDeadband->SetValue(Transformer::StringFromDouble(data.oltcVoltageDeadband));
	m_textCtrlMinTap->SetValue(Transformer::StringFromDouble(data.oltcMinTap));
	m_textCtrlMaxTap->SetValue(Transformer::StringFromDouble(data.oltcMaxTap));
	m_textCtrlTapStep->SetValue(Transformer::StringFromDouble(data.oltcTapStep));
	m_checkDiscreteTap->SetValue(data.oltcIsDiscrete);

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

	m_textCtrlTurnRatio->Enable(!enable);

	bool discrete = enable && m_checkDiscreteTap->GetValue();
	//m_staticTextTapStep->Enable(discrete);
	m_textCtrlTapStep->Enable(discrete);
}

TransformerForm::~TransformerForm() {}
void TransformerForm::OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
void TransformerForm::OnOKButtonClick(wxCommandEvent& event)
{
	if (ValidateData()) EndModal(wxID_OK);
}
void TransformerForm::OnStabilityButtonClick(wxCommandEvent& event)
{
	if (ValidateData()) {
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

	if (!m_transformer->DoubleFromString(m_parent, m_textCtrlNominalPower->GetValue(), data.nominalPower,
		_("Value entered incorrectly in the field \"Nominal power\".")))
		return false;
	switch (m_choiceNominalPower->GetSelection()) {
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

	if (!m_transformer->DoubleFromString(m_parent, m_textCtrlResistance->GetValue(), data.resistance,
		_("Value entered incorrectly in the field \"Resistance\".")))
		return false;
	switch (m_choiceResistance->GetSelection()) {
	case 0: {
		data.resistanceUnit = ElectricalUnit::UNIT_PU;
	} break;
	case 1: {
		data.resistanceUnit = ElectricalUnit::UNIT_OHM;
	} break;
	}

	if (!m_transformer->DoubleFromString(m_parent, m_textCtrlReactance->GetValue(), data.indReactance,
		_("Value entered incorrectly in the field \"Indutive reactance\".")))
		return false;
	switch (m_choiceReactance->GetSelection()) {
	case 0: {
		data.indReactanceUnit = ElectricalUnit::UNIT_PU;
	} break;
	case 1: {
		data.indReactanceUnit = ElectricalUnit::UNIT_OHM;
	} break;
	}

	switch (m_choiceConnection->GetSelection()) {
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

	if (!m_transformer->DoubleFromString(m_parent, m_textCtrlTurnRatio->GetValue(), data.turnsRatio,
		_("Value entered incorrectly in the field \"Turns ratio\".")))
		return false;

	if (!m_transformer->DoubleFromString(m_parent, m_textCtrlPhaseShift->GetValue(), data.phaseShift,
		_("Value entered incorrectly in the field \"Phase shift\".")))
		return false;

	data.useTransformerPower = m_checkUseTransformerPower->GetValue();

	if (!m_transformer->DoubleFromString(m_parent, m_textCtrlZeroResistance->GetValue(), data.zeroResistance,
		_("Value entered incorrectly in the field \"Zero sequence resistance\".")))
		return false;

	if (!m_transformer->DoubleFromString(
		m_parent, m_textCtrlZeroReactance->GetValue(), data.zeroIndReactance,
		_("Value entered incorrectly in the field \"Zero sequence indutive reactance\".")))
		return false;

	if (!m_transformer->DoubleFromString(m_parent, m_textCtrlPrimResistance->GetValue(), data.primaryGrndResistance,
		_("Value entered incorrectly in the field \"Primary ground resistance\".")))
		return false;

	if (!m_transformer->DoubleFromString(m_parent, m_textCtrlPrimReactance->GetValue(), data.primaryGrndReactance,
		_("Value entered incorrectly in the field \"Primary ground reactance\".")))
		return false;

	if (!m_transformer->DoubleFromString(m_parent, m_textCtrlSecResistance->GetValue(), data.secondaryGrndResistance,
		_("Value entered incorrectly in the field \"Secondary ground resistance\".")))
		return false;

	if (!m_transformer->DoubleFromString(m_parent, m_textCtrlSecReactance->GetValue(), data.secondaryGrndReactance,
		_("Value entered incorrectly in the field \"Secondary ground reactance\".")))
		return false;

	data.hasTapChanger = m_checkEnableTapChanger->GetValue();
	if (data.hasTapChanger) {
		data.oltcControlledBus = (m_choiceControlledBus->GetSelection() == 0) ? 0 : 1;

		if (!m_transformer->DoubleFromString(m_parent, m_textCtrlTargetVoltage->GetValue(), data.oltcTargetVoltage,
			_("Value entered incorrectly in the field \"Target voltage\".")))
			return false;
		if (data.oltcTargetVoltage <= 0.0) {
			wxMessageBox(_("Target voltage must be greater than 0."), _("Error"), wxOK | wxICON_ERROR, m_parent);
			return false;
		}

		if (!m_transformer->DoubleFromString(m_parent, m_textCtrlDeadband->GetValue(), data.oltcVoltageDeadband,
			_("Value entered incorrectly in the field \"Voltage deadband\".")))
			return false;
		if (data.oltcVoltageDeadband < 0.0) {
			wxMessageBox(_("Voltage deadband cannot be negative."), _("Error"), wxOK | wxICON_ERROR, m_parent);
			return false;
		}

		if (!m_transformer->DoubleFromString(m_parent, m_textCtrlMinTap->GetValue(), data.oltcMinTap,
			_("Value entered incorrectly in the field \"Minimum tap\".")))
			return false;
		if (data.oltcMinTap <= 0.0) {
			wxMessageBox(_("Minimum tap must be greater than 0."), _("Error"), wxOK | wxICON_ERROR, m_parent);
			return false;
		}

		if (!m_transformer->DoubleFromString(m_parent, m_textCtrlMaxTap->GetValue(), data.oltcMaxTap,
			_("Value entered incorrectly in the field \"Maximum tap\".")))
			return false;
		if (data.oltcMaxTap < data.oltcMinTap) {
			wxMessageBox(_("Maximum tap must be greater than or equal to minimum tap."), _("Error"), wxOK | wxICON_ERROR, m_parent);
			return false;
		}

		data.oltcIsDiscrete = m_checkDiscreteTap->GetValue();

		if (!m_transformer->DoubleFromString(m_parent, m_textCtrlTapStep->GetValue(), data.oltcTapStep,
			_("Value entered incorrectly in the field \"Tap step size\".")))
			return false;
		if (data.oltcTapStep < 0.0) {
			wxMessageBox(_("Tap step size cannot be negative."), _("Error"), wxOK | wxICON_ERROR, m_parent);
			return false;
		}
	}
	data.nominalTurnsRatio = data.turnsRatio;

	m_transformer->SetElectricaData(data);
	return true;
}
