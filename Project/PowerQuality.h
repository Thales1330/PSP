#ifndef POWERQUALITY_H
#define POWERQUALITY_H

#include "ElectricCalculation.h"

#include <wx/intl.h>  //_()
#include <wx/string.h>

/**
 * @class PowerQuality
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 24/04/2019
 * @brief Responsible for the power quality calculations.
 * @file PowerQuality.h
 */
class PowerQuality : public ElectricCalculation
{
   public:
    struct HarmonicYbus {
        double order;
        std::vector<std::vector<std::complex<double> > > yBus;
    };

    PowerQuality();
    PowerQuality(std::vector<Element*> elementList);
    ~PowerQuality();
    
    virtual void CalculateHarmonicYbusList(double systemPowerBase = 100e6);

    virtual bool CalculateDistortions(double systemPowerBase = 100e6);
    virtual std::vector<double> GetHarmonicOrdersList();

   protected:
    std::vector<HarmonicYbus> m_harmYbusList;
};

#endif  // POWERQUALITY_H
