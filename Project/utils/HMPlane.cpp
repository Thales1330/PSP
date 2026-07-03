#include "HMPlane.h"

//#include "Renderer.h"

//#include "VertexBuffer.h"
//#include "VertexBufferLayout.h"
//#include "IndexBuffer.h"
//#include "VertexArray.h"
//#include "Shader.h"

//#include "OpenGLText.h"

#include <wx/msgdlg.h>
#include <wx/dc.h>
#include <wx/settings.h>

#include <algorithm>


HMPlane::HMPlane(const double& width, const double& height, const double limits[2]) : m_width(width), m_height(height)
{
	// Fill mesh coords
	for (auto accHeight = 0; accHeight <= m_height + m_meshSize; accHeight += m_meshSize) {
		std::vector<BufferMeshCoords*> line;
		for (auto accWidth = 0; accWidth <= m_width + m_meshSize; accWidth += m_meshSize) {
			auto* bmc = new BufferMeshCoords;
			bmc->x = accWidth;
			bmc->y = accHeight;
			bmc->z = 0.0f;
			line.emplace_back(bmc);

			if (accHeight < 0.1f) m_meshTickX++;
		}
		m_meshTickY++;
		m_coords.emplace_back(line);
	}
	m_coordsT.resize(m_coords[0].size());
	for (auto& line : m_coordsT) line.resize(m_coords.size());
	for (size_t i = 0; i < m_coords.size(); ++i) {
		const auto& line = m_coords[i];
		for (size_t j = 0; j < line.size(); ++j) {
			m_coordsT[j][i] = m_coords[i][j];
		}
	}

	m_limits[0] = limits[0];
	m_limits[1] = limits[1];
}

HMPlane::~HMPlane()
{
	for (const auto& line : m_coords) {
		for (auto* bmv : line) {
			delete bmv;
		}
	}
	m_coords.clear();
}

void HMPlane::DrawDC(wxGraphicsContext* gc) const
{
	if (m_isClear) return;
	gc->SetPen(*wxTRANSPARENT_PEN);
	for (const auto& line : m_coords) {
		wxGraphicsGradientStops gStops;
		gStops.Add(wxColor(255, 255, 255), 0);
		for (const BufferMeshCoords* coords : line) {
			gStops.Add(VoltToColour(coords->z), coords->x / (m_width));
		}
		wxGraphicsBrush brush = gc->CreateLinearGradientBrush(0, line[0]->y, m_width, line[0]->y, gStops);
		gc->SetBrush(brush);
		gc->DrawRectangle(0, line[0]->y, m_width, m_meshSize);
	}

	for (const auto& line : m_coordsT) {
		wxGraphicsGradientStops gStops;
		gStops.Add(wxColor(255, 255, 255), 0);
		for (const BufferMeshCoords* coords : line) {
			gStops.Add(VoltToColour(coords->z), coords->y / (m_height));
		}
		wxGraphicsBrush brush = gc->CreateLinearGradientBrush(line[0]->x, 0, line[0]->x, m_height, gStops);
		gc->SetBrush(brush);
		gc->DrawRectangle(line[0]->x, 0, m_meshSize, m_height);
	}
}

void HMPlane::DrawDC(wxDC& dc) const
{
	if (m_isClear) return;
	dc.SetPen(*wxTRANSPARENT_PEN);

	for (const auto& line : m_coords)
	{
		int y = wxRound(line[0]->y);

		struct Stop {
			double pos;
			wxColour color;
		};

		std::vector<Stop> stops;

		stops.push_back({ 0.0, wxColour(255,255,255) });

		for (const BufferMeshCoords* coords : line)
		{
			double pos = coords->x / m_width;
			stops.push_back({ pos, VoltToColour(coords->z) });
		}

		auto ColorAt = [&](double t) -> wxColour
			{
				if (t <= stops.front().pos)
					return stops.front().color;

				if (t >= stops.back().pos)
					return stops.back().color;

				for (size_t i = 0; i < stops.size() - 1; ++i)
				{
					if (t >= stops[i].pos && t <= stops[i + 1].pos)
					{
						double localT =
							(t - stops[i].pos) /
							(stops[i + 1].pos - stops[i].pos);

						const wxColour& c1 = stops[i].color;
						const wxColour& c2 = stops[i + 1].color;

						return wxColour(
							c1.Red() + localT * (c2.Red() - c1.Red()),
							c1.Green() + localT * (c2.Green() - c1.Green()),
							c1.Blue() + localT * (c2.Blue() - c1.Blue())
						);
					}
				}

				return stops.back().color;
			};

		for (size_t i = 0; i < stops.size() - 1; ++i)
		{
			int x1 = wxRound(stops[i].pos * m_width);
			int x2 = wxRound(stops[i + 1].pos * m_width);

			if (x2 <= x1)
				continue;

			double t1 = stops[i].pos;
			double t2 = stops[i + 1].pos;

			wxColour c1 = ColorAt(t1);
			wxColour c2 = ColorAt(t2);

			wxRect rect(x1, y, x2 - x1, m_meshSize);

			dc.GradientFillLinear(rect, c1, c2, wxEAST);
		}
	}
}

void HMPlane::DrawLabelDC(wxGraphicsContext* gc) const
{
	wxGraphicsMatrix identityMatrix = gc->GetTransform();
	identityMatrix.Set();
	gc->PushState();
	gc->SetTransform(identityMatrix);
	gc->SetPen(*wxBLACK_PEN);
	if (wxSystemSettings::GetAppearance().IsDark())
	{
		gc->SetPen(wxPen(wxColour(205, 210, 215)));
	}
	wxGraphicsGradientStops gStops;
	gStops.Add(VoltToColour(-1.0, 210), 0.0);
	gStops.Add(VoltToColour(-0.5, 210), 0.25);
	gStops.Add(VoltToColour(0.0, 210), 0.5);
	gStops.Add(VoltToColour(0.5, 210), 0.75);
	gStops.Add(VoltToColour(1.0, 210), 1.0);
	wxGraphicsBrush brush = gc->CreateLinearGradientBrush(30, m_height - 45, 330, m_height - 45, gStops);
	gc->SetBrush(brush);

	gc->DrawRectangle(30, m_height - 45, 300, 30);

	wxPoint2DDouble lines[10];
	lines[0] = wxPoint2DDouble(30, m_height - 50);
	lines[1] = wxPoint2DDouble(30, m_height - 40);
	lines[2] = wxPoint2DDouble(105, m_height - 50);
	lines[3] = wxPoint2DDouble(105, m_height - 40);
	lines[4] = wxPoint2DDouble(180, m_height - 50);
	lines[5] = wxPoint2DDouble(180, m_height - 40);
	lines[6] = wxPoint2DDouble(255, m_height - 50);
	lines[7] = wxPoint2DDouble(255, m_height - 40);
	lines[8] = wxPoint2DDouble(330, m_height - 50);
	lines[9] = wxPoint2DDouble(330, m_height - 40);
	//gc->DrawLines(10, lines, wxWINDING_RULE);
	for (size_t i = 0; i < 10; i += 2) {
		gc->StrokeLine(lines[i].m_x, lines[i].m_y, lines[i + 1].m_x, lines[i + 1].m_y);
	}
	wxColour fontColour = *wxBLACK;
	if (wxSystemSettings::GetAppearance().IsDark())
	{
		fontColour = wxColour(205, 210, 215);
	}

	gc->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL), fontColour);
	gc->DrawText(_("Voltage (p.u.)"), 30, m_height - 15);
	wxString voltageText = "";
	double textWidth, textHeight;
	voltageText = wxString::Format("%.3f", m_limits[0]);
	gc->GetTextExtent(voltageText, &textWidth, &textHeight);
	gc->DrawText(voltageText, 330 - textWidth / 2, m_height - 65);
	voltageText = wxString::Format("%.3f", m_limits[0] * 0.75 + m_limits[1] * 0.25);
	gc->GetTextExtent(voltageText, &textWidth, &textHeight);
	gc->DrawText(voltageText, 255 - textWidth / 2, m_height - 65);
	voltageText = wxString::Format("%.3f", m_limits[0] * 0.5 + m_limits[1] * 0.5);
	gc->GetTextExtent(voltageText, &textWidth, &textHeight);
	gc->DrawText(voltageText, 180 - textWidth / 2, m_height - 65);
	voltageText = wxString::Format("%.3f", m_limits[0] * 0.25 + m_limits[1] * 0.75);
	gc->GetTextExtent(voltageText, &textWidth, &textHeight);
	gc->DrawText(voltageText, 105 - textWidth / 2, m_height - 65);
	voltageText = wxString::Format("%.3f", m_limits[1]);
	gc->GetTextExtent(voltageText, &textWidth, &textHeight);
	gc->DrawText(voltageText, 30 - textWidth / 2, m_height - 65);

	gc->PopState();
}

void HMPlane::DrawLabelDC(wxDC& dc) const
{
	dc.SetUserScale(1.0, 1.0);
	dc.SetDeviceOrigin(0, 0);

	dc.SetPen(*wxBLACK_PEN);
	if (wxSystemSettings::GetAppearance().IsDark())
	{
		dc.SetPen(wxPen(wxColour(205, 210, 215)));
	}
	int xStart = 30;
	int yStart = m_height - 45;
	int width = 300;
	int height = 30;

	struct Stop {
		double pos;
		wxColour color;
	};

	std::vector<Stop> stops = {
		{0.00, VoltToColour(-1.0, 210)},
		{0.25, VoltToColour(-0.5, 210)},
		{0.50, VoltToColour(0.0, 210)},
		{0.75, VoltToColour(0.5, 210)},
		{1.00, VoltToColour(1.0, 210)}
	};

	auto ColorAt = [&](double t) -> wxColour
		{
			for (size_t i = 0; i < stops.size() - 1; ++i)
			{
				if (t >= stops[i].pos && t <= stops[i + 1].pos)
				{
					double localT =
						(t - stops[i].pos) /
						(stops[i + 1].pos - stops[i].pos);

					const wxColour& c1 = stops[i].color;
					const wxColour& c2 = stops[i + 1].color;

					return wxColour(
						c1.Red() + localT * (c2.Red() - c1.Red()),
						c1.Green() + localT * (c2.Green() - c1.Green()),
						c1.Blue() + localT * (c2.Blue() - c1.Blue())
					);
				}
			}
			return stops.back().color;
		};

	for (int x = 0; x < width; ++x)
	{
		double t = (double)x / width;

		wxColour c = ColorAt(t);

		dc.SetPen(wxPen(c));
		dc.DrawLine(xStart + x,
			yStart,
			xStart + x,
			yStart + height);
	}

	dc.SetPen(*wxBLACK_PEN);
	if (wxSystemSettings::GetAppearance().IsDark())
	{
		dc.SetPen(wxPen(wxColour(205, 210, 215)));
	}
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.DrawRectangle(xStart, yStart, width, height);
	wxPoint2DDouble lines[10];
	lines[0] = wxPoint2DDouble(30, m_height - 50);
	lines[1] = wxPoint2DDouble(30, m_height - 40);
	lines[2] = wxPoint2DDouble(105, m_height - 50);
	lines[3] = wxPoint2DDouble(105, m_height - 40);
	lines[4] = wxPoint2DDouble(180, m_height - 50);
	lines[5] = wxPoint2DDouble(180, m_height - 40);
	lines[6] = wxPoint2DDouble(255, m_height - 50);
	lines[7] = wxPoint2DDouble(255, m_height - 40);
	lines[8] = wxPoint2DDouble(330, m_height - 50);
	lines[9] = wxPoint2DDouble(330, m_height - 40);
	//gc->DrawLines(10, lines, wxWINDING_RULE);
	for (size_t i = 0; i < 10; i += 2) {
		dc.DrawLine(lines[i].m_x, lines[i].m_y, lines[i + 1].m_x, lines[i + 1].m_y);
	}

	dc.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	dc.DrawText(_("Voltage (p.u.)"), 30, m_height - 15);
	wxString voltageText = "";
	int textWidth, textHeight;
	voltageText = wxString::Format("%.3f", m_limits[0]);
	dc.GetTextExtent(voltageText, &textWidth, &textHeight);
	dc.DrawText(voltageText, 330 - textWidth / 2, m_height - 65);
	voltageText = wxString::Format("%.3f", m_limits[0] * 0.75 + m_limits[1] * 0.25);
	dc.GetTextExtent(voltageText, &textWidth, &textHeight);
	dc.DrawText(voltageText, 255 - textWidth / 2, m_height - 65);
	voltageText = wxString::Format("%.3f", m_limits[0] * 0.5 + m_limits[1] * 0.5);
	dc.GetTextExtent(voltageText, &textWidth, &textHeight);
	dc.DrawText(voltageText, 180 - textWidth / 2, m_height - 65);
	voltageText = wxString::Format("%.3f", m_limits[0] * 0.25 + m_limits[1] * 0.75);
	dc.GetTextExtent(voltageText, &textWidth, &textHeight);
	dc.DrawText(voltageText, 105 - textWidth / 2, m_height - 65);
	voltageText = wxString::Format("%.3f", m_limits[1]);
	dc.GetTextExtent(voltageText, &textWidth, &textHeight);
	dc.DrawText(voltageText, 30 - textWidth / 2, m_height - 65);
}

void HMPlane::SetLabelLimits(const double& min, const double& max)
{
	m_limits[0] = max;
	m_limits[1] = min;
}

void HMPlane::SetRectSlope(const wxRect2DDouble& rect, const double& angle, const double& depth)
{
	for (const auto& line : m_coords) {
		for (auto* coord : line) {
			wxPoint2DDouble pt(coord->x, coord->y);
			if (std::abs(angle) > 0.01) {

				wxPoint2DDouble rotPt;
				rotPt.m_x = cos(angle) * (pt.m_x - rect.GetCentre().m_x) + sin(angle) * (pt.m_y - rect.GetCentre().m_y) + rect.GetCentre().m_x;
				rotPt.m_y = sin(angle) * (pt.m_x - rect.GetCentre().m_x) - cos(angle) * (pt.m_y - rect.GetCentre().m_y) + rect.GetCentre().m_y;

				pt = rotPt;
			}
			if (rect.Contains(pt)) {
				coord->z += depth;
				coord->z = std::min<double>(coord->z, 1.0f);
				coord->z = std::max<double>(coord->z, -1.0f);
			}

		}
	}
	m_isClear = false;
}

void HMPlane::Resize(const double& width, const double& height)
{
	//Clear();
	m_width = width;
	m_height = height;
	m_meshTickX = 0;
	m_meshTickY = 0;

	for (const auto& line : m_coords) {
		for (auto* bmv : line) {
			delete bmv;
		}
	}
	m_coords.clear();

	// Fill mesh coords
	for (auto accHeight = 0; accHeight < m_height + m_meshSize; accHeight += m_meshSize) {
		std::vector<BufferMeshCoords*> line;
		for (auto accWidth = 0; accWidth < m_width + m_meshSize; accWidth += m_meshSize) {
			auto* bmc = new BufferMeshCoords;
			bmc->x = accWidth;
			bmc->y = accHeight;
			bmc->z = 0.0f;
			line.emplace_back(bmc);

			if (accHeight < 0.1f) m_meshTickX++;
		}
		m_meshTickY++;
		m_coords.emplace_back(line);
	}
	FillCoordsBuffer();
	m_isClear = false;
}

void HMPlane::ResizeDC(const double& width, const double& height)
{
	m_width = width;
	m_height = height;
	m_meshTickX = 0;
	m_meshTickY = 0;

	for (const auto& line : m_coords) {
		for (auto* bmv : line) {
			delete bmv;
		}
	}
	m_coords.clear();
	m_coordsT.clear();

	// Fill mesh coords
	for (auto accHeight = 0; accHeight < m_height + m_meshSize; accHeight += m_meshSize) {
		std::vector<BufferMeshCoords*> line;
		for (auto accWidth = 0; accWidth < m_width + m_meshSize; accWidth += m_meshSize) {
			auto* bmc = new BufferMeshCoords;
			bmc->x = accWidth;
			bmc->y = accHeight;
			bmc->z = 0.0f;
			line.emplace_back(bmc);

			if (accHeight < 0.1f) m_meshTickX++;
		}
		m_meshTickY++;
		m_coords.emplace_back(line);
	}
	m_coordsT.resize(m_coords[0].size());
	for (auto& line : m_coordsT) line.resize(m_coords.size());
	for (size_t i = 0; i < m_coords.size(); ++i) {
		const auto& line = m_coords[i];
		for (size_t j = 0; j < line.size(); ++j) {
			m_coordsT[j][i] = m_coords[i][j];
		}
	}

	m_isClear = false;
}

void HMPlane::SmoothPlane(const unsigned int& iterations)
{
	const int maxTickX = static_cast<int>(m_meshTickX);
	const int maxTickY = static_cast<int>(m_meshTickY);

	std::vector< std::vector<BufferMeshCoords> > tmpCoords;
	for (const auto& line : m_coords) {
		std::vector<BufferMeshCoords> tmpCoordsLine;
		for (auto* bmv : line) {
			tmpCoordsLine.push_back(*bmv);
		}
		tmpCoords.push_back(tmpCoordsLine);
	}

	//Mean Blur
	//for (int i = 0; i < maxTick; ++i) {
	//    for (int j = 0; j < maxTick; ++j) {
	//        // Get the 8 neighbors and smooth z
	//        float mid = 0.0f;
	//        float div = 0.0f;
	//
	//        for (int ii = i - 1; ii <= i + 1; ++ii) {
	//            for (int jj = j - 1; jj <= j + 1; ++jj) {
	//
	//                if (ii >= 0 && ii < maxTick && jj >= 0 && jj < maxTick) {
	//                    mid += tmpCoords[ii][jj].z;
	//                    div += 1.0;
	//                }
	//            }
	//        }
	//        m_coords[i][j]->z = mid / div;
	//    }
	//}

	// Gaussian Blur
	float gaussianKernel[5][5] =
	{
	  {1.0f / 256.0f, 4.0f / 256.0f, 6 / 256.0f, 4.0f / 256, 1.0f / 256.0f},
	  {4.0f / 256.0f, 16.0f / 256.0f, 24 / 256.0f, 16.0f / 256, 4.0f / 256.0f},
	  {6.0f / 256.0f, 24.0f / 256.0f, 36 / 256.0f, 24.0f / 256, 6.0f / 256.0f},
	  {4.0f / 256.0f, 16.0f / 256.0f, 24 / 256.0f, 16.0f / 256, 4.0f / 256.0f},
	  {1.0f / 256.0f, 4.0f / 256.0f, 6 / 256.0f, 4.0f / 256, 1.0f / 256.0f}
	};

	for (size_t it = 0; it < iterations; ++it) {
		for (size_t i = 0; i < m_meshTickY; ++i) {
			for (size_t j = 0; j < m_meshTickX; ++j) {
				// Get the 24 neighbors and smooth z
				float value = 0.0f;

				for (size_t ii = i - 2; ii <= i + 2; ++ii) {
					for (size_t jj = j - 2; jj <= j + 2; ++jj) {

						if (ii >= 0 && ii < m_meshTickY && jj >= 0 && jj < m_meshTickX) {
							value += tmpCoords[ii][jj].z * gaussianKernel[ii - i + 2][jj - j + 2];
						}
					}
				}
				m_coords[i][j]->z = value;
			}
		}
		if (it < iterations - 1) {
			for (size_t i = 0; i < m_meshTickY; ++i) {
				for (size_t j = 0; j < m_meshTickX; ++j) {
					tmpCoords[i][j].z = m_coords[i][j]->z;
				}
			}
		}
	}

	FillCoordsBuffer();
	m_isClear = false;
}

void HMPlane::Clear()
{
	if (!m_isClear) {
		for (const auto& line : m_coords) {
			for (auto* bmv : line) {
				bmv->z = 0.0f;
			}
		}
		FillCoordsBuffer();
		m_isClear = true;
	}
}

void HMPlane::FillCoordsBuffer()
{
	m_bufferCoords.clear();

	for (const auto& line : m_coords) {
		for (auto* bmv : line) {
			m_bufferCoords.push_back(bmv->x);
			m_bufferCoords.push_back(bmv->y);
			m_bufferCoords.push_back(bmv->z);
		}
	}
}

wxColor HMPlane::VoltToColour(double volt, int alpha) const
{
	int red = 255, green = 255, blue = 255;
	if (wxSystemSettings::GetAppearance().IsDark())
	{
		red = 43;
		green = 46;
		blue = 52;

		if (volt <= -0.5) {
			red = 100 * volt + 100;
			green = -160 * volt + 40;
			blue = -150 * volt + 105;
		}
		else if (volt > -0.5 && volt < 0.0) {
			red = -14 * volt + 43;
			green = -148 * volt + 46;
			blue = -256 * volt + 52;
		}
		else if (volt >= 0.0 && volt < 0.5) {
			red = 304 * volt + 43;
			green = 198 * volt + 46;
			blue = -44 * volt + 52;
		}
		else { 
			red = 80 * volt + 155;
			green = 70 * volt + 110;
			blue = 30 * volt + 15;
		}

	}
	else {
		if (volt <= -0.5) {
			red = -100 * volt - 50;
			green = 200 * volt + 300;
			blue = 255;
		}
		else if (volt > -0.5 && volt < 0) {
			red = 510 * volt + 255;
			green = 110 * volt + 255;
			blue = 255;
		}
		else if (volt >= 0.0 && volt < 0.5) {
			red = 255;
			green = -110 * volt + 255;
			blue = -510 * volt + 255;
		}
		else if (volt >= 0.5) {
			red = 255;
			green = -200 * volt + 300;
			blue = 100 * volt - 50;
		}
	}
	return wxColor(red, green, blue, alpha);
}
