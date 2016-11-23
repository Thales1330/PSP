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

Text::Text() : Element()
{
    SetText(m_text);
}

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

void Text::Draw(wxPoint2DDouble translation, double scale, wxDC& dc)
{
    if(consolidate) {
        glString.setFont(wxFont(10, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        glString.consolidate(&dc);
        //glString.bind();
        consolidate = false;
    }
    /*if(m_selected) {
        glColor4d(0.0, 0.5, 1.0, 0.5);
        wxGLString backSelection(m_text);
        backSelection.setFont(wxFont(10, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        backSelection.consolidate(&dc);
        backSelection.bind();
        backSelection.render(m_position.m_x, m_position.m_y);
    }*/
    
    glColor4d(0.0, 0.0, 0.0, 1.0);
    //glString.render(m_position.m_x, m_position.m_y);
}

bool Text::Intersects(wxRect2DDouble rect) const
{
    if(m_angle == 0.0 || m_angle == 180.0) return m_rect.Intersects(rect);
    return RotatedRectanglesIntersects(m_rect, rect, m_angle, 0.0);
}

void Text::SetText(wxString text)
{
    m_text = text;
    glString = text;
    
    consolidate = true;
}
