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

#ifndef BUS_H
#define BUS_H

#include "../../forms/BusForm.h"
#include "PowerElement.h"

struct BusElectricalData {
	int number = 0;
	wxString name = "";
	double nominalVoltage = 138.0;
	ElectricalUnit nominalVoltageUnit = ElectricalUnit::UNIT_kV;
	bool isVoltageControlled = false;
	double controlledVoltage = 1.0;
	int controlledVoltageUnitChoice = 0;  // 0 = p.u., 1 = same as nominalVoltageUnit (ElectricalUnit::UNIT_V or ElectricalUnit::UNIT_kV).
	bool slackBus = false;
	bool isConnected = true;

	// Power flow (p.u.)
	std::complex<double> voltage = std::complex<double>(1.0, 0.0);
	std::complex<double> power = std::complex<double>(0.0, 0.0);
	int busType = 2;  // PQ

	// Fault
	bool hasFault = false;
	FaultData faultType = FaultData::FAULT_THREEPHASE;
	FaultData faultLocation = FaultData::FAULT_LINE_A;
	// p.u. fault data
	double faultResistance = 0.0;
	double faultReactance = 0.0;
	std::complex<double> faultCurrent[3] = { std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 0.0),
											std::complex<double>(0.0, 0.0) };
	std::complex<double> faultVoltage[3] = { std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 0.0),
											std::complex<double>(0.0, 0.0) };
	double scPower = 0.0;

	// Stability
	bool plotBus = false;
	bool stabHasFault = false;
	double stabFaultTime = 0.0;
	double stabFaultLength = 0.0;
	double stabFaultResistance = 0.0;
	double stabFaultReactance = 0.0;
	double oldAngle = 0.0;			// theta(k-1)
	double filteredAngle = 0.0;		// xt(k-1)
	double dxt = 0.0;				// Derivative of filteredAngle
	double filteredVelocity = 0.0;	// dw(k-1)
	double ddw = 0.0;				// Derivative of filteredVelocity
	double stabFreq = 60.0;
	std::vector<std::complex<double> > stabVoltageVector;
	std::vector<double> stabFreqVector;

	// Power Quality
	bool plotPQData = false;
	std::vector<int> harmonicOrder;
	std::vector< std::complex<double> > harmonicVoltage;
	double thd = 0.0;

	std::vector<double> absImpedanceVector;
};

/**
 * @class Bus
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Node for power elements. All others power elements are connected through this.
 * @file Bus.h
 */
class Bus : public PowerElement
{
public:
	/**
	 * @brief Default constructor.
	 *
	 */
	Bus();
	/**
	 * @brief Contructor of a new Busbar.
	 *
	 * \param position Position to be placed in Workspace.
	 */
	Bus(wxPoint2DDouble position);
	/**
	 * @brief Contructor of a new Busbar.
	 *
	 * \param position Position to be placed in Workspace.
	 * \param name Bus name
	 */
	Bus(wxPoint2DDouble position, wxString name);
	~Bus();
	virtual Element* GetCopy();
	/**
	 * @brief Add a parent element to the Bus.
	 * @warning Since the Bus is always a parent element, it doesn't make sense to add a parent to the bus. Therefore, this method will return true.
	 *
	 * \param parent Parent element.
	 * \param position Position of the parent element.
	 * \return True.
	 */
	virtual bool AddParent(Element* parent, wxPoint2DDouble position) { return true; }
	virtual bool Contains(wxPoint2DDouble position) const;
	virtual bool Intersects(wxRect2DDouble rect) const;
	//virtual void Draw(wxPoint2DDouble translation, double scale) const;
	virtual void DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const;
	virtual void Rotate(bool clockwise = true);
	virtual wxCursor GetBestPickboxCursor() const;
	virtual void MovePickbox(wxPoint2DDouble position);
	virtual bool PickboxContains(wxPoint2DDouble position);
	virtual bool GetContextMenu(wxMenu& menu);
	virtual wxString GetTipText() const;
	virtual BusElectricalData GetElectricalData() const { return m_electricalData; }
	virtual void SetElectricalData(const BusElectricalData& electricalData) { m_electricalData = electricalData; }
	virtual bool ShowForm(wxWindow* parent, Element* element);
	virtual bool GetPlotData(ElementPlotData& plotData, PlotStudy study = PlotStudy::STABILITY);

	virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
	virtual bool OpenElement(rapidxml::xml_node<>* elementNode);

protected:
	BusElectricalData m_electricalData;
};

#endif  // BUS_H
