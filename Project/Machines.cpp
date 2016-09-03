#include "Machines.h"

Machines::Machines() : Element()
{
}

Machines::~Machines()
{
}

bool Machines::AddParent(Element* parent, wxPoint2DDouble position)
{
    if(parent) {
	    m_parentList.push_back(parent);
	    wxPoint2DDouble parentPt =
	        parent->RotateAtPosition(position, -parent->GetAngle());        // Rotate click to horizontal position.
	    parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
	    parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.

	    m_position = parentPt + wxPoint2DDouble(-100.0, 0.0);  // Shifts the position to the left of the bus.
	    m_width = m_height = 50.0;
	    m_rect = wxRect2DDouble(m_position.m_x - 25.0, m_position.m_y - 25.0, m_width, m_height);

	    m_pointList.push_back(parentPt);
	    m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_position));
	    m_pointList.push_back(m_position + wxPoint2DDouble(35.0, 0.0));
	    m_pointList.push_back(m_position + wxPoint2DDouble(25.0, 0.0));
	    m_inserted = true;
	    return true;
	}
    return false;
}

void Machines::Draw(wxPoint2DDouble translation, double scale) const
{
    if(m_inserted) {
	    // Draw Selection (layer 1).
	    if(m_selected) {
		    glLineWidth(1.5 + m_borderSize * 2.0);
		    glColor4d(0.0, 0.5, 1.0, 0.5);
		    DrawCircle(m_position, 25.0 + (m_borderSize + 1.5) / scale, 20, GL_POLYGON);
		    DrawLine(m_pointList);

		    // Draw node selection.
		    DrawCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
		}

	    // Draw Machines (layer 2).
	    glLineWidth(1.5);

	    // Draw node.
	    glColor4d(0.2, 0.2, 0.2, 1.0);
	    DrawCircle(m_pointList[0], 5.0, 10, GL_POLYGON);

	    DrawLine(m_pointList);

	    glColor4d(1.0, 1.0, 1.0, 1.0);
	    DrawCircle(m_position, 25.0, 20, GL_POLYGON);

	    glColor4d(0.2, 0.2, 0.2, 1.0);
	    DrawCircle(m_position, 25.0, 20);

	    // Draw machine symbol.
	    DrawSymbol();
	}
}

void Machines::UpdateSwitchesPosition()
{
    if(m_parentList[0]) {
	    m_pointList[1] = GetSwitchPoint(m_parentList[0], m_pointList[0], m_pointList[2]);
	}
    else
	{
	    m_pointList[1] = m_pointList[0];
	}
}

void Machines::Move(wxPoint2DDouble position)
{
    SetPosition(m_movePos + position - m_moveStartPt);
    for(int i = 2; i < (int)m_pointList.size(); i++) {
	    m_pointList[i] = m_movePts[i] + position - m_moveStartPt;
	}
	if(!m_parentList[0]) {
		 m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
	}
    UpdateSwitchesPosition();
}

void Machines::MoveNode(Element* element, wxPoint2DDouble position)
{
    if(element) {
	    if(element == m_parentList[0]) {
		    m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
		}
	}
    else
	{
	    if(m_activeNodeID == 1) {
		    m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
		    m_parentList[0] = NULL;
		}
	}

    // Recalculate switches positions
    UpdateSwitchesPosition();
}

void Machines::StartMove(wxPoint2DDouble position)
{
    m_moveStartPt = position;
    m_movePts = m_pointList;
    m_movePos = m_position;
}

void Machines::RotateNode(Element* parent)
{
    if(parent == m_parentList[0]) {
	    m_pointList[0] = parent->RotateAtPosition(m_pointList[0], m_rotationAngle);
	    UpdateSwitchesPosition();
	}
}

void Machines::RemoveParent(Element* parent)
{
    if(parent == m_parentList[0]) {
	    m_parentList[0] = NULL;
	    UpdateSwitchesPosition();
	}
}

bool Machines::NodeContains(wxPoint2DDouble position)
{
    wxRect2DDouble nodeRect(m_pointList[0].m_x - 5.0 - m_borderSize, m_pointList[0].m_y - 5.0 - m_borderSize,
                            10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);

    if(nodeRect.Contains(position)) {
	    m_activeNodeID = 1;
	    return true;
	}

    m_activeNodeID = 0;
    return false;
}

bool Machines::SetNodeParent(Element* parent)
{
    if(parent && m_activeNodeID != 0) {
	    wxRect2DDouble nodeRect(m_pointList[0].m_x - 5.0 - m_borderSize, m_pointList[0].m_y - 5.0 - m_borderSize,
	                            10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);

	    if(parent->Intersects(nodeRect)) {
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
	    else
		{
		    m_parentList[0] = NULL;
		}
	}
    return false;
}

void Machines::UpdateNodes()
{
    if(m_parentList[0]) {
	    wxRect2DDouble nodeRect(m_pointList[0].m_x - 5.0 - m_borderSize, m_pointList[0].m_y - 5.0 - m_borderSize,
	                            10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);

	    if(!m_parentList[0]->Intersects(nodeRect)) {
		    m_parentList[0] = NULL;
		    UpdateSwitchesPosition();
		}
	}
}

void Machines::Rotate()
{
    m_pointList[2] = RotateAtPosition(m_pointList[2], m_rotationAngle);
    m_pointList[3] = RotateAtPosition(m_pointList[3], m_rotationAngle);
    UpdateSwitchesPosition();
}
