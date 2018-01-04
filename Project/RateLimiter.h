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

#ifndef RATELIMITER_H
#define RATELIMITER_H

#include "ControlElement.h"

class RateLimiterForm;

/**
 * @class RateLimiter
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Limits the rising and/or falling rate.<br>
 * @file RateLimiter.h
 */
class RateLimiter : public ControlElement
{
   public:
    RateLimiter(int id);
    ~RateLimiter();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);

    virtual void UpdatePoints();

    double GetUpLimit() const { return m_upLimit; }
    double GetLowLimit() const { return m_lowLimit; }
    void SetUpLimit(double upLimit) { m_upLimit = upLimit; }
    void SetLowLimit(double lowLimit) { m_lowLimit = lowLimit; }
    /**
     * @brief Calculate the rate and limits it if exceeds.<br>
     *
     * The rate is calculated by:
     * <center>\f$ rate = \frac{x(i) - y(i-1)}{\Delta t} \f$</center>
     * Where, \f$ x(i) \f$ is the current input and \f$ y(i-1) \f$ is the previous output.<br><br>
     * If the \f$rate\f$ is greater than rising rate (\f$ R \f$), the output will be:<br>
     * <center>\f$ output = \Delta t \cdot R + y(i-1)\f$</center>
     * If the \f$rate\f$ is lower than falling rate (\f$ F \f$), the output will be:<br><br>
     * <center>\f$ output = \Delta t \cdot F + y(i-1) \f$</center>
     * Otherwise:
     * <center>\f$ output = input \f$</center>.
     * @param input Input value.
     * @param timeStep Time step.
     * @return Always true.
     */
    virtual bool Solve(double* input, double timeStep);

    virtual void SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
    virtual bool OpenElement(rapidxml::xml_node<>* elementNode);

    virtual Element* GetCopy();

   protected:
    double m_upLimit = 5.0;
    double m_lowLimit = -5.0;
};

#endif  // RATELIMITER_H
