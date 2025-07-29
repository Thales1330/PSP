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

#include "IndMotor.h"
#include "../../forms/IndMotorForm.h"

IndMotor::IndMotor() : Machines()
{
	m_elementType = TYPE_IND_MOTOR;
}

IndMotor::IndMotor(wxString name) : Machines()
{
	m_elementType = TYPE_IND_MOTOR;
	m_electricalData.name = name;
}

IndMotor::~IndMotor() {}

//void IndMotor::DrawSymbol() const
//{
//	std::vector<wxPoint2DDouble> mPts;
//	mPts.push_back(wxPoint2DDouble(-10, 13) + m_position);
//	mPts.push_back(wxPoint2DDouble(-10, -13) + m_position);
//	mPts.push_back(wxPoint2DDouble(0, 2) + m_position);
//	mPts.push_back(wxPoint2DDouble(10, -13) + m_position);
//	mPts.push_back(wxPoint2DDouble(10, 13) + m_position);
//	DrawLine(mPts);
//}

void IndMotor::DrawDCSymbol(wxGraphicsContext* gc) const
{
	std::vector<wxPoint2DDouble> mPts;
	mPts.push_back(wxPoint2DDouble(-10, 13) + m_position);
	mPts.push_back(wxPoint2DDouble(-10, -13) + m_position);
	mPts.push_back(wxPoint2DDouble(0, 2) + m_position);
	mPts.push_back(wxPoint2DDouble(10, -13) + m_position);
	mPts.push_back(wxPoint2DDouble(10, 13) + m_position);
	gc->StrokeLines(mPts.size(), &mPts[0]);
}

bool IndMotor::GetContextMenu(wxMenu& menu)
{
	menu.Append(ID_EDIT_ELEMENT, _("Edit induction motor"));

	wxMenu* textMenu = new wxMenu();

	textMenu->Append(ID_TXT_NAME, _("Name"));
	textMenu->Append(ID_TXT_ACTIVE_POWER, _("Active power"));
	textMenu->Append(ID_TXT_REACTIVE_POWER, _("Reactive power"));
	textMenu->SetClientData(menu.GetClientData());
	menu.AppendSubMenu(textMenu, _("Add text"));

	GeneralMenuItens(menu);
	return true;
}

bool IndMotor::ShowForm(wxWindow* parent, Element* element)
{
	IndMotorForm* indMotorForm = new IndMotorForm(parent, this);
	if (indMotorForm->ShowModal() == wxID_OK) {
		indMotorForm->Destroy();
		return true;
	}
	indMotorForm->Destroy();
	return false;
}

IndMotorElectricalData IndMotor::GetPUElectricalData(double systemPowerBase)
{
	IndMotorElectricalData data = m_electricalData;

	switch (data.activePowerUnit) {
	case ElectricalUnit::UNIT_W: {
		data.activePower = data.activePower / systemPowerBase;
		data.activePowerUnit = ElectricalUnit::UNIT_PU;
	} break;
	case ElectricalUnit::UNIT_kW: {
		data.activePower = (data.activePower * 1e3) / systemPowerBase;
		data.activePowerUnit = ElectricalUnit::UNIT_PU;
	} break;
	case ElectricalUnit::UNIT_MW: {
		data.activePower = (data.activePower * 1e6) / systemPowerBase;
		data.activePowerUnit = ElectricalUnit::UNIT_PU;
	} break;
	default:
		break;
	}
	switch (data.reactivePowerUnit) {
	case ElectricalUnit::UNIT_var: {
		data.reactivePower = data.reactivePower / systemPowerBase;
		data.reactivePowerUnit = ElectricalUnit::UNIT_PU;
	} break;
	case ElectricalUnit::UNIT_kvar: {
		data.reactivePower = (data.reactivePower * 1e3) / systemPowerBase;
		data.reactivePowerUnit = ElectricalUnit::UNIT_PU;
	} break;
	case ElectricalUnit::UNIT_Mvar: {
		data.reactivePower = (data.reactivePower * 1e6) / systemPowerBase;
		data.reactivePowerUnit = ElectricalUnit::UNIT_PU;
	} break;
	default:
		break;
	}

	return data;
}

Element* IndMotor::GetCopy()
{
	IndMotor* copy = new IndMotor();
	*copy = *this;
	return copy;
}

wxString IndMotor::GetTipText() const
{
	wxString tipText = m_electricalData.name;
	tipText += "\n";
	double activePower = m_electricalData.activePower;
	if (!m_online) activePower = 0.0;
	tipText += _("\nP = ") + wxString::FromDouble(activePower, 5);
	switch (m_electricalData.activePowerUnit) {
	case ElectricalUnit::UNIT_PU: {
		tipText += _(" p.u.");
	} break;
	case ElectricalUnit::UNIT_W: {
		tipText += _(" W");
	} break;
	case ElectricalUnit::UNIT_kW: {
		tipText += _(" kW");
	} break;
	case ElectricalUnit::UNIT_MW: {
		tipText += _(" MW");
	} break;
	default:
		break;
	}
	double reactivePower = m_electricalData.reactivePower;
	if (!m_online) reactivePower = 0.0;
	tipText += _("\nQ = ") + wxString::FromDouble(reactivePower, 5);
	switch (m_electricalData.reactivePowerUnit) {
	case ElectricalUnit::UNIT_PU: {
		tipText += _(" p.u.");
	} break;
	case ElectricalUnit::UNIT_var: {
		tipText += _(" VAr");
	} break;
	case ElectricalUnit::UNIT_kvar: {
		tipText += _(" kVAr");
	} break;
	case ElectricalUnit::UNIT_Mvar: {
		tipText += _(" MVAr");
	} break;
	default:
		break;
	}

	return tipText;
}

rapidxml::xml_node<>* IndMotor::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
	auto elementNode = XMLParser::AppendNode(doc, elementListNode, "IndMotor");
	XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

	SaveCADProperties(doc, elementNode);

	// Element properties
	// General
	auto electricalProp = XMLParser::AppendNode(doc, elementNode, "ElectricalProperties");
	auto isOnline = XMLParser::AppendNode(doc, electricalProp, "IsOnline");
	XMLParser::SetNodeValue(doc, isOnline, m_online);
	auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
	XMLParser::SetNodeValue(doc, name, m_electricalData.name);
	auto ratedPower = XMLParser::AppendNode(doc, electricalProp, "RatedPower");
	XMLParser::SetNodeValue(doc, ratedPower, m_electricalData.ratedPower);
	XMLParser::SetNodeAttribute(doc, ratedPower, "UnitID", static_cast<int>(m_electricalData.activePowerUnit));
	auto activePower = XMLParser::AppendNode(doc, electricalProp, "ActivePower");
	XMLParser::SetNodeValue(doc, activePower, m_electricalData.activePower);
	XMLParser::SetNodeAttribute(doc, activePower, "UnitID", static_cast<int>(m_electricalData.activePowerUnit));
	auto reactivePower = XMLParser::AppendNode(doc, electricalProp, "ReactivePower");
	XMLParser::SetNodeValue(doc, reactivePower, m_electricalData.reactivePower);
	XMLParser::SetNodeAttribute(doc, reactivePower, "UnitID", static_cast<int>(m_electricalData.reactivePowerUnit));
	auto useMachineBase = XMLParser::AppendNode(doc, electricalProp, "UseMachineBase");
	XMLParser::SetNodeValue(doc, useMachineBase, m_electricalData.useMachinePowerAsBase);

	// Stability
	auto stability = XMLParser::AppendNode(doc, electricalProp, "Stability");
	auto plotMotor = XMLParser::AppendNode(doc, stability, "PlotIndMachine");
	XMLParser::SetNodeValue(doc, plotMotor, m_electricalData.plotIndMachine);
	auto inertia = XMLParser::AppendNode(doc, stability, "Inertia");
	XMLParser::SetNodeValue(doc, inertia, m_electricalData.inertia);
	auto r1 = XMLParser::AppendNode(doc, stability, "StatorResistence");
	XMLParser::SetNodeValue(doc, r1, m_electricalData.r1);
	auto x1 = XMLParser::AppendNode(doc, stability, "StatorReactance");
	XMLParser::SetNodeValue(doc, x1, m_electricalData.x1);
	auto r2 = XMLParser::AppendNode(doc, stability, "RotorResistence");
	XMLParser::SetNodeValue(doc, r2, m_electricalData.r2);
	auto x2 = XMLParser::AppendNode(doc, stability, "RotorReactance");
	XMLParser::SetNodeValue(doc, x2, m_electricalData.x2);
	auto xm = XMLParser::AppendNode(doc, stability, "MagnetizingReactance");
	XMLParser::SetNodeValue(doc, xm, m_electricalData.xm);
	auto useCageFactor = XMLParser::AppendNode(doc, stability, "UseCageFactor");
	XMLParser::SetNodeValue(doc, useCageFactor, m_electricalData.useKf);
	auto cageFactor = XMLParser::AppendNode(doc, stability, "CageFactor");
	XMLParser::SetNodeValue(doc, cageFactor, m_electricalData.kf);
	auto loadChar = XMLParser::AppendNode(doc, stability, "LoadCharacteristic");
	auto aw = XMLParser::AppendNode(doc, loadChar, "Constant");
	XMLParser::SetNodeValue(doc, aw, m_electricalData.aw);
	auto bw = XMLParser::AppendNode(doc, loadChar, "Linear");
	XMLParser::SetNodeValue(doc, bw, m_electricalData.bw);
	auto cw = XMLParser::AppendNode(doc, loadChar, "Quadratic");
	XMLParser::SetNodeValue(doc, cw, m_electricalData.cw);

	SaveSwitchingData(doc, electricalProp);

	return elementNode;
}

bool IndMotor::OpenElement(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList)
{
	if (!OpenCADProperties(elementNode, parentList)) return false;

	auto electricalProp = elementNode->first_node("ElectricalProperties");
	if (!electricalProp) return false;

	// Element properties
	SetOnline(XMLParser::GetNodeValueInt(electricalProp, "IsOnline"));
	m_electricalData.name = electricalProp->first_node("Name")->value();
	m_electricalData.ratedPower = XMLParser::GetNodeValueDouble(electricalProp, "RatedPower");
	m_electricalData.ratedPowerUnit =
		static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "RatedPower", "UnitID"));
	m_electricalData.activePower = XMLParser::GetNodeValueDouble(electricalProp, "ActivePower");
	m_electricalData.activePowerUnit =
		static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "ActivePower", "UnitID"));
	m_electricalData.reactivePower = XMLParser::GetNodeValueDouble(electricalProp, "ReactivePower");
	m_electricalData.reactivePowerUnit =
		static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "ReactivePower", "UnitID"));
	m_electricalData.useMachinePowerAsBase = XMLParser::GetNodeValueInt(electricalProp, "UseMachineBase");

	// Stability
	auto stability = electricalProp->first_node("Stability");
	m_electricalData.plotIndMachine = XMLParser::GetNodeValueInt(stability, "PlotIndMachine");
	m_electricalData.inertia = XMLParser::GetNodeValueDouble(stability, "Inertia");
	m_electricalData.r1 = XMLParser::GetNodeValueDouble(stability, "StatorResistence");
	m_electricalData.x1 = XMLParser::GetNodeValueDouble(stability, "StatorReactance");
	m_electricalData.r2 = XMLParser::GetNodeValueDouble(stability, "RotorResistence");
	m_electricalData.x2 = XMLParser::GetNodeValueDouble(stability, "RotorReactance");
	m_electricalData.xm = XMLParser::GetNodeValueDouble(stability, "MagnetizingReactance");
	m_electricalData.useKf = XMLParser::GetNodeValueInt(stability, "UseCageFactor");
	m_electricalData.kf = XMLParser::GetNodeValueDouble(stability, "CageFactor");
	auto loadChar = stability->first_node("LoadCharacteristic");
	m_electricalData.aw = XMLParser::GetNodeValueDouble(loadChar, "Constant");
	m_electricalData.bw = XMLParser::GetNodeValueDouble(loadChar, "Linear");
	m_electricalData.cw = XMLParser::GetNodeValueDouble(loadChar, "Quadratic");

	if (!OpenSwitchingData(electricalProp)) return false;
	if (m_swData.swTime.size() != 0) SetDynamicEvent(true);

	m_inserted = true;

	return true;
}

bool IndMotor::GetPlotData(ElementPlotData& plotData, PlotStudy study)
{
	if (!m_electricalData.plotIndMachine) return false;
	plotData.SetName(m_electricalData.name);
	plotData.SetCurveType(ElementPlotData::CurveType::CT_IND_MOTOR);

	plotData.AddData(m_electricalData.terminalVoltageVector, _("Terminal voltage"));
	plotData.AddData(m_electricalData.activePowerVector, _("Active power"));
	plotData.AddData(m_electricalData.reactivePowerVector, _("Reactive power"));
	plotData.AddData(m_electricalData.currentVector, _("Current"));
	plotData.AddData(m_electricalData.electricalTorqueVector, _("Electrical torque"));
	plotData.AddData(m_electricalData.mechanicalTorqueVector, _("Mechanical torque"));
	plotData.AddData(m_electricalData.velocityVector, _("Speed"));
	plotData.AddData(m_electricalData.slipVector, _("Slip"));
	return true;
}

void IndMotor::InitPowerFlowMotor(double systemPowerBase, int busNumber)
{
	double k = 1.0;  // Power base change factor.
	if (m_electricalData.useMachinePowerAsBase) {
		double oldBase = GetValueFromUnit(m_electricalData.ratedPower, m_electricalData.ratedPowerUnit);
		k = systemPowerBase / oldBase;
	}
	// Calculate the induction machine transient constants at the machine base
	m_electricalData.r1t = m_electricalData.r1 * k;
	m_electricalData.r2t = m_electricalData.r2 * k;
	m_electricalData.x1t = m_electricalData.x1 * k;
	m_electricalData.x2t = m_electricalData.x2 * k;
	m_electricalData.xmt = m_electricalData.xm * k;

	m_electricalData.xt = m_electricalData.x1t +
		(m_electricalData.x2t * m_electricalData.xmt) / (m_electricalData.x2t + m_electricalData.xmt);
	m_electricalData.x0 = m_electricalData.x1t + m_electricalData.xmt;

	double r1 = m_electricalData.r1t;
	double r2 = m_electricalData.r2t;
	if (m_electricalData.useKf) r2 *= (1.0 + m_electricalData.kf * m_electricalData.r2t);
	double x1 = m_electricalData.x1t;
	double x2 = m_electricalData.x2t;
	double xm = m_electricalData.xmt;
	m_electricalData.k1 = x2 + xm;
	m_electricalData.k2 = -x1 * m_electricalData.k1 - x2 * xm;
	m_electricalData.k3 = xm + x1;
	m_electricalData.k4 = r1 * m_electricalData.k1;

	auto puData = GetPUElectricalData(systemPowerBase);
	m_electricalData.p0 = puData.activePower;
	m_electricalData.busNum = busNumber;
}

bool IndMotor::CalculateReactivePower(double voltage)
{
	double a = m_electricalData.p0 *
		(m_electricalData.r1t * m_electricalData.r1t + m_electricalData.k3 * m_electricalData.k3) -
		voltage * voltage * m_electricalData.r1t;
	double b = 2.0 * m_electricalData.p0 *
		(m_electricalData.r1t * m_electricalData.k2 + m_electricalData.k3 * m_electricalData.k4) -
		voltage * voltage * (m_electricalData.k2 + m_electricalData.k1 * m_electricalData.k3);
	double c =
		m_electricalData.p0 * (m_electricalData.k2 * m_electricalData.k2 + m_electricalData.k4 * m_electricalData.k4) -
		voltage * voltage * m_electricalData.k1 * m_electricalData.k4;
	double d = (b * b - 4.0 * a * c);
	if (d < 0.0) return false;
	double r2_s = (-b + std::sqrt(d)) / (2.0 * a);

	double qa = m_electricalData.k1 * (r2_s * m_electricalData.r1t - m_electricalData.x1t * m_electricalData.k1 -
		m_electricalData.x2t * m_electricalData.xmt);
	double qb =
		r2_s * (r2_s * (m_electricalData.xmt + m_electricalData.x1t) + m_electricalData.r1t * m_electricalData.k1);
	double qc = r2_s * m_electricalData.r1t - m_electricalData.x1t * m_electricalData.k1 -
		m_electricalData.x2t * m_electricalData.xmt;
	double qd = r2_s * (m_electricalData.xmt + m_electricalData.x1t) + m_electricalData.r1t * m_electricalData.k1;
	m_electricalData.qValue = (-voltage * voltage * (qa - qb)) / (qc * qc + qd * qd);

	return true;
}
