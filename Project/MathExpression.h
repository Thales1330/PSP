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
//#include "OpenGLText.h"
#include "GCText.h"

class ConnectionLine;

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

    //virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);

    virtual bool Solve(double* input, double timeStep);
    virtual bool Initialize();
    virtual wxString GetMathExpression() { return m_mathExpression; }
    virtual void SetMathExpression(wxString mathExpression) { m_mathExpression = mathExpression; }
    virtual std::vector<wxString> GetVariables() { return m_variablesVector; }
    virtual void SetVariables(std::vector<wxString> variablesVector);
    virtual MathExprParser GetParser() { return m_fparser; }

    virtual void UpdatePoints();
    void AddInNode();
    void RemoveInNode();
    virtual bool UpdateText();

    virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
    virtual bool OpenElement(rapidxml::xml_node<>* elementNode);

    virtual Element* GetCopy();

   protected:
    void CalculateBlockSize(double numInNodes);

    MathExprParser m_fparser;
    wxString m_mathExpression = "sqrt(x^2 + y^2)";
    wxString m_variables = "time,step,x,y";
    std::vector<wxString> m_variablesVector;
    double* m_inputValues = nullptr;
    std::vector<GCText*> m_gcTextInputVector;
    GCText m_symbol;
    wxSize m_symbolSize;
    int m_maxSringSize = 0;
    double m_minimumSize = 36.0;
};

#endif  // MATHEXPRESSION_H
