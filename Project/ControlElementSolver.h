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

#ifndef CONTROLELEMENTSOLVER_H
#define CONTROLELEMENTSOLVER_H

#include <wx/window.h>
#include <vector>

class ControlElementContainer;
class ControlEditor;
class ConnectionLine;
class Constant;
class Exponential;
class Gain;
class IOControl;
class Limiter;
class Multiplier;
class RateLimiter;
class Sum;
class TransferFunction;

/**
 * @class ControlElementSolver
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Solves in the time the control system.
 * Can solve the control system directly from a ControlEditor or from a ControlEditorElement.
 * @file ControlElementSolver.h
 */
class ControlElementSolver
{
   public:
    ControlElementSolver() {}
    ControlElementSolver(ControlEditor* controlEditor, double timeStep = 1e-3, double integrationError = 1e-3);
    ControlElementSolver(ControlElementContainer* ctrlContainer,
                         double timeStep = 1e-3,
                         double integrationError = 1e-3,
                         wxWindow* parent = nullptr);
    virtual ~ControlElementSolver() {}
    virtual bool InitializeValues(bool startAllZero);
    virtual void SolveNextStep();
    virtual std::vector<double> GetSolutions() { return m_solutions; }
    virtual double GetLastSolution() { return m_solutions[m_solutions.size() - 1]; }
    virtual bool IsOK() const { return m_isOK; }
    virtual wxString GetErrorMessage() { return m_failMessage; }
    void SetSwitchStatus(bool value) { m_switchStatus = value; }
    void SetCurrentTime(double value) { m_currentTime = value; }
    void SetTerminalVoltage(double value) { m_terminalVoltage = value; }
    void SetVelocity(double value) { m_velocity = value; }
    void SetActivePower(double value) { m_activePower = value; }
    void SetReactivePower(double value) { m_reactivePower = value; }
    void SetInitialTerminalVoltage(double value) { m_initTerminalVoltage = value; }
    void SetInitialMecPower(double value) { m_initMecPower = value; }
    void SetInitialVelocity(double value) { m_initVelocity = value; }
    void SetDeltaVelocity(double value) { m_deltaVelocity = value; }
    void SetDeltaActivePower(double value) { m_deltaPe = value; }
    double GetFieldVoltage() { return m_fieldVoltage; }
    double GetMechanicalPower() { return m_mecPower; }
    double GetVelocity() { return m_velocity; }
    double GetActivePower() { return m_activePower; }
   protected:
    void Initialize(wxWindow* parent, double timeStep, double integrationError);
    void FillAllConnectedChildren(ConnectionLine* parent);
    ConnectionLine* SolveNextElement(ConnectionLine* currentLine);

    ControlElementContainer* m_ctrlContainer = nullptr;
    double m_timeStep = 1e-3;
    double m_integrationError = 1e-5;
    std::vector<double> m_solutions;
    bool m_isOK = false;
    wxString m_failMessage = _("Unknown error.");

    IOControl* m_inputControl = nullptr; /**< First input control to be solved */
    IOControl* m_outputControl = nullptr;
    // Inputs
    bool m_switchStatus = false;
    double m_currentTime = 0.0;
    double m_terminalVoltage = 0.0;
    double m_velocity = 0.0;
    double m_activePower = 0.0;
    double m_reactivePower = 0.0;
    double m_initTerminalVoltage = 0.0;
    double m_initMecPower = 0.0;
    double m_initVelocity = 0.0;
    double m_deltaVelocity = 0.0;
    double m_deltaPe = 0.0;
    
    /*
     * m_inputToSolve[0] = Current line value;
     * m_inputToSolve[1] = Current time;
     * m_inputToSolve[2] = Switch status.
     */
    double* m_inputToSolve = nullptr;
    // Outputs
    double m_fieldVoltage = 0.0;
    double m_mecPower = 0.0;
};

#endif  // CONTROLELEMENTSOLVER_H
