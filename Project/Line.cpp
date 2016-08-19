#include "Line.h"

Line::Line() {}
Line::~Line() {}
bool Line::Contains(wxPoint2DDouble position) const
{
    if(PointToLineDistance(position) < 5.0) {
	    return true;
	}
    return false;
}

void Line::Draw(wxPoint2DDouble translation, double scale) const
{
    std::vector<wxPoint2DDouble> pointList = m_pointList;
    if(!m_inserted && pointList.size() > 0) {
	    wxPoint2DDouble secondPoint = m_position;
	    if(pointList.size() > 2) {
		    secondPoint = pointList[2];
		}
	    pointList[1] = GetSwitchPoint(m_parentList[0], pointList[0], secondPoint);
	    pointList.push_back(m_position);
	}

    // Line selected (Layer 1).
    if(m_selected) {
	    glLineWidth(1.5 + m_borderSize * 2.0);
	    glColor4d(0.0, 0.5, 1.0, 0.5);
	    DrawLine(pointList);

	    // Draw nodes selection.
	    if(pointList.size() > 0) {
		    DrawCircle(pointList[0], 5.0 + m_borderSize, 10, GL_POLYGON);
		    if(m_inserted) {
			    DrawCircle(pointList[pointList.size() - 1], 5.0 + +m_borderSize, 10, GL_POLYGON);
			}
		}
	}

    // Draw line (Layer 2)
    glLineWidth(1.5);
    glColor4d(0.2, 0.2, 0.2, 1.0);
    DrawLine(pointList);

    // Draw nodes.
    if(pointList.size() > 0) {
	    glColor4d(0.2, 0.2, 0.2, 1.0);
	    DrawCircle(pointList[0], 5.0, 10, GL_POLYGON);
	    if(m_inserted) {
		    DrawCircle(pointList[pointList.size() - 1], 5.0, 10, GL_POLYGON);
		}
	}

    // Draw pickboxes (Layer 3).
    if(m_showPickbox) {
	    glPushMatrix();
	    glLoadIdentity();

	    for(int i = 2; i < (int)m_pointList.size() - 2; i++) {
		    DrawPickbox(WorldToScreen(m_pointList[i], translation, scale));
		}

	    glPopMatrix();
	}
}

wxCursor Line::GetBestPickboxCursor() const { return wxCURSOR_SIZING; }
bool Line::AddParent(Element* parent, wxPoint2DDouble position)
{
    if(parent) {
	    // First bus.
	    if(m_parentList.size() == 0) {
		    m_position = position;
		    m_parentList.push_back(parent);
		    wxPoint2DDouble parentPt = parent->RotateAtPosition(
		        position, -parent->GetAngle());        // Rotate click to horizontal position.
		    parentPt.m_y = parent->GetPosition().m_y;  // Centralize on bus.
		    parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.
		    m_pointList.push_back(parentPt);                                    // First point
		    m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_position));
		    return false;
		}
	    // Second bus.
	    else if(parent != m_parentList[0])
		{
		    m_parentList.push_back(parent);
		    wxPoint2DDouble parentPt = parent->RotateAtPosition(
		        position, -parent->GetAngle());        // Rotate click to horizontal position.
		    parentPt.m_y = parent->GetPosition().m_y;  // Centralize on bus.
		    parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.

		    // Set first switch point.
		    wxPoint2DDouble secondPoint = parentPt;
		    if(m_pointList.size() > 2) {
			    secondPoint = m_pointList[2];
			}
		    m_pointList[1] = GetSwitchPoint(m_parentList[0], m_pointList[0], secondPoint);

		    // Set the second switch point.
		    m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_pointList[m_pointList.size() - 1]));

		    m_pointList.push_back(parentPt);  // Last point.
		    m_inserted = true;
		    return true;
		}
	}
    return false;
}
bool Line::Intersects(wxRect2DDouble rect) const
{
    for(auto it = m_pointList.begin(); it != m_pointList.end(); ++it) {
	    if(rect.Contains(*it)) return true;
	}
    return false;
}
void Line::MovePickbox(wxPoint2DDouble position)
{
    if(m_activePickboxID == ID_PB_NONE) return;

    for(int i = 2; i < (int)m_pointList.size() - 2; i++) {
	    if(m_activePickboxID == i) {
		    m_pointList[i] = m_movePts[i] + position - m_moveStartPt;
			UpdateSwitchesPosition();
		}
	}
}
bool Line::PickboxContains(wxPoint2DDouble position)
{
    for(int i = 2; i < (int)m_pointList.size() - 2; i++) {
	    wxRect2DDouble rect(m_pointList[i].m_x - 5.0, m_pointList[i].m_y - 5.0, 10.0, 10.0);
	    if(rect.Contains(position)) {
		    m_activePickboxID = i;
		    return true;
		}
	}
    return false;
}
void Line::RotateNode(Element* parent)
{
    if(parent == m_parentList[0]) {
	    m_pointList[0] = parent->RotateAtPosition(m_pointList[0], m_rotationAngle);
	}
    else if(parent == m_parentList[1])
	{
	    m_pointList[m_pointList.size() - 1] =
	        parent->RotateAtPosition(m_pointList[m_pointList.size() - 1], m_rotationAngle);
	}
    UpdateSwitchesPosition();
}

void Line::AddPoint(wxPoint2DDouble point)
{
    if(m_parentList.size() != 0) {
	    m_pointList.push_back(point);
	}
}

void Line::StartMove(wxPoint2DDouble position)
{
    m_moveStartPt = position;
    m_movePts = m_pointList;
}

void Line::MoveNode(Element* parent, wxPoint2DDouble position)
{
    // First bus.
    if(parent == m_parentList[0]) {
	    m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
	}
    // Second bus.
    else if(parent == m_parentList[1])
	{
	    m_pointList[m_pointList.size() - 1] = m_movePts[m_pointList.size() - 1] + position - m_moveStartPt;
	}

    // If the line is selected, move all the points, except the switches and buses points.
    if(m_selected) {
	    for(int i = 2; i < (int)m_pointList.size() - 2; i++) {
		    m_pointList[i] = m_movePts[i] + position - m_moveStartPt;
		}
	}

    // Recalculate switches positions
    UpdateSwitchesPosition();
}

void Line::UpdateSwitchesPosition()
{
    m_pointList[1] = GetSwitchPoint(m_parentList[0], m_pointList[0], m_pointList[2]);
    m_pointList[m_pointList.size() - 2] =
        GetSwitchPoint(m_parentList[1], m_pointList[m_pointList.size() - 1], m_pointList[m_pointList.size() - 3]);
}

double Line::PointToLineDistance(wxPoint2DDouble point) const
{
    //[Ref] http://geomalgorithms.com/a02-_lines.html
    double distance = 100.0;  // Big initial distance.
    wxPoint2DDouble p0 = point;

    for(int i = 0; i < (int)m_pointList.size() - 1; i++) {
	    double d = 0.0;

	    wxPoint2DDouble p1 = m_pointList[i];
	    wxPoint2DDouble p2 = m_pointList[i + 1];

	    wxPoint2DDouble v = p2 - p1;
	    wxPoint2DDouble w = p0 - p1;

	    double c1 = w.m_x * v.m_x + w.m_y * v.m_y;
	    double c2 = v.m_x * v.m_x + v.m_y * v.m_y;

	    if(c1 <= 0.0) {
		    d = std::sqrt(std::pow(p0.m_y - p1.m_y, 2) + std::pow(p0.m_x - p1.m_x, 2));
		}
	    else if(c2 <= c1)
		{
		    d = std::sqrt(std::pow(p0.m_y - p2.m_y, 2) + std::pow(p0.m_x - p2.m_x, 2));
		}
	    else
		{
		    d = std::abs((p2.m_y - p1.m_y) * p0.m_x - (p2.m_x - p1.m_x) * p0.m_y + p2.m_x * p1.m_y -
		                 p2.m_y * p1.m_x) /
		        std::sqrt(std::pow(p2.m_y - p1.m_y, 2) + std::pow(p2.m_x - p1.m_x, 2));
		}
	    if(d < distance) distance = d;
	}

    return distance;
}

bool Line::GetContextMenu(wxMenu& menu)
{
	menu.Append(ID_EDIT_LINE, _("Edit line"));
	return true;
}
