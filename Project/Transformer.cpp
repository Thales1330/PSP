#include "Transformer.h"

Transformer::Transformer() : Element() {}
Transformer::~Transformer() {}
bool Transformer::AddParent(Element* parent, wxPoint2DDouble position)
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

		    // The average between the two bus points.
		    m_position = wxPoint2DDouble((m_pointList[0].m_x + m_pointList[m_pointList.size() - 1].m_x) / 2.0,
		                                 (m_pointList[0].m_y + m_pointList[m_pointList.size() - 1].m_y) / 2.0);

		    m_width = 80.0;
		    m_height = 50.0;
		    m_rect = wxRect2DDouble(m_position.m_x - m_width / 2.0, m_position.m_y - m_height / 2.0, m_width,
		                            m_height);

		    return true;
		}
	}
    return false;
}
bool Transformer::Contains(wxPoint2DDouble position) const {return m_rect.Contains(position);}
void Transformer::Draw(wxPoint2DDouble translation, double scale) const
{
	
}
bool Transformer::Intersects(wxRect2DDouble rect) const { return false;}
void Transformer::Rotate() {}
void Transformer::UpdateSwitchesPosition() {}
