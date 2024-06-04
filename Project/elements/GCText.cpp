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
#include <wx/dcmemory.h>

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
	gc->SetFont(wxFont(m_fontSize, m_fontFamily, m_fontStyle, m_fontWeight), colour);
	gc->DrawText(m_text, position.m_x, position.m_y, angle);
}

void GCText::SetText(wxString text)
{
	m_text = text;

	wxFont font = wxFont(m_fontSize, m_fontFamily, m_fontStyle, m_fontWeight);

	wxMemoryDC memDC;
	memDC.SetFont(font);
	m_size = memDC.GetTextExtent(m_text);
}


GCText* GCText::GetCopy()
{
	GCText* copy = new GCText();
	*copy = *this;

	copy->SetText(copy->m_text);
	return copy;
}
