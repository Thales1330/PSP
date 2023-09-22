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

#include "Line.h"

Line::Line() : Branch()
{
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) { m_electricalData.faultCurrent[i][j] = std::complex<double>(0.0, 0.0); }
	}
}

Line::Line(wxString name) : Branch()
{
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) { m_electricalData.faultCurrent[i][j] = std::complex<double>(0.0, 0.0); }
	}
	m_electricalData.name = name;
}
Line::~Line() {}
bool Line::Contains(wxPoint2DDouble position) const
{
	if (PointToLineDistance(position) < 5.0) { return true; }
	return false;
}

void Line::Draw(wxPoint2DDouble translation, double scale) const
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

	std::vector<wxPoint2DDouble> pointList = m_pointList;
	if (!m_inserted && pointList.size() > 0) {
		wxPoint2DDouble secondPoint = m_position;
		if (pointList.size() > 2) { secondPoint = pointList[2]; }
		pointList[1] = GetSwitchPoint(m_parentList[0], pointList[0], secondPoint);
		pointList.push_back(m_position);
	}

	// Line selected (Layer 1).
	if (m_selected) {
		glLineWidth(1.5 + m_borderSize * 2.0);
		glColor4dv(m_selectionColour.GetRGBA());
		DrawLine(pointList);

		// Draw nodes selection.
		if (pointList.size() > 0) {
			DrawCircle(pointList[0], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
			if (m_inserted) { DrawCircle(pointList[pointList.size() - 1], 5.0 + m_borderSize / scale, 10, GL_POLYGON); }
		}
	}

	// Draw line (Layer 2)
	glLineWidth(1.5);
	glColor4dv(elementColour.GetRGBA());
	DrawLine(pointList);

	if (m_inserted) {
		DrawSwitches();
		DrawPowerFlowPts();
	}

	// Draw nodes.
	if (pointList.size() > 0) {
		glColor4dv(elementColour.GetRGBA());
		DrawCircle(pointList[0], 5.0, 10, GL_POLYGON);
		if (m_inserted) { DrawCircle(pointList[pointList.size() - 1], 5.0, 10, GL_POLYGON); }
	}

	// Draw pickboxes (Layer 3).
	if (m_showPickbox) {
		glPushMatrix();
		glLoadIdentity();

		for (int i = 2; i < (int)m_pointList.size() - 2; i++) {
			DrawPickbox(WorldToScreen(m_pointList[i], translation, scale));
		}

		glPopMatrix();
	}
}

void Line::DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const
{
	//gc->SetBrush(*wxTRANSPARENT_BRUSH);

	wxGraphicsMatrix identityMatrix = gc->GetTransform();
	identityMatrix.Set(); // Set to identity

	OpenGLColour elementColour;
	if (m_online) {
		if (m_dynEvent)
			elementColour = m_dynamicEventColour;
		else
			elementColour = m_onlineElementColour;

	}
	else
		elementColour = m_offlineElementColour;

	std::vector<wxPoint2DDouble> pointList = m_pointList;
	if (!m_inserted && pointList.size() > 0) {
		wxPoint2DDouble secondPoint = m_position;
		if (pointList.size() > 2) { secondPoint = pointList[2]; }
		pointList[1] = GetSwitchPoint(m_parentList[0], pointList[0], secondPoint);
		pointList.push_back(m_position);
	}

	// Line selected (Layer 1).
	if (m_selected) {
		gc->SetPen(wxPen(wxColour(m_selectionColour.GetDcRGBA()), 2 + m_borderSize * 2.0));
		gc->SetBrush(*wxTRANSPARENT_BRUSH);
		if (pointList.size() > 0)
			gc->DrawLines(pointList.size(), &pointList[0]);

		// Draw nodes selection.
		gc->SetPen(*wxTRANSPARENT_PEN);
		gc->SetBrush(wxBrush(wxColour(m_selectionColour.GetDcRGBA())));
		if (pointList.size() > 0) {
			DrawDCCircle(pointList[0], 5.0 + m_borderSize / scale, 10, gc);
			if (m_inserted) { DrawDCCircle(pointList[pointList.size() - 1], 5.0 + m_borderSize / scale, 10, gc); }
		}
	}

	// Draw line (Layer 2)
	gc->SetPen(wxPen(wxColour(elementColour.GetDcRGBA()), 2));
	gc->SetBrush(*wxTRANSPARENT_BRUSH);
	if (pointList.size() > 0)
		gc->DrawLines(pointList.size(), &pointList[0]);

	if (m_inserted) {
		DrawDCSwitches(gc);
		DrawDCPowerFlowPts(gc);
	}

	// Draw nodes.
	gc->SetPen(*wxTRANSPARENT_PEN);
	gc->SetBrush(wxBrush(wxColour(elementColour.GetDcRGBA())));
	if (pointList.size() > 0) {
		DrawDCCircle(pointList[0], 5.0, 10, gc);
		if (m_inserted) { DrawDCCircle(pointList[pointList.size() - 1], 5.0, 10, gc); }
	}

	// Draw pickboxes (Layer 3).
	if (m_showPickbox) {
		gc->PushState();
		gc->SetTransform(identityMatrix);

		for (int i = 2; i < (int)m_pointList.size() - 2; i++) {
			DrawDCPickbox(WorldToScreen(m_pointList[i], translation - wxPoint2DDouble(4 / scale, 4 / scale), scale), gc);
		}

		gc->PopState();
	}
}

void Line::Move(wxPoint2DDouble position)
{
	if (!m_parentList[0]) {
		m_pointList[0] = m_movePts[0] + position - m_moveStartPt;
		UpdateSwitchesPosition();
		UpdatePowerFlowArrowsPosition();
	}
	if (!m_parentList[1]) {
		m_pointList[m_pointList.size() - 1] = m_movePts[m_pointList.size() - 1] + position - m_moveStartPt;
		UpdateSwitchesPosition();
		UpdatePowerFlowArrowsPosition();
	}

	if (!m_parentList[0] && !m_parentList[1]) {
		for (int i = 2; i < (int)m_pointList.size() - 2; i++) {
			m_pointList[i] = m_movePts[i] + position - m_moveStartPt;
		}
	}
}

bool Line::AddParent(Element* parent, wxPoint2DDouble position)
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
			UpdateSwitches();

			Bus* parentBus = static_cast<Bus*>(parent);
			m_electricalData.nominalVoltage = parentBus->GetElectricalData().nominalVoltage;
			m_electricalData.nominalVoltageUnit = parentBus->GetElectricalData().nominalVoltageUnit;

			return false;
		}
		// Second bus.
		else if (parent != m_parentList[0]) {
			Bus* parentBus = static_cast<Bus*>(parent);
			if (m_electricalData.nominalVoltage != parentBus->GetElectricalData().nominalVoltage ||
				m_electricalData.nominalVoltageUnit != parentBus->GetElectricalData().nominalVoltageUnit) {
				wxMessageDialog msgDialog(nullptr,
					_("Unable to connect two buses with different nominal voltages.\n"
						"Use a transformer or edit the bus properties."),
					_("Error"), wxOK | wxCENTRE | wxICON_ERROR);
				msgDialog.ShowModal();
				return false;
			}

			m_parentList.push_back(parent);
			parent->AddChild(this);
			wxPoint2DDouble parentPt =
				parent->RotateAtPosition(position, -parent->GetAngle());        // Rotate click to horizontal position.
			parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
			parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.

			// Set first switch point.
			wxPoint2DDouble secondPoint = parentPt;
			if (m_pointList.size() > 2) { secondPoint = m_pointList[2]; }
			m_pointList[1] = GetSwitchPoint(m_parentList[0], m_pointList[0], secondPoint);

			// Set the second switch point.
			m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_pointList[m_pointList.size() - 1]));

			m_pointList.push_back(parentPt);  // Last point.

			wxRect2DDouble genRect(0, 0, 0, 0);
			m_switchRect.push_back(genRect);
			UpdateSwitches();

			m_inserted = true;
			UpdatePowerFlowArrowsPosition();
			return true;
		}
	}
	return false;
}
bool Line::Intersects(wxRect2DDouble rect) const
{
	for (auto it = m_pointList.begin(); it != m_pointList.end(); ++it) {
		if (rect.Contains(*it)) return true;
	}
	return false;
}
void Line::MovePickbox(wxPoint2DDouble position)
{
	if (m_activePickboxID == ID_PB_NONE) return;

	for (int i = 2; i < (int)m_pointList.size() - 2; i++) {
		if (m_activePickboxID == i) {
			m_pointList[i] = m_movePts[i] + position - m_moveStartPt;
			UpdateSwitchesPosition();
			UpdatePowerFlowArrowsPosition();
		}
	}
}
bool Line::PickboxContains(wxPoint2DDouble position)
{
	for (int i = 2; i < (int)m_pointList.size() - 2; i++) {
		wxRect2DDouble rect(m_pointList[i].m_x - 5.0, m_pointList[i].m_y - 5.0, 10.0, 10.0);
		if (rect.Contains(position)) {
			m_activePickboxID = i;
			return true;
		}
	}
	return false;
}

void Line::AddPoint(wxPoint2DDouble point)
{
	if (m_parentList.size() != 0) { m_pointList.push_back(point); }
}

void Line::StartMove(wxPoint2DDouble position)
{
	m_moveStartPt = position;
	m_movePts = m_pointList;
}

void Line::MoveNode(Element* parent, wxPoint2DDouble position)
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

		// If the line is selected, move all the points, except the switches and buses points.
		if (m_selected) {
			for (int i = 2; i < (int)m_pointList.size() - 1; i++) {
				m_pointList[i] = m_movePts[i] + position - m_moveStartPt;
			}
		}
	}
	else {
		// If parent is setted to nullptr for the firts time, remove the parent child
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

bool Line::GetContextMenu(wxMenu& menu)
{
	wxFileName exeFileName(wxStandardPaths::Get().GetExecutablePath());
	wxString exePath = exeFileName.GetPath();

	menu.Append(ID_EDIT_ELEMENT, _("Edit line"));
	if (m_activePickboxID == ID_PB_NONE) {
		wxMenuItem* addNodeItem = new wxMenuItem(&menu, ID_LINE_ADD_NODE, _("Insert node"));
		addNodeItem->SetBitmap(
			wxImage(exePath + wxFileName::DirName("\\..\\data\\images\\menu\\addNode16.png", wxPATH_WIN).GetPath()));
		menu.Append(addNodeItem);
	}
	else {
		wxMenuItem* addNodeItem = new wxMenuItem(&menu, ID_LINE_REMOVE_NODE, _("Remove node"));
		addNodeItem->SetBitmap(
			wxImage(exePath + wxFileName::DirName("\\..\\data\\images\\menu\\removeNode16.png", wxPATH_WIN).GetPath()));
		menu.Append(addNodeItem);
	}
	wxMenuItem* deleteItem = new wxMenuItem(&menu, ID_DELETE, _("Delete"));
	deleteItem->SetBitmap(
		wxImage(exePath + wxFileName::DirName("\\..\\data\\images\\menu\\delete16.png", wxPATH_WIN).GetPath()));
	menu.Append(deleteItem);
	return true;
}

void Line::RemoveNode(wxPoint2DDouble point)
{
	if (PickboxContains(point)) {
		for (int i = 2; i < (int)m_pointList.size() - 2; i++) {
			if (m_activePickboxID == i) {
				m_pointList.erase(m_pointList.begin() + i);
				break;
			}
		}
	}
	UpdateSwitchesPosition();
	UpdatePowerFlowArrowsPosition();
}

void Line::AddNode(wxPoint2DDouble point)
{
	int segmentNumber = 0;
	PointToLineDistance(point, &segmentNumber);
	if (segmentNumber > 0 && segmentNumber < (int)m_pointList.size() - 2) {
		m_pointList.insert(m_pointList.begin() + segmentNumber + 1, point);
	}
	UpdateSwitchesPosition();
	UpdatePowerFlowArrowsPosition();
}

void Line::CalculateBoundaries(wxPoint2DDouble& leftUp, wxPoint2DDouble& rightBottom) const
{
	if (m_pointList.size() > 0) {
		// Check points list boundaries.
		leftUp = m_pointList[0];
		rightBottom = m_pointList[0];
		for (int i = 1; i < (int)m_pointList.size(); i++) {
			if (m_pointList[i].m_x < leftUp.m_x) leftUp.m_x = m_pointList[i].m_x;
			if (m_pointList[i].m_y < leftUp.m_y) leftUp.m_y = m_pointList[i].m_y;
			if (m_pointList[i].m_x > rightBottom.m_x) rightBottom.m_x = m_pointList[i].m_x;
			if (m_pointList[i].m_y > rightBottom.m_y) rightBottom.m_y = m_pointList[i].m_y;
		}
	}
}

bool Line::ShowForm(wxWindow* parent, Element* element)
{
	LineForm* lineForm = new LineForm(parent, this);
	if (lineForm->ShowModal() == wxID_OK) {
		lineForm->Destroy();
		return true;
	}
	lineForm->Destroy();
	return false;
}

void Line::SetNominalVoltage(std::vector<double> nominalVoltage, std::vector<ElectricalUnit> nominalVoltageUnit)
{
	if (nominalVoltage.size() > 0) {
		m_electricalData.nominalVoltage = nominalVoltage[0];
		m_electricalData.nominalVoltageUnit = nominalVoltageUnit[0];
	}
}

bool Line::SetNodeParent(Element* parent)
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
			// If the line has no parents set the new rated voltage, otherwise check if it's not connecting
			// two different voltages buses
			Bus* parentBus = static_cast<Bus*>(parent);
			if (!m_parentList[0] && !m_parentList[1]) {
				m_electricalData.nominalVoltage = parentBus->GetElectricalData().nominalVoltage;
				m_electricalData.nominalVoltageUnit = parentBus->GetElectricalData().nominalVoltageUnit;
			}
			else if (m_electricalData.nominalVoltage != parentBus->GetElectricalData().nominalVoltage ||
				m_electricalData.nominalVoltageUnit != parentBus->GetElectricalData().nominalVoltageUnit) {
				wxMessageDialog msgDialog(nullptr,
					_("Unable to connect two buses with different nominal voltages.\n"
						"Use a transformer or edit the bus properties."),
					_("Error"), wxOK | wxCENTRE | wxICON_ERROR);
				msgDialog.ShowModal();
				m_activeNodeID = 0;
				return false;
			}

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

void Line::SetPowerFlowDirection(PowerFlowDirection pfDirection)
{
	m_pfDirection = pfDirection;
	UpdatePowerFlowArrowsPosition();
}

void Line::UpdatePowerFlowArrowsPosition()
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

void Line::RotateNode(Element* parent, bool clockwise)
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

void Line::SetPointList(std::vector<wxPoint2DDouble> pointList)
{
	m_pointList = pointList;
	UpdateSwitchesPosition();
	UpdatePowerFlowArrowsPosition();
}

Element* Line::GetCopy()
{
	Line* copy = new Line();
	*copy = *this;
	return copy;
}

wxString Line::GetTipText() const
{
	wxString tipText = m_electricalData.name;

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

LineElectricalData Line::GetPUElectricalData(double systemBasePower)
{
	LineElectricalData data = m_electricalData;
	double lineBasePower = GetValueFromUnit(data.nominalPower, data.nominalPowerUnit);
	double baseVoltage = GetValueFromUnit(data.nominalVoltage, data.nominalVoltageUnit);
	double systemBaseImpedance = (baseVoltage * baseVoltage) / systemBasePower;
	double lineBaseImpedance = (baseVoltage * baseVoltage) / lineBasePower;

	// Resistance
	double r = data.resistance;
	if (data.resistanceUnit == ElectricalUnit::UNIT_OHM_km) r *= data.lineSize;
	if (data.resistanceUnit == ElectricalUnit::UNIT_PU) {
		if (data.useLinePower) data.resistance = (r * lineBaseImpedance) / systemBaseImpedance;
	}
	else {
		data.resistance = r / systemBaseImpedance;
	}
	data.resistanceUnit = ElectricalUnit::UNIT_PU;

	// Inductive reactance
	double x = data.indReactance;
	if (data.indReactanceUnit == ElectricalUnit::UNIT_OHM_km) x *= data.lineSize;
	if (data.indReactanceUnit == ElectricalUnit::UNIT_PU) {
		if (data.useLinePower) data.indReactance = (x * lineBaseImpedance) / systemBaseImpedance;
	}
	else {
		data.indReactance = x / systemBaseImpedance;
	}
	data.indReactanceUnit = ElectricalUnit::UNIT_PU;

	// Capacitive susceptance
	double b = data.capSusceptance;
	if (data.capSusceptanceUnit == ElectricalUnit::UNIT_OHM_km) b *= data.lineSize;
	if (data.capSusceptanceUnit == ElectricalUnit::UNIT_PU) {
		if (data.useLinePower) data.capSusceptance = (b * lineBaseImpedance) / systemBaseImpedance;
	}
	else {
		data.capSusceptance = b / systemBaseImpedance;
	}
	data.capSusceptanceUnit = ElectricalUnit::UNIT_PU;

	// Fault

	// Zero seq. resistance
	double r0 = data.zeroResistance;
	if (data.useLinePower) data.zeroResistance = (r0 * lineBaseImpedance) / systemBaseImpedance;

	// Zero seq. ind. reactance
	double x0 = data.zeroIndReactance;
	if (data.useLinePower) data.zeroIndReactance = (x0 * lineBaseImpedance) / systemBaseImpedance;

	// Zero seq. cap. susceptance
	double b0 = data.zeroCapSusceptance;
	if (data.useLinePower) data.zeroCapSusceptance = (b0 * lineBaseImpedance) / systemBaseImpedance;

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

rapidxml::xml_node<>* Line::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
	auto elementNode = XMLParser::AppendNode(doc, elementListNode, "Line");
	XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);
	auto cadProp = XMLParser::AppendNode(doc, elementNode, "CADProperties");
	auto nodeList = XMLParser::AppendNode(doc, cadProp, "NodeList");
	int nodeID = 0;
	// Parse all the points.
	for (unsigned int i = 0; i < m_pointList.size(); i++) {
		// Don't save switch points, the method UpdateSwitchesPosition() calculate these points properly after open the
		// element
		if ((i != 1) && (i != m_pointList.size() - 2)) {
			auto nodePos = XMLParser::AppendNode(doc, nodeList, "Node");
			XMLParser::SetNodeAttribute(doc, nodePos, "ID", nodeID);
			auto nodePosX = XMLParser::AppendNode(doc, nodePos, "X");
			XMLParser::SetNodeValue(doc, nodePosX, m_pointList[i].m_x);
			auto nodePosY = XMLParser::AppendNode(doc, nodePos, "Y");
			XMLParser::SetNodeValue(doc, nodePosY, m_pointList[i].m_y);
			nodeID++;
		}
	}

	auto parentIDList = XMLParser::AppendNode(doc, cadProp, "ParentIDList");
	for (unsigned int i = 0; i < m_parentList.size(); i++) {
		if (m_parentList[i]) {
			auto parentID = XMLParser::AppendNode(doc, parentIDList, "ParentID");
			XMLParser::SetNodeAttribute(doc, parentID, "ID", static_cast<int>(i));
			XMLParser::SetNodeValue(doc, parentID, m_parentList[i]->GetID());
		}
	}

	auto electricalProp = XMLParser::AppendNode(doc, elementNode, "ElectricalProperties");
	auto isOnline = XMLParser::AppendNode(doc, electricalProp, "IsOnline");
	XMLParser::SetNodeValue(doc, isOnline, m_online);
	auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
	XMLParser::SetNodeValue(doc, name, m_electricalData.name);
	auto nominalVoltage = XMLParser::AppendNode(doc, electricalProp, "NominalVoltage");
	XMLParser::SetNodeValue(doc, nominalVoltage, m_electricalData.nominalVoltage);
	XMLParser::SetNodeAttribute(doc, nominalVoltage, "UnitID", static_cast<int>(m_electricalData.nominalVoltageUnit));
	auto nominalPower = XMLParser::AppendNode(doc, electricalProp, "NominalPower");
	XMLParser::SetNodeValue(doc, nominalPower, m_electricalData.nominalPower);
	XMLParser::SetNodeAttribute(doc, nominalPower, "UnitID", static_cast<int>(m_electricalData.nominalPowerUnit));
	auto resistance = XMLParser::AppendNode(doc, electricalProp, "Resistance");
	XMLParser::SetNodeValue(doc, resistance, m_electricalData.resistance);
	XMLParser::SetNodeAttribute(doc, resistance, "UnitID", static_cast<int>(m_electricalData.resistanceUnit));
	auto indReactance = XMLParser::AppendNode(doc, electricalProp, "IndReactance");
	XMLParser::SetNodeValue(doc, indReactance, m_electricalData.indReactance);
	XMLParser::SetNodeAttribute(doc, indReactance, "UnitID", static_cast<int>(m_electricalData.indReactanceUnit));
	auto capSusceptance = XMLParser::AppendNode(doc, electricalProp, "CapSusceptance");
	XMLParser::SetNodeValue(doc, capSusceptance, m_electricalData.capSusceptance);
	XMLParser::SetNodeAttribute(doc, capSusceptance, "UnitID", static_cast<int>(m_electricalData.capSusceptanceUnit));
	auto lineSize = XMLParser::AppendNode(doc, electricalProp, "LineSize");
	XMLParser::SetNodeValue(doc, lineSize, m_electricalData.lineSize);
	auto useLinePower = XMLParser::AppendNode(doc, electricalProp, "UseLinePower");
	XMLParser::SetNodeValue(doc, useLinePower, m_electricalData.useLinePower);

	auto fault = XMLParser::AppendNode(doc, electricalProp, "Fault");
	auto zeroResistance = XMLParser::AppendNode(doc, fault, "ZeroResistance");
	XMLParser::SetNodeValue(doc, zeroResistance, m_electricalData.zeroResistance);
	auto zeroIndReactance = XMLParser::AppendNode(doc, fault, "ZeroIndReactance");
	XMLParser::SetNodeValue(doc, zeroIndReactance, m_electricalData.zeroIndReactance);
	auto zeroCapSusceptance = XMLParser::AppendNode(doc, fault, "ZeroCapSusceptance");
	XMLParser::SetNodeValue(doc, zeroCapSusceptance, m_electricalData.zeroCapSusceptance);

	SaveSwitchingData(doc, electricalProp);

	return elementNode;
}

bool Line::OpenElement(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList)
{
	auto cadPropNode = elementNode->first_node("CADProperties");
	if (!cadPropNode) return false;

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
		wxString(parentNode->first_attribute("ID")->value()).ToCLong(&index);
		wxString(parentNode->value()).ToCLong(&parentID[index]);
		parentNode = parentNode->next_sibling("ParentID");
	}

	std::vector<wxPoint2DDouble> nodePtsList;            // List of node points
	nodePtsList.push_back(ptsList[0]);                   // First point on the list
	nodePtsList.push_back(ptsList[ptsList.size() - 1]);  // Last point on the list

	// List of dummy buses to set not connected nodes properly
	std::vector<Bus*> dummyBusList;
	for (unsigned int i = 0; i < nodePtsList.size(); ++i) {
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

	// Add the others nodes (if exists)
	std::vector<wxPoint2DDouble> midPts;
	for (unsigned int i = 1; i < ptsList.size() - 1; i++) midPts.push_back(ptsList[i]);
	m_pointList.insert(m_pointList.begin() + 2, midPts.begin(), midPts.end());
	SetPointList(m_pointList);

	// Remove dummy buses
	for (auto it = dummyBusList.begin(), itEnd = dummyBusList.end(); it != itEnd; ++it) {
		RemoveParent(*it);
		delete* it;
	}
	dummyBusList.clear();

	auto electricalProp = elementNode->first_node("ElectricalProperties");
	if (!electricalProp) return false;

	SetOnline(XMLParser::GetNodeValueInt(electricalProp, "IsOnline"));
	m_electricalData.name = electricalProp->first_node("Name")->value();
	m_electricalData.nominalVoltage = XMLParser::GetNodeValueDouble(electricalProp, "NominalVoltage");
	m_electricalData.nominalVoltageUnit =
		static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "NominalVoltage", "UnitID"));
	m_electricalData.nominalPower = XMLParser::GetNodeValueDouble(electricalProp, "NominalPower");
	m_electricalData.nominalPowerUnit =
		static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "NominalPower", "UnitID"));
	m_electricalData.resistance = XMLParser::GetNodeValueDouble(electricalProp, "Resistance");
	m_electricalData.resistanceUnit =
		static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "Resistance", "UnitID"));
	m_electricalData.indReactance = XMLParser::GetNodeValueDouble(electricalProp, "IndReactance");
	m_electricalData.indReactanceUnit =
		static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "IndReactance", "UnitID"));
	m_electricalData.capSusceptance = XMLParser::GetNodeValueDouble(electricalProp, "CapSusceptance");
	m_electricalData.capSusceptanceUnit =
		static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "CapSusceptance", "UnitID"));
	m_electricalData.lineSize = XMLParser::GetNodeValueDouble(electricalProp, "LineSize");
	m_electricalData.useLinePower = XMLParser::GetNodeValueInt(electricalProp, "UseLinePower");

	auto fault = electricalProp->first_node("Fault");
	m_electricalData.zeroResistance = XMLParser::GetNodeValueDouble(fault, "ZeroResistance");
	m_electricalData.zeroIndReactance = XMLParser::GetNodeValueDouble(fault, "ZeroIndReactance");
	m_electricalData.zeroCapSusceptance = XMLParser::GetNodeValueDouble(fault, "ZeroCapSusceptance");

	if (!OpenSwitchingData(electricalProp)) return false;
	if (m_swData.swTime.size() != 0) SetDynamicEvent(true);
	return true;
}
