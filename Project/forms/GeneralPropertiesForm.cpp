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
#include "../utils/PropertiesData.h"

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
	//m_choiceRender->Clear();
	//m_choiceRender->Insert(_("OpenGL"), 0);
	//m_choiceRender->Insert(_("Device Context"), 0);
	m_choicePlotLib->Clear();
	m_choicePlotLib->Insert(_("Chart Director"), 0);
	m_choicePlotLib->Insert(_("wxMathPlot"), 1);
	


	switch (data.language) {
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
	switch (data.plotLib) {
	case PlotLib::wxCHART_DIR: {
		m_choicePlotLib->SetSelection(0);
	} break;
	case PlotLib::wxMATH_PLOT: {
		m_choicePlotLib->SetSelection(1);
	} break;
	}
	switch (data.theme) {
	case THEME_LIGHT: {
		m_choiceTheme->SetSelection(0);
	} break;
	case THEME_DARK: {
		m_choiceTheme->SetSelection(1);
	} break;
	}
	//if (data.useOpenGL) m_choiceRender->SetSelection(0);
	//else m_choiceRender->SetSelection(1);
	m_filePickerATPFolder->SetPath(data.atpPath.GetFullPath());
}

GeneralPropertiesForm::~GeneralPropertiesForm() {}
void GeneralPropertiesForm::OnButtonOKClick(wxCommandEvent& event)
{
	if (ValidateData()) EndModal(wxID_OK);
}

bool GeneralPropertiesForm::ValidateData()
{
	auto data = m_properties->GetGeneralPropertiesData();
	auto checkData = m_properties->GetGeneralPropertiesData();
	bool needRestart = false;
	data.atpPath = wxFileName(m_filePickerATPFolder->GetPath());

	//wxTextFile file("config.ini");
	wxFileName fn(wxStandardPaths::Get().GetDocumentsDir() + wxFileName::GetPathSeparator() + "PSP-UFU" + wxFileName::GetPathSeparator() + "config.ini");
	wxTextFile file(fn.GetFullPath());
	if (!file.Create()) {
		if (!file.Open()) {
			// Fail to access the file.
			wxMessageDialog msgDialog(this,
				_("It was not possible to access the init file.\nThe settings won't be applied."),
				_("Error"), wxOK | wxCENTRE | wxICON_ERROR);
			msgDialog.ShowModal();
		}
		file.Clear();
	}

	wxString line = "lang=";
	switch (m_choiceLanguage->GetSelection()) {
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
	if (data.language != checkData.language) needRestart = true;

	line = "plotlib=";
	switch (m_choicePlotLib->GetSelection()) {
	case 0: {
		line += "chartdir";
		data.plotLib = PlotLib::wxCHART_DIR;
	} break;
	case 1: {
		line += "mathplot";
		data.plotLib = PlotLib::wxMATH_PLOT;
	} break;
	}
	file.AddLine(line);
	//if (data.plotLib != checkData.plotLib) hasChanges = true;

	line = "theme=";
	switch (m_choiceTheme->GetSelection()) {
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
	if (data.theme != checkData.theme) needRestart = true;

	//line = "useOpenGL=";
	//switch (m_choiceRender->GetSelection()) {
	//case 0: {
	//    line += "yes";
	//    data.useOpenGL = true;
	//} break;
	//case 1: {
	//    line += "no";
	//    data.useOpenGL = false;
	//} break;
	//}
	//file.AddLine(line);
	//if (data.useOpenGL != checkData.useOpenGL) hasChanges = true;

	line = "atpfile=";
	line += data.atpPath.GetFullPath();
	file.AddLine(line);	

	file.Write();
	file.Close();

	if (needRestart) {
		wxMessageDialog msgDialog(this, _("The application must be restarted to settings changes be applied."),
			_("Info"), wxOK | wxCENTRE | wxICON_INFORMATION);
		msgDialog.ShowModal();
	}
	m_properties->SetGeneralPropertiesData(data);
	return true;
}
