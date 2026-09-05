/*
 *  Copyright (C) 2017  Thales Lima Oliveira <thales@ufu.br>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "Transformer.h"
#include "../../forms/TransformerForm.h"

Transformer::Transformer() : Branch()
{
	m_elementType = TYPE_TRANSFORMER;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) { m_electricalData.faultCurrent[i][j] = std::complex<double>(0.0, 0.0); }
	}
}
Transformer::Transformer(wxString name) : Branch()
{
	m_elementType = TYPE_TRANSFORMER;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) { m_electricalData.faultCurrent[i][j] = std::complex<double>(0.0, 0.0); }
	}
	m_electricalData.name = name;
}
Transformer::~Transformer() {}

bool Transformer::AddParent(Element* parent, wxPoint2DDouble position)
{
	if (parent) {
		// First bus.
		if (m_parentList.size() == 0) {
			m_position = position;
			m_parentList.push_back(parent);
			parent->AddChild(this);
			wxPoint2DDouble parentPt =
				parent->RotateAtPosition(position, -parent->GetAngle());        // Rotate click to horizontal position.
			parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
			parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.
			m_pointList.push_back(parentPt);                                    // First point
			m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_position));

			wxRect2DDouble genRect(0, 0, 0, 0);
			m_switchRect.push_back(genRect);

			return false;
		}
		// Second bus.
		else if (parent != m_parentList[0]) {
			m_parentList.push_back(parent);
			parent->AddChild(this);
			wxPoint2DDouble parentPt =
				parent->RotateAtPosition(position, -parent->GetAngle());        // Rotate click to horizontal position.
			parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
			parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.

			wxPoint2DDouble p1 = m_pointList[0];
			wxPoint2DDouble p2 = parentPt;
			double dx = p2.m_x - p1.m_x;
			double dy = p2.m_y - p1.m_y;

			// Determine orientation based on bus separation
			if (std::abs(dy) > std::abs(dx)) {
				m_angle = (dy >= 0.0) ? 90.0 : 270.0;
			}
			else {
				m_angle = (dx >= 0.0) ? 0.0 : 180.0;
			}

			// Get the midpoint between the two bus points.
			m_position = wxPoint2DDouble((p1.m_x + p2.m_x) / 2.0, (p1.m_y + p2.m_y) / 2.0);

			// Snap position to grid alignment
			if (std::abs(dx) < 1.0) {
				m_position.m_x = p1.m_x;
				m_position.m_y = std::round(m_position.m_y / 20.0) * 20.0;
			}
			else if (std::abs(dy) < 1.0) {
				m_position.m_y = p1.m_y;
				m_position.m_x = std::round(m_position.m_x / 20.0) * 20.0;
			}
			else {
				m_position.m_x = std::round(m_position.m_x / 20.0) * 20.0;
				m_position.m_y = std::round(m_position.m_y / 20.0) * 20.0;
			}

			if (std::abs(dy) > std::abs(dx)) {
				Element* bus1 = m_parentList[0];
				Element* bus2 = parent;
				wxPoint2DDouble loc1 = bus1->RotateAtPosition(wxPoint2DDouble(m_position.m_x, bus1->GetPosition().m_y), -bus1->GetAngle());
				wxPoint2DDouble loc2 = bus2->RotateAtPosition(wxPoint2DDouble(m_position.m_x, bus2->GetPosition().m_y), -bus2->GetAngle());
				double halfW1 = bus1->GetWidth() / 2.0 + 2.0;
				double halfW2 = bus2->GetWidth() / 2.0 + 2.0;
				if (std::abs(loc1.m_x - bus1->GetPosition().m_x) <= halfW1 &&
				    std::abs(loc2.m_x - bus2->GetPosition().m_x) <= halfW2) {
					m_pointList[0].m_x = m_position.m_x;
					parentPt.m_x = m_position.m_x;
				}
			}
			else {
				Element* bus1 = m_parentList[0];
				Element* bus2 = parent;
				wxPoint2DDouble loc1 = bus1->RotateAtPosition(wxPoint2DDouble(bus1->GetPosition().m_x, m_position.m_y), -bus1->GetAngle());
				wxPoint2DDouble loc2 = bus2->RotateAtPosition(wxPoint2DDouble(bus2->GetPosition().m_x, m_position.m_y), -bus2->GetAngle());
				double halfW1 = bus1->GetWidth() / 2.0 + 2.0;
				double halfW2 = bus2->GetWidth() / 2.0 + 2.0;
				if (std::abs(loc1.m_x - bus1->GetPosition().m_x) <= halfW1 &&
				    std::abs(loc2.m_x - bus2->GetPosition().m_x) <= halfW2) {
					m_pointList[0].m_y = m_position.m_y;
					parentPt.m_y = m_position.m_y;
				}
			}

			// Set the transformer rectangle.
			m_width = 70.0;
			m_height = 40.0;
			SetPosition(m_position);  // This method calculates the rectangle properly.

			// Set the terminals at 40.0 units offset (2 grid cells), rotated by m_angle
			wxPoint2DDouble term1 = m_position + RotateLocal(wxPoint2DDouble(-40.0, 0.0), m_angle);
			wxPoint2DDouble term2 = m_position + RotateLocal(wxPoint2DDouble(40.0, 0.0), m_angle);
			term1.m_x = std::round(term1.m_x);
			term1.m_y = std::round(term1.m_y);
			term2.m_x = std::round(term2.m_x);
			term2.m_y = std::round(term2.m_y);
			m_pointList.push_back(term1);
			m_pointList.push_back(term2);

			// Set first switch point.
			m_pointList[1] = GetSwitchPoint(m_parentList[0], m_pointList[0], term1);

			// Set the second switch point.
			m_pointList.push_back(GetSwitchPoint(parent, parentPt, term2));

			m_pointList.push_back(parentPt);  // Last point.
			m_inserted = true;

			wxRect2DDouble genRect(0, 0, 0, 0);
			m_switchRect.push_back(genRect);
			UpdateSwitches();
			UpdatePowerFlowArrowsPosition();

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

//void Transformer::Draw(wxPoint2DDouble translation, double scale) const
//{
//	OpenGLColour elementColour;
//	if (m_online) {
//		if (m_dynEvent)
//			elementColour = m_dynamicEventColour;
//		else
//			elementColour = m_onlineElementColour;
//	}
//	else
//		elementColour = m_offlineElementColour;
//
//	if (m_inserted) {
//		// Draw selection (layer 1).
//		if (m_selected) {
//			// Push the current matrix on stack.
//			glLineWidth(1.5 + m_borderSize * 2.0);
//			glColor4dv(m_selectionColour.GetRGBA());
//			DrawLine(m_pointList);
//			glPushMatrix();
//			// Rotate the matrix around the object position.
//			glTranslated(m_position.m_x, m_position.m_y, 0.0);
//			glRotated(m_angle, 0.0, 0.0, 1.0);
//			glTranslated(-m_position.m_x, -m_position.m_y, 0.0);
//
//			DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20 + (m_borderSize + 1.5) / scale, 20,
//				GL_POLYGON);
//			DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20 + (m_borderSize + 1.5) / scale, 20,
//				GL_POLYGON);
//
//			glPopMatrix();
//
//			// Draw nodes selection.
//			if (m_pointList.size() > 0) {
//				DrawCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
//				if (m_inserted) {
//					DrawCircle(m_pointList[m_pointList.size() - 1], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
//				}
//			}
//		}
//
//		// Draw transformer (layer 2).
//		// Transformer line
//		glLineWidth(1.5);
//		glColor4dv(elementColour.GetRGBA());
//		DrawLine(m_pointList);
//
//		// Draw nodes.
//		if (m_pointList.size() > 0) {
//			glColor4dv(elementColour.GetRGBA());
//			DrawCircle(m_pointList[0], 5.0, 10, GL_POLYGON);
//			if (m_inserted) { DrawCircle(m_pointList[m_pointList.size() - 1], 5.0, 10, GL_POLYGON); }
//		}
//
//		DrawSwitches();
//		DrawPowerFlowPts();
//
//		// Push the current matrix on stack.
//		glPushMatrix();
//		// Rotate the matrix around the object position.
//		glTranslated(m_position.m_x, m_position.m_y, 0.0);
//		glRotated(m_angle, 0.0, 0.0, 1.0);
//		glTranslated(-m_position.m_x, -m_position.m_y, 0.0);
//
//		glColor4d(1.0, 1.0, 1.0, 1.0);
//		DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20, 20, GL_POLYGON);
//		DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20, 20, GL_POLYGON);
//
//		glColor4dv(elementColour.GetRGBA());
//		DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20, 20);
//		DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20, 20);
//
//		DrawPoint(m_rect.GetPosition(), 8.0 * scale);
//
//		glPopMatrix();
//	}
//}

void Transformer::DrawDC(GUIColour* guiColour, wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const
{
	wxColour elementColour;
	if (m_online) {
		if (m_dynEvent)
			elementColour = guiColour->eventElement;
		else
			elementColour = guiColour->enabled;
	}
	else
		elementColour = guiColour->disable;

	if (m_inserted) {
		// Draw selection (layer 1).
		if (m_selected) {
			gc->SetPen(wxPen(guiColour->selection, 2 + m_borderSize * 2.0));
			gc->SetBrush(*wxTRANSPARENT_BRUSH);
			gc->StrokeLines(m_pointList.size(), &m_pointList[0]);

			// Push the current matrix on stack.
			gc->PushState();
			// Rotate the matrix around the object position.
			gc->Translate(m_position.m_x, m_position.m_y);
			gc->Rotate(wxDegToRad(m_angle));
			gc->Translate(-m_position.m_x, -m_position.m_y);

			gc->SetPen(*wxTRANSPARENT_PEN);
			gc->SetBrush(wxBrush(guiColour->selection));
			DrawDCCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20 + (m_borderSize + 1.5) / scale, 20, gc);
			DrawDCCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20 + (m_borderSize + 1.5) / scale, 20, gc);

			gc->PopState();

			// Draw nodes selection.
			gc->SetPen(*wxTRANSPARENT_PEN);
			gc->SetBrush(wxBrush(guiColour->selection));
			if (m_pointList.size() > 0) {
				DrawDCCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, gc);
				if (m_inserted) { DrawDCCircle(m_pointList[m_pointList.size() - 1], 5.0 + m_borderSize / scale, 10, gc); }
			}
		}

		// Draw transformer (layer 2).
		// Transformer line
		gc->SetPen(wxPen(elementColour, 2));
		gc->SetBrush(*wxTRANSPARENT_BRUSH);
		gc->StrokeLines(m_pointList.size(), &m_pointList[0]);

		// Draw nodes.
		gc->SetPen(*wxTRANSPARENT_PEN);
		gc->SetBrush(wxBrush(elementColour));
		if (m_pointList.size() > 0) {
			DrawDCCircle(m_pointList[0], 5.0, 10, gc);
			if (m_inserted) { DrawDCCircle(m_pointList[m_pointList.size() - 1], 5.0, 10, gc); }
		}

		DrawDCSwitches(guiColour, gc);
		DrawDCPowerFlowPts(guiColour, gc);

		// Push the current matrix on stack.
		gc->PushState();
		// Rotate the matrix around the object position.
		gc->Translate(m_position.m_x, m_position.m_y);
		gc->Rotate(wxDegToRad(m_angle));
		gc->Translate(-m_position.m_x, -m_position.m_y);

		//glColor4d(1.0, 1.0, 1.0, 1.0);
		gc->SetPen(*wxTRANSPARENT_PEN);
		gc->SetBrush(wxBrush(guiColour->background));
		DrawDCCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20, 20, gc);
		DrawDCCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20, 20, gc);

		gc->SetPen(wxPen(elementColour, 2));
		gc->SetBrush(*wxTRANSPARENT_BRUSH);
		DrawDCCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20, 20, gc);
		DrawDCCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20, 20, gc);

		// Point
		gc->SetPen(*wxTRANSPARENT_PEN);
		gc->SetBrush(wxBrush(elementColour));
		DrawDCCircle(m_rect.GetPosition(), 4, 10, gc);

		gc->PopState();
	}
}

void Transformer::DrawDC(GUIColour* guiColour, wxPoint2DDouble translation, double scale, wxDC& dc) const
{
	wxColour elementColour;
	if (m_online) {
		if (m_dynEvent)
			elementColour = guiColour->eventElement;
		else
			elementColour = guiColour->enabled;
	}
	else
		elementColour = guiColour->disable;

	std::vector<wxPoint> pointList;
	for (auto& pt : m_pointList) { pointList.emplace_back(static_cast<int>(pt.m_x), static_cast<int>(pt.m_y)); }

	if (m_inserted) {
		// Draw selection (layer 1).
		if (m_selected) {
			dc.SetPen(wxPen(wxColour(guiColour->selection), 2 + m_borderSize * 2.0));
			dc.SetBrush(*wxTRANSPARENT_BRUSH);
			dc.DrawLines(pointList.size(), &pointList[0]);

			dc.SetPen(*wxTRANSPARENT_PEN);
			dc.SetBrush(wxBrush(guiColour->selection));
			//DrawDCCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20 + (m_borderSize + 1.5) / scale, dc);
			//DrawDCCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20 + (m_borderSize + 1.5) / scale, dc);
			DrawDCCircle(m_position + RotateLocal(wxPoint2DDouble(15.0, 0.0), m_angle), 20 + (m_borderSize + 1.5) / scale, dc);
			DrawDCCircle(m_position + RotateLocal(wxPoint2DDouble(-15.0, 0.0), m_angle), 20 + (m_borderSize + 1.5) / scale, dc);

			// Draw nodes selection.
			dc.SetPen(*wxTRANSPARENT_PEN);
			dc.SetBrush(wxBrush(guiColour->selection));
			if (pointList.size() > 0) {
				DrawDCCircle(pointList[0], 5.0 + m_borderSize / scale, dc);
				if (m_inserted) { DrawDCCircle(pointList[pointList.size() - 1], 5.0 + m_borderSize / scale, dc); }
			}
		}

		// Draw transformer (layer 2).
		// Transformer line
		dc.SetPen(wxPen(elementColour, 2));
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.DrawLines(pointList.size(), &pointList[0]);

		// Draw nodes.
		dc.SetPen(*wxTRANSPARENT_PEN);
		dc.SetBrush(wxBrush(elementColour));
		if (pointList.size() > 0) {
			DrawDCCircle(pointList[0], 5.0, dc);
			if (m_inserted) { DrawDCCircle(pointList[pointList.size() - 1], 5.0, dc); }
		}

		DrawDCSwitches(guiColour, dc);
		DrawDCPowerFlowPts(guiColour, dc);

		dc.SetPen(*wxTRANSPARENT_PEN);
		dc.SetBrush(wxBrush(guiColour->background));
		DrawDCCircle(m_position + RotateLocal(wxPoint2DDouble(15.0, 0.0), m_angle), 20, dc);
		DrawDCCircle(m_position + RotateLocal(wxPoint2DDouble(-15.0, 0.0), m_angle), 20, dc);

		dc.SetPen(wxPen(elementColour, 2));
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		DrawDCCircle(m_position + RotateLocal(wxPoint2DDouble(15.0, 0.0), m_angle), 20, dc);
		DrawDCCircle(m_position + RotateLocal(wxPoint2DDouble(-15.0, 0.0), m_angle), 20, dc);

		// Point
		dc.SetPen(*wxTRANSPARENT_PEN);
		dc.SetBrush(wxBrush(elementColour));
		DrawDCCircle(m_position + RotateLocal(wxPoint2DDouble(-35, -20), m_angle), 4, dc);
	}
}

bool Transformer::Intersects(wxRect2DDouble rect) const
{
	if (m_angle == 0.0 || m_angle == 180.0) return m_rect.Intersects(rect);
	return RotatedRectanglesIntersects(m_rect, rect, m_angle, 0.0);
}

void Transformer::Rotate(bool clockwise)
{
	double rotAngle = m_rotationAngle;
	if (!clockwise) rotAngle = -m_rotationAngle;

	m_angle += rotAngle;
	while (m_angle >= 360.0) m_angle -= 360.0;
	while (m_angle < 0.0) m_angle += 360.0;

	// Clean up floating point precision for cardinal angles
	if (std::abs(m_angle - 90.0) < 1e-4) m_angle = 90.0;
	else if (std::abs(m_angle - 180.0) < 1e-4) m_angle = 180.0;
	else if (std::abs(m_angle - 270.0) < 1e-4) m_angle = 270.0;
	else if (std::abs(m_angle - 0.0) < 1e-4 || std::abs(m_angle - 360.0) < 1e-4) m_angle = 0.0;

	// Update terminals rigidly attached to transformer body at 40.0 units offset
	if (m_pointList.size() >= 4) {
		wxPoint2DDouble t1 = m_position + RotateLocal(wxPoint2DDouble(-40.0, 0.0), m_angle);
		wxPoint2DDouble t2 = m_position + RotateLocal(wxPoint2DDouble(40.0, 0.0), m_angle);
		m_pointList[2] = wxPoint2DDouble(std::round(t1.m_x), std::round(t1.m_y));
		m_pointList[3] = wxPoint2DDouble(std::round(t2.m_x), std::round(t2.m_y));
	}

	UpdateSwitchesPosition();
	UpdatePowerFlowArrowsPosition();
}

void Transformer::Move(wxPoint2DDouble position)
{
	SetPosition(m_movePos + position - m_moveStartPt);

	// Update terminals rigidly attached to transformer body at 40.0 units offset
	if (m_pointList.size() >= 4) {
		wxPoint2DDouble t1 = m_position + RotateLocal(wxPoint2DDouble(-40.0, 0.0), m_angle);
		wxPoint2DDouble t2 = m_position + RotateLocal(wxPoint2DDouble(40.0, 0.0), m_angle);
		m_pointList[2] = wxPoint2DDouble(std::round(t1.m_x), std::round(t1.m_y));
		m_pointList[3] = wxPoint2DDouble(std::round(t2.m_x), std::round(t2.m_y));
	}

	if (!m_parentList[0]) { m_pointList[0] = m_movePts[0] + position - m_moveStartPt; }
	if (!m_parentList[1]) {
		m_pointList[m_pointList.size() - 1] = m_movePts[m_pointList.size() - 1] + position - m_moveStartPt;
	}

	UpdateSwitchesPosition();
	UpdatePowerFlowArrowsPosition();
}

void Transformer::AlignToGrid(double gridSize)
{
	if (gridSize <= 0.0) gridSize = 20.0;
	// 1. Snap center position to grid
	m_position.m_x = std::round(m_position.m_x / gridSize) * gridSize;
	m_position.m_y = std::round(m_position.m_y / gridSize) * gridSize;
	SetPosition(m_position);

	// 2. Rigidly attach terminals at 40.0 units offset
	if (m_pointList.size() >= 4) {
		wxPoint2DDouble t1 = m_position + RotateLocal(wxPoint2DDouble(-40.0, 0.0), m_angle);
		wxPoint2DDouble t2 = m_position + RotateLocal(wxPoint2DDouble(40.0, 0.0), m_angle);
		m_pointList[2] = wxPoint2DDouble(std::round(t1.m_x), std::round(t1.m_y));
		m_pointList[3] = wxPoint2DDouble(std::round(t2.m_x), std::round(t2.m_y));
	}

	// 3. Align bus contact points
	bool isVertical = (std::abs(m_angle - 90.0) < 1.0 || std::abs(m_angle - 270.0) < 1.0);

	if (m_parentList.size() > 0 && m_parentList[0] && m_pointList.size() > 0) {
		Element* bus1 = m_parentList[0];
		if (isVertical) {
			wxPoint2DDouble loc = bus1->RotateAtPosition(wxPoint2DDouble(m_position.m_x, bus1->GetPosition().m_y), -bus1->GetAngle());
			double halfW = bus1->GetWidth() / 2.0 + 2.0;
			if (std::abs(loc.m_x - bus1->GetPosition().m_x) <= halfW) {
				m_pointList[0].m_x = m_position.m_x;
			}
			m_pointList[0].m_y = std::round(m_pointList[0].m_y / gridSize) * gridSize;
		}
		else {
			wxPoint2DDouble loc = bus1->RotateAtPosition(wxPoint2DDouble(bus1->GetPosition().m_x, m_position.m_y), -bus1->GetAngle());
			double halfW = bus1->GetWidth() / 2.0 + 2.0;
			if (std::abs(loc.m_x - bus1->GetPosition().m_x) <= halfW) {
				m_pointList[0].m_y = m_position.m_y;
			}
			m_pointList[0].m_x = std::round(m_pointList[0].m_x / gridSize) * gridSize;
		}
	}
	else if (m_pointList.size() > 0) {
		m_pointList[0].m_x = std::round(m_pointList[0].m_x / gridSize) * gridSize;
		m_pointList[0].m_y = std::round(m_pointList[0].m_y / gridSize) * gridSize;
	}

	if (m_parentList.size() > 1 && m_parentList[1] && m_pointList.size() > 1) {
		Element* bus2 = m_parentList[1];
		if (isVertical) {
			wxPoint2DDouble loc = bus2->RotateAtPosition(wxPoint2DDouble(m_position.m_x, bus2->GetPosition().m_y), -bus2->GetAngle());
			double halfW = bus2->GetWidth() / 2.0 + 2.0;
			if (std::abs(loc.m_x - bus2->GetPosition().m_x) <= halfW) {
				m_pointList.back().m_x = m_position.m_x;
			}
			m_pointList.back().m_y = std::round(m_pointList.back().m_y / gridSize) * gridSize;
		}
		else {
			wxPoint2DDouble loc = bus2->RotateAtPosition(wxPoint2DDouble(bus2->GetPosition().m_x, m_position.m_y), -bus2->GetAngle());
			double halfW = bus2->GetWidth() / 2.0 + 2.0;
			if (std::abs(loc.m_x - bus2->GetPosition().m_x) <= halfW) {
				m_pointList.back().m_y = m_position.m_y;
			}
			m_pointList.back().m_x = std::round(m_pointList.back().m_x / gridSize) * gridSize;
		}
	}
	else if (m_pointList.size() > 1) {
		m_pointList.back().m_x = std::round(m_pointList.back().m_x / gridSize) * gridSize;
		m_pointList.back().m_y = std::round(m_pointList.back().m_y / gridSize) * gridSize;
	}

	UpdateSwitchesPosition();
	UpdatePowerFlowArrowsPosition();
}

void Transformer::MoveNode(Element* parent, wxPoint2DDouble position)
{
	if (parent) {
		// First bus.
		if (parent == m_parentList[0]) {
			m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
		}
		// Second bus.
		else if (parent == m_parentList[1]) {
			m_pointList[m_pointList.size() - 1] = m_movePts[m_pointList.size() - 1] + position - m_moveStartPt;
		}
	}
	else {
		if (m_activeNodeID == 1) {
			m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
			if (m_parentList[0]) {
				m_parentList[0]->RemoveChild(this);
				m_parentList[0] = nullptr;
				m_online = false;
			}
		}
		else if (m_activeNodeID == 2) {
			m_pointList[m_pointList.size() - 1] = m_movePts[m_pointList.size() - 1] + position - m_moveStartPt;
			if (m_parentList[1]) {
				m_parentList[1]->RemoveChild(this);
				m_parentList[1] = nullptr;
				m_online = false;
			}
		}
	}

	// Recalculate switches positions
	UpdateSwitchesPosition();
	UpdatePowerFlowArrowsPosition();
}

void Transformer::StartMove(wxPoint2DDouble position)
{
	m_moveStartPt = position;
	m_movePts = m_pointList;
	m_movePos = m_position;
}

bool Transformer::GetContextMenu(wxMenu& menu)
{
	menu.Append(ID_EDIT_ELEMENT, _("Edit tranformer"));

	wxString busName[2] = { "?", "?" };
	if (m_parentList.size() == 2) {
		int i = 0;
		for (Element* element : m_parentList) {
			if (element) {
				Bus* bus = static_cast<Bus*>(element);
				busName[i] = bus->GetElectricalData().name;
			}
			i++;
		}
	}

	wxMenu* textMenu = new wxMenu();

	textMenu->Append(ID_TXT_NAME, _("Name"));
	textMenu->Append(ID_TXT_BRANCH_ACTIVE_POWER_1_2, _("Active power (") + busName[0] + _(" to ") + busName[1] + wxT(")"));
	textMenu->Append(ID_TXT_BRANCH_ACTIVE_POWER_2_1, _("Active power (") + busName[1] + _(" to ") + busName[0] + wxT(")"));
	textMenu->Append(ID_TXT_BRANCH_REACTIVE_POWER_1_2, _("Reactive power (") + busName[0] + _(" to ") + busName[1] + wxT(")"));
	textMenu->Append(ID_TXT_BRANCH_REACTIVE_POWER_2_1, _("Reactive power (") + busName[1] + _(" to ") + busName[0] + wxT(")"));
	textMenu->Append(ID_TXT_BRANCH_LOSSES, _("Losses"));
	textMenu->Append(ID_TXT_BRANCH_CURRENT_1_2, _("Current (") + busName[0] + _(" to ") + busName[1] + wxT(")"));
	textMenu->Append(ID_TXT_BRANCH_CURRENT_2_1, _("Current (") + busName[1] + _(" to ") + busName[0] + wxT(")"));
	textMenu->Append(ID_TXT_BRANCH_FAULT_CURRENT_1_2, _("Fault current (") + busName[0] + _(" to ") + busName[1] + wxT(")"));
	textMenu->Append(ID_TXT_BRANCH_FAULT_CURRENT_2_1, _("Fault current (") + busName[1] + _(" to ") + busName[0] + wxT(")"));
	textMenu->SetClientData(menu.GetClientData());
	menu.AppendSubMenu(textMenu, _("Add text"));

	GeneralMenuItens(menu);
	return true;
}

bool Transformer::ShowForm(wxWindow* parent, Element* element, wxWindow* workspace)
{
	TransformerForm transfForm(parent, this);
	transfForm.CenterOnParent();
	if (transfForm.ShowModal() == wxID_OK) {
		return true;
	}
	return false;
}

void Transformer::SetNominalVoltage(std::vector<double> nominalVoltage, std::vector<ElectricalUnit> nominalVoltageUnit)
{
	if (nominalVoltage.size() == 1) {
		m_electricalData.primaryNominalVoltage = nominalVoltage[0];
		m_electricalData.primaryNominalVoltageUnit = nominalVoltageUnit[0];
	}
	else if (nominalVoltage.size() == 2) {
		m_electricalData.primaryNominalVoltage = nominalVoltage[0];
		m_electricalData.primaryNominalVoltageUnit = nominalVoltageUnit[0];
		m_electricalData.secondaryNominalVoltage = nominalVoltage[1];
		m_electricalData.secondaryNominalVoltageUnit = nominalVoltageUnit[1];
	}
}

void Transformer::UpdatePowerFlowArrowsPosition()
{
	std::vector<wxPoint2DDouble> edges;
	switch (m_pfDirection) {
	case PowerFlowDirection::PF_NONE: {
		m_powerFlowArrow.clear();
	} break;
	case PowerFlowDirection::PF_BUS1_TO_BUS2: {
		for (int i = 1; i < (int)m_pointList.size() - 1; i++) { edges.push_back(m_pointList[i]); }
	} break;
	case PowerFlowDirection::PF_BUS2_TO_BUS1: {
		for (int i = (int)m_pointList.size() - 2; i > 0; i--) { edges.push_back(m_pointList[i]); }
	} break;
	default:
		break;
	}
	CalculatePowerFlowPts(edges);
}

void Transformer::RotateNode(Element* parent, bool clockwise)
{
	double rotAngle = m_rotationAngle;
	if (!clockwise) rotAngle = -m_rotationAngle;

	if (parent == m_parentList[0]) {
		m_pointList[0] = parent->RotateAtPosition(m_pointList[0], rotAngle);
	}
	else if (parent == m_parentList[1]) {
		m_pointList[m_pointList.size() - 1] = parent->RotateAtPosition(m_pointList[m_pointList.size() - 1], rotAngle);
	}
	UpdateSwitchesPosition();
	UpdatePowerFlowArrowsPosition();
}

bool Transformer::SetNodeParent(Element* parent)
{
	if (m_activeNodeID == 1 && parent == m_parentList[0]) return false;
	if (m_activeNodeID == 2 && parent == m_parentList[1]) return false;

	if (parent && m_activeNodeID != 0) {
		wxRect2DDouble nodeRect(0, 0, 0, 0);
		if (m_activeNodeID == 1) {
			nodeRect = wxRect2DDouble(m_pointList[0].m_x - 5.0 - m_borderSize, m_pointList[0].m_y - 5.0 - m_borderSize,
				10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);
		}
		if (m_activeNodeID == 2) {
			nodeRect = wxRect2DDouble(m_pointList[m_pointList.size() - 1].m_x - 5.0 - m_borderSize,
				m_pointList[m_pointList.size() - 1].m_y - 5.0 - m_borderSize,
				10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);
		}

		if (parent->Intersects(nodeRect)) {
			if (m_activeNodeID == 1) {
				// Check if the user is trying to connect the same bus.
				if (m_parentList[1] == parent) {
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
				UpdatePowerFlowArrowsPosition();
				return true;
			}
			if (m_activeNodeID == 2) {
				if (m_parentList[0] == parent) {
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
				UpdatePowerFlowArrowsPosition();
				return true;
			}
		}
		else {
			if (m_activeNodeID == 1) m_parentList[0] = nullptr;
			if (m_activeNodeID == 2) m_parentList[1] = nullptr;
		}
	}
	return false;
}

void Transformer::SetPowerFlowDirection(PowerFlowDirection pfDirection)
{
	m_pfDirection = pfDirection;
	UpdatePowerFlowArrowsPosition();
}

Element* Transformer::GetCopy()
{
	Transformer* copy = new Transformer();
	*copy = *this;
	return copy;
}

wxString Transformer::GetTipText() const
{
	wxString tipText = m_electricalData.name;
	wxString primVoltage = StringFromDouble(m_electricalData.primaryNominalVoltage);
	switch (m_electricalData.primaryNominalVoltageUnit) {
	case ElectricalUnit::UNIT_V: {
		primVoltage += _(" V");
	} break;
	case ElectricalUnit::UNIT_kV: {
		primVoltage += _(" kV");
	} break;
	default:
		break;
	}
	wxString secVoltage = StringFromDouble(m_electricalData.secondaryNominalVoltage);
	switch (m_electricalData.secondaryNominalVoltageUnit) {
	case ElectricalUnit::UNIT_V: {
		secVoltage += _(" V");
	} break;
	case ElectricalUnit::UNIT_kV: {
		secVoltage += _(" kV");
	} break;
	default:
		break;
	}

	tipText += "\n" + primVoltage + " / " + secVoltage;

	if (m_online) {
		tipText += "\n";
		int busNumber[2];
		busNumber[0] = static_cast<Bus*>(m_parentList[0])->GetElectricalData().number + 1;
		busNumber[1] = static_cast<Bus*>(m_parentList[1])->GetElectricalData().number + 1;

		tipText += _("\nP") + wxString::Format("(%d-%d) = ", busNumber[0], busNumber[1]) +
			wxString::FromDouble(m_electricalData.powerFlow[0].real(), 5) + _(" p.u.");
		tipText += _("\nQ") + wxString::Format("(%d-%d) = ", busNumber[0], busNumber[1]) +
			wxString::FromDouble(m_electricalData.powerFlow[0].imag(), 5) + _(" p.u.");
		tipText += _("\nP") + wxString::Format("(%d-%d) = ", busNumber[1], busNumber[0]) +
			wxString::FromDouble(m_electricalData.powerFlow[1].real(), 5) + _(" p.u.");
		tipText += _("\nQ") + wxString::Format("(%d-%d) = ", busNumber[1], busNumber[0]) +
			wxString::FromDouble(m_electricalData.powerFlow[1].imag(), 5) + _(" p.u.");

		if (m_electricalData.hasTapChanger) {
			tipText += _("\nOLTC: Enabled (Vset = ") +
				wxString::FromDouble(m_electricalData.oltcTargetVoltage, 3) +
				_(" p.u., Tap = ") +
				wxString::FromDouble(m_electricalData.turnsRatio, 4) +
				_(" p.u.)");
		}

		if (!m_electricalData.harmonicOrder.empty()) {
			tipText += _("\n\nHarmonic currents:");
			int i = 0;
			for (auto& hCurrent1 : m_electricalData.harmonicCurrent[0]) {
				auto& hCurrent2 = m_electricalData.harmonicCurrent[1][i];
				wxString i1, i2;
				i1.Printf(_("\nIh(%d)(%d-%d) = %.5e%s%.2f%s p.u."), m_electricalData.harmonicOrder[i], busNumber[0], busNumber[1], std::abs(hCurrent1), wxString(L'\u2220'), wxRadToDeg(std::arg(hCurrent1)), wxString(L'\u00B0'));
				i2.Printf(_("\nIh(%d)(%d-%d) = %.5e%s%.2f%s p.u."), m_electricalData.harmonicOrder[i], busNumber[1], busNumber[0], std::abs(hCurrent2), wxString(L'\u2220'), wxRadToDeg(std::arg(hCurrent2)), wxString(L'\u00B0'));

				tipText += i1 + i2;
				i++;
			}
		}
	}

	return tipText;
}

TransformerElectricalData Transformer::GetPUElectricalData(double systemBasePower) const
{
	TransformerElectricalData data;

	data.name = m_electricalData.name;

	data.nominalPower = m_electricalData.nominalPower;
	data.nominalPowerUnit = m_electricalData.nominalPowerUnit;

	data.primaryNominalVoltage = m_electricalData.primaryNominalVoltage;
	data.primaryNominalVoltageUnit = m_electricalData.primaryNominalVoltageUnit;

	data.secondaryNominalVoltage = m_electricalData.secondaryNominalVoltage;
	data.secondaryNominalVoltageUnit = m_electricalData.secondaryNominalVoltageUnit;

	data.useTransformerPower = m_electricalData.useTransformerPower;

	data.baseVoltage = m_electricalData.baseVoltage;

	data.resistance = m_electricalData.resistance;
	data.resistanceUnit = m_electricalData.resistanceUnit;

	data.indReactance = m_electricalData.indReactance;
	data.indReactanceUnit = m_electricalData.indReactanceUnit;

	data.connection = m_electricalData.connection;
	data.turnsRatio = m_electricalData.turnsRatio;
	data.phaseShift = m_electricalData.phaseShift;

	data.hasTapChanger = m_electricalData.hasTapChanger;
	data.nominalTurnsRatio = m_electricalData.nominalTurnsRatio;
	data.oltcControlledBus = m_electricalData.oltcControlledBus;
	data.oltcTargetVoltage = m_electricalData.oltcTargetVoltage;
	data.oltcVoltageDeadband = m_electricalData.oltcVoltageDeadband;
	data.oltcMinTap = m_electricalData.oltcMinTap;
	data.oltcMaxTap = m_electricalData.oltcMaxTap;
	data.oltcTapStep = m_electricalData.oltcTapStep;
	data.oltcIsDiscrete = m_electricalData.oltcIsDiscrete;

	data.zeroResistance = m_electricalData.zeroResistance;
	data.zeroIndReactance = m_electricalData.zeroIndReactance;

	data.primaryGrndResistance = m_electricalData.primaryGrndResistance;
	data.primaryGrndReactance = m_electricalData.primaryGrndReactance;

	data.secondaryGrndResistance = m_electricalData.secondaryGrndResistance;
	data.secondaryGrndReactance = m_electricalData.secondaryGrndReactance;

	data.powerFlow[0] = m_electricalData.powerFlow[0];
	data.powerFlow[1] = m_electricalData.powerFlow[1];

	data.faultCurrent[0][0] = m_electricalData.faultCurrent[0][0];
	data.faultCurrent[0][1] = m_electricalData.faultCurrent[0][1];
	data.faultCurrent[0][2] = m_electricalData.faultCurrent[0][2];
	data.faultCurrent[1][0] = m_electricalData.faultCurrent[1][0];
	data.faultCurrent[1][1] = m_electricalData.faultCurrent[1][1];
	data.faultCurrent[1][2] = m_electricalData.faultCurrent[1][2];


	data.harmonicOrder = m_electricalData.harmonicOrder;
	data.harmonicCurrent[0] = m_electricalData.harmonicCurrent[0];
	data.harmonicCurrent[1] = m_electricalData.harmonicCurrent[1];

	double transformerBasePower = GetValueFromUnit(data.nominalPower, data.nominalPowerUnit);
	double baseVoltage = 0.0;
	if (data.baseVoltage == 0) {
		baseVoltage = GetValueFromUnit(data.primaryNominalVoltage, data.primaryNominalVoltageUnit);
	}
	else {
		baseVoltage = GetValueFromUnit(data.secondaryNominalVoltage, data.secondaryNominalVoltageUnit);
	}
	double systemBaseImpedance = (baseVoltage * baseVoltage) / systemBasePower;
	double transformerBaseImpedance = (baseVoltage * baseVoltage) / transformerBasePower;

	// Resistance
	double r = data.resistance;
	if (data.resistanceUnit == ElectricalUnit::UNIT_PU) {
		if (data.useTransformerPower) data.resistance = (r * transformerBaseImpedance) / systemBaseImpedance;
	}
	else {
		data.resistance = r / systemBaseImpedance;
	}
	data.resistanceUnit = ElectricalUnit::UNIT_PU;

	// Indutive reactance
	double x = data.indReactance;
	if (data.indReactanceUnit == ElectricalUnit::UNIT_PU) {
		if (data.useTransformerPower) data.indReactance = (x * transformerBaseImpedance) / systemBaseImpedance;
	}
	else {
		data.indReactance = x / systemBaseImpedance;
	}
	data.indReactanceUnit = ElectricalUnit::UNIT_PU;

	// Fault

	// Zero seq. resistance
	double r0 = data.zeroResistance;
	if (data.useTransformerPower) data.zeroResistance = (r0 * transformerBaseImpedance) / systemBaseImpedance;

	// Zero seq. ind. reactance
	double x0 = data.zeroIndReactance;
	if (data.useTransformerPower) data.zeroIndReactance = (x0 * transformerBaseImpedance) / systemBaseImpedance;

	// Primary ground resistance
	double rgp = data.primaryGrndResistance;
	if (data.useTransformerPower) data.primaryGrndResistance = (rgp * transformerBaseImpedance) / systemBaseImpedance;

	// Primary ground ind reactance
	double xgp = data.primaryGrndReactance;
	if (data.useTransformerPower) data.primaryGrndReactance = (xgp * transformerBaseImpedance) / systemBaseImpedance;

	// Secondary ground resistance
	double rgs = data.secondaryGrndResistance;
	if (data.useTransformerPower) data.secondaryGrndResistance = (rgs * transformerBaseImpedance) / systemBaseImpedance;

	// Secondary ground ind reactance
	double xgs = data.secondaryGrndReactance;
	if (data.useTransformerPower) data.secondaryGrndReactance = (xgs * transformerBaseImpedance) / systemBaseImpedance;

	if (!m_online) {
		data.powerFlow[0] = std::complex<double>(0, 0);
		data.powerFlow[1] = std::complex<double>(0, 0);
		data.faultCurrent[0][0] = std::complex<double>(0, 0);
		data.faultCurrent[0][1] = std::complex<double>(0, 0);
		data.faultCurrent[0][2] = std::complex<double>(0, 0);
		data.faultCurrent[1][0] = std::complex<double>(0, 0);
		data.faultCurrent[1][1] = std::complex<double>(0, 0);
		data.faultCurrent[1][2] = std::complex<double>(0, 0);
	}

	return data;
}

rapidxml::xml_node<>* Transformer::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
	auto elementNode = XMLParser::AppendNode(doc, elementListNode, "Transfomer");
	XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);
	auto cadProp = XMLParser::AppendNode(doc, elementNode, "CADProperties");
	auto position = XMLParser::AppendNode(doc, cadProp, "Position");
	auto posX = XMLParser::AppendNode(doc, position, "X");
	XMLParser::SetNodeValue(doc, posX, m_position.m_x);
	auto posY = XMLParser::AppendNode(doc, position, "Y");
	XMLParser::SetNodeValue(doc, posY, m_position.m_y);
	auto size = XMLParser::AppendNode(doc, cadProp, "Size");
	auto width = XMLParser::AppendNode(doc, size, "Width");
	XMLParser::SetNodeValue(doc, width, m_width);
	auto height = XMLParser::AppendNode(doc, size, "Height");
	XMLParser::SetNodeValue(doc, height, m_height);
	auto angle = XMLParser::AppendNode(doc, cadProp, "Angle");
	XMLParser::SetNodeValue(doc, angle, m_angle);
	auto nodeList = XMLParser::AppendNode(doc, cadProp, "NodeList");
	auto nodePos1 = XMLParser::AppendNode(doc, nodeList, "Node");
	XMLParser::SetNodeAttribute(doc, nodePos1, "ID", 0);
	auto nodePosX1 = XMLParser::AppendNode(doc, nodePos1, "X");
	XMLParser::SetNodeValue(doc, nodePosX1, m_pointList[0].m_x);
	auto nodePosY1 = XMLParser::AppendNode(doc, nodePos1, "Y");
	XMLParser::SetNodeValue(doc, nodePosY1, m_pointList[0].m_y);
	auto nodePos2 = XMLParser::AppendNode(doc, nodeList, "Node");
	XMLParser::SetNodeAttribute(doc, nodePos2, "ID", 1);
	auto nodePosX2 = XMLParser::AppendNode(doc, nodePos2, "X");
	XMLParser::SetNodeValue(doc, nodePosX2, m_pointList[m_pointList.size() - 1].m_x);
	auto nodePosY2 = XMLParser::AppendNode(doc, nodePos2, "Y");
	XMLParser::SetNodeValue(doc, nodePosY2, m_pointList[m_pointList.size() - 1].m_y);

	auto parentIDList = XMLParser::AppendNode(doc, cadProp, "ParentIDList");
	for (unsigned int i = 0; i < m_parentList.size(); i++) {
		Element* parent = m_parentList[i];
		if (parent) {
			auto parentID = XMLParser::AppendNode(doc, parentIDList, "ParentID");
			XMLParser::SetNodeAttribute(doc, parentID, "ID", static_cast<int>(i));
			XMLParser::SetNodeValue(doc, parentID, parent->GetID());
		}
	}

	auto electricalProp = XMLParser::AppendNode(doc, elementNode, "ElectricalProperties");
	auto isOnline = XMLParser::AppendNode(doc, electricalProp, "IsOnline");
	XMLParser::SetNodeValue(doc, isOnline, m_online);
	auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
	XMLParser::SetNodeValue(doc, name, m_electricalData.name);
	auto primaryNominalVoltage = XMLParser::AppendNode(doc, electricalProp, "PrimaryNominalVoltage");
	XMLParser::SetNodeValue(doc, primaryNominalVoltage, m_electricalData.primaryNominalVoltage);
	XMLParser::SetNodeAttribute(doc, primaryNominalVoltage, "UnitID", static_cast<int>(m_electricalData.primaryNominalVoltageUnit));
	auto secondaryNominalVoltage = XMLParser::AppendNode(doc, electricalProp, "SecondaryNominalVoltage");
	XMLParser::SetNodeValue(doc, secondaryNominalVoltage, m_electricalData.secondaryNominalVoltage);
	XMLParser::SetNodeAttribute(doc, secondaryNominalVoltage, "UnitID", static_cast<int>(m_electricalData.secondaryNominalVoltageUnit));
	auto nominalPower = XMLParser::AppendNode(doc, electricalProp, "NominalPower");
	XMLParser::SetNodeValue(doc, nominalPower, m_electricalData.nominalPower);
	XMLParser::SetNodeAttribute(doc, nominalPower, "UnitID", static_cast<int>(m_electricalData.nominalPowerUnit));
	auto resistance = XMLParser::AppendNode(doc, electricalProp, "Resistance");
	XMLParser::SetNodeValue(doc, resistance, m_electricalData.resistance);
	XMLParser::SetNodeAttribute(doc, resistance, "UnitID", static_cast<int>(m_electricalData.resistanceUnit));
	auto indReactance = XMLParser::AppendNode(doc, electricalProp, "IndReactance");
	XMLParser::SetNodeValue(doc, indReactance, m_electricalData.indReactance);
	XMLParser::SetNodeAttribute(doc, indReactance, "UnitID", static_cast<int>(m_electricalData.indReactanceUnit));
	auto connection = XMLParser::AppendNode(doc, electricalProp, "Connection");
	XMLParser::SetNodeValue(doc, connection, m_electricalData.connection);
	auto turnsRatio = XMLParser::AppendNode(doc, electricalProp, "TurnsRatio");
	XMLParser::SetNodeValue(doc, turnsRatio, m_electricalData.turnsRatio);
	auto phaseShift = XMLParser::AppendNode(doc, electricalProp, "PhaseShift");
	XMLParser::SetNodeValue(doc, phaseShift, m_electricalData.phaseShift);
	auto useTransformerPower = XMLParser::AppendNode(doc, electricalProp, "UseTransfomerPower");
	XMLParser::SetNodeValue(doc, useTransformerPower, m_electricalData.useTransformerPower);

	auto oltcNode = XMLParser::AppendNode(doc, electricalProp, "TapChanger");
	auto hasTapChanger = XMLParser::AppendNode(doc, oltcNode, "Enabled");
	XMLParser::SetNodeValue(doc, hasTapChanger, m_electricalData.hasTapChanger ? 1 : 0);
	auto nominalTurnsRatio = XMLParser::AppendNode(doc, oltcNode, "NominalTurnsRatio");
	XMLParser::SetNodeValue(doc, nominalTurnsRatio, m_electricalData.nominalTurnsRatio);
	auto oltcControlledBus = XMLParser::AppendNode(doc, oltcNode, "ControlledBus");
	XMLParser::SetNodeValue(doc, oltcControlledBus, m_electricalData.oltcControlledBus);
	auto oltcTargetVoltage = XMLParser::AppendNode(doc, oltcNode, "TargetVoltage");
	XMLParser::SetNodeValue(doc, oltcTargetVoltage, m_electricalData.oltcTargetVoltage);
	auto oltcVoltageDeadband = XMLParser::AppendNode(doc, oltcNode, "VoltageDeadband");
	XMLParser::SetNodeValue(doc, oltcVoltageDeadband, m_electricalData.oltcVoltageDeadband);
	auto oltcMinTap = XMLParser::AppendNode(doc, oltcNode, "MinTap");
	XMLParser::SetNodeValue(doc, oltcMinTap, m_electricalData.oltcMinTap);
	auto oltcMaxTap = XMLParser::AppendNode(doc, oltcNode, "MaxTap");
	XMLParser::SetNodeValue(doc, oltcMaxTap, m_electricalData.oltcMaxTap);
	auto oltcTapStep = XMLParser::AppendNode(doc, oltcNode, "TapStep");
	XMLParser::SetNodeValue(doc, oltcTapStep, m_electricalData.oltcTapStep);
	auto oltcIsDiscrete = XMLParser::AppendNode(doc, oltcNode, "IsDiscrete");
	XMLParser::SetNodeValue(doc, oltcIsDiscrete, m_electricalData.oltcIsDiscrete ? 1 : 0);

	auto fault = XMLParser::AppendNode(doc, electricalProp, "Fault");
	auto zeroResistance = XMLParser::AppendNode(doc, fault, "ZeroResistance");
	XMLParser::SetNodeValue(doc, zeroResistance, m_electricalData.zeroResistance);
	auto zeroIndReactance = XMLParser::AppendNode(doc, fault, "ZeroIndReactance");
	XMLParser::SetNodeValue(doc, zeroIndReactance, m_electricalData.zeroIndReactance);
	auto primaryGrndResistance = XMLParser::AppendNode(doc, fault, "PrimaryGrndResistance");
	XMLParser::SetNodeValue(doc, primaryGrndResistance, m_electricalData.primaryGrndResistance);
	auto primaryGrndReactance = XMLParser::AppendNode(doc, fault, "PrimaryGrndReactance");
	XMLParser::SetNodeValue(doc, primaryGrndReactance, m_electricalData.primaryGrndReactance);
	auto secondaryGrndResistance = XMLParser::AppendNode(doc, fault, "SecondaryGrndResistance");
	XMLParser::SetNodeValue(doc, secondaryGrndResistance, m_electricalData.secondaryGrndResistance);
	auto secondaryGrndReactance = XMLParser::AppendNode(doc, fault, "SecondaryGrndReactance");
	XMLParser::SetNodeValue(doc, secondaryGrndReactance, m_electricalData.secondaryGrndReactance);

	SaveSwitchingData(doc, electricalProp);

	return elementNode;
}

bool Transformer::OpenElement(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList)
{
	auto cadPropNode = elementNode->first_node("CADProperties");
	if (!cadPropNode) return false;

	auto position = cadPropNode->first_node("Position");
	double posX = XMLParser::GetNodeValueDouble(position, "X");
	double posY = XMLParser::GetNodeValueDouble(position, "Y");
	auto size = cadPropNode->first_node("Size");
	m_width = XMLParser::GetNodeValueDouble(size, "Width");
	m_height = XMLParser::GetNodeValueDouble(size, "Height");
	double angle = XMLParser::GetNodeValueDouble(cadPropNode, "Angle");

	// Get nodes points
	std::vector<wxPoint2DDouble> ptsList;
	auto nodePosList = cadPropNode->first_node("NodeList");
	if (!nodePosList) return false;
	auto nodePos = nodePosList->first_node("Node");
	while (nodePos) {
		double nodePosX = XMLParser::GetNodeValueDouble(nodePos, "X");
		double nodePosY = XMLParser::GetNodeValueDouble(nodePos, "Y");
		ptsList.push_back(wxPoint2DDouble(nodePosX, nodePosY));
		nodePos = nodePos->next_sibling("Node");
	}

	// Get parents IDs
	auto parentIDList = cadPropNode->first_node("ParentIDList");
	if (!parentIDList) return false;
	auto parentNode = parentIDList->first_node("ParentID");
	long parentID[2] = { -1, -1 };
	while (parentNode) {
		long index = 0;
		wxString(parentNode->first_attribute("ID")->value()).ToLong(&index);
		wxString(parentNode->value()).ToCLong(&parentID[index]);
		parentNode = parentNode->next_sibling("ParentID");
	}

	std::vector<wxPoint2DDouble> nodePtsList;            // List of node points
	nodePtsList.push_back(ptsList[0]);                   // First point on the list
	nodePtsList.push_back(ptsList[ptsList.size() - 1]);  // Last point on the list

	// List of dummy buses to set not connected nodes properly
	std::vector<Bus*> dummyBusList;
	// Set parents (if have)
	for (unsigned int i = 0; i < 2; ++i) {
		if (parentID[i] == -1)  // No parent connected
		{
			Bus* dummyBus = new Bus(nodePtsList[i]);
			dummyBusList.push_back(dummyBus);
			AddParent(dummyBus, nodePtsList[i]);
		}
		else {  // Parent connected (necessarily a bus, get from bus list)
			AddParent(parentList[parentID[i]], nodePtsList[i]);
		}
	}

	StartMove(m_position);
	Move(wxPoint2DDouble(posX, posY));

	// Remove dummy buses
	for (auto it = dummyBusList.begin(), itEnd = dummyBusList.end(); it != itEnd; ++it) {
		RemoveParent(*it);
		delete* it;
	}
	dummyBusList.clear();

	// Set rotation properly.
	int numRot = angle / GetRotationAngle();
	bool clockwise = true;
	if (numRot < 0) {
		numRot = std::abs(numRot);
		clockwise = false;
	}
	for (int i = 0; i < numRot; i++) Rotate(clockwise);

	auto electricalProp = elementNode->first_node("ElectricalProperties");
	if (!electricalProp) return false;

	SetOnline(XMLParser::GetNodeValueInt(electricalProp, "IsOnline"));
	m_electricalData.name = electricalProp->first_node("Name")->value();
	m_electricalData.primaryNominalVoltage = XMLParser::GetNodeValueDouble(electricalProp, "PrimaryNominalVoltage");
	m_electricalData.primaryNominalVoltageUnit =
		static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "PrimaryNominalVoltage", "UnitID"));
	m_electricalData.secondaryNominalVoltage = XMLParser::GetNodeValueDouble(electricalProp, "SecondaryNominalVoltage");
	m_electricalData.secondaryNominalVoltageUnit = static_cast<ElectricalUnit>(
		XMLParser::GetAttributeValueInt(electricalProp, "SecondaryNominalVoltage", "UnitID"));
	m_electricalData.nominalPower = XMLParser::GetNodeValueDouble(electricalProp, "NominalPower");
	m_electricalData.nominalPowerUnit =
		static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "NominalPower", "UnitID"));
	m_electricalData.resistance = XMLParser::GetNodeValueDouble(electricalProp, "Resistance");
	m_electricalData.resistanceUnit =
		static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "Resistance", "UnitID"));
	m_electricalData.indReactance = XMLParser::GetNodeValueDouble(electricalProp, "IndReactance");
	m_electricalData.indReactanceUnit =
		static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "IndReactance", "UnitID"));
	m_electricalData.connection = (TransformerConnection)XMLParser::GetNodeValueInt(electricalProp, "Connection");
	m_electricalData.turnsRatio = XMLParser::GetNodeValueDouble(electricalProp, "TurnsRatio");
	m_electricalData.phaseShift = XMLParser::GetNodeValueDouble(electricalProp, "PhaseShift");
	m_electricalData.useTransformerPower = XMLParser::GetNodeValueInt(electricalProp, "UseTransfomerPower");

	auto oltcNode = electricalProp->first_node("TapChanger");
	if (oltcNode) {
		m_electricalData.hasTapChanger = XMLParser::GetNodeValueInt(oltcNode, "Enabled") == 1;
		auto nomNode = oltcNode->first_node("NominalTurnsRatio");
		if (nomNode) wxString(nomNode->value()).ToCDouble(&m_electricalData.nominalTurnsRatio);
		else m_electricalData.nominalTurnsRatio = m_electricalData.turnsRatio;
		auto ctrlBusNode = oltcNode->first_node("ControlledBus");
		if (ctrlBusNode) {
			long cb = 1;
			wxString(ctrlBusNode->value()).ToCLong(&cb);
			m_electricalData.oltcControlledBus = (int)cb;
		}
		auto targetVNode = oltcNode->first_node("TargetVoltage");
		if (targetVNode) wxString(targetVNode->value()).ToCDouble(&m_electricalData.oltcTargetVoltage);
		auto deadbandNode = oltcNode->first_node("VoltageDeadband");
		if (deadbandNode) wxString(deadbandNode->value()).ToCDouble(&m_electricalData.oltcVoltageDeadband);
		auto minTapNode = oltcNode->first_node("MinTap");
		if (minTapNode) wxString(minTapNode->value()).ToCDouble(&m_electricalData.oltcMinTap);
		auto maxTapNode = oltcNode->first_node("MaxTap");
		if (maxTapNode) wxString(maxTapNode->value()).ToCDouble(&m_electricalData.oltcMaxTap);
		auto tapStepNode = oltcNode->first_node("TapStep");
		if (tapStepNode) wxString(tapStepNode->value()).ToCDouble(&m_electricalData.oltcTapStep);
		m_electricalData.oltcIsDiscrete = XMLParser::GetNodeValueInt(oltcNode, "IsDiscrete") == 1;
	}
	else {
		m_electricalData.hasTapChanger = false;
		m_electricalData.nominalTurnsRatio = m_electricalData.turnsRatio;
		m_electricalData.oltcControlledBus = 1;
		m_electricalData.oltcTargetVoltage = 1.0;
		m_electricalData.oltcVoltageDeadband = 0.005;
		m_electricalData.oltcMinTap = 0.90;
		m_electricalData.oltcMaxTap = 1.10;
		m_electricalData.oltcTapStep = 0.00625;
		m_electricalData.oltcIsDiscrete = false;
	}

	auto fault = electricalProp->first_node("Fault");
	m_electricalData.zeroResistance = XMLParser::GetNodeValueDouble(fault, "ZeroResistance");
	m_electricalData.zeroIndReactance = XMLParser::GetNodeValueDouble(fault, "ZeroIndReactance");
	m_electricalData.primaryGrndResistance = XMLParser::GetNodeValueDouble(fault, "PrimaryGrndResistance");
	m_electricalData.primaryGrndReactance = XMLParser::GetNodeValueDouble(fault, "PrimaryGrndReactance");
	m_electricalData.secondaryGrndResistance = XMLParser::GetNodeValueDouble(fault, "SecondaryGrndResistance");
	m_electricalData.secondaryGrndReactance = XMLParser::GetNodeValueDouble(fault, "SecondaryGrndReactance");

	if (!OpenSwitchingData(electricalProp)) return false;
	if (m_swData.swTime.size() != 0) SetDynamicEvent(true);

	return true;
}

void Transformer::SetBestPositionAndRotation()
{
	wxPoint2DDouble p1 = m_pointList[0];
	wxPoint2DDouble p2 = m_pointList[m_pointList.size() - 1];
	double dx = p2.m_x - p1.m_x;
	double dy = p2.m_y - p1.m_y;

	if (std::abs(dy) > std::abs(dx)) {
		m_angle = (dy >= 0.0) ? 90.0 : 270.0;
	}
	else {
		m_angle = (dx >= 0.0) ? 0.0 : 180.0;
	}

	wxPoint2DDouble mid = (p1 + p2) / 2.0;
	if (std::abs(dx) < 1.0) {
		mid.m_x = p1.m_x;
		mid.m_y = std::round(mid.m_y / 20.0) * 20.0;
	}
	else if (std::abs(dy) < 1.0) {
		mid.m_y = p1.m_y;
		mid.m_x = std::round(mid.m_x / 20.0) * 20.0;
	}
	else {
		mid.m_x = std::round(mid.m_x / 20.0) * 20.0;
		mid.m_y = std::round(mid.m_y / 20.0) * 20.0;
	}

	SetPosition(mid);

	if (m_parentList.size() > 0 && m_parentList[0] && m_parentList.size() > 1 && m_parentList[1]) {
		Element* bus1 = m_parentList[0];
		Element* bus2 = m_parentList[1];
		if (std::abs(dy) > std::abs(dx)) {
			wxPoint2DDouble loc1 = bus1->RotateAtPosition(wxPoint2DDouble(m_position.m_x, bus1->GetPosition().m_y), -bus1->GetAngle());
			wxPoint2DDouble loc2 = bus2->RotateAtPosition(wxPoint2DDouble(m_position.m_x, bus2->GetPosition().m_y), -bus2->GetAngle());
			double halfW1 = bus1->GetWidth() / 2.0 + 2.0;
			double halfW2 = bus2->GetWidth() / 2.0 + 2.0;
			if (std::abs(loc1.m_x - bus1->GetPosition().m_x) <= halfW1 &&
			    std::abs(loc2.m_x - bus2->GetPosition().m_x) <= halfW2) {
				m_pointList[0].m_x = m_position.m_x;
				m_pointList.back().m_x = m_position.m_x;
			}
		}
		else {
			wxPoint2DDouble loc1 = bus1->RotateAtPosition(wxPoint2DDouble(bus1->GetPosition().m_x, m_position.m_y), -bus1->GetAngle());
			wxPoint2DDouble loc2 = bus2->RotateAtPosition(wxPoint2DDouble(bus2->GetPosition().m_x, m_position.m_y), -bus2->GetAngle());
			double halfW1 = bus1->GetWidth() / 2.0 + 2.0;
			double halfW2 = bus2->GetWidth() / 2.0 + 2.0;
			if (std::abs(loc1.m_x - bus1->GetPosition().m_x) <= halfW1 &&
			    std::abs(loc2.m_x - bus2->GetPosition().m_x) <= halfW2) {
				m_pointList[0].m_y = m_position.m_y;
				m_pointList.back().m_y = m_position.m_y;
			}
		}
	}

	if (m_pointList.size() >= 4) {
		wxPoint2DDouble t1 = m_position + RotateLocal(wxPoint2DDouble(-40.0, 0.0), m_angle);
		wxPoint2DDouble t2 = m_position + RotateLocal(wxPoint2DDouble(40.0, 0.0), m_angle);
		m_pointList[2] = wxPoint2DDouble(std::round(t1.m_x), std::round(t1.m_y));
		m_pointList[3] = wxPoint2DDouble(std::round(t2.m_x), std::round(t2.m_y));
	}
	UpdateSwitchesPosition();
	UpdatePowerFlowArrowsPosition();
}
