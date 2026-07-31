#ifndef EMTELEMENTFORM_H
#define EMTELEMENTFORM_H
#include "ElementFormBase.h"

class EMTElement;
class PropertiesData;
class Workspace;

class EMTElementForm : public EMTElementFormBase
{
public:
    EMTElementForm(wxWindow* parent, EMTElement* emtElement, Workspace* workspace);
    virtual ~EMTElementForm();
    virtual bool ValidateData();
protected:
    virtual void OnATPFileChange(wxFileDirPickerEvent& event);
    virtual void OnTestClick(wxCommandEvent& event);
    virtual void OnEditATPFileClick(wxCommandEvent& event);
    virtual void OnCancelClick(wxCommandEvent& event);
    virtual void OnOKClick(wxCommandEvent& event);

    virtual void LoadNodes(wxString atpFilePath);

    wxWindow* m_parent;
    EMTElement* m_emtElement = nullptr;
    Workspace* m_workspace = nullptr;
    PropertiesData* m_properties = nullptr;
};
#endif // EMTELEMENTFORM_H
