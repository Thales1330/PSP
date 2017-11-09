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

#ifndef TEXTFORM_H
#define TEXTFORM_H
#include "base/ElementFormBase.h"

#include "Text.h"
#include "ElectricCalculation.h"

/**
 * @class TextForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the text graphical data.
 * @file TextForm.h
 */
class TextForm : public TextFormBase
{
   public:
    TextForm(wxWindow* parent, Text* text, std::vector<Element*> elementList, double systemPowerBase = 100e6);
    virtual ~TextForm();

    virtual bool LoadChoices();

    virtual void ElementTypeChoice();
    virtual void ElementNumberChoice();
    virtual void DataTypeChoice();
    virtual void UnitChoice();

    virtual void Preview();
    virtual bool ValidateData();

   protected:
    virtual void OnUnitChoiceSelected(wxCommandEvent& event);
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnOKButtonClick(wxCommandEvent& event);
    virtual void OnElementChoiceSelected(wxCommandEvent& event);
    virtual void OnFromBusChoiceSelected(wxCommandEvent& event);
    virtual void OnNameChoiceSelected(wxCommandEvent& event);
    virtual void OnTextEnter(wxCommandEvent& event);
    virtual void OnToBusChoiceSelected(wxCommandEvent& event);
    virtual void OnTypeChoiceSelected(wxCommandEvent& event);

    Text* m_text = NULL;
    Text* m_textToEdit = NULL;
    wxWindow* m_parent = NULL;
    ElectricCalculation m_allElements;
    double m_systemPowerBase;
};
#endif  // TEXTFORM_H
