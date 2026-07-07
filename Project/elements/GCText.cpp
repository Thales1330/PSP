/*
 *  Copyright (C) 2024  Thales Lima Oliveira <thales@ufu.br>
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

#include "GCText.h"
#include <wx/dcscreen.h>
#include <wx/dcmemory.h>
#include <wx/dcgraph.h>
#include <wx/dcclient.h>
#include <wx/graphics.h>

GCText::GCText() {}
GCText::GCText(wxString text)
{
	SetText(text);
}

GCText::~GCText()
{
}

void GCText::Draw(wxPoint2DDouble position, wxGraphicsContext* gc, double angle, wxColour colour) const
{
	//gc->SetFont(wxFont(m_fontSize, m_fontFamily, m_fontStyle, m_fontWeight), colour);

	gc->SetFont(m_font, colour);
	gc->DrawText(m_text, position.m_x, position.m_y, angle);
}

void GCText::Draw(wxPoint2DDouble position, double width, double height, wxDC& dc, double angle, wxColour colour) const
{
	dc.SetFont(m_font);
	dc.SetPen(wxPen(colour));
	dc.SetTextForeground(colour);

	if(angle == 0.0) {
		dc.DrawText(m_text, position.m_x - width / 2.0, position.m_y - height / 2.0);
		return;
	}

	double rad = wxDegToRad(angle);

	double dx = -width / 2.0;
	double dy = -height / 2.0;

	double rx = dx * cos(rad) - dy * sin(rad);
	double ry = dx * sin(rad) + dy * cos(rad);

	double drawX = position.m_x + rx;
	double drawY = position.m_y + ry;

	dc.DrawRotatedText(m_text, drawX, drawY, -angle);
}

void GCText::SetText(wxString text)
{
	m_text = text;
	if (!m_customFont) {
		m_font = wxFont(m_fontSize,
			wxFONTFAMILY_DEFAULT,
			wxFONTSTYLE_NORMAL,
			wxFONTWEIGHT_NORMAL,
			false,
			wxT("Arial"));
	}


	wxScreenDC dc;
	dc.SetFont(m_font);
	m_size = dc.GetTextExtent(m_text);
}

void GCText::SetText(wxString text, wxSize size)
{
	m_text = text;
	m_size = size;
}

void GCText::SetFont(wxFont font)
{
	m_font = font;
	m_customFont = true;
	SetText(m_text); // Recalculate the size with the new font
}

GCText* GCText::GetCopy()
{
	GCText* copy = new GCText();
	*copy = *this;
	return copy;
}
