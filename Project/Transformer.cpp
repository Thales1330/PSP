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
#include "TransformerForm.h"

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

			// Get the average between the two bus points.
			m_position =
				wxPoint2DDouble((m_pointList[0].m_x + parentPt.m_x) / 2.0, (m_pointList[0].m_y + parentPt.m_y) / 2.0);
			// Set the transformer rectangle.
			m_width = 70.0;
			m_height = 40.0;
			SetPosition(m_position);  // This method calculates the rectangle propely.
			// Set the "side" points.
			m_pointList.push_back(
				wxPoint2DDouble(m_rect.GetPosition() + wxPoint2DDouble(-10 - m_borderSize, m_height / 2.0)));
			m_pointList.push_back(
				wxPoint2DDouble(m_rect.GetPosition() + wxPoint2DDouble(m_width + 10 + m_borderSize, m_height / 2.0)));

			// Set first switch point.
			wxPoint2DDouble secondPoint = parentPt;
			if (m_pointList.size() > 2) { secondPoint = m_pointList[2]; }
			m_pointList[1] = GetSwitchPoint(m_parentList[0], m_pointList[0], secondPoint);

			// Set the second switch point.
			m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_pointList[m_pointList.size() - 1]));

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

void Transformer::Draw(wxPoint2DDouble translation, double scale) const
{
	OpenGLColour elementColour;
	if (m_online) {
		if (m_dynEvent)
			elementColour = m_dynamicEventColour;
		else
			elementColour = m_onlineElementColour;
	}
	else
		elementColour = m_offlineElementColour;

	if (m_inserted) {
		// Draw selection (layer 1).
		if (m_selected) {
			// Push the current matrix on stack.
			glLineWidth(1.5 + m_borderSize * 2.0);
			glColor4dv(m_selectionColour.GetRGBA());
			DrawLine(m_pointList);
			glPushMatrix();
			// Rotate the matrix around the object position.
			glTranslated(m_position.m_x, m_position.m_y, 0.0);
			glRotated(m_angle, 0.0, 0.0, 1.0);
			glTranslated(-m_position.m_x, -m_position.m_y, 0.0);

			DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20 + (m_borderSize + 1.5) / scale, 20,
				GL_POLYGON);
			DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20 + (m_borderSize + 1.5) / scale, 20,
				GL_POLYGON);

			glPopMatrix();

			// Draw nodes selection.
			if (m_pointList.size() > 0) {
				DrawCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
				if (m_inserted) {
					DrawCircle(m_pointList[m_pointList.size() - 1], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
				}
			}
		}

		// Draw transformer (layer 2).
		// Transformer line
		glLineWidth(1.5);
		glColor4dv(elementColour.GetRGBA());
		DrawLine(m_pointList);

		// Draw nodes.
		if (m_pointList.size() > 0) {
			glColor4dv(elementColour.GetRGBA());
			DrawCircle(m_pointList[0], 5.0, 10, GL_POLYGON);
			if (m_inserted) { DrawCircle(m_pointList[m_pointList.size() - 1], 5.0, 10, GL_POLYGON); }
		}

		DrawSwitches();
		DrawPowerFlowPts();

		// Push the current matrix on stack.
		glPushMatrix();
		// Rotate the matrix around the object position.
		glTranslated(m_position.m_x, m_position.m_y, 0.0);
		glRotated(m_angle, 0.0, 0.0, 1.0);
		glTranslated(-m_position.m_x, -m_position.m_y, 0.0);

		glColor4d(1.0, 1.0, 1.0, 1.0);
		DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20, 20, GL_POLYGON);
		DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20, 20, GL_POLYGON);

		glColor4dv(elementColour.GetRGBA());
		DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20, 20);
		DrawCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20, 20);

		DrawPoint(m_rect.GetPosition(), 8.0 * scale);

		glPopMatrix();
	}
}

void Transformer::DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const
{
	OpenGLColour elementColour;
	if (m_online) {
		if (m_dynEvent)
			elementColour = m_dynamicEventColour;
		else
			elementColour = m_onlineElementColour;
	}
	else
		elementColour = m_offlineElementColour;

	if (m_inserted) {
		// Draw selection (layer 1).
		if (m_selected) {
			gc->SetPen(wxPen(wxColour(m_selectionColour.GetDcRGBA()), 2 + m_borderSize * 2.0));
			gc->SetBrush(*wxTRANSPARENT_BRUSH);
			gc->DrawLines(m_pointList.size(), &m_pointList[0]);

			// Push the current matrix on stack.
			gc->PushState();
			// Rotate the matrix around the object position.
			gc->Translate(m_position.m_x, m_position.m_y);
			gc->Rotate(wxDegToRad(m_angle));
			gc->Translate(-m_position.m_x, -m_position.m_y);

			gc->SetPen(*wxTRANSPARENT_PEN);
			gc->SetBrush(wxBrush(wxColour(m_selectionColour.GetDcRGBA())));
			DrawDCCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20 + (m_borderSize + 1.5) / scale, 20, gc);
			DrawDCCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20 + (m_borderSize + 1.5) / scale, 20, gc);

			gc->PopState();

			// Draw nodes selection.
			gc->SetPen(*wxTRANSPARENT_PEN);
			gc->SetBrush(wxBrush(wxColour(m_selectionColour.GetDcRGBA())));
			if (m_pointList.size() > 0) {
				DrawDCCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, gc);
				if (m_inserted) { DrawDCCircle(m_pointList[m_pointList.size() - 1], 5.0 + m_borderSize / scale, 10, gc); }
			}
		}

		// Draw transformer (layer 2).
		// Transformer line
		gc->SetPen(wxPen(wxColour(elementColour.GetDcRGBA()), 2));
		gc->SetBrush(*wxTRANSPARENT_BRUSH);
		gc->DrawLines(m_pointList.size(), &m_pointList[0]);

		// Draw nodes.
		gc->SetPen(*wxTRANSPARENT_PEN);
		gc->SetBrush(wxBrush(wxColour(elementColour.GetDcRGBA())));
		if (m_pointList.size() > 0) {
			DrawDCCircle(m_pointList[0], 5.0, 10, gc);
			if (m_inserted) { DrawDCCircle(m_pointList[m_pointList.size() - 1], 5.0, 10, gc); }
		}

		DrawDCSwitches(gc);
		DrawDCPowerFlowPts(gc);

		// Push the current matrix on stack.
		gc->PushState();
		// Rotate the matrix around the object position.
		gc->Translate(m_position.m_x, m_position.m_y);
		gc->Rotate(wxDegToRad(m_angle));
		gc->Translate(-m_position.m_x, -m_position.m_y);

		glColor4d(1.0, 1.0, 1.0, 1.0);
		gc->SetPen(*wxTRANSPARENT_PEN);
		gc->SetBrush(*wxWHITE_BRUSH);
		DrawDCCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20, 20, gc);
		DrawDCCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20, 20, gc);

		gc->SetPen(wxPen(wxColour(elementColour.GetDcRGBA()), 2));
		gc->SetBrush(*wxTRANSPARENT_BRUSH);
		DrawDCCircle(m_rect.GetPosition() + wxPoint2DDouble(20.0, 20.0), 20, 20, gc);
		DrawDCCircle(m_rect.GetPosition() + wxPoint2DDouble(50.0, 20.0), 20, 20, gc);

		// Point
		gc->SetPen(*wxTRANSPARENT_PEN);
		gc->SetBrush(wxBrush(wxColour(elementColour.GetDcRGBA())));
		DrawDCCircle(m_rect.GetPosition(), 4, 10, gc);

		gc->PopState();
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
	if (m_angle >= 360 || m_angle <= -360) m_angle = 0.0;

	// Rotate all the points, except the switches and buses points.
	for (int i = 2; i < (int)m_pointList.size() - 2; i++) {
		m_pointList[i] = RotateAtPosition(m_pointList[i], rotAngle);
	}
	UpdateSwitchesPosition();
	UpdatePowerFlowArrowsPosition();
}

void Transformer::Move(wxPoint2DDouble position)
{
	SetPosition(m_movePos + position - m_moveStartPt);

	// Move all the points, except the switches and buses points.
	for (int i = 2; i < (int)m_pointList.size() - 2; i++) { m_pointList[i] = m_movePts[i] + position - m_moveStartPt; }

	if (!m_parentList[0]) { m_pointList[0] = m_movePts[0] + position - m_moveStartPt; }
	if (!m_parentList[1]) {
		m_pointList[m_pointList.size() - 1] = m_movePts[m_pointList.size() - 1] + position - m_moveStartPt;
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

bool Transformer::ShowForm(wxWindow* parent, Element* element)
{
	TransformerForm* transfForm = new TransformerForm(parent, this);
	if (transfForm->ShowModal() == wxID_OK) {
		transfForm->Destroy();
		return true;
	}
	transfForm->Destroy();
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

TransformerElectricalData Transformer::GetPUElectricalData(double systemBasePower)
{
	TransformerElectricalData data = m_electricalData;
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
	wxPoint2DDouble mid = (p1 + p2) / 2.0;
	StartMove(m_position);
	Move(mid);
	double bestAngle = wxRadToDeg(std::atan2(p2.m_y - p1.m_y, p2.m_x - p1.m_x));
	bool clockwise = bestAngle > 0 ? true : false;
	while (std::abs(m_angle) < std::abs(bestAngle)) { Rotate(clockwise); }
}
