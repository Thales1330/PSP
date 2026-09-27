#include "BusFormArcFlashProtection.h"

#include <wx/dcbuffer.h>

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

	for (unsigned int i = 0; i < m_dvListCtrlDevices->GetColumnCount(); ++i)
		devicesWidth += m_dvListCtrlDevices->GetBestColumnWidth(i);

	m_dvListCtrlDevices->SetMinSize(wxSize(devicesWidth, 100));

	m_splitterPageLeft->Layout();
	m_splitterPageRight->Layout();

	GetSizer()->Fit(this);

	m_splitter->SetSashGravity(1.0);

	const int rightWidth = m_splitterPageRight->GetSizer()->CalcMin().x;
	const int sashPosition = m_splitter->GetClientSize().x - rightWidth - m_splitter->GetSashSize();

	m_splitter->SetSashPosition(sashPosition);

	// Chart popup
	wxWindow* corner = m_gridTCC->GetGridCornerLabelWindow();
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
			wxPoint mousePos = wxGetMousePosition();
			m_tccPopup->Move(mousePos + wxPoint(15, 15));
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

TCCPopup::TCCPopup(wxWindow* parent, wxGrid* grid) : wxPopupTransientWindow(parent, wxBORDER_SIMPLE), m_grid(grid)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	SetSize(320, 220);

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

	const wxSize size = GetClientSize();

	const int left = 55;
	const int right = 15;
	const int top = 15;
	const int bottom = 45;

	const int graphWidth = size.x - left - right;
	const int graphHeight = size.y - top - bottom;

	if (graphWidth <= 0 || graphHeight <= 0)
		return;

	struct Point {
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

	double logMinCurrent = std::log10(minCurrent);
	double logMaxCurrent = std::log10(maxCurrent);
	double logMinTime = std::log10(minTime);
	double logMaxTime = std::log10(maxTime);

	if (logMinCurrent == logMaxCurrent) {
		logMinCurrent -= 0.5;
		logMaxCurrent += 0.5;
	}

	if (logMinTime == logMaxTime) {
		logMinTime -= 0.5;
		logMaxTime += 0.5;
	}

	const double currentMargin = (logMaxCurrent - logMinCurrent) * 0.05;
	const double timeMargin = (logMaxTime - logMinTime) * 0.05;

	logMinCurrent -= currentMargin;
	logMaxCurrent += currentMargin;
	logMinTime -= timeMargin;
	logMaxTime += timeMargin;

	auto mapX = [&](double current) {
		return left + static_cast<int>((std::log10(current) - logMinCurrent) / (logMaxCurrent - logMinCurrent) * graphWidth);
		};

	auto mapY = [&](double time) {
		return size.y - bottom - static_cast<int>((std::log10(time) - logMinTime) / (logMaxTime - logMinTime) * graphHeight);
		};

	const int firstCurrentDecade = static_cast<int>(std::floor(logMinCurrent));
	const int lastCurrentDecade = static_cast<int>(std::ceil(logMaxCurrent));

	const int firstTimeDecade = static_cast<int>(std::floor(logMinTime));
	const int lastTimeDecade = static_cast<int>(std::ceil(logMaxTime));

	// Vertical grid lines: current
	dc.SetPen(wxPen(wxColour(220, 220, 220), 1));

	for (int exponent = firstCurrentDecade; exponent <= lastCurrentDecade; ++exponent)
	{
		const double current = std::pow(10.0, exponent);
		const int x = mapX(current);

		if (x >= left && x <= size.x - right)
			dc.DrawLine(x, top, x, size.y - bottom);
	}

	// Horizontal grid lines: time
	for (int exponent = firstTimeDecade; exponent <= lastTimeDecade; ++exponent)
	{
		const double time = std::pow(10.0, exponent);
		const int y = mapY(time);

		if (y >= top && y <= size.y - bottom)
			dc.DrawLine(left, y, size.x - right, y);
	}

	// Axes
	dc.SetPen(*wxBLACK_PEN);
	dc.DrawLine(left, top, left, size.y - bottom);
	dc.DrawLine(left, size.y - bottom, size.x - right, size.y - bottom);

	// X-axis values: current
	dc.SetTextForeground(*wxBLACK);

	for (int exponent = firstCurrentDecade; exponent <= lastCurrentDecade; ++exponent)
	{
		const double current = std::pow(10.0, exponent);
		const int x = mapX(current);

		if (x < left || x > size.x - right)
			continue;

		const wxString label = wxString::Format("1e%d", exponent);

		int textWidth;
		int textHeight;
		dc.GetTextExtent(label, &textWidth, &textHeight);

		dc.DrawText(label, x - textWidth / 2, size.y - bottom + 5);
	}

	// Y-axis values: time
	for (int exponent = firstTimeDecade; exponent <= lastTimeDecade; ++exponent)
	{
		const double time = std::pow(10.0, exponent);
		const int y = mapY(time);

		if (y < top || y > size.y - bottom)
			continue;

		const wxString label = wxString::Format("1e%d", exponent);

		int textWidth;
		int textHeight;
		dc.GetTextExtent(label, &textWidth, &textHeight);

		dc.DrawText(label, left - textWidth - 5, y - textHeight / 2);
	}

	// TCC curve
	std::vector<wxPoint> points;
	points.reserve(data.size());

	for (const auto& point : data)
		points.emplace_back(mapX(point.current), mapY(point.time));

	dc.SetPen(wxPen(wxColour(40, 100, 200), 2));

	for (size_t i = 1; i < points.size(); ++i)
		dc.DrawLine(points[i - 1], points[i]);

	// X-axis title
	const wxString xLabel = _("Current (A)");
	int xLabelWidth;
	int xLabelHeight;
	dc.GetTextExtent(xLabel, &xLabelWidth, &xLabelHeight);

	dc.DrawText(xLabel, left + (graphWidth - xLabelWidth) / 2, size.y - 20);

	// Y-axis title
	const wxString yLabel = _("Time (s)");
	int yLabelWidth;
	int yLabelHeight;
	dc.GetTextExtent(yLabel, &yLabelWidth, &yLabelHeight);

	dc.DrawRotatedText(yLabel, 15, top + (graphHeight + yLabelWidth) / 2, 90);
}
