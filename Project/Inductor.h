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

#ifndef INDUCTOR_H
#define INDUCTOR_H

#include "Shunt.h"

class ReactiveShuntElementForm;

struct InductorElectricalData {
    wxString name;
    double reactivePower = 100.0;
    ElectricalUnit reactivePowerUnit = UNIT_MVAr;
};

/**
 * @class Inductor
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Inductor shunt power element.
 * @file Inductor.h
 */
class Inductor : public Shunt
{
   public:
    Inductor();
    Inductor(wxString name);
    ~Inductor();

    virtual Element* GetCopy();
    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void Rotate(bool clockwise = true);
    virtual bool GetContextMenu(wxMenu& menu);
    virtual wxString GetTipText() const;
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual InductorElectricalData GetElectricalData() { return m_electricalData; }
    virtual InductorElectricalData GetPUElectricalData(double systemPowerBase);
    virtual void SetElectricalData(InductorElectricalData electricalData) { m_electricalData = electricalData; }
   protected:
    InductorElectricalData m_electricalData;
};

#endif  // INDUCTOR_H
