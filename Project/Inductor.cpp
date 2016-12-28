#include "ReactiveShuntElementForm.h"
#include "Inductor.h"

Inductor::Inductor() : Shunt() {}
Inductor::Inductor(wxString name) : Shunt() { m_electricalData.name = name; }
Inductor::~Inductor() {}
bool Inductor::AddParent(Element* parent, wxPoint2DDouble position)
{
    if(parent) {
        m_parentList.push_back(parent);
        wxPoint2DDouble parentPt =
            parent->RotateAtPosition(position, -parent->GetAngle());        // Rotate click to horizontal position.
        parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
        parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.

        m_position = parentPt + wxPoint2DDouble(0.0, 100.0);  // Shifts the position to the down of the bus.
        m_width = 20.0;
        m_height = 70.0;
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

void Inductor::Draw(wxPoint2DDouble translation, double scale) const
{
    OpenGLColour* elementColour;
    if(m_online) elementColour = m_onlineElementColour;
    else elementColour = m_offlineElementColour;
    
    if(m_inserted) {
        if(m_selected) {
            glLineWidth(1.5 + m_borderSize * 2.0);
            glColor4dv(m_selectionColour->GetRGBA());

            DrawLine(m_pointList);

            glPushMatrix();
            glTranslated(m_position.m_x, m_position.m_y, 0.0);
            glRotated(m_angle, 0.0, 0.0, 1.0);
            glTranslated(-m_position.m_x, -m_position.m_y, 0.0);

            DrawArc(m_position + wxPoint2DDouble(0, -m_height / 2.0 + 10.0), 10, 45, 270, 30, GL_LINE_STRIP);
            DrawArc(m_position + wxPoint2DDouble(0, -m_height / 2.0 + 25.0), 10, 45, 315, 30, GL_LINE_STRIP);
            DrawArc(m_position + wxPoint2DDouble(0, -m_height / 2.0 + 40.0), 10, 90, 315, 30, GL_LINE_STRIP);

            DrawGround(m_position + wxPoint2DDouble(0, -m_height / 2.0 + 50.0));

            glPopMatrix();

            // Draw node selection.
            DrawCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
        }
        // Draw Load (layer 2).
        glLineWidth(1.5);
        glColor4dv(elementColour->GetRGBA());
        DrawCircle(m_pointList[0], 5.0, 10, GL_POLYGON);
        DrawLine(m_pointList);

        DrawSwitches();

        glPushMatrix();
        glTranslated(m_position.m_x, m_position.m_y, 0.0);
        glRotated(m_angle, 0.0, 0.0, 1.0);
        glTranslated(-m_position.m_x, -m_position.m_y, 0.0);

        glColor4dv(elementColour->GetRGBA());
        DrawArc(m_position + wxPoint2DDouble(0, -m_height / 2.0 + 10.0), 10, 45, 270, 10, GL_LINE_STRIP);
        DrawArc(m_position + wxPoint2DDouble(0, -m_height / 2.0 + 25.0), 10, 45, 315, 10, GL_LINE_STRIP);
        DrawArc(m_position + wxPoint2DDouble(0, -m_height / 2.0 + 40.0), 10, 90, 315, 10, GL_LINE_STRIP);

        DrawGround(m_position + wxPoint2DDouble(0, -m_height / 2.0 + 50.0));

        glPopMatrix();
    }
}

void Inductor::Rotate(bool clockwise)
{
    double rotAngle = m_rotationAngle;
    if(!clockwise) rotAngle = -m_rotationAngle;

    m_angle += rotAngle;
    if(m_angle >= 360 || m_angle <= -360) m_angle = 0.0;
    m_pointList[2] = RotateAtPosition(m_pointList[2], rotAngle);
    m_pointList[3] = RotateAtPosition(m_pointList[3], rotAngle);
    UpdateSwitchesPosition();
}

bool Inductor::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_INDUCTOR, _("Edit Inductor"));
    GeneralMenuItens(menu);
    return true;
}

bool Inductor::Contains(wxPoint2DDouble position) const
{
    wxPoint2DDouble ptR = RotateAtPosition(position, -m_angle);
    return m_rect.Contains(ptR);
}

bool Inductor::Intersects(wxRect2DDouble rect) const { return RotatedRectanglesIntersects(m_rect, rect, m_angle, 0.0); }
bool Inductor::ShowForm(wxWindow* parent, Element* element)
{
    ReactiveShuntElementForm* capacitorForm = new ReactiveShuntElementForm(parent, this);
    capacitorForm->SetTitle(_("Inductor"));
    if(capacitorForm->ShowModal() == wxID_OK) {
        capacitorForm->Destroy();
        return true;
    }
    capacitorForm->Destroy();
    return false;
}

InductorElectricalData Inductor::GetPUElectricalData(double systemPowerBase)
{
    InductorElectricalData data = m_electricalData;
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
