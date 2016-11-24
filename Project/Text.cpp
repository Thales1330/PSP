#include "Text.h"

#include "Bus.h"
#include "Line.h"
#include "Transformer.h"
#include "SyncGenerator.h"
#include "IndMotor.h"
#include "SyncMotor.h"
#include "Load.h"
#include "Inductor.h"
#include "Capacitor.h"

Text::Text() : Element() { SetText(m_text); }
Text::Text(wxPoint2DDouble position) : Element()
{
    SetText(m_text);
    SetPosition(position);
}

Text::~Text() {}
bool Text::Contains(wxPoint2DDouble position) const
{
    wxPoint2DDouble ptR = RotateAtPosition(position, -m_angle);
    return m_rect.Contains(ptR);
}

void Text::Draw(wxPoint2DDouble translation, double scale)
{
    wxScreenDC dc;
    
    // Draw selection rectangle
    
    // Push the current matrix on stack.
    glPushMatrix();
    // Rotate the matrix around the object position.
    glTranslated(m_position.m_x, m_position.m_y, 0.0);
    glRotated(m_angle, 0.0, 0.0, 1.0);
    glTranslated(-m_position.m_x, -m_position.m_y, 0.0);
    
    if(m_selected) {
        glColor4d(0.0, 0.5, 1.0, 0.5);
        DrawRectangle(m_position + wxPoint2DDouble(m_borderSize / 2.0, m_borderSize / 2.0), m_rect.m_width,
                      m_rect.m_height);
    }
    
    // Draw text (layer 2)

    glColor4d(0.0, 0.0, 0.0, 1.0);
    wxGLString glString(m_text);
    glString.setFont(wxFont(m_fontSize, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    glString.consolidate(&dc);
    glString.bind();
    glString.render(m_position.m_x, m_position.m_y);
    
    glPopMatrix();
}

bool Text::Intersects(wxRect2DDouble rect) const
{
    if(m_angle == 0.0 || m_angle == 180.0) return m_rect.Intersects(rect);
    return RotatedRectanglesIntersects(m_rect, rect, m_angle, 0.0);
}

void Text::SetText(wxString text)
{
    m_text = text;

    // Generate a glString to get the text size.
    wxGLString glString(m_text);
    glString.setFont(wxFont(m_fontSize, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    wxScreenDC dc;
    glString.consolidate(&dc);
    glString.bind();

    m_width = glString.getWidth();
    m_height = glString.getheight();
}

void Text::Rotate(bool clockwise)
{
    double rotAngle = m_rotationAngle;
    if(!clockwise) rotAngle = -m_rotationAngle;

    m_angle += rotAngle;
    if(m_angle >= 360.0) m_angle = 0.0;
}
