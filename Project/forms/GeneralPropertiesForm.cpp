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
#include <wx/tokenzr.h>
#include <wx/imaglist.h>
#include <wx/dcmemory.h>
#include <algorithm>
#include "../utils/PropertiesData.h"


static bool ParseVoltageDouble(const wxString& str, double& value)
{
	wxString s = str;
	s.Trim().Trim(false);
	if (s.IsEmpty()) return false;

	// 1. Try standard ToDouble (respects current locale, e.g. "230,00" in pt_BR)
	if (s.ToDouble(&value)) return true;

	// 2. Try converting comma to dot via ToCDouble (e.g. "230.00" or "230,00" -> "230.00")
	wxString sDot = s;
	sDot.Replace(wxT(","), wxT("."));
	if (sDot.ToCDouble(&value)) return true;

	// 3. Try converting dot to comma via ToDouble (e.g. "230.00" -> "230,00" in pt_BR)
	wxString sComma = s;
	sComma.Replace(wxT("."), wxT(","));
	if (sComma.ToDouble(&value)) return true;

	return false;
}

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


	wxFont currentFont(data.labelFontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, data.labelFont);
	m_fontPickerText->SetSelectedFont(currentFont);

	// Busbar (Barramento) tab setup
	m_voltageLevels = data.voltageLevels;
	if (m_voltageLevels.empty()) {
		m_voltageLevels = PropertiesData::GetDefaultVoltageLevels();
	}

	m_panelBusbar = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* busbarMainSizer = new wxBoxSizer(wxHORIZONTAL);
	m_panelBusbar->SetSizer(busbarMainSizer);

	wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
	busbarMainSizer->Add(leftSizer, 1, wxALL | wxEXPAND, 5);

	m_listCtrlVoltages = new wxListCtrl(m_panelBusbar, wxID_ANY, wxDefaultPosition, wxSize(240, 220), wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_HRULES | wxLC_VRULES);
	m_listCtrlVoltages->InsertColumn(0, wxEmptyString, wxLIST_FORMAT_LEFT, 110);
	m_listCtrlVoltages->InsertColumn(1, wxEmptyString, wxLIST_FORMAT_LEFT, 110);
	leftSizer->Add(m_listCtrlVoltages, 1, wxEXPAND, 0);

	wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
	busbarMainSizer->Add(rightSizer, 0, wxALL | wxEXPAND, 5);

	m_staticTextVoltage = new wxStaticText(m_panelBusbar, wxID_ANY, wxEmptyString);
	rightSizer->Add(m_staticTextVoltage, 0, wxLEFT | wxRIGHT | wxTOP, 5);

	m_textCtrlVoltage = new wxTextCtrl(m_panelBusbar, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(140, -1));
	rightSizer->Add(m_textCtrlVoltage, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 5);

	m_staticTextColour = new wxStaticText(m_panelBusbar, wxID_ANY, wxEmptyString);
	rightSizer->Add(m_staticTextColour, 0, wxLEFT | wxRIGHT | wxTOP, 5);

	m_colourPickerBus = new wxColourPickerCtrl(m_panelBusbar, wxID_ANY, *wxBLACK, wxDefaultPosition, wxSize(140, -1));
	rightSizer->Add(m_colourPickerBus, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 5);

	rightSizer->AddSpacer(10);

	m_buttonAddVoltage = new wxButton(m_panelBusbar, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(140, -1));
	rightSizer->Add(m_buttonAddVoltage, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 5);

	m_buttonRemoveVoltage = new wxButton(m_panelBusbar, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(140, -1));
	rightSizer->Add(m_buttonRemoveVoltage, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 5);

	m_buttonDefaultVoltages = new wxButton(m_panelBusbar, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(140, -1));
	rightSizer->Add(m_buttonDefaultVoltages, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 5);

	m_notebook->AddPage(m_panelBusbar, wxEmptyString, false);
	m_busbarPageIndex = m_notebook->GetPageCount() - 1;

	m_choiceLanguage->Bind(wxEVT_CHOICE, &GeneralPropertiesForm::OnLanguageSelected, this);
	m_listCtrlVoltages->Bind(wxEVT_LIST_ITEM_SELECTED, &GeneralPropertiesForm::OnVoltageItemSelected, this);
	m_buttonAddVoltage->Bind(wxEVT_BUTTON, &GeneralPropertiesForm::OnButtonAddVoltage, this);
	m_buttonRemoveVoltage->Bind(wxEVT_BUTTON, &GeneralPropertiesForm::OnButtonRemoveVoltage, this);
	m_buttonDefaultVoltages->Bind(wxEVT_BUTTON, &GeneralPropertiesForm::OnButtonDefaultVoltages, this);

	UpdateLanguageUI();
	PopulateVoltageList();

	if (GetSizer()) {
		GetSizer()->Fit(this);
	}
	SetMinSize(wxSize(480, 380));
	Fit();
	Centre();
}

GeneralPropertiesForm::~GeneralPropertiesForm()
{
	if (m_choiceLanguage) {
		m_choiceLanguage->Unbind(wxEVT_CHOICE, &GeneralPropertiesForm::OnLanguageSelected, this);
	}
}
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
		wxTheApp->SetAppearance(wxApp::Appearance::Light);
	} break;
	case 1: {
		line += "dark";
		data.theme = THEME_DARK;
		wxTheApp->SetAppearance(wxApp::Appearance::Dark);
	} break;
	}
	file.AddLine(line);
	if (data.theme != checkData.theme) needRestart = true;

	line = "labelfont=";
	line += data.labelFont;
	file.AddLine(line);

	line = "labelfontsize=";
	line += wxString::Format("%d", data.labelFontSize);
	file.AddLine(line);


	line = "atpfile=";
	line += data.atpPath.GetFullPath();
	file.AddLine(line);	

	line = "voltage_levels=";
	for (size_t i = 0; i < m_voltageLevels.size(); ++i) {
		if (i > 0) line += ";";
		line += wxString::Format("%s:%s", wxString::FromCDouble(m_voltageLevels[i].voltage), m_voltageLevels[i].colour.GetAsString(wxC2S_HTML_SYNTAX));
	}
	file.AddLine(line);

	file.Write();
	file.Close();

	if (needRestart) {
		wxMessageDialog msgDialog(this, _("The application must be restarted to settings changes be applied."),
			_("Info"), wxOK | wxCENTRE | wxICON_INFORMATION);
		msgDialog.ShowModal();
	}
	data.voltageLevels = m_voltageLevels;
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

bool GeneralPropertiesForm::IsPortuguese() const
{
	if (m_choiceLanguage && m_choiceLanguage->GetSelection() != wxNOT_FOUND) {
		return (m_choiceLanguage->GetSelection() == 1);
	}
	if (m_properties) {
		return (m_properties->GetGeneralPropertiesData().language == wxLANGUAGE_PORTUGUESE_BRAZILIAN);
	}
	return false;
}

void GeneralPropertiesForm::UpdateLanguageUI()
{
	bool isPt = IsPortuguese();

	if (m_notebook && m_panelBusbar) {
		m_notebook->SetPageText(m_busbarPageIndex, isPt ? wxString(L"Barramento") : wxString(L"Busbar"));
	}

	if (m_listCtrlVoltages) {
		wxListItem col0;
		col0.SetMask(wxLIST_MASK_TEXT);
		col0.SetText(isPt ? wxString(L"Tens\u00e3o (kV)") : wxString(L"Voltage (kV)"));
		m_listCtrlVoltages->SetColumn(0, col0);

		wxListItem col1;
		col1.SetMask(wxLIST_MASK_TEXT);
		col1.SetText(isPt ? wxString(L"Cor") : wxString(L"Colour"));
		m_listCtrlVoltages->SetColumn(1, col1);
	}

	if (m_staticTextVoltage) {
		m_staticTextVoltage->SetLabel(isPt ? wxString(L"Tens\u00e3o (kV):") : wxString(L"Voltage (kV):"));
	}
	if (m_staticTextColour) {
		m_staticTextColour->SetLabel(isPt ? wxString(L"Cor:") : wxString(L"Colour:"));
	}
	if (m_buttonAddVoltage) {
		m_buttonAddVoltage->SetLabel(isPt ? wxString(L"Adicionar / Atualizar") : wxString(L"Add / Update"));
	}
	if (m_buttonRemoveVoltage) {
		m_buttonRemoveVoltage->SetLabel(isPt ? wxString(L"Remover") : wxString(L"Remove"));
	}
	if (m_buttonDefaultVoltages) {
		m_buttonDefaultVoltages->SetLabel(isPt ? wxString(L"Padr\u00e3o") : wxString(L"Default"));
	}
}

void GeneralPropertiesForm::OnLanguageSelected(wxCommandEvent& event)
{
	UpdateLanguageUI();
	event.Skip();
}

void GeneralPropertiesForm::PopulateVoltageList()
{
	m_listCtrlVoltages->DeleteAllItems();

	wxImageList* imgList = new wxImageList(16, 16, true);

	for (size_t i = 0; i < m_voltageLevels.size(); ++i) {
		const auto& lvl = m_voltageLevels[i];

		wxBitmap bmp(16, 16);
		wxMemoryDC memDC(bmp);
		memDC.SetPen(*wxTRANSPARENT_PEN);
		memDC.SetBrush(wxBrush(lvl.colour));
		memDC.DrawRectangle(0, 0, 16, 16);

		memDC.SetPen(wxPen(wxColour(120, 120, 120), 1));
		memDC.SetBrush(*wxTRANSPARENT_BRUSH);
		memDC.DrawRectangle(0, 0, 16, 16);
		memDC.SelectObject(wxNullBitmap);

		int imgIdx = imgList->Add(bmp);

		long itemIndex = m_listCtrlVoltages->InsertItem(static_cast<long>(i), wxString::Format(wxT("%.2f"), lvl.voltage));
		m_listCtrlVoltages->SetItem(itemIndex, 1, lvl.colour.GetAsString(wxC2S_HTML_SYNTAX), imgIdx);
	}

	m_listCtrlVoltages->AssignImageList(imgList, wxIMAGE_LIST_SMALL);
}

void GeneralPropertiesForm::OnVoltageItemSelected(wxListEvent& event)
{
	long itemIndex = event.GetIndex();
	if (itemIndex >= 0 && static_cast<size_t>(itemIndex) < m_voltageLevels.size()) {
		const auto& lvl = m_voltageLevels[itemIndex];
		m_textCtrlVoltage->SetValue(wxString::Format(wxT("%.2f"), lvl.voltage));
		m_colourPickerBus->SetColour(lvl.colour);
	}
}

void GeneralPropertiesForm::OnButtonAddVoltage(wxCommandEvent& event)
{
	wxString vStr = m_textCtrlVoltage->GetValue();
	double v = 0.0;
	bool isPt = IsPortuguese();

	if (!ParseVoltageDouble(vStr, v) || v <= 0.0) {
		wxString msg = isPt ? wxString(L"Por favor, insira um valor v\u00e1lido de tens\u00e3o em kV.")
		                    : wxString(L"Please enter a valid voltage value in kV.");
		wxString title = isPt ? wxString(L"Aviso") : wxString(L"Warning");
		wxMessageDialog msgDialog(this, msg, title, wxOK | wxICON_WARNING);
		msgDialog.ShowModal();
		return;
	}

	wxColour col = m_colourPickerBus->GetColour();

	bool found = false;
	for (auto& lvl : m_voltageLevels) {
		if (std::abs(lvl.voltage - v) < 0.001) {
			lvl.colour = col;
			found = true;
			break;
		}
	}

	if (!found) {
		m_voltageLevels.push_back({ v, col });
	}

	std::sort(m_voltageLevels.begin(), m_voltageLevels.end(), [](const VoltageLevelColour& a, const VoltageLevelColour& b) {
		return a.voltage > b.voltage;
	});

	PopulateVoltageList();

	for (size_t i = 0; i < m_voltageLevels.size(); ++i) {
		if (std::abs(m_voltageLevels[i].voltage - v) < 0.001) {
			m_listCtrlVoltages->SetItemState(static_cast<long>(i), wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED);
			m_listCtrlVoltages->EnsureVisible(static_cast<long>(i));
			break;
		}
	}
}

void GeneralPropertiesForm::OnButtonRemoveVoltage(wxCommandEvent& event)
{
	bool isPt = IsPortuguese();
	long itemIndex = m_listCtrlVoltages->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (itemIndex >= 0 && static_cast<size_t>(itemIndex) < m_voltageLevels.size()) {
		m_voltageLevels.erase(m_voltageLevels.begin() + itemIndex);
		PopulateVoltageList();
		m_textCtrlVoltage->Clear();
	} else {
		wxString msg = isPt ? wxString(L"Selecione um n\u00edvel de tens\u00e3o da lista para remover.")
		                    : wxString(L"Select a voltage level from the list to remove.");
		wxString title = isPt ? wxString(L"Aviso") : wxString(L"Warning");
		wxMessageDialog msgDialog(this, msg, title, wxOK | wxICON_WARNING);
		msgDialog.ShowModal();
	}
}

void GeneralPropertiesForm::OnButtonDefaultVoltages(wxCommandEvent& event)
{
	m_voltageLevels = PropertiesData::GetDefaultVoltageLevels();
	PopulateVoltageList();
	m_textCtrlVoltage->Clear();
}

