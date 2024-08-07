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

#ifndef SUM_H
#define SUM_H

#include "ControlElement.h"

class SumForm;
class ConnectionLine;

/**
 * @class Sum
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Sum the all inputs (can choose the input signal).
 * @file Sum.h
 */
class Sum : public ControlElement
{
   public:
    enum Signal { SIGNAL_POSITIVE = 0, SIGNAL_NEGATIVE };
    Sum(int id);
    ~Sum();

    //virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);

    virtual std::vector<Signal> GetSignalList() const { return m_signalList; }
    virtual void SetSignalList(std::vector<Signal> signalList) { m_signalList = signalList; }
    virtual bool Solve(double* input, double timeStep);

    virtual void UpdatePoints();
    void AddInNode();
    void RemoveInNode();

    virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
    virtual bool OpenElement(rapidxml::xml_node<>* elementNode);

    virtual Element* GetCopy();

   protected:
    std::vector<Signal> m_signalList;
};

#endif  // SUM_H
