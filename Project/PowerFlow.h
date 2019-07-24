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

#include <wx/intl.h>  //_()
#include <wx/string.h>

/**
 * @class PowerFlow
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Calculate the power flow.
 * @file PowerFlow.h
 */
class PowerFlow : public ElectricCalculation
{
   public:
    PowerFlow();
    PowerFlow(std::vector<Element *> elementList);
    ~PowerFlow();
    virtual bool InitPowerFlow(std::vector<BusType> &busType,
                               std::vector<std::complex<double> > &voltage,
                               std::vector<std::complex<double> > &power,
                               std::vector<std::complex<double> > &loadPower,
                               std::vector<ReactiveLimits> &reactiveLimit,
                               double systemPowerBase = 100e6,
                               double initAngle = 0.0);
    virtual bool RunGaussSeidel(double systemPowerBase = 100e6,
                                int maxIteration = 5000,
                                double error = 1e-6,
                                double initAngle = 0.0,
                                double accFactor = 1.0);
    virtual bool RunNewtonRaphson(double systemPowerBase = 100e6,
                                  int maxIteration = 5000,
                                  double error = 1e-6,
                                  double initAngle = 0.0);
    virtual bool RunGaussNewton(double systemPowerBase = 100e6,
                                int maxIteration = 5000,
                                double error = 1e-6,
                                double initAngle = 0.0,
                                double accFactor = 1.0,
                                double gaussTol = 10);

    virtual wxString GetErrorMessage() { return m_errorMsg; }

   protected:
    void GetNumPVPQ(std::vector<BusType> busType, int &numPQ, int &numPV);
    std::vector<std::vector<double> > CalculateJacobianMatrix(std::vector<std::complex<double> > voltage,
                                                              std::vector<BusType> busType,
                                                              int numPV,
                                                              int numPQ);
    bool CheckReactiveLimits(std::vector<BusType> &busType,
                             std::vector<ReactiveLimits> &reactiveLimit,
                             std::vector<std::complex<double> > power,
                             std::vector<std::complex<double> > loadPower);

    double GaussSeidel(std::vector<BusType> busType,
                       std::vector<std::complex<double> > &voltage,
                       std::vector<std::complex<double> > oldVoltage,
                       std::vector<std::complex<double> > &power,
                       double accFactor);
    void NewtonRaphson(std::vector<BusType> busType,
                       std::vector<std::complex<double> > &voltage,
                       std::vector<std::complex<double> > power,
                       int numPV,
                       int numPQ,
                       std::vector<double> dPdQ);

    std::vector<std::vector<std::complex<double> > > m_yBus;
    wxString m_errorMsg = "";
    int m_numberOfBuses = 0;
};

#endif  // POWERFLOW_H
