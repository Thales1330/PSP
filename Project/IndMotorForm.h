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

#ifndef INDMOTORFORM_H
#define INDMOTORFORM_H
#include "ElementFormBase.h"

class IndMotor;

/**
 * @class IndMotorForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the induction motor power data.
 * @file IndMotorForm.h
 */
class IndMotorForm : public IndMotorFormBase
{
   public:
    IndMotorForm(wxWindow* parent, IndMotor* indMotor);
    virtual ~IndMotorForm();
    virtual bool ValidateData();

   protected:
    virtual void OnCalcQInPFClick(wxCommandEvent& event);
    virtual void OnCheckboxUseCageFactorClick(wxCommandEvent& event);
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); };
    virtual void OnOKButtonClick(wxCommandEvent& event);
    virtual void OnStabilityButtonClick(wxCommandEvent& event);
    
    void UpdateFields();

    wxWindow* m_parent = NULL;
    IndMotor* m_indMotor = NULL;
};
#endif  // INDMOTORFORM_H
