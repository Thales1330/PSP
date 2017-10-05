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

#ifndef CONSTANT_H
#define CONSTANT_H

#include "ControlElement.h"

#include <wx/dcscreen.h>
#include "wxGLString.h"

class ConstantForm;

/**
 * @class Constant
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief A control element that provides a constant value.
 * @file Constant.h
 */
class Constant : public ControlElement
{
   public:
    Constant(int id);
    ~Constant();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);
    virtual void UpdateText() { SetValue(m_value); }
    virtual void SetValue(double value);
    virtual double GetValue() const { return m_value; }
    virtual void UpdatePoints();

    virtual Element* GetCopy();

   protected:
    double m_value = 1.0;

    wxGLString* m_glStringValue = NULL;
    int m_fontSize = 10;
};

#endif  // CONSTANT_H
