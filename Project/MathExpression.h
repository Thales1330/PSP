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

#ifndef MATHEXPRESSION_H
#define MATHEXPRESSION_H

#include "ControlElement.h"
#include "MathExprParser.h"
#include "OpenGLText.h"

class MathExpressionForm;

/**
 * @class MathExpression
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 19/12/2017
 * @brief A generic math expression block that can perform math and conditional operations with the inputs.
 * @file MathExpression.h
 */
class MathExpression : public ControlElement
{
   public:
    MathExpression(int id);
    ~MathExpression();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);

    virtual bool Solve(double input, double timeStep);

    virtual void UpdatePoints();
    void AddInNode();
    void RemoveInNode();

    virtual Element* GetCopy();

   protected:
    void CalculateBlockSize(double numInNodes);

    MathExprParser m_fparser;
    wxString m_mathExpression = "sqrt(x^2 + y^2)";
    wxString m_variables = "x,y";
    std::vector<wxString> m_variablesVector;
    std::vector<OpenGLText*> m_glTextInputVector;
    OpenGLText m_symbol;
    wxSize m_symbolSize;
    int m_maxSringSize = 0;
    double m_minimumSize = 36.0;
};

#endif  // MATHEXPRESSION_H
