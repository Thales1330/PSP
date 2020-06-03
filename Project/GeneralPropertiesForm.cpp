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

#include "GeneralPropertiesForm.h"
#include "PropertiesData.h"

GeneralPropertiesForm::GeneralPropertiesForm(wxWindow* parent, PropertiesData* properties)
    : GeneralPropertiesFormBase(parent)
{
    m_properties = properties;
    auto data = m_properties->GetGeneralPropertiesData();

    // Clear the choices and rebuild to set the correct translations.
    m_choiceLanguage->Clear();
    m_choiceLanguage->Insert(_("English"), 0);
    m_choiceLanguage->Insert(_("Portuguese"), 1);
    m_choiceTheme->Clear();
    m_choiceTheme->Insert(_("Light"), 0);
    m_choiceTheme->Insert(_("Dark"), 1);

    switch(data.language) {
        case wxLANGUAGE_ENGLISH: {
            m_choiceLanguage->SetSelection(0);
        } break;
        case wxLANGUAGE_PORTUGUESE_BRAZILIAN: {
            m_choiceLanguage->SetSelection(1);
        } break;
        default: {
            m_choiceLanguage->SetSelection(wxNOT_FOUND);
        } break;
    }
    switch(data.theme) {
        case THEME_LIGHT: {
            m_choiceTheme->SetSelection(0);
        } break;
        case THEME_DARK: {
            m_choiceTheme->SetSelection(1);
        } break;
    }
}

GeneralPropertiesForm::~GeneralPropertiesForm() {}
void GeneralPropertiesForm::OnButtonOKClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

bool GeneralPropertiesForm::ValidateData()
{
    auto data = m_properties->GetGeneralPropertiesData();
    auto checkData = m_properties->GetGeneralPropertiesData();
    bool hasChanges = false;

    //wxTextFile file("config.ini");
	wxFileName fn(wxStandardPaths::Get().GetExecutablePath());
	wxTextFile file(fn.GetPath() + wxFileName::GetPathSeparator() + "config.ini");
    if(!file.Create()) {
        if(!file.Open()) {
            // Fail to access the file.
            wxMessageDialog msgDialog(this,
                                      _("It was not possible to access the init file.\nThe settings won't be applied."),
                                      _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
            msgDialog.ShowModal();
        }
        file.Clear();
    }

    wxString line = "lang=";
    switch(m_choiceLanguage->GetSelection()) {
        case 0: {
            line += "en";
            data.language = wxLANGUAGE_ENGLISH;
        } break;
        case 1: {
            line += "pt-br";
            data.language = wxLANGUAGE_PORTUGUESE_BRAZILIAN;
        } break;
    }
    file.AddLine(line);
    if(data.language != checkData.language) hasChanges = true;

    line = "theme=";
    switch(m_choiceTheme->GetSelection()) {
        case 0: {
            line += "light";
            data.theme = THEME_LIGHT;
        } break;
        case 1: {
            line += "dark";
            data.theme = THEME_DARK;
        } break;
    }
    file.AddLine(line);
    if(data.theme != checkData.theme) hasChanges = true;

    file.Write();
    file.Close();

    if(hasChanges) {
        wxMessageDialog msgDialog(this, _("The application must be restarted to settings changes be applied."),
                                  _("Info"), wxOK | wxCENTRE | wxICON_INFORMATION);
        msgDialog.ShowModal();
    }
    m_properties->SetGeneralPropertiesData(data);
    return true;
}
