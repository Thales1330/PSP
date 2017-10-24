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

#include "OpenGLText.h"

OpenGLText::OpenGLText() { Init(); }
OpenGLText::OpenGLText(wxString text)
{
    Init();
    SetText(text);
}

OpenGLText::~OpenGLText()
{
    if(m_textureID) {
        glDeleteTextures(1, m_textureID);
    }
}

void OpenGLText::Init()
{
    m_textCoord = new wxPoint2DDouble[2];
    m_textCoord[0] = wxPoint2DDouble(0, 1);
    m_textCoord[1] = wxPoint2DDouble(1, 0);
}

void OpenGLText::Draw(wxPoint2DDouble position) const
{
    if(m_textureID) {
        glPushMatrix();
        
        glTranslated(position.m_x - m_bitmapSize.GetWidth() / 2, position.m_y - m_bitmapSize.GetHeight() / 2, 0);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_textureID[0]);

        glBegin(GL_QUADS);

        glTexCoord2f(m_textCoord[0].m_x, m_textCoord[0].m_y);
        glVertex2f(0, 0);

        glTexCoord2f(m_textCoord[1].m_x, m_textCoord[0].m_y);
        glVertex2f(m_bitmapSize.GetWidth(), 0);

        glTexCoord2f(m_textCoord[1].m_x, m_textCoord[1].m_y);
        glVertex2f(m_bitmapSize.GetWidth(), m_bitmapSize.GetHeight());

        glTexCoord2f(m_textCoord[0].m_x, m_textCoord[1].m_y);
        glVertex2f(0, m_bitmapSize.GetHeight());
        glEnd();

        glDisable(GL_TEXTURE_2D);

        glPopMatrix();
    }
}

void OpenGLText::SetText(wxString text)
{
    m_text = text;
    TextToBitmap();
    LoadTextTexture();
}

int OpenGLText::RoundToPowerOfTwo(int value, int min)
{
    //[Ref] https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
    double baseOfTwo = std::log(static_cast<double>(value)) / std::log(2.0);
    int powerOfTwo = static_cast<int>(std::pow(2.0, static_cast<int>(std::ceil(baseOfTwo))));
    return std::max(min, powerOfTwo);
}

void OpenGLText::TextToBitmap()
{
    wxFont font = wxFont(m_fontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    wxMemoryDC memDC;
    memDC.SetFont(font);
    m_bitmapSize = memDC.GetTextExtent(m_text);

    int p2w = RoundToPowerOfTwo(m_bitmapSize.GetWidth());
    int p2h = RoundToPowerOfTwo(m_bitmapSize.GetHeight());

    m_bitmap = wxBitmap(p2w, p2h);

    memDC.SelectObject(m_bitmap);
    memDC.SetBackground(*wxWHITE_BRUSH);
    memDC.Clear();
    memDC.DrawText(m_text, 0, 0);

    m_textCoord[1].m_x = static_cast<double>(m_bitmapSize.GetWidth()) / static_cast<double>(p2w);
    m_textCoord[1].m_y = 1.0 - static_cast<double>(m_bitmapSize.GetHeight()) / static_cast<double>(p2h);
}

void OpenGLText::LoadTextTexture()
{
    if(m_textureID) glDeleteTextures(1, m_textureID);
    m_textureID = new GLuint[1];
    glGenTextures(1, &m_textureID[0]);

    glBindTexture(GL_TEXTURE_2D, *m_textureID);

    wxImage img = m_bitmap.ConvertToImage();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    const int w = img.GetWidth(), h = img.GetHeight();
    int bytesPerPixel = 4;
    GLubyte* bitmapData = img.GetData();
    int imageSize = w * h * bytesPerPixel;
    GLubyte* imageData = new GLubyte[imageSize];
    int revVal = h - 1;

    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            imageData[(x + y * w) * bytesPerPixel + 0] = 255;
            imageData[(x + y * w) * bytesPerPixel + 1] = 255;
            imageData[(x + y * w) * bytesPerPixel + 2] = 255;

            // alpha
            imageData[(x + y * w) * bytesPerPixel + 3] = 255 - bitmapData[(x + (revVal - y) * w) * 3];
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, bytesPerPixel, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    delete imageData;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

OpenGLText* OpenGLText::GetCopy()
{
    OpenGLText* copy = new OpenGLText();
    *copy = *this;
    copy->m_textureID = NULL;
    copy->m_bitmapSize = wxSize(0, 0);
    copy->m_bitmap = wxNullBitmap;
    copy->SetText(copy->m_text);
    return copy;
}
