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

#ifndef TRANSFERFUNCTION_H
#define TRANSFERFUNCTION_H

#include "ControlElement.h"

#include <wx/dcscreen.h>
#include "OpenGLText.h"

class TransferFunctionForm;

/**
 * @class TransferFunction
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Calculates the time response by a frequency domain transfer function.
 * @file TransferFunction.h
 */
class TransferFunction : public ControlElement
{
   public:
    struct SpaceState {
        std::vector<std::vector<double> > A;
        std::vector<double> B;
        std::vector<double> C;
        double D;
    };

    TransferFunction(int id);
    ~TransferFunction();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual void Rotate(bool clockwise = true);

    virtual std::vector<double> GetNumerator() const { return m_numerator; }
    virtual std::vector<double> GetDenominator() const { return m_denominator; }
    virtual void SetNumerator(std::vector<double> numerator) { m_numerator = numerator; }
    virtual void SetDenominator(std::vector<double> denominator) { m_denominator = denominator; }
    virtual void UpdateTFText();
    virtual bool UpdateText();
    virtual SpaceState GetSpaceState() { return m_ss; }
    /**
     * @brief Convert the transfer function to space state on controllable canonical form (CCF).
     * @param maxIteration Max number of solution iteration.
     * @param error Tolerance for calculation.
     */
    virtual void CalculateSpaceState(int maxIteration = 100, double error = 1e-3);
    /**
     * @brief Calculates the time response by the space state form of transfer function.
     *
     * Uses the implicit trapezoidal rule to solve:
     * <center>\f$ y_{n+1}=y_n+\frac{1}{2}h(y'_n+y'_{n+1}) \f$</center>
     * @param input Input value.
     * @param timeStep Time step.
     * @return true if the calculation converges, false otherwise.
     */
    virtual bool Solve(double input, double timeStep, double currentTime);

    virtual Element* GetCopy();

   protected:
    virtual void SetText(wxString numerator, wxString denominator);
    virtual wxString GetSuperscriptNumber(int number);
    virtual void GetTFString(wxString& numerator, wxString& denominator);

    wchar_t m_supNumber[10];

    OpenGLText* m_glTextNum = NULL;
    OpenGLText* m_glTextDen = NULL;
    int m_fontSize = 10;

    std::vector<double> m_numerator;
    std::vector<double> m_denominator;
    SpaceState m_ss;

    std::vector<double> m_x;
    std::vector<double> m_dx;
    double m_error = 1e-3;
    int m_maxIteration = 100;
};

#endif  // TRANSFERFUNCTION_H
