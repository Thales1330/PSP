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

#ifndef LOAD_H
#define LOAD_H

#include "LoadForm.h"
#include "Shunt.h"

enum LoadType { CONST_POWER = 0, CONST_IMPEDANCE };

struct LoadElectricalData {
    wxString name;
    double activePower = 100.0;
    ElectricalUnit activePowerUnit = UNIT_MW;
    double reactivePower = 0.0;
    ElectricalUnit reactivePowerUnit = UNIT_MVAr;
    LoadType loadType = CONST_POWER;

    // Stability
    bool plotLoad = false;
    // ZIP load
    bool useCompLoad = false;
    // The power injected on the "i" bus flollow the quadratic equation:
    // -p(i) = pz0 * (v(i) / v0) ^ 2 + pi0 * (v(i) / v0) + pp0
    double v0 = 1.0;   // Initial load voltage from load flow in p.u.
    double pz0 = 1.0;  // Initial active power modelled as constant impedance from load flow in p.u.
    double pi0 = 0.0;  // Initial active power modelled as constant current from load flow in p.u.
    double pp0 = 0.0;  // Initial active power modelled as constant power from load flow in p.u.
    double qz0 = 1.0;  // Initial reactive power modelled as constant impedance from load flow in p.u.
    double qi0 = 0.0;  // Initial reactive power modelled as constant current from load flow in p.u.
    double qp0 = 0.0;  // Initial reactive power modelled as constant power from load flow in p.u.
    double constImpedanceActive = 100.0;    // Constant impedance portion of active power (%).
    double constCurrentActive = 0.0;        // Constant current portion of active power (%).
    double constPowerActive = 0.0;          // Constant power portion of active power (%).
    double constImpedanceReactive = 100.0;  // Constant impedance portion of reactive power (%).
    double constCurrentReactive = 0.0;      // Constant current portion of reactive power (%).
    double constPowerReactive = 0.0;        // Constant power portion of reactive power (%).
    std::complex<double> y0;                // Steady-state equivalent admittance calculated from power flow.
    // Undervoltage (in p.u.) which the constant current portion will be modelled as constant impedance.
    double constCurrentUV = 0.7;
    // Undervoltage (in p.u.) which the constant power portion will be modelled as constant impedance.
    double constPowerUV = 0.7;

    // Load state variables
    std::complex<double> voltage;
    std::vector<std::complex<double> > voltageVector;
    std::complex<double> electricalPower;
    std::vector<std::complex<double> > electricalPowerVector;
};

/**
 * @class Load
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Loas shunt power element.
 * @file Load.h
 */
class Load : public Shunt
{
   public:
    Load();
    Load(wxString name);
    ~Load();

    virtual Element* GetCopy();
    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void Rotate(bool clockwise = true);
    virtual bool GetContextMenu(wxMenu& menu);
    virtual wxString GetTipText() const;
    virtual bool ShowForm(wxWindow* parent, Element* element);
    LoadElectricalData GetElectricalData() { return m_electricalData; }
    LoadElectricalData GetPUElectricalData(double systemPowerBase);
    void SetElectricalData(LoadElectricalData electricalData) { m_electricalData = electricalData; }
    virtual bool GetPlotData(ElementPlotData& plotData);

    virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
    virtual bool OpenElement(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList);

   protected:
    std::vector<wxPoint2DDouble> m_triangPts;
    LoadElectricalData m_electricalData;
};

#endif  // LOAD_H
