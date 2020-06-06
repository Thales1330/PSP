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

#include "Load.h"

Load::Load() : Shunt() {}
Load::Load(wxString name) : Shunt() { m_electricalData.name = name; }
Load::~Load() {}
bool Load::AddParent(Element* parent, wxPoint2DDouble position)
{
    if(parent) {
        m_parentList.push_back(parent);
        parent->AddChild(this);
        wxPoint2DDouble parentPt =
            parent->RotateAtPosition(position, -parent->GetAngle());        // Rotate click to horizontal position.
        parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
        parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.

        m_position = parentPt + wxPoint2DDouble(0.0, 100.0);  // Shifts the position to the down of the bus.
        m_width = m_height = 20.0;
        m_rect = wxRect2DDouble(m_position.m_x - 10.0, m_position.m_y - 10.0, m_width, m_height);

        m_pointList.push_back(parentPt);
        m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_position));
        m_pointList.push_back(m_position + wxPoint2DDouble(0.0, -20.0));
        m_pointList.push_back(m_position + wxPoint2DDouble(0.0, -10.0));

        m_triangPts.push_back(wxPoint2DDouble(-m_width / 2.0, -m_height / 2.0));
        m_triangPts.push_back(wxPoint2DDouble(m_width / 2.0, -m_height / 2.0));
        m_triangPts.push_back(wxPoint2DDouble(0.0, m_height / 2.0));

        m_inserted = true;

        wxRect2DDouble genRect(0, 0, 0, 0);
        m_switchRect.push_back(genRect);  // Push a general rectangle.
        UpdateSwitches();
        m_pfDirection = PowerFlowDirection::PF_TO_ELEMENT;
        UpdatePowerFlowArrowsPosition();

        return true;
    }
    return false;
}

void Load::Draw(wxPoint2DDouble translation, double scale) const
{
    OpenGLColour elementColour;
    if(m_online) {
        if(m_dynEvent)
            elementColour = m_dynamicEventColour;
        else
            elementColour = m_onlineElementColour;
    } else
        elementColour = m_offlineElementColour;

    if(m_inserted) {
        // Draw Selection (layer 1).
        if(m_selected) {
            glLineWidth(1.5 + m_borderSize * 2.0);
            glColor4dv(m_selectionColour.GetRGBA());
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
        glColor4dv(elementColour.GetRGBA());
        DrawCircle(m_pointList[0], 5.0, 10, GL_POLYGON);

        DrawLine(m_pointList);

        DrawSwitches();
        DrawPowerFlowPts();

        std::vector<wxPoint2DDouble> triangPts;
        for(int i = 0; i < 3; i++) { triangPts.push_back(m_triangPts[i] + m_position); }
        glPushMatrix();
        glTranslated(m_position.m_x, m_position.m_y, 0.0);
        glRotated(m_angle, 0.0, 0.0, 1.0);
        glTranslated(-m_position.m_x, -m_position.m_y, 0.0);
        glColor4dv(elementColour.GetRGBA());
        DrawTriangle(triangPts);
        glPopMatrix();
    }
}

void Load::DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const
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
		// Draw Selection (layer 1).
		if (m_selected) {
			gc->SetPen(wxPen(wxColour(m_selectionColour.GetDcRGBA()), 2 + m_borderSize * 2.0));
			gc->SetBrush(*wxTRANSPARENT_BRUSH);

			gc->DrawLines(m_pointList.size(), &m_pointList[0]);

            gc->SetPen(*wxTRANSPARENT_PEN);
            gc->SetBrush(wxBrush(wxColour(m_selectionColour.GetDcRGBA())));
			
			std::vector<wxPoint2DDouble> selTriangPts;
			selTriangPts.push_back(m_triangPts[0] + m_position +
				wxPoint2DDouble(-m_borderSize / scale, -m_borderSize / scale));
			selTriangPts.push_back(m_triangPts[1] + m_position +
				wxPoint2DDouble(m_borderSize / scale, -m_borderSize / scale));
			selTriangPts.push_back(m_triangPts[2] + m_position + wxPoint2DDouble(0.0, m_borderSize / scale));

			// Push the current matrix on stack.
			gc->PushState();
			// Rotate the matrix around the object position.
			gc->Translate(m_position.m_x, m_position.m_y);
			gc->Rotate(wxDegToRad(m_angle));
			gc->Translate(-m_position.m_x, -m_position.m_y);
			DrawDCTriangle(selTriangPts, gc);
            gc->PopState();

			// Draw node selection.
			DrawDCCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, gc);
		}

		// Draw Load (layer 2).
		gc->SetPen(wxPen(wxColour(elementColour.GetDcRGBA()), 2));
		gc->SetBrush(*wxTRANSPARENT_BRUSH);
        gc->DrawLines(m_pointList.size(), &m_pointList[0]);

		// Draw node.
		gc->SetPen(*wxTRANSPARENT_PEN);
		gc->SetBrush(wxBrush(wxColour(elementColour.GetDcRGBA())));
		DrawDCCircle(m_pointList[0], 5.0, 10, gc);

		DrawDCSwitches(gc);
		DrawDCPowerFlowPts(gc);

		std::vector<wxPoint2DDouble> triangPts;
		for (int i = 0; i < 3; i++) { triangPts.push_back(m_triangPts[i] + m_position); }
		gc->PushState();
		gc->Translate(m_position.m_x, m_position.m_y);
		gc->Rotate(wxDegToRad(m_angle));
		gc->Translate(-m_position.m_x, -m_position.m_y);
		gc->SetPen(*wxTRANSPARENT_PEN);
		gc->SetBrush(wxBrush(wxColour(elementColour.GetDcRGBA())));
		DrawDCTriangle(triangPts, gc);
        gc->PopState();
	}
}

void Load::Rotate(bool clockwise)
{
    double rotAngle = m_rotationAngle;
    if(!clockwise) rotAngle = -m_rotationAngle;

    m_angle += rotAngle;
    if(m_angle >= 360 || m_angle <= -360) m_angle = 0.0;
    m_pointList[2] = RotateAtPosition(m_pointList[2], rotAngle);
    m_pointList[3] = RotateAtPosition(m_pointList[3], rotAngle);
    UpdateSwitchesPosition();
    UpdatePowerFlowArrowsPosition();
}

bool Load::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_ELEMENT, _("Edit Load"));
    GeneralMenuItens(menu);
    return true;
}

bool Load::ShowForm(wxWindow* parent, Element* element)
{
    LoadForm* loadForm = new LoadForm(parent, this);
    if(loadForm->ShowModal() == wxID_OK) {
        loadForm->Destroy();
        return true;
    }
    loadForm->Destroy();
    return false;
}

LoadElectricalData Load::GetPUElectricalData(double systemPowerBase)
{
    LoadElectricalData data = m_electricalData;
    switch(data.activePowerUnit) {
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
    switch(data.reactivePowerUnit) {
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

Element* Load::GetCopy()
{
    Load* copy = new Load();
    *copy = *this;
    return copy;
}

wxString Load::GetTipText() const
{
    wxString tipText = m_electricalData.name;

    // TODO: Avoid power calculation.
    double activePower = m_electricalData.activePower;
    double reactivePower = m_electricalData.reactivePower;
    if(!m_online) {
        activePower = 0.0;
        reactivePower = 0.0;
    }
    if(m_online && m_electricalData.loadType == CONST_IMPEDANCE) {
        std::complex<double> v = static_cast<Bus*>(m_parentList[0])->GetElectricalData().voltage;
        reactivePower *= std::pow(std::abs(v), 2);
        activePower *= std::pow(std::abs(v), 2);
    }
    tipText += "\n";
    tipText += _("\nP = ") + wxString::FromDouble(activePower, 5);
    switch(m_electricalData.activePowerUnit) {
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
    tipText += _("\nQ = ") + wxString::FromDouble(reactivePower, 5);
    switch(m_electricalData.reactivePowerUnit) {
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

bool Load::GetPlotData(ElementPlotData& plotData, PlotStudy study)
{
    if(!m_electricalData.plotLoad) return false;
    plotData.SetName(m_electricalData.name);
    plotData.SetCurveType(ElementPlotData::CurveType::CT_LOAD);

    std::vector<double> absVoltage, activePower, reactivePower, current;
    for(unsigned int i = 0; i < m_electricalData.voltageVector.size(); ++i) {
        absVoltage.push_back(std::abs(m_electricalData.voltageVector[i]));
        activePower.push_back(std::real(m_electricalData.electricalPowerVector[i]));
        reactivePower.push_back(std::imag(m_electricalData.electricalPowerVector[i]));
        current.push_back(std::abs(std::complex<double>(activePower[i], -reactivePower[i]) /
                                   std::conj(m_electricalData.voltageVector[i])));
    }

    plotData.AddData(absVoltage, _("Voltage"));
    plotData.AddData(activePower, _("Active power"));
    plotData.AddData(reactivePower, _("Reactive power"));
    plotData.AddData(current, _("Current"));

    return true;
}

rapidxml::xml_node<>* Load::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
    auto elementNode = XMLParser::AppendNode(doc, elementListNode, "Load");
    XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

    SaveCADProperties(doc, elementNode);

    auto electricalProp = XMLParser::AppendNode(doc, elementNode, "ElectricalProperties");
    auto isOnline = XMLParser::AppendNode(doc, electricalProp, "IsOnline");
    XMLParser::SetNodeValue(doc, isOnline, m_online);
    auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
    XMLParser::SetNodeValue(doc, name, m_electricalData.name);
    auto activePower = XMLParser::AppendNode(doc, electricalProp, "ActivePower");
    XMLParser::SetNodeValue(doc, activePower, m_electricalData.activePower);
    XMLParser::SetNodeAttribute(doc, activePower, "UnitID", static_cast<int>(m_electricalData.activePowerUnit));
    auto reactivePower = XMLParser::AppendNode(doc, electricalProp, "ReactivePower");
    XMLParser::SetNodeValue(doc, reactivePower, m_electricalData.reactivePower);
    XMLParser::SetNodeAttribute(doc, reactivePower, "UnitID", static_cast<int>(m_electricalData.reactivePowerUnit));
    auto loadType = XMLParser::AppendNode(doc, electricalProp, "LoadType");
    XMLParser::SetNodeValue(doc, loadType, m_electricalData.loadType);

    auto stability = XMLParser::AppendNode(doc, electricalProp, "Stability");
    auto plotLoad = XMLParser::AppendNode(doc, stability, "PlotLoad");
    XMLParser::SetNodeValue(doc, plotLoad, m_electricalData.plotLoad);
    auto useCompLoad = XMLParser::AppendNode(doc, stability, "UseCompositeLoad");
    XMLParser::SetNodeValue(doc, useCompLoad, m_electricalData.useCompLoad);
    auto activePowerCompl = XMLParser::AppendNode(doc, stability, "ActivePowerComposition");
    auto pzl = XMLParser::AppendNode(doc, activePowerCompl, "ConstantImpedance");
    XMLParser::SetNodeValue(doc, pzl, m_electricalData.constImpedanceActive);
    auto pil = XMLParser::AppendNode(doc, activePowerCompl, "ConstantCurrent");
    XMLParser::SetNodeValue(doc, pil, m_electricalData.constCurrentActive);
    auto ppl = XMLParser::AppendNode(doc, activePowerCompl, "ConstantPower");
    XMLParser::SetNodeValue(doc, ppl, m_electricalData.constPowerActive);
    auto reactivePowerCompl = XMLParser::AppendNode(doc, stability, "ReactivePowerComposition");
    auto qzl = XMLParser::AppendNode(doc, reactivePowerCompl, "ConstantImpedance");
    XMLParser::SetNodeValue(doc, qzl, m_electricalData.constImpedanceReactive);
    auto qil = XMLParser::AppendNode(doc, reactivePowerCompl, "ConstantCurrent");
    XMLParser::SetNodeValue(doc, qil, m_electricalData.constCurrentReactive);
    auto qpl = XMLParser::AppendNode(doc, reactivePowerCompl, "ConstantPower");
    XMLParser::SetNodeValue(doc, qpl, m_electricalData.constPowerReactive);

    SaveSwitchingData(doc, electricalProp);

    return elementNode;
}

bool Load::OpenElement(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList)
{
    if(!OpenCADProperties(elementNode, parentList)) return false;
    // The load have to insert the points that define his triangle
    m_triangPts.push_back(wxPoint2DDouble(-m_width / 2.0, -m_height / 2.0));
    m_triangPts.push_back(wxPoint2DDouble(m_width / 2.0, -m_height / 2.0));
    m_triangPts.push_back(wxPoint2DDouble(0.0, m_height / 2.0));

    auto electricalProp = elementNode->first_node("ElectricalProperties");
    if(!electricalProp) return false;

    SetOnline(XMLParser::GetNodeValueInt(electricalProp, "IsOnline"));
    m_electricalData.name = electricalProp->first_node("Name")->value();
    m_electricalData.activePower = XMLParser::GetNodeValueDouble(electricalProp, "ActivePower");
    m_electricalData.activePowerUnit =
        static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "ActivePower", "UnitID"));
    m_electricalData.reactivePower = XMLParser::GetNodeValueDouble(electricalProp, "ReactivePower");
    m_electricalData.reactivePowerUnit =
        static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "ReactivePower", "UnitID"));
    m_electricalData.loadType = static_cast<LoadType>(XMLParser::GetNodeValueInt(electricalProp, "LoadType"));
    // Stability
    auto stability = electricalProp->first_node("Stability");
    if(stability) {
        m_electricalData.plotLoad = XMLParser::GetNodeValueInt(stability, "PlotLoad");
        m_electricalData.useCompLoad = XMLParser::GetNodeValueInt(stability, "UseCompositeLoad");
        auto activePowerComp = stability->first_node("ActivePowerComposition");
        m_electricalData.constImpedanceActive = XMLParser::GetNodeValueDouble(activePowerComp, "ConstantImpedance");
        m_electricalData.constCurrentActive = XMLParser::GetNodeValueDouble(activePowerComp, "ConstantCurrent");
        m_electricalData.constPowerActive = XMLParser::GetNodeValueDouble(activePowerComp, "ConstantPower");
        auto reactivePowerComp = stability->first_node("ReactivePowerComposition");
        m_electricalData.constImpedanceReactive = XMLParser::GetNodeValueDouble(reactivePowerComp, "ConstantImpedance");
        m_electricalData.constCurrentReactive = XMLParser::GetNodeValueDouble(reactivePowerComp, "ConstantCurrent");
        m_electricalData.constPowerReactive = XMLParser::GetNodeValueDouble(reactivePowerComp, "ConstantPower");
    }

    if(!OpenSwitchingData(electricalProp)) return false;
    if(m_swData.swTime.size() != 0) SetDynamicEvent(true);

    m_inserted = true;

    return true;
}
