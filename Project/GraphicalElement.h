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

#ifndef GRAPHICALELEMENT_H
#define GRAPHICALELEMENT_H

#include "Element.h"

/**
 * @class GraphicalElement
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Abstract class for graphical elements shown with power elements in workspace.
 * @file GraphicalElement.h
 */
class GraphicalElement : public Element
{
   public:
    GraphicalElement();
    ~GraphicalElement();
};

#endif  // GRAPHICALELEMENT_H
