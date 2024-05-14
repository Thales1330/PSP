#include "HMPlane.h"

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include "OpenGLText.h"

#include <wx/msgdlg.h>

HMPlane::HMPlane(Shader* shader, Shader* labelShader, const float& width, const float& height, const float limits[2])
	: m_width(width), m_height(height), m_shader(shader), m_labelShader(labelShader)
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


	m_limits[0] = limits[0];
	m_limits[1] = limits[1];

	FillCoordsBuffer();
	FillIndexBuffer();
	BindOpenGLBuffers();

	CreateLabel();
}

HMPlane::HMPlane(const float& width, const float& height, const float limits[2]) : m_width(width), m_height(height)
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
	delete m_ib;
	delete m_vb;
	delete m_layout;
	delete m_va;

	delete m_ibL;
	delete m_vbL;
	delete m_layoutL;
	delete m_vaL;

	for (auto glText : m_glTexts) {
		delete glText;
	}
	m_glTexts.clear();

	for (const auto& line : m_coords) {
		for (auto* bmv : line) {
			delete bmv;
		}
	}
	m_coords.clear();
}

void HMPlane::Draw(const Renderer& renderer, const glm::mat4& projectionViewMatrix) const
{
	//const glm::mat4 mvp = projectionViewMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(m_width / 2.0f, m_height / 2.0f, 0.0f));
	const glm::mat4 mvp = projectionViewMatrix;

	m_shader->Bind();
	//m_shader->SetUniform1f("u_scale", m_scale);
	m_shader->SetUniformMatrix4fv("u_mvpMatrix", mvp);

	m_va->UpdateBuffer(*m_vb, m_bufferCoords.data(), m_bufferCoords.size() * sizeof(float));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	renderer.Draw(*m_va, *m_ib, *m_shader);

	// Unbind
	m_va->Unbind();
	m_vb->Unbind();
	m_ib->Unbind();

	m_shader->Unbind();
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

void HMPlane::DrawLabel(const Renderer& renderer, const glm::mat4& projectionViewMatrix, const float& x, const float& y) const
{
	const glm::mat4 mvp = projectionViewMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
	//const glm::mat4 mvp = projectionViewMatrix;

	m_labelShader->Bind();
	//m_shader->SetUniform1f("u_scale", m_scale);
	m_labelShader->SetUniformMatrix4fv("u_mvpMatrix", mvp);
	m_labelShader->SetUniform4f("u_offset", x, y, m_width, m_height);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	renderer.Draw(*m_vaL, *m_ibL, *m_labelShader);

	// Unbind
	m_vaL->Unbind();
	m_vbL->Unbind();
	m_ibL->Unbind();

	m_labelShader->Unbind();

	glColor4d(0.0, 0.0, 0.0, 1.0);
	m_glTexts[0]->Draw(wxPoint2DDouble(x + 35.0 + m_glTexts[0]->GetWidth() / 2.0, y));
	m_glTexts[1]->Draw(wxPoint2DDouble(x + 35.0 + m_glTexts[1]->GetWidth() / 2.0, y + 150.0));
	m_glTexts[2]->Draw(wxPoint2DDouble(x + 35.0 + m_glTexts[2]->GetWidth() / 2.0, y + 300.0));
}

void HMPlane::DrawLabelDC(wxGraphicsContext* gc) const
{
	wxGraphicsMatrix identityMatrix = gc->GetTransform();
	identityMatrix.Set();
	gc->PushState();
	gc->SetTransform(identityMatrix);
	gc->SetPen(*wxBLACK_PEN);
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

	gc->SetFont(gc->CreateFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)));
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

void HMPlane::SetLabelLimits(const float& min, const float& max)
{
	m_limits[0] = max;
	m_limits[1] = min;

	if (m_glTexts.size() > 0) {
		m_glTexts[0]->SetText(wxString::Format("%.2f p.u.", m_limits[0]));
		m_glTexts[1]->SetText(wxString::Format("%.2f p.u.", (m_limits[0] + m_limits[1]) / 2.0f));
		m_glTexts[2]->SetText(wxString::Format("%.2f p.u.", m_limits[1]));
	}
}

void HMPlane::SetRectSlope(const wxRect2DDouble& rect, const float& angle, const float& depth)
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
				if (coord->z > 1.0f) coord->z = 1.0f;
				if (coord->z < -1.0f) coord->z = -1.0f;
			}

		}
	}
	m_isClear = false;
	//FillCoordsBuffer();
	//SmoothPlane();
}

void HMPlane::Resize(const float& width, const float& height)
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
	FillIndexBuffer();

	BindOpenGLBuffers();

	//m_va->Bind();
	//m_vb->Bind();
	//
	//delete m_ib;
	//m_ib = new IndexBuffer(m_indexBuffer.data(), m_indexBuffer.size());
	//m_ib->Unbind();
	m_isClear = false;
}

void HMPlane::ResizeDC(const float& width, const float& height)
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

	for (unsigned int it = 0; it < iterations; ++it) {
		for (int i = 0; i < m_meshTickY; ++i) {
			for (int j = 0; j < m_meshTickX; ++j) {
				// Get the 24 neighbors and smooth z
				float value = 0.0f;

				for (int ii = i - 2; ii <= i + 2; ++ii) {
					for (int jj = j - 2; jj <= j + 2; ++jj) {

						if (ii >= 0 && ii < m_meshTickY && jj >= 0 && jj < m_meshTickX) {
							value += tmpCoords[ii][jj].z * gaussianKernel[ii - i + 2][jj - j + 2];
						}
					}
				}
				m_coords[i][j]->z = value;
			}
		}
		if (it < iterations - 1) {
			for (int i = 0; i < m_meshTickY; ++i) {
				for (int j = 0; j < m_meshTickX; ++j) {
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

void HMPlane::FillIndexBuffer()
{
	m_indexBuffer.clear();
	for (auto i = 0; i < m_meshTickY - 1; ++i) {
		for (auto j = 0; j < m_meshTickX - 1; ++j) {
			m_indexBuffer.push_back(i * m_meshTickX + j);
			m_indexBuffer.push_back(i * m_meshTickX + j + 1);
			m_indexBuffer.push_back((i + 1) * m_meshTickX + j + 1);
			m_indexBuffer.push_back(i * m_meshTickX + j);
			m_indexBuffer.push_back((i + 1) * m_meshTickX + j);
			m_indexBuffer.push_back((i + 1) * m_meshTickX + j + 1);
		}
	}
}

void HMPlane::BindOpenGLBuffers()
{
	delete m_va;
	delete m_vb;
	delete m_ib;
	// Generate vertex array (this will bind vertex buffer with its layout)
	m_va = new VertexArray();

	// Generate vertex buffer
	m_vb = new VertexBuffer(m_bufferCoords.data(), m_bufferCoords.size() * sizeof(float), GL_DYNAMIC_DRAW);

	m_layout = new VertexBufferLayout();
	m_layout->Push<float>(3); // 3 = each triplet from array set the coords -> [x1, y1, z1, x2, y2, z2, ...]
	m_va->AddBuffer(*m_vb, *m_layout); // Add buffer and bind vertex array

	// Set index buffer
	m_ib = new IndexBuffer(m_indexBuffer.data(), m_indexBuffer.size());

	// Unbind
	m_va->Unbind();
	m_vb->Unbind();
	m_ib->Unbind();
}

void HMPlane::CreateLabel()
{
	float vertexBufferLabel[4 * 3] =
	{
		0.0f, 0.0f, 1.0f,
		30.0f, 0.0f, 1.0f,
		0.0f, 300.0f, -1.0f,
		30.0f, 300.0f, -1.0f
	};
	std::copy(vertexBufferLabel, vertexBufferLabel + 3 * 4, m_vertexBufferLabel);

	unsigned int indexBufferLabel[6] =
	{
		0, 1, 2,
		1, 2, 3
	};
	std::copy(indexBufferLabel, indexBufferLabel + 6, m_indexBufferLabel);

	delete m_vaL;
	delete m_vbL;
	delete m_ibL;
	// Generate vertex array (this will bind vertex buffer with its layout)
	m_vaL = new VertexArray();

	// Generate vertex buffer
	m_vbL = new VertexBuffer(m_vertexBufferLabel, 4 * 3 * sizeof(float), GL_STATIC_DRAW);

	m_layoutL = new VertexBufferLayout();
	m_layoutL->Push<float>(3); // 3 = each triplet from array set the coords -> [x1, y1, z1, x2, y2, z2, ...]
	m_vaL->AddBuffer(*m_vbL, *m_layoutL); // Add buffer and bind vertex array

	// Set index buffer
	m_ibL = new IndexBuffer(m_indexBufferLabel, 6);

	// Unbind
	m_vaL->Unbind();
	m_vbL->Unbind();
	m_ibL->Unbind();

	m_glTexts.emplace_back(new OpenGLText(wxString::Format("%.2f p.u.", m_limits[0])));
	m_glTexts.emplace_back(new OpenGLText(wxString::Format("%.2f p.u.", (m_limits[0] + m_limits[1]) / 2.0)));
	m_glTexts.emplace_back(new OpenGLText(wxString::Format("%.2f p.u.", m_limits[1])));
}

wxColor HMPlane::VoltToColour(double volt, int alpha) const
{
	//int red = 255 + 255 * (volt < 0 ? volt : 0);
	//int green = 255;
	//int blue = 255 - 255 * (volt >= 0 ? volt : 0);
	//int red = volt >= 0 ? 255 : 255 - 255 * (std::abs(volt));
	//int green = 255 - 205 * (std::abs(volt));
	//int blue = volt <= 0 ? 255 : 255 - 255 * (std::abs(volt));
	int red = 255, green = 255, blue = 255;
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
	return wxColor(red, green, blue, alpha);
}
