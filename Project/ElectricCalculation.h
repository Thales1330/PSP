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

#ifndef ELECTRICCALCULATION_H
#define ELECTRICCALCULATION_H

#include <complex>
#include <vector>

#include "Bus.h"
#include "Capacitor.h"
#include "Element.h"
#include "HarmCurrent.h"
#include "IndMotor.h"
#include "Inductor.h"
#include "Line.h"
#include "Load.h"
#include "PowerElement.h"
#include "SyncGenerator.h"
#include "SyncMotor.h"
#include "Transformer.h"

#include "PropertiesData.h"

enum BusType { BUS_SLACK = 0, BUS_PV, BUS_PQ };

enum ReactiveLimitsType {
    RL_UNLIMITED = 0,     // The bus can generate any ammount of reactive power.
    RL_LIMITED,           // The bus reactive power generation is limited.
    RL_UNLIMITED_SOURCE,  // The bus have at least one source of infinite reative power.
    RL_MAX_REACHED,       // Max limit reached
    RL_MIN_REACHED,       // Min limit reached
    RL_NONE_REACHED       // No limits reached
};

enum YBusSequence { POSITIVE_SEQ = 0, NEGATIVE_SEQ, ZERO_SEQ };

struct ReactiveLimits {
    double maxLimit = 0.0;
    double minLimit = 0.0;
    ReactiveLimitsType maxLimitType = RL_UNLIMITED;
    ReactiveLimitsType minLimitType = RL_UNLIMITED;
    ReactiveLimitsType limitReached = RL_NONE_REACHED;
};

/**
 * @class ElectricCalculation
 * @author Thales Lima Oliveira
 * @date 09/01/2017
 * @brief Base class of electric calculations, with general methods.
 * @file ElectricCalculation.h
 */
class ElectricCalculation
{
   public:
    /**
     * @brief Constructor.
     */
    ElectricCalculation();

    /**
     * @brief Destructor.
     */
    ~ElectricCalculation();

    /**
     * @brief Separate the power elements from a generic list.
     * @param elementList List of generic elements.
     */
    virtual void GetElementsFromList(std::vector<Element*> elementList);

    /**
     * @brief Get the admittance matrix from the list of elements (use GetElementsFromList first).
     * @param yBus Admittance matrix. The previous content will be erased.
     * @param systemPowerBase Base power of the system.
     * @param sequence Sequence of admittance matrix (positive, negative and zero).
     * @param includeSyncMachines Include the synchronous machines on calculation.
     * @return Return true if was possible to build the admittance matrix.
     */
    virtual bool GetYBus(std::vector<std::vector<std::complex<double> > >& yBus,
                         double systemPowerBase,
                         YBusSequence sequence = POSITIVE_SEQ,
                         bool includeSyncMachines = false,
                         bool allLoadsAsImpedances = false,
                         bool usePowerFlowVoltagesOnImpedances = false);

    /**
     * @brief Invert a matrix.
     * @param matrix Matrix to invert.
     * @param inverse Inverted matrix. The previous content will be erased.
     * @return Return true if was possible to invert the matrix.
     */
    virtual bool InvertMatrix(std::vector<std::vector<std::complex<double> > > matrix,
                              std::vector<std::vector<std::complex<double> > >& inverse);

    /**
     * @brief Update the elements after the power flow calculation.
     * @param voltage Array with the buses voltages.
     * @param power Array with the buses injected power.
     * @param busType Array with the buses type.
     * @param reactiveLimit Array with the reactive limit data.
     * @param systemPowerBase Base power of the system.
     */
    virtual void UpdateElementsPowerFlow(std::vector<std::complex<double> > voltage,
                                         std::vector<std::complex<double> > power,
                                         std::vector<BusType> busType,
                                         std::vector<ReactiveLimits> reactiveLimit,
                                         double systemPowerBase);

    void ABCtoDQ0(std::complex<double> complexValue, double angle, double& dValue, double& qValue);
    void DQ0toABC(double dValue, double qValue, double angle, std::complex<double>& complexValue);

    std::vector<std::complex<double> > GaussianElimination(std::vector<std::vector<std::complex<double> > > matrix,
                                                           std::vector<std::complex<double> > array);
    std::vector<double> GaussianElimination(std::vector<std::vector<double> > matrix, std::vector<double> array);

    Machines::SyncMachineModel GetMachineModel(SyncGenerator* generator);

    std::vector<std::complex<double> > ComplexMatrixTimesVector(std::vector<std::vector<std::complex<double> > > matrix,
                                                                std::vector<std::complex<double> > vector);

    void GetLUDecomposition(std::vector<std::vector<std::complex<double> > > matrix,
                            std::vector<std::vector<std::complex<double> > >& matrixL,
                            std::vector<std::vector<std::complex<double> > >& matrixU);

    std::vector<std::complex<double> > LUEvaluate(std::vector<std::vector<std::complex<double> > > u,
                                                  std::vector<std::vector<std::complex<double> > > l,
                                                  std::vector<std::complex<double> > b);

    /**
     * @brief Get the power elements of the system (use GetElementsFromList first).
     * @return A list of power elements.
     */
    const std::vector<PowerElement*> GetPowerElementList() const { return m_powerElementList; }
    /**
     * @brief Get the buses of the system (use GetElementsFromList first).
     * @return A list of bus elements.
     */
    const std::vector<Bus*> GetBusList() const { return m_busList; }
    /**
     * @brief Get the capacitors of the system (use GetElementsFromList first).
     * @return A list of capacitor elements.
     */
    const std::vector<Capacitor*> GetCapacitorList() const { return m_capacitorList; }
    /**
     * @brief Get the induction motors of the system (use GetElementsFromList first).
     * @return A list of induction motor elements.
     */
    const std::vector<IndMotor*> GetIndMotorList() const { return m_indMotorList; }
    /**
     * @brief Get the inductors of the system (use GetElementsFromList first).
     * @return A list of inductor elements.
     */
    const std::vector<Inductor*> GetInductorList() const { return m_inductorList; }
    /**
     * @brief Get the lines of the system (use GetElementsFromList first).
     * @return A list of line elements.
     */
    const std::vector<Line*> GetLineList() const { return m_lineList; }
    /**
     * @brief Get the loads of the system (use GetElementsFromList first).
     * @return A list of load elements.
     */
    const std::vector<Load*> GetLoadList() const { return m_loadList; }
    /**
     * @brief Get the synchronous generators of the system (use GetElementsFromList first).
     * @return A list of synchronous generator elements.
     */
    const std::vector<SyncGenerator*> GetSyncGeneratorList() const { return m_syncGeneratorList; }
    /**
     * @brief Get the synchronous motors of the system (use GetElementsFromList first).
     * @return A list of synchronous motor elements.
     */
    const std::vector<SyncMotor*> GetSyncMotorList() const { return m_syncMotorList; }
    /**
     * @brief Get the transformers of the system (use GetElementsFromList first).
     * @return A list of transformer elements.
     */
    const std::vector<Transformer*> GetTransformerList() const { return m_transformerList; }
    /**
     * @brief Get the harmonic current source of the system (use GetElementsFromList first).
     * @return A list of harmonic current sources elements.
     */
    const std::vector<HarmCurrent*> GetHarmCurrentList() const { return m_harmCurrentList; }

   protected:
    std::vector<PowerElement*> m_powerElementList;
    std::vector<Bus*> m_busList;
    std::vector<Capacitor*> m_capacitorList;
    std::vector<IndMotor*> m_indMotorList;
    std::vector<Inductor*> m_inductorList;
    std::vector<Line*> m_lineList;
    std::vector<Load*> m_loadList;
    std::vector<SyncGenerator*> m_syncGeneratorList;
    std::vector<SyncMotor*> m_syncMotorList;
    std::vector<Transformer*> m_transformerList;
    std::vector<HarmCurrent*> m_harmCurrentList;
};

#endif  // ELECTRICCALCULATION_H
