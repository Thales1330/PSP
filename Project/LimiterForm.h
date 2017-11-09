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

#ifndef LIMITERFORM_H
#define LIMITERFORM_H
#include "base/ElementFormBase.h"

class Limiter;

/**
 * @class LimiterForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the limit control data.
 * @file LimiterForm.h
 */
class LimiterForm : public LimiterFormBase
{
   public:
    LimiterForm(wxWindow* parent, Limiter* limiter);
    virtual ~LimiterForm();
    virtual bool ValidateData();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnOKButtonClick(wxCommandEvent& event);

    wxWindow* m_parent = NULL;
    Limiter* m_limiter = NULL;
};
#endif  // LIMITERFORM_H
