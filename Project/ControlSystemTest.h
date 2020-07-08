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

#ifndef CONTROLSYSTEMTEST_H
#define CONTROLSYSTEMTEST_H

#include <GL/glew.h>

#include "ControlEditorBase.h"

class ControlEditor;

/**
 * @class ControlSystemTest
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit properties to test the control system created.
 * @file ControlSystemTest.h
 */
class ControlSystemTest : public ControlSystemTestBase
{
   public:
    ControlSystemTest(ControlEditor* parent,
                      int* inputType,
                      double* startTime,
                      double* slope,
                      double* timeStep,
                      double* simTime);
    virtual ~ControlSystemTest();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnRunButtonClick(wxCommandEvent& event);
    int* m_inputType = nullptr;
    double* m_startTime = nullptr;
    double* m_slope = nullptr;
    double* m_timeStep = nullptr;
    double* m_simTime = nullptr;
};
#endif  // CONTROLSYSTEMTEST_H
