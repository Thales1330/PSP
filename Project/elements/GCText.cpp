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
		//m_font.SetFaceName(wxT("CMU Serif"));
		//m_font.SetPointSize(m_fontSize);
		m_font = wxFont(m_fontSize,
			wxFONTFAMILY_DEFAULT,
			wxFONTSTYLE_NORMAL,
			wxFONTWEIGHT_NORMAL,
			false,
			wxT("Arial"));
	}

	//wxFont font = wxFont(m_fontSize, m_fontFamily, m_fontStyle, m_fontWeight);
	//wxBitmap bmp(1, 1);
	//wxMemoryDC mdc(bmp);
	//
	//wxGraphicsContext* gc = wxGraphicsContext::Create(mdc);
	//
	//gc->SetFont(m_font, *wxBLACK);
	//
	//wxDouble w, h, descent, extLead;
	//gc->GetTextExtent(text, &w, &h, &descent, &extLead);
	//
	//delete gc;
	//
	//m_size =  wxSize((int)w, (int)h);

	wxScreenDC dc;
	dc.SetFont(m_font);
	m_size = dc.GetTextExtent(m_text);

	//m_size = CalculateTextExtend();


	//wxMemoryDC memDC;
	////memDC.SetFont(font);
	//
	//wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
	//if (gc) {
	//	gc->SetFont(m_font, *wxBLACK);
	//	double width, height, descent, externalLeading;
	//	gc->GetTextExtent(m_text, &width, &height, &descent, &externalLeading);
	//	m_size = wxSize(std::ceil(width + descent), std::ceil(height + externalLeading));
	//	delete gc;
	//}
	//else {
	//	memDC.SetFont(m_font);
	//	m_size = memDC.GetTextExtent(m_text);
	//}
}

void GCText::SetFont(wxFont font)
{
	m_font = font;
	m_customFont = true;
}

//wxSize GCText::CalculateTextExtend()
//{
//	// DC offscreen
//	wxBitmap bmp(1, 1);
//	wxMemoryDC mdc(bmp);
//
//	mdc.SetFont(m_font);
//
//	wxGCDC dc(mdc);
//	dc.SetFont(m_font);
//
//	wxCoord w, h;
//	dc.GetTextExtent(m_text, &w, &h);
//
//	wxArrayInt partial;
//	dc.GetPartialTextExtents(m_text, partial);
//
//	int leftOffset = 0;
//
//	if (!m_text.empty() && partial.size() >= 1)
//	{
//		wxCoord firstGlyphW, firstGlyphH;
//		dc.GetTextExtent(m_text.Mid(0, 1), &firstGlyphW, &firstGlyphH);
//
//		int advanceFirst = partial[0];
//
//		// se o glifo invade a esquerda → bearing negativo
//		leftOffset = firstGlyphW - advanceFirst;
//	}
//
//	return wxSize(w + leftOffset, h);
//}


GCText* GCText::GetCopy()
{
	GCText* copy = new GCText();
	*copy = *this;

	copy->SetText(copy->m_text);
	return copy;
}
