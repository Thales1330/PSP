#ifndef IMPORTFORM_H
#define IMPORTFORM_H

#include "base/PropertiesFormBase.h"

#include <wx/msgdlg.h>

class Workspace;

class ImportForm : public ImportFormBase
{
   public:
    ImportForm(wxWindow* parent, Workspace* workspace);
    virtual ~ImportForm();

    Workspace* GetWorkspace() { return m_workspace; }

   protected:
    virtual void OnButtonCancelClick(wxCommandEvent& event);
    virtual void OnButtonOKClick(wxCommandEvent& event);
    bool ImportSelectedFiles();

    Workspace* m_workspace = NULL;
    wxWindow* m_parent;
};
#endif  // IMPORTFORM_H
