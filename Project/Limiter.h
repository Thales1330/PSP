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

#ifndef LIMITER_H
#define LIMITER_H

#include "ControlElement.h"

class LimiterForm;

/**
 * @class Limiter
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Limits the input value by superior and inferior values.
 * @file Limiter.h
 */
class Limiter : public ControlElement
{
   public:
    Limiter(int id);
    ~Limiter();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);
    virtual bool Solve(double* input, double timeStep);

    virtual void UpdatePoints();

    double GetUpLimit() const { return m_upLimit; }
    double GetLowLimit() const { return m_lowLimit; }
    void SetUpLimit(double upLimit) { m_upLimit = upLimit; }
    void SetLowLimit(double lowLimit) { m_lowLimit = lowLimit; }

    virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
    virtual bool OpenElement(rapidxml::xml_node<>* elementNode);

    virtual Element* GetCopy();

   protected:
    double m_upLimit = 5.0;
    double m_lowLimit = -5.0;
};

#endif  // LIMITER_H
