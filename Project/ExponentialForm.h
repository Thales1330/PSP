#ifndef EXPONENTIALFORM_H
#define EXPONENTIALFORM_H
#include "ElementForm.h"

class Exponential;

class ExponentialForm : public ExponentialFormBase
{
   public:
    ExponentialForm(wxWindow* parent, Exponential* exponential);
    virtual ~ExponentialForm();
    virtual bool ValidateData();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnOKButtonClick(wxCommandEvent& event);

    wxWindow* m_parent = NULL;
    Exponential* m_exponential = NULL;
};
#endif  // EXPONENTIALFORM_H
