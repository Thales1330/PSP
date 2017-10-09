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

#include <wx/dcscreen.h>
#include "wxGLString.h"

class IOControlForm;

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
    };

    IOControl(int ioFlags, int id);
    ~IOControl();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);
    virtual void UpdateText() { SetValue(m_value); }
    virtual wxString GenerateText();
    virtual void UpdatePoints();

    virtual IOFlags GetValue() const { return m_value; }
    virtual void SetValue(IOFlags value);
    virtual int GetIOFlags() const { return m_ioFlags; }
    virtual Node::NodeType GetType() { return m_ioNodeType; }
    virtual Element* GetCopy();

   protected:
    IOFlags m_value;
    int m_ioFlags;

    Node::NodeType m_ioNodeType = Node::NODE_IN;

    wxGLString* m_glStringValue = NULL;
    int m_fontSize = 10;
};

#endif  // IOCONTROL_H
