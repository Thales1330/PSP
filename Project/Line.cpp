#include "Line.h"

Line::Line() {}
Line::~Line() {}
bool Line::Contains(wxPoint2DDouble position) const { return false; }
void Line::Draw(wxPoint2DDouble translation, double scale) const {}
wxCursor Line::GetBestPickboxCursor() const { return wxCURSOR_ARROW; }
bool Line::AddParent(Element* parent, wxPoint2DDouble position)
{
    if(parent) {
	    if(m_parentList.size() == 0) {
		    m_parentList.push_back(parent);
		    m_pointList.push_back(position);  // First point
		    return false;
		}
	    else if(parent != m_parentList[0])
		{
		    m_parentList.push_back(parent);
		    m_pointList.push_back(position);  // Last point
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
