#include "Bus.h"

Bus::Bus(wxPoint2DDouble position) : Element()
{
    m_width = 100.0;
    m_height = 5.0;
    SetPosition(position);
}
Bus::~Bus() {}
void Bus::Draw(wxPoint2DDouble translation, double scale) const
{
    // Draw selection (behind)
    if(m_selected) {
	    // If the object is selected, the matrix is reset to remove scale effects applied to it, thus keeping the
	    // edges with fixed sizes for all zoom levels.
	    glPushMatrix();
	    glLoadIdentity();
	    // The matrix was reset, so we must use screen coordinates (WorldToScreen).
	    wxPoint2DDouble screenPt = WorldToScreen(translation, scale);
	    glTranslated(screenPt.m_x, screenPt.m_y, 0.0);
	    glRotated(m_angle, 0.0, 0.0, 1.0);
	    glTranslated(-screenPt.m_x, -screenPt.m_y, 0.0);

	    glColor4d(0.0, 0.5, 1.0, 0.5);

	    wxPoint2DDouble pts[4] = {WorldToScreen(translation, scale, -(m_width / 2.0), -(m_height / 2.0)) -
	                                  wxPoint2DDouble(m_borderSize, m_borderSize),
	                              WorldToScreen(translation, scale, -(m_width / 2.0), (m_height / 2.0)) -
	                                  wxPoint2DDouble(m_borderSize, -m_borderSize),
	                              WorldToScreen(translation, scale, (m_width / 2.0), (m_height / 2.0)) -
	                                  wxPoint2DDouble(-m_borderSize, -m_borderSize),
	                              WorldToScreen(translation, scale, (m_width / 2.0), -(m_height / 2.0)) -
	                                  wxPoint2DDouble(-m_borderSize, m_borderSize)};
	    DrawRectangle(pts);
	    glPopMatrix();
	}
    // Draw element (middle)
    // Push the current matrix on stack.
    glPushMatrix();
    // Rotate the matrix around the object position.
    glTranslated(m_position.m_x, m_position.m_y, 0.0);
    glRotated(m_angle, 0.0, 0.0, 1.0);
    glTranslated(-m_position.m_x, -m_position.m_y, 0.0);

    glColor4d(0.0, 0.3, 1.0, 1.0);
    DrawRectangle(m_position, m_width, m_height);
    // Pop the old matrix back.
    glPopMatrix();

    // Draw pickbox (above)
    if(m_showPickbox) {
	    glPushMatrix();
	    glLoadIdentity();

	    wxPoint2DDouble screenPt = WorldToScreen(translation, scale);
	    glTranslated(screenPt.m_x, screenPt.m_y, 0.0);
	    glRotated(m_angle, 0.0, 0.0, 1.0);
	    glTranslated(-screenPt.m_x, -screenPt.m_y, 0.0);

	    wxPoint2DDouble pbPosition[2] = {WorldToScreen(translation, scale, m_width / 2.0),
	                                     WorldToScreen(translation, scale, -m_width / 2.0)};
	    DrawPickbox(pbPosition[0]);
	    DrawPickbox(pbPosition[1]);

	    glPopMatrix();
	}
}
bool Bus::Contains(wxPoint2DDouble position) const
{
    wxPoint2DDouble ptR = RotateAtPosition(position, -m_angle);
    return m_rect.Contains(ptR);
}
int Bus::PickboxContains(wxPoint2DDouble position) const { return 0; }
wxCursor Bus::GetBestPickboxCursor() const
{
    double angle = m_angle;
    while(angle >= 157.5) angle -= 180.0;

    if(angle >= -22.5 && angle < 22.5)
	return wxCursor(wxCURSOR_SIZEWE);
    else if(angle >= 22.5 && angle < 67.5)
	return wxCursor(wxCURSOR_SIZENWSE);
    else if(angle >= 67.5 && angle < 112.5)
	return wxCursor(wxCURSOR_SIZENS);
    else if(angle >= 112.5 && angle < 157.5)
	return wxCursor(wxCURSOR_SIZENESW);

    return wxCursor(wxCURSOR_ARROW);
}
void Bus::MovePickbox(wxPoint2DDouble position, int pickboxID) {}
void Bus::Rotate()
{
    m_angle += 45.0;
    if(m_angle >= 360.0) m_angle = 0.0;
}
