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

#include "Bus.h"
#ifdef USING_WX_3_0_X
#include "DegreesAndRadians.h"
#endif

Bus::Bus() : PowerElement() {}
Bus::Bus(wxPoint2DDouble position) : PowerElement()
{
	m_width = 100.0;
	m_height = 5.0;
	SetPosition(position);
}

Bus::Bus(wxPoint2DDouble position, wxString name)
{
	m_width = 100.0;
	m_height = 5.0;
	SetPosition(position);

	m_electricalData.name = name;
}

Bus::~Bus() {}
void Bus::Draw(wxPoint2DDouble translation, double scale) const
{
	// Draw selection (layer 1)
	if (m_selected) {
		// If the object is selected, the matrix is reset to remove scale effects applied to it, thus keeping the
		// edges with fixed sizes for all zoom levels.
		glPushMatrix();
		glLoadIdentity();
		// The matrix was reset, so we must use screen coordinates (WorldToScreen).
		wxPoint2DDouble screenPt = WorldToScreen(translation, scale);
		glTranslated(screenPt.m_x, screenPt.m_y, 0.0);
		glRotated(m_angle, 0.0, 0.0, 1.0);
		glTranslated(-screenPt.m_x, -screenPt.m_y, 0.0);

		glColor4dv(m_selectionColour.GetRGBA());

		wxPoint2DDouble pts[4] = { WorldToScreen(translation, scale, -(m_width / 2.0), -(m_height / 2.0)) -
									  wxPoint2DDouble(m_borderSize, m_borderSize),
								  WorldToScreen(translation, scale, -(m_width / 2.0), (m_height / 2.0)) -
									  wxPoint2DDouble(m_borderSize, -m_borderSize),
								  WorldToScreen(translation, scale, (m_width / 2.0), (m_height / 2.0)) -
									  wxPoint2DDouble(-m_borderSize, -m_borderSize),
								  WorldToScreen(translation, scale, (m_width / 2.0), -(m_height / 2.0)) -
									  wxPoint2DDouble(-m_borderSize, m_borderSize) };
		DrawRectangle(pts);
		glPopMatrix();
	}
	// Draw bus (layer 2)
	// Push the current matrix on stack.
	glPushMatrix();
	// Rotate the matrix around the object position.
	glTranslated(m_position.m_x, m_position.m_y, 0.0);
	glRotated(m_angle, 0.0, 0.0, 1.0);
	glTranslated(-m_position.m_x, -m_position.m_y, 0.0);


	if (!m_electricalData.isConnected)
		glColor4dv(m_offlineElementColour.GetRGBA());
	else if (m_dynEvent)
		glColor4dv(m_dynamicEventColour.GetRGBA());
	else
		glColor4dv(m_busColour.GetRGBA());


	DrawRectangle(m_position, m_width, m_height);
	// Pop the old matrix back.
	glPopMatrix();

	// Draw pickbox (layer 3)
	if (m_showPickbox) {
		glPushMatrix();
		glLoadIdentity();

		wxPoint2DDouble screenPt = WorldToScreen(translation, scale);
		glTranslated(screenPt.m_x, screenPt.m_y, 0.0);
		glRotated(m_angle, 0.0, 0.0, 1.0);
		glTranslated(-screenPt.m_x, -screenPt.m_y, 0.0);

		wxPoint2DDouble pbPosition[2] = { WorldToScreen(translation, scale, m_width / 2.0),
										 WorldToScreen(translation, scale, -m_width / 2.0) };
		DrawPickbox(pbPosition[0]);
		DrawPickbox(pbPosition[1]);

		glPopMatrix();
	}

	// Draw faulted bus symbol (layer 4)
	if (m_electricalData.hasFault) {
		glPushMatrix();
		glLoadIdentity();

		wxPoint2DDouble screenPt = WorldToScreen(translation, scale);
		glTranslated(screenPt.m_x, screenPt.m_y, 0.0);
		glRotated(m_angle, 0.0, 0.0, 1.0);
		glTranslated(-screenPt.m_x, -screenPt.m_y, 0.0);

		wxPoint2DDouble fsPosition = WorldToScreen(translation, scale, + m_width / 2.0);

		OpenGLColour faultSymbolColour(1.0, 0.0, 0.0, 1.0);
		glColor4dv(faultSymbolColour.GetRGBA());
		
		double scale = 1.5;
		glBegin(GL_POLYGON);
		glVertex2f(fsPosition.m_x + 1 * scale, fsPosition.m_y + 3 * scale);
		glVertex2f(fsPosition.m_x + 1 * scale, fsPosition.m_y - 3 * scale);
		glVertex2f(fsPosition.m_x + 11 * scale, fsPosition.m_y - 1 * scale);
		glVertex2f(fsPosition.m_x + 11 * scale, fsPosition.m_y + 5 * scale);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(fsPosition.m_x + 7 * scale, fsPosition.m_y - 5 * scale);
		glVertex2f(fsPosition.m_x + 21 * scale, fsPosition.m_y + 3 * scale);
		glVertex2f(fsPosition.m_x + 7 * scale, fsPosition.m_y + 1 * scale);

		glEnd();


		glPopMatrix();
	}
}

void Bus::DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const
{
	wxPoint2DDouble gcPosition = m_position - wxPoint2DDouble(m_width / 2.0, m_height / 2.0);
	wxGraphicsMatrix identityMatrix = gc->GetTransform();
	identityMatrix.Set(); // Set to identity

	gc->SetPen(*wxTRANSPARENT_PEN);

	// Draw selection (layer 1)
	if (m_selected) {
		// If the object is selected, the matrix is reset to remove scale effects applied to it, thus keeping the
		// edges with fixed sizes for all zoom levels.
		gc->PushState();
		gc->SetTransform(identityMatrix);
		// The matrix was reset, so we must use screen coordinates (WorldToScreen).
		wxPoint2DDouble screenPt = WorldToScreen(translation, scale);
		gc->Translate(screenPt.m_x, screenPt.m_y);
		gc->Rotate(wxDegToRad(m_angle));
		gc->Translate(-screenPt.m_x, -screenPt.m_y);

		gc->SetBrush(wxBrush(m_selectionColour.GetDcRGBA()));

		wxPoint2DDouble pts[4] = { WorldToScreen(translation, scale, -(m_width / 2.0), -(m_height / 2.0)) -
									  wxPoint2DDouble(m_borderSize, m_borderSize),
								  WorldToScreen(translation, scale, -(m_width / 2.0), (m_height / 2.0)) -
									  wxPoint2DDouble(m_borderSize, -m_borderSize),
								  WorldToScreen(translation, scale, (m_width / 2.0), (m_height / 2.0)) -
									  wxPoint2DDouble(-m_borderSize, -m_borderSize),
								  WorldToScreen(translation, scale, (m_width / 2.0), -(m_height / 2.0)) -
									  wxPoint2DDouble(-m_borderSize, m_borderSize) };
		gc->DrawLines(4, pts);
		gc->PopState();
	}

	gc->PushState();
	gc->Translate(m_position.m_x, m_position.m_y);
	gc->Rotate(wxDegToRad(m_angle));
	gc->Translate(-m_position.m_x, -m_position.m_y);


	if (!m_electricalData.isConnected)
		gc->SetBrush(wxBrush(m_offlineElementColour.GetDcRGBA()));
	else if (m_dynEvent)
		gc->SetBrush(wxBrush(m_dynamicEventColour.GetDcRGBA()));
	else
		gc->SetBrush(wxBrush(m_busColour.GetDcRGBA()));

	gc->DrawRectangle(gcPosition.m_x, gcPosition.m_y, m_width, m_height);

	gc->PopState();

	// Draw pickbox (layer 3)
	if (m_showPickbox) {
		gc->PushState();
		gc->SetTransform(identityMatrix);

		wxPoint2DDouble screenPt = WorldToScreen(translation, scale);
		gc->Translate(screenPt.m_x, screenPt.m_y);
		gc->Rotate(wxDegToRad(m_angle));
		gc->Translate(-screenPt.m_x, -screenPt.m_y);

		wxPoint2DDouble pbPosition[2] = { WorldToScreen(translation, scale, m_width / 2.0) - wxPoint2DDouble(4, 4),
										 WorldToScreen(translation, scale, -m_width / 2.0) - wxPoint2DDouble(4, 4) };
		DrawDCPickbox(pbPosition[0], gc);
		DrawDCPickbox(pbPosition[1], gc);

		gc->PopState();
	}

	// Draw faulted bus symbol (layer 4)
	if (m_electricalData.hasFault) {
		gc->PushState();
		gc->SetTransform(identityMatrix);

		wxPoint2DDouble screenPt = WorldToScreen(translation, scale);
		gc->Translate(screenPt.m_x, screenPt.m_y);
		gc->Rotate(wxDegToRad(m_angle));
		gc->Translate(-screenPt.m_x, -screenPt.m_y);

		wxPoint2DDouble fsPosition = WorldToScreen(translation, scale, m_width / 2.0);

		OpenGLColour faultSymbolColour(1.0, 0.0, 0.0, 1.0);
		glColor4dv(faultSymbolColour.GetRGBA());

		double scale = 1.5;
		/*glBegin(GL_POLYGON);
		glVertex2f(fsPosition.m_x + 1 * scale, fsPosition.m_y + 3 * scale);
		glVertex2f(fsPosition.m_x + 1 * scale, fsPosition.m_y - 3 * scale);
		glVertex2f(fsPosition.m_x + 11 * scale, fsPosition.m_y - 1 * scale);
		glVertex2f(fsPosition.m_x + 11 * scale, fsPosition.m_y + 5 * scale);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(fsPosition.m_x + 7 * scale, fsPosition.m_y - 5 * scale);
		glVertex2f(fsPosition.m_x + 21 * scale, fsPosition.m_y + 3 * scale);
		glVertex2f(fsPosition.m_x + 7 * scale, fsPosition.m_y + 1 * scale);*/

		gc->SetBrush(*wxRED_BRUSH);
		gc->SetPen(*wxTRANSPARENT_PEN);

		wxPoint2DDouble* points = new wxPoint2DDouble[4];
		points[0] = wxPoint2DDouble(fsPosition.m_x + 1 * scale, fsPosition.m_y + 3 * scale);
		points[1] = wxPoint2DDouble(fsPosition.m_x + 1 * scale, fsPosition.m_y - 3 * scale);
		points[2] = wxPoint2DDouble(fsPosition.m_x + 11 * scale, fsPosition.m_y - 1 * scale);
		points[3] = wxPoint2DDouble(fsPosition.m_x + 11 * scale, fsPosition.m_y + 5 * scale);

		gc->DrawLines(4, points);

		points[0] = wxPoint2DDouble(fsPosition.m_x + 7 * scale, fsPosition.m_y - 5 * scale);
		points[1] = wxPoint2DDouble(fsPosition.m_x + 21 * scale, fsPosition.m_y + 3 * scale);
		points[2] = wxPoint2DDouble(fsPosition.m_x + 7 * scale, fsPosition.m_y + 1 * scale);

		gc->DrawLines(3, points);

		delete[] points;

		gc->PopState();
	}
}

bool Bus::Contains(wxPoint2DDouble position) const
{
	wxPoint2DDouble ptR = RotateAtPosition(position, -m_angle);
	return m_rect.Contains(ptR);
}

bool Bus::Intersects(wxRect2DDouble rect) const
{
	if (m_angle == 0.0 || m_angle == 180.0) return m_rect.Intersects(rect);

	return RotatedRectanglesIntersects(m_rect, rect, m_angle, 0.0);
}

bool Bus::PickboxContains(wxPoint2DDouble position)
{
	m_activePickboxID = ID_PB_NONE;

	wxPoint2DDouble ptR = RotateAtPosition(position, -m_angle);

	wxPoint2DDouble center(m_position.m_x + m_width / 2.0, m_position.m_y);
	wxRect2DDouble rectRight(center.m_x - 5.0, center.m_y - 5.0, 10.0, 10.0);

	center = wxPoint2DDouble(m_position.m_x - m_width / 2.0, m_position.m_y);
	wxRect2DDouble rectLeft(center.m_x - 5.0, center.m_y - 5.0, 10.0, 10.0);

	if (rectRight.Contains(ptR)) {
		m_activePickboxID = ID_PB_RIGHT;
		return true;
	}
	if (rectLeft.Contains(ptR)) {
		m_activePickboxID = ID_PB_LEFT;
		return true;
	}

	return false;
}

wxCursor Bus::GetBestPickboxCursor() const
{
	double angle = m_angle;
	while (angle >= 157.5) angle -= 180.0;

	if (angle >= -22.5 && angle < 22.5)
		return wxCursor(wxCURSOR_SIZEWE);
	else if (angle >= 22.5 && angle < 67.5)
		return wxCursor(wxCURSOR_SIZENWSE);
	else if (angle >= 67.5 && angle < 112.5)
		return wxCursor(wxCURSOR_SIZENS);
	else if (angle >= 112.5 && angle < 157.5)
		return wxCursor(wxCURSOR_SIZENESW);

	return wxCursor(wxCURSOR_ARROW);
}

void Bus::MovePickbox(wxPoint2DDouble position)
{
	if (m_activePickboxID == ID_PB_NONE) return;

	wxPoint2DDouble ptR = RotateAtPosition(position, -m_angle);

	double dx = 0.0;
	if (m_activePickboxID == ID_PB_RIGHT)
		dx = ptR.m_x - m_position.m_x - m_width / 2.0;
	else if (m_activePickboxID == ID_PB_LEFT)
		dx = m_position.m_x - m_width / 2.0 - ptR.m_x;

	if (m_width + dx < 20.0) return;

	if (m_activePickboxID == ID_PB_RIGHT) {
		m_position.m_x += (dx / 2.0) * std::cos(wxDegToRad(m_angle));
		m_position.m_y += (dx / 2.0) * std::sin(wxDegToRad(m_angle));
	}
	else if (m_activePickboxID == ID_PB_LEFT) {
		m_position.m_x -= (dx / 2.0) * std::cos(wxDegToRad(m_angle));
		m_position.m_y -= (dx / 2.0) * std::sin(wxDegToRad(m_angle));
	}
	m_width += dx;

	SetPosition(m_position);
}

void Bus::Rotate(bool clockwise)
{
	double rotAngle = m_rotationAngle;
	if (!clockwise) rotAngle = -m_rotationAngle;

	m_angle += rotAngle;
	if (m_angle >= 360 || m_angle <= -360) m_angle = 0.0;
}

bool Bus::GetContextMenu(wxMenu& menu)
{
	menu.Append(ID_EDIT_ELEMENT, _("Edit bus"));
	GeneralMenuItens(menu);
	return true;
}

bool Bus::ShowForm(wxWindow* parent, Element* element)
{
	BusForm* busForm = new BusForm(parent, this);
	if (busForm->ShowModal() == wxID_OK) {
		busForm->Destroy();
		return true;
	}

	busForm->Destroy();
	return false;
}

Element* Bus::GetCopy()
{
	Bus* copy = new Bus();
	*copy = *this;
	return copy;
}
wxString Bus::GetTipText() const
{
	wxString tipText = m_electricalData.name;
	tipText += wxString::Format(" (%d)", m_electricalData.number + 1);
	tipText += "\n";
	tipText += StringFromDouble(m_electricalData.nominalVoltage, 1) +
		(m_electricalData.nominalVoltageUnit == ElectricalUnit::UNIT_V
			? _(" V")
			: _(" kV"));
	tipText += "\n";
	tipText += _("\nV = ") + wxString::FromDouble(std::abs(m_electricalData.voltage), 5) + _(" p.u.");
	tipText += "\n";
	tipText += wxString(L'\u03B8') + " = " + wxString::FromDouble(wxRadToDeg(std::arg(m_electricalData.voltage)), 5)
		+ wxString(L'\u00B0');

	tipText += _("\n\nFault info:");
	tipText += _("\nVa = ") + wxString::FromDouble(std::abs(m_electricalData.faultVoltage[0]), 5) + _(" p.u.");
	tipText += _("\nVb = ") + wxString::FromDouble(std::abs(m_electricalData.faultVoltage[1]), 5) + _(" p.u.");
	tipText += _("\nVc = ") + wxString::FromDouble(std::abs(m_electricalData.faultVoltage[2]), 5) + _(" p.u.");
	if (m_electricalData.hasFault) {
		tipText += _("\nIa = ") + wxString::FromDouble(std::abs(m_electricalData.faultCurrent[0]), 5) + _(" p.u.");
		tipText += _("\nIb = ") + wxString::FromDouble(std::abs(m_electricalData.faultCurrent[1]), 5) + _(" p.u.");
		tipText += _("\nIc = ") + wxString::FromDouble(std::abs(m_electricalData.faultCurrent[2]), 5) + _(" p.u.");
	}

	tipText += _("\n\nSsc = ") + wxString::FromDouble(std::abs(m_electricalData.scPower), 5) + _(" p.u.");
	tipText += _("\n\nTHD = ") + wxString::FromDouble(std::abs(m_electricalData.thd), 5) + wxT("%");
	int i = 0;
	for (auto& hVoltage : m_electricalData.harmonicVoltage) {
		wxString hVoltageStr;
		hVoltageStr.Printf(_("\nVh(%d) = %.5e%s%.2f%s p.u."), m_electricalData.harmonicOrder[i], std::abs(hVoltage), wxString(L'\u2220'), wxRadToDeg(std::arg(hVoltage)), wxString(L'\u00B0'));
		tipText += hVoltageStr;
		i++;
	}


	return tipText;
}

bool Bus::GetPlotData(ElementPlotData& plotData, PlotStudy study)
{
	if (study == PlotStudy::STABILITY) {
		if (!m_electricalData.plotBus) return false;
		plotData.SetName(m_electricalData.name);
		plotData.SetCurveType(ElementPlotData::CurveType::CT_BUS);

		std::vector<double> absVoltage, argVoltage;
		for (unsigned int i = 0; i < m_electricalData.stabVoltageVector.size(); ++i) {
			absVoltage.push_back(std::abs(m_electricalData.stabVoltageVector[i]));
			argVoltage.push_back(wxRadToDeg(std::arg(m_electricalData.stabVoltageVector[i])));
		}
		plotData.AddData(absVoltage, _("Voltage"));
		plotData.AddData(argVoltage, _("Angle"));
	}
	else if (study == PlotStudy::FREQRESPONSE) {
		if (!m_electricalData.plotPQData) return false;
		plotData.SetName(m_electricalData.name);
		plotData.SetCurveType(ElementPlotData::CurveType::CT_BUS);
		plotData.AddData(m_electricalData.absImpedanceVector, _("Impedance"));
	}
	return true;
}

rapidxml::xml_node<>* Bus::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
	m_electricalData.number = m_elementID;

	auto elementNode = XMLParser::AppendNode(doc, elementListNode, "Bus");
	XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

	Element::SaveCADProperties(doc, elementNode);

	auto electricalProp = XMLParser::AppendNode(doc, elementNode, "ElectricalProperties");
	auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
	XMLParser::SetNodeValue(doc, name, m_electricalData.name);
	auto nominalVoltage = XMLParser::AppendNode(doc, electricalProp, "NominalVoltage");
	XMLParser::SetNodeValue(doc, nominalVoltage, m_electricalData.nominalVoltage);
	XMLParser::SetNodeAttribute(doc, nominalVoltage, "UnitID", static_cast<int>(m_electricalData.nominalVoltageUnit));
	auto isVoltageControlled = XMLParser::AppendNode(doc, electricalProp, "IsVoltageControlled");
	XMLParser::SetNodeValue(doc, isVoltageControlled, m_electricalData.isVoltageControlled);
	auto controlledVoltage = XMLParser::AppendNode(doc, electricalProp, "ControlledVoltage");
	XMLParser::SetNodeValue(doc, controlledVoltage, m_electricalData.controlledVoltage);
	XMLParser::SetNodeAttribute(doc, controlledVoltage, "Choice", m_electricalData.controlledVoltageUnitChoice);
	auto slackBus = XMLParser::AppendNode(doc, electricalProp, "SlackBus");
	XMLParser::SetNodeValue(doc, slackBus, m_electricalData.slackBus);

	auto fault = XMLParser::AppendNode(doc, electricalProp, "Fault");
	auto hasFault = XMLParser::AppendNode(doc, fault, "HasFault");
	XMLParser::SetNodeValue(doc, hasFault, m_electricalData.hasFault);
	auto faultType = XMLParser::AppendNode(doc, fault, "Type");
	XMLParser::SetNodeValue(doc, faultType, static_cast<int>(m_electricalData.faultType));
	auto faultLocation = XMLParser::AppendNode(doc, fault, "Location");
	XMLParser::SetNodeValue(doc, faultLocation, static_cast<int>(m_electricalData.faultLocation));
	auto faultResistance = XMLParser::AppendNode(doc, fault, "Resistance");
	XMLParser::SetNodeValue(doc, faultResistance, m_electricalData.faultResistance);
	auto faultReactance = XMLParser::AppendNode(doc, fault, "Reactance");
	XMLParser::SetNodeValue(doc, faultReactance, m_electricalData.faultReactance);

	auto stability = XMLParser::AppendNode(doc, electricalProp, "Stability");
	auto plotBus = XMLParser::AppendNode(doc, stability, "Plot");
	XMLParser::SetNodeValue(doc, plotBus, m_electricalData.plotBus);
	auto stabHasFault = XMLParser::AppendNode(doc, stability, "HasFault");
	XMLParser::SetNodeValue(doc, stabHasFault, m_electricalData.stabHasFault);
	auto stabFaultTime = XMLParser::AppendNode(doc, stability, "FaultTime");
	XMLParser::SetNodeValue(doc, stabFaultTime, m_electricalData.stabFaultTime);
	auto stabFaultLength = XMLParser::AppendNode(doc, stability, "FaultLength");
	XMLParser::SetNodeValue(doc, stabFaultLength, m_electricalData.stabFaultLength);
	auto stabFaultResistance = XMLParser::AppendNode(doc, stability, "FaultResistance");
	XMLParser::SetNodeValue(doc, stabFaultResistance, m_electricalData.stabFaultResistance);
	auto stabFaultReactance = XMLParser::AppendNode(doc, stability, "FaultReactance");
	XMLParser::SetNodeValue(doc, stabFaultReactance, m_electricalData.stabFaultReactance);

	auto powerQuality = XMLParser::AppendNode(doc, electricalProp, "PowerQuality");
	auto plotPQData = XMLParser::AppendNode(doc, powerQuality, "Plot");
	XMLParser::SetNodeValue(doc, plotPQData, m_electricalData.plotPQData);

	return elementNode;
}

bool Bus::OpenElement(rapidxml::xml_node<>* elementNode)
{
	if (!Element::OpenCADProperties(elementNode)) return false;

	auto electricalProp = elementNode->first_node("ElectricalProperties");
	if (!electricalProp) return false;

	m_electricalData.name = electricalProp->first_node("Name")->value();
	m_electricalData.nominalVoltage = XMLParser::GetNodeValueDouble(electricalProp, "NominalVoltage");
	m_electricalData.nominalVoltageUnit =
		(ElectricalUnit)XMLParser::GetAttributeValueInt(electricalProp, "NominalVoltage", "UnitID");
	m_electricalData.isVoltageControlled = XMLParser::GetNodeValueInt(electricalProp, "IsVoltageControlled");
	m_electricalData.controlledVoltage = XMLParser::GetNodeValueDouble(electricalProp, "ControlledVoltage");
	m_electricalData.controlledVoltageUnitChoice =
		XMLParser::GetAttributeValueInt(electricalProp, "ControlledVoltage", "Choice");
	m_electricalData.slackBus = XMLParser::GetNodeValueInt(electricalProp, "SlackBus");
	auto fault = electricalProp->first_node("Fault");
	m_electricalData.hasFault = XMLParser::GetNodeValueInt(fault, "HasFault");
	m_electricalData.faultType = (FaultData)XMLParser::GetNodeValueInt(fault, "Type");
	m_electricalData.faultLocation = (FaultData)XMLParser::GetNodeValueInt(fault, "Location");
	m_electricalData.faultResistance = XMLParser::GetNodeValueDouble(fault, "Resistance");
	m_electricalData.faultReactance = XMLParser::GetNodeValueDouble(fault, "Reactance");
	auto stability = electricalProp->first_node("Stability");
	m_electricalData.plotBus = XMLParser::GetNodeValueInt(stability, "Plot");
	m_electricalData.stabHasFault = XMLParser::GetNodeValueInt(stability, "HasFault");
	m_electricalData.stabFaultTime = XMLParser::GetNodeValueDouble(stability, "FaultTime");
	m_electricalData.stabFaultLength = XMLParser::GetNodeValueDouble(stability, "FaultLength");
	m_electricalData.stabFaultResistance = XMLParser::GetNodeValueDouble(stability, "FaultResistance");
	m_electricalData.stabFaultReactance = XMLParser::GetNodeValueDouble(stability, "FaultReactance");

	auto powerQuality = electricalProp->first_node("PowerQuality");
	if (powerQuality) m_electricalData.plotPQData = XMLParser::GetNodeValueInt(powerQuality, "Plot");

	if (m_electricalData.stabHasFault) SetDynamicEvent(true);
	return true;
}
