#include "BusFormArcFlashProtection.h"

#include <wx/clipbrd.h>
#include <wx/dcbuffer.h>
#include <wx/textfile.h>
#include <wx/filedlg.h>
#include <wx/numformatter.h>

#include "../elements/powerElement/Bus.h"
#include "../elements/powerElement/Line.h"
#include "../elements/powerElement/Transformer.h"

namespace
{
	wxString FormatProtectionNumber(double value)
	{
		return wxNumberFormatter::ToString(value, 12, wxNumberFormatter::Style_None | wxNumberFormatter::Style_NoTrailingZeroes);
	}

	wxString FormatOptionalNumber(double value)
	{
		return value == 0.0 ? wxString() : FormatProtectionNumber(value);
	}
}


BusFormArcFlashProtection::BusFormArcFlashProtection(wxWindow* parent, Bus* bus, double basePower)
	: BusFormArcFlashProtectionBase(parent), m_bus(bus), m_basePower(basePower)
{
	const auto& busData = m_bus->GetElectricalDataRef();
	double baseVoltage = m_bus->GetValueFromUnit(
		m_bus->GetElectricalDataRef().nominalVoltage,
		m_bus->GetElectricalDataRef().nominalVoltageUnit);
	double baseCurrent = m_basePower / (std::sqrt(3.0) * baseVoltage);
	double ibf = std::abs(busData.faultCurrent[0]) * baseCurrent;

	if (busData.hasFault) {
		m_staticTextIbf->SetLabel(_("Total Ibf (fault): ") + Bus::StringFromDouble(ibf / 1e3, 0, 4) + wxT(" kA"));
	}

	auto addDevice = [&](auto* device) {
		const auto& parentList = device->GetParentList();
		if (parentList.size() != 2)
			return;

		const auto& data = device->GetElectricalDataRef();
		int busIndex = parentList[0] == m_bus ? 0 : 1;

		double faultCurrent = std::abs(data.faultCurrent[busIndex][0]) * baseCurrent;

		if (!device->IsOnline())
			faultCurrent = 0.0;

		const bool check = data.arcFlashProtection.configured
			? data.arcFlashProtection.consider
			: faultCurrent > 1e-3 && data.faultCurrent[busIndex][0].real() > 1e-6;

		wxString deviceName = data.name;
		wxString faultCurrentString = Bus::StringFromDouble(faultCurrent / 1e3, 0, 2);

		m_dvListCtrlDevices->AppendItem({
			wxVariant(check),
			wxVariant(deviceName),
			wxVariant(faultCurrentString)
			},
			reinterpret_cast<wxUIntPtr>(device));
		};
	for (auto* element : m_bus->GetChildList()) {
		if (auto* line = dynamic_cast<Line*>(element))
			addDevice(line);
		else if (auto* transformer = dynamic_cast<Transformer*>(element))
			addDevice(transformer);
	}

	m_gridTCC->AppendCols(2);


	m_gridTCC->SetColLabelValue(0, _("Current (A)"));
	m_gridTCC->SetColLabelValue(1, _("Time (s)"));

	m_gridTCC->SetColFormatFloat(0, -1, 3);
	m_gridTCC->SetColFormatFloat(1, -1, 3);

	int gridWidth = m_gridTCC->GetRowLabelSize();

	for (int col = 0; col < m_gridTCC->GetNumberCols(); ++col)
		gridWidth += m_gridTCC->GetColSize(col);

	m_gridTCC->SetMinSize(wxSize(gridWidth + 100, -1));
	m_splitterPageRight->SetMinSize(wxSize(gridWidth + 100, -1));

	int devicesWidth = 30;

	for (unsigned int i = 0; i < m_dvListCtrlDevices->GetColumnCount(); ++i) {
#ifdef __WXMSW__
		devicesWidth += static_cast<int>(m_dvListCtrlDevices->GetBestColumnWidth(static_cast<int>(i)));
#else
		devicesWidth += m_dvListCtrlDevices->GetColumn(i)->GetWidth();
#endif
	}

	m_dvListCtrlDevices->SetMinSize(wxSize(devicesWidth, 100));

	m_splitterPageLeft->Layout();
	m_splitterPageRight->Layout();

	GetSizer()->Fit(this);

	m_splitter->SetSashGravity(1.0);

	//const int rightWidth = m_splitterPageRight->GetSizer()->CalcMin().x;
	const int rightWidth = m_splitterPageRight->GetMinWidth();
	const int sashPosition = m_splitter->GetClientSize().x - rightWidth - m_splitter->GetSashSize();

	m_splitter->SetSashPosition(sashPosition);

	m_gridTCC->Bind(wxEVT_KEY_DOWN, &BusFormArcFlashProtection::OnGridKeyDown, this);

	// Chart popup
	wxWindow* corner = m_gridTCC->GetGridCornerLabelWindow();
	corner->Bind(wxEVT_PAINT, &BusFormArcFlashProtection::OnGridCornerPaint, this);
	corner->Bind(wxEVT_ENTER_WINDOW, [this](wxMouseEvent& event) {
		if (!m_tccPopup)
			m_tccPopup = new TCCPopup(this, m_gridTCC);

		wxPoint mousePos = wxGetMousePosition();
		m_tccPopup->Move(mousePos + wxPoint(15, 15));
		m_tccPopup->Popup();

		event.Skip();
		});

	corner->Bind(wxEVT_MOTION, [this](wxMouseEvent& event) {
		if (m_tccPopup && m_tccPopup->IsShown()) {
			const wxPoint mouse = wxGetMousePosition();
			const wxSize popupSize = m_tccPopup->GetSize();
			const int offset = FromDIP(10);

			wxPoint pos = mouse + wxPoint(offset, offset);

			const wxRect screen = wxGetClientDisplayRect();

			if (pos.x + popupSize.x > screen.GetRight())
				pos.x -= pos.x + popupSize.x - screen.GetRight();

			if (pos.y + popupSize.y > screen.GetBottom())
				pos.y -= pos.y + popupSize.y - screen.GetBottom();

			if (pos.y < screen.GetTop())
				pos.y = screen.GetTop();

			m_tccPopup->SetPosition(pos);
		}

		event.Skip();
		});

	corner->Bind(wxEVT_LEAVE_WINDOW, [this](wxMouseEvent& event) {
		if (m_tccPopup)
			m_tccPopup->Dismiss();

		event.Skip();
		});

	InitializeCBChoices();
	UpdateRadioI1Selection();
	EnableFields();
	SetSize(GetBestSize());
}

BusFormArcFlashProtection::~BusFormArcFlashProtection()
{
	wxWindow* corner = m_gridTCC->GetGridCornerLabelWindow();
	corner->Unbind(wxEVT_PAINT, &BusFormArcFlashProtection::OnGridCornerPaint, this);
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
		m_panelTCC->Enable();
		m_panelAI->Enable(false);
		m_panelAH->Enable(false);
		m_notebook->SetSelection(0);
		break;
	case 1:
		//m_gridTCC->Enable(false);
		//m_bmpButtonAdd->Enable(false);
		//m_bmpButtonRemove->Enable(false);
		//m_buttonImport->Enable(false);
		m_panelTCC->Enable(false);
		if (m_choiceType->GetSelection() == 0)
		{
			m_panelAI->Enable();
			m_panelAH->Enable(false);
			m_notebook->SetSelection(1);
		}
		else
		{
			m_panelAI->Enable(false);
			m_panelAH->Enable();
			m_notebook->SetSelection(2);
		}
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
	//auto devicePtr = m_dvListCtrlDevices->GetItemData(event.GetItem());
	//auto* element = reinterpret_cast<Element*>(devicePtr);
	//ArcFlashProtectionData afpData;
	//if (auto* line = dynamic_cast<Line*>(element))
	//	afpData = line->GetElectricalDataRef().arcFlashProtection;
	//else if (auto* transformer = dynamic_cast<Transformer*>(element))
	//	afpData = transformer->GetElectricalDataRef().arcFlashProtection;
	//LoadProtectionData(afpData);
	if (m_restoringDeviceSelection)
		return;

	const wxDataViewItem newItem = event.GetItem();

	if (!newItem.IsOk())
		return;

	if (newItem == m_previousDeviceSelection)
		return;

	auto getElement = [this](const wxDataViewItem& item) -> Element* {
		auto devicePtr = m_dvListCtrlDevices->GetItemData(item);
		return reinterpret_cast<Element*>(devicePtr);
		};

	auto getProtectionData = [](Element* element) -> ArcFlashProtectionData* {
		if (auto* line = dynamic_cast<Line*>(element))
			return &line->GetElectricalDataRef().arcFlashProtection;

		if (auto* transformer = dynamic_cast<Transformer*>(element))
			return &transformer->GetElectricalDataRef().arcFlashProtection;

		return nullptr;
		};

	// Validate and save the previously selected device.
	if (m_previousDeviceSelection.IsOk()) {
		auto* previousElement = getElement(m_previousDeviceSelection);
		auto* previousData = getProtectionData(previousElement);

		if (previousData) {
			ArcFlashProtectionData validatedData = *previousData;

			const int previousRow =
				m_dvListCtrlDevices->ItemToRow(m_previousDeviceSelection);

			if (previousRow != wxNOT_FOUND)
				validatedData.consider =
				m_dvListCtrlDevices->GetToggleValue(previousRow, 0);

			if (!ValidateAndGetProtectionData(validatedData)) {
				m_restoringDeviceSelection = true;
				m_dvListCtrlDevices->Unselect(newItem);
				m_dvListCtrlDevices->Select(m_previousDeviceSelection);
				m_restoringDeviceSelection = false;
				return;
			}

			*previousData = validatedData;
		}
	}

	// Load the newly selected device.
	m_previousDeviceSelection = newItem;

	if (auto* element = getElement(newItem)) {
		if (auto* protectionData = getProtectionData(element))
			LoadProtectionData(*protectionData);
	}
}

void BusFormArcFlashProtection::OnImportButtonClick(wxCommandEvent& event)
{
	wxFileDialog dialog(
		this,
		_("Import TCC curve"),
		"",
		"",
		_("CSV files (*.csv)|*.csv|All files (*.*)|*.*"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (dialog.ShowModal() != wxID_OK)
		return;

	wxTextFile file;

	if (!file.Open(dialog.GetPath()))
	{
		wxMessageBox(_("Unable to open the selected file."), _("Import error"), wxOK | wxICON_ERROR, this);
		return;
	}


	m_gridTCC->ClearGrid();

	if (m_gridTCC->GetNumberRows() > 0)
		m_gridTCC->DeleteRows(0, m_gridTCC->GetNumberRows());

	int row = 0;

	for (size_t i = 0; i < file.GetLineCount(); ++i)
	{
		wxString line = file.GetLine(i).Trim(true).Trim(false);

		if (line.IsEmpty())
			continue;

		wxArrayString columns;

		if (line.Find(';') != wxNOT_FOUND)
		{
			wxStringTokenizer tokenizer(line, ";");

			while (tokenizer.HasMoreTokens())
				columns.Add(tokenizer.GetNextToken());
		}
		else if (line.Find('\t') != wxNOT_FOUND)
		{
			wxStringTokenizer tokenizer(line, "\t");

			while (tokenizer.HasMoreTokens())
				columns.Add(tokenizer.GetNextToken());
		}
		else
		{
			wxStringTokenizer tokenizer(line, ",");

			while (tokenizer.HasMoreTokens())
				columns.Add(tokenizer.GetNextToken());
		}

		if (columns.size() < 2)
			continue;

		wxString currentString = columns[0].Trim(true).Trim(false);
		wxString timeString = columns[1].Trim(true).Trim(false);

		double current = 0.0;
		double time = 0.0;

		if (!ParseDouble(currentString, current) || !ParseDouble(timeString, time))
		{
			if (row == 0)
				continue;

			wxMessageBox(
				wxString::Format(_("Invalid data at line %zu."), i + 1),
				_("Import error"),
				wxOK | wxICON_ERROR,
				this);

			file.Close();
			return;
		}

		if (current <= 0.0 || time <= 0.0)
		{
			wxMessageBox(
				wxString::Format(_("Invalid data at line %zu. Current and time must be greater than zero."), i + 1),
				_("Import error"),
				wxOK | wxICON_ERROR,
				this);

			file.Close();
			return;
		}

		m_gridTCC->AppendRows(1);
		m_gridTCC->SetCellValue(row, 0, wxString::FromDouble(current));
		m_gridTCC->SetCellValue(row, 1, wxString::FromDouble(time));

		++row;
	}

	file.Close();

	m_gridTCC->ForceRefresh();

	if (m_tccPopup && m_tccPopup->IsShown())
		m_tccPopup->UpdateGraph();
}

void BusFormArcFlashProtection::OnMethodSelected(wxCommandEvent& event)
{
	EnableFields();
}

void BusFormArcFlashProtection::OnOKButtonClick(wxCommandEvent& event)
{
	const unsigned int itemCount = m_dvListCtrlDevices->GetItemCount();

	if (itemCount == 0) {
		EndModal(wxID_OK);
		return;
	}

	auto getElement = [this](const wxDataViewItem& item) -> Element* {
		return reinterpret_cast<Element*>(
			m_dvListCtrlDevices->GetItemData(item));
		};

	auto getProtectionData = [](Element* element) -> ArcFlashProtectionData* {
		if (auto* line = dynamic_cast<Line*>(element))
			return &line->GetElectricalDataRef().arcFlashProtection;

		if (auto* transformer = dynamic_cast<Transformer*>(element))
			return &transformer->GetElectricalDataRef().arcFlashProtection;

		return nullptr;
		};

	std::vector<std::pair<ArcFlashProtectionData*,
		ArcFlashProtectionData>> updates;

	updates.reserve(itemCount);

	const wxDataViewItem originalSelection = m_previousDeviceSelection;

	// Validate the currently displayed device first.
	if (originalSelection.IsOk()) {
		auto* element = getElement(originalSelection);
		auto* protectionData = getProtectionData(element);

		if (protectionData) {
			ArcFlashProtectionData data = *protectionData;

			const int row = m_dvListCtrlDevices->ItemToRow(originalSelection);
			if (row != wxNOT_FOUND)
				data.consider = m_dvListCtrlDevices->GetToggleValue(row, 0);

			if (!ValidateAndGetProtectionData(data))
				return;

			updates.emplace_back(protectionData, data);
		}
	}

	// Validate all remaining devices.
	m_restoringDeviceSelection = true;

	for (unsigned int row = 0; row < itemCount; ++row) {
		const wxDataViewItem item = m_dvListCtrlDevices->RowToItem(row);

		if (item == originalSelection)
			continue;

		auto* element = getElement(item);
		auto* protectionData = getProtectionData(element);

		if (!protectionData)
			continue;

		LoadProtectionData(*protectionData);

		ArcFlashProtectionData data = *protectionData;
		data.consider = m_dvListCtrlDevices->GetToggleValue(row, 0);

		if (!ValidateAndGetProtectionData(data)) {
			m_restoringDeviceSelection = false;

			m_dvListCtrlDevices->UnselectAll();
			m_dvListCtrlDevices->Select(item);
			m_previousDeviceSelection = item;

			LoadProtectionData(*protectionData);
			return;
		}

		updates.emplace_back(protectionData, data);
	}

	m_restoringDeviceSelection = false;

	// Commit only after every device has been validated.
	for (auto& [protectionData, data] : updates)
		*protectionData = data;

	EndModal(wxID_OK);
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
	EndModal(wxID_CANCEL);
}

void BusFormArcFlashProtection::OnGridKeyDown(wxKeyEvent& event)
{
	if (!event.ControlDown() || event.GetKeyCode() != 'V') {
		event.Skip();
		return;
	}

	if (!wxTheClipboard->Open()) {
		event.Skip();
		return;
	}

	wxTextDataObject data;

	if (!wxTheClipboard->GetData(data)) {
		wxTheClipboard->Close();
		event.Skip();
		return;
	}

	wxTheClipboard->Close();

	wxString text = data.GetText();
	text.Replace("\r\n", "\n");
	text.Replace("\r", "\n");

	wxArrayString rows;
	wxStringTokenizer rowTokenizer(text, "\n");

	while (rowTokenizer.HasMoreTokens())
		rows.Add(rowTokenizer.GetNextToken());

	int startRow = m_gridTCC->GetGridCursorRow();
	int startCol = m_gridTCC->GetGridCursorCol();

	if (startRow < 0)
		startRow = 0;

	if (startCol < 0)
		startCol = 0;

	int row = startRow;

	for (const auto& rowText : rows) {
		if (rowText.IsEmpty())
			continue;

		wxArrayString columns;
		wxStringTokenizer columnTokenizer(rowText, "\t");

		while (columnTokenizer.HasMoreTokens())
			columns.Add(columnTokenizer.GetNextToken());

		while (row >= m_gridTCC->GetNumberRows())
			m_gridTCC->AppendRows(1);

		for (size_t i = 0; i < columns.size() && startCol + static_cast<int>(i) < m_gridTCC->GetNumberCols(); ++i)
			m_gridTCC->SetCellValue(row, startCol + static_cast<int>(i), columns[i]);

		++row;
	}

	m_gridTCC->ForceRefresh();
}

void BusFormArcFlashProtection::OnGridCornerPaint(wxPaintEvent& event)
{
	wxWindow* corner = m_gridTCC->GetGridCornerLabelWindow();

	wxAutoBufferedPaintDC dc(corner);
	dc.SetBackground(wxBrush(corner->GetBackgroundColour()));
	dc.Clear();

	std::unique_ptr<wxGraphicsContext> gc(wxGraphicsContext::Create(dc));
	if (!gc)
		return;

	gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);

	const wxSize size = corner->GetClientSize();

	const double left = 2.0;
	const double right = size.x - 2.0;
	const double top = 2.0;
	const double bottom = size.y - 2.0;

	if (size.x <= 4 || size.y <= 4)
		return;

	const wxColour axisColour(90, 90, 90);
	const wxColour curveColour(40, 100, 200);

	// Draw axes.
	gc->SetPen(wxPen(axisColour, 1.0));
	gc->StrokeLine(left, top, left, bottom);
	gc->StrokeLine(left, bottom, right, bottom);

	// Define a TCC-like curve.
	const double x0 = left + size.x * 0.23;
	const double y0 = top + size.y * 0.12;

	const double x1 = left + size.x * 0.42;
	const double y1 = top + size.y * 0.50;

	const double x2 = left + size.x * 0.63;
	const double y2 = top + size.y * 0.72;

	const double x3 = right - size.x * 0.08;
	const double y3 = top + size.y * 0.80;

	wxGraphicsPath path = gc->CreatePath();

	path.MoveToPoint(x0, y0);

	path.AddCurveToPoint(
		x0 + size.x * 0.01, y0 + size.y * 0.12,
		x1 - size.x * 0.10, y1 - size.y * 0.12,
		x1, y1
	);

	path.AddCurveToPoint(
		x1 + size.x * 0.08, y1 + size.y * 0.12,
		x2 - size.x * 0.05, y2 - size.y * 0.03,
		x2, y2
	);

	path.AddCurveToPoint(
		x2 + size.x * 0.10, y2 + size.y * 0.06,
		x3 - size.x * 0.08, y3,
		x3, y3
	);

	// Draw the TCC curve.
	gc->SetPen(wxPen(curveColour, m_gridCornerHover ? 2.0 : 1.5));
	gc->StrokePath(path);
}

bool BusFormArcFlashProtection::ParseDouble(const wxString& text, double& value)
{
	wxString str = text;
	str.Trim(true).Trim(false);
	if (str.Find(',') != wxNOT_FOUND)
		str.Replace(",", ".");

	return str.ToCDouble(&value);
}

void BusFormArcFlashProtection::InitializeCBChoices()
{
	m_choiceCBType->Clear();
	m_choiceCBType->Append(_("MCCB"));
	m_choiceCBType->Append(_("ICCB"));
	m_choiceCBType->Append(_("LVPCB"));
	m_choiceCBType->SetSelection(0);

	m_choiceCBVoltage->Clear();
	m_choiceCBVoltage->Append(_("\u2264 480 V"));
	m_choiceCBVoltage->Append(_("575\u2013690 V"));
	m_choiceCBVoltage->SetSelection(0);

	UpdateCBChoices();
}

void BusFormArcFlashProtection::UpdateCBCurrentChoices()
{
	const int type = m_choiceCBType->GetSelection();

	m_choiceCBCurrent->Clear();
	m_choiceCBCurrent->Enable(true);

	switch (type)
	{
	case 0: // MCCB
		m_choiceCBCurrent->Append(_("100\u2013400A"));
		m_choiceCBCurrent->Append(_("600\u20131200A"));
		m_choiceCBCurrent->SetSelection(0);
		break;

	case 1: // ICCB
		m_choiceCBCurrent->Append(_("1600\u20136000A"));
		m_choiceCBCurrent->SetSelection(0);
		m_choiceCBCurrent->Enable(false);
		break;

	case 2: // LVPCB
		m_choiceCBCurrent->Append(_("800\u20136300A"));
		m_choiceCBCurrent->SetSelection(0);
		m_choiceCBCurrent->Enable(false);
		break;
	}
}

void BusFormArcFlashProtection::UpdateCBTripChoices()
{
	const int type = m_choiceCBType->GetSelection();
	const int current = m_choiceCBCurrent->GetSelection();

	m_choiceCBTrip->Clear();
	m_choiceCBTrip->Enable(true);

	if (type == 0) // MCCB
	{
		if (current == 0) // 100–400 A
		{
			m_choiceCBTrip->Append(_("TM or M"));
			m_choiceCBTrip->SetSelection(0);
			m_choiceCBTrip->Enable(false);
		}
		else // 600–1200 A
		{
			m_choiceCBTrip->Append(_("TM or M"));
			m_choiceCBTrip->Append(_("Electronic (E, LI)"));
			m_choiceCBTrip->SetSelection(0);
		}
	}
	else if (type == 1) // ICCB
	{
		m_choiceCBTrip->Append(_("TM or M"));
		m_choiceCBTrip->SetSelection(0);
		m_choiceCBTrip->Enable(false);
	}
	else if (type == 2) // LVPCB
	{
		m_choiceCBTrip->Append(_("Electronic (E, LI)"));
		m_choiceCBTrip->Append(_("Electronic, short-time delay (E, LS)"));
		m_choiceCBTrip->SetSelection(0);
	}
}

void BusFormArcFlashProtection::UpdateCBChoices()
{
	UpdateCBCurrentChoices();
	UpdateCBTripChoices();
}

void BusFormArcFlashProtection::UpdateRadioI1Selection()
{
	if (m_radioBoxI1->GetSelection() == 0) {
		m_textCtrlIn->Enable(false);
		m_staticTextI1->SetLabel(_("Minimum current (I₁) [A]"));
	}
	else {
		m_staticTextI1->SetLabel(_("Instant/Short-Time (Iₜ) [A]"));
		m_textCtrlIn->Enable();
	}
}

void BusFormArcFlashProtection::UpdateFuseCurrentChoices()
{
	m_choiceFuseCurrent->Clear();

	switch (m_choiceFuseClass->GetSelection())
	{
	case 0: // Class L
		m_choiceFuseCurrent->Append(_("601–800A"));
		m_choiceFuseCurrent->Append(_("801–1200A"));
		m_choiceFuseCurrent->Append(_("1201–1600A"));
		m_choiceFuseCurrent->Append(_("1601–2000A"));
		break;

	case 1: // Class RK1
		m_choiceFuseCurrent->Append(_("≤ 100A"));
		m_choiceFuseCurrent->Append(_("101–200A"));
		m_choiceFuseCurrent->Append(_("201–400A"));
		m_choiceFuseCurrent->Append(_("401–600A"));
		break;

	default:
		break;
	}

	if (m_choiceFuseCurrent->GetCount() > 0)
		m_choiceFuseCurrent->SetSelection(0);
}

void BusFormArcFlashProtection::LoadProtectionData(const ArcFlashProtectionData& data)
{
	const bool isBreaker = data.type == ArcFlashProtectionType::CIRCUIT_BREAKER;
	const bool isFuse = !isBreaker;

	// Protection type and method.
	m_choiceType->SetSelection(isBreaker ? 0 : 1);
	m_choiceMethod->SetString(1, isBreaker ? _("Appendix I") : _("Appendix H"));

	const bool isAppendixI = isBreaker && data.method == ArcFlashProtectionMethod::ANNEX_I;
	const bool isAppendixH = isFuse && data.method == ArcFlashProtectionMethod::ANNEX_H;
	const bool validAnnex = isAppendixI || isAppendixH;

	wxASSERT_MSG(data.method == ArcFlashProtectionMethod::TCC_CURVE || validAnnex,
		"Invalid protection type and method combination");

	const int methodSelection = data.method == ArcFlashProtectionMethod::TCC_CURVE || !validAnnex ? 0 : 1;
	m_choiceMethod->SetSelection(methodSelection);

	EnableFields();

	// Show the corresponding notebook page.
	const int page = methodSelection == 0 ? 0 : (isBreaker ? 1 : 2);
	m_notebook->ChangeSelection(page);

	// TCC curve.
	m_gridTCC->BeginBatch();

	if (m_gridTCC->GetNumberRows() > 0)
		m_gridTCC->DeleteRows(0, m_gridTCC->GetNumberRows());

	if (!data.tccPoints.empty())
	{
		m_gridTCC->AppendRows(static_cast<int>(data.tccPoints.size()));

		for (size_t i = 0; i < data.tccPoints.size(); ++i)
		{
			m_gridTCC->SetCellValue(static_cast<int>(i), 0, FormatProtectionNumber(data.tccPoints[i].current));
			m_gridTCC->SetCellValue(static_cast<int>(i), 1, FormatProtectionNumber(data.tccPoints[i].time));
		}
	}

	m_gridTCC->EndBatch();
	m_gridTCC->ForceRefresh();

	m_checkBoxIsMeltTime->SetValue(data.curveIsMeltTimeOnly);
	m_textCtrlDelay->ChangeValue(FormatOptionalNumber(data.additionalDelay));

	// Appendix I: map BreakerFamily to the three cascading choices.
	int breakerType = 0;
	int currentRange = 0;
	int tripUnit = 0;

	switch (data.breakerFamily)
	{
	case ArcFlashBreakerFamily::MCCB_100_400A:
		breakerType = 0; currentRange = 0; tripUnit = 0;
		break;

	case ArcFlashBreakerFamily::MCCB_600_1200A_TM_OR_M:
		breakerType = 0; currentRange = 1; tripUnit = 0;
		break;

	case ArcFlashBreakerFamily::MCCB_600_1200A_E_LI:
		breakerType = 0; currentRange = 1; tripUnit = 1;
		break;

	case ArcFlashBreakerFamily::MCCB_OR_ICCB_1600_6000A:
		breakerType = 1; currentRange = 0; tripUnit = 0;
		break;

	case ArcFlashBreakerFamily::LVPCB_800_6300A_E_LI:
		breakerType = 2; currentRange = 0; tripUnit = 0;
		break;

	case ArcFlashBreakerFamily::LVPCB_800_6300A_E_LS:
		breakerType = 2; currentRange = 0; tripUnit = 1;
		break;
	}

	m_choiceCBType->SetSelection(breakerType);
	UpdateCBCurrentChoices();

	m_choiceCBCurrent->SetSelection(currentRange);
	UpdateCBTripChoices();

	m_choiceCBTrip->SetSelection(tripUnit);
	m_choiceCBVoltage->SetSelection(static_cast<int>(data.breakerVoltageBand));

	m_textCtrlI2->ChangeValue(FormatOptionalNumber(data.i2_kA));

	// Load I1 source and update the I1/It label and field state.
	const int i1Selection = data.i1Source == ArcFlashI1Source::DIRECT ? 0 : 1;
	m_radioBoxI1->SetSelection(i1Selection);
	UpdateRadioI1Selection();

	// The nominal current is optional and is not the same as It.
	m_textCtrlIn->ChangeValue(FormatOptionalNumber(data.breakerNominalCurrent_A));

	// The input field is I1 in amperes, or It in amperes, depending on the radio selection.
	if (data.i1Source == ArcFlashI1Source::DIRECT)
		m_textCtrl1->ChangeValue(FormatOptionalNumber(data.i1_kA * 1000.0));
	else
		m_textCtrl1->ChangeValue(FormatOptionalNumber(data.it_A));

	// Appendix H: map FuseFamily to class and current range.
	const int fuseFamily = static_cast<int>(data.fuseFamily);

	m_choiceFuseClass->SetSelection(fuseFamily < 4 ? 0 : 1);
	UpdateFuseCurrentChoices();
	m_choiceFuseCurrent->SetSelection(fuseFamily < 4 ? fuseFamily : fuseFamily - 4);

	// Update the "Use" checkbox for the currently selected device.
	//const int selectedRow = m_dvListCtrlDevices->GetSelectedRow();
	//
	//if (selectedRow != wxNOT_FOUND)
	//	m_dvListCtrlDevices->SetToggleValue(data.consider, selectedRow, 0);

	m_notebook->Layout();
	m_splitterPageRight->Layout();
	Layout();

	if (m_tccPopup && m_tccPopup->IsShown())
		m_tccPopup->UpdateGraph();
}

bool BusFormArcFlashProtection::ValidateAndGetProtectionData(ArcFlashProtectionData& data)
{
	ArcFlashProtectionData result = data;

	auto showError = [&](const wxString& message) {
		wxMessageBox(message, _("Invalid data"), wxOK | wxICON_WARNING, m_parent);
		return false;
		};

	auto parseDouble = [&](const wxString& text, double& value, const wxString& field) {
		return m_bus->DoubleFromString(
			m_parent,
			text,
			value,
			wxString::Format(_("Value entered incorrectly in the field \"%s\"."), field));
		};

	auto requirePositive = [&](double value, const wxString& field) {
		if (!std::isfinite(value) || value <= 0.0)
			return showError(wxString::Format(
				_("The field \"%s\" must be greater than zero."), field));

		return true;
		};

	// Protection type.
	const int typeSelection = m_choiceType->GetSelection();

	if (typeSelection < 0 || typeSelection > 1)
		return showError(_("Select a valid protection type."));

	result.type = typeSelection == 0
		? ArcFlashProtectionType::CIRCUIT_BREAKER
		: ArcFlashProtectionType::FUSE;

	// Protection method.
	const int methodSelection = m_choiceMethod->GetSelection();

	if (methodSelection < 0 || methodSelection > 1)
		return showError(_("Select a valid protection method."));

	if (methodSelection == 0)
		result.method = ArcFlashProtectionMethod::TCC_CURVE;
	else if (result.type == ArcFlashProtectionType::CIRCUIT_BREAKER)
		result.method = ArcFlashProtectionMethod::ANNEX_I;
	else
		result.method = ArcFlashProtectionMethod::ANNEX_H;

	// Additional delay is optional; an empty field means zero.
	wxString delayText = m_textCtrlDelay->GetValue();
	delayText.Trim(false).Trim(true);

	if (delayText.IsEmpty())
		result.additionalDelay = 0.0;
	else if (!parseDouble(delayText, result.additionalDelay, _("Additional delay (s)")))
		return false;

	if (!std::isfinite(result.additionalDelay) || result.additionalDelay < 0.0)
		return showError(_("The additional delay must be greater than or equal to zero."));

	// TCC curve.
	if (result.method == ArcFlashProtectionMethod::TCC_CURVE) {
		const int rows = m_gridTCC->GetNumberRows();

		if (m_gridTCC->GetNumberCols() < 2 || rows < 2)
			return showError(_("The TCC curve must contain at least two points."));

		std::vector<ArcFlashTCCPoint> points;
		points.reserve(rows);

		for (int row = 0; row < rows; ++row) {
			ArcFlashTCCPoint point;

			const wxString currentField =
				wxString::Format(_("Current (A), row %d of the TCC curve"), row + 1);

			const wxString timeField =
				wxString::Format(_("Time (s), row %d of the TCC curve"), row + 1);

			if (!parseDouble(m_gridTCC->GetCellValue(row, 0), point.current, currentField))
				return false;

			if (!requirePositive(point.current, currentField))
				return false;

			if (!parseDouble(m_gridTCC->GetCellValue(row, 1), point.time, timeField))
				return false;

			if (!requirePositive(point.time, timeField))
				return false;

			points.push_back(point);
		}

		result.tccPoints = points;
		result.curveIsMeltTimeOnly = m_checkBoxIsMeltTime->GetValue();
	}

	// Appendix I: circuit breaker.
	if (result.method == ArcFlashProtectionMethod::ANNEX_I) {
		const int cbType = m_choiceCBType->GetSelection();
		const int currentRange = m_choiceCBCurrent->GetSelection();
		const int tripUnit = m_choiceCBTrip->GetSelection();

		if (cbType < 0 || cbType > 2 || currentRange < 0 || tripUnit < 0)
			return showError(_("Select valid circuit breaker parameters."));

		// Map the type, current range and trip unit to the breaker family.
		switch (cbType) {
		case 0: // MCCB
			if (currentRange == 0 && tripUnit == 0) {
				result.breakerFamily = ArcFlashBreakerFamily::MCCB_100_400A;
			}
			else if (currentRange == 1 && tripUnit == 0) {
				result.breakerFamily = ArcFlashBreakerFamily::MCCB_600_1200A_TM_OR_M;
			}
			else if (currentRange == 1 && tripUnit == 1) {
				result.breakerFamily = ArcFlashBreakerFamily::MCCB_600_1200A_E_LI;
			}
			else {
				return showError(
					_("The selected MCCB current range and trip unit are not compatible."));
			}
			break;

		case 1: // ICCB
			if (currentRange != 0 || tripUnit != 0)
				return showError(
					_("The selected ICCB current range and trip unit are not compatible."));

			result.breakerFamily = ArcFlashBreakerFamily::MCCB_OR_ICCB_1600_6000A;
			break;

		case 2: // LVPCB
			if (currentRange != 0 || (tripUnit != 0 && tripUnit != 1))
				return showError(
					_("The selected LVPCB current range and trip unit are not compatible."));

			result.breakerFamily = tripUnit == 0
				? ArcFlashBreakerFamily::LVPCB_800_6300A_E_LI
				: ArcFlashBreakerFamily::LVPCB_800_6300A_E_LS;
			break;
		}

		// Voltage range.
		const int voltageSelection = m_choiceCBVoltage->GetSelection();

		if (voltageSelection < 0 || voltageSelection > 1)
			return showError(_("Select a valid circuit breaker voltage range."));

		result.breakerVoltageBand = voltageSelection == 0
			? ArcFlashBreakerVoltageBand::V480_AND_LOWER
			: ArcFlashBreakerVoltageBand::V575_TO_690;

		// Interrupting capacity I2, in kA.
		if (!parseDouble(m_textCtrlI2->GetValue(), result.i2_kA,
			_("Interrupting capacity (I₂) [kA]")))
			return false;

		if (!requirePositive(result.i2_kA, _("Interrupting capacity (I₂) [kA]")))
			return false;

		// I1 entered directly, or calculated from It.
		const int i1Selection = m_radioBoxI1->GetSelection();

		if (i1Selection < 0 || i1Selection > 1)
			return showError(_("Select a valid minimum current calculation method."));

		if (i1Selection == 0) {
			double i1_A = 0.0;

			if (!parseDouble(m_textCtrl1->GetValue(), i1_A,
				_("Minimum current (I₁) [A]")))
				return false;

			if (!requirePositive(i1_A, _("Minimum current (I₁) [A]")))
				return false;

			result.i1Source = ArcFlashI1Source::DIRECT;
			result.i1_kA = i1_A / 1000.0;
			result.it_A = 0.0;
			result.breakerNominalCurrent_A = 0.0;
		}
		else {
			double nominalCurrent_A = 0.0;
			double it_A = 0.0;

			if (!parseDouble(m_textCtrlIn->GetValue(), nominalCurrent_A,
				_("Nominal current [A]")))
				return false;

			if (!requirePositive(nominalCurrent_A, _("Nominal current [A]")))
				return false;

			if (!parseDouble(m_textCtrl1->GetValue(), it_A,
				_("Instant/Short-Time current (Iₜ) [A]")))
				return false;

			if (!requirePositive(it_A, _("Instant/Short-Time current (Iₜ) [A]")))
				return false;

			result.i1Source = ArcFlashI1Source::FROM_IT;
			result.breakerNominalCurrent_A = nominalCurrent_A;
			result.it_A = it_A;
			result.i1_kA = 0.0;
		}
	}

	// Appendix H: fuse.
	if (result.method == ArcFlashProtectionMethod::ANNEX_H) {
		const int fuseClass = m_choiceFuseClass->GetSelection();
		const int currentSelection = m_choiceFuseCurrent->GetSelection();
		const int currentCount = static_cast<int>(m_choiceFuseCurrent->GetCount());

		if (fuseClass < 0 || fuseClass > 1 || currentSelection < 0)
			return showError(_("Select a valid fuse class and current range."));

		int familyIndex = -1;

		if (currentCount == 4) {
			// The current-range choices contain only the ranges for the selected class.
			familyIndex = fuseClass * 4 + currentSelection;
		}
		else if (currentCount == 8) {
			// Support the original list containing both fuse classes.
			const int firstIndex = fuseClass * 4;

			if (currentSelection < firstIndex || currentSelection >= firstIndex + 4)
				return showError(
					_("The selected fuse current range does not match the fuse class."));

			familyIndex = currentSelection;
		}
		else {
			return showError(_("The fuse current range choices are invalid."));
		}

		if (familyIndex < 0 || familyIndex > 7)
			return showError(_("Select a valid fuse current range."));

		result.fuseFamily = static_cast<ArcFlashFuseFamily>(familyIndex);
	}

	// Use flag associated with the currently selected device.
	const int selectedRow = m_dvListCtrlDevices->GetSelectedRow();

	//if (selectedRow >= 0)
	//	result.consider = m_dvListCtrlDevices->GetToggleValue(selectedRow, 0);

	// The remaining fields that have no corresponding controls are preserved from the original data structure.
	result.configured = true;

	data = result;
	return true;
}

TCCPopup::TCCPopup(wxWindow* parent, wxGrid* grid) : wxPopupTransientWindow(parent, wxBORDER_SIMPLE), m_grid(grid)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	SetSize(500, 600);

	Bind(wxEVT_PAINT, &TCCPopup::OnPaint, this);
}

TCCPopup::~TCCPopup()
{
	Unbind(wxEVT_PAINT, &TCCPopup::OnPaint, this);
}

void TCCPopup::UpdateGraph()
{
	Refresh();
	Update();
}

bool TCCPopup::GetCellDouble(int row, int col, double& value) const
{
	wxString str = m_grid->GetCellValue(row, col);
	return str.ToDouble(&value);
}

void TCCPopup::OnPaint(wxPaintEvent& event)
{
	wxAutoBufferedPaintDC dc(this);

	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();

	if (!m_grid)
		return;

	std::unique_ptr<wxGraphicsContext> gc(wxGraphicsContext::Create(dc));

	if (!gc)
		return;

	gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);

	const wxSize size = GetClientSize();

	const int left = 60;
	const int right = 20;
	const int top = 20;
	const int bottom = 50;

	const int graphWidth = size.x - left - right;
	const int graphHeight = size.y - top - bottom;

	if (graphWidth <= 0 || graphHeight <= 0)
		return;

	struct Point
	{
		double current;
		double time;
	};

	std::vector<Point> data;

	double minCurrent = std::numeric_limits<double>::max();
	double maxCurrent = std::numeric_limits<double>::lowest();
	double minTime = std::numeric_limits<double>::max();
	double maxTime = std::numeric_limits<double>::lowest();

	for (int row = 0; row < m_grid->GetNumberRows(); ++row)
	{
		double current = 0.0;
		double time = 0.0;

		if (!GetCellDouble(row, 0, current) || !GetCellDouble(row, 1, time))
			continue;

		if (current <= 0.0 || time <= 0.0)
			continue;

		data.push_back({ current, time });

		minCurrent = std::min(minCurrent, current);
		maxCurrent = std::max(maxCurrent, current);
		minTime = std::min(minTime, time);
		maxTime = std::max(maxTime, time);
	}

	if (data.size() < 2)
		return;

	// Use complete decades for both axes.
	double logMinCurrent = std::floor(std::log10(minCurrent));
	double logMaxCurrent = std::ceil(std::log10(maxCurrent));
	double logMinTime = std::floor(std::log10(minTime));
	double logMaxTime = std::ceil(std::log10(maxTime));

	if (logMinCurrent == logMaxCurrent)
	{
		--logMinCurrent;
		++logMaxCurrent;
	}

	if (logMinTime == logMaxTime)
	{
		--logMinTime;
		++logMaxTime;
	}

	auto mapX = [&](double current) {
		return left + (std::log10(current) - logMinCurrent) / (logMaxCurrent - logMinCurrent) * graphWidth;
		};

	auto mapY = [&](double time) {
		return size.y - bottom - (std::log10(time) - logMinTime) / (logMaxTime - logMinTime) * graphHeight;
		};

	const int firstCurrentDecade = static_cast<int>(logMinCurrent);
	const int lastCurrentDecade = static_cast<int>(logMaxCurrent);

	const int firstTimeDecade = static_cast<int>(logMinTime);
	const int lastTimeDecade = static_cast<int>(logMaxTime);

	// Minor vertical grid lines: current.
	gc->SetPen(wxPen(wxColour(230, 230, 230), 1));

	for (int exponent = firstCurrentDecade; exponent <= lastCurrentDecade; ++exponent)
	{
		const double decade = std::pow(10.0, exponent);

		for (int i = 2; i <= 9; ++i)
		{
			const double current = i * decade;
			const double x = mapX(current);

			if (x >= left && x <= size.x - right)
				gc->StrokeLine(x, top, x, size.y - bottom);
		}
	}

	// Minor horizontal grid lines: time.
	for (int exponent = firstTimeDecade; exponent <= lastTimeDecade; ++exponent)
	{
		const double decade = std::pow(10.0, exponent);

		for (int i = 2; i <= 9; ++i)
		{
			const double time = i * decade;
			const double y = mapY(time);

			if (y >= top && y <= size.y - bottom)
				gc->StrokeLine(left, y, size.x - right, y);
		}
	}

	// Major vertical grid lines: current.
	gc->SetPen(wxPen(wxColour(190, 190, 190), 1));

	for (int exponent = firstCurrentDecade; exponent <= lastCurrentDecade; ++exponent)
	{
		const double current = std::pow(10.0, exponent);
		const double x = mapX(current);

		if (x >= left && x <= size.x - right)
			gc->StrokeLine(x, top, x, size.y - bottom);
	}

	// Major horizontal grid lines: time.
	for (int exponent = firstTimeDecade; exponent <= lastTimeDecade; ++exponent)
	{
		const double time = std::pow(10.0, exponent);
		const double y = mapY(time);

		if (y >= top && y <= size.y - bottom)
			gc->StrokeLine(left, y, size.x - right, y);
	}

	// Axes.
	gc->SetPen(wxPen(*wxBLACK, 1.5));
	gc->StrokeLine(left, top, left, size.y - bottom);
	gc->StrokeLine(left, size.y - bottom, size.x - right, size.y - bottom);

	// X-axis labels: current.
	gc->SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT), *wxBLACK);

	for (int exponent = firstCurrentDecade; exponent <= lastCurrentDecade; ++exponent)
	{
		const double current = std::pow(10.0, exponent);
		const double x = mapX(current);

		if (x < left || x > size.x - right)
			continue;

		const wxString label = wxString::Format("1e%d", exponent);

		double textWidth;
		double textHeight;
		gc->GetTextExtent(label, &textWidth, &textHeight);

		gc->DrawText(label, x - textWidth / 2.0, size.y - bottom + 5);
	}

	// Y-axis labels: time.
	for (int exponent = firstTimeDecade; exponent <= lastTimeDecade; ++exponent)
	{
		const double time = std::pow(10.0, exponent);
		const double y = mapY(time);

		if (y < top || y > size.y - bottom)
			continue;

		const wxString label = wxString::Format("1e%d", exponent);

		double textWidth;
		double textHeight;
		gc->GetTextExtent(label, &textWidth, &textHeight);

		gc->DrawText(label, left - textWidth - 5, y - textHeight / 2.0);
	}

	// TCC curve.
	wxGraphicsPath path = gc->CreatePath();

	for (size_t i = 0; i < data.size(); ++i)
	{
		const double x = mapX(data[i].current);
		const double y = mapY(data[i].time);

		if (i == 0)
			path.MoveToPoint(x, y);
		else
			path.AddLineToPoint(x, y);
	}

	gc->SetPen(wxPen(wxColour(40, 100, 200), 2.0));
	gc->StrokePath(path);

	// X-axis title.
	const wxString xLabel = _("Current (A)");

	double xLabelWidth;
	double xLabelHeight;
	gc->GetTextExtent(xLabel, &xLabelWidth, &xLabelHeight);

	gc->DrawText(xLabel, left + (graphWidth - xLabelWidth) / 2.0, size.y - 20);

	// Y-axis title.
	const wxString yLabel = _("Time (s)");

	double yLabelWidth;
	double yLabelHeight;
	gc->GetTextExtent(yLabel, &yLabelWidth, &yLabelHeight);

	gc->PushState();
	gc->Translate(15, top + graphHeight / 2.0);
	gc->Rotate(-M_PI / 2.0);
	gc->DrawText(yLabel, -yLabelWidth / 2.0, -yLabelHeight / 2.0);
	gc->PopState();
}
void BusFormArcFlashProtection::OnChoiceCBCurrent(wxCommandEvent& event)
{
	UpdateCBTripChoices();
}
void BusFormArcFlashProtection::OnChoiceCBTrip(wxCommandEvent& event)
{
	//UpdateCBChoices();
}
void BusFormArcFlashProtection::OnChoiceCBType(wxCommandEvent& event)
{
	UpdateCBChoices();
}
void BusFormArcFlashProtection::OnRadioI1Selected(wxCommandEvent& event)
{
	UpdateRadioI1Selection();
}
void BusFormArcFlashProtection::OnNominalCurrentUpdated(wxCommandEvent& event)
{
	double value = 0.0;
	if (m_textCtrlIn->GetValue().ToDouble(&value))
	{
		if (value <= 100.0) {
			m_textCtrl1->SetValue(wxString::FromDouble(1300));
		}
		else {
			m_textCtrl1->SetValue(wxString::FromDouble(value * 10.0));
		}
	}
}
void BusFormArcFlashProtection::OnChoiceFuseClass(wxCommandEvent& event)
{
	UpdateFuseCurrentChoices();
}
