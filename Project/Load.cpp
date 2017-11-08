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
        m_pfDirection = PF_TO_ELEMENT;
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
        for(int i = 0; i < 3; i++) {
            triangPts.push_back(m_triangPts[i] + m_position);
        }
        glPushMatrix();
        glTranslated(m_position.m_x, m_position.m_y, 0.0);
        glRotated(m_angle, 0.0, 0.0, 1.0);
        glTranslated(-m_position.m_x, -m_position.m_y, 0.0);
        glColor4dv(elementColour.GetRGBA());
        DrawTriangle(triangPts);
        glPopMatrix();
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

bool Load::GetPlotData(ElementPlotData& plotData)
{
    if(!m_electricalData.plotLoad) return false;
    plotData.SetName(m_electricalData.name);
    plotData.SetCurveType(ElementPlotData::CT_LOAD);

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
