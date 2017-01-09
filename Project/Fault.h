#ifndef FAULT_H
#define FAULT_H

#include "ElectricCalculation.h"

class Fault : public ElectricCalculation
{
public:
    Fault(std::vector<Element*> elementList);
    Fault();
    ~Fault();

    virtual bool RunFaultCalculation(double systemPowerBase);
    virtual void UpdateElementsFault(double systemPowerBase);
    virtual wxString GetErrorMessage() { return m_errorMsg; }

protected:
    wxString m_errorMsg = "";

    std::vector<std::vector<std::complex<double> > > m_zBusPos;
    std::vector<std::vector<std::complex<double> > > m_zBusNeg;
    std::vector<std::vector<std::complex<double> > > m_zBusZero;

    std::vector<std::complex<double> > m_posFaultVoltagePos;
    std::vector<std::complex<double> > m_posFaultVoltageNeg;
    std::vector<std::complex<double> > m_posFaultVoltageZero;

    std::complex<double> m_fCurrentA;
    std::complex<double> m_fCurrentB;
    std::complex<double> m_fCurrentC;

    std::vector<std::complex<double> > m_posFaultVoltageA;
    std::vector<std::complex<double> > m_posFaultVoltageB;
    std::vector<std::complex<double> > m_posFaultVoltageC;
};

#endif // FAULT_H
