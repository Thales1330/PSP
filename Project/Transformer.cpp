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

		    // Get the average between the two bus points.
		    m_position = wxPoint2DDouble((m_pointList[0].m_x + parentPt.m_x) / 2.0,
		                                 (m_pointList[0].m_y + parentPt.m_y) / 2.0);
		    // Set the transformer rectangle.
		    m_width = 70.0;
		    m_height = 40.0;
		    m_rect = wxRect2DDouble(m_position.m_x - m_width / 2.0, m_position.m_y - m_height / 2.0, m_width,
		                            m_height);
		    // Set the "side" points.
		    m_pointList.push_back(wxPoint2DDouble(m_rect.GetPosition() + wxPoint2DDouble(-10, m_height / 2.0)));
		    m_pointList.push_back(
		        wxPoint2DDouble(m_rect.GetPosition() + wxPoint2DDouble(m_width + 10, m_height / 2.0)));

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

bool Transformer::Contains(wxPoint2DDouble position) const
{
    wxPoint2DDouble ptR = RotateAtPosition(position, -m_angle);
    return m_rect.Contains(ptR);
}

void Transformer::Draw(wxPoint2DDouble translation, double scale) const
{
    if(m_inserted) {
	    // Draw selection (layer 1).
	    if(m_selected) {
		    // Push the current matrix on stack.
		    glLineWidth(1.5 + m_borderSize * 2.0);
		    glColor4d(0.0, 0.5, 1.0, 0.5);
		    DrawLine(m_pointList);
		    glPushMatrix();
		    // Rotate the matrix around the object position.
		    glTranslated(m_position.m_x, m_position.m_y, 0.0);
		    glRotated(m_angle, 0.0, 0.0, 1.0);
		    glTranslated(-m_position.m_x, -m_position.m_y, 0.0);

		    // glColor4d(0.0, 0.5, 1.0, 0.5);
		    DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20 + (m_borderSize + 1.5) / scale,
		               20, GL_POLYGON);
		    DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20 + (m_borderSize + 1.5) / scale,
		               20, GL_POLYGON);

		    glPopMatrix();

		    // Draw nodes selection.
		    if(m_pointList.size() > 0) {
			    DrawCircle(m_pointList[0], 5.0 + m_borderSize, 10, GL_POLYGON);
			    if(m_inserted) {
				    DrawCircle(m_pointList[m_pointList.size() - 1], 5.0 + m_borderSize, 10, GL_POLYGON);
				}
			}
		}

	    // Draw transformer (layer 2).
	    // Transformer line
	    glLineWidth(1.5);
	    glColor4d(0.2, 0.2, 0.2, 1.0);
	    DrawLine(m_pointList);

	    // Draw nodes.
	    if(m_pointList.size() > 0) {
		    glColor4d(0.2, 0.2, 0.2, 1.0);
		    DrawCircle(m_pointList[0], 5.0, 10, GL_POLYGON);
		    if(m_inserted) {
			    DrawCircle(m_pointList[m_pointList.size() - 1], 5.0, 10, GL_POLYGON);
			}
		}

	    // Push the current matrix on stack.
	    glPushMatrix();
	    // Rotate the matrix around the object position.
	    glTranslated(m_position.m_x, m_position.m_y, 0.0);
	    glRotated(m_angle, 0.0, 0.0, 1.0);
	    glTranslated(-m_position.m_x, -m_position.m_y, 0.0);

	    glColor4d(1.0, 1.0, 1.0, 1.0);
	    DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20, 20, GL_POLYGON);
	    DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20, 20, GL_POLYGON);

	    glColor4d(0.2, 0.2, 0.2, 1.0);
	    DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20, 20);
	    DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20, 20);

	    DrawPoint(m_rect.GetPosition(), 8.0 * scale);

	    glPopMatrix();
	}
}

bool Transformer::Intersects(wxRect2DDouble rect) const
{
    if(m_angle == 0.0 || m_angle == 180.0) return m_rect.Intersects(rect);
    return RotatedRectanglesIntersects(m_rect, rect, m_angle, 0.0);
}

void Transformer::Rotate()
{
    m_angle += m_rotationAngle;
    if(m_angle >= 360.0) m_angle = 0.0;

    // Rotate all the points, except the switches and buses points.
    for(int i = 2; i < (int)m_pointList.size() - 2; i++) {
	    m_pointList[i] = RotateAtPosition(m_pointList[i], m_rotationAngle);
	}
}

void Transformer::UpdateSwitchesPosition()
{
    if(m_parentList[0]) {
	    m_pointList[1] = GetSwitchPoint(m_parentList[0], m_pointList[0], m_pointList[2]);
	}
    else
	{
	    m_pointList[1] = m_pointList[0];
	}
    if(m_parentList[1]) {
	    m_pointList[m_pointList.size() - 2] = GetSwitchPoint(m_parentList[1], m_pointList[m_pointList.size() - 1],
	                                                         m_pointList[m_pointList.size() - 3]);
	}
    else
	{
	    m_pointList[m_pointList.size() - 2] = m_pointList[m_pointList.size() - 1];
	}
}

void Transformer::Move(wxPoint2DDouble position)
{
    SetPosition(m_movePos + position - m_moveStartPt);

    // Move all the points, except the switches and buses points.
    for(int i = 2; i < (int)m_pointList.size() - 2; i++) {
	    m_pointList[i] = m_movePts[i] + position - m_moveStartPt;
	}

    if(!m_parentList[0]) {
	    m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
	}
    if(!m_parentList[1]) {
	    m_pointList[m_pointList.size() - 1] = m_movePts[m_pointList.size() - 1] + position - m_moveStartPt;
	}

    UpdateSwitchesPosition();
}

void Transformer::MoveNode(Element* parent, wxPoint2DDouble position)
{
    if(parent) {
	    // First bus.
	    if(parent == m_parentList[0]) {
		    m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
		}
	    // Second bus.
	    else if(parent == m_parentList[1])
		{
		    m_pointList[m_pointList.size() - 1] = m_movePts[m_pointList.size() - 1] + position - m_moveStartPt;
		}
	}
    else
	{
	    if(m_activeNodeID == 1) {
		    m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
		    m_parentList[0] = NULL;
		}
	    else if(m_activeNodeID == 2)
		{
		    m_pointList[m_pointList.size() - 1] = m_movePts[m_pointList.size() - 1] + position - m_moveStartPt;
		    m_parentList[1] = NULL;
		}
	}

    // Recalculate switches positions
    UpdateSwitchesPosition();
}

void Transformer::StartMove(wxPoint2DDouble position)
{
    m_moveStartPt = position;
    m_movePts = m_pointList;
    m_movePos = m_position;
}

void Transformer::RemoveParent(Element* parent)
{
    for(int i = 0; i < 2; i++) {
	    if(parent == m_parentList[i]) {
		    m_parentList[i] = NULL;
		    m_parentList[i] = NULL;
		    UpdateSwitchesPosition();
		}
	}
}

bool Transformer::NodeContains(wxPoint2DDouble position)
{
    wxRect2DDouble nodeRect1(m_pointList[0].m_x - 5.0 - m_borderSize, m_pointList[0].m_y - 5.0 - m_borderSize,
                             10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);
    wxRect2DDouble nodeRect2(m_pointList[m_pointList.size() - 1].m_x - 5.0 - m_borderSize,
                             m_pointList[m_pointList.size() - 1].m_y - 5.0 - m_borderSize, 10 + 2.0 * m_borderSize,
                             10 + 2.0 * m_borderSize);

    if(nodeRect1.Contains(position)) {
	    m_activeNodeID = 1;
	    return true;
	}
    if(nodeRect2.Contains(position)) {
	    m_activeNodeID = 2;
	    return true;
	}

    m_activeNodeID = 0;
    return false;
}

bool Transformer::SetNodeParent(Element* parent)
{
    if(m_activeNodeID == 1 && parent == m_parentList[0]) return false;
    if(m_activeNodeID == 2 && parent == m_parentList[1]) return false;

    if(parent && m_activeNodeID != 0) {
	    wxRect2DDouble nodeRect(0, 0, 0, 0);
	    if(m_activeNodeID == 1) {
		    nodeRect =
		        wxRect2DDouble(m_pointList[0].m_x - 5.0 - m_borderSize, m_pointList[0].m_y - 5.0 - m_borderSize,
		                       10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);
		}
	    if(m_activeNodeID == 2) {
		    nodeRect = wxRect2DDouble(m_pointList[m_pointList.size() - 1].m_x - 5.0 - m_borderSize,
		                              m_pointList[m_pointList.size() - 1].m_y - 5.0 - m_borderSize,
		                              10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);
		}

	    if(parent->Intersects(nodeRect)) {
		    if(m_activeNodeID == 1) {
			    // Check if the user is trying to connect the same bus.
			    if(m_parentList[1] == parent) {
				    m_activeNodeID = 0;
				    return false;
				}

			    m_parentList[0] = parent;

			    // Centralize the node on bus.
			    wxPoint2DDouble parentPt = parent->RotateAtPosition(
			        m_pointList[0], -parent->GetAngle());  // Rotate click to horizontal position.
			    parentPt.m_y = parent->GetPosition().m_y;  // Centralize on bus.
			    parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());
			    m_pointList[0] = parentPt;

			    UpdateSwitchesPosition();
			    return true;
			}
		    if(m_activeNodeID == 2) {
			    if(m_parentList[0] == parent) {
				    m_activeNodeID = 0;
				    return false;
				}

			    m_parentList[1] = parent;

			    wxPoint2DDouble parentPt =
			        parent->RotateAtPosition(m_pointList[m_pointList.size() - 1], -parent->GetAngle());
			    parentPt.m_y = parent->GetPosition().m_y;
			    parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());
			    m_pointList[m_pointList.size() - 1] = parentPt;

			    UpdateSwitchesPosition();
			    return true;
			}
		}
	    else
		{
		    if(m_activeNodeID == 1) m_parentList[0] = NULL;
		    if(m_activeNodeID == 2) m_parentList[1] = NULL;
		}
	}
    return false;
}

void Transformer::UpdateNodes()
{
    if(m_parentList[0]) {
	    wxRect2DDouble nodeRect(m_pointList[0].m_x - 5.0 - m_borderSize, m_pointList[0].m_y - 5.0 - m_borderSize,
	                            10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);

	    if(!m_parentList[0]->Intersects(nodeRect)) {
		    m_parentList[0] = NULL;
		    UpdateSwitchesPosition();
		}
	}
    if(m_parentList[1]) {
	    wxRect2DDouble nodeRect = wxRect2DDouble(m_pointList[m_pointList.size() - 1].m_x - 5.0 - m_borderSize,
	                                             m_pointList[m_pointList.size() - 1].m_y - 5.0 - m_borderSize,
	                                             10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);

	    if(!m_parentList[1]->Intersects(nodeRect)) {
		    m_parentList[1] = NULL;
		    UpdateSwitchesPosition();
		}
	}
}

void Transformer::RotateNode(Element* parent)
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

bool Transformer::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_TRANSFORMER, _("Edit tranformer"));
    menu.Append(ID_DELETE, _("Delete"));
    return true;
}
