#include "Capacitor.h"

Capacitor::Capacitor() : Shunt()
{
}

Capacitor::~Capacitor()
{
}

bool Capacitor::AddParent(Element* parent, wxPoint2DDouble position)
{
	if(parent) {
	    m_parentList.push_back(parent);
	    wxPoint2DDouble parentPt =
	        parent->RotateAtPosition(position, -parent->GetAngle());        // Rotate click to horizontal position.
	    parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
	    parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.

	    m_position = parentPt + wxPoint2DDouble(0.0, 100.0);  // Shifts the position to the down of the bus.
	    m_width = 40;
		m_height = 30;
	    m_rect = wxRect2DDouble(m_position.m_x - m_width / 2.0, m_position.m_y - m_height / 2.0, m_width, m_height);

	    m_pointList.push_back(parentPt);
	    m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_position));
	    m_pointList.push_back(m_position + wxPoint2DDouble(0.0, -m_height / 2.0 - 10.0));
	    m_pointList.push_back(m_position + wxPoint2DDouble(0.0, -m_height / 2.0));

	    m_inserted = true;
		
		wxRect2DDouble genRect(0,0,0,0);
	    m_switchRect.push_back(genRect);  // Push a general rectangle.
		UpdateSwitches();		
		
	    return true;
	}
    return false;
}

void Capacitor::Draw(wxPoint2DDouble translation, double scale) const
{
	if(m_inserted) {		
		std::vector<wxPoint2DDouble> capPts;
		capPts.push_back(wxPoint2DDouble(m_position.m_x - m_width / 2.0, m_position.m_y - m_height / 2.0));
		capPts.push_back(wxPoint2DDouble(m_position.m_x + m_width / 2.0, m_position.m_y - m_height / 2.0));
		capPts.push_back(wxPoint2DDouble(m_position.m_x - m_width / 2.0, m_position.m_y - m_height / 2.0 + 10.0));
		capPts.push_back(wxPoint2DDouble(m_position.m_x + m_width / 2.0, m_position.m_y - m_height / 2.0 + 10.0));
		
	    if(m_selected) {
			glLineWidth(1.5 + m_borderSize * 2.0);
		    glColor4d(0.0, 0.5, 1.0, 0.5);
			
			DrawLine(m_pointList);

			glPushMatrix();
			glTranslated(m_position.m_x, m_position.m_y, 0.0);
			glRotated(m_angle, 0.0, 0.0, 1.0);
			glTranslated(-m_position.m_x, -m_position.m_y, 0.0);
			
			DrawLine(capPts, GL_LINES);
			
			DrawGround(m_position + wxPoint2DDouble(0, -m_height / 2.0 + 10.0));

			glPopMatrix();
			
			// Draw node selection.
		    DrawCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
		}
	    // Draw Capacitor (layer 2).
	    glLineWidth(1.5);
	    glColor4d(0.2, 0.2, 0.2, 1.0);
	    DrawCircle(m_pointList[0], 5.0, 10, GL_POLYGON);
	    DrawLine(m_pointList);
		
		DrawSwitches();

	    glPushMatrix();
	    glTranslated(m_position.m_x, m_position.m_y, 0.0);
	    glRotated(m_angle, 0.0, 0.0, 1.0);
	    glTranslated(-m_position.m_x, -m_position.m_y, 0.0);
		
		glColor4d(0.2, 0.2, 0.2, 1.0);
	    DrawLine(capPts, GL_LINES);
		
		DrawGround(m_position + wxPoint2DDouble(0, -m_height / 2.0 + 10.0));

	    glPopMatrix();
	}
}

void Capacitor::Rotate(bool clockwise)
{
	double rotAngle = m_rotationAngle;
	if(!clockwise) rotAngle = -m_rotationAngle;
	
	m_angle += rotAngle;
    m_pointList[2] = RotateAtPosition(m_pointList[2], rotAngle);
    m_pointList[3] = RotateAtPosition(m_pointList[3], rotAngle);
    UpdateSwitchesPosition();
}

bool Capacitor::GetContextMenu(wxMenu& menu)
{
	menu.Append(ID_EDIT_CAPACITOR, _("Edit Capacitor"));
    GeneralMenuItens(menu);
    return true;
}

bool Capacitor::Contains(wxPoint2DDouble position) const
{
	wxPoint2DDouble ptR = RotateAtPosition(position, -m_angle);
	return m_rect.Contains(ptR);
}

bool Capacitor::Intersects(wxRect2DDouble rect) const
{
	return RotatedRectanglesIntersects(m_rect, rect, m_angle, 0.0);
}
