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

#ifndef GAIN_H
#define GAIN_H

#include "ControlElement.h"

#include <wx/dcscreen.h>
#include "OpenGLText.h"

class GainForm;

/**
 * @class Gain
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Provide an output multiplying the input by a constant.
 * @file Gain.h
 */
class Gain : public ControlElement
{
   public:
    Gain(int id);
    ~Gain();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);
    virtual void Move(wxPoint2DDouble position);
    virtual bool UpdateText();
    virtual void SetValue(double value);
    virtual double GetValue() const { return m_value; }
    virtual void UpdatePoints();
    /**
     * @brief Multiply the input by a constant
     *
     * <center>\f$ output = K \cdot input \f$</center>
     * @param input Input value.
     * @param timeStep Time step.
     * @return Always true.
     */
    virtual bool Solve(double* input, double timeStep);

    virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
    virtual bool OpenElement(rapidxml::xml_node<>* elementNode);

    virtual Element* GetCopy();

   protected:
    double m_value = 1.0;

    OpenGLText* m_glText = NULL;

    std::vector<wxPoint2DDouble> m_triPts;
};

#endif  // GAIN_H
