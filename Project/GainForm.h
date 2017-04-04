#ifndef GAINFORM_H
#define GAINFORM_H
#include "ElementForm.h"

class Gain;

class GainForm : public GainFormBase
{
   public:
    GainForm(wxWindow* parent, Gain* gain);
    virtual ~GainForm();
    
    virtual bool ValidateData();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnOKButtonClick(wxCommandEvent& event);

    wxWindow* m_parent;
    Gain* m_gain;
};
#endif  // GAINFORM_H
