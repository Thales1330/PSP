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

#ifndef GENERATORSTABFORM_H
#define GENERATORSTABFORM_H

#include <wx/glcanvas.h>
#include "ElementFormBase.h"

class SwitchingForm;
class SyncGenerator;
class ControlEditor;
class ControlElementContainer;

/**
 * @class GeneratorStabForm
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Form to edit the synchronous generator data for electromechanical studies.
 * @file GeneratorStabForm.h
 */
class GeneratorStabForm : public GeneratorStabFormBase
{
   public:
    GeneratorStabForm(wxWindow* parent, SyncGenerator* syncGenerator, wxGLContext* sharedGLContext);
    virtual ~GeneratorStabForm();

   protected:
    virtual void UseAVRClick(wxCommandEvent& event);
    virtual void UseSGClick(wxCommandEvent& event);
    virtual void OnCancelButtonClick(wxCommandEvent& event);
    virtual void OnEditAVRButtonClick(wxCommandEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);
    virtual void OnSpeedGovernorButtonClick(wxCommandEvent& event);
    virtual void OnSwitchingButtonClick(wxCommandEvent& event);
    wxGLContext* GetSharedGLContext() const { return m_sharedGLContext; }

    virtual bool ValidateData();

    SyncGenerator* m_syncGenerator = NULL;
    wxWindow* m_parent = NULL;
    wxGLContext* m_sharedGLContext = NULL;
};
#endif  // GENERATORSTABFORM_H
