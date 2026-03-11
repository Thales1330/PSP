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

#include "../elements/Element.h"
#include "../elements/powerElement/Bus.h"
#include "../elements/powerElement/Capacitor.h"
#include "../elements/powerElement/HarmCurrent.h"
#include "../elements/powerElement/IndMotor.h"
#include "../elements/powerElement/Inductor.h"
#include "../elements/powerElement/Line.h"
#include "../elements/powerElement/Load.h"
#include "../elements/powerElement/PowerElement.h"
#include "../elements/powerElement/SyncGenerator.h"
#include "../elements/powerElement/SyncMotor.h"
#include "../elements/powerElement/Transformer.h"
#include "../elements/powerElement/EMTElement.h"

#include "../utils/PropertiesData.h"

 /**
  * @enum BusType
  * @brief Types of buses used in power flow analysis.
  */
enum BusType {
	BUS_SLACK = 0, /**< Slack (reference) bus. Voltage magnitude and angle are specified. */
	BUS_PV,        /**< PV bus (generator bus). Active power and voltage magnitude are specified. */
	BUS_PQ         /**< PQ bus (load bus). Active and reactive powers are specified. */
};

/**
 * @enum ReactiveLimitsType
 * @brief Types of reactive power limit conditions for buses.
 */
enum ReactiveLimitsType {
	RL_UNLIMITED = 0,     /**< The bus can generate any amount of reactive power. */
	RL_LIMITED,           /**< The bus reactive power generation is limited. */
	RL_UNLIMITED_SOURCE,  /**< The bus has at least one source capable of unlimited reactive power. */
	RL_MAX_REACHED,       /**< Maximum reactive power limit reached. */
	RL_MIN_REACHED,       /**< Minimum reactive power limit reached. */
	RL_NONE_REACHED       /**< No reactive power limits reached. */
};

/**
 * @enum YBusSequence
 * @brief Sequence type used when building the system admittance matrix.
 */
enum YBusSequence {
	POSITIVE_SEQ = 0, /**< Positive sequence admittance matrix. */
	NEGATIVE_SEQ,     /**< Negative sequence admittance matrix. */
	ZERO_SEQ          /**< Zero sequence admittance matrix. */
};

/**
 * @struct ReactiveLimits
 * @brief Stores reactive power limits and their status for a bus.
 */
struct ReactiveLimits {
	double maxLimit = 0.0; /**< Maximum reactive power limit. */
	double minLimit = 0.0; /**< Minimum reactive power limit. */

	ReactiveLimitsType maxLimitType = RL_UNLIMITED; /**< Type of maximum reactive power limit. */
	ReactiveLimitsType minLimitType = RL_UNLIMITED; /**< Type of minimum reactive power limit. */

	ReactiveLimitsType limitReached = RL_NONE_REACHED; /**< Indicates whether a reactive limit has been reached. */
};

/**
 * @struct ReactiveMachine
 * @brief Auxiliary structure used to distribute reactive power among machines connected to a bus.
 */
struct ReactiveMachine {
	double qMax; /**< Maximum reactive power capability. */
	double qMin; /**< Minimum reactive power capability. */

	bool hasMax; /**< Indicates if the machine has a maximum reactive power limit. */
	bool hasMin; /**< Indicates if the machine has a minimum reactive power limit. */

	bool fixed = false; /**< Indicates whether the machine reactive power has been fixed during distribution. */

	double q = 0.0; /**< Reactive power assigned to the machine. */

	Machines* machine; /**< Pointer to the machine element (generator or motor). */
	bool isGenerator;  /**< True if the machine is a generator, false if it is a motor. */
};

/**
 * @class ElectricCalculation
 * @author Thales Lima Oliveira
 * @date 09/01/2017
 * @brief Base class for electrical calculations providing general utility methods.
 * @details
 * This class provides common utilities used by different electrical calculations such as power flow, short-circuit analysis and EMT simulations.
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

	/**
	 * @brief Convert a complex phasor in ABC representation to DQ components.
	 * @param complexValue Complex phasor value in ABC reference frame.
	 * @param angle Electrical angle used for the transformation.
	 * @param dValue Resulting direct-axis component.
	 * @param qValue Resulting quadrature-axis component.
	 */
	void ABCtoDQ0(std::complex<double> complexValue, double angle, double& dValue, double& qValue);

	/**
	 * @brief Convert DQ components to a complex phasor in ABC representation.
	 * @param dValue Direct-axis component.
	 * @param qValue Quadrature-axis component.
	 * @param angle Electrical angle used for the transformation.
	 * @param complexValue Resulting complex phasor in ABC reference frame.
	 */
	void DQ0toABC(double dValue, double qValue, double angle, std::complex<double>& complexValue);

	/**
	 * @brief Solve a linear system using Gaussian elimination (complex version).
	 * @param matrix Coefficient matrix of the system.
	 * @param array Right-hand side vector.
	 * @return Solution vector of the linear system.
	 */
	std::vector<std::complex<double> > GaussianElimination(std::vector<std::vector<std::complex<double> > > matrix,
		std::vector<std::complex<double> > array);

	/**
	 * @brief Solve a linear system using Gaussian elimination (real version).
	 * @param matrix Coefficient matrix of the system.
	 * @param array Right-hand side vector.
	 * @return Solution vector of the linear system.
	 */
	std::vector<double> GaussianElimination(std::vector<std::vector<double> > matrix, std::vector<double> array);

	/**
	 * @brief Get the synchronous machine model used by the generator based on user-defined parameters.
	 * @param generator Pointer to the synchronous generator.
	 * @return Enumeration indicating the synchronous machine model.
	 */
	Machines::SyncMachineModel GetMachineModel(SyncGenerator* generator);

	/**
	 * @brief Multiply a complex matrix by a complex vector.
	 * @param matrix Complex matrix.
	 * @param vector Complex vector.
	 * @return Resulting complex vector.
	 */
	std::vector<std::complex<double> > ComplexMatrixTimesVector(std::vector<std::vector<std::complex<double> > > matrix,
		std::vector<std::complex<double> > vector);

	/**
	 * @brief Compute the LU decomposition of a matrix.
	 * @param matrix Matrix to be decomposed.
	 * @param matrixL Lower triangular matrix.
	 * @param matrixU Upper triangular matrix.
	 */
	void GetLUDecomposition(std::vector<std::vector<std::complex<double> > > matrix,
		std::vector<std::vector<std::complex<double> > >& matrixL,
		std::vector<std::vector<std::complex<double> > >& matrixU);

	/**
	 * @brief Solve a linear system using LU decomposition.
	 * @param u Upper triangular matrix.
	 * @param l Lower triangular matrix.
	 * @param b Right-hand side vector.
	 * @return Solution vector.
	 */
	std::vector<std::complex<double> > LUEvaluate(std::vector<std::vector<std::complex<double> > > u,
		std::vector<std::vector<std::complex<double> > > l,
		std::vector<std::complex<double> > b);

	/**
	 * @brief Get the parent bus of a given shunt element.
	 * @param childElement Pointer to the element.
	 * @param parentBus Pointer to the parent bus.
	 * @return True if the parent bus was successfully found.
	 */
	bool GetParentBus(Element* childElement, Bus*& parentBus);

	/**
	 * @brief Get the parent buses of a two-terminal element (branch).
	 * @param childElement Pointer to the element.
	 * @param parentBus1 Pointer to the first parent bus.
	 * @param parentBus2 Pointer to the second parent bus.
	 * @return True if both parent buses were successfully found.
	 */
	bool GetParentBus(Element* childElement, Bus*& parentBus1, Bus*& parentBus2);

	/**
	 * @brief Calculate the admittance of EMT elements.
	 * @param basePower System base power.
	 * @param errorMsg Error message in case of failure.
	 * @return True if the calculation was successful.
	 */
	bool CalculateEMTElementsAdmittance(const double& basePower, wxString& errorMsg);

	/**
	 * @brief Calculate the power of EMT elements.
	 * @param basePower System base power.
	 * @param errorMsg Error message in case of failure.
	 * @param updateCurrent If true, update the element currents during the calculation unsing
	 *		  \ref EMTElement::CalculateCurrent(wxString&, const bool&).
	 * @return True if the calculation was successful.
	 */
	bool CalculateEMTElementsPower(const double& basePower, wxString& errorMsg, bool updateCurrent = true);

	/**
	 * @brief Calculate the power mismatch error for EMT simulation.
	 * @param voltage Vector with bus voltages.
	 * @param power Vector with injected powers.
	 * @param basePower System base power.
	 * @param errorMsg Error message in case of failure.
	 * @return The calculated power error.
	 */
	double CalculateEMTPowerError(const std::vector< std::complex<double> >& voltage, std::vector< std::complex<double> >& power, const double& basePower, wxString& errorMsg);

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
	/**
	 * @brief Get the electromagnetic element list of the system (use GetElementsFromList first).
	 * @return A list of electromagnetic elements.
	 */
	const std::vector<EMTElement*> GetEMTElementList() const { return m_emtElementList; }

protected:
	/**
	 * @brief Recursively check if a bus is electrically connected to the slack bus.
	 * @param checkBusNumber Index of the bus being checked.
	 * @param yBus Admittance matrix of the system.
	 * @param connToSlack Boolean vector indicating if each bus is connected to the slack bus.
	 */
	void GetNextConnection(const unsigned int& checkBusNumber, const std::vector< std::vector< std::complex<double> > >& yBus, std::vector<bool>& connToSlack);

	/**
	 * @brief Distribute reactive power among synchronous machines connected to the same bus.
	 * @details The method ensures that reactive power limits of individual machines are respected during the distribution process.
	 * @param machines List of machines participating in the reactive power sharing.
	 * @param qTotal Total reactive power to be distributed.
	 */
	void DistributeReactivePower(std::vector<ReactiveMachine>& machines, double qTotal);

	/** @brief List of power elements in the system. */
	std::vector<PowerElement*> m_powerElementList;
	/** @brief List of buses in the system. */
	std::vector<Bus*> m_busList;
	/** @brief List of capacitor elements in the system. */
	std::vector<Capacitor*> m_capacitorList;
	/** @brief List of induction motors in the system. */
	std::vector<IndMotor*> m_indMotorList;
	/** @brief List of inductors in the system. */
	std::vector<Inductor*> m_inductorList;
	/** @brief List of transmission lines in the system. */
	std::vector<Line*> m_lineList;
	/** @brief List of load elements in the system. */
	std::vector<Load*> m_loadList;
	/** @brief List of synchronous generators in the system. */
	std::vector<SyncGenerator*> m_syncGeneratorList;
	/** @brief List of synchronous motors in the system. */
	std::vector<SyncMotor*> m_syncMotorList;
	/** @brief List of transformers in the system. */
	std::vector<Transformer*> m_transformerList;
	/** @brief List of harmonic current sources in the system. */
	std::vector<HarmCurrent*> m_harmCurrentList;
	/** @brief List of electromagnetic transient (EMT) elements in the system. */
	std::vector<EMTElement*> m_emtElementList;
};

#endif  // ELECTRICCALCULATION_H
