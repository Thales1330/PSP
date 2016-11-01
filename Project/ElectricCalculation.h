#ifndef ELECTRICCALCULATION_H
#define ELECTRICCALCULATION_H

#include <vector>
#include <complex>

class Element;
class Bus;
class Capacitor;
class IndMotor;
class Inductor;
class Line;
class Load;
class SyncGenerator;
class SyncMotor;
class Transformer;

class ElectricCalculation
{
   public:
    ElectricCalculation();
    ~ElectricCalculation();
    virtual void GetElementsFromList(std::vector<Element*> elementList);
    virtual bool GetYBus(std::vector<std::vector<std::complex<double> > >& yBus, double systemPowerBase);

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
