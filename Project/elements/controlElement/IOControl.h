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

#ifndef IOCONTROL_H
#define IOCONTROL_H

#include "ControlElement.h"

 //#include "OpenGLText.h"
#include "../GCText.h"

class IOControlForm;

struct SimTestData {
	double startTime = 1.0;
	double initialValue = 0.0;
	double slope = 1.0;
	int type = 0;
};

/**
 * @class IOControl
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Provides the communication with the power element.
 * @file IOControl.h
 */
class IOControl : public ControlElement
{
public:
	enum IOFlags {
		IN_TERMINAL_VOLTAGE = 1 << 0,
		IN_VELOCITY = 1 << 1,
		IN_ACTIVE_POWER = 1 << 2,
		IN_REACTIVE_POWER = 1 << 3,
		OUT_FIELD_VOLTAGE = 1 << 4,
		OUT_MEC_POWER = 1 << 5,
		IN_INITIAL_TERMINAL_VOLTAGE = 1 << 6,
		IN_INITIAL_MEC_POWER = 1 << 7,
		IN_INITIAL_VELOCITY = 1 << 8,
		IN_DELTA_VELOCITY = 1 << 9,
		IN_DELTA_ACTIVE_POWER = 1 << 10,
		IN_TEST = 1 << 11,
	};

	IOControl(int ioFlags, int id);
	~IOControl();

	//virtual void Draw(wxPoint2DDouble translation, double scale) const;
	virtual void DrawDC(GUIColour* guiColour, wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const;
	virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
	virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
	virtual bool ShowForm(wxWindow* parent, Element* element, wxWindow* workspace = nullptr);
	virtual void Rotate(bool clockwise = true);
	virtual bool UpdateText();
	virtual wxString GenerateText();
	virtual void UpdatePoints();

	virtual IOFlags GetValue() const { return m_value; }
	virtual void SetValue(IOFlags value);
	virtual int GetIOFlags() const { return m_ioFlags; }
	virtual Node::NodeType GetType() { return m_ioNodeType; }
	virtual wxString GetName() const { return m_name; }
	virtual void SetName(const wxString& name) { m_name = name; }
	virtual SimTestData GetSimTestData() const { return m_simTestData; }
	virtual void SetSimTestData(const SimTestData& simTestData) { m_simTestData = simTestData; }
	virtual double GetTestValue() const { return m_testValue; }
	virtual void SetTestValue(const double& testValue) { m_testValue = testValue; }
	virtual bool Initialize();

	virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
	virtual bool OpenElement(rapidxml::xml_node<>* elementNode);

	virtual Element* GetCopy();

protected:
	IOFlags m_value;
	int m_ioFlags;
	wxString m_name = _("Input / Output");
	SimTestData m_simTestData;
	double m_testValue = 0.0;

	Node::NodeType m_ioNodeType = Node::NodeType::NODE_IN;

	//OpenGLText* m_glText = nullptr;
	GCText* m_gcText = nullptr;
};

#endif  // IOCONTROL_H
