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

#ifndef LINEFORM_H
#define LINEFORM_H
#include "base/ElementFormBase.h"

class Line;
class SwitchingForm;

/**
 * @class LineForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the line power data.
 * @file LineForm.h
 */
class LineForm : public LineFormBase
{
   public:
    LineForm(wxWindow* parent, Line* line);
    virtual ~LineForm();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);
    virtual void OnStabilityButtonClick(wxCommandEvent& event);
    virtual void ReplaceStaticTextLabelChar(wxStaticText* staticText, wchar_t newChar);
    virtual bool ValidateData();

    wxWindow* m_parent = NULL;
    Line* m_line = NULL;
};
#endif  // LINEFORM_H
