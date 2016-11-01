#ifndef POWERFLOW_H
#define POWERFLOW_H

#include "ElectricCalculation.h"

#include <wx/string.h>//temp
#include <wx/log.h>//temp

class PowerFlow : public ElectricCalculation
{
   public:
    PowerFlow(std::vector<Element*> elementList);
    ~PowerFlow();
    virtual bool RunGaussSeidel();

   protected:
    std::vector<std::vector<std::complex<double> > > m_yBus;
};

#endif  // POWERFLOW_H
