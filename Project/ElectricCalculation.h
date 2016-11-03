#ifndef ELECTRICCALCULATION_H
#define ELECTRICCALCULATION_H

#include <vector>
#include <complex>

#include "Element.h"
#include "Bus.h"
#include "Capacitor.h"
#include "IndMotor.h"
#include "Inductor.h"
#include "Line.h"
#include "Load.h"
#include "SyncGenerator.h"
#include "SyncMotor.h"
#include "Transformer.h"

class ElectricCalculation
{
   public:
    ElectricCalculation();
    ~ElectricCalculation();
    virtual void GetElementsFromList(std::vector<Element*> elementList);
    virtual bool GetYBus(std::vector<std::vector<std::complex<double> > >& yBus, double systemPowerBase);
    virtual void ValidateElementsPowerFlow(std::vector<std::complex<double> > voltage, std::vector<std::complex<double> > power, double systemPowerBase);

   protected:
    std::vector<Bus*> m_busList;
    std::vector<Capacitor*> m_capacitorList;
    std::vector<IndMotor*> m_indMotorList;
    std::vector<Inductor*> m_inductorList;
    std::vector<Line*> m_lineList;
    std::vector<Load*> m_loadList;
    std::vector<SyncGenerator*> m_syncGeneratorList;
    std::vector<SyncMotor*> m_syncMotorList;
    std::vector<Transformer*> m_transformerList;
};

#endif  // ELECTRICCALCULATION_H
