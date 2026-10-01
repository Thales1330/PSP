#include "BusFormArcFlashProtection.h"

#include <wx/clipbrd.h>
#include <wx/dcbuffer.h>
#include <wx/textfile.h>
#include <wx/filedlg.h>

#include "../elements/powerElement/Bus.h"
#include "../elements/powerElement/Line.h"
#include "../elements/powerElement/Transformer.h"


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

		bool check = faultCurrent > 1e-3 && data.faultCurrent[busIndex][0].real() > 1e-6;

		wxString deviceName = data.name;
		wxString faultCurrentString = Bus::StringFromDouble(faultCurrent / 1e3, 0, 2);

		m_dvListCtrlDevices->AppendItem({
			wxVariant(check),
			wxVariant(deviceName),
			wxVariant(faultCurrentString)
			});
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

	m_gridTCC->SetMinSize(wxSize(gridWidth + 30, -1));
	m_splitterPageRight->SetMinSize(wxSize(gridWidth + 30, -1));

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

	const int rightWidth = m_splitterPageRight->GetSizer()->CalcMin().x;
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
