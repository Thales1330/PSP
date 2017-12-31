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

#ifndef DIVIDER_H
#define DIVIDER_H

#include "MathOperation.h"

class ConnectionLine;

/**
 * @class Divider
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 09/10/2017
 * @brief Control element that divides two inputs.
 * @file Divider.h
 */
class Divider : public MathOperation
{
   public:
    Divider(int id);
    ~Divider();

    virtual void DrawSymbol() const;
    virtual bool Solve(double* input, double timeStep);
    virtual Element* GetCopy();
};

#endif  // DIVIDER_H
