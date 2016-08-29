#include "Bus.h"

Bus::Bus() : Element() {}
Bus::Bus(wxPoint2DDouble position) : Element()
{
    m_width = 100.0;
    m_height = 5.0;
    SetPosition(position);
}

Bus::~Bus() {}
void Bus::Draw(wxPoint2DDouble translation, double scale) const
{
    // Draw selection (layer 1)
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
    // Draw element (layer 2)
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

    // Draw pickbox (layer 3)
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

bool Bus::Intersects(wxRect2DDouble rect) const
{
    if(m_angle == 0.0 || m_angle == 180.0) return m_rect.Intersects(rect);

    wxPoint2DDouble m_rectCorners[4] = {m_rect.GetLeftTop(), m_rect.GetLeftBottom(), m_rect.GetRightBottom(),
                                        m_rect.GetRightTop()};
    wxPoint2DDouble rectCorners[4] = {rect.GetLeftTop(), rect.GetLeftBottom(), rect.GetRightBottom(),
                                      rect.GetRightTop()};

    // Rotate the m_rect corners
    for(int i = 0; i < 4; i++) {
	    m_rectCorners[i] = RotateAtPosition(m_rectCorners[i], m_angle);
	}

    //[Ref] http://www.gamedev.net/page/resources/_/technical/game-programming/2d-rotated-rectangle-collision-r2604

    // Find the rectangles axis to project
    wxPoint2DDouble axis[4] = {m_rectCorners[3] - m_rectCorners[0], m_rectCorners[3] - m_rectCorners[2],
                               rectCorners[3] - rectCorners[0], rectCorners[3] - rectCorners[2]};

    // Calculate the projected points to each axis
    wxPoint2DDouble m_rectProjPts[4][4];  // [axis][corner]
    wxPoint2DDouble rectProjPts[4][4];    // [axis][corner]
    for(int i = 0; i < 4; i++) {
	    double den = axis[i].m_x * axis[i].m_x + axis[i].m_y * axis[i].m_y;
	    for(int j = 0; j < 4; j++) {
		    double m_rectProj = (m_rectCorners[j].m_x * axis[i].m_x + m_rectCorners[j].m_y * axis[i].m_y) / den;
		    double rectProj = (rectCorners[j].m_x * axis[i].m_x + rectCorners[j].m_y * axis[i].m_y) / den;

		    m_rectProjPts[i][j] = wxPoint2DDouble(m_rectProj * axis[i].m_x, m_rectProj * axis[i].m_y);
		    rectProjPts[i][j] = wxPoint2DDouble(rectProj * axis[i].m_x, rectProj * axis[i].m_y);
		}
	}

    // Calculate the scalar value to identify the max and min values on projections
    double m_rectScalar[4][4];  //[axis][corner]
    double rectScalar[4][4];    //[axis][corner]
    for(int i = 0; i < 4; i++) {
	    for(int j = 0; j < 4; j++) {
		    m_rectScalar[i][j] = m_rectProjPts[i][j].m_x * axis[i].m_x + m_rectProjPts[i][j].m_y * axis[i].m_y;
		    rectScalar[i][j] = rectProjPts[i][j].m_x * axis[i].m_x + rectProjPts[i][j].m_y * axis[i].m_y;
		}
	}
    // Identify the max and min values
    double m_rectMin[4];
    double m_rectMax[4];
    double rectMin[4];
    double rectMax[4];

    for(int i = 0; i < 4; i++) {
	    m_rectMax[i] = m_rectScalar[i][0];
	    rectMax[i] = rectScalar[i][0];
	    m_rectMin[i] = m_rectScalar[i][0];
	    rectMin[i] = rectScalar[i][0];

	    for(int j = 1; j < 4; j++) {
		    if(m_rectMax[i] < m_rectScalar[i][j]) m_rectMax[i] = m_rectScalar[i][j];
		    if(rectMax[i] < rectScalar[i][j]) rectMax[i] = rectScalar[i][j];

		    if(m_rectMin[i] > m_rectScalar[i][j]) m_rectMin[i] = m_rectScalar[i][j];
		    if(rectMin[i] > rectScalar[i][j]) rectMin[i] = rectScalar[i][j];
		}
	}

    // Check if any segment don't overlap
    for(int i = 0; i < 4; i++) {
	    if(!(rectMin[i] <= m_rectMax[i] && rectMax[i] >= m_rectMin[i])) return false;
	}

    return true;
    // return rect.Intersects(m_rect);
}
bool Bus::PickboxContains(wxPoint2DDouble position)
{
    m_activePickboxID = ID_PB_NONE;

    wxPoint2DDouble ptR = RotateAtPosition(position, -m_angle);

    wxPoint2DDouble center(m_position.m_x + m_width / 2.0, m_position.m_y);
    wxRect2DDouble rectRight(center.m_x - 5.0, center.m_y - 5.0, 10.0, 10.0);

    center = wxPoint2DDouble(m_position.m_x - m_width / 2.0, m_position.m_y);
    wxRect2DDouble rectLeft(center.m_x - 5.0, center.m_y - 5.0, 10.0, 10.0);

    if(rectRight.Contains(ptR)) {
	    m_activePickboxID = ID_PB_RIGHT;
	    return true;
	}
    if(rectLeft.Contains(ptR)) {
	    m_activePickboxID = ID_PB_LEFT;
	    return true;
	}

    return false;
}

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

void Bus::MovePickbox(wxPoint2DDouble position)
{
    if(m_activePickboxID == ID_PB_NONE) return;

    wxPoint2DDouble ptR = RotateAtPosition(position, -m_angle);

    double dx = 0.0;
    if(m_activePickboxID == ID_PB_RIGHT)
	dx = ptR.m_x - m_position.m_x - m_width / 2.0;
    else if(m_activePickboxID == ID_PB_LEFT)
	dx = m_position.m_x - m_width / 2.0 - ptR.m_x;

    if(m_width + dx < 20.0) return;

    if(m_activePickboxID == ID_PB_RIGHT) {
	    m_position.m_x += (dx / 2.0) * std::cos(wxDegToRad(m_angle));
	    m_position.m_y += (dx / 2.0) * std::sin(wxDegToRad(m_angle));
	}
    else if(m_activePickboxID == ID_PB_LEFT)
	{
	    m_position.m_x -= (dx / 2.0) * std::cos(wxDegToRad(m_angle));
	    m_position.m_y -= (dx / 2.0) * std::sin(wxDegToRad(m_angle));
	}
    m_width += dx;

    SetPosition(m_position);
}

void Bus::Rotate()
{
    m_angle += m_rotationAngle;
    if(m_angle >= 360.0) m_angle = 0.0;
}

bool Bus::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_BUS, _("Edit bus"));
    menu.Append(ID_ROTATE, _("Rotate"));
	menu.Append(ID_DELETE, _("Delete bus"));
    return true;
}
