#include "Element.h"

Element::Element() {}
Element::~Element() {}
void Element::SetPosition(const wxPoint2DDouble position)
{
    m_position = position;
    m_rect =
        wxRect2DDouble(m_position.m_x - m_width / 2.0 - m_borderSize, m_position.m_y - m_height / 2.0 - m_borderSize,
                       m_width + 2.0 * m_borderSize, m_height + 2.0 * m_borderSize);
}

wxPoint2DDouble Element::WorldToScreen(wxPoint2DDouble translation, double scale, double offsetX, double offsetY) const
{
    return wxPoint2DDouble(m_position.m_x + offsetX + translation.m_x, m_position.m_y + offsetY + translation.m_y) *
           scale;
}

void Element::DrawCircle(wxPoint2DDouble position, double radius, int numSegments, GLenum mode) const
{
    glBegin(mode);
    for(int i = 0; i < numSegments; i++) {
	    double theta = 2.0 * 3.1415926 * double(i) / double(numSegments);
	    glVertex2f(radius * std::cos(theta) + position.m_x, radius * std::sin(theta) + position.m_y);
	}
    glEnd();
}

void Element::DrawRectangle(wxPoint2DDouble position, double width, double height, GLenum mode) const
{
    glBegin(mode);  // TODO: GL_QUADS é obsoleto (OpenGL 3.0+), encontrar outra solução.
    glVertex2d(position.m_x - width / 2.0, position.m_y - height / 2.0);
    glVertex2d(position.m_x - width / 2.0, position.m_y + height / 2.0);
    glVertex2d(position.m_x + width / 2.0, position.m_y + height / 2.0);
    glVertex2d(position.m_x + width / 2.0, position.m_y - height / 2.0);
    glEnd();
}
