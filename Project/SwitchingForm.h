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

#ifndef SWITCHINGFORM_H
#define SWITCHINGFORM_H

#include "ElementForm.h"

class PowerElement;

/**
 * @class SwitchingForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the switching data of power elements for electromechanical transient studies.
 * @file SwitchingForm.h
 */
class SwitchingForm : public SwitchingFormBase
{
   public:
    SwitchingForm(wxWindow* parent);
    SwitchingForm(wxWindow* parent, PowerElement* element);
    virtual ~SwitchingForm();

   protected:
    virtual void OnDownButtonClick(wxCommandEvent& event);
    virtual void OnUpButtonClick(wxCommandEvent& event);
    virtual void OnChangeProperties(wxPropertyGridEvent& event);
    virtual void OnSelectItem(wxListEvent& event);
    virtual void OnCancelButtonClick(wxCommandEvent& event);
    virtual void OnInsertButtonClick(wxCommandEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);
    virtual void OnRemoveButtonClick(wxCommandEvent& event);

    int m_maxID = 0;

    PowerElement* m_element = NULL;
};
#endif  // SWITCHINGFORM_H
