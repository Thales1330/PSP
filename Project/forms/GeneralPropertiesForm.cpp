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
#include <wx/fontenum.h>
#include <wx/font.h>
#include <wx/stdpaths.h>
#include <wx/app.h>
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
	m_checkBoxElementsToolBar->SetValue(data.showElementsToolBar);

	wxFont currentFont(data.labelFontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, data.labelFont);
	m_fontPickerText->SetSelectedFont(currentFont);
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
	wxFont slectedFont = m_fontPickerText->GetSelectedFont();
	data.labelFont = slectedFont.GetFaceName();
	data.labelFontSize = slectedFont.GetPointSize();

	switch (m_choiceLanguage->GetSelection()) {
	case 0: {
		data.language = wxLANGUAGE_ENGLISH;
	} break;
	case 1: {
		data.language = wxLANGUAGE_PORTUGUESE_BRAZILIAN;
	} break;
	}
	if (data.language != checkData.language) needRestart = true;

	switch (m_choicePlotLib->GetSelection()) {
	case 0: {
		data.plotLib = PlotLib::wxCHART_DIR;
	} break;
	case 1: {
		data.plotLib = PlotLib::wxMATH_PLOT;
	} break;
	}

	switch (m_choiceTheme->GetSelection()) {
	case 0: {
		data.theme = THEME_LIGHT;
		wxTheApp->SetAppearance(wxApp::Appearance::Light);
	} break;
	case 1: {
		data.theme = THEME_DARK;
		wxTheApp->SetAppearance(wxApp::Appearance::Dark);
	} break;
	}
	if (data.theme != checkData.theme) needRestart = true;

	data.showElementsToolBar = m_checkBoxElementsToolBar->GetValue();

	if (!PropertiesData::SaveConfigFile(data)) {
		wxMessageDialog msgDialog(this,
			_("It was not possible to access the init file.\nThe settings won't be applied."),
			_("Error"), wxOK | wxCENTRE | wxICON_ERROR);
		msgDialog.ShowModal();
	}

	if (needRestart) {
		wxMessageDialog msgDialog(this, _("The application must be restarted to settings changes be applied."),
			_("Info"), wxOK | wxCENTRE | wxICON_INFORMATION);
		msgDialog.ShowModal();
	}
	m_properties->SetGeneralPropertiesData(data);
	return true;
}

void GeneralPropertiesForm::OnThemeSelected(wxCommandEvent& event)
{
	switch (event.GetSelection())
	{
	case 0:
		wxTheApp->SetAppearance(wxApp::Appearance::Light);
		break;
	case 1:
		wxTheApp->SetAppearance(wxApp::Appearance::Dark);
		break;
	}
}
