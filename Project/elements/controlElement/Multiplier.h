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

#ifndef MULTIPLIER_H
#define MULTIPLIER_H

#include "MathOperation.h"

class ConnectionLine;

/**
 * @class Multiplier
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Multiplies two inputs.
 * @file Multiplier.h
 */
class Multiplier : public MathOperation
{
   public:
    Multiplier(int id);
    ~Multiplier();

    //virtual void DrawSymbol() const;
    virtual void DrawDCSymbol(wxGraphicsContext* gc) const;
    virtual bool Solve(double* input, double timeStep);
    
    virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
    virtual bool OpenElement(rapidxml::xml_node<>* elementNode);
    
    virtual Element* GetCopy();
};

#endif  // MULTIPLIER_H
