#ifndef LABELMANAGER_H
#define LABELMANAGER_H
#include "PropertiesFormBase.h"

class Workspace;

class LabelManager : public LabelManagerBase
{
public:
    LabelManager(wxWindow* parent, Workspace* workspace);
    virtual ~LabelManager();
protected:
    virtual void OnApplyButtonClick(wxCommandEvent& event);
    virtual void OnCancelButtonClick(wxCommandEvent& event);
    virtual void OnPrecisionTextUpdate(wxCommandEvent& event);
    virtual void OnUnitChanged(wxCommandEvent& event);
    virtual void OnChoicebookChanged(wxChoicebookEvent& event);
    virtual void OnCheckListSelected(wxCommandEvent& event);

	void UpdatePreview(int selection);

	wxString GetBusPreviewText(int dataID);
	wxString GetGeneratorPreviewText(int dataID);
	wxString GetLoadPreviewText(int dataID);
	wxString GetLinePreviewText(int dataID);
	wxString GetTransformerPreviewText(int dataID);
	wxString GetCapacitorPreviewText(int dataID);
	wxString GetInductorPreviewText(int dataID);
	wxString GetIndMotorPreviewText(int dataID);
	wxString GetSyncMotorPreviewText(int dataID);

    wxString GetPowerText(wxString unit, double puValue);
    wxString GetCurrent3fText(const std::array<double, 3>& puCurrents);
	wxString GetCurrentOrVoltageText(wxString unit, double puValue);

	void SetPreviewText(wxCheckListBox* checkListBox);

	Workspace* m_workspace;
    size_t m_precision = 2;
};
#endif // LABELMANAGER_H
