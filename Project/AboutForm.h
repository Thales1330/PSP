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

#ifndef ABOUTFORM_H
#define ABOUTFORM_H
#include "PropertiesForm.h"

#include <wx/textfile.h>
#include <wx/stdpaths.h>

/**
 * @class AboutForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to show some informations.
 * @file AboutForm.h
 */
class AboutForm : public AboutFormBase
{
   public:
    AboutForm(wxWindow* parent);
    virtual ~AboutForm();
    virtual void Init();

   protected:
    virtual void OnOKButtonClick(wxCommandEvent& event) { EndModal(wxID_OK); };
};
#endif  // ABOUTFORM_H
