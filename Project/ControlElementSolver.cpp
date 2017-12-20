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

#include "ControlElementSolver.h"

#include "ControlElementContainer.h"
#include "ControlEditor.h"
#include "ConnectionLine.h"
#include "Constant.h"
#include "Exponential.h"
#include "Gain.h"
#include "IOControl.h"
#include "Limiter.h"
#include "Multiplier.h"
#include "RateLimiter.h"
#include "Sum.h"
#include "TransferFunction.h"

ControlElementSolver::ControlElementSolver(ControlEditor* controlEditor, double timeStep, double integrationError)
{
    m_ctrlContainer = new ControlElementContainer();
    m_ctrlContainer->FillContainer(controlEditor);
    Initialize(controlEditor, timeStep, integrationError);
}

ControlElementSolver::ControlElementSolver(ControlElementContainer* ctrlContainer,
                                           double timeStep,
                                           double integrationError,
                                           wxWindow* parent)
{
    m_ctrlContainer = ctrlContainer;
    Initialize(parent, timeStep, integrationError);
}

void ControlElementSolver::Initialize(wxWindow* parent, double timeStep, double integrationError)
{
    // Check if the sistem have one input and one output
    bool fail = false;
    auto ioList = m_ctrlContainer->GetIOControlList();
    if(ioList.size() < 2) {
        fail = true;
        m_failMessage = _("The control system must have at least one input and one output.");
    }
    bool haveInput, haveOutput;
    haveInput = haveOutput = false;
    for(auto it = ioList.begin(), itEnd = ioList.end(); it != itEnd; ++it) {
        IOControl* io = *it;
        if(io->GetType() == Node::NODE_OUT && !haveInput) {
            m_inputControl = io;
            haveInput = true;
        } else if(io->GetType() == Node::NODE_IN) {
            m_outputControl = io;
            haveOutput = true;
        }
    }
    if(!fail && !haveInput) {
        fail = true;
        m_failMessage = _("There is no input in the control system.");
    }
    if(!fail && !haveOutput) {
        fail = true;
        m_failMessage = _("There is no output in the control system.");
    }
    if(!fail) {
        if(m_inputControl->GetChildList().size() == 0) {
            fail = true;
            m_failMessage = _("Input not connected.");
        }
    }

    m_timeStep = timeStep;
    m_integrationError = integrationError;
    if(!fail) {
        if(!InitializeValues(true)) {
            fail = true;
            m_failMessage = _("It was not possible to initialize the control system.");
        }
    }

    if(fail) {
        wxMessageDialog msgDialog(parent, m_failMessage, _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
    } else {
        m_isOK = true;
    }
}

bool ControlElementSolver::InitializeValues(bool startAllZero)
{
    // Reset Elements values
    auto elementList = m_ctrlContainer->GetControlElementsList();
    for(auto it = elementList.begin(), itEnd = elementList.end(); it != itEnd; ++it) {
        ControlElement* element = *it;
        element->SetSolved(false);
        element->SetOutput(0.0);
    }
    auto tfList = m_ctrlContainer->GetTFList();
    for(auto it = tfList.begin(), itEnd = tfList.end(); it != itEnd; ++it) {
        TransferFunction* tf = *it;
        tf->CalculateSpaceState(100, m_integrationError);
    }
    auto connectionLineList = m_ctrlContainer->GetConnectionLineList();
    for(auto it = connectionLineList.begin(), itEnd = connectionLineList.end(); it != itEnd; ++it) {
        ConnectionLine* cLine = *it;
        cLine->SetSolved(false);
        cLine->SetValue(0.0);
    }

    if(!startAllZero) {
        double origTimeStep = m_timeStep;
        double minStep = m_timeStep / 10;
        double maxStep = m_timeStep * 10;
        // Calculate the steady-state results according to the input.
        double minError = 1e-7 * m_timeStep;
        int maxIteration = 100 / m_timeStep;

        double prevSol = 0.0;
        double currentSol = 1.0;
        double error = 1.0;
        double prevError = 1.0;
        int numIt = 0;
        while(error > minError) {
            prevSol = currentSol;
            prevError = error;
            SolveNextStep();
            currentSol = GetLastSolution();
            numIt++;
            error = std::abs(prevSol - currentSol);
            if(std::abs(error - prevError) < 1e-1) {
                if(m_timeStep < maxStep) {
                    m_timeStep *= 1.5;
                }
            } else if(std::abs(error - prevError) > 10) {
                if(m_timeStep > minStep) {
                    m_timeStep /= 1.5;
                }
            }
            if(numIt >= maxIteration) {
                m_failMessage = _("It was not possible to initialize the control system.");
                return false;
            }
        }
        m_timeStep = origTimeStep;
        m_solutions.clear();
    }

    return true;
}

void ControlElementSolver::SolveNextStep()
{
    // Set all elements as not solved
    auto elementList = m_ctrlContainer->GetControlElementsList();
    for(auto it = elementList.begin(), itEnd = elementList.end(); it != itEnd; ++it) {
        ControlElement* element = *it;
        element->SetSolved(false);
    }
    auto connectionLineList = m_ctrlContainer->GetConnectionLineList();
    for(auto it = connectionLineList.begin(), itEnd = connectionLineList.end(); it != itEnd; ++it) {
        ConnectionLine* cLine = *it;
        cLine->SetSolved(false);
    }

    // Get first node connection
    ConnectionLine* firstConn = static_cast<ConnectionLine*>(m_inputControl->GetChildList()[0]);
    /*m_inputControl->SetSolved();
    firstConn->SetValue(1);
    firstConn->SetSolved();
    FillAllConnectedChildren(firstConn);*/

    // Set value to the connected lines in constants
    auto constantList = m_ctrlContainer->GetConstantList();
    for(auto it = constantList.begin(), itEnd = constantList.end(); it != itEnd; ++it) {
        Constant* constant = *it;
        if(constant->GetChildList().size() == 1) {
            constant->SetSolved();
            ConnectionLine* child = static_cast<ConnectionLine*>(constant->GetChildList()[0]);
            child->SetValue(constant->GetValue());
            child->SetSolved();
            FillAllConnectedChildren(child);
        }
    }

    // Set value to the connected lines in inputs
    auto ioList = m_ctrlContainer->GetIOControlList();
    for(auto it = ioList.begin(), itEnd = ioList.end(); it != itEnd; ++it) {
        IOControl* io = *it;
        if(io->GetChildList().size() == 1) {
            io->SetSolved();
            ConnectionLine* child = static_cast<ConnectionLine*>(io->GetChildList()[0]);
            if(m_inputControl == io) firstConn = child;
            bool inputType = true;
            io->SetSolved();
            switch(io->GetValue()) {
                case IOControl::IN_TERMINAL_VOLTAGE: {
                    child->SetValue(m_terminalVoltage);
                } break;
                case IOControl::IN_VELOCITY: {
                    child->SetValue(m_velocity);
                } break;
                case IOControl::IN_ACTIVE_POWER: {
                    child->SetValue(m_activePower);
                } break;
                case IOControl::IN_REACTIVE_POWER: {
                    child->SetValue(m_reactivePower);
                } break;
                case IOControl::IN_INITIAL_TERMINAL_VOLTAGE: {
                    child->SetValue(m_initTerminalVoltage);
                } break;
                case IOControl::IN_INITIAL_MEC_POWER: {
                    child->SetValue(m_initMecPower);
                } break;
                case IOControl::IN_INITIAL_VELOCITY: {
                    child->SetValue(m_initVelocity);
                } break;
                case IOControl::IN_DELTA_VELOCITY: {
                    child->SetValue(m_deltaVelocity);
                } break;
                case IOControl::IN_DELTA_ACTIVE_POWER: {
                    child->SetValue(m_deltaPe);
                } break;
                default: {
                    inputType = false;
                    io->SetSolved(false);
                } break;
            }
            if(inputType) {
                child->SetSolved();
                FillAllConnectedChildren(child);
            }
        }
    }

    ConnectionLine* currentLine = firstConn;
    while(currentLine) {
        currentLine = SolveNextElement(currentLine);
    }

    bool haveUnsolvedElement = true;
    while(haveUnsolvedElement) {
        haveUnsolvedElement = false;
        // Get the solved line connected with unsolved element (elements not connected in the main branch).
        for(auto it = connectionLineList.begin(), itEnd = connectionLineList.end(); it != itEnd; ++it) {
            ConnectionLine* cLine = *it;
            if(cLine->IsSolved()) {
                auto parentList = cLine->GetParentList();
                for(auto itP = parentList.begin(), itPEnd = parentList.end(); itP != itPEnd; ++itP) {
                    ControlElement* parent = static_cast<ControlElement*>(*itP);
                    if(!parent->IsSolved()) {
                        haveUnsolvedElement = true;
                        // Solve secondary branch.
                        currentLine = cLine;
                        while(currentLine) {
                            currentLine = SolveNextElement(currentLine);
                        }
                        break;
                    }
                }
            }
            if(haveUnsolvedElement) break;
        }
    }

    // Set the control system output.
    /*if(m_outputControl->GetChildList().size() == 1) {
        ConnectionLine* cLine = static_cast<ConnectionLine*>(m_outputControl->GetChildList()[0]);
        m_solutions.push_back(cLine->GetValue());
    } else
        m_solutions.push_back(0.0);*/
    for(auto it = ioList.begin(), itEnd = ioList.end(); it != itEnd; ++it) {
        IOControl* io = *it;
        if(io->GetChildList().size() == 1) {
            io->SetSolved();
            ConnectionLine* child = static_cast<ConnectionLine*>(io->GetChildList()[0]);
            switch(io->GetValue()) {
                case IOControl::OUT_MEC_POWER: {
                    m_mecPower = child->GetValue();
                    m_solutions.push_back(m_mecPower);
                } break;
                case IOControl::OUT_FIELD_VOLTAGE: {
                    m_fieldVoltage = child->GetValue();
                    m_solutions.push_back(m_fieldVoltage);
                } break;
                default:
                    break;
            }
        }
    }
}

void ControlElementSolver::FillAllConnectedChildren(ConnectionLine* parent)
{
    auto childList = parent->GetLineChildList();
    for(auto it = childList.begin(), itEnd = childList.end(); it != itEnd; ++it) {
        ConnectionLine* child = *it;
        child->SetValue(parent->GetValue());
        child->SetSolved();
        FillAllConnectedChildren(child);
    }
}

ConnectionLine* ControlElementSolver::SolveNextElement(ConnectionLine* currentLine)
{
    auto parentList = currentLine->GetParentList();
    for(auto it = parentList.begin(), itEnd = parentList.end(); it != itEnd; ++it) {
        ControlElement* element = static_cast<ControlElement*>(*it);
        // Solve the unsolved parent.
        if(!element->IsSolved()) {
            if(!element->Solve(currentLine->GetValue(), m_timeStep)) return NULL;
            element->SetSolved();

            // Get the output node (must have one or will result NULL).
            Node* outNode = NULL;
            auto nodeList = element->GetNodeList();
            for(auto itN = nodeList.begin(), itNEnd = nodeList.end(); itN != itNEnd; ++itN) {
                Node* node = *itN;
                if(node->GetNodeType() == Node::NODE_OUT) outNode = node;
            }
            if(!outNode) return NULL;

            // Set connection line value associated with the output node.
            auto childList = element->GetChildList();
            for(auto itC = childList.begin(), itCEnd = childList.end(); itC != itCEnd; ++itC) {
                ConnectionLine* cLine = static_cast<ConnectionLine*>(*itC);
                if(!cLine->IsSolved()) {  // Only check unsolved lines
                    // Check if the connection line have the output node on the list
                    auto lineNodeList = cLine->GetNodeList();
                    for(auto itCN = nodeList.begin(), itCNEnd = nodeList.end(); itCN != itCNEnd; ++itCN) {
                        Node* childNode = *itCN;
                        if(childNode == outNode) {
                            // Check if the line connect two elements, otherwise return NULL
                            if(cLine->GetType() != ConnectionLine::ELEMENT_ELEMENT) return NULL;

                            // Set the connection line value and return it.
                            cLine->SetValue(element->GetOutput());
                            cLine->SetSolved();
                            FillAllConnectedChildren(cLine);
                            return cLine;
                        }
                    }
                }
            }
        }
    }
    return NULL;
}
