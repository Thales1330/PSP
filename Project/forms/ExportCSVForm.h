#ifndef EXPORTCSVFORM_H
#define EXPORTCSVFORM_H
#include "DataReportBase.h"

class ExportCSVForm : public ExportCSVFormBase
{
public:
    ExportCSVForm(wxWindow* parent, const std::vector<wxGrid*>& gridList);
    virtual ~ExportCSVForm();
protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event);
    virtual void OnExportButtonClick(wxCommandEvent& event);
	virtual bool ExportGridToCSV(wxGrid* grid, const wxString& filePath);

    std::vector<wxGrid*> m_gridList;
	std::vector<wxCheckBox*> m_checkBoxList;
};
#endif // EXPORTCSVFORM_H
