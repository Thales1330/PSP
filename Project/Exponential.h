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

#ifndef EXPONENTIAL_H
#define EXPONENTIAL_H

#include "ControlElement.h"

class ExponentialForm;

/**
 * @class Exponential
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Generates an output following an exponential function.
 * @file Exponential.h
 */
class Exponential : public ControlElement
{
   public:
    Exponential(int id);
    ~Exponential();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);

    virtual void UpdatePoints();

    virtual void GetValues(double& aValue, double& bValue);
    virtual void SetValues(double aValue, double bValue);

    /**
     * @brief Calculates the exponential.
     *
     * <center>\f$ output = A\cdot e^{B\cdot input} \f$</center>
     * @param input Input value.
     * @param timeStep Time step.
     * @return Aways true.
     */
    virtual bool Solve(double* input, double timeStep);

    virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
    virtual bool OpenElement(rapidxml::xml_node<>* elementNode);

    virtual Element* GetCopy();

   protected:
    double m_aValue = 0.001;
    double m_bValue = 5.0;
};

#endif  // EXPONENTIAL_H
