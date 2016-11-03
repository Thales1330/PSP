#ifndef POWERFLOW_H
#define POWERFLOW_H

#include "ElectricCalculation.h"

#include <wx/string.h>
#include <wx/intl.h>  //_()
#include <wx/log.h>   //temp

enum BusType { BUS_SLACK = 0, BUS_PV, BUS_PQ };

class PowerFlow : public ElectricCalculation
{
   public:
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
