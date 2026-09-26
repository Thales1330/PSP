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

#ifndef BUSFORM_H
#define BUSFORM_H

#include <wx/popupwin.h>
#include <wx/statbmp.h>

#include "ElementFormBase.h"
class Bus;


class BitmapPopup : public wxPopupWindow
{
public:
    BitmapPopup(wxWindow* parent);

    void SetBitmap(const wxBitmap& bitmap);

private:
    wxStaticBitmap* m_bitmap;
};

/**
 * @class BusForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the bus power data.
 * @file BusForm.h
 */
class BusForm : public BusFormBase
{
   public:
    BusForm(wxWindow* parent, Bus* bus, wxWindow* workspace);
    virtual ~BusForm();

   protected:
    void OnnConfigProtectionButtonClick(wxCommandEvent& event) override;
    void OnMouseMotionElectrodeConfig(wxMouseEvent& event) override;
    void OnMouseEnterElectrodeConfig(wxMouseEvent& event) override;
    void OnMouseLeaveElectrodeConfig(wxMouseEvent& event) override;
    virtual void OnFaultTypeChoice(wxCommandEvent& event);
    virtual void OnControlledVoltageClick(wxCommandEvent& event);
    virtual void OnInsertFaultClick(wxCommandEvent& event);
    virtual void OnInsertStabFaultClick(wxCommandEvent& event);
    virtual void OnNominalVoltageChoice(wxCommandEvent& event);
    virtual void OnButtonCancelClick(wxCommandEvent& event);
    virtual void OnButtonOKClick(wxCommandEvent& event);
    virtual void EnableCtrlVoltageFields(bool enable = true);
    virtual void EnableFaultFields(bool enable = true);
    virtual void EnableStabFaultFields(bool enable = true);
    virtual void UpdateChoiceBoxes();

    Bus* m_bus = nullptr;
    wxWindow* m_parent = nullptr;
    wxWindow* m_workspace = nullptr;
    BitmapPopup* m_electrodePreview;
};
#endif  // BUSFORM_H
