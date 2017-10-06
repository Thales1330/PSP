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

#ifndef LINE_H
#define LINE_H

#include "LineForm.h"
#include "Branch.h"

struct LineElectricalData {
    // General
    wxString name = "";
    double nominalVoltage = 138.0;
    ElectricalUnit nominalVoltageUnit = UNIT_kV;
    double nominalPower = 100.0;
    ElectricalUnit nominalPowerUnit = UNIT_MVA;
    double resistance = 0.0;
    ElectricalUnit resistanceUnit = UNIT_PU;
    double indReactance = 1.0;
    ElectricalUnit indReactanceUnit = UNIT_PU;
    double capSusceptance = 0.0;
    ElectricalUnit capSusceptanceUnit = UNIT_PU;
    double lineSize = 100.0;
    bool useLinePower = false;

    // Power flow (p.u.)
    std::complex<double> current[2] = {std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 0.0)};
    std::complex<double> powerFlow[2] = {std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 0.0)};

    // Fault
    double zeroResistance = 0.0;
    double zeroIndReactance = 1.0;
    double zeroCapSusceptance = 0.0;
    // p.u. fault data
    std::complex<double> faultCurrent[2][3];
};

/**
 * @class Line
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Power line element.
 * @file Line.h
 */
class Line : public Branch
{
   public:
    Line();
    Line(wxString name);
    ~Line();
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void Move(wxPoint2DDouble position);
    virtual void StartMove(wxPoint2DDouble position);
    virtual void MoveNode(Element* parent, wxPoint2DDouble position);
    virtual bool SetNodeParent(Element* parent);
    virtual wxCursor GetBestPickboxCursor() const { return wxCURSOR_SIZING; }
    virtual Element* GetCopy();
    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void MovePickbox(wxPoint2DDouble position);
    virtual bool PickboxContains(wxPoint2DDouble position);
    virtual void AddPoint(wxPoint2DDouble point);
    virtual bool GetContextMenu(wxMenu& menu);
    virtual wxString GetTipText() const;
    virtual void RemoveNode(wxPoint2DDouble point);
    virtual void AddNode(wxPoint2DDouble point);
    virtual void RotateNode(Element* parent, bool clockwise = true);
    virtual void CalculateBoundaries(wxPoint2DDouble& leftUp, wxPoint2DDouble& rightBottom) const;
    virtual void SetPowerFlowDirection(PowerFlowDirection pfDirection);
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual LineElectricalData GetElectricalData() const { return m_electricalData; }
    virtual LineElectricalData GetPUElectricalData(double systemBasePower);
    virtual void SetElectricalData(LineElectricalData electricalData) { m_electricalData = electricalData; }
    virtual void SetNominalVoltage(std::vector<double> nominalVoltage, std::vector<ElectricalUnit> nominalVoltageUnit);
    virtual void SetPointList(std::vector<wxPoint2DDouble> pointList);

   protected:
    void UpdatePowerFlowArrowsPosition();
    LineElectricalData m_electricalData;
};

#endif  // LINE_H
