#include "TextForm.h"
#include "Text.h"

#include "ElectricCalculation.h"
#include "Bus.h"
#include "Line.h"
#include "Transformer.h"
#include "SyncGenerator.h"
#include "IndMotor.h"
#include "SyncMotor.h"
#include "Load.h"
#include "Inductor.h"
#include "Capacitor.h"

Text::Text()
    : Element()
{
    SetText(m_text);
}
Text::Text(wxPoint2DDouble position)
    : Element()
{
    SetText(m_text);
    SetPosition(position);
}

Text::~Text() {}
bool Text::Contains(wxPoint2DDouble position) const
{
    wxPoint2DDouble ptR = RotateAtPosition(position, -m_angle);
    return m_rect.Contains(ptR);
}

void Text::Draw(wxPoint2DDouble translation, double scale)
{
    wxScreenDC dc;

    // Draw selection rectangle

    // Push the current matrix on stack.
    glPushMatrix();
    // Rotate the matrix around the object position.
    glTranslated(m_position.m_x, m_position.m_y, 0.0);
    glRotated(m_angle, 0.0, 0.0, 1.0);
    glTranslated(-m_position.m_x, -m_position.m_y, 0.0);

    if(m_selected) {
        glColor4d(0.0, 0.5, 1.0, 0.5);
        DrawRectangle(
            m_position + wxPoint2DDouble(m_borderSize / 2.0, m_borderSize / 2.0), m_rect.m_width, m_rect.m_height);
    }

    // Draw text (layer 2)

    glColor4d(0.0, 0.0, 0.0, 1.0);
    wxGLString glString(m_text);
    glString.setFont(wxFont(m_fontSize, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    glString.consolidate(&dc);
    glString.bind();
    glString.render(m_position.m_x, m_position.m_y);

    glPopMatrix();
}

bool Text::Intersects(wxRect2DDouble rect) const
{
    if(m_angle == 0.0 || m_angle == 180.0) return m_rect.Intersects(rect);
    return RotatedRectanglesIntersects(m_rect, rect, m_angle, 0.0);
}

void Text::SetText(wxString text)
{
    m_text = text;

    // Creating a glString to get the text size.
    wxGLString glString(m_text);
    glString.setFont(wxFont(m_fontSize, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    wxScreenDC dc;
    glString.consolidate(&dc);
    glString.bind();

    m_width = glString.getWidth();
    m_height = glString.getheight();

    // Update text rectangle.
    SetPosition(m_position);
}

void Text::Rotate(bool clockwise)
{
    double rotAngle = m_rotationAngle;
    if(!clockwise) rotAngle = -m_rotationAngle;

    m_angle += rotAngle;
    if(m_angle >= 360.0) m_angle = 0.0;
}

bool Text::ShowForm(wxWindow* parent, std::vector<Element*> elementList)
{
    TextForm* textForm = new TextForm(parent, this, elementList);
    if(textForm->ShowModal() == wxID_OK) {
        textForm->Destroy();
        return true;
    }
    textForm->Destroy();
    return false;
}

void Text::UpdateText(double systemPowerBase)
{
    switch(m_elementType) {
        case TYPE_NONE:
            break;
        case TYPE_BUS: {
            Bus* bus = (Bus*)m_element;
            if(bus) {
                BusElectricalData data = bus->GetEletricalData();
                switch(m_dataType) {
                    case DATA_NAME: {
                        SetText(bus->GetEletricalData().name);
                    } break;
                    case DATA_VOLTAGE: {
                        double baseVoltage = data.nominalVoltage;
                        if(data.nominalVoltageUnit == UNIT_kV) baseVoltage *= 1e3;
                        double voltage = std::abs(data.voltage);
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(voltage, m_decimalPlaces) + _(" p.u."));
                            } break;
                            case UNIT_V: {
                                SetText(wxString::FromDouble(voltage * baseVoltage, m_decimalPlaces) + _(" V"));
                            } break;
                            case UNIT_kV: {
                                SetText(wxString::FromDouble(voltage * baseVoltage / 1e3, m_decimalPlaces) + _(" kV"));
                            } break;
                        }
                    } break;
                }
            }
        } break;
        case TYPE_SYNC_GENERATOR: {

        } break;
        case TYPE_LINE: {

        } break;
        case TYPE_TRANSFORMER: {

        } break;
        case TYPE_LOAD: {

        } break;
        case TYPE_SYNC_MOTOR: {

        } break;
        case TYPE_IND_MOTOR: {

        } break;
        case TYPE_CAPACITOR: {

        } break;
        case TYPE_INDUCTOR: {

        } break;
    }
}
