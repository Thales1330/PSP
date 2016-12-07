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
    if(!m_isMultlineText) { // Only one line
        wxGLString glString(m_text);
        glString.setFont(wxFont(m_fontSize, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        glString.consolidate(&dc);
        glString.bind();
        glString.render(m_position.m_x, m_position.m_y);
    } else { // Multiples lines
        wxGLStringArray glStringArray;
        // Fill the string array.
        for(int i = 0; i < (int)m_multlineText.size(); i++) glStringArray.addString(m_multlineText[i]);
        glStringArray.setFont(wxFont(m_fontSize, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        glStringArray.consolidate(&dc);
        glStringArray.bind();
        // The text will be printed centralized.
        for(int i = 0; i < (int)m_multlineText.size(); i++) {
            glStringArray.get(i).render(m_position.m_x, m_position.m_y - m_height / 2.0 +
                    glStringArray.get(i).getheight() * double(i) + glStringArray.get(i).getheight() / 2.0);
        }
    }

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
    int numberOfLines = m_text.Freq('\n') + 1;
    if(numberOfLines == 1) { // Only one line
        m_isMultlineText = false;
        wxGLString glString(m_text);
        glString.setFont(wxFont(m_fontSize, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        wxScreenDC dc;
        glString.consolidate(&dc);
        glString.bind();

        m_width = glString.getWidth();
        m_height = glString.getheight();
    } else {
        m_isMultlineText = true;
        m_multlineText.clear();
        wxString text = m_text;
        double w = 0.0, h = 0.0;
        for(int i = 0; i < numberOfLines; ++i) {
            wxString nextLine;
            wxString currentLine = text.BeforeFirst('\n', &nextLine);
            text = nextLine;
            m_multlineText.push_back(currentLine);

            wxGLString glString(currentLine);
            glString.setFont(wxFont(m_fontSize, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            wxScreenDC dc;
            glString.consolidate(&dc);
            glString.bind();

            if(w < glString.getWidth()) w = glString.getWidth(); // Get the major width.
            h = glString.getheight();
        }
        m_width = w;
        m_height = h * (double)numberOfLines;
    }

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
                double baseVoltage = data.nominalVoltage;
                if(data.nominalVoltageUnit == UNIT_kV) baseVoltage *= 1e3;
                double baseCurrent = systemPowerBase / (std::sqrt(3.0) * baseVoltage);

                switch(m_dataType) {
                    case DATA_NAME: {
                        SetText(bus->GetEletricalData().name);
                    } break;
                    case DATA_VOLTAGE: {
                        double voltage = std::abs(data.voltage);
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(voltage, m_decimalPlaces) + " p.u.");
                            } break;
                            case UNIT_V: {
                                SetText(wxString::FromDouble(voltage * baseVoltage, m_decimalPlaces) + " V");
                            } break;
                            case UNIT_kV: {
                                SetText(wxString::FromDouble(voltage * baseVoltage / 1e3, m_decimalPlaces) + " kV");
                            } break;
                        }
                    } break;
                    case DATA_ANGLE: {
                        double angle = std::arg(data.voltage);
                        switch(m_unit) {
                            case UNIT_RADIAN: {
                                SetText(wxString::FromDouble(angle, m_decimalPlaces) + " rad");
                            } break;
                            case UNIT_DEGREE: {
                                SetText(wxString::FromDouble(wxRadToDeg(angle), m_decimalPlaces) + "°");
                            } break;
                        }
                    } break;
                    case DATA_SC_CURRENT: {
                        double faultCurrent[3] = { std::abs(data.faultCurrent[0]), std::abs(data.faultCurrent[1]),
                            std::abs(data.faultCurrent[2]) };
                        switch(m_unit) {
                            case UNIT_PU: {
                                wxString str =
                                    "Ia = " + wxString::FromDouble(faultCurrent[0], m_decimalPlaces) + " p.u.";
                                str += "\nIb = " + wxString::FromDouble(faultCurrent[1], m_decimalPlaces) + " p.u.";
                                str += "\nIc = " + wxString::FromDouble(faultCurrent[2], m_decimalPlaces) + " p.u.";
                                SetText(str);
                            } break;
                        }
                    }
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
