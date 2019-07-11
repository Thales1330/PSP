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

#ifndef INDMOTOR_H
#define INDMOTOR_H

#include "Machines.h"

class IndMotorForm;

struct IndMotorElectricalData {
    wxString name;
    double ratedPower = 100.0;
    ElectricalUnit ratedPowerUnit = UNIT_MVA;
    double activePower = 100.0;
    ElectricalUnit activePowerUnit = UNIT_MW;
    double reactivePower = 0.0;
    ElectricalUnit reactivePowerUnit = UNIT_MVAr;

    bool useMachinePowerAsBase = true;

    // Stability
    bool plotIndMachine = true;
    double inertia = 1.0;  // Motor and load inertia
    double s0;             // Initial slip
    double q0;             // Initial reactive power
    double r1 = 0.0;       // Stator resistence data
    double x1 = 1.0;       // Stator reactance data
    double r2 = 1.0;       // Rotor resistence data
    double x2 = 0.0;       // Rotor reactance data
    double xm = 100.0;     // Magnetizing reactance data

    // Transient values
    double xt = 1.0;     // Transient reactance
    double x0 = 1.0;     // Open-circuit reactance
    double r1t = 0.0;    // Stator resistence in system power base
    double x1t = 1.0;    // Stator reactance  in system power base
    double r2t = 1.0;    // Rotor resistence  in system power base
    double x2t = 0.0;    // Rotor reactance  in system power base
    double xmt = 100.0;  // Magnetizing reactance  in system power base

    double t0 = 1.0;  // Open-circuit time constant

    double aw = 1.0;  // Velocity constant torque
    double bw = 0.0;  // Velocity linear dependent torque
    double cw = 0.0;  // Velocity quadratic dependent torque
    double as = 0.0;  // Slip constant torque
    double bs = 0.0;  // Slip linear dependent torque
    double cs = 0.0;  // Slip quadratic dependent torque

    // Internal machine variables
    double tranEr;
    double tranEm;
    double ir;
    double im;

    // Variables to extrapolate
    double oldIr;
    double oldIm;
    double oldTe;

    // Integration constants
    IntegrationConstant icSlip;
    IntegrationConstant icTranEr;
    IntegrationConstant icTranEm;

    // Machine state variables
    std::complex<double> terminalVoltage;
    std::vector<double> terminalVoltageVector;
    double slip = 1.0;  // Machine slip
    std::vector<double> slipVector;
    double te;
    std::vector<double> electricalTorqueVector;
    std::vector<double> mechanicalTorqueVector;
    std::complex<double> electricalPower;
    std::vector<std::complex<double> > electricalPowerVector;
};

/**
 * @class IndMotor
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Induction motor power element.
 * @file IndMotor.h
 */
class IndMotor : public Machines
{
   public:
    IndMotor();
    IndMotor(wxString name);
    ~IndMotor();

    virtual Element* GetCopy();
    virtual void DrawSymbol() const;
    virtual bool GetContextMenu(wxMenu& menu);
    virtual wxString GetTipText() const;
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual IndMotorElectricalData GetElectricalData() { return m_electricalData; }
    virtual IndMotorElectricalData GetPUElectricalData(double systemPowerBase);
    virtual void SetElectricalData(IndMotorElectricalData electricalData) { m_electricalData = electricalData; }
    
    virtual bool GetPlotData(ElementPlotData& plotData, PlotStudy study = STABILITY);

    virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
    virtual bool OpenElement(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList);

   protected:
    IndMotorElectricalData m_electricalData;
};

#endif  // INDMOTOR_H
