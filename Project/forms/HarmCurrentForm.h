#ifndef HARMCURRENTFORM_H
#define HARMCURRENTFORM_H
#include "ElementFormBase.h"

class HarmCurrent;

class HarmCurrentForm : public HarmCurrentFormBase
{
   public:
    HarmCurrentForm(wxWindow* parent, HarmCurrent* harmCurrent);
    virtual ~HarmCurrentForm();
    virtual bool ValidateData();
    virtual void SortList();

   protected:
    virtual void OnAddButtonClick(wxCommandEvent& event);
    virtual void OnRemoveButtonClick(wxCommandEvent& event);
    virtual void OnCancelButtonClick(wxCommandEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);

    wxWindow* m_parent;
    HarmCurrent* m_harmCurrent = nullptr;
    int m_maxID = 0;
};
#endif  // HARMCURRENTFORM_H
