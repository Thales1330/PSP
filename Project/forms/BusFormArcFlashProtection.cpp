#include "BusFormArcFlashProtection.h"

#include "../elements/powerElement/Bus.h"
#include "../elements/powerElement/Line.h"
#include "../elements/powerElement/Transformer.h"

BusFormArcFlashProtection::BusFormArcFlashProtection(wxWindow* parent, Bus* bus, double basePower)
	: BusFormArcFlashProtectionBase(parent), m_bus(bus), m_basePower(basePower)
{
	for (auto* element : m_bus->GetChildList()) {
		if (auto* line = dynamic_cast<Line*>(element)) {
			if (line->GetParentList().size() == 2) {
				const auto& data = line->GetElectricalDataRef();
				int busIndex = line->GetParentList()[0] == m_bus ? 0 : 1;

				double baseVoltage = m_bus->GetValueFromUnit(
					m_bus->GetElectricalDataRef().nominalVoltage,
					m_bus->GetElectricalDataRef().nominalVoltageUnit);
				double baseCurrent = m_basePower / (std::sqrt(3.0) * baseVoltage);
				double faultCurrent = std::abs(data.faultCurrent[busIndex][0]) * baseCurrent;
				if (!line->IsOnline()) faultCurrent = 0.0;

				bool check = faultCurrent > 1e-3 && data.faultCurrent[busIndex][0].real() > 1e-6;

				wxString deviceName = data.name;
				wxString faultCurrentString = Bus::StringFromDouble(faultCurrent, 0, 2);

				m_dvListCtrlDevices->AppendItem({
					wxVariant(check),
					wxVariant(deviceName),
					wxVariant(faultCurrentString)
					});
			}
		}
	}

	m_gridTCC->AppendCols(2);

	m_gridTCC->SetColLabelValue(0, _("Time (s)"));
	m_gridTCC->SetColLabelValue(1, _("Current (A)"));

	m_gridTCC->SetColFormatFloat(0, -1, 3);
	m_gridTCC->SetColFormatFloat(1, -1, 3);

	EnableFields();

	GetSizer()->Layout();
	GetSizer()->Fit(this);
	SetSize(GetBestSize());
}

BusFormArcFlashProtection::~BusFormArcFlashProtection()
{
}

void BusFormArcFlashProtection::EnableFields()
{
	switch (m_choiceType->GetSelection())
	{
	case 0:
		m_choiceMethod->Enable();
		m_choiceMethod->SetString(1, _("Appendix I"));
		m_checkBoxIsMeltTime->Enable(false);
		m_textCtrlDelay->Enable();
		break;
	case 1:
		m_choiceMethod->Enable();
		m_choiceMethod->SetString(1, _("Appendix H"));
		m_checkBoxIsMeltTime->Enable();
		m_textCtrlDelay->Enable(false);
		break;
	default: break;
	}
	switch (m_choiceMethod->GetSelection())
	{
	case 0:
		m_gridTCC->Enable();
		m_bmpButtonAdd->Enable();
		m_bmpButtonRemove->Enable();
		m_buttonImport->Enable();
		break;
	case 1:
		m_gridTCC->Enable(false);
		m_bmpButtonAdd->Enable(false);
		m_bmpButtonRemove->Enable(false);
		m_buttonImport->Enable(false);
		break;
	default: break;
	}
}

void BusFormArcFlashProtection::OnAddButtonClick(wxCommandEvent& event)
{
	const int row = m_gridTCC->GetNumberRows();
	m_gridTCC->AppendRows(1);
	m_gridTCC->SetCellValue(row, 0, "0.0");
	m_gridTCC->SetCellValue(row, 1, "0.0");
}

void BusFormArcFlashProtection::OnDeviceChanged(wxDataViewEvent& event)
{
}

void BusFormArcFlashProtection::OnImportButtonClick(wxCommandEvent& event)
{
}
void BusFormArcFlashProtection::OnMethodSelected(wxCommandEvent& event)
{
	EnableFields();
}

void BusFormArcFlashProtection::OnOKButtonClick(wxCommandEvent& event)
{
}

void BusFormArcFlashProtection::OnRemoveButtonClick(wxCommandEvent& event)
{
	wxArrayInt rows = m_gridTCC->GetSelectedRows();

	for (int i = rows.size() - 1; i >= 0; --i)
	{
		m_gridTCC->DeleteRows(rows[i], 1);
	}
}

void BusFormArcFlashProtection::OnTypeSelected(wxCommandEvent& event)
{
	EnableFields();
}

void BusFormArcFlashProtection::OnnCancelButtonClick(wxCommandEvent& event)
{
}
