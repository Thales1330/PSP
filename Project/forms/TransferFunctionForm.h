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

#ifndef TRANSFERFUNCTIONFORM_H
#define TRANSFERFUNCTIONFORM_H

#include "ElementFormBase.h"

class TransferFunction;

/**
 * @class TransferFunctionForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the transfer function control data.
 * @file TransferFunctionForm.h
 */
class TransferFunctionForm : public TransferFunctionFormBase
{
   public:
    TransferFunctionForm(wxWindow* parent, TransferFunction* transferFunction);
    virtual ~TransferFunctionForm();
    bool ValidateData();

   protected:
    virtual void OnCancelClick(wxCommandEvent& event);
    virtual void OnOKClick(wxCommandEvent& event);
    void LoadTFData();

    wxWindow* m_parent = nullptr;
    TransferFunction* m_tf = nullptr;
};
#endif  // TRANSFERFUNCTIONFORM_H
