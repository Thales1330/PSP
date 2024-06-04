#ifndef PROJECTPROPERTIESFORM_H
#define PROJECTPROPERTIESFORM_H
#include "PropertiesFormBase.h"

class Workspace;

class ProjectPropertiesForm : public ProjectPropertiesFormBase
{
public:
    ProjectPropertiesForm(wxWindow* parent, Workspace* workspace);
    virtual ~ProjectPropertiesForm() { }

protected:
    virtual void OnAutomaticLabelClick(wxCommandEvent& event);
    virtual void OnCancelClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnOKClick(wxCommandEvent& event) { if (ValidateData()) EndModal(wxID_OK); }

    bool ValidateData();
    void EnableLabelLimits(const bool& enable);

    Workspace* m_workspace = nullptr;
};
#endif // PROJECTPROPERTIESFORM_H
