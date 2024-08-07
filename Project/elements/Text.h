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

#ifndef TEXT_H
#define TEXT_H

//#include <GL/glew.h>

//#ifdef _MSC_VER
 //#include <windows.h>
//#endif 
//#include <GL/gl.h>
#include <wx/dcmemory.h>

#include "GraphicalElement.h"
#include "powerElement/PowerElement.h"
#include "GCText.h"

class TextForm;

class Bus;
class Line;
class Transformer;
class SyncGenerator;
class IndMotor;
class SyncMotor;
class Load;
class Inductor;
class Capacitor;
class HarmCurrent;

enum DataType {
	DATA_NAME,
	DATA_VOLTAGE,
	DATA_ANGLE,
	DATA_SC_CURRENT,
	DATA_SC_VOLTAGE,
	DATA_SC_POWER,
	DATA_ACTIVE_POWER,
	DATA_REACTIVE_POWER,
	DATA_PF_ACTIVE,
	DATA_PF_REACTIVE,
	DATA_PF_LOSSES,
	DATA_PF_CURRENT,
	DATA_PQ_THD
};

/**
 * @class Text
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Element that shows power element informations in workspace.
 * @file Text.h
 */
class Text : public GraphicalElement
{
public:
	Text();
	Text(wxPoint2DDouble position);
	~Text();

	virtual Element* GetCopy();
	virtual bool AddParent(Element* parent, wxPoint2DDouble position) { return true; };
	virtual bool Contains(wxPoint2DDouble position) const;
	//virtual void Draw(wxPoint2DDouble translation, double scale);
	virtual void DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc);
	virtual bool Intersects(wxRect2DDouble rect) const;
	virtual void Rotate(bool clockwise = true);
	virtual bool ShowForm(wxWindow* parent, std::vector<Element*> elementList);
	virtual void UpdateText(double systemPowerBase);
	virtual wxString GetText() const { return m_text; }
	virtual void SetText(wxString text);
	virtual void SetAltSelectionColour(bool useAltSelectionColour = true) { m_useAltSelectionColour = useAltSelectionColour; }
	virtual void SetAllowRotation(bool allowRotation = true) { m_allowRotation = allowRotation; }
	//virtual bool IsGLTextOK();

	void SetDataType(const DataType& dataType) { m_dataType = dataType; }
	void SetDirection(int direction) { m_direction = direction; }
	void SetElement(Element* element) { m_element = element; }
	void SetElementNumber(int elementNumber) { m_elementNumber = elementNumber; }
	void SetElementTypeText(const ElementType elementType) { m_elementTypeText = elementType; }
	void SetUnit(const ElectricalUnit unit) { m_unit = unit; }
	void SetDecimalPlaces(int decimalPlaces) { m_decimalPlaces = decimalPlaces; }
	const DataType GetDataType() const { return m_dataType; }
	int GetDirection() const { return m_direction; }
	Element* GetElement() { return m_element; }
	int GetElementNumber() const { return m_elementNumber; }
	const ElementType GetElementTypeText() const { return m_elementTypeText; }
	const ElectricalUnit GetUnit() const { return m_unit; }
	int GetDecimalPlaces() const { return m_decimalPlaces; }

	virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
	virtual bool OpenElement(rapidxml::xml_node<>* elementNode);

protected:
	wxString m_text = _("Text");
	int m_numberOfLines = 0;
	bool m_isMultlineText = false;
	bool m_useAltSelectionColour = false;
	bool m_allowRotation = true;

	std::vector<GCText*> m_gcTextList;

	Element* m_element = nullptr;
	ElementType m_elementTypeText = TYPE_NONE;
	int m_elementNumber;
	DataType m_dataType;
	ElectricalUnit m_unit;
	int m_direction = 0;
	int m_decimalPlaces = 2;
};

#endif  // TEXT_H
