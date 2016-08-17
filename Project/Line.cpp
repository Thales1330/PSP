#include "Line.h"

Line::Line() {}
Line::~Line() {}
bool Line::Contains(wxPoint2DDouble position) const { return false; }
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
}

wxCursor Line::GetBestPickboxCursor() const { return wxCURSOR_ARROW; }
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
bool Line::Intersects(wxRect2DDouble rect) const { return false; }
void Line::MovePickbox(wxPoint2DDouble position) {}
bool Line::PickboxContains(wxPoint2DDouble position) { return false; }
void Line::Rotate() {}
void Line::AddPoint(wxPoint2DDouble point)
{
    if(m_parentList.size() != 0) {
	    m_pointList.push_back(point);
	}
}

void Line::MoveNode(Element* parent, wxPoint2DDouble position)
{
    // First bus.
    if(parent == m_parentList[0]) {
	    m_pointList[0] = parent->GetMovePosition() + position - parent->GetMoveStartPosition();
	}
    // Second bus.
    else if(parent == m_parentList[1])
	{
	    m_pointList[m_pointList.size() - 1] = parent->GetMovePosition() + position - parent->GetMoveStartPosition();
	}
    // Recalculate switches positions
    m_pointList[1] = GetSwitchPoint(m_parentList[0], m_pointList[0], m_pointList[2]);
    m_pointList[m_pointList.size() - 2] =
        GetSwitchPoint(m_parentList[1], m_pointList[m_pointList.size() - 1], m_pointList[m_pointList.size() - 3]);
}
