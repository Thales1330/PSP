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

#include "AboutForm.h"

AboutForm::AboutForm(wxWindow* parent) : AboutFormBase(parent) { Init(); }
AboutForm::~AboutForm() {}
void AboutForm::Init()
{
    // Create developers table
    m_gridCredits->EnableGridLines(false);
    wxFont headerFont = m_gridCredits->GetDefaultCellFont();
    headerFont.SetWeight(wxFONTWEIGHT_BOLD);
    headerFont.SetPointSize(headerFont.GetPointSize() + 1);
    wxColour headerColour(200, 200, 200);
    wxColour hyperlinkColour(6, 69, 173);

    m_gridCredits->AppendCols(3);
    m_gridCredits->AppendRows(6);
    m_gridCredits->HideColLabels();
    m_gridCredits->HideRowLabels();
    m_gridCredits->SetCellSize(0, 0, 1, 3);
    m_gridCredits->SetCellSize(3, 0, 1, 3);

    m_gridCredits->SetCellValue(0, 0, _("Developers"));
    m_gridCredits->SetCellAlignment(0, 0, wxALIGN_CENTRE, wxALIGN_CENTRE);
    m_gridCredits->SetCellBackgroundColour(0, 0, headerColour);
    m_gridCredits->SetCellFont(0, 0, headerFont);
    m_gridCredits->SetCellValue(1, 0, wxT("Thales Lima Oliveira"));
    m_gridCredits->SetCellValue(1, 1, _("Main developer and project admin"));
    m_gridCredits->SetCellValue(1, 2, wxT("thales@ufu.br"));

    m_gridCredits->SetRowMinimalHeight(2, 30);

    m_gridCredits->SetCellValue(3, 0, _("Contributors / Special Thanks"));
    m_gridCredits->SetCellAlignment(3, 0, wxALIGN_CENTRE, wxALIGN_CENTRE);
    m_gridCredits->SetCellBackgroundColour(3, 0, headerColour);
    m_gridCredits->SetCellFont(3, 0, headerFont);
    // Caixeta
    m_gridCredits->SetCellValue(4, 0, wxT("Geraldo Caixeta Guimar") + static_cast<wxString>(L'\u00E3') + wxT("es"));
    m_gridCredits->SetCellValue(4, 1, _("Chief advisor"));
    m_gridCredits->SetCellValue(4, 2, wxT("gcaixeta@ufu.br"));
    // Marcio Tamashiro
    m_gridCredits->SetCellValue(5, 0, wxT("M") + static_cast<wxString>(L'\u00E1') + wxT("rcio Augusto Tamashiro"));
    m_gridCredits->SetCellValue(5, 1, "");
    m_gridCredits->SetCellValue(5, 2, wxT("tamashiro@ifto.edu.br"));

    for(int i = 0; i < m_gridCredits->GetNumberRows(); ++i) {
        m_gridCredits->SetCellTextColour(i, 2, hyperlinkColour);
    }

    m_gridCredits->AutoSize();

    // Last col size
    int lastColSize = m_notebook->GetPage(1)->GetSize().GetWidth();
    int lastColNumber = m_gridCredits->GetNumberCols() - 1;
    for(int i = 0; i < lastColNumber; ++i) {
        lastColSize -= m_gridCredits->GetColSize(i);
    }
    m_gridCredits->SetColSize(lastColNumber, lastColSize);
    m_gridCredits->SetSize(m_notebook->GetPage(1)->GetSize());

    // Load license file
    wxString licenseStr = "";
    wxTextFile file;
    wxFileName fn(wxStandardPaths::Get().GetExecutablePath());
    wxString licensePath = fn.GetPath() + "\\..\\data\\LICENSE";
    if(!file.Open(licensePath)) {
        // Error message
    } else {
        licenseStr += file.GetFirstLine() + "\n";
        while(!file.Eof()) {
            licenseStr += file.GetNextLine() + "\n";
        }
    }
    wxFont font = m_richTextCtrlLicense->GetFont();
    font.SetFamily(wxFONTFAMILY_TELETYPE);
    m_richTextCtrlLicense->SetFont(font);
    m_richTextCtrlLicense->SetEditable(false);
    m_richTextCtrlLicense->AppendText(licenseStr);
}
