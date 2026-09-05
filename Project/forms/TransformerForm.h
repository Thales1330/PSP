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

#ifndef TRANSFORMERFORM_H
#define TRANSFORMERFORM_H

#include "ElementFormBase.h"

class SwitchingForm;
class Transformer;

/**
 * @class TransformerForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @author Luiz Gonzaga Rocha Junior <luizgrj@gmail.com>
 * @date 05/10/2017
 * @brief Form to edit the transformer power data and OLTC tap changer.
 * @file TransformerForm.h
 */
class TransformerForm : public TransformerFormBase
{
   public:
    TransformerForm(wxWindow* parent, Transformer* transformer);
    virtual ~TransformerForm();
    bool ValidateData();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);
    virtual void OnStabilityButtonClick(wxCommandEvent& event);
    void OnEnableTapChangerCheck(wxCommandEvent& event);
    void OnDiscreteTapCheck(wxCommandEvent& event);
    void UpdateTapChangerFields();

    wxWindow* m_parent = nullptr;
    Transformer* m_transformer = nullptr;

    // OLTC Controls
    wxPanel* m_panelTapChanger = nullptr;
    wxCheckBox* m_checkEnableTapChanger = nullptr;
    wxStaticText* m_staticTextControlledBus = nullptr;
    wxChoice* m_choiceControlledBus = nullptr;
    wxStaticText* m_staticTextTargetVoltage = nullptr;
    wxTextCtrl* m_textCtrlTargetVoltage = nullptr;
    wxStaticText* m_staticTextDeadband = nullptr;
    wxTextCtrl* m_textCtrlDeadband = nullptr;
    wxStaticText* m_staticTextMinTap = nullptr;
    wxTextCtrl* m_textCtrlMinTap = nullptr;
    wxStaticText* m_staticTextMaxTap = nullptr;
    wxTextCtrl* m_textCtrlMaxTap = nullptr;
    wxCheckBox* m_checkDiscreteTap = nullptr;
    wxStaticText* m_staticTextTapStep = nullptr;
    wxTextCtrl* m_textCtrlTapStep = nullptr;
};
#endif  // TRANSFORMERFORM_H
