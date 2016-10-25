#ifndef LOADFORM_H
#define LOADFORM_H
#include "ElementForm.h"

class Load;
class SwitchingForm;

class LoadForm : public LoadFormBase
{
   public:
    LoadForm(wxWindow* parent, Load* load);
    virtual ~LoadForm();

    virtual bool ValidateData();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); };
    virtual void OnOnButtonClick(wxCommandEvent& event);
    virtual void OnStabilityButtonClick(wxCommandEvent& event);

    wxWindow* m_parent;
    Load* m_load;
};
#endif  // LOADFORM_H
