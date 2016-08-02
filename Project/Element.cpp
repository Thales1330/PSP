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

void Element::DrawRectangle(wxPoint2DDouble* points, GLenum mode) const
{
    glBegin(mode);  // TODO: GL_QUADS é obsoleto (OpenGL 3.0+), encontrar outra solução.
    glVertex2d(points[0].m_x, points[0].m_y);
    glVertex2d(points[1].m_x, points[1].m_y);
    glVertex2d(points[2].m_x, points[2].m_y);
    glVertex2d(points[3].m_x, points[3].m_y);
    glEnd();
}

void Element::DrawPickbox(wxPoint2DDouble position) const
{
    glColor4d(1.0, 1.0, 1.0, 0.8);
    DrawRectangle(position, 8.0, 8.0);
    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawRectangle(position, 8.0, 8.0, GL_LINE_LOOP);
}

wxPoint2DDouble Element::RotateAtPosition(wxPoint2DDouble pointToRotate, double angle, bool degrees) const
{
    double radAngle = angle;
    if(degrees) radAngle = wxDegToRad(angle);
    return wxPoint2DDouble(std::cos(radAngle) * (pointToRotate.m_x - m_position.m_x) -
                               std::sin(radAngle) * (pointToRotate.m_y - m_position.m_y) + m_position.m_x,
                           std::sin(radAngle) * (pointToRotate.m_x - m_position.m_x) +
                               std::cos(radAngle) * (pointToRotate.m_y - m_position.m_y) + m_position.m_y);
}
