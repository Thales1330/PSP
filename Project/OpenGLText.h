/*
 *  Copyright (C) 2017  Thales Lima Oliveira <thales@ufu.br>
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

#ifndef OPENGLTEXT_H
#define OPENGLTEXT_H

#include <GL/gl.h>
#include <wx/dcmemory.h>

/**
 * @class OpenGLText
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 24/10/2017
 * @brief Class to draw text on OpenGL using wxWidgets.
 * @file OpenGLText.h
 */
class OpenGLText
{
   public:
    OpenGLText();
    OpenGLText(wxString text);
    virtual ~OpenGLText();

    virtual void Draw(wxPoint2DDouble position) const;
    virtual OpenGLText* GetCopy();

    virtual void SetText(wxString text);
    virtual wxString GetText() const { return m_text; }
    virtual int GetWidth() const { return m_bitmapSize.GetWidth(); }
    virtual int GetHeight() const { return m_bitmapSize.GetHeight(); }
    virtual bool IsTextureOK();
   protected:
    void Init();
    int RoundToPowerOfTwo(int value, int min = 32);
    void TextToBitmap();
    void LoadTextTexture();

    wxString m_text = _("Text");
    int m_fontSize = 10;

    wxBitmap m_bitmap = wxNullBitmap;
    wxSize m_bitmapSize = wxSize(0, 0);
    wxPoint2DDouble* m_textCoord = NULL;
    GLuint* m_textureID = NULL;
};

#endif  // OPENGLTEXT_H