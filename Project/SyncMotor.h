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

#ifndef SYNCMOTOR_H
#define SYNCMOTOR_H

#include "Machines.h"

class SyncMachineForm;

struct SyncMotorElectricalData {
    // General
    wxString name = "";
    double nominalPower = 100.0;
    ElectricalUnit nominalPowerUnit = ElectricalUnit::UNIT_MVA;
    double nominalVoltage = 13.8;
    ElectricalUnit nominalVoltageUnit = ElectricalUnit::UNIT_kV;
    double activePower = 100.0;
    ElectricalUnit activePowerUnit = ElectricalUnit::UNIT_MW;
    double reactivePower = 0.0;
    ElectricalUnit reactivePowerUnit = ElectricalUnit::UNIT_Mvar;
    bool haveMaxReactive = false;
    double maxReactive = 9999.0;
    ElectricalUnit maxReactiveUnit = ElectricalUnit::UNIT_Mvar;
    bool haveMinReactive = false;
    double minReactive = -9999.0;
    ElectricalUnit minReactiveUnit = ElectricalUnit::UNIT_Mvar;
    bool useMachineBase = true;

    // Fault
    double positiveResistance = 0.0;
    double positiveReactance = 1.0;
    double negativeResistance = 0.0;
    double negativeReactance = 1.0;
    double zeroResistance = 0.0;
    double zeroReactance = 1.0;
    double groundResistance = 0.0;
    double groundReactance = 0.0;
    bool groundNeutral = true;
    // p.u. fault data
    std::complex<double> faultCurrent[3] = {std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 0.0),
                                            std::complex<double>(0.0, 0.0)};

    // Stability
    bool plotSyncMachine = false;
    double inertia = 1.0;
    double damping = 0.0;
    bool useAVR = false;
    bool useSpeedGovernor = false;

    double armResistance = 0.0;
    double potierReactance = 0.0;
    double satFactor = 0.0;

    double syncXd = 0.0;
    double syncXq = 0.0;
    double transXd = 1.0;
    double transXq = 0.0;
    double transTd0 = 0.0;
    double transTq0 = 0.0;
    double subXd = 0.0;
    double subXq = 0.0;
    double subTd0 = 0.0;
    double subTq0 = 0.0;

    // Machine state variables
    std::complex<double> terminalVoltage;
    std::vector<std::complex<double> > terminalVoltageVector;
    std::complex<double> electricalPower;
    std::vector<std::complex<double> > electricalPowerVector;
    double pm;
    std::vector<double> mechanicalPowerVector;
    double speed;
    std::vector<double> freqVector;
    double fieldVoltage;
    std::vector<double> fieldVoltageVector;
    double delta;
    std::vector<double> deltaVector;

    double initialFieldVoltage;

    // Internal machine variables
    double tranEq;
    double tranEd;
    double subEq;
    double subEd;
    double pe;

    // Variables to extrapolate
    double oldId;
    double oldIq;
    double oldPe;

    // Integration constants
    IntegrationConstant icSpeed;
    IntegrationConstant icDelta;
    IntegrationConstant icTranEq;
    IntegrationConstant icTranEd;
    IntegrationConstant icSubEq;
    IntegrationConstant icSubEd;

    // Control
    ControlElementContainer* avr = nullptr;
    ControlElementContainer* speedGov = nullptr;

    // Control solvers
    ControlElementSolver* avrSolver = nullptr;
    ControlElementSolver* speedGovSolver = nullptr;

    Machines::SyncMachineModel model = Machines::SM_MODEL_1;
};

/**
 * @class SyncMotor
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Synchronous motor (synchronous compensator) power element.
 * @file SyncMotor.h
 */
class SyncMotor : public Machines
{
   public:
    SyncMotor();
    SyncMotor(wxString name);
    ~SyncMotor();

    virtual Element* GetCopy();
    virtual void DrawSymbol() const;
    virtual void DrawDCSymbol(wxGraphicsContext* gc) const;
    virtual bool GetContextMenu(wxMenu& menu);
    virtual wxString GetTipText() const;
    virtual SyncMotorElectricalData GetElectricalData() { return m_electricalData; }
    virtual SyncMotorElectricalData GetPUElectricalData(double systemPowerBase);
    virtual void SetNominalVoltage(std::vector<double> nominalVoltage, std::vector<ElectricalUnit> nominalVoltageUnit);
    virtual void SetElectricalData(SyncMotorElectricalData electricalData) { m_electricalData = electricalData; }
    virtual bool ShowForm(wxWindow* parent, Element* element);

    virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
    virtual bool OpenElement(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList);

   protected:
    SyncMotorElectricalData m_electricalData;
};

#endif  // SYNCMOTOR_H
