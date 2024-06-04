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

#ifndef EXPONENTIALFORM_H
#define EXPONENTIALFORM_H
#include "ElementFormBase.h"

class Exponential;

/**
 * @class ExponentialForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the exponential control data.
 * @file ExponentialForm.h
 */
class ExponentialForm : public ExponentialFormBase
{
   public:
    ExponentialForm(wxWindow* parent, Exponential* exponential);
    virtual ~ExponentialForm();
    virtual bool ValidateData();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnOKButtonClick(wxCommandEvent& event);

    wxWindow* m_parent = nullptr;
    Exponential* m_exponential = nullptr;
};
#endif  // EXPONENTIALFORM_H
