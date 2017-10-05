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

#ifndef POWERFLOW_H
#define POWERFLOW_H

#include "ElectricCalculation.h"

#include <wx/string.h>
#include <wx/intl.h>  //_()

class PowerFlow : public ElectricCalculation
{
   public:
    PowerFlow();
    PowerFlow(std::vector<Element*> elementList);
    ~PowerFlow();
    virtual bool RunGaussSeidel(double systemPowerBase = 100e6,
                                int maxIteration = 5000,
                                double error = 1e-6,
                                double initAngle = 0.0,
                                double accFactor = 1.0);

    virtual wxString GetErrorMessage() { return m_errorMsg; }
   protected:
    std::vector<std::vector<std::complex<double> > > m_yBus;
    wxString m_errorMsg = "";
};

#endif  // POWERFLOW_H
