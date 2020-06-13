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

#ifndef LOADFORM_H
#define LOADFORM_H
#include "ElementFormBase.h"

class Load;
class SwitchingForm;

/**
 * @class LoadForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the load power data.
 * @file LoadForm.h
 */
class LoadForm : public LoadFormBase
{
   public:
    LoadForm(wxWindow* parent, Load* load);
    virtual ~LoadForm();

    virtual bool ValidateData();

   protected:
    virtual void OnCheckBoxCompLoadClick(wxCommandEvent& event) { UpdateZIPLoadFieldStatus(); }
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); };
    virtual void OnOnButtonClick(wxCommandEvent& event);
    virtual void OnStabilityButtonClick(wxCommandEvent& event);
    virtual void UpdateZIPLoadFieldStatus();

    wxWindow* m_parent = nullptr;
    Load* m_load = nullptr;
};
#endif  // LOADFORM_H
