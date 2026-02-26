#include "LabelManager.h"

#include <wx/msgdlg.h>
#include <array>

#include <../editors/Workspace.h>

LabelManager::LabelManager(wxWindow* parent, Workspace* workspace)
	: LabelManagerBase(parent), m_workspace(workspace)
{
	std::vector<wxString> branchOptions = { _(L"Name"),
		_(L"Active power (i → j)"), _(L"Active power (j → i)"),
		_(L"Rective power (i → j)"), _(L"Rective power (j → i)"),
		_(L"Losses"),
		_(L"Current (i → j)"), _(L"Current (j → i)"),
		_(L"Fault current (i → j)"), _(L"Fault current (j → i)") };

	m_checkListBoxLine->Append(branchOptions);
	m_checkListBoxTransformer->Append(branchOptions);

	m_textCtrlPrecision->SetValue(wxString::Format("%d", m_precision));

	Layout();
	SetSize(GetBestSize());
}

LabelManager::~LabelManager()
{
}

void LabelManager::OnCheckListSelected(wxCommandEvent& event)
{
	//wxMessageBox(wxString::Format("%d", event.GetId()));
	UpdatePreview(m_choicebook->GetSelection());

	event.Skip();
}

void LabelManager::UpdatePreview(int selection)
{
	std::vector<wxCheckListBox*> checkListBoxes = { m_checkListBoxBus, m_checkListBoxGenerator,
		m_checkListBoxLoad, m_checkListBoxLine, m_checkListBoxTransformer, m_checkListBoxCapacitor,
		m_checkListBoxInductor, m_checkListBoxIndMotor, m_checkListBoxSyncMotor };

	m_richTextCtrlPreview->Clear();
	for (auto* checkListBox : checkListBoxes) {
		SetPreviewText(checkListBox);
	}
}

wxString LabelManager::GetBusPreviewText(int dataID)
{
	switch (dataID)
	{
	case 0: { // Name
		return _("Bus 1\n");
	} break;
	case 1: { // Voltage
		switch (m_radioBoxVoltage->GetSelection())
		{
		case 0: {
			return wxString::Format("%.*f p.u.\n", m_precision, 1.0);
		} break;
		case 1: {
			return wxString::Format("%.*f V\n", m_precision, 13800.0);
		} break;
		case 2: {
			return wxString::Format("%.*f kV\n", m_precision, 13.8);
		} break;
		default:
			break;
		}
	} break;
	case 2: { // Angle
		switch (m_radioBoxAngle->GetSelection())
		{
		case 0: {
			return wxString::Format("%.*f°\n", m_precision, 30.0);
		} break;
		case 1: {
			return wxString::Format("%.*f rad\n", m_precision, 0.5235987756);
		} break;
		default:
			break;
		}
	} break;
	case 3: { // Fault current
		return GetCurrent3fText({ 25.54, 0.0, 0.0 });
	} break;
	case 4: { // Fault voltage
		switch (m_radioBoxVoltage->GetSelection())
		{
		case 0: {
			return wxString::Format("Va = %.*f p.u.\t Vb = %.*f p.u.\t Vc = %.*f p.u.\n", m_precision, 0.0, m_precision, 1.0404, m_precision, 1.0246);
		} break;
		case 1: {
			return wxString::Format("Va = %.*f V\t Vb = %.*f V\t Vc = %.*f V\n", m_precision, 0.0, m_precision, 14356.91, m_precision, 14140.11);
		} break;
		case 2: {
			return wxString::Format("Va = %.*f kV\t Vb = %.*f kV\t Vc = %.*f kV\n", m_precision, 0.0, m_precision, 14.35691, m_precision, 14.14011);
		} break;
		default:
			break;
		}
	} break;
	case 5: { // Short-circuit power
		return GetPowerText(_("VA"), 5.0);
	} break;
	case 6: { // Voltage THD
		return wxString::Format(_("THD = %.*f%%\n"), m_precision, 5.36);
	} break;
	default:
		break;
	}
	return wxString();
}

wxString LabelManager::GetGeneratorPreviewText(int dataID)
{
	switch (dataID)
	{
	case 0: { // Name
		return _("Generator 1\n");
	} break;
	case 1: { // Active power
		return GetPowerText(_("W"), 1.0);
	} break;
	case 2: { // Reactive power
		return GetPowerText(_("var"), 0.3);
	} break;
	case 3: { // Fault current
		return GetCurrent3fText({ 22.54, 0.0, 0.0 });
	} break;
	default:
		break;
	}
	return wxString();
}

wxString LabelManager::GetPowerText(wxString unit, double puValue)
{
	switch (m_radioBoxPower->GetSelection()) {
	case 0: {
		return wxString::Format("%.*f p.u.\n", m_precision, puValue);
	} break;
	case 1: {
		return wxString::Format("%.*f %s\n", m_precision, puValue * 1.0e8, unit);
	} break;
	case 2: {
		return wxString::Format("%.*f k%s\n", m_precision, puValue * 1.0e5, unit);
	} break;
	case 3: {
		return wxString::Format("%.*f M%s\n", m_precision, puValue * 1.0e2, unit);
	} break;
	default:
		break;
	}
	return wxString();
}

wxString LabelManager::GetCurrent3fText(const std::array<double, 3>& puCurrents)
{
	switch (m_radioBoxCurrent->GetSelection())
	{
	case 0: {
		return wxString::Format("Ia = %.*f p.u.\t Ib = %.*f p.u.\t Ic = %.*f p.u.\n", m_precision, puCurrents[0], m_precision, puCurrents[1], m_precision, puCurrents[2]);
	} break;
	case 1: {
		return wxString::Format("Ia = %.*f A\t Ib = %.*f A\t Ic = %.*f A\n", m_precision, puCurrents[0] * 1e3, m_precision, puCurrents[1] * 1e3, m_precision, puCurrents[2] * 1e3);
	} break;
	case 2: {
		return wxString::Format("Ia = %.*f kA\t Ib = %.*f kA\t Ic = %.*f kA\n", m_precision, puCurrents[0], m_precision, puCurrents[1], m_precision, puCurrents[2]);
	} break;
	default:
		break;
	}
	return wxString();
}

wxString LabelManager::GetCurrentOrVoltageText(wxString unit, double puValue)
{
	if (unit == _("V"))
	{
		switch (m_radioBoxVoltage->GetSelection())
		{
		case 0: {
			return wxString::Format("%.*f p.u.\n", m_precision, puValue);
		} break;
		case 1: {
			return wxString::Format("%.*f V\n", m_precision, puValue * 13800.0);
		} break;
		case 2: {
			return wxString::Format("%.*f kV\n", m_precision, puValue * 13.8);
		} break;
		default:
			break;
		}
	}
	else if (unit == _("A"))
	{
		switch (m_radioBoxCurrent->GetSelection())
		{
		case 0: {
			return wxString::Format("%.*f p.u.\n", m_precision, puValue);
		} break;
		case 1: {
			return wxString::Format("%.*f A\n", m_precision, puValue * 1e3);
		} break;
		case 2: {
			return wxString::Format("%.*f kA\n", m_precision, puValue);
		} break;
		default:
			break;
		}
	}

	return wxString();
}

void LabelManager::SetPreviewText(wxCheckListBox* checkListBox)
{
	int id = checkListBox->GetId();
	wxString name = "";
	wxString text = "";
	wxArrayInt checked;
	checkListBox->GetCheckedItems(checked);
	switch (id)
	{
	case ID_CL_BUS: {
		name = _("Bus label");
		for (int i : checked) {
			text += GetBusPreviewText(i);
		}
	} break;
	case ID_CL_GENERATOR: {
		name = _("Generator label");
		for (int i : checked) {
			text += GetGeneratorPreviewText(i);
		}
	} break;
	case ID_CL_LOAD: {
		name = _("Load label");
		for (int i : checked) {
			text += GetLoadPreviewText(i);
		}
	} break;
	case ID_CL_LINE: {
		name = _("Line label");
		for (int i : checked) {
			text += GetLinePreviewText(i);
		}
	} break;
	case ID_CL_TRANSFORMER: {
		name = _("Transformer label");
		for (int i : checked) {
			text += GetTransformerPreviewText(i);
		}
	} break;
	case ID_CL_CAPACITOR: {
		name = _("Capacitor label");
		for (int i : checked) {
			text += GetCapacitorPreviewText(i);
		}
	} break;
	case ID_CL_INDUCTOR: {
		name = _("Inductor label");
		for (int i : checked) {
			text += GetInductorPreviewText(i);
		}
	} break;
	case ID_CL_INDMOTOR: {
		name = _("Induction motor label");
		for (int i : checked) {
			text += GetIndMotorPreviewText(i);
		}
	} break;
	case ID_CL_SYNCMOTOR: {
		name = _("Synchronous motor label");
		for (int i : checked) {
			text += GetSyncMotorPreviewText(i);
		}
	} break;
	default:
		break;
	}

	if (!checked.IsEmpty()) {
		m_richTextCtrlPreview->BeginUnderline();
		m_richTextCtrlPreview->BeginBold();
		m_richTextCtrlPreview->WriteText(name);
		m_richTextCtrlPreview->EndBold();
		m_richTextCtrlPreview->EndUnderline();
		m_richTextCtrlPreview->Newline();
		m_richTextCtrlPreview->BeginLeftIndent(60);
		m_richTextCtrlPreview->WriteText(text);
		m_richTextCtrlPreview->EndLeftIndent();
		m_richTextCtrlPreview->Newline();
	}
}

wxString LabelManager::GetLoadPreviewText(int dataID)
{
	switch (dataID)
	{
	case 0: { // Name
		return _("Load 1\n");
	} break;
	case 1: { // Active power
		return GetPowerText(_("W"), 1.0);
	} break;
	case 2: { // Reactive power
		return GetPowerText(_("var"), 0.3);
	} break;
		  return wxString();
	default:
		break;
	}
	return wxString();
}

wxString LabelManager::GetLinePreviewText(int dataID)
{
	switch (dataID)
	{
	case 0: { // Name
		return _("Line 1\n");
	} break;
	case 1: { // Active power (i → j)
		return GetPowerText(_("W"), 1.0);
	} break;
	case 2: { // Active power (j → i)
		return GetPowerText(_("W"), -1.0);
	} break;
	case 3: { // Reactive power (i → j)
		return GetPowerText(_("var"), 0.3);
	} break;
	case 4: { // Reactive power (j → i)
		return GetPowerText(_("var"), -0.3);
	} break;
	case 5: { // Losses
		return GetPowerText(_("W"), 0.02);
	} break;
	case 6: { // Current (i → j)
		return GetCurrentOrVoltageText(_("A"), 0.5);
	} break;
	case 7: { // Current (j → i)
		return GetCurrentOrVoltageText(_("A"), -0.5);
	} break;
	case 8: { // Fault current (i → j)
		return GetCurrent3fText({ 25.56, 0.0, 0.0 });
	} break;
	case 9: { // Fault current (j → i)
		return GetCurrent3fText({ -25.56, 0.0, 0.0 });
	} break;
	default:
		break;
	}
	return wxString();
}

wxString LabelManager::GetTransformerPreviewText(int dataID)
{
	switch (dataID)
	{
	case 0: { // Name
		return _("Transformer 1\n");
	} break;
	case 1: { // Active power (i → j)
		return GetPowerText(_("W"), 1.0);
	} break;
	case 2: { // Active power (j → i)
		return GetPowerText(_("W"), -1.0);
	} break;
	case 3: { // Reactive power (i → j)
		return GetPowerText(_("var"), 0.3);
	} break;
	case 4: { // Reactive power (j → i)
		return GetPowerText(_("var"), -0.3);
	} break;
	case 5: { // Losses
		return GetPowerText(_("W"), 0.02);
	} break;
	case 6: { // Current (i → j)
		return GetCurrentOrVoltageText(_("A"), 0.5);
	} break;
	case 7: { // Current (j → i)
		return GetCurrentOrVoltageText(_("A"), -0.5);
	} break;
	case 8: { // Fault current (i → j)
		return GetCurrent3fText({ 25.56, 0.0, 0.0 });
	} break;
	case 9: { // Fault current (j → i)
		return GetCurrent3fText({ -25.56, 0.0, 0.0 });
	} break;
	default:
		break;
	}
	return wxString();
}

wxString LabelManager::GetCapacitorPreviewText(int dataID)
{
	switch (dataID)
	{
	case 0: { // Name
		return _("Capacitor 1\n");
	} break;
	case 1: { // Reactive power
		return GetPowerText(_("var"), 1.0);
	} break;
		  return wxString();
	default:
		break;
	}
	return wxString();
}

wxString LabelManager::GetInductorPreviewText(int dataID)
{
	switch (dataID)
	{
	case 0: { // Name
		return _("Inductor 1\n");
	} break;
	case 1: { // Reactive power
		return GetPowerText(_("var"), 1.0);
	} break;
		  return wxString();
	default:
		break;
	}
	return wxString();
}

wxString LabelManager::GetIndMotorPreviewText(int dataID)
{
	switch (dataID)
	{
	case 0: { // Name
		return _("Induction Motor 1\n");
	} break;
	case 1: { // Active power
		return GetPowerText(_("W"), 1.0);
	} break;
	case 2: { // Reactive power
		return GetPowerText(_("var"), 0.3);
	} break;
		  return wxString();
	default:
		break;
	}
	return wxString();
}

wxString LabelManager::GetSyncMotorPreviewText(int dataID)
{
	switch (dataID)
	{
	case 0: { // Name
		return _("Synchronous Compensator 1\n");
	} break;
	case 1: { // Active power
		return GetPowerText(_("W"), 0.01);
	} break;
	case 2: { // Reactive power
		return GetPowerText(_("var"), 1.0);
	} break;
		  return wxString();
	default:
		break;
	}
	return wxString();
}

void LabelManager::OnChoicebookChanged(wxChoicebookEvent& event)
{
	UpdatePreview(m_choicebook->GetSelection());
	event.Skip();
}

void LabelManager::OnUnitChanged(wxCommandEvent& event)
{
	UpdatePreview(m_choicebook->GetSelection());
	event.Skip();
}

void LabelManager::OnPrecisionTextUpdate(wxCommandEvent& event)
{
	long precision;
	if (m_textCtrlPrecision->GetValue().ToLong(&precision) && precision >= 0) {
		m_precision = static_cast<size_t>(precision);
		UpdatePreview(m_choicebook->GetSelection());
	}
	event.Skip();
}

void LabelManager::OnApplyButtonCliick(wxCommandEvent& event)
{
	ElectricalUnit voltageUnitList[3] = { ElectricalUnit::UNIT_PU, ElectricalUnit::UNIT_V, ElectricalUnit::UNIT_kV };
	ElectricalUnit currentUnitList[3] = { ElectricalUnit::UNIT_PU, ElectricalUnit::UNIT_A, ElectricalUnit::UNIT_kA };
	ElectricalUnit sUnitList[4] = { ElectricalUnit::UNIT_PU, ElectricalUnit::UNIT_VA, ElectricalUnit::UNIT_kVA, ElectricalUnit::UNIT_MVA };
	ElectricalUnit pUnitList[4] = { ElectricalUnit::UNIT_PU, ElectricalUnit::UNIT_W, ElectricalUnit::UNIT_kW, ElectricalUnit::UNIT_MW };
	ElectricalUnit qUnitList[4] = { ElectricalUnit::UNIT_PU, ElectricalUnit::UNIT_var, ElectricalUnit::UNIT_kvar, ElectricalUnit::UNIT_Mvar };
	ElectricalUnit angleUnitList[2] = { ElectricalUnit::UNIT_DEGREE, ElectricalUnit::UNIT_RADIAN };

	ElectricalUnit voltageUnit = voltageUnitList[m_radioBoxVoltage->GetSelection()];
	ElectricalUnit currentUnit = currentUnitList[m_radioBoxCurrent->GetSelection()];
	ElectricalUnit sUnit = sUnitList[m_radioBoxPower->GetSelection()];
	ElectricalUnit pUnit = pUnitList[m_radioBoxPower->GetSelection()];
	ElectricalUnit qUnit = qUnitList[m_radioBoxPower->GetSelection()];
	ElectricalUnit angleUnit = angleUnitList[m_radioBoxAngle->GetSelection()];

	auto elementList = m_workspace->GetElementList();
	int numTxtElements = 0;
	for (auto* element : elementList) {
		ElementType elementType = element->GetElementType();
		wxArrayInt checked;
		switch (elementType)
		{
		case TYPE_BUS:
		{
			TextID busTextOptions[7] = { ID_TXT_NAME, ID_TXT_VOLTAGE, ID_TXT_ANGLE, ID_TXT_FAULTCURRENT, ID_TXT_FAULTVOLTAGE, ID_TXT_SCC, ID_TXT_THD };
			ElectricalUnit busTextUnits[7] = { ElectricalUnit::UNIT_NONE, voltageUnit, angleUnit, currentUnit, voltageUnit, sUnit, ElectricalUnit::UNIT_NONE };
			m_checkListBoxBus->GetCheckedItems(checked);
			for (int i : checked) {
				m_workspace->InsertTextElement(busTextOptions[i], element, busTextUnits[i], m_precision);
				numTxtElements++;
			}
		} break;
		case TYPE_CAPACITOR:
			break;
		case TYPE_IND_MOTOR:
			break;
		case TYPE_INDUCTOR:
			break;
		case TYPE_LINE:
			break;
		case TYPE_LOAD:
			break;
		case TYPE_SYNC_GENERATOR:
			break;
		case TYPE_SYNC_MOTOR:
			break;
		case TYPE_TRANSFORMER:
			break;
		case TYPE_HARMCURRENT:
			break;
		case TYPE_TEXT:
			break;
		default:
			break;
		}
	}
	if (numTxtElements != 0) {
		m_workspace->SaveCurrentState();
		m_workspace->UpdateTextElements();
	}
	EndModal(wxID_OK);
}

void LabelManager::OnCancelButtonClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}
