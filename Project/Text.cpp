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

#include "Text.h"
#include "TextForm.h"

#ifdef USING_WX_3_0_X
#include "DegreesAndRadians.h"
#endif
#include "Bus.h"
#include "Capacitor.h"
#include "ElectricCalculation.h"
#include "IndMotor.h"
#include "Inductor.h"
#include "Line.h"
#include "Load.h"
#include "SyncGenerator.h"
#include "SyncMotor.h"
#include "Transformer.h"

Text::Text() : GraphicalElement() { SetText(m_text); }
Text::Text(wxPoint2DDouble position) : GraphicalElement()
{
    m_position = position;
    SetText(m_text);
}

Text::~Text() {}
bool Text::Contains(wxPoint2DDouble position) const
{
    wxPoint2DDouble ptR = RotateAtPosition(position, -m_angle);
    return m_rect.Contains(ptR);
}

void Text::Draw(wxPoint2DDouble translation, double scale)
{
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
    glColor4d(0.0, 0.0, 0.0, 1.0);
    if(m_isMultlineText) {
        for(unsigned int i = 0; i < m_openGLTextList.size(); ++i) {
            m_openGLTextList[i]->Draw(
                m_position +
                wxPoint2DDouble(0.0, (m_height * static_cast<double>(i) / static_cast<double>(m_numberOfLines)) -
                                         (m_height * static_cast<double>(m_numberOfLines - 1) /
                                          static_cast<double>(2 * m_numberOfLines))));
        }
    } else if(m_openGLTextList.size() > 0) {
        m_openGLTextList[0]->Draw(m_position);
    }
    glPopMatrix();
}

void Text::DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc)
{
	// Draw selection rectangle

	// Push the current matrix on stack.
	gc->PushState();
	// Rotate the matrix around the object position.
	gc->Translate(m_position.m_x, m_position.m_y);
	gc->Rotate(wxDegToRad(m_angle));
	gc->Translate(-m_position.m_x, -m_position.m_y);

	if (m_selected) {
		glColor4d(0.0, 0.5, 1.0, 0.5);
        gc->SetPen(*wxTRANSPARENT_PEN);
        gc->SetBrush(wxBrush(wxColour(0, 125, 255, 125)));
        wxPoint2DDouble pos = m_position - wxPoint2DDouble(m_borderSize / 2.0 + m_width / 2, m_borderSize / 2.0 + m_height / 2);
        gc->DrawRectangle(pos.m_x, pos.m_y, m_rect.m_width, m_rect.m_height);
	}

	// Draw text (layer 2)
	//gc->SetPen(wxPen(wxColour(0, 0, 0, 255)));
	//gc->SetBrush(*wxTRANSPARENT_BRUSH);
    wxPoint2DDouble pos = m_position - wxPoint2DDouble(m_width / 2, m_height / 2);
	if (m_isMultlineText) {
		for (unsigned int i = 0; i < m_openGLTextList.size(); ++i) {
			m_openGLTextList[i]->DrawDC(
                pos +
				wxPoint2DDouble(0.0, (m_height * static_cast<double>(i) / static_cast<double>(m_numberOfLines))), gc);
		}
	}
	else if (m_openGLTextList.size() > 0) {
		m_openGLTextList[0]->DrawDC(pos, gc);
	}
    gc->PopState();
}

bool Text::Intersects(wxRect2DDouble rect) const
{
    if(m_angle == 0.0 || m_angle == 180.0) return m_rect.Intersects(rect);
    return RotatedRectanglesIntersects(m_rect, rect, m_angle, 0.0);
}

void Text::SetText(wxString text)
{
    m_text = text;

    // Clear OpenGL text list
    for(auto it = m_openGLTextList.begin(), itEnd = m_openGLTextList.end(); it != itEnd; ++it) { delete *it; }
    m_openGLTextList.clear();

    m_numberOfLines = m_text.Freq('\n') + 1;
    if(m_numberOfLines > 1) m_isMultlineText = true;
    m_width = 0.0;
    m_height = 0.0;
    wxString multText = m_text;
    for(int i = 0; i < m_numberOfLines; ++i) {
        wxString nextLine;
        wxString currentLine = multText.BeforeFirst('\n', &nextLine);
        multText = nextLine;
        m_openGLTextList.push_back(new OpenGLText(currentLine));
        if(m_openGLTextList[i]->GetWidth() > m_width) m_width = m_openGLTextList[i]->GetWidth();
        m_height += m_openGLTextList[i]->GetHeight();
    }
    SetPosition(m_position);  // Update element rectangle.
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
                if(data.nominalVoltageUnit == ElectricalUnit::UNIT_kV) baseVoltage *= 1e3;
                double baseCurrent = systemPowerBase / (std::sqrt(3.0) * baseVoltage);

                switch(m_dataType) {
                    case DATA_NAME: {
                        SetText(bus->GetElectricalData().name);
                    } break;
                    case DATA_VOLTAGE: {
                        double voltage = std::abs(data.voltage);
                        switch(m_unit) {
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(voltage, m_decimalPlaces) + " p.u.");
                            } break;
                            case ElectricalUnit::UNIT_V: {
                                SetText(wxString::FromDouble(voltage * baseVoltage, m_decimalPlaces) + " V");
                            } break;
                            case ElectricalUnit::UNIT_kV: {
                                SetText(wxString::FromDouble(voltage * baseVoltage / 1e3, m_decimalPlaces) + " kV");
                            } break;
                            default:
                                break;
                        }
                    } break;
                    case DATA_ANGLE: {
                        double angle = std::arg(data.voltage);
                        switch(m_unit) {
                            case ElectricalUnit::UNIT_RADIAN: {
                                SetText(wxString::FromDouble(angle, m_decimalPlaces) + " rad");
                            } break;
                            case ElectricalUnit::UNIT_DEGREE: {
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
                            case ElectricalUnit::UNIT_PU: {
                                wxString str =
                                    "Ia = " + wxString::FromDouble(faultCurrent[0], m_decimalPlaces) + " p.u.";
                                str += "\nIb = " + wxString::FromDouble(faultCurrent[1], m_decimalPlaces) + " p.u.";
                                str += "\nIc = " + wxString::FromDouble(faultCurrent[2], m_decimalPlaces) + " p.u.";
                                SetText(str);
                            } break;
                            case ElectricalUnit::UNIT_A: {
                                wxString str =
                                    "Ia = " + wxString::FromDouble(faultCurrent[0] * baseCurrent, m_decimalPlaces) +
                                    " A";
                                str += "\nIb = " +
                                       wxString::FromDouble(faultCurrent[1] * baseCurrent, m_decimalPlaces) + " A";
                                str += "\nIc = " +
                                       wxString::FromDouble(faultCurrent[2] * baseCurrent, m_decimalPlaces) + " A";
                                SetText(str);
                            } break;
                            case ElectricalUnit::UNIT_kA: {
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
                            case ElectricalUnit::UNIT_PU: {
                                wxString str =
                                    "Va = " + wxString::FromDouble(faultVoltage[0], m_decimalPlaces) + " p.u.";
                                str += "\nVb = " + wxString::FromDouble(faultVoltage[1], m_decimalPlaces) + " p.u.";
                                str += "\nVc = " + wxString::FromDouble(faultVoltage[2], m_decimalPlaces) + " p.u.";
                                SetText(str);
                            } break;
                            case ElectricalUnit::UNIT_V: {
                                wxString str =
                                    "Va = " + wxString::FromDouble(faultVoltage[0] * baseVoltage, m_decimalPlaces) +
                                    " V";
                                str += "\nVb = " +
                                       wxString::FromDouble(faultVoltage[1] * baseVoltage, m_decimalPlaces) + " V";
                                str += "\nVc = " +
                                       wxString::FromDouble(faultVoltage[2] * baseVoltage, m_decimalPlaces) + " V";
                                SetText(str);
                            } break;
                            case ElectricalUnit::UNIT_kV: {
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
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(data.scPower, m_decimalPlaces) + " p.u.");
                            } break;
                            case ElectricalUnit::UNIT_VA: {
                                SetText(wxString::FromDouble(data.scPower * systemPowerBase, m_decimalPlaces) + " VA");
                            } break;
                            case ElectricalUnit::UNIT_kVA: {
                                SetText(wxString::FromDouble(data.scPower * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVA");
                            } break;
                            case ElectricalUnit::UNIT_MVA: {
                                SetText(wxString::FromDouble(data.scPower * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MVA");
                            } break;
                            default:
                                break;
                        }
                    } break;
                    case DATA_PQ_THD: {
                        SetText("THD = " + wxString::FromDouble(data.thd, m_decimalPlaces) + "%");
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
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(activePower, m_decimalPlaces) + " p.u.");
                            } break;
                            case ElectricalUnit::UNIT_W: {
                                SetText(wxString::FromDouble(activePower * systemPowerBase, m_decimalPlaces) + " W");
                            } break;
                            case ElectricalUnit::UNIT_kW: {
                                SetText(wxString::FromDouble(activePower * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kW");
                            } break;
                            case ElectricalUnit::UNIT_MW: {
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
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(reactivePower, m_decimalPlaces) + " p.u.");
                            } break;
                            case ElectricalUnit::UNIT_var: {
                                SetText(wxString::FromDouble(reactivePower * systemPowerBase, m_decimalPlaces) +
                                        " VAr");
                            } break;
                            case ElectricalUnit::UNIT_kvar: {
                                SetText(wxString::FromDouble(reactivePower * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            } break;
                            case ElectricalUnit::UNIT_Mvar: {
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
                            case ElectricalUnit::UNIT_PU: {
                                wxString str =
                                    "Ia = " + wxString::FromDouble(faultCurrent[0], m_decimalPlaces) + " p.u.";
                                str += "\nIb = " + wxString::FromDouble(faultCurrent[1], m_decimalPlaces) + " p.u.";
                                str += "\nIc = " + wxString::FromDouble(faultCurrent[2], m_decimalPlaces) + " p.u.";
                                SetText(str);
                            } break;
                            case ElectricalUnit::UNIT_A: {
                                wxString str =
                                    "Ia = " + wxString::FromDouble(faultCurrent[0] * baseCurrent, m_decimalPlaces) +
                                    " A";
                                str += "\nIb = " +
                                       wxString::FromDouble(faultCurrent[1] * baseCurrent, m_decimalPlaces) + " A";
                                str += "\nIc = " +
                                       wxString::FromDouble(faultCurrent[2] * baseCurrent, m_decimalPlaces) + " A";
                                SetText(str);
                            } break;
                            case ElectricalUnit::UNIT_kA: {
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
                if(data.nominalVoltageUnit == ElectricalUnit::UNIT_kV) baseVoltage *= 1e3;
                double baseCurrent = systemPowerBase / (std::sqrt(3.0) * baseVoltage);
                switch(m_dataType) {
                    case DATA_NAME: {
                        SetText(data.name);
                    } break;
                    case DATA_PF_ACTIVE: {
                        double activePF = std::real(data.powerFlow[m_direction]);
                        if(!line->IsOnline()) activePF = 0.0;
                        switch(m_unit) {
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(activePF, m_decimalPlaces) + " p.u.");
                            } break;
                            case ElectricalUnit::UNIT_W: {
                                SetText(wxString::FromDouble(activePF * systemPowerBase, m_decimalPlaces) + " W");
                            } break;
                            case ElectricalUnit::UNIT_kW: {
                                SetText(wxString::FromDouble(activePF * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kW");
                            } break;
                            case ElectricalUnit::UNIT_MW: {
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
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(reactivePF, m_decimalPlaces) + " p.u.");
                            } break;
                            case ElectricalUnit::UNIT_var: {
                                SetText(wxString::FromDouble(reactivePF * systemPowerBase, m_decimalPlaces) + " VAr");
                            } break;
                            case ElectricalUnit::UNIT_kvar: {
                                SetText(wxString::FromDouble(reactivePF * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            } break;
                            case ElectricalUnit::UNIT_Mvar: {
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
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(losses, m_decimalPlaces) + " p.u.");
                            } break;
                            case ElectricalUnit::UNIT_W: {
                                SetText(wxString::FromDouble(losses * systemPowerBase, m_decimalPlaces) + " W");
                            } break;
                            case ElectricalUnit::UNIT_kW: {
                                SetText(wxString::FromDouble(losses * systemPowerBase / 1e3, m_decimalPlaces) + " kW");
                            } break;
                            case ElectricalUnit::UNIT_MW: {
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
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(current, m_decimalPlaces) + " p.u.");
                            } break;
                            case ElectricalUnit::UNIT_A: {
                                SetText(wxString::FromDouble(current * baseCurrent, m_decimalPlaces) + " A");
                            } break;
                            case ElectricalUnit::UNIT_kA: {
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
                            case ElectricalUnit::UNIT_PU: {
                                wxString str =
                                    "Ia = " + wxString::FromDouble(faultCurrent[0], m_decimalPlaces) + " p.u.";
                                str += "\nIb = " + wxString::FromDouble(faultCurrent[1], m_decimalPlaces) + " p.u.";
                                str += "\nIc = " + wxString::FromDouble(faultCurrent[2], m_decimalPlaces) + " p.u.";
                                SetText(str);
                            } break;
                            case ElectricalUnit::UNIT_A: {
                                wxString str =
                                    "Ia = " + wxString::FromDouble(faultCurrent[0] * baseCurrent, m_decimalPlaces) +
                                    " A";
                                str += "\nIb = " +
                                       wxString::FromDouble(faultCurrent[1] * baseCurrent, m_decimalPlaces) + " A";
                                str += "\nIc = " +
                                       wxString::FromDouble(faultCurrent[2] * baseCurrent, m_decimalPlaces) + " A";
                                SetText(str);
                            } break;
                            case ElectricalUnit::UNIT_kA: {
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

                if(data.primaryNominalVoltageUnit == ElectricalUnit::UNIT_kV) baseVoltage[0] *= 1e3;
                if(data.secondaryNominalVoltageUnit == ElectricalUnit::UNIT_kV) baseVoltage[1] *= 1e3;

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
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(activePF, m_decimalPlaces) + " p.u.");
                            } break;
                            case ElectricalUnit::UNIT_W: {
                                SetText(wxString::FromDouble(activePF * systemPowerBase, m_decimalPlaces) + " W");
                            } break;
                            case ElectricalUnit::UNIT_kW: {
                                SetText(wxString::FromDouble(activePF * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kW");
                            } break;
                            case ElectricalUnit::UNIT_MW: {
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
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(reactivePF, m_decimalPlaces) + " p.u.");
                            } break;
                            case ElectricalUnit::UNIT_var: {
                                SetText(wxString::FromDouble(reactivePF * systemPowerBase, m_decimalPlaces) + " VAr");
                            } break;
                            case ElectricalUnit::UNIT_kvar: {
                                SetText(wxString::FromDouble(reactivePF * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            } break;
                            case ElectricalUnit::UNIT_Mvar: {
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
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(losses, m_decimalPlaces) + " p.u.");
                            } break;
                            case ElectricalUnit::UNIT_W: {
                                SetText(wxString::FromDouble(losses * systemPowerBase, m_decimalPlaces) + " W");
                            } break;
                            case ElectricalUnit::UNIT_kW: {
                                SetText(wxString::FromDouble(losses * systemPowerBase / 1e3, m_decimalPlaces) + " kW");
                            } break;
                            case ElectricalUnit::UNIT_MW: {
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
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(current, m_decimalPlaces) + " p.u.");
                            } break;
                            case ElectricalUnit::UNIT_A: {
                                SetText(wxString::FromDouble(current * baseCurrent[m_direction], m_decimalPlaces) +
                                        " A");
                            } break;
                            case ElectricalUnit::UNIT_kA: {
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
                            case ElectricalUnit::UNIT_PU: {
                                wxString str =
                                    "Ia = " + wxString::FromDouble(faultCurrent[0], m_decimalPlaces) + " p.u.";
                                str += "\nIb = " + wxString::FromDouble(faultCurrent[1], m_decimalPlaces) + " p.u.";
                                str += "\nIc = " + wxString::FromDouble(faultCurrent[2], m_decimalPlaces) + " p.u.";
                                SetText(str);
                            } break;
                            case ElectricalUnit::UNIT_A: {
                                wxString str =
                                    "Ia = " +
                                    wxString::FromDouble(faultCurrent[0] * baseCurrent[m_direction], m_decimalPlaces) +
                                    " A";
                                str +=
                                    "\nIb = " +
                                    wxString::FromDouble(faultCurrent[1] * baseCurrent[m_direction], m_decimalPlaces) +
                                    " A";
                                str +=
                                    "\nIc = " +
                                    wxString::FromDouble(faultCurrent[2] * baseCurrent[m_direction], m_decimalPlaces) +
                                    " A";
                                SetText(str);
                            } break;
                            case ElectricalUnit::UNIT_kA: {
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
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(sPower.real(), m_decimalPlaces) + " p.u.");
                            }
                            case ElectricalUnit::UNIT_W: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase, m_decimalPlaces) + " W");
                            }
                            case ElectricalUnit::UNIT_kW: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kW");
                            }
                            case ElectricalUnit::UNIT_MW: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MW");
                            }
                            default:
                                break;
                        }
                    } break;
                    case DATA_REACTIVE_POWER: {
                        switch(m_unit) {
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(sPower.imag(), m_decimalPlaces) + " p.u.");
                            }
                            case ElectricalUnit::UNIT_var: {
                                SetText(wxString::FromDouble(sPower.imag() * systemPowerBase, m_decimalPlaces) +
                                        " VAr");
                            }
                            case ElectricalUnit::UNIT_kvar: {
                                SetText(wxString::FromDouble(sPower.imag() * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            }
                            case ElectricalUnit::UNIT_Mvar: {
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
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(sPower.real(), m_decimalPlaces) + " p.u.");
                            }
                            case ElectricalUnit::UNIT_W: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase, m_decimalPlaces) + " W");
                            }
                            case ElectricalUnit::UNIT_kW: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kW");
                            }
                            case ElectricalUnit::UNIT_MW: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MW");
                            }
                            default:
                                break;
                        }
                    } break;
                    case DATA_REACTIVE_POWER: {
                        switch(m_unit) {
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(sPower.imag(), m_decimalPlaces) + " p.u.");
                            }
                            case ElectricalUnit::UNIT_var: {
                                SetText(wxString::FromDouble(sPower.imag() * systemPowerBase, m_decimalPlaces) +
                                        " VAr");
                            }
                            case ElectricalUnit::UNIT_kvar: {
                                SetText(wxString::FromDouble(sPower.imag() * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            }
                            case ElectricalUnit::UNIT_Mvar: {
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
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(sPower.real(), m_decimalPlaces) + " p.u.");
                            }
                            case ElectricalUnit::UNIT_W: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase, m_decimalPlaces) + " W");
                            }
                            case ElectricalUnit::UNIT_kW: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kW");
                            }
                            case ElectricalUnit::UNIT_MW: {
                                SetText(wxString::FromDouble(sPower.real() * systemPowerBase / 1e6, m_decimalPlaces) +
                                        " MW");
                            }
                            default:
                                break;
                        }
                    } break;
                    case DATA_REACTIVE_POWER: {
                        switch(m_unit) {
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(sPower.imag(), m_decimalPlaces) + " p.u.");
                            }
                            case ElectricalUnit::UNIT_var: {
                                SetText(wxString::FromDouble(sPower.imag() * systemPowerBase, m_decimalPlaces) +
                                        " VAr");
                            }
                            case ElectricalUnit::UNIT_kvar: {
                                SetText(wxString::FromDouble(sPower.imag() * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            }
                            case ElectricalUnit::UNIT_Mvar: {
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
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(reativePower, m_decimalPlaces) + " p.u.");
                            }
                            case ElectricalUnit::UNIT_var: {
                                SetText(wxString::FromDouble(reativePower * systemPowerBase, m_decimalPlaces) + " VAr");
                            }
                            case ElectricalUnit::UNIT_kvar: {
                                SetText(wxString::FromDouble(reativePower * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            }
                            case ElectricalUnit::UNIT_Mvar: {
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
                            case ElectricalUnit::UNIT_PU: {
                                SetText(wxString::FromDouble(reativePower, m_decimalPlaces) + " p.u.");
                            }
                            case ElectricalUnit::UNIT_var: {
                                SetText(wxString::FromDouble(reativePower * systemPowerBase, m_decimalPlaces) + " VAr");
                            }
                            case ElectricalUnit::UNIT_kvar: {
                                SetText(wxString::FromDouble(reativePower * systemPowerBase / 1e3, m_decimalPlaces) +
                                        " kVAr");
                            }
                            case ElectricalUnit::UNIT_Mvar: {
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
        case TYPE_HARMCURRENT: {
            HarmCurrent* harmCurrent = static_cast<HarmCurrent*>(m_element);
            if(harmCurrent) {
                auto data = harmCurrent->GetElectricalData();
                switch(m_dataType) {
                    case DATA_NAME: {
                        SetText(data.name);
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
    std::vector<OpenGLText*> copyList;
    for(auto it = m_openGLTextList.begin(), itEnd = m_openGLTextList.end(); it != itEnd; ++it) {
        copyList.push_back((*it)->GetCopy());
    }
    copy->m_openGLTextList = copyList;
    return copy;
}

bool Text::IsGLTextOK()
{
    bool isOk = true;
    for(auto it = m_openGLTextList.begin(), itEnd = m_openGLTextList.end(); it != itEnd; ++it) {
        if(!(*it)->IsTextureOK()) isOk = false;
    }
    return isOk;
}

rapidxml::xml_node<>* Text::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
    auto elementNode = XMLParser::AppendNode(doc, elementListNode, "Text");
    XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

    SaveCADProperties(doc, elementNode);

    auto textProperties = XMLParser::AppendNode(doc, elementNode, "TextProperties");
    auto elementType = XMLParser::AppendNode(doc, textProperties, "ElementType");
    XMLParser::SetNodeValue(doc, elementType, m_elementType);
    auto elementNumber = XMLParser::AppendNode(doc, textProperties, "ElementNumber");
    XMLParser::SetNodeValue(doc, elementNumber, m_elementNumber);
    auto dataType = XMLParser::AppendNode(doc, textProperties, "DataType");
    XMLParser::SetNodeValue(doc, dataType, m_dataType);
    auto dataUnit = XMLParser::AppendNode(doc, textProperties, "DataUnit");
    XMLParser::SetNodeValue(doc, dataUnit, static_cast<int>(m_unit));
    auto direction = XMLParser::AppendNode(doc, textProperties, "Direction");
    XMLParser::SetNodeValue(doc, direction, m_direction);
    auto decimalPlaces = XMLParser::AppendNode(doc, textProperties, "DecimalPlaces");
    XMLParser::SetNodeValue(doc, decimalPlaces, m_decimalPlaces);

    return elementNode;
}

bool Text::OpenElement(rapidxml::xml_node<>* elementNode)
{
    if(!OpenCADProperties(elementNode)) return false;

    auto textProperties = elementNode->first_node("TextProperties");
    if(!textProperties) return false;

    SetElementType(static_cast<ElementType>(XMLParser::GetNodeValueDouble(textProperties, "ElementType")));
    SetDataType(static_cast<DataType>(XMLParser::GetNodeValueDouble(textProperties, "DataType")));
    SetUnit(static_cast<ElectricalUnit>(XMLParser::GetNodeValueDouble(textProperties, "DataUnit")));
    SetDirection(XMLParser::GetNodeValueDouble(textProperties, "Direction"));
    SetDecimalPlaces(XMLParser::GetNodeValueDouble(textProperties, "DecimalPlaces"));
    SetElementNumber(XMLParser::GetNodeValueInt(textProperties, "ElementNumber"));
    return true;
}
