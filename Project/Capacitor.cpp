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

#include "ReactiveShuntElementForm.h"
#include "Capacitor.h"

Capacitor::Capacitor() : Shunt() {}
Capacitor::Capacitor(wxString name) : Shunt() { m_electricalData.name = name; }
Capacitor::~Capacitor() {}
bool Capacitor::AddParent(Element* parent, wxPoint2DDouble position)
{
    if(parent) {
        m_parentList.push_back(parent);
        parent->AddChild(this);
        wxPoint2DDouble parentPt =
            parent->RotateAtPosition(position, -parent->GetAngle());        // Rotate click to horizontal position.
        parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
        parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.

        m_position = parentPt + wxPoint2DDouble(0.0, 100.0);  // Shifts the position to the down of the bus.
        m_width = 40;
        m_height = 30;
        m_rect = wxRect2DDouble(m_position.m_x - m_width / 2.0, m_position.m_y - m_height / 2.0, m_width, m_height);

        m_pointList.push_back(parentPt);
        m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_position));
        m_pointList.push_back(m_position + wxPoint2DDouble(0.0, -m_height / 2.0 - 10.0));
        m_pointList.push_back(m_position + wxPoint2DDouble(0.0, -m_height / 2.0));

        m_inserted = true;

        wxRect2DDouble genRect(0, 0, 0, 0);
        m_switchRect.push_back(genRect);  // Push a general rectangle.
        UpdateSwitches();

        return true;
    }
    return false;
}

void Capacitor::Draw(wxPoint2DDouble translation, double scale) const
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
        std::vector<wxPoint2DDouble> capPts;
        capPts.push_back(wxPoint2DDouble(m_position.m_x - m_width / 2.0, m_position.m_y - m_height / 2.0));
        capPts.push_back(wxPoint2DDouble(m_position.m_x + m_width / 2.0, m_position.m_y - m_height / 2.0));
        capPts.push_back(wxPoint2DDouble(m_position.m_x - m_width / 2.0, m_position.m_y - m_height / 2.0 + 10.0));
        capPts.push_back(wxPoint2DDouble(m_position.m_x + m_width / 2.0, m_position.m_y - m_height / 2.0 + 10.0));

        if(m_selected) {
            glLineWidth(1.5 + m_borderSize * 2.0);
            glColor4dv(m_selectionColour.GetRGBA());

            DrawLine(m_pointList);

            glPushMatrix();
            glTranslated(m_position.m_x, m_position.m_y, 0.0);
            glRotated(m_angle, 0.0, 0.0, 1.0);
            glTranslated(-m_position.m_x, -m_position.m_y, 0.0);

            DrawLine(capPts, GL_LINES);

            DrawGround(m_position + wxPoint2DDouble(0, -m_height / 2.0 + 10.0));

            glPopMatrix();

            // Draw node selection.
            DrawCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
        }
        // Draw Capacitor (layer 2).
        glLineWidth(1.5);
        glColor4dv(elementColour.GetRGBA());
        DrawCircle(m_pointList[0], 5.0, 10, GL_POLYGON);
        DrawLine(m_pointList);

        DrawSwitches();

        glPushMatrix();
        glTranslated(m_position.m_x, m_position.m_y, 0.0);
        glRotated(m_angle, 0.0, 0.0, 1.0);
        glTranslated(-m_position.m_x, -m_position.m_y, 0.0);

        glColor4dv(elementColour.GetRGBA());
        DrawLine(capPts, GL_LINES);

        DrawGround(m_position + wxPoint2DDouble(0, -m_height / 2.0 + 10.0));

        glPopMatrix();
    }
}

void Capacitor::Rotate(bool clockwise)
{
    double rotAngle = m_rotationAngle;
    if(!clockwise) rotAngle = -m_rotationAngle;

    m_angle += rotAngle;
    if(m_angle >= 360 || m_angle <= -360) m_angle = 0.0;
    m_pointList[2] = RotateAtPosition(m_pointList[2], rotAngle);
    m_pointList[3] = RotateAtPosition(m_pointList[3], rotAngle);
    UpdateSwitchesPosition();
}

bool Capacitor::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_ELEMENT, _("Edit Capacitor"));
    GeneralMenuItens(menu);
    return true;
}

bool Capacitor::Contains(wxPoint2DDouble position) const
{
    wxPoint2DDouble ptR = RotateAtPosition(position, -m_angle);
    return m_rect.Contains(ptR);
}

bool Capacitor::Intersects(wxRect2DDouble rect) const
{
    return RotatedRectanglesIntersects(m_rect, rect, m_angle, 0.0);
}

bool Capacitor::ShowForm(wxWindow* parent, Element* element)
{
    ReactiveShuntElementForm* capacitorForm = new ReactiveShuntElementForm(parent, this);
    capacitorForm->SetTitle(_("Capacitor"));
    if(capacitorForm->ShowModal() == wxID_OK) {
        capacitorForm->Destroy();
        return true;
    }
    capacitorForm->Destroy();
    return false;
}

CapacitorElectricalData Capacitor::GetPUElectricalData(double systemPowerBase)
{
    CapacitorElectricalData data = m_electricalData;
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

Element* Capacitor::GetCopy()
{
    Capacitor* copy = new Capacitor();
    *copy = *this;
    return copy;
}

wxString Capacitor::GetTipText() const
{
    wxString tipText = m_electricalData.name;

    // TODO: Avoid reactive power calculation.
    double reactivePower = m_electricalData.reactivePower;
    if(!m_online)
        reactivePower = 0.0;
    else {
        std::complex<double> v = static_cast<Bus*>(m_parentList[0])->GetElectricalData().voltage;
        reactivePower *= std::pow(std::abs(v), 2);
    }
    tipText += "\n";
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
