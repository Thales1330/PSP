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

#include "PropertiesData.h"

#include <wx/msgdlg.h>
#include <wx/textfile.h>
#include <wx/stdpaths.h>

PropertiesData::PropertiesData() { SetGUIColourTheme(); }
PropertiesData::~PropertiesData() {}

void PropertiesData::SetGUIColourTheme()
{
	if (m_genData.theme == THEME_LIGHT)
	{
		m_guiColour.background = wxColour(250, 249, 246);

		m_guiColour.text = wxColour(35, 35, 35);
		m_guiColour.enabled = wxColour(20, 20, 20);
		m_guiColour.disable = wxColour(145, 145, 145);

		m_guiColour.selection = wxColour(40, 120, 255, 90);
		m_guiColour.altSelection = wxColour(40, 200, 70, 90);

		m_guiColour.bus = wxColour(40, 105, 235);
		m_guiColour.slackBus = wxColour(40, 105, 235);

		m_guiColour.eventElement = wxColour(255, 120, 0);
		m_guiColour.pfArrow = wxColour(255, 120, 0);

		m_guiColour.swClose = wxColour(35, 150, 45);
		m_guiColour.swOpen = wxColour(225, 55, 55);
		m_guiColour.grid = wxColour(220, 222, 225);
	}
	else if (m_genData.theme == THEME_DARK)
	{
		m_guiColour.background = wxColour(43, 46, 52);

		m_guiColour.text = wxColour(205, 210, 215);
		m_guiColour.enabled = wxColour(220, 224, 228);
		m_guiColour.disable = wxColour(125, 130, 136);

		m_guiColour.selection = wxColour(70, 140, 255, 90);
		m_guiColour.altSelection = wxColour(70, 200, 90, 90);

		m_guiColour.bus = wxColour(70, 135, 255);
		m_guiColour.slackBus = wxColour(95, 155, 255);

		m_guiColour.eventElement = wxColour(240, 165, 60);
		m_guiColour.pfArrow = wxColour(240, 165, 60);

		m_guiColour.swClose = wxColour(60, 180, 70);
		m_guiColour.swOpen = wxColour(235, 85, 85);
		m_guiColour.grid = wxColour(60, 64, 72);
	}

	if (m_genData.voltageLevels.empty()) {
		m_genData.voltageLevels = GetDefaultVoltageLevels();
	}
	m_guiColour.voltageLevels = m_genData.voltageLevels;
}

std::vector<VoltageLevelColour> PropertiesData::GetDefaultVoltageLevels()
{
	return {
		{ 500.0, wxColour(0, 90, 220) },    // >= 500 kV - Azul Real
		{ 440.0, wxColour(160, 82, 45) },   // 440 kV - Marrom
		{ 345.0, wxColour(190, 110, 50) },  // 345 kV - Marrom claro / Ocre
		{ 230.0, wxColour(220, 35, 35) },   // 230 kV - Vermelho
		{ 138.0, wxColour(0, 155, 65) },    // 138 kV - Verde
		{ 69.0,  wxColour(145, 40, 205) },  // 69 kV  - Violeta / Roxo
		{ 34.5,  wxColour(215, 35, 135) },  // 34.5 kV - Magenta / Rosa
		{ 13.8,  wxColour(0, 165, 195) },   // 13.8 kV - Ciano / Azul claro
		{ 4.16,  wxColour(205, 150, 15) },  // 4.16 kV - Dourado / Âmbar
		{ 0.38,  wxColour(120, 125, 135) }  // 0.38 kV (380 V) - Cinza
	};
}

void PropertiesData::SetGeneralPropertiesData(GeneralData generalData)
{
	m_genData = generalData;
	if (m_genData.voltageLevels.empty()) {
		m_genData.voltageLevels = GetDefaultVoltageLevels();
	}
	m_guiColour.voltageLevels = m_genData.voltageLevels;
}

bool PropertiesData::SaveConfigFile(const GeneralData& data)
{
	wxFileName fn(wxStandardPaths::Get().GetDocumentsDir() + wxFileName::GetPathSeparator() + "PSP-UFU" + wxFileName::GetPathSeparator() + "config.ini");
	if (!fn.DirExists()) {
		fn.Mkdir();
	}
	wxTextFile file(fn.GetFullPath());
	if (!file.Create()) {
		if (!file.Open()) {
			return false;
		}
		file.Clear();
	}

	wxString line = "lang=";
	switch (data.language) {
	case wxLANGUAGE_PORTUGUESE_BRAZILIAN:
		line += "pt-br";
		break;
	case wxLANGUAGE_ENGLISH:
	default:
		line += "en";
		break;
	}
	file.AddLine(line);

	line = "plotlib=";
	switch (data.plotLib) {
	case PlotLib::wxMATH_PLOT:
		line += "mathplot";
		break;
	case PlotLib::wxCHART_DIR:
	default:
		line += "chartdir";
		break;
	}
	file.AddLine(line);

	line = "theme=";
	switch (data.theme) {
	case THEME_DARK:
		line += "dark";
		break;
	case THEME_LIGHT:
	default:
		line += "light";
		break;
	}
	file.AddLine(line);

	line = "labelfont=" + data.labelFont;
	file.AddLine(line);

	line = wxString::Format("labelfontsize=%d", data.labelFontSize);
	file.AddLine(line);

	line = "atpfile=" + data.atpPath.GetFullPath();
	file.AddLine(line);

	line = wxString("elementstoolbar=") + (data.showElementsToolBar ? "yes" : "no");
	file.AddLine(line);

	if (!data.voltageLevels.empty()) {
		line = "voltage_levels=";
		for (size_t i = 0; i < data.voltageLevels.size(); ++i) {
			if (i > 0) line += ";";
			line += wxString::Format("%s:%s", wxString::FromCDouble(data.voltageLevels[i].voltage), data.voltageLevels[i].colour.GetAsString(wxC2S_HTML_SYNTAX));
		}
		file.AddLine(line);
	}

	file.Write();
	file.Close();
	return true;
}
