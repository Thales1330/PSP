#include "TransferFunction.h"

TransferFunction::TransferFunction()
{
    // Superscript unicode numbers
    m_supNumber[0] = L'\u2070';
    m_supNumber[1] = L'\u00B9';
    m_supNumber[2] = L'\u00B2';
    m_supNumber[3] = L'\u00B3';
    m_supNumber[4] = L'\u2074';
    m_supNumber[5] = L'\u2075';
    m_supNumber[6] = L'\u2076';
    m_supNumber[7] = L'\u2077';
    m_supNumber[8] = L'\u2078';
    m_supNumber[9] = L'\u2079';

    wxString str = "";
    for(int i = 0; i < 10; i++) {
        str += "0,1s" + wxString(m_supNumber[i]) + " ";
    }
    SetText(str, "");

    m_width = 100;
    m_height = 50;
    SetPosition(m_position); // Update rect;
}

TransferFunction::~TransferFunction() {}

void TransferFunction::Draw(wxPoint2DDouble translation, double scale) const
{
    if(m_selected) {
        glColor4dv(m_selectionColour.GetRGBA());
        double borderSize = (m_borderSize * 2.0 + 1.0) / scale;
        DrawRectangle(m_position, m_width + borderSize, m_height + borderSize);
    }
    glColor4d(1.0, 1.0, 1.0, 1.0);
    DrawRectangle(m_position, m_width, m_height);
    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawRectangle(m_position, m_width, m_height, GL_LINE_LOOP);
    
    glEnable(GL_TEXTURE_2D);
    glColor4d(0.0, 0.0, 0.0, 1.0);
    m_glStringNum->bind();
    m_glStringNum->render(m_position.m_x, m_position.m_y);
    glDisable(GL_TEXTURE_2D);
}

void TransferFunction::SetText(wxString numerator, wxString denominator)
{
    wxFont font(m_fontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    wxScreenDC dc;

    if(m_glStringNum) {
        delete m_glStringNum;
        m_glStringNum = NULL;
    }
    m_glStringNum = new wxGLString(numerator);
    m_glStringNum->setFont(font);
    m_glStringNum->consolidate(&dc);
}
