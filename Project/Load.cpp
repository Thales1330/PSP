#include "Load.h"

Load::Load() : Shunt() {}
Load::~Load() {}
bool Load::AddParent(Element* parent, wxPoint2DDouble position)
{
    if(parent) {
	    m_parentList.push_back(parent);
	    wxPoint2DDouble parentPt =
	        parent->RotateAtPosition(position, -parent->GetAngle());        // Rotate click to horizontal position.
	    parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
	    parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.

	    m_position = parentPt + wxPoint2DDouble(0.0, 100.0);  // Shifts the position to the down of the bus.
	    m_width = m_height = 20.0;
	    m_rect = wxRect2DDouble(m_position.m_x - 10.0, m_position.m_y - 10.0, m_width, m_height);

	    m_pointList.push_back(parentPt);
	    m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_position));
	    m_pointList.push_back(m_position + wxPoint2DDouble(0.0, -15.0));
	    m_pointList.push_back(m_position + wxPoint2DDouble(0.0, -10.0));

	    m_triangPts.push_back(wxPoint2DDouble(-m_width / 2.0, -m_height / 2.0));
	    m_triangPts.push_back(wxPoint2DDouble(m_width / 2.0, -m_height / 2.0));
	    m_triangPts.push_back(wxPoint2DDouble(0.0, m_height / 2.0));

	    m_inserted = true;
	    return true;
	}
    return false;
}

void Load::Draw(wxPoint2DDouble translation, double scale) const
{
    if(m_inserted) {
	    // Draw Selection (layer 1).
	    if(m_selected) {
		    glLineWidth(1.5 + m_borderSize * 2.0);
		    glColor4d(0.0, 0.5, 1.0, 0.5);
		    std::vector<wxPoint2DDouble> selTriangPts;
		    selTriangPts.push_back(m_triangPts[0] + m_position +
		                           wxPoint2DDouble(-m_borderSize / scale, -m_borderSize / scale));
		    selTriangPts.push_back(m_triangPts[1] + m_position +
		                           wxPoint2DDouble(m_borderSize / scale, -m_borderSize / scale));
		    selTriangPts.push_back(m_triangPts[2] + m_position + wxPoint2DDouble(0.0, m_borderSize / scale));

		    glPushMatrix();
		    glTranslated(m_position.m_x, m_position.m_y, 0.0);
		    glRotated(m_angle, 0.0, 0.0, 1.0);
		    glTranslated(-m_position.m_x, -m_position.m_y, 0.0);
		    DrawTriangle(selTriangPts);
		    glPopMatrix();

		    DrawLine(m_pointList);

		    // Draw node selection.
		    DrawCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
		}

	    // Draw Load (layer 2).
	    glLineWidth(1.5);

	    // Draw node.
	    glColor4d(0.2, 0.2, 0.2, 1.0);
	    DrawCircle(m_pointList[0], 5.0, 10, GL_POLYGON);

	    DrawLine(m_pointList);

	    std::vector<wxPoint2DDouble> triangPts;
	    for(int i = 0; i < 3; i++) {
		    triangPts.push_back(m_triangPts[i] + m_position);
		}
	    glPushMatrix();
	    glTranslated(m_position.m_x, m_position.m_y, 0.0);
	    glRotated(m_angle, 0.0, 0.0, 1.0);
	    glTranslated(-m_position.m_x, -m_position.m_y, 0.0);
	    DrawTriangle(triangPts);
	    glPopMatrix();
	}
}

void Load::Rotate()
{
    m_angle += m_rotationAngle;
    m_pointList[2] = RotateAtPosition(m_pointList[2], m_rotationAngle);
    m_pointList[3] = RotateAtPosition(m_pointList[3], m_rotationAngle);
    UpdateSwitchesPosition();
}

bool Load::GetContextMenu(wxMenu& menu)
{
	menu.Append(ID_EDIT_LOAD, _("Edit Load"));
    menu.Append(ID_ROTATE, _("Rotate"));
    menu.Append(ID_DELETE, _("Delete"));
    return true;
}
