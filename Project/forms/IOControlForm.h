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

#ifndef IOCONTROLFORM_H
#define IOCONTROLFORM_H

#include "ElementFormBase.h"
#include "../elements/controlElement/IOControl.h"

/**
 * @class IOControlForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the input/output control data.
 * @file IOControlForm.h
 */
class IOControlForm : public IOControlFormBase
{
   public:
    IOControlForm(wxWindow* parent, IOControl* ioControl);
    virtual ~IOControlForm();

    virtual bool ValidateData();

   protected:
    virtual void OnInputChecked(wxCommandEvent& event);
    virtual void OnOutputChecked(wxCommandEvent& event);
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); };
    virtual void OnOKButtonClick(wxCommandEvent& event);

    wxWindow* m_parent;
    IOControl* m_ioControl;

    std::vector<IOControl::IOFlags> m_inputFlags;
    std::vector<IOControl::IOFlags> m_outputFlags;
};
#endif  // IOCONTROLFORM_H
