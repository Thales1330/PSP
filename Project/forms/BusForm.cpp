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

#include <cmath>
#include <wx/numformatter.h>

#include "BusForm.h"
#include "../elements/powerElement/Bus.h"
#include "../utils/Path.h"
#include "BusFormArcFlashProtection.h"
#include "../editors/Workspace.h"

namespace
{
	wxString FormatArcFlashNumber(double value)
	{
		return wxNumberFormatter::ToString(
			value, 12,
			wxNumberFormatter::Style_None |
			wxNumberFormatter::Style_NoTrailingZeroes);
	}

	wxString FormatArcFlashInput(double value)
	{
		return value == 0.0 ? wxString() : FormatArcFlashNumber(value);
	}
}

BusForm::BusForm(wxWindow* parent, Bus* bus, wxWindow* workspace) : BusFormBase(parent), m_bus(bus), m_parent(parent), m_workspace(workspace)
{
	m_choiceFaultType->SetString(0, _("Three-phase"));
	m_choiceFaultType->SetString(1, _("Line-to-line"));
	m_choiceFaultType->SetString(2, _("Double line-to-ground"));
	m_choiceFaultType->SetString(3, _("Line-to-ground"));

	// Arcflash
	struct ElectrodeConfiguration
	{
		wxString code;
		wxString image;
	};

	const std::vector<ElectrodeConfiguration> configurations = {
		{.code = _("VCB  - Vertical, Metal Box"), .image = "VCB.png"},
		{.code = _("VCBB - Vertical, Insulating Barrier"), .image = "VCBB.png"},
		{.code = _("HCB  - Horizontal, Metal Box"), .image = "HCB.png"},
		{.code = _("VOA  - Vertical, Open Air"), .image = "VOA.png"},
		{.code = _("HOA  - Horizontal, Open Air"), .image = "HOA.png"}
	};

	const wxSize size = FromDIP(wxSize(32, 32));
	const wxString imgPath = Paths::GetDataPath() + "/images/arcflash/";

	for (const auto& config : configurations)
	{
		wxImage image(imgPath + config.image, wxBITMAP_TYPE_PNG);
		if (!image.IsOk()) {
			wxMessageBox(_("Could not load the electrode configuration image:\n") + imgPath + config.image, _("Arc Flash"), wxOK | wxICON_ERROR, this);
			continue;
		}

		image.Rescale(size.x, size.y, wxIMAGE_QUALITY_HIGH);
		m_bmpComboBoxConfig->Append(config.code, wxBitmap(image));
	}

	m_electrodePreview = new BitmapPopup(this);
	m_electrodePreview->Hide();

	m_textCtrlName->SetValue(bus->GetElectricalData().name);
	m_textCtrlNomVoltage->SetValue(bus->StringFromDouble(bus->GetElectricalData().nominalVoltage));

	if (bus->GetElectricalData().nominalVoltageUnit == ElectricalUnit::UNIT_V)
		m_choiceNomVoltage->SetSelection(0);
	else
		m_choiceNomVoltage->SetSelection(1);

	m_checkBoxCtrlVoltage->SetValue(bus->GetElectricalData().isVoltageControlled);
	m_textCtrlCtrlVoltage->SetValue(bus->StringFromDouble(bus->GetElectricalData().controlledVoltage));
	m_choiceCtrlVoltage->SetSelection(bus->GetElectricalData().controlledVoltageUnitChoice);
	m_checkBoxSlackBus->SetValue(bus->GetElectricalData().slackBus);

	m_checkBoxFault->SetValue(bus->GetElectricalData().hasFault);
	switch (bus->GetElectricalData().faultType) {
	case FaultData::FAULT_THREEPHASE: {
		m_choiceFaultType->SetSelection(0);
	} break;
	case FaultData::FAULT_2LINE: {
		m_choiceFaultType->SetSelection(1);
	} break;
	case FaultData::FAULT_2LINE_GROUND: {
		m_choiceFaultType->SetSelection(2);
	} break;
	case FaultData::FAULT_LINE_GROUND: {
		m_choiceFaultType->SetSelection(3);
	} break;
	default:
		break;
	}
	switch (bus->GetElectricalData().faultLocation) {
	case FaultData::FAULT_LINE_A: {
		m_choiceFaultPlace->SetSelection(0);
	} break;
	case FaultData::FAULT_LINE_B: {
		m_choiceFaultPlace->SetSelection(1);
	} break;
	case FaultData::FAULT_LINE_C: {
		m_choiceFaultPlace->SetSelection(2);
	} break;
	default:
		break;
	}
	m_textCtrlFaultResistance->SetValue(bus->StringFromDouble(bus->GetElectricalData().faultResistance));
	m_textCtrlFaultReactance->SetValue(bus->StringFromDouble(bus->GetElectricalData().faultReactance));

	m_checkBoxPlotData->SetValue(bus->GetElectricalData().plotBus);
	m_checkBoxStabFault->SetValue(bus->GetElectricalData().stabHasFault);
	m_textCtrlStabFaultTime->SetValue(bus->StringFromDouble(bus->GetElectricalData().stabFaultTime));
	m_textCtrlStabFaultLength->SetValue(bus->StringFromDouble(bus->GetElectricalData().stabFaultLength));
	m_textCtrlStabFaultResistance->SetValue(bus->StringFromDouble(bus->GetElectricalData().stabFaultResistance));
	m_textCtrlStabFaultReactance->SetValue(bus->StringFromDouble(bus->GetElectricalData().stabFaultReactance));

	EnableCtrlVoltageFields(bus->GetElectricalData().isVoltageControlled);
	EnableFaultFields(bus->GetElectricalData().hasFault);
	EnableStabFaultFields(bus->GetElectricalData().stabHasFault);

	m_checkBoxPlotPQData->SetValue(bus->GetElectricalData().plotPQData);

	LoadArcFlashData(m_bus->GetElectricalDataRef().arcFlash);

	m_staticBitmapWarning->Hide();

	GetSizer()->Layout();
	GetSizer()->Fit(this);
	SetSize(GetBestSize());
}

BusForm::~BusForm() {}
void BusForm::OnButtonCancelClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
void BusForm::OnButtonOKClick(wxCommandEvent& event)
{
	BusElectricalData data = m_bus->GetElectricalData();
	data.name = m_textCtrlName->GetValue();
	if (!m_bus->DoubleFromString(m_parent, m_textCtrlNomVoltage->GetValue(), data.nominalVoltage,
		_("Value entered incorrectly in the field \"Rated voltage\".")))
		return;
	data.nominalVoltageUnit = m_choiceNomVoltage->GetSelection() == 0 ? ElectricalUnit::UNIT_V : ElectricalUnit::UNIT_kV;
	data.isVoltageControlled = m_checkBoxCtrlVoltage->GetValue();
	if (data.isVoltageControlled) {
		if (!m_bus->DoubleFromString(m_parent, m_textCtrlCtrlVoltage->GetValue(), data.controlledVoltage,
			_("Value entered incorrectly in the field \"Controlled voltage\".")))
			return;
		data.controlledVoltageUnitChoice = m_choiceCtrlVoltage->GetSelection();
	}
	data.slackBus = m_checkBoxSlackBus->GetValue();

	data.hasFault = m_checkBoxFault->GetValue(); 
	switch (m_choiceFaultType->GetSelection()) {
	case 0: {
		data.faultType = FaultData::FAULT_THREEPHASE;
	} break;
	case 1: {
		data.faultType = FaultData::FAULT_2LINE;
	} break;
	case 2: {
		data.faultType = FaultData::FAULT_2LINE_GROUND;
	} break;
	case 3: {
		data.faultType = FaultData::FAULT_LINE_GROUND;
	} break;
	}

	switch (m_choiceFaultPlace->GetSelection()) {
	case 0: {
		data.faultLocation = FaultData::FAULT_LINE_A;
	} break;
	case 1: {
		data.faultLocation = FaultData::FAULT_LINE_B;
	} break;
	case 2: {
		data.faultLocation = FaultData::FAULT_LINE_C;
	} break;
	}

	if (!m_bus->DoubleFromString(m_parent, m_textCtrlFaultResistance->GetValue(), data.faultResistance,
		_("Value entered incorrectly in the field \"Fault resistance\".")))
		return;

	if (!m_bus->DoubleFromString(m_parent, m_textCtrlFaultReactance->GetValue(), data.faultReactance,
		_("Value entered incorrectly in the field \"Fault reactance\".")))
		return;

	data.plotBus = m_checkBoxPlotData->GetValue();
	data.stabHasFault = m_checkBoxStabFault->GetValue();

	if (!m_bus->DoubleFromString(m_parent, m_textCtrlStabFaultTime->GetValue(), data.stabFaultTime,
		_("Value entered incorrectly in the field \"Time\".")))
		return;

	if (!m_bus->DoubleFromString(m_parent, m_textCtrlStabFaultLength->GetValue(), data.stabFaultLength,
		_("Value entered incorrectly in the field \"Fault lenght\".")))
		return;

	if (!m_bus->DoubleFromString(m_parent, m_textCtrlStabFaultResistance->GetValue(), data.stabFaultResistance,
		_("Value entered incorrectly in the field \"Fault resistence (stability)\".")))
		return;

	if (!m_bus->DoubleFromString(m_parent, m_textCtrlStabFaultReactance->GetValue(), data.stabFaultReactance,
		_("Value entered incorrectly in the field \"Fault reactance (stability)\".")))
		return;

	data.plotPQData = m_checkBoxPlotPQData->GetValue();

	m_bus->SetElectricalData(data);

	if (data.stabHasFault)
		m_bus->SetDynamicEvent(true);
	else
		m_bus->SetDynamicEvent(false);

	auto& arcFlash = m_bus->GetElectricalDataRef().arcFlash;

	ArcFlashData afData = arcFlash;

	if (!ValidateAndGetArcFlashData(afData))
		return;

	arcFlash = std::move(afData);

	EndModal(wxID_OK);
}

void BusForm::OnNominalVoltageChoice(wxCommandEvent& event) { UpdateChoiceBoxes(); }
void BusForm::OnFaultTypeChoice(wxCommandEvent& event) { UpdateChoiceBoxes(); }
void BusForm::OnControlledVoltageClick(wxCommandEvent& event)
{
	EnableCtrlVoltageFields(m_checkBoxCtrlVoltage->GetValue());
}
void BusForm::OnInsertFaultClick(wxCommandEvent& event) { EnableFaultFields(m_checkBoxFault->GetValue()); }
void BusForm::OnInsertStabFaultClick(wxCommandEvent& event) { EnableStabFaultFields(m_checkBoxStabFault->GetValue()); }
void BusForm::EnableCtrlVoltageFields(bool enable)
{
	m_textCtrlCtrlVoltage->Enable(enable);
	m_choiceCtrlVoltage->Enable(enable);

	UpdateChoiceBoxes();
}

void BusForm::EnableFaultFields(bool enable)
{
	m_choiceFaultType->Enable(enable);
	m_choiceFaultPlace->Enable(enable);
	m_textCtrlFaultReactance->Enable(enable);
	m_textCtrlFaultResistance->Enable(enable);
	m_staticTextPU_1->Enable(enable);
	m_staticTextPU_2->Enable(enable);

	UpdateChoiceBoxes();
}

void BusForm::EnableStabFaultFields(bool enable)
{
	m_textCtrlStabFaultTime->Enable(enable);
	m_textCtrlStabFaultLength->Enable(enable);
	m_staticTextS_1->Enable(enable);
	m_staticTextS_2->Enable(enable);
	m_textCtrlStabFaultReactance->Enable(enable);
	m_textCtrlStabFaultResistance->Enable(enable);
	m_staticTextPU_3->Enable(enable);
	m_staticTextPU_4->Enable(enable);
}

void BusForm::UpdateChoiceBoxes()
{
	switch (m_choiceFaultType->GetSelection()) {
	case 0:  // three-phase
	{
		m_choiceFaultPlace->Enable(false);
	} break;
	case 1:  // line-to-line
	case 2:  // double line-to-line
	{
		if (m_checkBoxFault->GetValue()) m_choiceFaultPlace->Enable(true);
		m_choiceFaultPlace->SetString(0, _("Lines AB"));
		m_choiceFaultPlace->SetString(1, _("Lines BC"));
		m_choiceFaultPlace->SetString(2, _("Lines CA"));
	} break;
	case 3:  // line-to-ground
	{
		if (m_checkBoxFault->GetValue()) m_choiceFaultPlace->Enable(true);
		m_choiceFaultPlace->SetString(0, _("Line A"));
		m_choiceFaultPlace->SetString(1, _("Line B"));
		m_choiceFaultPlace->SetString(2, _("Line C"));
	} break;
	default:
		break;
	}
	switch (m_choiceNomVoltage->GetSelection()) {
	case 0: {
		m_choiceCtrlVoltage->SetString(1, _("V"));
	} break;
	case 1: {
		m_choiceCtrlVoltage->SetString(1, _("kV"));
	} break;
	default:
		break;
	}
}

bool BusForm::ValidateAndGetArcFlashData(ArcFlashData& data)
{
	ArcFlashData result = data;

	auto showError = [&](const wxString& message) {
		wxMessageBox(message, _("Invalid data"),
			wxOK | wxICON_WARNING, m_parent);
		return false;
		};

	auto readPositiveValue = [&](wxTextCtrl* control, double& value,
		const wxString& field) {
			if (!m_bus->DoubleFromString(
				m_parent, control->GetValue(), value,
				wxString::Format(
					_("Value entered incorrectly in the field \"%s.\""),
					field)))
				return false;

			if (!std::isfinite(value) || value <= 0.0)
				return showError(wxString::Format(
					_("The field \"%s\" must be greater than zero."),
					field));

			return true;
		};

	// Electrode configuration.
	const int configSelection = m_bmpComboBoxConfig->GetSelection();

	if (configSelection < 0 ||
		configSelection >= static_cast<int>(m_bmpComboBoxConfig->GetCount()) ||
		configSelection > static_cast<int>(ArcFlashElectrodeConfig::HOA))
		return showError(_("Select a valid electrode configuration."));

	result.electrodeConfig =
		static_cast<ArcFlashElectrodeConfig>(configSelection);

	// Common input parameters.
	if (!readPositiveValue(m_textCtrlGap, result.conductorGap,
		_("Electrode gap (mm)")))
		return false;

	if (!readPositiveValue(m_textCtrlWorkDist, result.workingDistance,
		_("Working distance (mm)")))
		return false;

	// Enclosure dimensions are required only for enclosed configurations.
	if (IsEnclosedConfig(result.electrodeConfig)) {
		if (!readPositiveValue(m_textCtrlHeight, result.enclosureHeight,
			_("Height (mm)")))
			return false;

		if (!readPositiveValue(m_textCtrlWidth, result.enclosureWidth,
			_("Width (mm)")))
			return false;

		if (!readPositiveValue(m_textCtrlGap525558, result.enclosureDepth,
			_("Depth (mm)")))
			return false;
	}
	else {
		result.enclosureHeight = 0.0;
		result.enclosureWidth = 0.0;
		result.enclosureDepth = 0.0;
	}

	// Invalidate calculated results only if relevant inputs have changed.
	const bool inputsChanged =
		result.electrodeConfig != data.electrodeConfig ||
		result.conductorGap != data.conductorGap ||
		result.workingDistance != data.workingDistance ||
		(IsEnclosedConfig(result.electrodeConfig) &&
			(result.enclosureHeight != data.enclosureHeight ||
				result.enclosureWidth != data.enclosureWidth ||
				result.enclosureDepth != data.enclosureDepth));

	if (inputsChanged) {
		result.calculated = false;
		result.iarc_kA = 0.0;
		result.iarcMin_kA = 0.0;
		result.incidentEnergy_Jcm2 = 0.0;
		result.incidentEnergy_calcm2 = 0.0;
		result.arcFlashBoundary_mm = 0.0;
		result.usedReducedCurrent = false;
		result.warnings.clear();
	}

	// Preserve data.enabled: there is no corresponding control in the
	// interface shown.
	data = std::move(result);

	if (inputsChanged) {
		m_staticTextEnergy->SetLabel(_("E = -- J/cm²"));
		m_staticTextAFB->SetLabel(_("AFB = -- mm"));
		m_staticBitmapWarning->Hide();
		m_panelArcFlash->Layout();
	}

	return true;
}

void BusForm::LoadArcFlashData(const ArcFlashData& data)
{
	// Electrode configuration.
	const int configSelection = static_cast<int>(data.electrodeConfig);

	if (configSelection >= 0 &&
		configSelection < static_cast<int>(m_bmpComboBoxConfig->GetCount()))
		m_bmpComboBoxConfig->SetSelection(configSelection);
	else if (m_bmpComboBoxConfig->GetCount() > 0)
		m_bmpComboBoxConfig->SetSelection(0);

	// Input parameters.
	m_textCtrlGap->ChangeValue(FormatArcFlashInput(data.conductorGap));
	m_textCtrlWorkDist->ChangeValue(FormatArcFlashInput(data.workingDistance));
	m_textCtrlHeight->ChangeValue(FormatArcFlashInput(data.enclosureHeight));
	m_textCtrlWidth->ChangeValue(FormatArcFlashInput(data.enclosureWidth));
	m_textCtrlGap525558->ChangeValue(FormatArcFlashInput(data.enclosureDepth));

	UpdateArcFlashEnclosureFields();

	// Calculated results.
	if (data.calculated) {
		m_staticTextEnergy->SetLabel(wxString::Format(
			_("E = %s J/cm²"),
			FormatArcFlashNumber(data.incidentEnergy_Jcm2)));

		m_staticTextAFB->SetLabel(wxString::Format(
			_("AFB = %s mm"),
			FormatArcFlashNumber(data.arcFlashBoundary_mm)));
	}
	else {
		m_staticTextEnergy->SetLabel(_("E = -- J/cm²"));
		m_staticTextAFB->SetLabel(_("AFB = -- mm"));
	}

	// Display the warning icon and its tooltip when applicable.
	wxString warningText;

	if (data.calculated) {
		for (const auto& warning : data.warnings) {
			if (!warningText.IsEmpty())
				warningText += "\n";

			warningText += warning;
		}

		if (data.usedReducedCurrent) {
			if (!warningText.IsEmpty())
				warningText += "\n";

			warningText += _("Reduced-current calculation was used.");
		}
	}

	m_staticBitmapWarning->SetToolTip(warningText);
	m_staticBitmapWarning->Show(!warningText.IsEmpty());

	m_panelArcFlash->Layout();
	m_panelArcFlash->Refresh();
}

void BusForm::UpdateArcFlashEnclosureFields()
{
	const int selection = m_bmpComboBoxConfig->GetSelection();

	const bool isValidSelection =
		selection >= 0 &&
		selection <= static_cast<int>(ArcFlashElectrodeConfig::HOA);

	const bool isEnclosed = isValidSelection &&
		IsEnclosedConfig(
			static_cast<ArcFlashElectrodeConfig>(selection));

	m_staticTextHeight->Enable(isEnclosed);
	m_textCtrlHeight->Enable(isEnclosed);

	m_staticTextWidth->Enable(isEnclosed);
	m_textCtrlWidth->Enable(isEnclosed);

	m_staticTextDepth->Enable(isEnclosed);
	m_textCtrlGap525558->Enable(isEnclosed);
}

void BusForm::OnMouseEnterElectrodeConfig(wxMouseEvent& event)
{
	wxString configStr = "";
	if (m_bmpComboBoxConfig->GetSelection() == -1)
	{
		event.Skip();
		return;
	}
	else
	{
		switch (m_bmpComboBoxConfig->GetSelection())
		{
		case 0:
			configStr = wxT("VCB");
			break;
		case 1:
			configStr = wxT("VCBB");
			break;
		case 2:
			configStr = wxT("HCB");
			break;
		case 3:
			configStr = wxT("VOA");
			break;
		case 4:
			configStr = wxT("HOA");
			break;
		}
	}
	const wxString imgPath = Paths::GetDataPath() + "/images/arcflash/" + configStr + ".png";
	wxImage image(imgPath, wxBITMAP_TYPE_PNG);
	const wxSize size = FromDIP(wxSize(300, 300));
	image.Rescale(size.x, size.y, wxIMAGE_QUALITY_HIGH);
	m_electrodePreview->SetBitmap(wxBitmap(image));

	m_electrodePreview->Position(wxGetMousePosition() + wxPoint(10, 10), wxSize(0, 0));

	m_electrodePreview->Show();
	m_electrodePreview->Raise();

	event.Skip();
}

void BusForm::OnMouseLeaveElectrodeConfig(wxMouseEvent& event)
{
	m_electrodePreview->Hide();
	event.Skip();
}

void BusForm::OnMouseMotionElectrodeConfig(wxMouseEvent& event)
{
	const wxPoint mouse = wxGetMousePosition();
	const wxSize popupSize = m_electrodePreview->GetSize();

	wxPoint pos = mouse + FromDIP(wxPoint(10, 10));

	const wxRect screen = wxGetClientDisplayRect();

	if (pos.x + popupSize.x > screen.GetRight())
		pos.x = mouse.x - popupSize.x - FromDIP(10);

	if (pos.y + popupSize.y > screen.GetBottom())
		pos.y = mouse.y - popupSize.y - FromDIP(10);

	m_electrodePreview->SetPosition(pos);
}

BitmapPopup::BitmapPopup(wxWindow* parent) : wxPopupWindow(parent, wxBORDER_SIMPLE)
{
	m_bitmap = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap);
}

void BitmapPopup::SetBitmap(const wxBitmap& bitmap)
{
	m_bitmap->SetBitmap(bitmap);
	m_bitmap->SetSize(bitmap.GetSize());
	SetSize(bitmap.GetSize());
}
void BusForm::OnnConfigProtectionButtonClick(wxCommandEvent& event)
{
	double basePower = 100e6; // Default base power is 100 MVA
	if (auto* workspace = dynamic_cast<Workspace*>(m_workspace))
	{
		basePower = m_bus->GetValueFromUnit(
			workspace->GetProperties()->GetSimulationPropertiesData().basePower,
			workspace->GetProperties()->GetSimulationPropertiesData().basePowerUnit);
	}

	BusFormArcFlashProtection protectionConfig(this, m_bus, basePower);
	if (protectionConfig.ShowModal() == wxID_OK)
	{
		// Handle the protection configuration here
	}
}
void BusForm::OnCBArcFlashConfigSelected(wxCommandEvent& event)
{
	UpdateArcFlashEnclosureFields();
}
