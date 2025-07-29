/*
 *  Copyright (C) 2024  Thales Lima Oliveira <thales@ufu.br>
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

#ifndef EMTELEMENT_H
#define EMTELEMENT_H

#include "Shunt.h"
#include <map>

class wxTextFile;
class PropertiesData;

struct EMTElementData {
	wxString name = "";
	wxFileName atpFile;
	wxString atpNodeName = "";
	double stepSize = 1e-6;
	int cyclesToSS = 1;
	int recordFrequency = 1;
	bool useMedianFilter = true;
	std::complex<double> puVoltage = std::complex<double>(1.0, 0.0);
	double baseVoltage = 138.0e3;
	double frequency = 60.0;
	wxString atpWorkFolder = "";
	wxFileName atpPath;

	// Power Flow
	std::complex<double> y0 = std::complex<double>(0.0, 0.0); // Base admittance
	std::complex<double> power = std::complex<double>(0.0, 0.0);
	std::complex<double> powerDiff = std::complex<double>(0.0, 0.0);

	// Harmonics
	int numMaxHarmonics = 15;
	double harmonicsThreshold = 0.3;
	//std::vector< std::complex<double> > currHarmonics;
	//std::vector<int> currHarmonicsOrder;
	std::map<int, std::complex<double> > currHarmonics;
	std::vector< std::pair<double, double> > atpData;
	std::vector< std::pair<double, double> > inFFTData;
	std::vector< std::pair<double ,std::complex<double> > > outFFTData;
};

/**
 * @class EMTElement
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 01/07/2024
 * @brief Element to connect ATP-EMTP.
 * @file EMTElement.h
 */
class EMTElement : public Shunt
{
public:
	EMTElement();
	EMTElement(wxString name);
	~EMTElement();

	virtual Element* GetCopy();
	virtual bool AddParent(Element* parent, wxPoint2DDouble position);
	virtual void DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const;
	virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
	virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
	virtual void Rotate(bool clockwise = true);
	virtual bool GetContextMenu(wxMenu& menu);
	virtual wxString GetTipText() const;
	virtual bool ShowForm(wxWindow* parent, Element* element);

	virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
	virtual bool OpenElement(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList);

	virtual EMTElementData GetEMTElementData() { return m_data; }
	virtual void SetEMTElementData(EMTElementData data) { m_data = data; }

	wxArrayString GetATPNodes(wxArrayString atpFile);
	bool SetATPParameter(wxTextFile& atpFile, const wxString& card, const int& line, const int& initPos, const int& size, const wxString& value);
	bool AddConnectionToNode(wxTextFile& atpFile, const wxString& node);
	std::vector<double> MedianFilter(const std::vector<double>& data);
	bool CalculateCurrent( wxString& errorMsg, const bool& saveFFTData = false);
	void UpdateData(const PropertiesData* properties = nullptr, bool updateVoltageBase = false);

protected:
	std::vector<double> DoMedianFilter(double* extension, std::vector<double>& result, const int& n);
	EMTElementData m_data;
};

#endif // EMTELEMENT_H

