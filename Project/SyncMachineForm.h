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

#ifndef SYNCMACHINEFORM_H
#define SYNCMACHINEFORM_H

#include "base/ElementFormBase.h"

class GeneratorStabForm;
class SyncGenerator;
class SyncMotor;

/**
 * @class SyncMachineForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the synchronous machine power data.
 * @file SyncMachineForm.h
 */
class SyncMachineForm : public SyncMachineFormBase
{
   public:
    SyncMachineForm(wxWindow* parent, SyncGenerator* syncGenerator);
    SyncMachineForm(wxWindow* parent, SyncMotor* syncMotor);
    virtual ~SyncMachineForm();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); };
    virtual void OnCheckMaxReactive(wxCommandEvent& event);
    virtual void OnCheckMinReactive(wxCommandEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);
    virtual void OnStabilityButtonClick(wxCommandEvent& event);

    virtual bool ValidateData();
    virtual void ReplaceStaticTextLabelChar(wxStaticText* staticText, wchar_t newChar);

    SyncGenerator* m_syncGenerator = NULL;
    SyncMotor* m_syncMotor = NULL;
    wxWindow* m_parent = NULL;
};
#endif  // SYNCMACHINEFORM_H
