#ifndef LIMITERFORM_H
#define LIMITERFORM_H
#include "ElementForm.h"

class Limiter;

class LimiterForm : public LimiterFormBase
{
   public:
    LimiterForm(wxWindow* parent, Limiter* limiter);
    virtual ~LimiterForm();
    virtual bool ValidateData();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnOKButtonClick(wxCommandEvent& event);

    wxWindow* m_parent = NULL;
    Limiter* m_limiter = NULL;
};
#endif  // LIMITERFORM_H
