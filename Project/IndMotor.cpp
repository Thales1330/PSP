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
#include "IndMotorForm.h"

IndMotor::IndMotor() : Machines() {}
IndMotor::IndMotor(wxString name) : Machines() { m_electricalData.name = name; }
IndMotor::~IndMotor() {}
void IndMotor::DrawSymbol() const
{
    std::vector<wxPoint2DDouble> mPts;
    mPts.push_back(wxPoint2DDouble(-10, 13) + m_position);
    mPts.push_back(wxPoint2DDouble(-10, -13) + m_position);
    mPts.push_back(wxPoint2DDouble(0, 2) + m_position);
    mPts.push_back(wxPoint2DDouble(10, -13) + m_position);
    mPts.push_back(wxPoint2DDouble(10, 13) + m_position);
    DrawLine(mPts);
}

bool IndMotor::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_ELEMENT, _("Edit induction motor"));
    GeneralMenuItens(menu);
    return true;
}

bool IndMotor::ShowForm(wxWindow* parent, Element* element)
{
    IndMotorForm* indMotorForm = new IndMotorForm(parent, this);
    if(indMotorForm->ShowModal() == wxID_OK) {
        indMotorForm->Destroy();
        return true;
    }
    indMotorForm->Destroy();
    return false;
}

IndMotorElectricalData IndMotor::GetPUElectricalData(double systemPowerBase)
{
    IndMotorElectricalData data = m_electricalData;

    switch(data.activePowerUnit) {
        case UNIT_W: {
            data.activePower = data.activePower / systemPowerBase;
            data.activePowerUnit = UNIT_PU;
        } break;
        case UNIT_kW: {
            data.activePower = (data.activePower * 1e3) / systemPowerBase;
            data.activePowerUnit = UNIT_PU;
        } break;
        case UNIT_MW: {
            data.activePower = (data.activePower * 1e6) / systemPowerBase;
            data.activePowerUnit = UNIT_PU;
        } break;
        default:
            break;
    }
    switch(data.reactivePowerUnit) {
        case UNIT_VAr: {
            data.reactivePower = data.reactivePower / systemPowerBase;
            data.reactivePowerUnit = UNIT_PU;
        } break;
        case UNIT_kVAr: {
            data.reactivePower = (data.reactivePower * 1e3) / systemPowerBase;
            data.reactivePowerUnit = UNIT_PU;
        } break;
        case UNIT_MVAr: {
            data.reactivePower = (data.reactivePower * 1e6) / systemPowerBase;
            data.reactivePowerUnit = UNIT_PU;
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
    if(!m_online) activePower = 0.0;
    tipText += _("\nP = ") + wxString::FromDouble(activePower, 5);
    switch(m_electricalData.activePowerUnit) {
        case UNIT_PU: {
            tipText += _(" p.u.");
        } break;
        case UNIT_W: {
            tipText += _(" W");
        } break;
        case UNIT_kW: {
            tipText += _(" kW");
        } break;
        case UNIT_MW: {
            tipText += _(" MW");
        } break;
        default:
            break;
    }
    double reactivePower = m_electricalData.reactivePower;
    if(!m_online) reactivePower = 0.0;
    tipText += _("\nQ = ") + wxString::FromDouble(reactivePower, 5);
    switch(m_electricalData.reactivePowerUnit) {
        case UNIT_PU: {
            tipText += _(" p.u.");
        } break;
        case UNIT_VAr: {
            tipText += _(" VAr");
        } break;
        case UNIT_kVAr: {
            tipText += _(" kVAr");
        } break;
        case UNIT_MVAr: {
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
    auto electricalProp = XMLParser::AppendNode(doc, elementNode, "ElectricalProperties");
    auto isOnline = XMLParser::AppendNode(doc, electricalProp, "IsOnline");
    XMLParser::SetNodeValue(doc, isOnline, m_online);
    auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
    XMLParser::SetNodeValue(doc, name, m_electricalData.name);
    auto activePower = XMLParser::AppendNode(doc, electricalProp, "ActivePower");
    XMLParser::SetNodeValue(doc, activePower, m_electricalData.activePower);
    XMLParser::SetNodeAttribute(doc, activePower, "UnitID", m_electricalData.activePowerUnit);
    auto reactivePower = XMLParser::AppendNode(doc, electricalProp, "ReactivePower");
    XMLParser::SetNodeValue(doc, reactivePower, m_electricalData.reactivePower);
    XMLParser::SetNodeAttribute(doc, reactivePower, "UnitID", m_electricalData.reactivePowerUnit);

    return elementNode;
}

bool IndMotor::OpenElement(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList)
{
    if(!OpenCADProperties(elementNode, parentList)) return false;

    auto electricalProp = elementNode->first_node("ElectricalProperties");
    if(!electricalProp) return false;

    // Element properties
    SetOnline(XMLParser::GetNodeValueInt(electricalProp, "IsOnline"));
    m_electricalData.name = electricalProp->first_node("Name")->value();
    m_electricalData.activePower = XMLParser::GetNodeValueDouble(electricalProp, "ActivePower");
    m_electricalData.activePowerUnit =
        static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "ActivePower", "UnitID"));
    m_electricalData.reactivePower = XMLParser::GetNodeValueDouble(electricalProp, "ReactivePower");
    m_electricalData.reactivePowerUnit =
        static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(electricalProp, "ReactivePower", "UnitID"));

    m_inserted = true;

    return true;
}
