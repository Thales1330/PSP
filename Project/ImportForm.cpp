#include "ImportForm.h"
#include "Workspace.h"

ImportForm::ImportForm(wxWindow* parent, Workspace* workspace) : ImportFormBase(parent)
{
    SetInitialSize();

    m_parent = parent;
    m_workspace = workspace;
}

ImportForm::~ImportForm() {}

void ImportForm::OnButtonCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
    if(m_workspace) delete m_workspace;
}

void ImportForm::OnButtonOKClick(wxCommandEvent& event)
{
    if(ImportSelectedFiles())
        EndModal(wxID_OK);
    else {
        // Error message
        wxMessageDialog msgDialog(this, _("It was not possible to import the selected files."), _("Error"),
                                  wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
    }
}

bool ImportForm::ImportSelectedFiles() { return true; }
