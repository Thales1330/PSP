#ifndef POWERFLOW_H
#define POWERFLOW_H

#include "ElectricCalculation.h"

#include <wx/string.h>
#include <wx/intl.h>  //_()
#include <wx/log.h>   //temp

enum ReactiveLimitsType {
    RL_UNLIMITED = 0,     // The bus can generate any ammount of reactive power.
    RL_LIMITED,           // The bus reactive power generation is limited.
    RL_UNLIMITED_SOURCE,  // The bus have at least one source of infinite reative power.
};

struct ReactiveLimits {
    double maxLimit = 0.0;
    double minLimit = 0.0;
    ReactiveLimitsType maxLimitType = RL_UNLIMITED;
    ReactiveLimitsType minLimitType = RL_UNLIMITED;
};

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
