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

#include "TextForm.h"
#include "Text.h"

#ifdef USING_WX_3_0_X
#include "DegreesAndRadians.h"
#endif
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

Text::Text() : GraphicalElement() { SetText(m_text); }
Text::Text(wxPoint2DDouble position) : GraphicalElement()
{
    m_position = position;
    SetText(m_text);
}

Text::~Text()
{
    if(m_glString) delete m_glString;
    if(m_glStringArray) delete m_glStringArray;
}
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
        DrawRectangle(m_position + wxPoint2DDouble(m_borderSize / 2.0, m_borderSize / 2.0), m_rect.m_width,
                      m_rect.m_height);
    }

    // Draw text (layer 2)
    glEnable(GL_TEXTURE_2D);
    glColor4d(0.0, 0.0, 0.0, 1.0);
    if(!m_isMultlineText) {  // Only one line
        m_glString->bind();
        m_glString->render(m_position.m_x, m_position.m_y);
    } else {  // Multiples lines
        m_glStringArray->bind();
        // The text will be printed centralized.
        double lineHeight = m_height / (double)m_numberOfLines;
        for(int i = 0; i < m_numberOfLines; i++) {
            m_glStringArray->get(i)
                .render(m_position.m_x, m_position.m_y - m_height / 2.0 + lineHeight / 2.0 + lineHeight * double(i));
        }
    }
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

bool Text::Intersects(wxRect2DDouble rect) const
{
    if(m_angle == 0.0 || m_angle == 180.0) return m_rect.Intersects(rect);
    return RotatedRectanglesIntersects(m_rect, rect, m_angle, 0.0);
}

void Text::SetText(wxString text)
{
    glEnable(GL_TEXTURE_2D);
    m_text = text;
    //wxFont font(m_fontSize, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    wxFont font = wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT);

    wxScreenDC dc;
    GLuint* idString = NULL;
    GLuint* idStringArray = NULL;

    if(m_glString) {
        delete m_glString;
        m_glString = NULL;

        idString = new GLuint;
        glGenTextures(1, idString);
    }
    if(m_glStringArray) {
        delete m_glStringArray;
        m_glStringArray = NULL;

        idStringArray = new GLuint;
        glGenTextures(1, idStringArray);
    }

    m_numberOfLines = m_text.Freq('\n') + 1;
    if(m_numberOfLines == 1) {  // Only one line
        m_isMultlineText = false;
        m_glString = new wxGLString(m_text);
        m_glString->setFont(font);
        m_glString->consolidate(&dc);
        m_width = m_glString->getWidth();
        m_height = m_glString->getheight();
    } else {
        m_isMultlineText = true;
        m_glStringArray = new wxGLStringArray();
        dc.SetFont(font);

        m_width = 0.0;
        m_height = 0.0;
        wxString multText = m_text;
        for(int i = 0; i < m_numberOfLines; ++i) {
            wxString nextLine;
            wxString currentLine = multText.BeforeFirst('\n', &nextLine);
            multText = nextLine;
            m_glStringArray->addString(currentLine);

            wxSize size = dc.GetTextExtent(currentLine);
            if(size.GetWidth() > m_width) m_width = size.GetWidth();
            m_height += size.GetHeight();
        }

        m_glStringArray->setFont(font);
        m_glStringArray->consolidate(&dc);
    }

    if(idString) glDeleteTextures(1, idString);
    if(idStringArray) glDeleteTextures(1, idStringArray);

    // Update text rectangle.
    SetPosition(m_position);
    glDisable(GL_TEXTURE_2D);
}

void Text::Rotate(bool clockwise)
{
    double rotAngle = m_rotationAngle;
    if(!clockwise) rotAngle = -m_rotationAngle;

    m_angle += rotAngle;
    if(m_angle >= 360 || m_angle <= -360) m_angle = 0.0;
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
            SetText(m_text);
            break;
        case TYPE_BUS: {
            Bus* bus = static_cast<Bus*>(m_element);
            if(bus) {
                BusElectricalData data = bus->GetElectricalData();
                double baseVoltage = data.nominalVoltage;
                if(data.nominalVoltageUnit == UNIT_kV) baseVoltage *= 1e3;
                double baseCurrent = systemPowerBase / (std::sqrt(3.0) * baseVoltage);

                switch(m_dataType) {
                    case DATA_NAME: {
                        SetText(bus->GetElectricalData().name);
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
                            default:
                                break;
                        }
                    } break;
                    case DATA_ANGLE: {
                        double angle = std::arg(data.voltage);
                        switch(m_unit) {
                            case UNIT_RADIAN: {
                                SetText(wxString::FromDouble(angle, m_decimalPlaces) + " rad");
                            } break;
                            case UNIT_DEGREE: {
                                SetText(wxString::FromDouble(wxRadToDeg(angle), m_decimalPlaces) + (wxString)L'\u00B0');
                            } break;
                            default:
                                break;
                        }
                    } break;
                    case DATA_SC_CURRENT: {
                        double faultCurrent[3] = {std::abs(data.faultCurrent[0]), std::abs(data.faultCurrent[1]),
                                                  std::abs(data.faultCurrent[2])};
                        switch(m_unit) {
                            case UNIT_PU: {
                                wxString str =
                                    "Ia = " + wxString::FromDouble(faultCurrent[0], m_decimalPlaces) + " p.u.";
                                str += "\nIb = " + wxString::FromDouble(faultCurrent[1], m_decimalPlaces) + " p.u.";
                                str += "\nIc = " + wxString::FromDouble(faultCurrent[2], m_decimalPlaces) + " p.u.";
                                SetText(str);
                            } break;
                            case UNIT_A: {
                                wxString str = "Ia = " +
                                               wxString::FromDouble(faultCurrent[0] * baseCurrent, m_decimalPlaces) +
                                               " A";
                                str += "\nIb = " +
                                       wxString::FromDouble(faultCurrent[1] * baseCurrent, m_decimalPlaces) + " A";
                                str += "\nIc = " +
                                       wxString::FromDouble(faultCurrent[2] * baseCurrent, m_decimalPlaces) + " A";
                                SetText(str);
                            } break;
                            case UNIT_kA: {
                                wxString str =
                                    "Ia = " +
                                    wxString::FromDouble(faultCurrent[0] * baseCurrent / 1e3, m_decimalPlaces) + " kA";
                                str += "\nIb = " +
                                       wxString::FromDouble(faultCurrent[1] * baseCurrent / 1e3, m_decimalPlaces) +
                                       " kA";
                                str += "\nIc = " +
                                       wxString::FromDouble(faultCurrent[2] * baseCurrent / 1e3, m_decimalPlaces) +
                                       " kA";
                                SetText(str);
                            } break;
                            default:
                                break;
                        }
                    } break;
                    case DATA_SC_VOLTAGE: {
                        double faultVoltage[3] = {std::abs(data.faultVoltage[0]), std::abs(data.faultVoltage[1]),
                                                  std::abs(data.faultVoltage[2])};
                        switch(m_unit) {
                            case UNIT_PU: {
                                wxString str =
                                    "Va = " + wxString::FromDouble(faultVoltage[0], m_decimalPlaces) + " p.u.";
                                str += "\nVb = " + wxString::FromDouble(faultVoltage[1], m_decimalPlaces) + " p.u.";
                                str += "\nVc = " + wxString::FromDouble(faultVoltage[2], m_decimalPlaces) + " p.u.";
                                SetText(str);
                            } break;
                            case UNIT_V: {
                                wxString str = "Va = " +
                                               wxString::FromDouble(faultVoltage[0] * baseVoltage, m_decimalPlaces) +
                                               " V";
                                str += "\nVb = " +
                                       wxString::FromDouble(faultVoltage[1] * baseVoltage, m_decimalPlaces) + " V";
                                str += "\nVc = " +
                                       wxString::FromDouble(faultVoltage[2] * baseVoltage, m_decimalPlaces) + " V";
                                SetText(str);
                            } break;
                            case UNIT_kV: {
                                wxString str =
                                    "Va = " +
                                    wxString::FromDouble(faultVoltage[0] * baseVoltage / 1e3, m_decimalPlaces) + " kV";
                                str += "\nVb = " +
                                       wxString::FromDouble(faultVoltage[1] * baseVoltage / 1e3, m_decimalPlaces) +
                                       " kV";
                                str += "\nVc = " +
                                       wxString::FromDouble(faultVoltage[2] * baseVoltage / 1e3, m_decimalPlaces) +
                                       " kV";
                                SetText(str);
                            } break;
                            default:
                                break;
                        }
                    } break;
                    case DATA_SC_POWER: {
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(data.scPower, m_decimalPlaces) + " p.u.");
                            } break;
                            case UNIT_VA: {
                                SetText(wxString::FromDouble(data.scPower * systemPowerBase, m_decimalPlaces) + " VA");
                            } break;
                            case UNIT_kVA: {
                                SetText(wxString::FromDouble(data.scPower * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVA");
                            } break;
                            case UNIT_MVA: {
                                SetText(wxString::FromDouble(data.scPower * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MVA");
                            } break;
                            default:
                                break;
                        }
                    } break;
                    default:
                        break;
                }
            }
        } break;
        case TYPE_SYNC_GENERATOR: {
            SyncGenerator* syncGenerator = static_cast<SyncGenerator*>(m_element);
            if(syncGenerator) {
                SyncGeneratorElectricalData data = syncGenerator->GetPUElectricalData(systemPowerBase);
                double baseVoltage = syncGenerator->GetValueFromUnit(data.nominalVoltage, data.nominalVoltageUnit);
                double baseCurrent = systemPowerBase / (std::sqrt(3.0) * baseVoltage);
                switch(m_dataType) {
                    case DATA_NAME: {
                        SetText(data.name);
                    } break;
                    case DATA_ACTIVE_POWER: {
                        double activePower = data.activePower;
                        if(!syncGenerator->IsOnline()) activePower = 0.0;
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(activePower, m_decimalPlaces) + " p.u.");
                            } break;
                            case UNIT_W: {
                                SetText(wxString::FromDouble(activePower * systemPowerBase, m_decimalPlaces) + " W");
                            } break;
                            case UNIT_kW: {
                                SetText(wxString::FromDouble(activePower * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kW");
                            } break;
                            case UNIT_MW: {
                                SetText(wxString::FromDouble(activePower * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MW");
                            } break;
                            default:
                                break;
                        }
                    } break;
                    case DATA_REACTIVE_POWER: {
                        double reactivePower = data.reactivePower;
                        if(!syncGenerator->IsOnline()) reactivePower = 0.0;
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(reactivePower, m_decimalPlaces) + " p.u.");
                            } break;
                            case UNIT_VAr: {
                                SetText(wxString::FromDouble(reactivePower * systemPowerBase, m_decimalPlaces) +
                                        " VAr");
                            } break;
                            case UNIT_kVAr: {
                                SetText(wxString::FromDouble(reactivePower * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            } break;
                            case UNIT_MVAr: {
                                SetText(wxString::FromDouble(reactivePower * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MVAr");
                            } break;
                            default:
                                break;
                        }
                    } break;
                    case DATA_SC_CURRENT: {
                        double faultCurrent[3] = {std::abs(data.faultCurrent[0]), std::abs(data.faultCurrent[1]),
                                                  std::abs(data.faultCurrent[2])};
                        switch(m_unit) {
                            case UNIT_PU: {
                                wxString str =
                                    "Ia = " + wxString::FromDouble(faultCurrent[0], m_decimalPlaces) + " p.u.";
                                str += "\nIb = " + wxString::FromDouble(faultCurrent[1], m_decimalPlaces) + " p.u.";
                                str += "\nIc = " + wxString::FromDouble(faultCurrent[2], m_decimalPlaces) + " p.u.";
                                SetText(str);
                            } break;
                            case UNIT_A: {
                                wxString str = "Ia = " +
                                               wxString::FromDouble(faultCurrent[0] * baseCurrent, m_decimalPlaces) +
                                               " A";
                                str += "\nIb = " +
                                       wxString::FromDouble(faultCurrent[1] * baseCurrent, m_decimalPlaces) + " A";
                                str += "\nIc = " +
                                       wxString::FromDouble(faultCurrent[2] * baseCurrent, m_decimalPlaces) + " A";
                                SetText(str);
                            } break;
                            case UNIT_kA: {
                                wxString str =
                                    "Ia = " +
                                    wxString::FromDouble(faultCurrent[0] * baseCurrent / 1e3, m_decimalPlaces) + " kA";
                                str += "\nIb = " +
                                       wxString::FromDouble(faultCurrent[1] * baseCurrent / 1e3, m_decimalPlaces) +
                                       " kA";
                                str += "\nIc = " +
                                       wxString::FromDouble(faultCurrent[2] * baseCurrent / 1e3, m_decimalPlaces) +
                                       " kA";
                                SetText(str);
                            } break;
                            default:
                                break;
                        }
                    } break;
                    default:
                        break;
                }
            }
        } break;
        case TYPE_LINE: {
            Line* line = static_cast<Line*>(m_element);
            if(line) {
                LineElectricalData data = line->GetElectricalData();
                double baseVoltage = data.nominalVoltage;
                if(data.nominalVoltageUnit == UNIT_kV) baseVoltage *= 1e3;
                double baseCurrent = systemPowerBase / (std::sqrt(3.0) * baseVoltage);
                switch(m_dataType) {
                    case DATA_NAME: {
                        SetText(data.name);
                    } break;
                    case DATA_PF_ACTIVE: {
                        double activePF = std::real(data.powerFlow[m_direction]);
                        if(!line->IsOnline()) activePF = 0.0;
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(activePF, m_decimalPlaces) + " p.u.");
                            } break;
                            case UNIT_W: {
                                SetText(wxString::FromDouble(activePF * systemPowerBase, m_decimalPlaces) + " W");
                            } break;
                            case UNIT_kW: {
                                SetText(wxString::FromDouble(activePF * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kW");
                            } break;
                            case UNIT_MW: {
                                SetText(wxString::FromDouble(activePF * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MW");
                            } break;
                            default:
                                break;
                        }
                    } break;
                    case DATA_PF_REACTIVE: {
                        double reactivePF = std::imag(data.powerFlow[m_direction]);
                        if(!line->IsOnline()) reactivePF = 0.0;
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(reactivePF, m_decimalPlaces) + " p.u.");
                            } break;
                            case UNIT_VAr: {
                                SetText(wxString::FromDouble(reactivePF * systemPowerBase, m_decimalPlaces) + " VAr");
                            } break;
                            case UNIT_kVAr: {
                                SetText(wxString::FromDouble(reactivePF * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            } break;
                            case UNIT_MVAr: {
                                SetText(wxString::FromDouble(reactivePF * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MVAr");
                            } break;
                            default:
                                break;
                        }
                    } break;
                    case DATA_PF_LOSSES: {
                        double losses = std::abs(std::real(data.powerFlow[0]) + std::real(data.powerFlow[1]));
                        if(!line->IsOnline()) losses = 0.0;
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(losses, m_decimalPlaces) + " p.u.");
                            } break;
                            case UNIT_W: {
                                SetText(wxString::FromDouble(losses * systemPowerBase, m_decimalPlaces) + " W");
                            } break;
                            case UNIT_kW: {
                                SetText(wxString::FromDouble(losses * systemPowerBase / 1e3, m_decimalPlaces) + " kW");
                            } break;
                            case UNIT_MW: {
                                SetText(wxString::FromDouble(losses * systemPowerBase / 1e6, m_decimalPlaces) + " MW");
                            } break;
                            default:
                                break;
                        }
                    } break;
                    case DATA_PF_CURRENT: {
                        double current = std::abs(data.current[m_direction]);
                        if(!line->IsOnline()) current = 0.0;
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(current, m_decimalPlaces) + " p.u.");
                            } break;
                            case UNIT_A: {
                                SetText(wxString::FromDouble(current * baseCurrent, m_decimalPlaces) + " A");
                            } break;
                            case UNIT_kA: {
                                SetText(wxString::FromDouble(current * baseCurrent / 1e3, m_decimalPlaces) + " kA");
                            } break;
                            default:
                                break;
                        }
                    } break;
                    case DATA_SC_CURRENT: {
                        double faultCurrent[3] = {std::abs(data.faultCurrent[m_direction][0]),
                                                  std::abs(data.faultCurrent[m_direction][1]),
                                                  std::abs(data.faultCurrent[m_direction][2])};
                        if(!line->IsOnline()) faultCurrent[0] = faultCurrent[1] = faultCurrent[2] = 0.0;
                        switch(m_unit) {
                            case UNIT_PU: {
                                wxString str =
                                    "Ia = " + wxString::FromDouble(faultCurrent[0], m_decimalPlaces) + " p.u.";
                                str += "\nIb = " + wxString::FromDouble(faultCurrent[1], m_decimalPlaces) + " p.u.";
                                str += "\nIc = " + wxString::FromDouble(faultCurrent[2], m_decimalPlaces) + " p.u.";
                                SetText(str);
                            } break;
                            case UNIT_A: {
                                wxString str = "Ia = " +
                                               wxString::FromDouble(faultCurrent[0] * baseCurrent, m_decimalPlaces) +
                                               " A";
                                str += "\nIb = " +
                                       wxString::FromDouble(faultCurrent[1] * baseCurrent, m_decimalPlaces) + " A";
                                str += "\nIc = " +
                                       wxString::FromDouble(faultCurrent[2] * baseCurrent, m_decimalPlaces) + " A";
                                SetText(str);
                            } break;
                            case UNIT_kA: {
                                wxString str =
                                    "Ia = " +
                                    wxString::FromDouble(faultCurrent[0] * baseCurrent / 1e3, m_decimalPlaces) + " kA";
                                str += "\nIb = " +
                                       wxString::FromDouble(faultCurrent[1] * baseCurrent / 1e3, m_decimalPlaces) +
                                       " kA";
                                str += "\nIc = " +
                                       wxString::FromDouble(faultCurrent[2] * baseCurrent / 1e3, m_decimalPlaces) +
                                       " kA";
                                SetText(str);
                            } break;
                            default:
                                break;
                        }
                    } break;
                    default:
                        break;
                }
            }
        } break;
        case TYPE_TRANSFORMER: {
            Transformer* transformer = static_cast<Transformer*>(m_element);
            if(transformer) {
                TransformerElectricalData data = transformer->GetElectricalData();
                double baseVoltage[2] = {data.primaryNominalVoltage, data.secondaryNominalVoltage};

                if(data.primaryNominalVoltageUnit == UNIT_kV) baseVoltage[0] *= 1e3;
                if(data.secondaryNominalVoltageUnit == UNIT_kV) baseVoltage[1] *= 1e3;

                double baseCurrent[2] = {systemPowerBase / (std::sqrt(3.0) * baseVoltage[0]),
                                         systemPowerBase / (std::sqrt(3.0) * baseVoltage[1])};
                switch(m_dataType) {
                    case DATA_NAME: {
                        SetText(data.name);
                    } break;
                    case DATA_PF_ACTIVE: {
                        double activePF = std::real(data.powerFlow[m_direction]);
                        if(!transformer->IsOnline()) activePF = 0.0;
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(activePF, m_decimalPlaces) + " p.u.");
                            } break;
                            case UNIT_W: {
                                SetText(wxString::FromDouble(activePF * systemPowerBase, m_decimalPlaces) + " W");
                            } break;
                            case UNIT_kW: {
                                SetText(wxString::FromDouble(activePF * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kW");
                            } break;
                            case UNIT_MW: {
                                SetText(wxString::FromDouble(activePF * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MW");
                            } break;
                            default:
                                break;
                        }
                    } break;
                    case DATA_PF_REACTIVE: {
                        double reactivePF = std::imag(data.powerFlow[m_direction]);
                        if(!transformer->IsOnline()) reactivePF = 0.0;
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(reactivePF, m_decimalPlaces) + " p.u.");
                            } break;
                            case UNIT_VAr: {
                                SetText(wxString::FromDouble(reactivePF * systemPowerBase, m_decimalPlaces) + " VAr");
                            } break;
                            case UNIT_kVAr: {
                                SetText(wxString::FromDouble(reactivePF * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            } break;
                            case UNIT_MVAr: {
                                SetText(wxString::FromDouble(reactivePF * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MVAr");
                            } break;
                            default:
                                break;
                        }
                    } break;
                    case DATA_PF_LOSSES: {
                        double losses = std::abs(std::real(data.powerFlow[0]) + std::real(data.powerFlow[1]));
                        if(!transformer->IsOnline()) losses = 0.0;
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(losses, m_decimalPlaces) + " p.u.");
                            } break;
                            case UNIT_W: {
                                SetText(wxString::FromDouble(losses * systemPowerBase, m_decimalPlaces) + " W");
                            } break;
                            case UNIT_kW: {
                                SetText(wxString::FromDouble(losses * systemPowerBase / 1e3, m_decimalPlaces) + " kW");
                            } break;
                            case UNIT_MW: {
                                SetText(wxString::FromDouble(losses * systemPowerBase / 1e6, m_decimalPlaces) + " MW");
                            } break;
                            default:
                                break;
                        }
                    } break;
                    case DATA_PF_CURRENT: {
                        double current = std::abs(data.current[m_direction]);
                        if(!transformer->IsOnline()) current = 0.0;
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(current, m_decimalPlaces) + " p.u.");
                            } break;
                            case UNIT_A: {
                                SetText(wxString::FromDouble(current * baseCurrent[m_direction], m_decimalPlaces) +
                                        " A");
                            } break;
                            case UNIT_kA: {
                                SetText(
                                    wxString::FromDouble(current * baseCurrent[m_direction] / 1e3, m_decimalPlaces) +
                                    " kA");
                            } break;
                            default:
                                break;
                        }
                    } break;
                    case DATA_SC_CURRENT: {
                        double faultCurrent[3] = {std::abs(data.faultCurrent[m_direction][0]),
                                                  std::abs(data.faultCurrent[m_direction][1]),
                                                  std::abs(data.faultCurrent[m_direction][2])};
                        if(!transformer->IsOnline()) faultCurrent[0] = faultCurrent[1] = faultCurrent[2] = 0.0;
                        switch(m_unit) {
                            case UNIT_PU: {
                                wxString str =
                                    "Ia = " + wxString::FromDouble(faultCurrent[0], m_decimalPlaces) + " p.u.";
                                str += "\nIb = " + wxString::FromDouble(faultCurrent[1], m_decimalPlaces) + " p.u.";
                                str += "\nIc = " + wxString::FromDouble(faultCurrent[2], m_decimalPlaces) + " p.u.";
                                SetText(str);
                            } break;
                            case UNIT_A: {
                                wxString str =
                                    "Ia = " +
                                    wxString::FromDouble(faultCurrent[0] * baseCurrent[m_direction], m_decimalPlaces) +
                                    " A";
                                str += "\nIb = " + wxString::FromDouble(faultCurrent[1] * baseCurrent[m_direction],
                                                                        m_decimalPlaces) +
                                       " A";
                                str += "\nIc = " + wxString::FromDouble(faultCurrent[2] * baseCurrent[m_direction],
                                                                        m_decimalPlaces) +
                                       " A";
                                SetText(str);
                            } break;
                            case UNIT_kA: {
                                wxString str = "Ia = " +
                                               wxString::FromDouble(faultCurrent[0] * baseCurrent[m_direction] / 1e3,
                                                                    m_decimalPlaces) +
                                               " kA";
                                str += "\nIb = " +
                                       wxString::FromDouble(faultCurrent[1] * baseCurrent[m_direction] / 1e3,
                                                            m_decimalPlaces) +
                                       " kA";
                                str += "\nIc = " +
                                       wxString::FromDouble(faultCurrent[2] * baseCurrent[m_direction] / 1e3,
                                                            m_decimalPlaces) +
                                       " kA";
                                SetText(str);
                            } break;
                            default:
                                break;
                        }
                    } break;
                    default:
                        break;
                }
            }
        } break;
        case TYPE_LOAD: {
            Load* load = static_cast<Load*>(m_element);
            if(load) {
                LoadElectricalData data = load->GetPUElectricalData(systemPowerBase);
                std::complex<double> sPower(data.activePower, data.reactivePower);
                if(data.loadType == CONST_IMPEDANCE && load->IsOnline()) {
                    std::complex<double> v = static_cast<Bus*>(load->GetParentList()[0])->GetElectricalData().voltage;
                    sPower = std::pow(std::abs(v), 2) * sPower;
                }
                if(!load->IsOnline()) sPower = std::complex<double>(0.0, 0.0);
                switch(m_dataType) {
                    case DATA_NAME: {
                        SetText(data.name);
                    } break;
                    case DATA_ACTIVE_POWER: {
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(sPower.real(), m_decimalPlaces) + " p.u.");
                            }
                            case UNIT_W: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase, m_decimalPlaces) + " W");
                            }
                            case UNIT_kW: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kW");
                            }
                            case UNIT_MW: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MW");
                            }
                            default:
                                break;
                        }
                    } break;
                    case DATA_REACTIVE_POWER: {
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(sPower.imag(), m_decimalPlaces) + " p.u.");
                            }
                            case UNIT_VAr: {
                                SetText(wxString::FromDouble(sPower.imag() * systemPowerBase, m_decimalPlaces) +
                                        " VAr");
                            }
                            case UNIT_kVAr: {
                                SetText(wxString::FromDouble(sPower.imag() * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            }
                            case UNIT_MVAr: {
                                SetText(wxString::FromDouble(sPower.imag() * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MVAr");
                            }
                            default:
                                break;
                        }
                    } break;
                    default:
                        break;
                }
            }
        } break;
        case TYPE_SYNC_MOTOR: {
            SyncMotor* syncMotor = static_cast<SyncMotor*>(m_element);
            if(syncMotor) {
                SyncMotorElectricalData data = syncMotor->GetPUElectricalData(systemPowerBase);
                std::complex<double> sPower(data.activePower, data.reactivePower);
                if(!syncMotor->IsOnline()) sPower = std::complex<double>(0.0, 0.0);
                switch(m_dataType) {
                    case DATA_NAME: {
                        SetText(data.name);
                    } break;
                    case DATA_ACTIVE_POWER: {
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(sPower.real(), m_decimalPlaces) + " p.u.");
                            }
                            case UNIT_W: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase, m_decimalPlaces) + " W");
                            }
                            case UNIT_kW: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kW");
                            }
                            case UNIT_MW: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MW");
                            }
                            default:
                                break;
                        }
                    } break;
                    case DATA_REACTIVE_POWER: {
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(sPower.imag(), m_decimalPlaces) + " p.u.");
                            }
                            case UNIT_VAr: {
                                SetText(wxString::FromDouble(sPower.imag() * systemPowerBase, m_decimalPlaces) +
                                        " VAr");
                            }
                            case UNIT_kVAr: {
                                SetText(wxString::FromDouble(sPower.imag() * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            }
                            case UNIT_MVAr: {
                                SetText(wxString::FromDouble(sPower.imag() * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MVAr");
                            }
                            default:
                                break;
                        }
                    } break;
                    default:
                        break;
                }
            }
        } break;
        case TYPE_IND_MOTOR: {
            IndMotor* indMotor = static_cast<IndMotor*>(m_element);
            if(indMotor) {
                IndMotorElectricalData data = indMotor->GetPUElectricalData(systemPowerBase);
                std::complex<double> sPower(data.activePower, data.reactivePower);
                if(!indMotor->IsOnline()) sPower = std::complex<double>(0.0, 0.0);
                switch(m_dataType) {
                    case DATA_NAME: {
                        SetText(data.name);
                    } break;
                    case DATA_ACTIVE_POWER: {
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(sPower.real(), m_decimalPlaces) + " p.u.");
                            }
                            case UNIT_W: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase, m_decimalPlaces) + " W");
                            }
                            case UNIT_kW: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kW");
                            }
                            case UNIT_MW: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MW");
                            }
                            default:
                                break;
                        }
                    } break;
                    case DATA_REACTIVE_POWER: {
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(sPower.imag(), m_decimalPlaces) + " p.u.");
                            }
                            case UNIT_VAr: {
                                SetText(wxString::FromDouble(sPower.imag() * systemPowerBase, m_decimalPlaces) +
                                        " VAr");
                            }
                            case UNIT_kVAr: {
                                SetText(wxString::FromDouble(sPower.imag() * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            }
                            case UNIT_MVAr: {
                                SetText(wxString::FromDouble(sPower.imag() * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MVAr");
                            }
                            default:
                                break;
                        }
                    } break;
                    default:
                        break;
                }
            }
        } break;
        case TYPE_CAPACITOR: {
            Capacitor* capacitor = static_cast<Capacitor*>(m_element);
            if(capacitor) {
                CapacitorElectricalData data = capacitor->GetPUElectricalData(systemPowerBase);
                double reativePower = data.reactivePower;
                if(!capacitor->IsOnline())
                    reativePower = 0.0;
                else {
                    std::complex<double> v =
                        static_cast<Bus*>(capacitor->GetParentList()[0])->GetElectricalData().voltage;
                    reativePower *= std::pow(std::abs(v), 2);
                }
                switch(m_dataType) {
                    case DATA_NAME: {
                        SetText(data.name);
                    } break;
                    case DATA_REACTIVE_POWER: {
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(reativePower, m_decimalPlaces) + " p.u.");
                            }
                            case UNIT_VAr: {
                                SetText(wxString::FromDouble(reativePower * systemPowerBase, m_decimalPlaces) + " VAr");
                            }
                            case UNIT_kVAr: {
                                SetText(wxString::FromDouble(reativePower * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            }
                            case UNIT_MVAr: {
                                SetText(wxString::FromDouble(reativePower * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MVAr");
                            }
                            default:
                                break;
                        }
                    } break;
                    default:
                        break;
                }
            }
        } break;
        case TYPE_INDUCTOR: {
            Inductor* inductor = static_cast<Inductor*>(m_element);
            if(inductor) {
                InductorElectricalData data = inductor->GetPUElectricalData(systemPowerBase);
                double reativePower = data.reactivePower;
                if(!inductor->IsOnline())
                    reativePower = 0.0;
                else {
                    std::complex<double> v =
                        static_cast<Bus*>(inductor->GetParentList()[0])->GetElectricalData().voltage;
                    reativePower *= std::pow(std::abs(v), 2);
                }
                switch(m_dataType) {
                    case DATA_NAME: {
                        SetText(data.name);
                    } break;
                    case DATA_REACTIVE_POWER: {
                        switch(m_unit) {
                            case UNIT_PU: {
                                SetText(wxString::FromDouble(reativePower, m_decimalPlaces) + " p.u.");
                            }
                            case UNIT_VAr: {
                                SetText(wxString::FromDouble(reativePower * systemPowerBase, m_decimalPlaces) + " VAr");
                            }
                            case UNIT_kVAr: {
                                SetText(wxString::FromDouble(reativePower * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            }
                            case UNIT_MVAr: {
                                SetText(wxString::FromDouble(reativePower * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MVAr");
                            }
                            default:
                                break;
                        }
                    } break;
                    default:
                        break;
                }
            }
        } break;
    }
}

Element* Text::GetCopy()
{
    Text* copy = new Text();
    *copy = *this;
    // The pointers to wxGLString must be different or can cause crashes.
    copy->m_glString = NULL;
    copy->m_glStringArray = NULL;
    copy->SetText(copy->m_text);
    return copy;
}
