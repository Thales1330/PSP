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
    double activePower = 100.0;
    ElectricalUnit activePowerUnit = UNIT_MW;
    double reactivePower = 0.0;
    ElectricalUnit reactivePowerUnit = UNIT_MVAr;
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
   protected:
    IndMotorElectricalData m_electricalData;
};

#endif  // INDMOTOR_H
