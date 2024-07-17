#ifndef ATPFILEEDITORFORM_H
#define ATPFILEEDITORFORM_H
#include "ElementFormBase.h"

class ATPFileEditorForm : public ATPFileEditorFormBase
{
public:
    ATPFileEditorForm(wxWindow* parent, wxString path);
    virtual ~ATPFileEditorForm();
protected:
    virtual void OnCancelClick(wxCommandEvent& event);
    virtual void OnOKClick(wxCommandEvent& event);

    wxString m_path;
};
#endif // ATPFILEEDITORFORM_H
