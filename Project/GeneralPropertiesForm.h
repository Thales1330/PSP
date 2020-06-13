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

#ifndef GENERALPROPERTIESFORM_H
#define GENERALPROPERTIESFORM_H

#include "PropertiesFormBase.h"

#include <wx/textfile.h>
#include <wx/msgdlg.h>

class PropertiesData;

/**
 * @class GeneralPropertiesForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the software's general data.
 * @file GeneralPropertiesForm.h
 */
class GeneralPropertiesForm : public GeneralPropertiesFormBase
{
   public:
    GeneralPropertiesForm(wxWindow* parent, PropertiesData* properties);
    virtual ~GeneralPropertiesForm();

   protected:
    virtual void OnButtonCancelClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnButtonOKClick(wxCommandEvent& event);
    virtual bool ValidateData();

    PropertiesData* m_properties = nullptr;
};
#endif  // GENERALPROPERTIESFORM_H
