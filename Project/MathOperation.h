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

#ifndef MATHOPERATION_H
#define MATHOPERATION_H

#include "ControlElement.h"

class ConnectionLine;

/**
 * @class MathOperation
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 09/10/2017
 * @brief Abstract class that define the general behavior of math operation control block.
 * @file MathOperation.h
 */
class MathOperation : public ControlElement
{
   public:
    MathOperation(int id);
    ~MathOperation();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const;
    virtual void DrawSymbol() const {}
    virtual void DrawDCSymbol(wxGraphicsContext* gc) const {}
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element) { return false; }
    virtual void Rotate(bool clockwise = true);

    virtual void UpdatePoints();
};

#endif  // MATHOPERATION_H
