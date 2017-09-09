#ifndef FAULT_H
#define FAULT_H

#include "ElectricCalculation.h"
/**
 * @class Fault
 * @author Thales Lima Oliveira
 * @date 10/01/2017
 * @file Fault.h
 * @brief Calculate the fault of the system and update the elements data.
 */

class Fault : public ElectricCalculation
{
public:
    /**
     * @brief Contructor.
     * @param elementList List of elements in workspace
     */
    Fault(std::vector<Element*> elementList);

    /**
     * @brief Default contructor. Use GetElementsFromList(std::vector<Element*> elementList).
     */
    Fault();

    /**
     * @brief Destructor.
     */
    ~Fault();

    /**
     * @brief Calculate the fault of the system. Return true if was possible the calculation.
     * @param systemPowerBase System base of power.
     */
    virtual bool RunFaultCalculation(double systemPowerBase);

    /**
     * @brief Calculate the short-circuit power of the system. Return true if was possible the calculation.
     * @param systemPowerBase System base of power.
     */
    virtual bool RunSCPowerCalcutation(double systemPowerBase);

    /**
     * @brief Update the data of the elements.
     * @param systemPowerBase System base of power.
     */
    virtual void UpdateElementsFault(double systemPowerBase);

    /**
     * @brief Get the error message generated in RunFaultCalculation(double systemPowerBase).
     * @return Error message.
     */
    virtual wxString GetErrorMessage() { return m_errorMsg; }

protected:
    wxString m_errorMsg = "";
    
    double m_systemPowerBase;

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
