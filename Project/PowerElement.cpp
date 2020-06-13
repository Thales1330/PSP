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

#include "PowerElement.h"
#ifdef USING_WX_3_0_X
#include "DegreesAndRadians.h"
#endif
#include <wx/brush.h>

PowerElement::PowerElement() : Element()
{
    m_busColour.SetRGBA(0.0, 0.3, 1.0, 1.0);
    m_onlineElementColour.SetRGBA(0.2, 0.2, 0.2, 1.0);
    m_offlineElementColour.SetRGBA(0.5, 0.5, 0.5, 1.0);
    m_closedSwitchColour.SetRGBA(0.0, 0.4, 0.0, 1.0);
    m_openedSwitchColour.SetRGBA(1.0, 0.1, 0.1, 1.0);
    m_powerFlowArrowColour.SetRGBA(1.0, 0.51, 0.0, 1.0);
    m_dynamicEventColour.SetRGBA(1.0, 0.51, 0.0, 1.0);
}

PowerElement::~PowerElement() {}
void PowerElement::SetNominalVoltage(std::vector<double> nominalVoltage, std::vector<ElectricalUnit> nominalVoltageUnit)
{
}

wxPoint2DDouble PowerElement::GetSwitchPoint(Element* parent,
                                             wxPoint2DDouble parentPoint,
                                             wxPoint2DDouble secondPoint) const
{
    double swLineSize = 25.0;
    wxPoint2DDouble swPoint = wxPoint2DDouble(parentPoint.m_x, parentPoint.m_y - swLineSize);

    // Rotate the second point (to compare).
    double angle = parent->GetAngle();

    secondPoint =
        wxPoint2DDouble(std::cos(wxDegToRad(-angle)) * (secondPoint.m_x - parentPoint.m_x) -
                            std::sin(wxDegToRad(-angle)) * (secondPoint.m_y - parentPoint.m_y) + parentPoint.m_x,
                        std::sin(wxDegToRad(-angle)) * (secondPoint.m_x - parentPoint.m_x) +
                            std::cos(wxDegToRad(-angle)) * (secondPoint.m_y - parentPoint.m_y) + parentPoint.m_y);

    // Rotate
    if(secondPoint.m_y > parentPoint.m_y) angle -= 180.0;
    return wxPoint2DDouble(std::cos(wxDegToRad(angle)) * (swPoint.m_x - parentPoint.m_x) -
                               std::sin(wxDegToRad(angle)) * (swPoint.m_y - parentPoint.m_y) + parentPoint.m_x,
                           std::sin(wxDegToRad(angle)) * (swPoint.m_x - parentPoint.m_x) +
                               std::cos(wxDegToRad(angle)) * (swPoint.m_y - parentPoint.m_y) + parentPoint.m_y);
}

bool PowerElement::SwitchesContains(wxPoint2DDouble position) const
{
    for(int i = 0; i < (int)m_switchRect.size(); i++) {
        if(m_parentList[i]) {
            if(m_switchRect[i].Contains(position)) return true;
        }
    }
    return false;
}

void PowerElement::UpdateSwitches()
{
    // General method, to one switch only.
    wxPoint2DDouble swCenter = wxPoint2DDouble((m_pointList[0].m_x + m_pointList[1].m_x) / 2.0,
                                               (m_pointList[0].m_y + m_pointList[1].m_y) / 2.0);
    m_switchRect[0] = wxRect2DDouble(swCenter.m_x - m_switchSize / 2.0, swCenter.m_y - m_switchSize / 2.0, m_switchSize,
                                     m_switchSize);
}

void PowerElement::DrawSwitches() const
{
    int i = 0;
    for(auto it = m_parentList.begin(); it != m_parentList.end(); it++) {
        Element* parent = *it;
        if(parent) {
            if(m_online) {
                glColor4dv(m_closedSwitchColour.GetRGBA());
            } else {
                glColor4dv(m_openedSwitchColour.GetRGBA());
            }

            glPushMatrix();
            glTranslated(m_switchRect[i].GetPosition().m_x + m_switchSize / 2.0,
                         m_switchRect[i].GetPosition().m_y + m_switchSize / 2.0, 0.0);
            glRotated(parent->GetAngle(), 0.0, 0.0, 1.0);
            glTranslated(-m_switchRect[i].GetPosition().m_x - m_switchSize / 2.0,
                         -m_switchRect[i].GetPosition().m_y - m_switchSize / 2.0, 0.0);

            DrawRectangle(m_switchRect[i].GetPosition() + wxPoint2DDouble(m_switchSize / 2.0, m_switchSize / 2.0),
                          m_switchSize, m_switchSize);

            glPopMatrix();
        }
        i++;
    }
}

void PowerElement::DrawDCSwitches(wxGraphicsContext* gc) const
{
	gc->SetPen(*wxTRANSPARENT_PEN);
	
	int i = 0;
    for (auto parent : m_parentList) {
		if (parent) {
			if (m_online) {
                gc->SetBrush(wxBrush(wxColour(m_closedSwitchColour.GetDcRGBA())));
			}
			else {
                gc->SetBrush(wxBrush(wxColour(m_openedSwitchColour.GetDcRGBA())));
			}

            gc->PushState();
			gc->Translate(m_switchRect[i].GetPosition().m_x + m_switchSize / 2.0,
				m_switchRect[i].GetPosition().m_y + m_switchSize / 2.0);
            gc->Rotate(wxDegToRad(parent->GetAngle()));
			gc->Translate(-m_switchRect[i].GetPosition().m_x - m_switchSize / 2.0,
				-m_switchRect[i].GetPosition().m_y - m_switchSize / 2.0);

            wxPoint2DDouble switchPos = m_switchRect[i].GetPosition();
			gc->DrawRectangle(switchPos.m_x, switchPos.m_y, m_switchSize, m_switchSize);

            gc->PopState();
		}
		i++;
    }
}

void PowerElement::CalculatePowerFlowPts(std::vector<wxPoint2DDouble> edges)
{
    double arrowRate = 100.0;  // One arrow to each "arrowRate" distance in pixels.

    if(edges.size() < 2) return;

    // Clear all power flow points
    for(int i = 0; i < (int)m_powerFlowArrow.size(); i++) m_powerFlowArrow[i].clear();
    m_powerFlowArrow.clear();

    for(int i = 1; i < (int)edges.size(); i++) {
        wxPoint2DDouble pt1 = edges[i - 1];
        wxPoint2DDouble pt2 = edges[i];

        double angle = std::atan2(pt2.m_y - pt1.m_y, pt2.m_x - pt1.m_x);

        wxPoint2DDouble rotPt2(
            std::cos(-angle) * (pt2.m_x - pt1.m_x) - std::sin(-angle) * (pt2.m_y - pt1.m_y) + pt1.m_x,
            std::sin(-angle) * (pt2.m_x - pt1.m_x) + std::cos(-angle) * (pt2.m_y - pt1.m_y) + pt1.m_y);

        int numArrows = std::abs(pt1.m_x - rotPt2.m_x) / arrowRate;
        if(numArrows == 0) numArrows = 1;

        for(int i = 0; i < numArrows; i++) {
            wxPoint2DDouble arrowCenter(pt1.m_x + ((rotPt2.m_x - pt1.m_x) / double(numArrows + 1)) * double(i + 1),
                                        pt1.m_y + ((rotPt2.m_y - pt1.m_y) / double(numArrows + 1)) * double(i + 1));

            std::vector<wxPoint2DDouble> triPts;
            triPts.push_back(arrowCenter + wxPoint2DDouble(5.0, 0.0));
            triPts.push_back(arrowCenter + wxPoint2DDouble(-5.0, 5.0));
            triPts.push_back(arrowCenter + wxPoint2DDouble(-5.0, -5.0));

            // Rotate back.
            for(int i = 0; i < 3; i++) {
                triPts[i] = wxPoint2DDouble(
                    std::cos(angle) * (triPts[i].m_x - pt1.m_x) - std::sin(angle) * (triPts[i].m_y - pt1.m_y) + pt1.m_x,
                    std::sin(angle) * (triPts[i].m_x - pt1.m_x) + std::cos(angle) * (triPts[i].m_y - pt1.m_y) +
                        pt1.m_y);
            }
            m_powerFlowArrow.push_back(triPts);
        }
    }
}

void PowerElement::DrawPowerFlowPts() const
{
    if(m_online) {
        glColor4dv(m_powerFlowArrowColour.GetRGBA());
        for(int i = 0; i < (int)m_powerFlowArrow.size(); i++) { DrawTriangle(m_powerFlowArrow[i]); }
    }
}

void PowerElement::DrawDCPowerFlowPts(wxGraphicsContext* gc) const
{
    gc->SetPen(*wxTRANSPARENT_PEN);
	if (m_online) {
        gc->SetBrush(wxBrush(wxColour(m_powerFlowArrowColour.GetDcRGBA())));
		for (auto arrow : m_powerFlowArrow) { DrawDCTriangle(arrow, gc); }
	}
}

double PowerElement::GetValueFromUnit(double value, ElectricalUnit valueUnit)
{
    switch(valueUnit) {
        case ElectricalUnit::UNIT_kV:
        case ElectricalUnit::UNIT_kA:
        case ElectricalUnit::UNIT_kW:
        case ElectricalUnit::UNIT_kVA:
        case ElectricalUnit::UNIT_kvar: {
            return value * 1e3;
        } break;
        case ElectricalUnit::UNIT_MW:
        case ElectricalUnit::UNIT_MVA:
        case ElectricalUnit::UNIT_Mvar: {
            return value * 1e6;
        }
        default:
            break;
    }
    return value;
}

bool PowerElement::OpenCADProperties(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList)
{
    auto cadPropNode = elementNode->first_node("CADProperties");
    if(!cadPropNode) return false;

    auto position = cadPropNode->first_node("Position");
    double posX = XMLParser::GetNodeValueDouble(position, "X");
    double posY = XMLParser::GetNodeValueDouble(position, "Y");
    auto size = cadPropNode->first_node("Size");
    m_width = XMLParser::GetNodeValueDouble(size, "Width");
    m_height = XMLParser::GetNodeValueDouble(size, "Height");
    double angle = XMLParser::GetNodeValueDouble(cadPropNode, "Angle");
    SetPosition(wxPoint2DDouble(posX, posY));

    auto nodePosition = cadPropNode->first_node("NodePosition");
    double nodePosX = XMLParser::GetNodeValueDouble(nodePosition, "X");
    double nodePosY = XMLParser::GetNodeValueDouble(nodePosition, "Y");

    int parentID = XMLParser::GetNodeValueInt(cadPropNode, "ParentID");
    // If the opened power element has no parent, set up the basics CAD properties of the element manually, otherwise
    // just class method AddParent to calculate properly.
    if(parentID == -1) {
        m_parentList.push_back(nullptr);
        m_pointList.push_back(wxPoint2DDouble(nodePosX, nodePosY));
        m_pointList.push_back(wxPoint2DDouble(nodePosX, nodePosY));
        m_pointList.push_back(m_position + wxPoint2DDouble(0.0, -m_height / 2.0 - 10.0));
        m_pointList.push_back(m_position + wxPoint2DDouble(0.0, -m_height / 2.0));

        wxRect2DDouble genRect(0, 0, 0, 0);
        m_switchRect.push_back(genRect);  // Push a general rectangle.
        UpdateSwitches();

        m_online = false;  // Not connected elements are always offline.
    } else {
        AddParent(parentList[parentID], wxPoint2DDouble(nodePosX, nodePosY));
    }

    // Set up the points properly.
    StartMove(m_position);
    Move(wxPoint2DDouble(posX, posY));

    // Set the rotation properly.
    int numRot = angle / m_rotationAngle;
    bool clockwise = true;
    if(numRot < 0) {
        numRot = std::abs(numRot);
        clockwise = false;
    }
    for(int i = 0; i < numRot; i++) Rotate(clockwise);

    return true;
}

void PowerElement::SaveCADProperties(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementNode)
{
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
    auto nodePos = XMLParser::AppendNode(doc, cadProp, "NodePosition");
    auto nodePosX = XMLParser::AppendNode(doc, nodePos, "X");
    XMLParser::SetNodeValue(doc, nodePosX, m_pointList[0].m_x);
    auto nodePosY = XMLParser::AppendNode(doc, nodePos, "Y");
    XMLParser::SetNodeValue(doc, nodePosY, m_pointList[0].m_y);
    auto parentID = XMLParser::AppendNode(doc, cadProp, "ParentID");
    Element* parent = m_parentList[0];
    if(parent) XMLParser::SetNodeValue(doc, parentID, parent->GetID());
}

void PowerElement::SaveSwitchingData(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* electricalNode)
{
    auto switchingList = XMLParser::AppendNode(doc, electricalNode, "SwitchingList");
    for(int i = 0; i < static_cast<int>(m_swData.swType.size()); i++) {
        auto switching = XMLParser::AppendNode(doc, switchingList, "Switching");
        XMLParser::SetNodeAttribute(doc, switching, "ID", i);
        auto swType = XMLParser::AppendNode(doc, switching, "Type");
        XMLParser::SetNodeValue(doc, swType, static_cast<int>(m_swData.swType[i]));
        auto swTime = XMLParser::AppendNode(doc, switching, "Time");
        XMLParser::SetNodeValue(doc, swTime, m_swData.swTime[i]);
    }
}

bool PowerElement::OpenSwitchingData(rapidxml::xml_node<>* electricalNode)
{
    auto switchingList = electricalNode->first_node("SwitchingList");
    if(!switchingList) return false;
    auto swNode = switchingList->first_node("Switching");
    while(swNode) {
        m_swData.swType.push_back((SwitchingType)XMLParser::GetNodeValueInt(swNode, "Type"));
        m_swData.swTime.push_back(XMLParser::GetNodeValueDouble(swNode, "Time"));
        swNode = swNode->next_sibling("Switching");
    }
    return true;
}
