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

#ifndef SIMULATIONSSETTINGSFORM_H
#define SIMULATIONSSETTINGSFORM_H

#include "PropertiesFormBase.h"

class PropertiesData;

/**
 * @class SimulationsSettingsForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the simulation data.
 * @file SimulationsSettingsForm.h
 */
class SimulationsSettingsForm : public SimulationsSettingsFormBase
{
   public:
    SimulationsSettingsForm(wxWindow* parent, PropertiesData* properties, wxLocale* locale);
    virtual ~SimulationsSettingsForm();

   protected:
    virtual void OnBusFreqEstimationSelect(wxCommandEvent& event);
    virtual void OnCheckboxUseCompLoadClick(wxCommandEvent& event) { UpdateZIPLoadFieldStatus(); }
    virtual void OnPFMethodChoiceSelected(wxCommandEvent& event);
    virtual void OnButtonCancelClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnButtonOKClick(wxCommandEvent& event);
    virtual bool ValidateData();
    virtual void UpdateZIPLoadFieldStatus();
    virtual void UpdatePFFieldStatus();
    virtual void UpdateBusFreqFieldStatus();

    PropertiesData* m_properties;
};
#endif  // SIMULATIONSSETTINGSFORM_H
