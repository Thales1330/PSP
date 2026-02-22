#include "ExportCSVForm.h"
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/ffile.h>

ExportCSVForm::ExportCSVForm(wxWindow* parent, const std::vector<wxGrid*>& gridList)
	: ExportCSVFormBase(parent), m_gridList(gridList)
{
	m_checkBoxList = { m_checkBoxPFPowerFlow, m_checkBoxPFBuses, m_checkBoxPFBranches, m_checkBoxCCFault, m_checkBoxCCBuses,
						m_checkBoxCCBranches, m_checkBoxCCGenerators, m_checkBoxHarmSources, m_checkBoxHarmVoltages,
						m_checkBoxharmCurrents };
}

ExportCSVForm::~ExportCSVForm()
{
}

void ExportCSVForm::OnCancelButtonClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void ExportCSVForm::OnExportButtonClick(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this,
		_("Export CSV"),
		"",
		_("results.csv"),
		_("CSV files (*.csv)|*.csv"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	wxString fileAddName[10] = { _("_PowerFlow"), _("_PFBuses"), _("_PFBranches"), _("_Fault"), _("_FaultBuses"), _("_FaultBranches"),
									_("_FaultGenerators"), _("_HarmSources"), _("_HarmVoltages"), _("_HarmCurrents") };
	std::vector<wxString> filesSaved;
	int i = 0;
	for (auto& checkBox : m_checkBoxList)
	{
		if (checkBox->GetValue())
		{
			wxString path = saveFileDialog.GetPath();
			path.insert(path.Length() - 4, fileAddName[i]);
			ExportGridToCSV(m_gridList[i], path);

			wxFileName fn(path);
			filesSaved.push_back(fn.GetFullName());
		}
		i++;
	}

	wxString message = _("The following files have been successfully exported:\n");
	for (const auto& file : filesSaved)
		message += file + "\n";

	wxMessageDialog msgDialog(this, message, _("Information"), wxOK | wxCENTRE | wxICON_INFORMATION);
	msgDialog.ShowModal();

	EndModal(wxID_OK);
}

bool ExportCSVForm::ExportGridToCSV(wxGrid* grid, const wxString& filePath)
{
	if (!grid)
		return false;

	wxFFile file(filePath, "wb");
	if (!file.IsOpened())
		return false;

	// Write UTF-8 BOM so Excel detects encoding properly
	file.Write("\xEF\xBB\xBF", 3);

	int rows = grid->GetNumberRows();
	int cols = grid->GetNumberCols();

	for (int row = 0; row < rows; ++row)
	{
		wxString line;
		for (int col = 0; col < cols; )
		{

			int rowspan = 1;
			int colspan = 1;

			grid->GetCellSize(row, col, &rowspan, &colspan);


			// Cell covered by vertical merge
			if (rowspan <= 0 || colspan <= 0)
			{
				line << wxT("\u200B");
				++col;
				continue;
			}
			wxString value = grid->GetCellValue(row, col);

			// Escape quotes for CSV
			value.Replace("\"", "\"\"");

			// Wrap with quotes if necessary
			if (value.Contains(";") || value.Contains("\"") || value.Contains("\n"))
			{
				value = "\"" + value + "\"";
			}

			if (!line.IsEmpty())
				line << ";";

			line << value;

			// Horizontal merge
			for (int c = 1; c < colspan; ++c)
			{
				line << ";";
			}

			col += colspan;
		}

		line << "\r\n";

		// Convert once to UTF-8 and write raw bytes
		wxScopedCharBuffer buffer = line.utf8_str();
		file.Write(buffer.data(), buffer.length());
	}

	file.Close();
	return true;
}
