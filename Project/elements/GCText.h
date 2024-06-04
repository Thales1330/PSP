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

#ifndef GCTEXT_H
#define GCTEXT_H

#include <wx/graphics.h>
#include <wx/intl.h>

 /**
  * @class GCText
  * @author Thales Lima Oliveira <thales@ufu.br>
  * @date 16/05/2024
  * @brief Class to draw text on Graphics Context using wxWidgets.
  * @file GCText.h
  */
class GCText
{
public:
	GCText();
	GCText(wxString text);
	virtual ~GCText();
	/**
	 * @brief Draw the text in wxGraphicsContext.
	 * 
	 * \param position Text position.
	 * \param gc Graphics Context.
	 * \param angle Text angle.
	 * \param colour Text colour.
	 */
	virtual void Draw(wxPoint2DDouble position, wxGraphicsContext* gc, double angle = 0.0, wxColour colour = *wxBLACK) const;
	/**
	 * \brief Get a deep text copy.
	 * \warning The original object is not freed.
	 * \return New text pointer.
	 */
	virtual GCText* GetCopy();

	/**
	 * \brief Set correctly a new text string.
	 * 
	 * \param text New text string.
	 */
	virtual void SetText(wxString text);
	virtual wxString GetText() const { return m_text; }
	virtual int GetWidth() const { return m_size.GetWidth(); }
	virtual int GetHeight() const { return m_size.GetHeight(); }
	virtual void SetFontSize(int fontSize) { m_fontSize = fontSize; }
	virtual void SetFontWeight(wxFontWeight fontWeight) { m_fontWeight = fontWeight; }
	virtual void SetFontStyle(wxFontStyle fontStyle) { m_fontStyle = fontStyle; }
	virtual void SetFontFamily(wxFontFamily fontFamily) { m_fontFamily = fontFamily; }

protected:

	wxString m_text = _("Text");
	int m_fontSize = 10;
	wxFontWeight m_fontWeight = wxFONTWEIGHT_NORMAL;
	wxFontStyle m_fontStyle = wxFONTSTYLE_NORMAL;
	wxFontFamily m_fontFamily = wxFONTFAMILY_DEFAULT;

	wxSize m_size;
};

#endif  // GCTEXT_H
