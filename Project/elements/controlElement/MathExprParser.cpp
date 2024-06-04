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

#include "MathExprParser.h"

MathExprParser::MathExprParser()
{
    AddConstant("pi", 3.14159265358979323846);
    AddConstant("e", 2.71828182845904523536);
    AddConstant("closed", 1);
    AddConstant("open", 0);
    AddConstant("true", 1);
    AddConstant("false", 0);
}

MathExprParser::~MathExprParser() {}
