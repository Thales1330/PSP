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

#ifndef SUMFORM_H
#define SUMFORM_H

#include "ElementForm.h"

class Sum;

/**
 * @class SumForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the sum control data.
 * @file SumForm.h
 */
class SumForm : public SumFormBase
{
   public:
    SumForm(wxWindow* parent, Sum* sum);
    virtual ~SumForm();

    bool ValidateData();

   protected:
    virtual void OnCancelClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnOKClick(wxCommandEvent& event);
    wxWindow* m_parent = NULL;
    Sum* m_sum = NULL;
};
#endif  // SUMFORM_H
