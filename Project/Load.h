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
};

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
   protected:
    std::vector<wxPoint2DDouble> m_triangPts;
    LoadElectricalData m_electricalData;
};

#endif  // LOAD_H
