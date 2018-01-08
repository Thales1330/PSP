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

#ifndef BUS_H
#define BUS_H

#include "BusForm.h"
#include "PowerElement.h"

struct BusElectricalData {
    int number = 0;
    wxString name = "";
    double nominalVoltage = 138.0;
    ElectricalUnit nominalVoltageUnit = UNIT_kV;
    bool isVoltageControlled = false;
    double controlledVoltage = 1.0;
    int controlledVoltageUnitChoice = 0;  // 0 = p.u., 1 = same as nominalVoltageUnit (UNIT_V or UNIT_kV).
    bool slackBus = false;

    // Power flow (p.u.)
    std::complex<double> voltage = std::complex<double>(1.0, 0.0);
    std::complex<double> power = std::complex<double>(0.0, 0.0);
    int busType = 2;  // PQ

    // Fault
    bool hasFault = false;
    FaultData faultType = FAULT_THREEPHASE;
    FaultData faultLocation = FAULT_LINE_A;
    // p.u. fault data
    double faultResistance = 0.0;
    double faultReactance = 0.0;
    std::complex<double> faultCurrent[3] = {std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 0.0),
                                            std::complex<double>(0.0, 0.0)};
    std::complex<double> faultVoltage[3] = {std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 0.0),
                                            std::complex<double>(0.0, 0.0)};
    double scPower = 0.0;

    // Stability
    bool plotBus = false;
    bool stabHasFault = false;
    double stabFaultTime = 0.0;
    double stabFaultLength = 0.0;
    double stabFaultResistance = 0.0;
    double stabFaultReactance = 0.0;
    std::vector<std::complex<double> > stabVoltageVector;
};

/**
 * @class Bus
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Node for power elements. All others power elements are connected through this.
 * @file Bus.h
 */
class Bus : public PowerElement
{
   public:
    Bus();
    Bus(wxPoint2DDouble position);
    Bus(wxPoint2DDouble position, wxString name);
    ~Bus();
    virtual Element* GetCopy();
    virtual bool AddParent(Element* parent, wxPoint2DDouble position) { return true; }
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void Rotate(bool clockwise = true);
    virtual wxCursor GetBestPickboxCursor() const;
    virtual void MovePickbox(wxPoint2DDouble position);
    virtual bool PickboxContains(wxPoint2DDouble position);
    virtual bool GetContextMenu(wxMenu& menu);
    virtual wxString GetTipText() const;
    virtual BusElectricalData GetElectricalData() const { return m_electricalData; }
    virtual void SetElectricalData(BusElectricalData electricalData) { m_electricalData = electricalData; }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual bool GetPlotData(ElementPlotData& plotData);
    
    virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
    virtual bool OpenElement(rapidxml::xml_node<>* elementNode);

   protected:
    BusElectricalData m_electricalData;
};

#endif  // BUS_H
